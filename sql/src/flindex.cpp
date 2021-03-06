//------------------------------------------------------------------------------
// Desc:	Contains FLAIM API routines that aid the user in managing indexes.
// Tabs:	3
//
// Copyright (c) 2000-2007 Novell, Inc. All Rights Reserved.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; version 2.1
// of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, contact Novell, Inc.
//
// To contact Novell about this file by physical or electronic mail, 
// you may find current contact information at www.novell.com.
//
// $Id$
//------------------------------------------------------------------------------

#include "flaimsys.h"

FSTATIC RCODE SQFAPI flmBackgroundIndexBuildThrd(
	IF_Thread *		pThread);
	
/****************************************************************************
Desc : Return the status of the index.
Notes:
****************************************************************************/
RCODE F_Db::indexStatus(
	FLMUINT					uiIndexNum,
	SFLM_INDEX_STATUS *	pIndexStatus)
{
	RCODE				rc = NE_SFLM_OK;
	FLMBOOL			bStartedTrans = FALSE;
	F_BKGND_IX *	pBackgroundIx;
	FLMBOOL			bMutexLocked = FALSE;

	flmAssert( pIndexStatus);

	if (RC_BAD( rc = checkState( __FILE__, __LINE__)))
	{
		goto Exit;
	}

	if( m_eTransType != SFLM_NO_TRANS)
	{
		if( !okToCommitTrans())
		{
			rc = RC_SET( NE_SFLM_ABORT_TRANS);
			goto Exit;
		}
	}
	else
	{
		// Need to have at least a read transaction going.

		if( RC_BAD( rc = beginTrans( SFLM_READ_TRANS)))
		{
			goto Exit;
		}
		bStartedTrans = TRUE;
	}

	f_mutexLock( gv_SFlmSysData.hShareMutex);
	bMutexLocked = TRUE;

	pBackgroundIx = flmBackgroundIndexGet( m_pDatabase, uiIndexNum, TRUE);
	if (pBackgroundIx)
	{
		f_memcpy( pIndexStatus, &pBackgroundIx->indexStatus,
			sizeof( SFLM_INDEX_STATUS));
		f_mutexUnlock( gv_SFlmSysData.hShareMutex);
		bMutexLocked = FALSE;
		flmAssert( pIndexStatus->uiIndexNum == uiIndexNum);
	}
	else
	{
		F_INDEX *	pIndex;

		f_mutexUnlock( gv_SFlmSysData.hShareMutex);
		bMutexLocked = FALSE;

		pIndex = m_pDict->getIndex( uiIndexNum);

		// Populate the index status structure.

		f_memset( pIndexStatus, 0, sizeof( SFLM_INDEX_STATUS));
		pIndexStatus->uiIndexNum = uiIndexNum;

		if( !(pIndex->uiFlags & (IXD_SUSPENDED | IXD_OFFLINE)))
		{
			pIndexStatus->ui64LastRowIndexed = ~((FLMUINT64)0);
			pIndexStatus->eState = SFLM_INDEX_ONLINE;
		}
		else
		{
			// NOTE: If we are in a read transaction, the last node indexed
			// value may not be read-consistent.  It is only guaranteed to
			// be correct for update transactions.

			pIndexStatus->ui64LastRowIndexed = pIndex->ui64LastRowIndexed;
			pIndexStatus->eState = (pIndex->uiFlags & IXD_SUSPENDED)
												? SFLM_INDEX_SUSPENDED
												: SFLM_INDEX_BRINGING_ONLINE;
		}
	}

Exit:

	if( bMutexLocked)
	{
		f_mutexUnlock( gv_SFlmSysData.hShareMutex);
	}

	if( bStartedTrans)
	{
		abortTrans();
	}

	return( rc);
}

