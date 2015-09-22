#include "stdafx.h"
#include "system_paths.h"
#include "FileOpHandler.h"

CString get_local_helpfile_name( CString path ) 
{
	file::CPath filePath( path ) ;
	filePath.Append(resource_string(IDS_HELPFILE_NAME) ) ;
	return filePath.Path() ;
}


CString get_help_file_path( CString path )
{
	file::CPath filePath( get_local_helpfile_name(path) ) ;

	if ( ! filePath.FileExists() ) 
	{
		filePath.ReplaceFileSpec(resource_string(IDS_PDF_HELPFILE_NAME)) ;
	}

	if ( ! filePath.FileExists() ) 
	{
		return resource_string(IDS_ONLINE_HELPFILE_URL) ;
	}

	return filePath.Path() ;
}
CString get_docs_path()
{
	// get the instance
	file::CPath modpath(fileops::get_local_appdata_folder()) ;
	modpath.Append(_T("Felix")) ;

	modpath.Append( _T("DOCS") ) ;
	modpath.AddBackslash() ;

	return modpath.Path() ;
}