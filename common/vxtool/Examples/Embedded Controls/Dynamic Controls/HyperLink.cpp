// HyperLink.cpp : implementation file for class CHyperLink
//

#include "stdafx.h"
#include "HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLink

CHyperLink::CHyperLink()
{
	m_bSunken = false;		// No sinking effect

	m_bFocusKilled = true;	// No input focus

	m_bInitalized = false;	// The object hasn't been initialized yet

	style = HLS_DEFAULT;	// Use default hyperlink style

	m_state = hlNormal;		// The hyperlink is in its normal state

	m_bTransparent = true;	// Draw the text using transparency

	m_bSizeValid = false;	// Variable m_LastSize is not valid

	m_bgColor = GetSysColor(COLOR_WINDOW); // Default text background color

// Initializing default palette:
	m_colors[hlNormal] = RGB(0, 0, 0xA0);
	m_colors[hlHighlight] = RGB(0, 0, 0xFF);
	m_colors[hlNormalVisited] = RGB(0x70, 0, 0);
	m_colors[hlHighlightVisited] = RGB(0xA0, 0, 0);
}

BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
	//{{AFX_MSG_MAP(CHyperLink)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperLink message handlers

void CHyperLink::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(HandleMouseClick())						// If handled the event correctly
	{
		CStatic::OnLButtonUp(nFlags, point);	// Call the parent handler
		SendMessage(WM_LBUTTONDOWN, 0, 0);
	}
}

void CHyperLink::OnSetFocus(CWnd * /*pOldWnd*/)
{
	m_bFocusKilled = false;
	Invalidate();
}

void CHyperLink::OnKillFocus(CWnd * /*pNewWnd*/)
{
	m_bFocusKilled = true;
	GetParent()->RedrawWindow(GetTabRect());
}

void CHyperLink::OnKeyUp(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if(nChar == VK_SPACE)
		HandleKeyEvent();
}

void CHyperLink::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if(nChar == VK_RETURN)
		HandleKeyEvent();
}

void CHyperLink::HandleKeyEvent()
{
	if(AutoVisit())					// If allow to mark the hyperlink "visited" automatically
		state = hlNormalVisited;	// Change the current state

	SendMessage(WM_LBUTTONDOWN, 0, 0);
	
	OnActivate();
}

// WM_MOUSEMOVE event handler
void CHyperLink::OnMouseMove(UINT nFlags, CPoint point)
{
	RECT r;
	GetClientRect(&r);				// Retrieving client rectangle

	if(!m_bSizeValid)				// If contents of m_LastSize is not valid
	{
		m_bSizeValid = true;		// Making it valid
		m_LastSize = GetSize();		// Updating size of the drawn contents
	}

	DWORD wndStyle = GetStyle();	// Retreiving style of the static control

	if(bHighlighted)				// If the hyperlink is highlighted
	{
		if(style & HLS_TRUNCATE_LINK)	// If supports text truncation
		{
			if(wndStyle & SS_RIGHT) // If aligned on the right
			{
				if(point.x < (r.right - m_LastSize.cx))
					point.x = -1; // Do not highlight
			}
			else
				if(wndStyle & SS_CENTER) // If aligned in the center
				{
					if(point.x < (r.right - m_LastSize.cx) / 2 || (point.x > (r.right - m_LastSize.cx) / 2 + m_LastSize.cx))
						point.x = -1; // Do not highlight
				}
				else // If aligned on the left
				{
					if(point.x > m_LastSize.cx)
						point.x = -1; // Do not highlight
				}
			if(wndStyle & SS_CENTERIMAGE) // If aligned vertically, in the middle
			{
				if(point.y < (r.bottom - m_LastSize.cy) / 2 || (point.y > (r.bottom - m_LastSize.cy) / 2 + m_LastSize.cy))
					point.y = -1; // Do not highlight
			}
			else // If aligned to the top
			{
				if(point.y > m_LastSize.cy)
					point.y = -1; // Do not highlight
			}
		}

		// Changing the highlight state
		bHighlighted = !(point.x < 0 || point.x > (r.right + 1) || point.y < 0 || point.y > (r.bottom + 1));

	}
	else // If not highlighted
	{
		bool canCapture = false;		// Do not capture the mouse

		if(style & HLS_TRUNCATE_LINK)	// If text must be trancated
		{
			if(wndStyle & SS_RIGHT)		// If aligned to the right
			{
				if(point.x >= (r.right - m_LastSize.cx))
					canCapture = true;	// Allow capturing
			}
			else
				if(wndStyle & SS_CENTER) // If aligned in the center
				{
					if(point.x >= (r.right - m_LastSize.cx) / 2 && point.x <= (r.right - m_LastSize.cx) / 2 + m_LastSize.cx)
						canCapture = true; // Allow capturing
				}
				else // If aligned to the left
				{
					if(point.x <= m_LastSize.cx)
						canCapture = true;
				}
			if(wndStyle & SS_CENTERIMAGE) // If aligned vertically, in the middle
			{
				if(point.y < (r.bottom - m_LastSize.cy) / 2 || point.y > (r.bottom - m_LastSize.cy) / 2 + m_LastSize.cy)
					canCapture = false; // Do not allow capturing
			}
			else // If aligned to the top
			{
				if(point.y > m_LastSize.cy)
					canCapture = false; // Do not allow capturing
			}
		}
		else
			canCapture = true; // Allow capturing

		if(canCapture)
			bHighlighted = true; // Changing highlighted state
	}

	CStatic::OnMouseMove(nFlags, point); // Calling the parent's method
}

