#include "pch.h"
#include "Globals.h"


void FindFiles( A2Array< CString >& aRetFiles, const CString& sStartDir )
{
	WIN32_FIND_DATA sFD;
	CString sPath = sStartDir + _T( "\\*" );
	HANDLE handle = FindFirstFile( sPath, &sFD );
	if ( handle != INVALID_HANDLE_VALUE )
	{
		do
		{
			if ( !( sFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				if ( ( wcsncmp( _T( "." ), sFD.cFileName, 1 ) !=0 ) && ( wcsncmp( _T( ".." ), sFD.cFileName, 2 ) != 0 ) )
				{
					CString sNextDir = sStartDir + _T( "\\" ) + sFD.cFileName;
					aRetFiles.Add( sNextDir );
				}
			}

		} while ( FindNextFile( handle, &sFD ) != 0 );
	}
}
