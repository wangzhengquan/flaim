/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class xflaim_DbSystem */

#ifndef _Included_xflaim_DbSystem
#define _Included_xflaim_DbSystem
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     xflaim_DbSystem
 * Method:    _createDbSystem
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1createDbSystem
  (JNIEnv *, jobject);

/*
 * Class:     xflaim_DbSystem
 * Method:    _release
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1release
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _dbCreate
 * Signature: (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lxflaim/CREATEOPTS;)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1dbCreate
  (JNIEnv *, jobject, jlong, jstring, jstring, jstring, jstring, jstring, jobject);

/*
 * Class:     xflaim_DbSystem
 * Method:    _dbOpen
 * Signature: (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1dbOpen
  (JNIEnv *, jobject, jlong, jstring, jstring, jstring, jstring, jboolean);

/*
 * Class:     xflaim_DbSystem
 * Method:    _dbRemove
 * Signature: (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1dbRemove
  (JNIEnv *, jobject, jlong, jstring, jstring, jstring, jboolean);

/*
 * Class:     xflaim_DbSystem
 * Method:    _dbRestore
 * Signature: (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lxflaim/RestoreClient;Lxflaim/RestoreStatus;)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1dbRestore
  (JNIEnv *, jobject, jlong, jstring, jstring, jstring, jstring, jstring, jobject, jobject);

/*
 * Class:     xflaim_DbSystem
 * Method:    _dbCheck
 * Signature: (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILxflaim/DbCheckStatus;)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1dbCheck
  (JNIEnv *, jobject, jlong, jstring, jstring, jstring, jstring, jint, jobject);

/*
 * Class:     xflaim_DbSystem
 * Method:    _dbCopy
 * Signature: (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lxflaim/DbCopyStatus;)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1dbCopy
  (JNIEnv *, jobject, jlong, jstring, jstring, jstring, jstring, jstring, jstring, jobject);

/*
 * Class:     xflaim_DbSystem
 * Method:    _dbRename
 * Signature: (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZLxflaim/DbRenameStatus;)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1dbRename
  (JNIEnv *, jobject, jlong, jstring, jstring, jstring, jstring, jboolean, jobject);

/*
 * Class:     xflaim_DbSystem
 * Method:    _dbRebuild
 * Signature: (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lxflaim/CREATEOPTS;Lxflaim/RebuildStatus;)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1dbRebuild
  (JNIEnv *, jobject, jlong, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jobject, jobject);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openBufferIStream
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openBufferIStream
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openFileIStream
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openFileIStream
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openMultiFileIStream
 * Signature: (JLjava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openMultiFileIStream
  (JNIEnv *, jobject, jlong, jstring, jstring);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openBufferedIStream
 * Signature: (JJI)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openBufferedIStream
  (JNIEnv *, jobject, jlong, jlong, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openUncompressingIStream
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openUncompressingIStream
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openBase64Encoder
 * Signature: (JJZ)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openBase64Encoder
  (JNIEnv *, jobject, jlong, jlong, jboolean);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openBase64Decoder
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openBase64Decoder
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openFileOStream
 * Signature: (JLjava/lang/String;Z)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openFileOStream
  (JNIEnv *, jobject, jlong, jstring, jboolean);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openMultiFileOStream
 * Signature: (JLjava/lang/String;Ljava/lang/String;IZ)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openMultiFileOStream
  (JNIEnv *, jobject, jlong, jstring, jstring, jint, jboolean);

/*
 * Class:     xflaim_DbSystem
 * Method:    _removeMultiFileStream
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1removeMultiFileStream
  (JNIEnv *, jobject, jlong, jstring, jstring);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openBufferedOStream
 * Signature: (JJI)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openBufferedOStream
  (JNIEnv *, jobject, jlong, jlong, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _openCompressingOStream
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1openCompressingOStream
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _writeToOStream
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1writeToOStream
  (JNIEnv *, jobject, jlong, jlong, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _createJDataVector
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1createJDataVector
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _updateIniFile
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1updateIniFile
  (JNIEnv *, jobject, jlong, jstring, jstring);

/*
 * Class:     xflaim_DbSystem
 * Method:    _dbDup
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_xflaim_DbSystem__1dbDup
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setDynamicMemoryLimit
 * Signature: (JIIII)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setDynamicMemoryLimit
  (JNIEnv *, jobject, jlong, jint, jint, jint, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setHardMemoryLimit
 * Signature: (JIZIIIZ)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setHardMemoryLimit
  (JNIEnv *, jobject, jlong, jint, jboolean, jint, jint, jint, jboolean);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getDynamicCacheSupported
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_xflaim_DbSystem__1getDynamicCacheSupported
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getCacheInfo
 * Signature: (J)Lxflaim/CacheInfo;
 */