void CHyperLink::OnPaint()
{
	CPaintDC dc(this);

	// Try to initialize the object
	Initialize();

	// If the object is highlighted, and the mouse has been
	// recaptured by another object we mark the object as not
	// highlighted immediately:
	if(bHighlighted && ::GetCapture() != m_hWnd)
		bHighlighted = false;

	if(!m_bSizeValid)				// If contents of m_LastSize is not valid
	{
		m_bSizeValid = true;		// Making it valid
		m_LastSize = GetSize();		// Updating size of the drawn contents
	}

	DWORD wndStyle = GetStyle();		// Retrieving style of the static control
										// to know how to align the output
	RECT r;
	GetClientRect(&r);					// Retrieving client rectangle of the control

	int x = 0;							// Assuming no shift to the right

	if(wndStyle & SS_CENTER)			// If center-aligned
		x = (r.right - m_LastSize.cx) / 2;
	else
		if(wndStyle & SS_RIGHT)			// If right-aligned
			x = r.right - m_LastSize.cx;

	if(x < 0)
		x = 0;

	int y = 0;							// Assuming no down shift

	if(wndStyle & SS_CENTERIMAGE)
		y = (r.bottom - m_LastSize.cy) / 2;

	if(y < 0)
		y = 0;

	OnDrawLink(dc, x, y); // Draw the object's contents

	if(GetFocus() == this && !m_bFocusKilled && GetStyle()&WS_TABSTOP)
	{
		r.right --;
		r.bottom --;
		OnDrawFocus(dc, r);
	}
}

BOOL CALLBACK CHyperLink::DrawStateProc(HDC hDC, LPARAM lData, WPARAM wData, int cx, int cy)
{
	CHyperLink * pObject = (CHyperLink*)lData;
	int DrawStyle = wData;
	try
	{
		CString text;
		pObject->GetWindowText(text);
		
		RECT r = {0, 0, cx, cy};

		// Drawing the shadow:
		::DrawTextEx(hDC, (LPTSTR)(LPCTSTR)text, text.GetLength(), &r, DrawStyle, NULL);

		return TRUE;
	}
	catch(...)
	{
	}
	return FALSE;
}