/****************************************************************************
Desc : Suspend the selected index from doing any key updates on records
		 that are equal or higher than the next record ID value
		 in the container that the index references.  If the index is offline
		 then the background process will be suspended.  If the index is
		 online then it will be suspended.  If the index is already
		 suspended NE_SFLM_OK will be returned.  A suspended index is not
		 persistent if the database goes down.
Notes: An update transaction will be started if necessary.
****************************************************************************/
RCODE F_Db::indexSuspend(
	FLMUINT		uiIndexNum)
{
	RCODE				rc = NE_SFLM_OK;
	F_INDEX *		pIndex;
	FLMUINT64		ui64HighestRowId;
	FLMBOOL			bStartedTrans = FALSE;
	F_TABLE *		pTable;
	FLMBOOL			bMustAbortOnError = FALSE;
	F_Rfl *			pRfl = m_pDatabase->m_pRfl;
	FLMUINT			uiRflToken = 0;

	if (RC_BAD( rc = checkState( __FILE__, __LINE__)))
	{
		goto Exit;
	}

	if (m_eTransType != SFLM_NO_TRANS)
	{
		if (!okToCommitTrans())
		{
			rc = RC_SET( NE_SFLM_ABORT_TRANS);
			goto Exit;
		}
		else if (m_eTransType == SFLM_READ_TRANS)
		{
			rc = RC_SET_AND_ASSERT( NE_SFLM_ILLEGAL_TRANS_OP);
			goto Exit;
		}
	}
	else
	{
		// Need to have an update transaction going.

		if (RC_BAD( rc = beginTrans( SFLM_UPDATE_TRANS)))
		{
			goto Exit;
		}
		bStartedTrans = TRUE;
	}

	// See if the index is valid

	pIndex = m_pDict->getIndex( uiIndexNum);
	
	if (pIndex->uiFlags & IXD_SUSPENDED)
	{
		// Index is already suspended.

		goto Exit;
	}

	// If the index is not currently offline, the highest row indexed
	// is the tables's last row ID.  Otherwise, it is
	// simply the value that is stored in the F_INDEX.

	if (!(pIndex->uiFlags & IXD_OFFLINE))
	{
		pTable = m_pDict->getTable( pIndex->uiTableNum);
		ui64HighestRowId = pTable->lfInfo.ui64NextRowId - 1;
	}
	else
	{
		ui64HighestRowId = pIndex->ui64LastRowIndexed;
	}

	// Disable RFL logging

	pRfl->disableLogging( &uiRflToken);

	// Must abort on error

	bMustAbortOnError = TRUE;

	// There may be a background thread still assigned to the
	// index even though the index may be "on-line."  This is because
	// the background thread may have just commited a transaction that
	// transitioned the index from off-line to on-line, but the thread
	// has not yet exited (even though it will not do any more work
	// to update the index).  We want to wait for the thread to terminate
	// before our transaction is allowed to commit.  This is so that if
	// we immediately call resume, it won't find the yet-to-terminate
	// thread still running in the background.

	if (!(m_uiFlags & FDB_REPLAYING_RFL))
	{
		if( RC_BAD( rc = addToStopList( uiIndexNum)))
		{
			goto Exit;
		}
	}

	if (RC_BAD( rc = setIxStateInfo( uiIndexNum, ui64HighestRowId,
								IXD_SUSPENDED)))
	{
		goto Exit;
	}

	// setIxStateInfo may have changed to a new dictionary, so pIndex is no
	// good after this point

	pIndex = NULL;

	// Log the suspend packet to the RFL

	pRfl->enableLogging( &uiRflToken);

	if (RC_BAD( rc = m_pDatabase->m_pRfl->logIndexSuspendOrResume(
								this, uiIndexNum, RFL_INDEX_SUSPEND_PACKET)))
	{
		goto Exit;
	}

Exit:

	if( uiRflToken)
	{
		pRfl->enableLogging( &uiRflToken);
	}

	if( RC_BAD( rc))
	{
		if( bStartedTrans)
		{
			abortTrans();
		}
		else if( bMustAbortOnError)
		{
			setMustAbortTrans( rc);
		}
	}
	else if( bStartedTrans)
	{
		rc = commitTrans( 0, FALSE);
	}

	return( rc);
}

