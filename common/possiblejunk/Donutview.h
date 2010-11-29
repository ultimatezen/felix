// DonutView.h : interface of the CDonutView class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#define docHostUIFlagDEFAULT docHostUIFlagFLAT_SCROLLBAR | docHostUIFlagNO3DBORDER | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE

#include "IDocHostUIHandlerDispatchImpl.h"

#include "MtlBase.h"
#include "MtlCom.h"
#include "MtlBrowser.h"

class CDonutView :
	public CWindowImpl<CDonutView, CAxWindow>,
	public CWebBrowser2/*,
	// if you implement UIHandler and DropTarget, NO3DEBORDER and AUTOCOMPLETE are ignored,
	// and the drop to TEXTFORM is ignored too.
	public IDocHostUIHandlerDispatchImpl,
	public IDropTargetImpl<CDonutView>*/
{
public:
// Declaration
	DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())

// Data members
	CComQIPtr<IAxWinAmbientDispatchEx> m_spAxAmbient;
	CDonutViewOption<CDonutView<CWindowImpl<CDonutView, CAxWindow>, CWebBrowser2> > m_ViewOption;
	DWORD m_dwDefaultDLControlFlags;
	
// Constructor
	CDonutView(DWORD dwDefaultDLControlFlags)
		: m_ViewOption(this), m_dwDefaultDLControlFlags(dwDefaultDLControlFlags)
	{ }

// Methods
	DWORD GetDLControlFlags()
	{
		DWORD dwDLControlFlags = DLCTL_DEFAULT;
		m_spAxAmbient->get_DLControlFlags(&dwDLControlFlags);
		return dwDLControlFlags;
	}

	void PutDLControlFlags(DWORD dwDLControlFlags)
	{
		m_spAxAmbient->put_DLControlFlags(dwDLControlFlags);
	}

// Overrides		
	BOOL PreTranslateMessage(MSG* pMsg)
	{
		if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
			return FALSE;

		// give HTML page a chance to translate this message
		return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
	}

// Message map and handlers
	BEGIN_MSG_MAP(CDonutView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		COMMAND_ID_HANDLER_EX(ID_DLCTL_BGSOUNDS, OnMultiBgsounds)
		COMMAND_ID_HANDLER_EX(ID_DLCTL_VIDEOS, OnMultiVideos)
		COMMAND_ID_HANDLER_EX(ID_DLCTL_DLIMAGES, OnMultiDlImages)
		COMMAND_ID_HANDLER_EX(ID_DLCTL_RUNACTIVEXCTLS, OnSecurRunactivexctls)
		COMMAND_ID_HANDLER_EX(ID_DLCTL_DLACTIVEXCTLS, OnSecurDlactivexctls)
		COMMAND_ID_HANDLER_EX(ID_DLCTL_SCRIPTS, OnSecurScritps)
		COMMAND_ID_HANDLER_EX(ID_DLCTL_JAVA, OnSecurJava)
		CHAIN_MSG_MAP_MEMBER(m_ViewOption)
	END_MSG_MAP()
	
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		ATLTRACE2(atlTraceUser, 4, _T("CDonutView::OnCreate in\n"));
		// Let me initialize itself
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);

		HRESULT hr = QueryControl(IID_IWebBrowser2, (void**)&m_spBrowser);
		if (FAILED(hr)) {
			ATLTRACE2(atlTraceUser, 4, _T(" failed to get IWebBrowser2\n"));
			return lRet;
		}

		// Set flat scrollbar style
		CComPtr<IAxWinHostWindow> spAxWindow;
		hr = QueryHost(&spAxWindow);
		if (FAILED(hr))
			return lRet;

//		::SendMessage(GetTopLevelParent(), WM_USER_INITIAL_UPDATE, (WPARAM)m_spBrowser.p, (LPARAM)spAxWindow.p);

		m_spAxAmbient = spAxWindow;
		if (!m_spAxAmbient)
			return lRet;
		
		hr = m_spAxAmbient->put_DocHostFlags(docHostUIFlagDEFAULT);
		if (FAILED(hr)) {
			ATLTRACE2(atlTraceUser, 4, _T(" failed to put_DocHostFlags\n"));
			return lRet;
		}

		_InitDLControlFlags();
		
		ATLASSERT(m_ViewOption.m_dwExStyle == 0);
		if (_bNaviLock) {
			m_ViewOption.m_dwExStyle |= DVS_EX_OPENNEWWIN;
		}

		ATLTRACE2(atlTraceUser, 4, _T("CDonutView::OnCreate out\n"));
		return lRet;
	}

	void OnDestroy()
	{
		SetMsgHandled(FALSE);

		m_ViewOption.Uninit();
	}

	void OnMultiBgsounds(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		_ToggleFlag(ID_DLCTL_BGSOUNDS, DLCTL_BGSOUNDS);
		_LightRefresh();
	}

	void OnMultiVideos(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		_ToggleFlag(ID_DLCTL_VIDEOS, DLCTL_VIDEOS);
		_LightRefresh();
	}

	void OnMultiDlImages(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		ATLTRACE2(atlTraceUser, 4, _T("CDonutView::OnMultiDlImages\n"));
		if (_ToggleFlag(ID_DLCTL_DLIMAGES, DLCTL_DLIMAGES))
			_LightRefresh();
	}

	void OnSecurRunactivexctls(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		_ToggleFlag(ID_DLCTL_RUNACTIVEXCTLS, DLCTL_NO_RUNACTIVEXCTLS, TRUE);
		_LightRefresh();
	}

	void OnSecurDlactivexctls(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		if (!_ToggleFlag(ID_DLCTL_DLACTIVEXCTLS, DLCTL_NO_DLACTIVEXCTLS, TRUE))
			_LightRefresh();
	}

	void OnSecurScritps(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		_ToggleFlag(ID_DLCTL_SCRIPTS, DLCTL_NO_SCRIPTS, TRUE);
		_LightRefresh();
	}

	void OnSecurJava(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		_ToggleFlag(ID_DLCTL_JAVA, DLCTL_NO_JAVA, TRUE);
		_LightRefresh();
	}

	void _InitDLControlFlags()
	{
		m_spAxAmbient->put_DLControlFlags(m_dwDefaultDLControlFlags);
	}

	DWORD _GetDLControlFlags()
	{
		DWORD dwDLControlFlags = DLCTL_DEFAULT;
		if (m_spAxAmbient)
			m_spAxAmbient->get_DLControlFlags(&dwDLControlFlags);
		return dwDLControlFlags;
	}
	