JNIEXPORT jobject JNICALL Java_xflaim_DbSystem__1getCacheInfo
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _enableCacheDebug
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1enableCacheDebug
  (JNIEnv *, jobject, jlong, jboolean);

/*
 * Class:     xflaim_DbSystem
 * Method:    _cacheDebugEnabled
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_xflaim_DbSystem__1cacheDebugEnabled
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _closeUnusedFiles
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1closeUnusedFiles
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _startStats
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1startStats
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _stopStats
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1stopStats
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _resetStats
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1resetStats
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getStats
 * Signature: (J)Lxflaim/Stats;
 */
JNIEXPORT jobject JNICALL Java_xflaim_DbSystem__1getStats
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setTempDir
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setTempDir
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getTempDir
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_xflaim_DbSystem__1getTempDir
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setCheckpointInterval
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setCheckpointInterval
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getCheckpointInterval
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_xflaim_DbSystem__1getCheckpointInterval
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setCacheAdjustInterval
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setCacheAdjustInterval
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getCacheAdjustInterval
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_xflaim_DbSystem__1getCacheAdjustInterval
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setCacheCleanupInterval
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setCacheCleanupInterval
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getCacheCleanupInterval
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_xflaim_DbSystem__1getCacheCleanupInterval
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setUnusedCleanupInterval
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setUnusedCleanupInterval
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getUnusedCleanupInterval
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_xflaim_DbSystem__1getUnusedCleanupInterval
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setMaxUnusedTime
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setMaxUnusedTime
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getMaxUnusedTime
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_xflaim_DbSystem__1getMaxUnusedTime
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _deactivateOpenDb
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1deactivateOpenDb
  (JNIEnv *, jobject, jlong, jstring, jstring);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setQuerySaveMax
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setQuerySaveMax
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getQuerySaveMax
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_xflaim_DbSystem__1getQuerySaveMax
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _setDirtyCacheLimits
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1setDirtyCacheLimits
  (JNIEnv *, jobject, jlong, jint, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getMaxDirtyCacheLimit
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_xflaim_DbSystem__1getMaxDirtyCacheLimit
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _getLowDirtyCacheLimit
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_xflaim_DbSystem__1getLowDirtyCacheLimit
  (JNIEnv *, jobject, jlong);

/*
 * Class:     xflaim_DbSystem
 * Method:    _compareStrings
 * Signature: (JLjava/lang/String;ZLjava/lang/String;ZII)I
 */
JNIEXPORT jint JNICALL Java_xflaim_DbSystem__1compareStrings
  (JNIEnv *, jobject, jlong, jstring, jboolean, jstring, jboolean, jint, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _hasSubStr
 * Signature: (JLjava/lang/String;Ljava/lang/String;II)Z
 */
JNIEXPORT jboolean JNICALL Java_xflaim_DbSystem__1hasSubStr
  (JNIEnv *, jobject, jlong, jstring, jstring, jint, jint);

/*
 * Class:     xflaim_DbSystem
 * Method:    _uniIsUpper
 * Signature: (JC)Z
 */
JNIEXPORT jboolean JNICALL Java_xflaim_DbSystem__1uniIsUpper
  (JNIEnv *, jobject, jlong, jchar);

/*
 * Class:     xflaim_DbSystem
 * Method:    _uniIsLower
 * Signature: (JC)Z
 */
JNIEXPORT jboolean JNICALL Java_xflaim_DbSystem__1uniIsLower
  (JNIEnv *, jobject, jlong, jchar);

/*
 * Class:     xflaim_DbSystem
 * Method:    _uniIsAlpha
 * Signature: (JC)Z
 */
JNIEXPORT jboolean JNICALL Java_xflaim_DbSystem__1uniIsAlpha
  (JNIEnv *, jobject, jlong, jchar);

/*
 * Class:     xflaim_DbSystem
 * Method:    _uniIsDecimalDigit
 * Signature: (JC)Z
 */
JNIEXPORT jboolean JNICALL Java_xflaim_DbSystem__1uniIsDecimalDigit
  (JNIEnv *, jobject, jlong, jchar);

/*
 * Class:     xflaim_DbSystem
 * Method:    _uniToLower
 * Signature: (JC)C
 */
JNIEXPORT jchar JNICALL Java_xflaim_DbSystem__1uniToLower
  (JNIEnv *, jobject, jlong, jchar);

/*
 * Class:     xflaim_DbSystem
 * Method:    _waitToClose
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1waitToClose
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     xflaim_DbSystem
 * Method:    _clearCache
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_xflaim_DbSystem__1clearCache
  (JNIEnv *, jobject, jlong, jlong);

#ifdef __cplusplus
}
#endif
#endif