/****************************************************************************
Desc : If the index was suspended, restart the background process that
		 will get the index up to date so that it will eventually be online.
		 Returns NE_SFLM_OK with no change if the index is already online.
Notes: An update transaction will be started if necessary.
****************************************************************************/
RCODE F_Db::indexResume(
	FLMUINT		uiIndexNum)
{
	RCODE			rc = NE_SFLM_OK;
	F_INDEX *	pIndex;
	FLMBOOL		bStartedTrans = FALSE;
	FLMBOOL		bMustAbortOnError = FALSE;
	FLMUINT		uiRflToken = 0;
	F_Rfl *		pRfl = m_pDatabase->m_pRfl;

	if (RC_BAD( rc = checkState( __FILE__, __LINE__)))
	{
		goto Exit;
	}

	if (m_eTransType != SFLM_NO_TRANS)
	{
		if (!okToCommitTrans())
		{
			rc = RC_SET( NE_SFLM_ABORT_TRANS);
			goto Exit;
		}
		else if (m_eTransType == SFLM_READ_TRANS)
		{
			rc = RC_SET_AND_ASSERT( NE_SFLM_ILLEGAL_TRANS_OP);
			goto Exit;
		}
	}
	else
	{
		// Need to have an update transaction going.

		if (RC_BAD( rc = beginTrans( SFLM_UPDATE_TRANS)))
		{
			goto Exit;
		}
		bStartedTrans = TRUE;
	}

	// See if the index is valid

	pIndex = m_pDict->getIndex( uiIndexNum);

	if (!(pIndex->uiFlags & (IXD_SUSPENDED | IXD_OFFLINE)))
	{
		// Index is already on-line

		goto Exit;
	}

	if (!(pIndex->uiFlags & IXD_SUSPENDED))
	{
		// Index is not suspended.  It is offline (see test
		// above), but a thread should already be building the
		// index.

		flmAssert( flmBackgroundIndexGet( m_pDatabase,
							uiIndexNum, FALSE) != NULL);

		goto Exit;
	}

	// Disable RFL logging

	pRfl->disableLogging( &uiRflToken);

	// Point of no return -- must abort on error

	bMustAbortOnError = TRUE;

	// Better not have a background thread running

	flmAssert( flmBackgroundIndexGet( m_pDatabase, uiIndexNum, FALSE) == NULL);

	// Update the index state info to show "offline"

	if (RC_BAD( rc = setIxStateInfo( uiIndexNum, pIndex->ui64LastRowIndexed,
									IXD_OFFLINE)))
	{
		goto Exit;
	}

	// setIxStateInfo may have changed to a new dictionary, so pIndex is no
	// good after this point

	pIndex = NULL;

	// Add an entry to the start list so that an indexing thread
	// will be started when this transaction commits.

	if (!(m_uiFlags & FDB_REPLAYING_RFL))
	{
		if (RC_BAD( rc = addToStartList( uiIndexNum)))
		{
			goto Exit;
		}
	}

	// Log the resume packet to the RFL

	pRfl->enableLogging( &uiRflToken);

	if (RC_BAD( rc = pRfl->logIndexSuspendOrResume(
								this, uiIndexNum, RFL_INDEX_RESUME_PACKET)))
	{
		goto Exit;
	}

Exit:

	if( uiRflToken)
	{
		pRfl->enableLogging( &uiRflToken);
	}

	if( RC_BAD( rc))
	{
		if( bStartedTrans)
		{
			abortTrans();
		}
		else if( bMustAbortOnError)
		{
			setMustAbortTrans( rc);
		}
	}
	else if( bStartedTrans)
	{
		rc = commitTrans( 0, FALSE);
	}

	return( rc);
}

