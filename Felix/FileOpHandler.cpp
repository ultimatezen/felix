#include "StdAfx.h"
#include "FileOpHandler.h"

namespace fileops
{

void addExtensionAsNeeded( CString &fileName, const CString ext )
{
	ATLASSERT( ext.IsEmpty() == false && fileName.IsEmpty() == false ) ;
	ATLASSERT( ext.GetAt( 0 ) == _T('.') ) ;

	const file::CPath path( fileName ) ;
	if ( path.FindExtension().IsEmpty() )
	{
		fileName += ext ;
	}
}

unsigned int detect_codepage(LPCSTR text, FileFormat file_format)
{
	text ;
	file_format ;
	return CP_ACP ;
}
}