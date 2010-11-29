
#pragma once

#include <vector>
#include <string>

#include <boost/assign/std/vector.hpp> // for 'operator+=()'
using namespace boost::assign; // bring 'operator+=()' into scope

namespace ATL
{
	/////////////////////////////////////////////////////////////////////////////
	// CFakeWindow - client side for a Windows window

	class CFakeWindow
	{
	public:
		static RECT rcDefault;
		RECT m_client_rect ;
		RECT m_window_rect ;
		HWND m_hWnd;

		std::vector< std::string > m_FakeWindow_calls ;
		BOOL m_FakeWindow_is_window ;

		CFakeWindow(HWND hWnd = NULL) throw() :
			m_hWnd(hWnd),
			m_FakeWindow_is_window(TRUE)
		{
			m_client_rect.bottom = m_client_rect.left = m_client_rect.right = m_client_rect.top = 0 ;
			m_window_rect.bottom = m_window_rect.left = m_window_rect.right = m_window_rect.top = 0 ;
		}

		CFakeWindow& operator=(HWND hWnd) throw()
		{
			m_hWnd = hWnd;
			return *this;
		}

		void sense(std::string value)
		{
			m_FakeWindow_calls += value ;
		}
		static LPCTSTR GetWndClassName() throw()
		{
			return NULL;
		}

		void Attach(HWND hWndNew) throw()
		{
			ATLASSUME(m_hWnd == NULL);
			m_hWnd = hWndNew;
		}

		HWND Detach() throw()
		{
			sense("Detach") ;
			HWND hWnd = m_hWnd;
			m_hWnd = NULL;
			return hWnd;
		}

		HWND Create(LPCTSTR lpstrWndClass, HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL) throw()
		{
			lpstrWndClass ;
			hWndParent ;
			rect ;
			szWindowName ;
			dwExStyle ;
			dwStyle ;
			MenuOrID ;
			lpCreateParam ;

			sense( "Create" ) ;
			m_FakeWindow_is_window =  TRUE ;
			return m_hWnd;
		}

		BOOL DestroyWindow() throw()
		{
			ATLASSERT(this->IsWindow());

			this->m_FakeWindow_is_window = FALSE ;
			m_hWnd = NULL;
			sense( "DestroyWindow" ) ;
			return TRUE;
		}

		// Attributes

		operator HWND() const throw()
		{ 
			return m_hWnd; 
		}

		DWORD GetStyle() const throw()
		{
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		DWORD GetExStyle() const throw()
		{
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		LONG GetWindowLong(int nIndex) const throw()
		{
			nIndex ;

			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		LONG_PTR GetWindowLongPtr(int nIndex) const throw()
		{
			nIndex ;

			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		LONG SetWindowLong(int nIndex, LONG dwNewLong) throw()
		{
			nIndex ;
			dwNewLong ;

			ATLASSERT(this->IsWindow());
			sense( "SetWindowLong" ) ;
			return 0 ;
		}

		LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) throw()
		{
			nIndex ;
			dwNewLong ;

			ATLASSERT(this->IsWindow());
			sense( "SetWindowLongPtr" ) ;
			return 0 ;
		}

		WORD GetWindowWord(int nIndex) const throw()
		{
			nIndex ;

			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		WORD SetWindowWord(int nIndex, WORD wNewWord) throw()
		{
			nIndex ;
			wNewWord ;

			ATLASSERT(this->IsWindow());
			sense( "SetWindowWord" ) ;
			return 0 ;
		}

		// Message Functions

		LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) throw()
		{
			message ;
			wParam ;
			lParam ;

			ATLASSERT(this->IsWindow());
			sense( "SendMessage" ) ;
			return 0 ;
		}

		BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) throw()
		{
			message ;
			wParam ;
			lParam ;

			ATLASSERT(this->IsWindow());
			sense( "PostMessage" ) ;
			return TRUE ;
		}

		BOOL SendNotifyMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) throw()
		{
			message ;
			wParam ;
			lParam ;

			ATLASSERT(this->IsWindow());
			sense( "SendNotifyMessage" ) ;
			return TRUE ;
		}

		// support for C style macros
		static LRESULT SendMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) throw()
		{
			hWnd ;
			message ;
			wParam ;
			lParam ;

			return TRUE ;
		}