// This method draws the hyperlink
void CHyperLink::OnDrawLink(CDC & dc, int x, int y)
{
	CString text;
	GetWindowText(text);			// Retrieve text contents of the static object

	if(m_bTransparent)				// If must use transparency
		dc.SetBkMode(TRANSPARENT);	// Draw text using transparency
	else
	{
		dc.SetBkMode(OPAQUE);
		dc.SetBkColor(m_bgColor);
	}

	CGdiObject * pOldGdi = NULL;	// old GDI object

	if(bHighlighted)				// If the text is to be drawn highlighted
	{
		if(bVisited)	// If the hyperlink has been visited
			dc.SetTextColor(m_colors[hlHighlightVisited]);	// Selecting Highlighted Visited color
		else			// If the hyperlink hasn't been visited
			dc.SetTextColor(m_colors[hlHighlight]);			// Selecting Highlighted color

		// Selecting a font object depending on whether or not style HLS_UNDERLINE_HIGHLIGHT is used
		pOldGdi = dc.SelectObject((style&HLS_UNDERLINE_HIGHLIGHT)?(&m_highlightFont):(&m_normalFont));
	}
	else // If not highlighted
	{
		// Setting the text color
		dc.SetTextColor((style&HLS_NORMAL_VISITED && bVisited)?m_colors[hlNormalVisited]:m_colors[hlNormal]);

		// Selecting font
		pOldGdi = dc.SelectObject((style&HLS_UNDERLINE_NORMAL)?(&m_highlightFont):(&m_normalFont));
	}

	RECT r;
	GetClientRect(&r);	// Retrieving client coordinates of the control

	DWORD wndStyle = GetStyle();		// Retrieving style of the static control
										// to know how to align the output

	long DrawStyle = DT_LEFT;	// Assuming left allignment first

	if(wndStyle & SS_CENTER)	// If aligned in the center
		DrawStyle = DT_CENTER;	// Use center alignment style for the drawing API function
	else
		if(wndStyle & SS_RIGHT)	// If aligned to the right side
			DrawStyle = DT_RIGHT;

	if(wndStyle & SS_CENTERIMAGE)	// If aligned virtically, in the middle
		DrawStyle |= DT_VCENTER|DT_SINGLELINE;	// Note: Only a single-line text can be aligned
												// virtically according to Windows standard API

	DrawStyle |= DT_WORDBREAK|DT_WORD_ELLIPSIS|DT_EXPANDTABS;

	if(!IsWindowEnabled() || bSunken) // If disabled or needs to sink
	{
		// Drawing disabled hyperlink:
		::DrawState((HDC)dc, NULL, DrawStateProc, (long)this, DrawStyle, x, y, m_LastSize.cx, m_LastSize.cy, DST_COMPLEX|DSS_DISABLED);
	}
	if(IsWindowEnabled())	// If enabled:
		// Drawing the text using ellipsis when the text doesn't fit into the static rectangle.
		// Words will be broken into several lines, if the text is too long horizontally.
		// For more details about how the drawing works read MSDN, API function DrawTextEx(...)
		::DrawTextEx(dc, (LPTSTR)(LPCTSTR)text, text.GetLength(), &r, DrawStyle, NULL);

	// Unselecting the current font object to garantee it will be released
	// automatically when its destructor is called.
	dc.SelectObject(pOldGdi);
}

// This method is called when the objects needs
// to draw the focus rectangle.
// Override this method to draw your own focus rectangle.
void CHyperLink::OnDrawFocus(CDC & dc, RECT & r)
{
	LOGBRUSH info;
	info.lbStyle = BS_SOLID;
	info.lbColor = ::GetSysColor(COLOR_WINDOWFRAME);

	HPEN hPen = ::ExtCreatePen(PS_GEOMETRIC|PS_DOT, 1, &info, 0, NULL);

	HGDIOBJ hOldObj = ::SelectObject(dc, hPen);
	::MoveToEx(dc, r.left, r.top, NULL);
	::LineTo(dc, r.right, r.top);
	::LineTo(dc, r.right, r.bottom);
	::LineTo(dc, r.left, r.bottom);
	::LineTo(dc, r.left, r.top);
	::SelectObject(dc, hOldObj);
	::DeleteObject(hPen);
}

// This method calculates the updating rectangle
CRect CHyperLink::GetUpdateRect()
{
	RECT r;
	GetWindowRect(&r);	// Retrieving the client rectangle

	r.right -= r.left;
	r.bottom -= r.top;

	::ScreenToClient(GetParent()->m_hWnd, (LPPOINT)&r); // Adjusting to the screen coordinates

	r.right += r.left;
	r.bottom += r.top;

	if(style & HLS_TRUNCATE_LINK)	// Then we can optimize the update procedure to update only the area
									// taken by the drawn contents
	{
		DWORD wndStyle = GetStyle(); // Retrieving style of the static control

		if(m_LastSize.cx < (r.right - r.left - 1))	// If widht of the contents is smaller than the client area
		{
			int x = (r.right - r.left - m_LastSize.cx - 1)/2;	// Horizontal offset
			if(wndStyle & SS_CENTER)	// If aligned in the center
			{
				r.left += x;
				r.right -= x;
			}
			else
				if(wndStyle & SS_RIGHT)	// If aligned to the right
					r.left += x * 2;
				else					// If aligned to the left
					r.right -= x * 2;
		}
		if(m_LastSize.cy < (r.bottom - r.top - 1)) // If height of the contents is smaller than the client area
		{
			int y = (r.bottom - r.top - m_LastSize.cy - 1)/2; // Vertical offset
			if(wndStyle & SS_CENTERIMAGE)	// If aligned in the middle
			{
				r.top += y;
				r.bottom -= y;
			}
			else	// If aligned to the top
				r.bottom -= y * 2;
		}
	}

	// When Sunken effect is used the drawing area extends by 1 pixel
	// in both directions, to which we must adjust the updating rectangle:
	r.right ++;
	r.bottom ++;

	return r;
}

