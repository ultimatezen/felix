#include "StdAfx.h"
#include ".\drop.h"


CDropHandle::CDropHandle(HDROP dropped) :
	m_drop( dropped )
{
}

CDropHandle::~CDropHandle(void)
{
}




BOOL CDropHandle::DragQueryPoint(POINT* pt) const
{
	return ::DragQueryPoint( m_drop, pt );
}

POINT CDropHandle::DragQueryPoint() const
{
	POINT pt ;
	this->DragQueryPoint(&pt) ;
	return pt ;
}
UINT CDropHandle::NumDragFiles(void) const
{
	return ::DragQueryFile(m_drop, 0xffffFFFF, NULL, 0);
}


UINT CDropHandle::FileLen(UINT index) const
{
	return ::DragQueryFile(m_drop, index, NULL, 0 );
}

CString CDropHandle::DragQueryFile(UINT index) const
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