/****************************************************************************
Desc:		Add the index to the stop list of background threads.
****************************************************************************/
RCODE	F_Db::addToStopList(
	FLMUINT	uiIndexNum)
{
	RCODE					rc = NE_SFLM_OK;
	F_BKGND_IX *		pBackgroundIx;
	F_BKGND_IX *		pNextBackgroundIx;

	// We'd better not be replaying the RFL

	flmAssert( !(m_uiFlags & FDB_REPLAYING_RFL));

	// First look in the start list and remove any index matches.
	// This is need if you add an index and drop
	// it within the same transaction.

	for( pBackgroundIx = m_pIxStartList;
			pBackgroundIx; pBackgroundIx = pNextBackgroundIx)
	{
		pNextBackgroundIx = pBackgroundIx->pNext;

		if (pBackgroundIx->indexStatus.uiIndexNum == uiIndexNum)
		{
			if (pNextBackgroundIx)
			{
				pNextBackgroundIx->pPrev = pBackgroundIx->pPrev;
			}

			if (pBackgroundIx->pPrev)
			{
				pBackgroundIx->pPrev->pNext = pNextBackgroundIx;
			}
			else
			{
				m_pIxStartList = pNextBackgroundIx;
			}

			f_free( &pBackgroundIx);
		}
	}

	// See if we already have an entry in the stop list for the index.  There
	// is no reason to have the index in the list more than once.

	for (pBackgroundIx = m_pIxStopList;
			pBackgroundIx; pBackgroundIx = pNextBackgroundIx)
	{
		pNextBackgroundIx = pBackgroundIx->pNext;

		if (pBackgroundIx->indexStatus.uiIndexNum == uiIndexNum)
		{
			goto Exit;  // Should return NE_SFLM_OK
		}
	}

	// Allocate and add the thread structure to the thread list.

	if (RC_BAD( rc = f_calloc( (FLMUINT)( sizeof( F_BKGND_IX)),
		&pBackgroundIx)))
	{
		goto Exit;
	}

	pBackgroundIx->indexStatus.uiIndexNum  = uiIndexNum;
	pBackgroundIx->pPrev = NULL;
	if ((pBackgroundIx->pNext = m_pIxStopList) != NULL)
	{
		m_pIxStopList->pPrev = pBackgroundIx;
	}
	m_pIxStopList = pBackgroundIx;

Exit:

	return( rc);
}

/****************************************************************************
Desc:		Add the index to the start list of background threads.
****************************************************************************/
RCODE	F_Db::addToStartList(
	FLMUINT	uiIndexNum)
{
	RCODE				rc = NE_SFLM_OK;
	F_BKGND_IX *	pBackgroundIx;
	F_BKGND_IX *	pNextBackgroundIx;

	// We'd better not be replaying the RFL

	flmAssert( !(m_uiFlags & FDB_REPLAYING_RFL));

	// Look in the start list to make sure we don't already
	// have an entry for this index.  We don't want to
	// start more than one thread per index.  The background
	// indexing code is not structured to handle multiple build
	// threads on the same index.

	// NOTE: We don't want to remove any entries in the stop
	// list corresponding to this index.  The reason for this
	// is the index may have been deleted, re-added, deleted,
	// modified, etc. several times during the transaction.
	// We want to make sure that an existing background indexing
	// thread is terminated and a new one is started.  The stop
	// list is always processed first at transaction commit time.
	// Then new indexing threads (in the start list) are started.

	for( pBackgroundIx = m_pIxStartList;
			pBackgroundIx; pBackgroundIx = pNextBackgroundIx)
	{
		pNextBackgroundIx = pBackgroundIx->pNext;

		if (pBackgroundIx->indexStatus.uiIndexNum == uiIndexNum)
		{
			goto Exit; // Should return NE_SFLM_OK
		}
	}

	// Allocate and add the thread structure to the pDb thread list.

	if (RC_BAD( rc = f_calloc( (FLMUINT)( sizeof( F_BKGND_IX)),
		&pBackgroundIx)))
	{
		goto Exit;
	}

	pBackgroundIx->indexStatus.uiIndexNum = uiIndexNum;
	pBackgroundIx->pPrev = NULL;
	if ((pBackgroundIx->pNext = m_pIxStartList) != NULL)
	{
		m_pIxStartList->pPrev = pBackgroundIx;
	}
	m_pIxStartList = pBackgroundIx;

Exit:

	return( rc);
}

/****************************************************************************
Desc:		After Abort and before we unlock, stop and start all indexing.
****************************************************************************/
void F_Db::indexingAfterAbort( void)
{
	F_BKGND_IX *	pStartIx;
	F_BKGND_IX *	pStopIx;
	F_BKGND_IX *	pNextIx;

	pStopIx = m_pIxStopList;
	m_pIxStopList = NULL;
	for( ; pStopIx; pStopIx = pNextIx)
	{
		pNextIx = pStopIx->pNext;
		f_free( &pStopIx);
	}

	pStartIx = m_pIxStartList;
	m_pIxStartList = NULL;
	for( ; pStartIx; pStartIx = pNextIx)
	{
		pNextIx = pStartIx->pNext;
		f_free( &pStartIx);
	}
}

