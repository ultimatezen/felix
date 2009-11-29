#include "StdAfx.h"
#include ".\drop.h"


CDropHandle::CDropHandle(HDROP dropped) :
	m_drop( dropped )
{
}

CDropHandle::~CDropHandle(void)
{
}




BOOL CDropHandle::DragQueryPoint(POINT* pt)
{
	return ::DragQueryPoint( m_drop, pt );
}

UINT CDropHandle::NumDragFiles(void)
{
	return ::DragQueryFile(m_drop, 0xffffFFFF, NULL, 0);
}


UINT CDropHandle::FileLen(UINT index)
{
	return ::DragQueryFile(m_drop, index, NULL, 0 );
}

CString CDropHandle::DragQueryFile(UINT index)
{
	UINT len = FileLen( index ) ;

	CString dropfile ;
	UINT len_copied = ::DragQueryFile( m_drop, index, dropfile.GetBuffer(len+1), len+1 ) ;
	dropfile.ReleaseBuffer(len_copied) ;

	return dropfile ;
}

CDrop::CDrop(HDROP dropped) :
	CDropHandle( dropped )
{
}

CDrop::~CDrop(void)
{
	::DragFinish( m_drop ) ;
}
