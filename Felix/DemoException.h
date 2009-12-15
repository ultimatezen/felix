#pragma once
#include "exceptions.h"
#include "NagDialog.h"

//////////////////////////////////////////////////////////////////////////
// Exception raised if the demo limit is reached (max memory size).
// \class CDemoException
// 
//////////////////////////////////////////////////////////////////////////
class CDemoException :
	public except::CException
{
public:
	CDemoException(void);
public:
	~CDemoException(void);

	int notify_user
		( 
		const CString &top_message, 
		UINT flags = MB_OK, 
		const CString &title = _T("ERROR"),
		HWND parent_hwnd = ::GetActiveWindow() 
		);


};
