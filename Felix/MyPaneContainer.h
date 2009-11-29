/** 
 *  MyPaneContainer   version:  1.0   -  date: 08/30/2006
 *
 */
#pragma once


/**
@class CMyPaneContainer 
@brief Customized pane container control.
*/
class CMyPaneContainer 
	: public CPaneContainerImpl<CMyPaneContainer>
{
public:
	DECLARE_WND_CLASS_EX(_T("WTL_MyPaneContainer"), 0, -1)

	BEGIN_MSG_MAP(CMyPaneContainer)
		PASS_UP_NOTIFICATION_COMMAND( EN_CHANGE )
		PASS_UP_NOTIFICATION_COMMAND( EN_IMECHANGE )
		if(CPaneContainerImpl<CMyPaneContainer>::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult))
			return TRUE ;
	END_MSG_MAP()

} ;
