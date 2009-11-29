#pragma once

#include "CStringInterface.h"
#include "Path.h"

template< class baseClass >
class CFileOpHandler
{
protected:
	void addExtensionAsNeeded( CString &fileName, const CString ext )
	{
		ATLASSERT( ext.IsEmpty() == false && fileName.IsEmpty() == false ) ;
		ATLASSERT( ext.GetAt( 0 ) == _T('.') ) ;

		file::CPath path( fileName ) ;
		if ( path.FindExtension().IsEmpty() )
		{
			fileName += ext ;
		}
	}
public:
	CFileOpHandler(void){}
	~CFileOpHandler(void){}
};