		// Window Text Functions

		BOOL SetWindowText(LPCTSTR lpszString) throw()
		{
			lpszString ;

			ATLASSERT(this->IsWindow());
			sense( "SetWindowText" ) ;
			return TRUE ;
		}

		int GetWindowText(__out_ecount_part_z(nMaxCount, return + 1) LPTSTR lpszStringBuf, __in int nMaxCount) const throw()
		{
			nMaxCount ;
			lpszStringBuf ;

			ATLASSERT(this->IsWindow());
			return 0 ;
		}
		int GetWindowText(CSimpleString& strText) const
		{
			strText ;
			return 0 ;
		}
		int GetWindowTextLength() const throw()
		{
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		// Font Functions

		void SetFont(HFONT hFont, BOOL bRedraw = TRUE) throw()
		{
			hFont ;
			bRedraw ;

			ATLASSERT(this->IsWindow());
			sense( "SetFont" ) ;
		}

		HFONT GetFont() const throw()
		{
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		// Menu Functions (non-child windows only)

		HMENU GetMenu() const throw()
		{
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		BOOL SetMenu(HMENU hMenu) throw()
		{
			hMenu ;
			ATLASSERT(this->IsWindow());
			sense( "SetMenu" ) ;
			return TRUE ;
		}

		BOOL DrawMenuBar() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "DrawMenuBar" ) ;
			return TRUE ;
		}

		HMENU GetSystemMenu(BOOL bRevert) const throw()
		{
			bRevert ;
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		BOOL HiliteMenuItem(HMENU hMenu, UINT uItemHilite, UINT uHilite) throw()
		{
			hMenu ;
			uItemHilite ;
			uHilite ;

			ATLASSERT(this->IsWindow());
			sense( "HiliteMenuItem" ) ;
			return TRUE ;
		}

		// Window Size and Position Functions

		BOOL IsIconic() const throw()
		{
			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		BOOL IsZoomed() const throw()
		{
			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		BOOL MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE) throw()
		{
			x ;
			y ;
			nWidth ;
			nHeight ;
			bRepaint ;

			ATLASSERT(this->IsWindow());
			sense( "MoveWindow" ) ;
			return TRUE ;
		}

		BOOL MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE) throw()
		{
			lpRect ;
			bRepaint ;

			ATLASSERT(this->IsWindow());
			sense( "MoveWindow" ) ;
			return TRUE ;
		}

		BOOL SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags) throw()
		{
			hWndInsertAfter ;
			x ;
			y ;
			cx ;
			cy ;
			nFlags ;

			ATLASSERT(this->IsWindow());
			sense( "SetWindowPos" ) ;
			return TRUE ;
		}

		BOOL SetWindowPos(HWND hWndInsertAfter, LPCRECT lpRect, UINT nFlags) throw()
		{
			hWndInsertAfter ;
			lpRect ;
			nFlags ;

			ATLASSERT(this->IsWindow());
			sense( "SetWindowPos" ) ;
			return TRUE ;
		}

		UINT ArrangeIconicWindows() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ArrangeIconicWindows" ) ;
			return 0 ;
		}

