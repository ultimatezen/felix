#if !defined(AFX_PARSERTESTSHEET_H__87744B9F_5F31_4C5C_A00C_4FAFB35D3413__INCLUDED_)
#define AFX_PARSERTESTSHEET_H__87744B9F_5F31_4C5C_A00C_4FAFB35D3413__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParserTestSheet.h : header file
//

// This class we need just to support automatic updates
// for the toolbar.
// (to make macro ON_UPDATE_COMMAND_UI work for toolbars)
class CToolCmdUI : public CCmdUI
{
protected:
	virtual void Enable(BOOL bOn)
	{
		m_bEnableChanged = TRUE;
		CToolBar* pToolBar = (CToolBar*)m_pOther;
		UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) & ~TBBS_DISABLED;
		if (!bOn)
		{
			nNewStyle |= TBBS_DISABLED;
			nNewStyle &= ~TBBS_PRESSED;
		}
		pToolBar->SetButtonStyle(m_nIndex, nNewStyle);
	}
};

#include "InputPPG.h"
#include "OutputPPG.h"
#include "SettingsPPG.h"

/////////////////////////////////////////////////////////////////////////////
// CParserTestSheet

class CParserTestSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CParserTestSheet)

// Construction
public:
	CParserTestSheet();

// Attributes
public:

	CInputPPG m_Input;
	COutputPPG m_Output;
	CSettingsPPG m_Settings;

	CVXPTooltipManager m_tm;

	bool m_bCanClose;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParserTestSheet)
	public:
	virtual BOOL ContinueModal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CParserTestSheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT OnKickIdle(WPARAM, LPARAM);
	afx_msg void OnUpdateCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectAll(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	HACCEL m_hAccel;
	CVXPQuickHTML m_footer;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARSERTESTSHEET_H__87744B9F_5F31_4C5C_A00C_4FAFB35D3413__INCLUDED_)
