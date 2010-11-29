// VXPStatic.cpp : implementation file
//

#include "stdafx.h"
#include "StaticText.h"
#include "VXPStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVXPStatic

CVXPStatic::CVXPStatic()
{
	Transparent = true;	// Draw text using Transparency

	m_bSunken = false;	// Do not sink the output by default
	
	m_UpdateRect.left = -1; // No calling UpdateControl has been made
}

CVXPStatic & CVXPStatic::operator = (LPCTSTR Text)
{
	m_bModified = true;		// QHTML of the control has been modified

	SetSourceText(Text);	// Assigning text to the QHTML parser
	if(::IsWindow(m_hWnd))	// If the static control has been created
	{
		RecalculateLayout();// Recalculate the QHTML document layout
		UpdateControl();	// Update the control
	}
	return *this;
}

CVXPStatic & CVXPStatic::operator = (BSTR bstrText)
{
	m_bModified = true;		// QHTML of the control has been modified

	SetSourceText(bstrText);// Assigning text to the QHTML parser
	if(::IsWindow(m_hWnd))	// If the static control has been created
	{
		RecalculateLayout();// Recalculate the QHTML document layout
		UpdateControl();	// Update the control
	}
	return *this;
}

void CVXPStatic::SetWindowText(LPCTSTR lpszString)
{
	(*this) = lpszString;	// Just passing it to the operator
}

BEGIN_MESSAGE_MAP(CVXPStatic, CStatic)
	//{{AFX_MSG_MAP(CVXPStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVXPStatic message handlers

void CVXPStatic::OnPaint()
{
	CPaintDC dc(this);					// device context for painting
	
	DWORD wndStyle = GetStyle();		// Retrieving style of the static control
										// to know how to align the output
	RECT r;
	GetClientRect(&r);					// Retrieving client rectangle of the control

	// If OnPaint was called through UpdateControl, and QHTML document
	// of the control hasn't been just modified:
	if(m_UpdateRect.left >= 0 && !m_bModified)
	{
		FillRect(dc, &m_UpdateRect, ::GetSysColorBrush(COLOR_3DFACE));
		m_UpdateRect.left = -1;			// Has processed last call from UpdateControl
	}
	else
		FillRect(dc, &r, ::GetSysColorBrush(COLOR_3DFACE));	// Fill the whole area

	int x = 0;							// Assuming no shift to the right

	if(wndStyle & SS_CENTER)			// If center-aligned
		x = (r.right - Width) / 2;
	else
		if(wndStyle & SS_RIGHT)			// If right-aligned
			x = r.right - Width;

	int y = 0;							// Assuming no down shift

	if(wndStyle & SS_CENTERIMAGE)		// If vertically aligned
		y = (r.bottom - Height) / 2;

// The output can be too large and exceed the size
// of the static control. In case it happens:
	if(x < 0)
		x = 0;
	if(y < 0)
		y = 0;

// Drawing the control:
	if(wndStyle & WS_DISABLED)				// If the control is disabled
		DrawState(dc, (short)x, (short)y, dssDisabled);	// Drawing the control's disabled state
	else
	{
		if(m_bSunken)	// If Sunken effect is to be applied
			DrawState(dc, (short)x, (short)y, dssDisabled);
		Draw(dc, (short)x, (short)y);	// Drawing the document
	}
}

void CVXPStatic::PreSubclassWindow()
{
	// Recalculating the QHTML document to save time
	// when the initial drawing is called:
	RecalculateLayout();

	// Calling the parent' method:
	CStatic::PreSubclassWindow();

	// Removing any default text in the control to avoid the
	// control's drawing its original (not QHTML) document
	// underneath the QHTML document:
	CStatic::SetWindowText(NULL);
}

bool CVXPStatic::GetSunken()
{
	return m_bSunken;
}

void CVXPStatic::SetSunken(bool bSunken)
{
	if(m_bSunken != bSunken) // If the Sunken effect actually changes
	{
		m_bSunken = bSunken;
		UpdateControl(bSunken?false:true);
	}
}

void CVXPStatic::UpdateControl(bool bEraseBackground)
{
	if(!::IsWindow(m_hWnd))	// If the control hasn't been created yet
		return;				// Exit the method

	m_UpdateRect.left = 0;
	m_UpdateRect.top = 0;

	if(!bEraseBackground)
	{
		m_UpdateRect.right = 0;
		m_UpdateRect.bottom = 0;
		InvalidateRect(NULL, FALSE);
		return;
	}

	DWORD wndStyle = GetStyle();		// Retrieving style of the static control
										// to know how to align the output
	RECT r;
	GetClientRect(&r);					// Retrieving client rectangle of the control

	if(m_bModified)						// If QHTML of the control has been modified
	{
		// Updating the entire client area:
		InvalidateRect(&r, TRUE);
		return;
	}

	if(Width >= r.right) // If the output is too wide
		m_UpdateRect.right = r.right;
	else
		if(wndStyle & SS_CENTER)			// If center-aligned
		{
			m_UpdateRect.left = (r.right - Width) / 2;
			m_UpdateRect.right = m_UpdateRect.left + Width;
		}
		else
			if(wndStyle & SS_RIGHT)			// If right-aligned
			{
				m_UpdateRect.left = r.right - Width;
				m_UpdateRect.right = r.right;
			}
			else							// If left-aligned
				m_UpdateRect.right = Width;

	if(Height >= r.bottom)	// If the output is too high
		m_UpdateRect.bottom = r.bottom;
	else
		if(wndStyle & SS_CENTERIMAGE)		// If aligned vertically
		{
			m_UpdateRect.top = (r.bottom - Height) / 2;
			m_UpdateRect.bottom = m_UpdateRect.top + Height;
		}
		else
			m_UpdateRect.bottom = Height;

	// When Sunken effect is used the drawing area extends by 1 pixel
	// in both directions, to which we must adjust the updating rectangle:
	m_UpdateRect.right ++;
	m_UpdateRect.bottom ++;

	// Updating the area:
	InvalidateRect(&m_UpdateRect, TRUE);
}