/****************************************************************************
Desc:		Stops a background indexing thread
Notes:	This routine DOES NOT assume that the global mutex is locked.  It
			will lock and unlock the mutex as needed.
****************************************************************************/
void F_Db::stopBackgroundIndexThread(
	FLMUINT		uiIndexNum,
	FLMBOOL		bWait,
	FLMBOOL *	pbStopped)
{
	F_BKGND_IX *	pBackgroundIx;
	FLMUINT			uiThreadId;
	FLMBOOL			bMutexLocked = FALSE;

	if (pbStopped)
	{
		*pbStopped = FALSE;
	}

	for (;;)
	{
		// Lock the global mutex

		if (!bMutexLocked)
		{
			f_mutexLock( gv_SFlmSysData.hShareMutex);
			bMutexLocked = TRUE;
		}

		// Get the background index

		if ((pBackgroundIx = flmBackgroundIndexGet( m_pDatabase,
										uiIndexNum, TRUE, &uiThreadId)) == NULL)
		{
			if (pbStopped)
			{
				*pbStopped = TRUE;
			}
			goto Exit;
		}

		// Set the thread's shutdown flag first.

		gv_SFlmSysData.pThreadMgr->setThreadShutdownFlag( uiThreadId);

		// Unlock the global mutex

		f_mutexUnlock( gv_SFlmSysData.hShareMutex);
		bMutexLocked = FALSE;

		// The thread may be waiting to start a transaction.

		m_pDatabase->m_pDatabaseLockObj->timeoutLockWaiter( uiThreadId);
		m_pDatabase->m_pWriteLockObj->timeoutLockWaiter( uiThreadId);

		if( !bWait)
		{
			break;
		}

		// Wait for the thread to terminate

		f_sleep( 50);
	}

Exit:

	if (bMutexLocked)
	{
		f_mutexUnlock( gv_SFlmSysData.hShareMutex);
	}
}

/****************************************************************************
Desc:		After commit and before we unlock, stop and start all indexing.
****************************************************************************/
void F_Db::indexingAfterCommit( void)
{
	F_BKGND_IX *	pStartIx;
	F_BKGND_IX *	pStopIx;
	F_BKGND_IX *	pNextIx;
	FLMBOOL			bThreadsActive;
	FLMBOOL			bStopped;

	// Signal all background indexing threads in the stop list
	// to shutdown.  Poll until all have terminated.

	for( ;;)
	{
		bThreadsActive = FALSE;
		for( pStopIx = m_pIxStopList; pStopIx; pStopIx = pStopIx->pNext)
		{
			stopBackgroundIndexThread( pStopIx->indexStatus.uiIndexNum,
												FALSE, &bStopped);
			if( !bStopped)
			{
				bThreadsActive = TRUE;
			}
		}

		if( !bThreadsActive)
		{
			break;
		}

		f_sleep( 50);
	}

	// Now that all of the threads have been stopped, discard the stop list

	pStopIx = m_pIxStopList;
	m_pIxStopList = NULL;
	for (; pStopIx; pStopIx = pNextIx)
	{
		pNextIx = pStopIx->pNext;
		f_free( &pStopIx);
	}

	// Start threads listed in the index start list.

	pStartIx = m_pIxStartList;
	m_pIxStartList = NULL;
	for (; pStartIx; pStartIx = pNextIx)
	{
		pNextIx = pStartIx->pNext;
		(void)startIndexBuild( pStartIx->indexStatus.uiIndexNum);
		f_free( &pStartIx);
	}
}