// This method calculates the updating tab rectangle
CRect CHyperLink::GetTabRect()
{
	RECT r;
	GetWindowRect(&r);	// Retrieving the client rectangle

	r.right -= r.left;
	r.bottom -= r.top;

	::ScreenToClient(GetParent()->m_hWnd, (LPPOINT)&r); // Adjusting to the screen coordinates

	r.right += r.left;
	r.bottom += r.top;

	return r;
}

// WM_LBUTTONDOWN event handler:
void CHyperLink::OnLButtonDown(UINT nFlags, CPoint point)
{
//	GetParent()->RedrawWindow(GetTabRect());
	SetFocus();
	const MSG * pMsg = GetCurrentMessage();
	if(!pMsg->lParam)						// If handled the event correctly
		CStatic::OnLButtonDown(nFlags, point);	// Call the parent handler
}

// WM_LBUTTONDBLCLK event handler:
void CHyperLink::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(HandleMouseClick())						// If handled the event correctly
		CStatic::OnLButtonDblClk(nFlags, point);// Call the parent handler
}

// Processes mouse events WM_LBUTTONUP and WM_LBUTTONDBLCLK;
// It returns True, if the object was highlighted when the event occured,
// of False otherwise.
bool CHyperLink::HandleMouseClick()
{
	const MSG * pMsg = GetCurrentMessage();
	if(!bHighlighted && pMsg->lParam)	// If the object is not being highlighted
		return false;	// The mouse click hasn't been handled properly

	if(AutoVisit())					// If allow to mark the hyperlink "visited" automatically
	{
		if(pMsg->lParam)
			state = hlHighlightVisited;	// Change the current state
		else
			state = hlNormalVisited;	// Change the current state
	}

	OnActivate();
	
	return true;	// The mouse click has been handled successfully
}

void CHyperLink::OnActivate()
{
	CString strURL = m_url;
	if (strURL.IsEmpty())		// If URL doesn't contain anything
		GetWindowText(strURL);	// Then take the static text as URL

	if(strURL.IsEmpty())		// If it is still empty
		return;					// Do nothing

	// Try to open the URL:
	int result = (int)::ShellExecute(AfxGetMainWnd()->m_hWnd, GetVerb(), strURL, GetParams(), GetDirectory(), GetShowCmd());
	if(result <= 32)	// If failed to open the URL
		OnURLError(result);
	else				// If succeeded to open the URL
		ReleaseLink();	// Release (unhighlight) the hyperlink
}

// This method retrieves a hyperlink color for
// a particular state
COLORREF CHyperLink::GetHLColor(hlState s)
{
	return m_colors[s];
}

// This method sets a hyperlink color for
// a particular state
void CHyperLink::SetHLColor(hlState s, COLORREF color, bool bRedraw)
{
	m_colors[s] = color;	// Setting new color
	if(bRedraw)				// If needs to redraw
		UpdateLink();		// Redrawing the hyperlink
}

// This method returns True, if the object has been visited,
// or False otherswise
bool CHyperLink::GetVisited()
{
	return state == hlNormalVisited || state == hlHighlightVisited;
}

// It sets "Visited" state of the object
void CHyperLink::PutVisited(bool bVis)
{
	if(bVis) // If to be set "Visited"
	{
		if(state == hlNormal)
			state = hlNormalVisited;
		else
			if(state == hlHighlight)
				state = hlHighlightVisited;
	}
	else	// If to be set "Unvisited"
	{
		if(state == hlNormalVisited)
			state = hlNormal;
		else
			if(state == hlHighlightVisited)
				state = hlHighlight;
	}
}

