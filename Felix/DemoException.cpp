#include "StdAfx.h"
#include "DemoException.h"

CDemoException::CDemoException(void) :
	CException( _T("Demo Exception") )
{
}

CDemoException::~CDemoException(void)
{
}

/** Give the demo notice.
 FullName:  CDemoException::notify_user
 Access:    public const 
 Qualifier: 
 \return    int
 \param     const CString &top_message
 \param     UINT flags 
 \param     const CString &title 
 \param     HWND parent_hwnd 
 */
int CDemoException::notify_user( const CString & /*top_message*/,  UINT /*flags*/ /*= MB_OK*/,  const CString & /*title*/ /*= _T("ERROR")*/, HWND parent_hwnd /*= ::GetActiveWindow()  */ ) 
{
	parent_hwnd ;
	// nag him!
	CNagDialog nagger( _T("NAGPAGE_SIZE.HTML") ) ;
#ifndef UNIT_TEST
	return  nagger.DoModal( parent_hwnd ) ;
#else
	return IDCANCEL ;
#endif
}