/****************************************************************************
Desc:	Thread that will build an index in the background.
****************************************************************************/
RCODE F_Db::startIndexBuild(
	FLMUINT	uiIndexNum)
{
	RCODE				rc = NE_SFLM_OK;
	FLMUINT			uiGMT;
	F_INDEX *		pIndex;
	F_BKGND_IX *	pBackgroundIx = NULL;
	char				szThreadName[ F_PATH_MAX_SIZE];
	char				szBaseName[ F_FILENAME_SIZE];

	f_timeGetSeconds( &uiGMT );

	if (flmBackgroundIndexGet( m_pDatabase, uiIndexNum, FALSE) != NULL)
	{
		// There is already a background thread running on this index.

		rc = RC_SET_AND_ASSERT( NE_SFLM_FAILURE);
		goto Exit;
	}

	pIndex = m_pDict->getIndex( uiIndexNum);

	// Allocate the background thread and index status strucutures.

	if (RC_BAD( rc = f_calloc( (FLMUINT)sizeof( F_BKGND_IX), &pBackgroundIx)))
	{
		goto Exit;
	}

	pBackgroundIx->pDatabase = m_pDatabase;
	pBackgroundIx->indexStatus.eState = SFLM_INDEX_BRINGING_ONLINE;
	pBackgroundIx->indexStatus.uiIndexNum = uiIndexNum;
	pBackgroundIx->indexStatus.uiStartTime = uiGMT;
	pBackgroundIx->indexStatus.ui64LastRowIndexed =
		pIndex->ui64LastRowIndexed;
	pBackgroundIx->indexStatus.ui64KeysProcessed = 0;
	pBackgroundIx->indexStatus.ui64RowsProcessed = 0;
	pBackgroundIx->indexStatus.ui64Transactions = 0;

	pBackgroundIx->uiIndexingAction = FTHREAD_ACTION_INDEX_OFFLINE;
	pBackgroundIx->pPrev = NULL;
	pBackgroundIx->pNext = NULL;

	// Generate the thread name

	if (RC_BAD( rc = gv_SFlmSysData.pFileSystem->pathReduce( m_pDatabase->m_pszDbPath,
							szThreadName, szBaseName)))
	{
		goto Exit;
	}

	f_sprintf( (char *)szThreadName, "BldIX %u (%s)",
		(unsigned)uiIndexNum, szBaseName);

	// Start the thread in the background indexing thread group.
	// The new thread will cleanup pBackgroundIx on termination.

	if (RC_BAD( rc = gv_SFlmSysData.pThreadMgr->createThread( NULL,
						flmBackgroundIndexBuildThrd, szThreadName,
						gv_SFlmSysData.uiIndexingThreadGroup, uiIndexNum,
						(void *)pBackgroundIx, NULL, 24000)))
	{
		goto Exit;
	}

Exit:

	if (RC_BAD( rc) && pBackgroundIx)
	{
		f_free( &pBackgroundIx);
	}

	return( rc);
}