// Returns True, if the object is highlighted
bool CHyperLink::GetHighlighted()
{
	return state == hlHighlight || state == hlHighlightVisited;
}

// Sets the highlighted state of the object
void CHyperLink::PutHighlighted(bool bHov)
{
	if(bHov)	// If to be set "Highlighted"
	{
		if(state == hlNormal)
			state = hlHighlight;
		else
			if(state == hlNormalVisited)
				state = hlHighlightVisited;
	}
	else	// If to be set "Unhighlighted" (normal)
	{
		if(state == hlHighlight)
			state = hlNormal;
		else
			if(state == hlHighlightVisited)
				state = hlNormalVisited;
	}
}

// Sets up a new state of the object
void CHyperLink::PutState(hlState NewState)
{
	// If the state hasn't changed, or window of the object
	// doesn't exist then return
	if(m_state == NewState || !::IsWindow(m_hWnd))
		return;

	bool bWasHighlighted = bHighlighted;
	m_state = NewState;	// Setting new state

	m_bSizeValid = false;

	if(m_state == hlNormal || m_state == hlNormalVisited) // If not highlighted
	{
		if(bWasHighlighted && !bHighlighted)
		{
			ShowCursor(FALSE);	// Hide the cursor (to be shown again in method OnCaptureChanged)

			ReleaseCapture();	// Release the mouse capture

			OnHighlight(false);	// Notify derived classes that the link has been unhighlighted
		}
	}
	else	// If highlighted
	{
		SetCapture();	// Capturing all mouse events

		if(style & HLS_CHANGE_CURSOR) // If the mouse cursor must be changed
		{
			HCURSOR hCursor = GetHighlightingCursor(); // Getting the cursor handle

			if(hCursor)					// If a cursor handle was returned from a derived class
				::SetCursor(hCursor);	// Selecting the new cursor
		}

		OnHighlight(true);	// Notify derived classes that the link has been highlighted
	}
	
	// Redrawing the hyperlink:
	UpdateLink();
}

// WM_CAPTURECHANGED event handler to be called when the control
// loses its mouse capture (read MSDN for details)
void CHyperLink::OnCaptureChanged(CWnd * /*pWnd*/)
{
	ShowCursor(TRUE);	// Show the cursor
}

void CHyperLink::Initialize()
{
	if(!m_bInitalized && ::IsWindow(m_hWnd))	// If hasn't been initialized yet,
												// and the window has been created
	{
		m_bInitalized = true;		// Initialized now

		ModifyStyle(0, SS_NOTIFY);	// Modify style to make sure the parent window
									// will be notifying this control with mouse events,

		// Now taking the default font from the control, and creating its underlined
		// version:
		LOGFONT logFont;
		
		// Retrieving the font:
		CFont::FromHandle((HFONT)SendMessage(WM_GETFONT, 0, 0))->GetLogFont(&logFont);
		
		// Making sure the original font is not underlined:
		logFont.lfUnderline = FALSE;

		// Creating the normal font:
		m_normalFont.CreateFontIndirect(&logFont);

		logFont.lfUnderline = TRUE;						// Making it underlined;
		m_highlightFont.CreateFontIndirect(&logFont);	// Creating the font;
														// (this font will be destroyed automatically).

		// Notify a derived class that the control has been created
		// successfully, and it is time to do any initialization necessary:
		OnInitialUpdate();
	}
}

// This method must return size of the text output
CSize CHyperLink::GetSize()
{
	RECT r;
	GetClientRect(&r);		// Retrieving client rectangle of the control

	CString text;
	GetWindowText(text);	// Retrieving window text of the control

	CDC * dc = GetDC();		// Retriving DC of the this window

	CGdiObject * pOldGdi = dc->SelectObject(&m_normalFont);	// Selecting font

	RECT size = r;			// Destination size variable

	// Calculating the output size by using flag DT_CALCRECT (read MSDN for details):
	::DrawTextEx(*dc, (LPTSTR)(LPCTSTR)text, text.GetLength(), &size, DT_LEFT|DT_WORDBREAK|DT_WORD_ELLIPSIS|DT_CALCRECT|DT_EXPANDTABS, NULL);
	
	dc->SelectObject(pOldGdi);	// Unselecting font
	
	ReleaseDC(dc);	// Releasing DC

	return CSize(size.right, size.bottom);	// Returning the resulting size
}

