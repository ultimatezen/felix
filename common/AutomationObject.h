// AutomationObject.h: interface for the AutomationObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOMATIONOBJECT_H__FB9ABBEF_26B3_4FA7_BDBF_7D744D0991D3__INCLUDED_)
#define AFX_AUTOMATIONOBJECT_H__FB9ABBEF_26B3_4FA7_BDBF_7D744D0991D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Exceptions.h" // exception support
#include "StringEx.h"	// string support
#include "atlbase.h"	// CComVariant

// class AutomationObject
// throws _com_error

class AutomationObject  
{
	typedef	CComPtr< IDispatch > dispatch_ptr ;
protected:
	dispatch_ptr	m_app ;

public:
	AutomationObject() 
	{
		COM_ENFORCE( ::CoInitialize(NULL), "AutomationObject constructor: Call to CoInitialize failed!" ) ;
	}
	~AutomationObject() 
	{
		m_app.Release() ;
		::CoUninitialize();
	}

};

#endif // !defined(AFX_AUTOMATIONOBJECT_H__FB9ABBEF_26B3_4FA7_BDBF_7D744D0991D3__INCLUDED_)