/****************************************************************************
Desc:	This routine is called by the thread that performs background
		indexing.
****************************************************************************/
RCODE F_Db::backgroundIndexBuild(
	IF_Thread *		pThread,
	FLMBOOL *		pbShutdown,
	FLMINT *			piErrorLine)
{
	RCODE					rc = NE_SFLM_OK;
	F_INDEX *			pIndex;
	F_BKGND_IX * 		pBackgroundIx = (F_BKGND_IX *)pThread->getParm1();
	FLMBOOL				bStartedTrans = FALSE;
	FLMUINT64			ui64FirstRowId;
	FLMUINT				uiIndexNum;
	FLMBOOL				bHitEnd;
	SFLM_INDEX_STATUS	savedIxStatus;

	if (RC_BAD( rc = checkState( __FILE__, __LINE__)))
	{
		*piErrorLine = (FLMINT)__LINE__;
		goto Exit;
	}

	flmAssert( m_eTransType == SFLM_NO_TRANS);

	m_uiFlags |= FDB_BACKGROUND_INDEXING;
	uiIndexNum = pBackgroundIx->indexStatus.uiIndexNum;

	for (;;)
	{

		// Set the thread's status

		pThread->setThreadStatus( FLM_THREAD_STATUS_RUNNING);

		// See if we should shut down.

		if (pThread->getShutdownFlag())
		{
			*pbShutdown = TRUE;
			break;
		}

		// Start a transaction

		if( RC_BAD( rc = beginBackgroundTrans( pThread)))
		{
			if (rc == NE_SFLM_DATABASE_LOCK_REQ_TIMEOUT)
			{
				// This would only happen if we were signaled to shut down.
				// So, it's ok to exit

				flmAssert( pThread->getShutdownFlag());
				*pbShutdown = TRUE;
				rc = NE_SFLM_OK;
			}
			else
			{
				*piErrorLine = (FLMINT)__LINE__;
			}
			goto Exit;
		}
		bStartedTrans = TRUE;

		if ((pIndex = m_pDict->getIndex( uiIndexNum)) == NULL)
		{
			rc = RC_SET( NE_SFLM_INVALID_INDEX_NUM);

			// Index may have been deleted by another transaction, or
			// there may have been some other error.

			*piErrorLine = (FLMINT)__LINE__;
			goto Exit;
		}

		// If the index is suspended, this thread should have been
		// shut down.  The suspending thread will keep the database
		// locked until we exit.  So, if we have the database locked,
		// the index better not be suspended.

		flmAssert( !(pIndex->uiFlags & IXD_SUSPENDED));
		pBackgroundIx->indexStatus.eState = SFLM_INDEX_BRINGING_ONLINE;

		if ((ui64FirstRowId = pIndex->ui64LastRowIndexed) == ~((FLMUINT64)0))
		{
			goto Exit;
		}

		// Setup the NODE range we want to index.

		ui64FirstRowId++;

		// Set the thread's status

		pThread->setThreadStatus( "Indexing %u:%I64u",
			(unsigned)pIndex->uiTableNum, ui64FirstRowId);

		// Read and index up to the highest row ID (or node higher than ui64EndNodeId)
		// or until time runs out.  The 500 is millisecs to take for the transaction.

		f_memcpy( &savedIxStatus,
			&pBackgroundIx->indexStatus, sizeof( SFLM_INDEX_STATUS));

		if (RC_BAD( rc = indexSetOfRows( uiIndexNum,
			ui64FirstRowId, FLM_MAX_UINT64, NULL, NULL,
			&pBackgroundIx->indexStatus, &bHitEnd, pThread)))
		{
			// Lock the mutex while copying the saved index status back to
			// the main index status so that someone requesting the index status
			// won't see the status while the memcpy is in progress.

			f_mutexLock( gv_SFlmSysData.hShareMutex);
			f_memcpy( &pBackgroundIx->indexStatus,
				&savedIxStatus, sizeof( SFLM_INDEX_STATUS));
			f_mutexUnlock( gv_SFlmSysData.hShareMutex);
			*piErrorLine = (FLMINT)__LINE__;
			goto Exit;
		}

		// Commit the transaction (even if we didn't do any indexing work).

		bStartedTrans = FALSE;
		if (RC_BAD( rc = commitTrans( 0, FALSE)))
		{
			*piErrorLine = (FLMINT)__LINE__;
			goto Exit;
		}
		pBackgroundIx->indexStatus.ui64Transactions++;

		if (bHitEnd)
		{
			break;
		}
	}

Exit:

	if (bStartedTrans)
	{
		(void)abortTrans();
		bStartedTrans = FALSE;
	}

	return( rc);
}

