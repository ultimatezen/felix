#pragma once

/**
@class CMySplitterWindow 
@brief Passes up EM notifications to parent (replace with listener!).
*/
class CMySplitterWindow : public CSplitterWindowImpl<CSplitterWindowT<true>, true>
{
public:
	DECLARE_WND_CLASS_EX(_T("WTL_MySplitterWindow"), CS_DBLCLKS, COLOR_WINDOW)

	BEGIN_MSG_MAP(CMySplitterWindow)
		PASS_UP_NOTIFICATION_COMMAND( EN_CHANGE )
		PASS_UP_NOTIFICATION_COMMAND( EN_IMECHANGE )
		if(CSplitterWindowImpl<CSplitterWindowT<true>, true>::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult))
			return TRUE ;
	END_MSG_MAP()

} ;