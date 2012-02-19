#include "StdAfx.h"
#include "FileOpHandler.h"
#include <shlobj.h>
#include "ComUtilities.h"
#include "Exceptions.h"

namespace fileops
{

void add_extension_as_needed( CString &fileName, const CString ext )
{
	ATLASSERT( ext.IsEmpty() == false && fileName.IsEmpty() == false ) ;
	ATLASSERT( ext.GetAt( 0 ) == _T('.') ) ;

	const file::CPath path( fileName ) ;
	if ( path.FindExtension().IsEmpty() )
	{
		fileName += ext ;
	}
}


CString get_local_appdata_folder()
{
	TCHAR szPath[MAX_PATH];
	COM_ENFORCE(SHGetFolderPath(NULL, // hwndOwner
		CSIDL_LOCAL_APPDATA,		  // nFolder
		(HANDLE)NULL,				  // hToken (-1 means "default user")
		SHGFP_TYPE_CURRENT,			  // dwFlags 
		szPath
		), _T("Failed to retrieve local app data folder") ) ; 

	return CString(szPath) ;
}
}