/****************************************************************************
Desc:		Thread that will build an index in the background.
			Caller will create a pDb to use.  This pDb must be
			freed at the conclusion of the routine.
****************************************************************************/
FSTATIC RCODE SQFAPI flmBackgroundIndexBuildThrd(
	IF_Thread *		pThread)
{
	RCODE				rc = NE_SFLM_OK;
	F_BKGND_IX *	pBackgroundIx = (F_BKGND_IX *)pThread->getParm1();
	F_Db *			pDb = NULL;
	FLMBOOL			bForcedShutdown = FALSE;
	FLMUINT			uiIndexNum;
	char				szMsg [128];
	FLMINT			iErrorLine = 0;
	F_DbSystem		dbSystem;

	pThread->setThreadStatus( FLM_THREAD_STATUS_INITIALIZING);

Loop_Again:

	rc = NE_SFLM_OK;
	uiIndexNum = pBackgroundIx->indexStatus.uiIndexNum;
	flmAssert( pThread->getThreadAppId() == uiIndexNum);
	pDb = NULL;

	// We could loop forever on internalDbOpen errors, 
	// check if we should exit.

	if( pThread->getShutdownFlag())
	{
		bForcedShutdown = TRUE;
		goto Exit;
	}

	if( RC_BAD( rc = dbSystem.internalDbOpen( 
		pBackgroundIx->pDatabase, &pDb)))
	{

		// If the file is being closed, this is not an error.

		if (pBackgroundIx->pDatabase->getFlags() & DBF_BEING_CLOSED)
		{
			bForcedShutdown = TRUE;
			rc = NE_SFLM_OK;
		}
		else
		{
			iErrorLine = (FLMINT)__LINE__;
		}
		goto Exit;
	}

	if (RC_BAD( rc = pDb->backgroundIndexBuild( pThread, &bForcedShutdown,
										&iErrorLine)))
	{
		goto Exit;
	}

Exit:

	pThread->setThreadStatus( FLM_THREAD_STATUS_TERMINATING);

	if (pDb)
	{
		pDb->Release();
		pDb = NULL;
	}

	if (RC_BAD(rc) && !bForcedShutdown)
	{
		if (rc == NE_SFLM_MEM || rc == NE_FLM_IO_DISK_FULL ||
			 rc == NE_SFLM_MUST_WAIT_CHECKPOINT)
		{
			// Log the error

			f_sprintf( szMsg,
				"Background indexing thread %u (index %u)",
				(unsigned)pThread->getThreadId(), (unsigned)uiIndexNum);
			flmLogError( rc, szMsg, __FILE__, iErrorLine);

			// Sleep a half second and try again.

			f_sleep( 500);
			goto Loop_Again;
		}
		else
		{
			f_sprintf( szMsg,
				"Background indexing thread %u (index %u) -- unrecoverable error.",
				(unsigned)pThread->getThreadId(), (unsigned)uiIndexNum);
			flmLogError( rc, szMsg, __FILE__, iErrorLine);
		}
	}

	// Set the thread's app ID to 0, so that it will not
	// be found now that the thread is terminating (we don't
	// want flmBackgroundIndexGet to find the thread).

	pThread->setThreadAppId( 0);

	// Free the background index structure

	f_mutexLock( gv_SFlmSysData.hShareMutex);
	pThread->setParm1( NULL);
	f_mutexUnlock( gv_SFlmSysData.hShareMutex);
	f_free( &pBackgroundIx);

	return( rc);
}

/****************************************************************************
Desc: Looks for a background indexing thread that is running with
		a matching action and value.
Note:	The shared semaphore must be locked on the outside while
		calling this routine and accessing anything within the F_BKGND_IX
		structure.
****************************************************************************/
F_BKGND_IX * flmBackgroundIndexGet(
	F_Database *	pDatabase,
	FLMUINT			uiIndexNum,
	FLMBOOL			bMutexLocked,
	FLMUINT *		puiThreadId)
{
	RCODE					rc = NE_SFLM_OK;
	IF_Thread *			pThread;
	FLMUINT				uiThreadId;
	F_BKGND_IX *		pBackgroundIx = NULL;

	if( !bMutexLocked)
	{
		f_mutexLock( gv_SFlmSysData.hShareMutex);
	}

	uiThreadId = 0;
	for( ;;)
	{
		if (RC_BAD( rc = gv_SFlmSysData.pThreadMgr->getNextGroupThread(
			&pThread, gv_SFlmSysData.uiIndexingThreadGroup, &uiThreadId)))
		{
			if( rc == NE_SFLM_NOT_FOUND)
			{
				rc = NE_SFLM_OK;
				break;
			}
			else
			{
				RC_UNEXPECTED_ASSERT( rc);
			}
		}

		if (pThread->getThreadAppId())
		{
			F_BKGND_IX *		pTmpIx = NULL;

			pTmpIx = (F_BKGND_IX *)pThread->getParm1();
			if (pTmpIx->indexStatus.uiIndexNum == uiIndexNum &&
				 pTmpIx->pDatabase == pDatabase)
			{
				flmAssert( pThread->getThreadAppId() == uiIndexNum);
				pBackgroundIx = pTmpIx;
				pThread->Release();
				if( puiThreadId)
				{
					*puiThreadId = uiThreadId;
				}
				break;
			}
		}
		pThread->Release();
	}

	if (!bMutexLocked)
	{
		f_mutexUnlock( gv_SFlmSysData.hShareMutex);
	}

	return( pBackgroundIx);
}