// Returns supported state:
hlState CHyperLink::GetSupportedState()
{
	if(state == hlHighlight && (style & HLS_HIGHLIGHT))
		return hlHighlight;

	if(state == hlNormalVisited && (style & HLS_NORMAL_VISITED))
		return hlNormalVisited;

	if(state == hlHighlightVisited)
	{
		if(style & HLS_HIGHLIGHT_VISITED)
			return hlHighlightVisited;
		else
			if(style & HLS_HIGHLIGHT)
				return hlHighlight;
	}
	return hlNormal;
}

// We override this virtual method, because this is the only method
// called when the control is created using DDX methods
void CHyperLink::PreSubclassWindow()
{
	Initialize(); // Try to initialize the control
}

void CHyperLink::PutBGColor(COLORREF NewColor)
{
	if(NewColor != m_bgColor)
	{
		m_bgColor = NewColor;
		UpdateLink();	// Redrawing the control
	}
}

void CHyperLink::PutTransparent(bool NewVal)
{
	if(NewVal != m_bTransparent)
	{
		m_bTransparent = NewVal;
		UpdateLink();	// Redrawing the control
	}
}

void CHyperLink::UpdateLink()
{
	if(::IsWindow(m_hWnd))
	{
		// Making the parent window update its contents only within rectangle area that
		// belongs to this control (background contents of this control):
		GetParent()->RedrawWindow(GetUpdateRect());

		// Make this control redraw itself without erasing its background:
		Invalidate(FALSE);
	}
}

void CHyperLink::SetFont(CFont * pFont, BOOL bRedraw)
{
	if(!pFont) // If passed NULL
	{
		if(::IsWindow(m_hWnd))// If the control has been created
			pFont = CFont::FromHandle((HFONT)SendMessage(WM_GETFONT, 0, 0)); // Restore the original font
		else
			return;
	}

	m_bSizeValid = false; // Size of the output must be recalculated again

	// Destroying the previous font objects
	m_normalFont.DeleteObject();
	m_highlightFont.DeleteObject();

	LOGFONT logFont;
	pFont->GetLogFont(&logFont);	// Retrieving the font;
	logFont.lfUnderline = FALSE;	// Making sure the original font is not underlined;
	m_normalFont.CreateFontIndirect(&logFont);
	logFont.lfUnderline = TRUE;						// Making it underlined;
	m_highlightFont.CreateFontIndirect(&logFont);	// Creating the font;
	
	if(bRedraw)			// If asked to be redrawn immediately
		UpdateLink();	// Redraw the control
}

CFont * CHyperLink::GetFont()
{
	return &m_normalFont;
}

void CHyperLink::UpdateCursor()
{
	if(!::IsWindow(m_hWnd)) // If the hyperlink hasn't been created yet
		return;				// return from the method

	if(bHighlighted && style & HLS_CHANGE_CURSOR) // If the mouse cursor is to be changed
	{
		HCURSOR hCursor = GetHighlightingCursor(); // Getting the cursor handle

		if(hCursor)					// If a cursor handle was returned from a derived class
			::SetCursor(hCursor);	// Selecting the new cursor
		else
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));	// Selecting standard cursor
	}
}

// Releases the hyperlink:
UINT CHyperLink::ReleaseLink()
{
	if(::IsWindow(m_hWnd))		// If the hyperlink has been created
	{
		bHighlighted = false;	// Unhighlight the object
		return GetDlgCtrlID();	// Return ID of the static control
	}
	return 0;					// Return error ID
}

void CHyperLink::SetWindowText(LPCTSTR lpszString)
{
	if(::IsWindow(m_hWnd))					// If the hyperlink has been created
	{
		CStatic::SetWindowText(lpszString);	// Setting the text in the control
		ReleaseLink();						// Releasing (unhighlighting) the link
		UpdateLink();						// Redrawing the link
	}
}

bool CHyperLink::GetSunken()
{
	return m_bSunken;
}

void CHyperLink::PutSunken(bool bNewVal)
{
	if(m_bSunken != bNewVal) // If the Sunken effect actually changes
	{
		m_bSunken = bNewVal;
		UpdateLink();
	}
}
