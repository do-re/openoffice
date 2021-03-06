/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_filter.hxx"


#include <osl/file.h>

#if defined( UNX) || defined(OS2)

#include <stdio.h>
#include <string.h>
#include <osl/thread.h>

oslFileError SAL_CALL my_getTempDirURL( rtl_uString** pustrTempDir )
{
    const char *pValue = getenv( "TEMP" );

    if ( !pValue )
	{
        pValue = getenv( "TMP" );
#if defined(SOLARIS) || defined (LINUX)
		if ( !pValue )
			pValue = P_tmpdir;
#endif
	}

	if ( pValue )
	{
		oslFileError error;
		rtl_uString	*ustrTempPath = NULL;

		rtl_string2UString( &ustrTempPath, pValue, strlen( pValue ), osl_getThreadTextEncoding(), OSTRING_TO_OUSTRING_CVTFLAGS );
		error = osl_getFileURLFromSystemPath( ustrTempPath, pustrTempDir );
		rtl_uString_release( ustrTempPath );
		
		return error;
	}
	else
		return osl_File_E_NOENT;
}
#else

#ifdef NDEBUG
#    define NO_DEBUG_CRT
#endif

#ifndef _WIN32_WINNT 
#	define _WIN32_WINNT 0x0400
#	define _CTYPE_DISABLE_MACROS /* wg. dynamischer C-Runtime MH */
#endif

#if defined _MSC_VER
#pragma warning(push, 1)
#endif 

#define WIN32_LEAN_AND_MEAN
#include <windows.h>    
#include <malloc.h>

#if defined _MSC_VER
#pragma warning(pop)
#endif 

#define elementsof(arr) (sizeof(arr)/sizeof(arr[0]))

oslFileError SAL_CALL my_getTempDirURL( rtl_uString** pustrTempDir )
{
	WCHAR	szBuffer[MAX_PATH];
	LPWSTR	lpBuffer = szBuffer;
	DWORD	nBufferLength = elementsof(szBuffer) - 1;

	DWORD			nLength;
	oslFileError	error;

	do
	{
		nLength = GetTempPathW( elementsof(szBuffer), lpBuffer );
		if ( nLength > nBufferLength )
		{
			nLength++;
			lpBuffer = (LPWSTR)alloca( sizeof(WCHAR) * nLength );
			nBufferLength = nLength - 1;
		}
	} while ( nLength > nBufferLength );

	if ( nLength )
	{
		rtl_uString	*ustrTempPath = NULL;

		if ( '\\' == lpBuffer[nLength-1] )
			lpBuffer[nLength-1] = 0;

		rtl_uString_newFromStr( &ustrTempPath, reinterpret_cast<const sal_Unicode*>(lpBuffer) );

		error = osl_getFileURLFromSystemPath( ustrTempPath, pustrTempDir );

		rtl_uString_release( ustrTempPath );
	}
	else
		error = GetLastError() == ERROR_SUCCESS ? osl_File_E_None : osl_File_E_INVAL;

	return error;
}
#endif

#include "tempfile.hxx"

using namespace rtl;

TempFile::TempFile( const OUString& rTempFileURL )
:osl::File( rTempFileURL ), maURL( rTempFileURL )
{
}

TempFile::~TempFile()
{
	close();

	if( maURL.getLength() )
		osl::File::remove( maURL );
}

OUString TempFile::createTempFileURL()
{
	OUString aTempFileURL;

	const sal_uInt32 nRadix = 26;

	OUString aTempDirURL;
	/* oslFileError nRC = */ my_getTempDirURL( &aTempDirURL.pData );

	static sal_uInt32 u = osl_getGlobalTimer();
	for ( sal_uInt32 nOld = u; ++u != nOld; )
	{
		u %= (nRadix*nRadix*nRadix);
		OUString aTmp( aTempDirURL );
        if( aTmp.getStr()[ aTmp.getLength() - 1 ] != sal_Unicode( '/' ) )
            aTmp += OUString( RTL_CONSTASCII_USTRINGPARAM( "/" ));
		aTmp += OUString::valueOf( (sal_Int32) (unsigned) u, nRadix );
		aTmp += OUString::createFromAscii( ".tmp" );

		osl::File aFile( aTmp );
		osl::FileBase::RC err = aFile.open(osl_File_OpenFlag_Create);
		if (  err == FileBase::E_None )
		{
			aTempFileURL = aTmp;
			aFile.close();
			break;
		}
		else if ( err != FileBase::E_EXIST )
		{
			 // if f.e. name contains invalid chars stop trying to create files
			 break;
		}
	}

	return aTempFileURL;
}

OUString TempFile::getFileURL()
{
	return maURL;
}