		BOOL BringWindowToTop() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "BringWindowToTop" ) ;
			return TRUE ;
		}

		BOOL GetWindowRect(LPRECT lpRect) const throw()
		{
			lpRect->bottom = m_window_rect.bottom;
			lpRect->left = m_window_rect.left;
			lpRect->right = m_window_rect.right;
			lpRect->top = m_window_rect.top;
			return TRUE ;
		}

		BOOL GetClientRect(LPRECT lpRect) const throw()
		{
			lpRect->bottom = m_client_rect.bottom;
			lpRect->left = m_client_rect.left;
			lpRect->right = m_client_rect.right;
			lpRect->top = m_client_rect.top;
			return TRUE ;
		}

		BOOL GetWindowPlacement(WINDOWPLACEMENT FAR* lpwndpl) const throw()
		{
			lpwndpl ;

			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		BOOL SetWindowPlacement(const WINDOWPLACEMENT FAR* lpwndpl) throw()
		{
			lpwndpl ;

			ATLASSERT(this->IsWindow());
			sense( "SetWindowPlacement" ) ;
			return TRUE ;
		}

		// Coordinate Mapping Functions

		BOOL ClientToScreen(LPPOINT lpPoint) const throw()
		{
			lpPoint ;

			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		BOOL ClientToScreen(LPRECT lpRect) const throw()
		{
			lpRect ;

			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		BOOL ScreenToClient(LPPOINT lpPoint) const throw()
		{
			lpPoint ;

			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		BOOL ScreenToClient(LPRECT lpRect) const throw()
		{
			lpRect ;

			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		int MapWindowPoints(HWND hWndTo, LPPOINT lpPoint, UINT nCount) const throw()
		{
			hWndTo ;
			lpPoint ;
			nCount ;

			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		int MapWindowPoints(HWND hWndTo, LPRECT lpRect) const throw()
		{
			hWndTo ;
			lpRect ;

			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		// Update and Painting Functions

		HDC BeginPaint(LPPAINTSTRUCT lpPaint) throw()
		{
			lpPaint ;

			ATLASSERT(this->IsWindow());
			sense( "BeginPaint" ) ;
			return 0 ;
		}

		void EndPaint(LPPAINTSTRUCT lpPaint) throw()
		{
			lpPaint ;

			ATLASSERT(this->IsWindow());
			sense( "EndPaint" ) ;
		}

		HDC GetDC() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "GetDC" ) ;
			return 0 ;
		}

		HDC GetWindowDC() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "GetWindowDC" ) ;
			return 0 ;
		}

		int ReleaseDC(HDC hDC) throw()
		{
			hDC ;

			ATLASSERT(this->IsWindow());
			sense( "ReleaseDC" ) ;
			return 0 ;
		}

		void Print(HDC hDC, DWORD dwFlags) const throw()
		{
			hDC ;
			dwFlags ;

			ATLASSERT(this->IsWindow());
		}

		void PrintClient(HDC hDC, DWORD dwFlags) const throw()
		{
			hDC ;
			dwFlags ;

			ATLASSERT(this->IsWindow());
		}

		BOOL UpdateWindow() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "UpdateWindow" ) ;
			return TRUE ;
		}

		void SetRedraw(BOOL bRedraw = TRUE) throw()
		{
			bRedraw ;

			ATLASSERT(this->IsWindow());
			sense( "SetRedraw" ) ;
		}

		BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE) throw()
		{
			lpRect ;
			bErase ;

			ATLASSERT(this->IsWindow());
			sense( "GetUpdateRect" ) ;
			return TRUE ;
		}

		int GetUpdateRgn(HRGN hRgn, BOOL bErase = FALSE) throw()
		{
			hRgn ;
			bErase ;

			ATLASSERT(this->IsWindow());
			sense( "GetUpdateRgn" ) ;
			return 0 ;
		}

		BOOL Invalidate(BOOL bErase = TRUE) throw()
		{
			bErase ;

			ATLASSERT(this->IsWindow());
			sense( "Invalidate" ) ;
			return TRUE ;
		}

		BOOL InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE) throw()
		{
			lpRect ;
			bErase ;

			ATLASSERT(this->IsWindow());
			sense( "InvalidateRect" ) ;
			return TRUE ;
		}

		BOOL ValidateRect(LPCRECT lpRect) throw()
		{
			lpRect ;

			ATLASSERT(this->IsWindow());
			sense( "ValidateRect" ) ;
			return TRUE ;
		}

		void InvalidateRgn(HRGN hRgn, BOOL bErase = TRUE) throw()
		{
			hRgn ;
			bErase ;

			ATLASSERT(this->IsWindow());
			sense( "InvalidateRgn" ) ;
		}

		BOOL ValidateRgn(HRGN hRgn) throw()
		{
			hRgn ;

			ATLASSERT(this->IsWindow());
			sense( "ValidateRgn" ) ;
			return TRUE ;
		}

		BOOL ShowWindow(int nCmdShow) throw()
		{
			nCmdShow ;
			ATLASSERT(this->IsWindow());
			sense( "ShowWindow" ) ;
			return TRUE ;
		}

		BOOL IsWindowVisible() const throw()
		{
			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		BOOL ShowOwnedPopups(BOOL bShow = TRUE) throw()
		{
			bShow ;

			ATLASSERT(this->IsWindow());
			sense( "ShowOwnedPopups" ) ;
			return TRUE ;
		}

		HDC GetDCEx(HRGN hRgnClip, DWORD flags) throw()
		{
			hRgnClip ;
			flags ;

			ATLASSERT(this->IsWindow());
			sense( "GetDCEx" ) ;
			return 0 ;
		}

		BOOL LockWindowUpdate(BOOL bLock = TRUE) throw()
		{
			bLock ;

			ATLASSERT(this->IsWindow());
			sense( "LockWindowUpdate" ) ;
			return TRUE ;
		}

		BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL, HRGN hRgnUpdate = NULL, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE) throw()
		{
			lpRectUpdate ;
			hRgnUpdate ;
			flags ;

			ATLASSERT(this->IsWindow());
			sense( "RedrawWindow" ) ;
			return TRUE ;
		}

		// Timer Functions

		UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse, void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD) = NULL) throw()
		{
			nIDEvent ;
			nElapse ;
			lpfnTimer ;

			ATLASSERT(this->IsWindow());
			sense( "SetTimer" ) ;
			return 0 ;
		}

		BOOL KillTimer(UINT_PTR nIDEvent) throw()
		{
			nIDEvent ;

			ATLASSERT(this->IsWindow());
			sense( "KillTimer" ) ;
			return TRUE ;
		}

		// Window State Functions

		BOOL IsWindowEnabled() const throw()
		{
			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		BOOL EnableWindow(BOOL bEnable = TRUE) throw()
		{
			bEnable ;

			ATLASSERT(this->IsWindow());
			sense( "EnableWindow" ) ;
			return TRUE ;
		}

		HWND SetActiveWindow() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "SetActiveWindow" ) ;
			return 0 ;
		}

		HWND SetCapture() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "SetCapture" ) ;
			return 0 ;
		}

		HWND SetFocus() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "SetFocus" ) ;
			return 0 ;
		}

		// Dialog-Box Item Functions

		BOOL CheckDlgButton(int nIDButton, UINT nCheck) throw()
		{
			nIDButton ;
			nCheck ;

			ATLASSERT(this->IsWindow());
			sense( "CheckDlgButton" ) ;
			return TRUE ;
		}

		BOOL CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) throw()
		{
			nIDFirstButton ;
			nIDLastButton ;
			nIDCheckButton ;

			ATLASSERT(this->IsWindow());
			sense( "CheckRadioButton" ) ;
			return TRUE ;
		}

		int DlgDirList(__inout_z LPTSTR lpPathSpec, __in int nIDListBox, __in int nIDStaticPath, __in UINT nFileType) throw()
		{
			lpPathSpec ;
			nIDListBox ;
			nIDStaticPath ;
			nFileType ;

			ATLASSERT(this->IsWindow());
			sense( "DlgDirList" ) ;
			return TRUE ;
		}

		int DlgDirListComboBox(__inout_z LPTSTR lpPathSpec, __in int nIDComboBox, __in int nIDStaticPath, __in UINT nFileType) throw()
		{
			lpPathSpec ;
			nIDComboBox ;
			nIDStaticPath ;
			nFileType ;

			ATLASSERT(this->IsWindow());
			sense( "DlgDirListComboBox" ) ;
			return TRUE ;
		}

		BOOL DlgDirSelect(__out_ecount_z(nCount) LPTSTR /*lpString*/, __in int /*nCount*/, __in int /*nIDListBox*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "DlgDirSelect" ) ;
			return TRUE ;
		}

		BOOL DlgDirSelectComboBox(__out_ecount_z(nCount) LPTSTR /*lpString*/, __in int /*nCount*/, __in int /*nIDComboBox*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "DlgDirSelectComboBox" ) ;
			return TRUE ;
		}

		UINT GetDlgItemInt(int /*nID*/, BOOL* /* lpTrans = NULL*/, BOOL /*bSigned = TRUE*/) const throw()
		{
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		UINT GetDlgItemText(__in int nID, __out_ecount_part_z(nMaxCount, return + 1) LPTSTR /*lpStr*/, __in int /*nMaxCount*/) const throw()
		{
			nID ;

			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		UINT GetDlgItemText(int /*nID*/, CSimpleString& /*strText*/) const
		{
			return 0 ;
		}
#ifdef _OLEAUTO_H_
		BOOL GetDlgItemText(int nID, BSTR& bstrText) const throw()
		{
			ATLASSERT(this->IsWindow());

			HWND hWndCtl = GetDlgItem(nID);
			if(hWndCtl == NULL)
				return FALSE;

			return CFakeWindow(hWndCtl).GetWindowText(bstrText);
		}
#endif // _OLEAUTO_H_
		CFakeWindow GetNextDlgGroupItem(HWND hWndCtl, BOOL bPrevious = FALSE) const throw()
		{
			hWndCtl ;
			bPrevious ;
			ATLASSERT(this->IsWindow());
			return CFakeWindow() ;
		}

		CFakeWindow GetNextDlgTabItem(HWND hWndCtl, BOOL bPrevious = FALSE) const throw()
		{
			hWndCtl ;
			bPrevious ;
			ATLASSERT(this->IsWindow());
			return CFakeWindow() ;
		}

		UINT IsDlgButtonChecked(int nIDButton) const throw()
		{
			nIDButton ;
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		LRESULT SendDlgItemMessage(int /*nID*/, UINT /*message*/, WPARAM /*wParam = 0*/, LPARAM /*lParam = 0*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "SendDlgItemMessage" ) ;
			return 0 ;
		}

		BOOL SetDlgItemInt(int /*nID*/, UINT /*nValue*/, BOOL /*bSigned = TRUE*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "SetDlgItemInt" ) ;
			return 0 ;
		}

		BOOL SetDlgItemText(int /*nID*/, LPCTSTR /*lpszString*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "SetDlgItemText" ) ;
			return 0 ;
		}

#ifndef _ATL_NO_HOSTING
		HRESULT GetDlgControl(int /*nID*/, REFIID /*iid*/, void** /*ppCtrl*/) throw()
		{
			sense( "GetDlgControl" ) ;
			return S_OK ;
		}
		HRESULT GetDlgHost(int /*nID*/, REFIID /*iid*/, void** /*ppHost*/) throw()
		{
			sense( "GetDlgHost" ) ;
			return S_OK ;
		}
#endif //!_ATL_NO_HOSTING

		// Scrolling Functions

		int GetScrollPos(int /*nBar*/) const throw()
		{
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		BOOL GetScrollRange(int /*nBar*/, LPINT /*lpMinPos*/, LPINT /*lpMaxPos*/) const throw()
		{
			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		BOOL ScrollWindow(int /*xAmount*/, int /*yAmount*/, LPCRECT /*lpRect = NULL*/, LPCRECT /*lpClipRect = NULL*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		int ScrollWindowEx(int /*dx*/, int /*dy*/, LPCRECT /*lpRectScroll*/, LPCRECT /*lpRectClip*/, HRGN /*hRgnUpdate*/, LPRECT /*lpRectUpdate*/, UINT /*uFlags*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return 0 ;
		}

		int ScrollWindowEx(int /*dx*/, int /*dy*/, UINT /*uFlags*/, LPCRECT /*lpRectScroll = NULL*/, LPCRECT /*lpRectClip = NULL*/, HRGN /*hRgnUpdate = NULL*/, LPRECT /*lpRectUpdate = NULL*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		int SetScrollPos(int /*nBar*/, int /*nPos*/, BOOL bRedraw = TRUE) throw()
		{
			bRedraw ;
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL SetScrollRange(int /*nBar*/, int /*nMinPos*/, int /*nMaxPos*/, BOOL bRedraw = TRUE) throw()
		{
			bRedraw ;
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL ShowScrollBar(UINT /*nBar*/, BOOL bShow = TRUE) throw()
		{
			bShow ;

			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL EnableScrollBar(UINT /*uSBFlags*/, UINT uArrowFlags = ESB_ENABLE_BOTH) throw()
		{
			uArrowFlags ;

			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		// Window Access Functions

		CFakeWindow ChildWindowFromPoint(POINT /*point*/) const throw()
		{
			ATLASSERT(this->IsWindow());
			return CFakeWindow() ;
		}

		CFakeWindow ChildWindowFromPointEx(POINT /*point*/, UINT /*uFlags*/) const throw()
		{
			ATLASSERT(this->IsWindow());
			return CFakeWindow() ;
		}

		CFakeWindow GetTopWindow() const throw()
		{
			ATLASSERT(this->IsWindow());
			return CFakeWindow() ;
		}

		CFakeWindow GetWindow(UINT /*nCmd*/) const throw()
		{
			ATLASSERT(this->IsWindow());
			return CFakeWindow() ;
		}

		CFakeWindow GetLastActivePopup() const throw()
		{
			ATLASSERT(this->IsWindow());
			return CFakeWindow() ;
		}

		BOOL IsChild(HWND /*hWnd*/) const throw()
		{
			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		CFakeWindow GetParent() const throw()
		{
			ATLASSERT(this->IsWindow());
			return CFakeWindow() ;
		}

		CFakeWindow SetParent(HWND /*hWndNewParent*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return CFakeWindow() ;
		}

		// Window Tree Access

		int GetDlgCtrlID() const throw()
		{
			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		int SetDlgCtrlID(int /*nID*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		CFakeWindow GetDlgItem(int /*nID*/) const throw()
		{
			ATLASSERT(this->IsWindow());
			return CFakeWindow() ;
		}

		// Alert Functions

		BOOL FlashWindow(BOOL /*bInvert*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		int MessageBox(LPCTSTR /*lpszText*/, LPCTSTR lpszCaption = _T(""), UINT nType = MB_OK) throw()
		{
			lpszCaption ;
			nType ;

			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		// Clipboard Functions

		BOOL ChangeClipboardChain(HWND /*hWndNewNext*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		HWND SetClipboardViewer() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return 0 ;
		}

		BOOL OpenClipboard() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		// Caret Functions

		BOOL CreateCaret(HBITMAP /*hBitmap*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL CreateSolidCaret(int /*nWidth*/, int /*nHeight*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL CreateGrayCaret(int /*nWidth*/, int /*nHeight*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL HideCaret() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL ShowCaret() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

#ifdef _INC_SHELLAPI
		// Drag-Drop Functions
		void DragAcceptFiles(BOOL bAccept = TRUE) throw()
		{
			bAccept ;

			ATLASSERT(this->IsWindow()); 
			sense( "ClientToScreen" ) ;
		}
#endif

		// Icon Functions

		HICON SetIcon(HICON /*hIcon*/, BOOL bBigIcon = TRUE) throw()
		{
			bBigIcon ;

			sense( "ClientToScreen" ) ;
			return 0 ;
		}

		HICON GetIcon(BOOL bBigIcon = TRUE) const throw()
		{
			bBigIcon ;

			return 0 ;
		}

		// Help Functions

		BOOL WinHelp(LPCTSTR /*lpszHelp*/, UINT nCmd = HELP_CONTEXT, DWORD dwData = 0) throw()
		{
			nCmd ;
			dwData ;

			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL SetWindowContextHelpId(DWORD /*dwContextHelpId*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		DWORD GetWindowContextHelpId() const throw()
		{
			ATLASSERT(this->IsWindow());
			return 0 ;
		}

		// Hot Key Functions

		int SetHotKey(WORD /*wVirtualKeyCode*/, WORD /*wModifiers*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		DWORD GetHotKey() const throw()
		{
			ATLASSERT(this->IsWindow());
			return TRUE ;
		}

		// Misc. Operations

		//N new
		BOOL GetScrollInfo(int /*nBar*/, LPSCROLLINFO /*lpScrollInfo*/) throw()
		{
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}
		int SetScrollInfo(int /*nBar*/, LPSCROLLINFO /*lpScrollInfo*/, BOOL bRedraw = TRUE) throw()
		{
			bRedraw ;

			sense( "ClientToScreen" ) ;
			return TRUE ;
		}
		BOOL IsDialogMessage(LPMSG /*lpMsg*/) throw()
		{
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		void NextDlgCtrl() const throw()
		{
			ATLASSERT(this->IsWindow());
		}
		void PrevDlgCtrl() const throw()
		{
			ATLASSERT(this->IsWindow());
		}
		void GotoDlgCtrl(HWND /*hWndCtrl*/) const throw()
		{
			ATLASSERT(this->IsWindow());
		}

		BOOL ResizeClient(int /*nWidth*/, int /*nHeight*/, BOOL bRedraw = TRUE) throw()
		{
			bRedraw ;
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		int GetWindowRgn(HRGN /*hRgn*/) throw()
		{
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}
		int SetWindowRgn(HRGN /*hRgn*/, BOOL bRedraw = FALSE) throw()
		{
			bRedraw ;

			sense( "ClientToScreen" ) ;
			return TRUE ;
		}
		HDWP DeferWindowPos(HDWP /*hWinPosInfo*/, HWND /*hWndInsertAfter*/, int /*x*/, int /*y*/, int /*cx*/, int /*cy*/, UINT /*uFlags*/) throw()
		{
			sense( "ClientToScreen" ) ;
			return 0 ;
		}
		DWORD GetWindowThreadID() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}
		DWORD GetWindowProcessID() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}
		BOOL IsWindow() const throw()
		{
			return this->m_FakeWindow_is_window ;
		}
		BOOL IsWindowUnicode() const throw()
		{
			ATLASSERT(this->IsWindow());
			return TRUE ;
		}
		BOOL IsParentDialog() throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}
		BOOL ShowWindowAsync(int /*nCmdShow*/) throw()
		{
			ATLASSERT(this->IsWindow());
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		CFakeWindow GetDescendantWindow(int /*nID*/) const throw()
		{
			ATLASSERT(this->IsWindow());

			return CFakeWindow();
		}

		void SendMessageToDescendants(UINT /*message*/, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bDeep = TRUE) throw()
		{
			wParam ;
			lParam ;
			bDeep ;

			sense( "ClientToScreen" ) ;
		}

		BOOL CenterWindow(HWND hWndCenter = NULL) throw()
		{
			hWndCenter ;

			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL ModifyStyle(DWORD /*dwRemove*/, DWORD /*dwAdd*/, UINT nFlags = 0) throw()
		{
			nFlags ;

			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

		BOOL ModifyStyleEx(DWORD /*dwRemove*/, DWORD /*dwAdd*/, UINT nFlags = 0) throw()
		{
			nFlags ;

			sense( "ClientToScreen" ) ;
			return TRUE ;
		}

#ifdef _OLEAUTO_H_
		BOOL GetWindowText(BSTR* pbstrText) throw()
		{
			return GetWindowText(*pbstrText);
		}
		BOOL GetWindowText(BSTR& /*bstrText*/) throw()
		{
			sense( "ClientToScreen" ) ;
			return TRUE ;
		}
#endif // _OLEAUTO_H_
		CFakeWindow GetTopLevelParent() const throw()
		{
			ATLASSERT(this->IsWindow());

			return CFakeWindow();
		}

		CFakeWindow GetTopLevelWindow() const throw()
		{
			ATLASSERT(this->IsWindow());
			return CFakeWindow();
		}
	};

	_declspec(selectany) RECT CFakeWindow::rcDefault = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };
}