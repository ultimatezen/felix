/*!
	@file Drop.h
	@brief Interface for CDrop and CDropHandle classes.
	@date 2005/06/25
	Time: 15:17:11
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

/**
	@class CDropHandle
	@brief Wrapper for HDROP.
 */
class CDropHandle
{
protected:
	HDROP m_drop ;
public:
	CDropHandle(HDROP dropped);
	virtual ~CDropHandle(void);
	BOOL DragQueryPoint(POINT* pt) const;
	UINT NumDragFiles(void) const;
	UINT FileLen(UINT index) const;
	CString DragQueryFile(UINT index) const;
};

/**
	@class CDrop  
	@brief Wrapper for HDROP.
	Closes the handle in the dtor.
 */
class CDrop : public CDropHandle
{
public:
	CDrop(HDROP dropped);
	~CDrop(void);
};
