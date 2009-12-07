#include "StdAfx.h"
#include "FileOpHandler.h"

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