// Implementation
protected:
	bool _ToggleFlag(WORD wID, DWORD dwFlag, BOOL bReverse = FALSE)
	{
		bool bRet = false;
		DWORD dwDLControlFlags = DLCTL_DEFAULT;
		m_spAxAmbient->get_DLControlFlags(&dwDLControlFlags);

		if (dwDLControlFlags & dwFlag) {
			dwDLControlFlags &= ~dwFlag;
		}
		else {
			dwDLControlFlags |= dwFlag;
			bRet = true;
		}

		m_spAxAmbient->put_DLControlFlags(dwDLControlFlags);
		return bRet;
	}

	void _LightRefresh()
	{
		CString strURL = GetLocationURL();
		Navigate2(strURL);
	}

	BEGIN_UPDATE_COMMAND_UI_MAP(CDonutView)
		CHAIN_UPDATE_COMMAND_UI_MEMBER(m_ViewOption)

		UPDATE_COMMAND_UI(ID_DLCTL_DLIMAGES, OnUpdateDLCTL_DLIMAGES)				// with popup
		UPDATE_COMMAND_UI(ID_DLCTL_RUNACTIVEXCTLS, OnUpdateDLCTL_RUNACTIVEXCTLS)	// with popup
		UPDATE_COMMAND_UI_ENABLE_SETCHECK_FLAG(ID_DLCTL_BGSOUNDS, DLCTL_BGSOUNDS, GetDLControlFlags())
		UPDATE_COMMAND_UI_ENABLE_SETCHECK_FLAG(ID_DLCTL_VIDEOS, DLCTL_VIDEOS, GetDLControlFlags())
		UPDATE_COMMAND_UI_ENABLE_SETCHECK_FLAG_REV(ID_DLCTL_DLACTIVEXCTLS, DLCTL_NO_DLACTIVEXCTLS, GetDLControlFlags())
		UPDATE_COMMAND_UI_ENABLE_SETCHECK_FLAG_REV(ID_DLCTL_SCRIPTS, DLCTL_NO_SCRIPTS, GetDLControlFlags())
		UPDATE_COMMAND_UI_ENABLE_SETCHECK_FLAG_REV(ID_DLCTL_JAVA, DLCTL_NO_JAVA, GetDLControlFlags())
	END_UPDATE_COMMAND_UI_MAP()

	void OnUpdateDLCTL_DLIMAGES(CCmdUI* pCmdUI)
	{
		if (pCmdUI->m_menuSub.m_hMenu) {// popup menu
			pCmdUI->m_menu.EnableMenuItem(pCmdUI->m_nIndex, MF_BYPOSITION | MF_ENABLED);
		}
		else {
			pCmdUI->Enable();
			pCmdUI->SetCheck(GetDLControlFlags() & DLCTL_DLIMAGES ? 1 : 0);
		}
	}

	void OnUpdateDLCTL_RUNACTIVEXCTLS(CCmdUI* pCmdUI)
	{
		if (pCmdUI->m_menuSub.m_hMenu) {// popup menu
			pCmdUI->m_menu.EnableMenuItem(pCmdUI->m_nIndex, MF_BYPOSITION | MF_ENABLED);
		}
		else {
			pCmdUI->Enable();
			pCmdUI->SetCheck(GetDLControlFlags() & DLCTL_NO_RUNACTIVEXCTLS ? 0 : 1);
		}
	}

	void OnUpdateDocHostUIOpenNewWinUI(CCmdUI* pCmdUI)
	{
		DWORD dwDocHostFlags;
		m_spAxAmbient->get_DocHostFlags(&dwDocHostFlags);
		pCmdUI->Enable();
		pCmdUI->SetCheck(dwDocHostFlags & docHostUIFlagOPENNEWWIN ? 1 : 0);
	}

	bool OnScroll(UINT nScrollCode, UINT nPos, bool bDoScroll = true)
	{
		return false;
	}

// Overrides
/*
	STDMETHOD(GetDropTarget)(IUnknown  *pDropTarget, IUnknown  **ppDropTarget)
	{
		*ppDropTarget = (IUnknown*)(_IDropTargetLocator*)this;
		return S_OK;
	}
	DROPEFFECT OnDragEnter(IDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		return _MtlStandardDropEffect(dwKeyState);
	}
	DROPEFFECT OnDragOver(IDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		return DROPEFFECT_LINK | _MtlStandardDropEffect(dwKeyState);
	}
	DROPEFFECT OnDrop(IDataObject* pDataObject,	DROPEFFECT dropEffect,
		DROPEFFECT dropEffectList, CPoint point)
	{
		return DROPEFFECT_NONE;
	}
	void OnDragLeave()
	{
	}
*/
};

/////////////////////////////////////////////////////////////////////////////
