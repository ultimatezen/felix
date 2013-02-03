#pragma once

/**
	@class CCommandEdit 
	@brief Edit class handling keyboard shortcuts for cut/paste etc.
 */
class CCommandEdit : public CWindowImpl<CCommandEdit, CEditT<CWindow> >,
					public CEditCommands<CCommandEdit>
{
	public:

		void SetText(const wstring text)
		{
			SetWindowText(text.c_str()) ;
		}
		wstring GetText() const
		{
			CString text ;
			GetWindowText(text) ;
			return static_cast<LPCTSTR>(text) ;
		}
		CCommandEdit& operator=(const ATL::CWindow &window)
		{
			this->m_hWnd = window.m_hWnd ;
			return *this ;
		}
	
	BEGIN_MSG_MAP(CCommandEdit)
		CHAIN_MSG_MAP_ALT(CEditCommands<CCommandEdit>, 1)
	END_MSG_MAP()
} ;