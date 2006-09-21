//------------------------------------------------------------------------------
// Desc:	Output Stream
//
// Tabs:	3
//
//		Copyright (c) 2006 Novell, Inc. All Rights Reserved.
//
//		This program is free software; you can redistribute it and/or
//		modify it under the terms of version 2 of the GNU General Public
//		License as published by the Free Software Foundation.
//
//		This program is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//		GNU General Public License for more details.
//
//		You should have received a copy of the GNU General Public License
//		along with this program; if not, contact Novell, Inc.
//
//		To contact Novell about this file by physical or electronic mail,
//		you may find current contact information at www.novell.com
//
// $Id$
//------------------------------------------------------------------------------

using System;
using System.Runtime.InteropServices;

namespace xflaim
{

	/// <summary>
	/// The OStream class encapsulates an IF_OStream object that was allocated
	/// in unmanaged space.  It will make sure that the stream object will be
	/// freed when the C# OStream object goes away.
	/// NOTE: This object should NEVER be allocated by an application directly.
	/// It is returned from various methods in the <see cref="DbSystem"/> class,
	/// such as <see cref="DbSystem.openFileOStream"/>,
	/// <see cref="DbSystem.openMultiFileOStream"/>, etc.
	/// </summary>
	public class OStream 
	{
		private ulong			m_pOStream;		// Pointer to IF_OStream object allocated in unmanaged space.
		private DbSystem		m_dbSystem;

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="pOStream">
		/// Pointer to IF_OStream object that was allocated from unmanaged space.
		/// </param>
		/// <param name="dbSystem">
		/// Pointer to <see cref="DbSystem"/> object.
		/// </param>
		internal OStream(
			ulong			pOStream,
			DbSystem 	dbSystem)
		{
			if (pOStream == 0)
			{
				throw new XFlaimException( "Invalid IF_OStream object pointer");
			}
			
			m_pOStream = pOStream;
			
			if (dbSystem == null)
			{
				throw new XFlaimException( "Invalid DbSystem object");
			}
			
			m_dbSystem = dbSystem;

			// Must call something inside of DbSystem.  Otherwise, the
			// m_dbSystem object gets a compiler warning on linux because
			// it is not used anywhere.  Other than that, there is really
			// no need to make the following call.
			if (m_dbSystem.getDbSystem() == 0)
			{
				throw new XFlaimException( "Invalid DbSystem.IF_DbSystem object");
			}
		}
		
		/// <summary>
		/// Destructor
		/// </summary>
		~OStream()
		{
			close();
		}

		/// <summary>
		/// Close the output stream and free the IF_OStream object.
		/// </summary>
		public void close()
		{
			if (m_pOStream != 0)
			{
				xflaim_OStream_Release( m_pOStream);
				m_pOStream = 0;
			}

			m_dbSystem = null;
		}

		[DllImport("xflaim")]
		private static extern void xflaim_OStream_Release(
			ulong	pOStream);

		internal ulong getOStream()
		{
			return( m_pOStream);
		}
	}
}
