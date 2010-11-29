/////////////////////////////////////////////////////////////
// Class CVXPStatic
//
// This class implements a standard STATIC control, which instead
// of drawing simple text draws QHTML documents. The class provides
// for all settings of the standard STATIC control, such as
// alignment and disable state. It also supports an extra feature
// called sunken effect that can be applied when drawing QHTML.
//
// Last Modified on July 20, 2003
//
// This library is an intellectual property of Vitaly Tomilov
//
// For more details and updates visit www.tooltips.net
//////////////////////////////////////////////////////////////

#if !defined(AFX_VXPSTATIC_H__1F05D2BB_6165_47DA_AD58_0B75BEF4E34D__INCLUDED_)
#define AFX_VXPSTATIC_H__1F05D2BB_6165_47DA_AD58_0B75BEF4E34D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VXPStatic.h : header file
//

#include "VXPLib.h"

/////////////////////////////////////////////////////////////////////////////
// CVXPStatic window

class CVXPStatic : public CStatic, public CVXPQuickHTML
{
// Construction
public:
	CVXPStatic();

// Attributes
public:

// Two operators and one function for easier text assignment
// to the conrol. They all do the same by passing the text
// to the QHTML parser.
	CVXPStatic & operator = (LPCTSTR Text);
	CVXPStatic & operator = (BSTR bstrText);
	void SetWindowText(LPCTSTR lpszString);
////////////////////////////////////////////////////////////

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVXPStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

	void SetSunken(bool bSunken);
	bool GetSunken();

	// This method will update the control in a way much more
	// efficient than when calling Invalidate or RedrawWindow
	// Call this method to update the static control, unless
	// you change the control's alignment when calling Invalidate
	// is inevitable for correct update.
	void UpdateControl(bool bEraseBackground = true);

	// Generated message map functions
protected:
	//{{AFX_MSG(CVXPStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	bool m_bSunken;	// When True the static object is drawn with a special effect.

	RECT m_UpdateRect;	// Rectangle to be updated.

	bool m_bModified;	// Contains true to indicate that QHTML document
						// of the control just has been modified.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VXPSTATIC_H__1F05D2BB_6165_47DA_AD58_0B75BEF4E34D__INCLUDED_)
