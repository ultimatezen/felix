/////////////////////////////////////////////////////////////
// Class CHyperLink
//
// This class implements a standard HyperLink object
//
// Last Modified on March 27, 2003
//
// This library is an intellectual property of Vitaly Tomilov
//
// For more details and updates visit www.tooltips.net
//////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// Info
///////////////////////////////////////////////////////////////
//
// Class Hyperlink derived from CStatic implements a simple
// hyperlink object that can take window text of a static
// control, and display it as a hyperlink. It supports four
// possible states of the object as described in type hlState.
// A number of virual methods make it very easy to override
// behavour of this class in derived classes.
//
///////////////////////////////////////////////////////////////

#ifndef HyperLink_BaseClass
#define HyperLink_BaseClass

/////////////////////////////////////////////////////////////////////////////////////
// Hyperlink styles supported by class CHyperLink:
#define HLS_NORMAL				0x0		// Support default style(color) (always used)
#define HLS_HIGHLIGHT			0x1		// Support style(color) "hightlight"
#define HLS_NORMAL_VISITED		0x2		// Support style(color) "visited"
#define HLS_HIGHLIGHT_VISITED	0x4		// Support style(color) "highlight visited"
#define HLS_CHANGE_CURSOR		0x8		// Change cursor when highlighted
#define HLS_TRUNCATE_LINK		0x10	// Activate hyperlink only when mouse is over
										// the actual contents(text) of the object

// The following two styles to simplify the most common use of hyperlink selection
// using underlined fonts. If you want to change the font completely use method
// SetFont(...)
#define HLS_UNDERLINE_NORMAL	0x20	// Underline hyperlink when in normal state
#define HLS_UNDERLINE_HIGHLIGHT	0x40	// Underline hyperlink when highlighted

// Default style for all hyperlink objects:
#define HLS_DEFAULT				HLS_NORMAL | HLS_HIGHLIGHT | HLS_NORMAL_VISITED |\
								HLS_HIGHLIGHT_VISITED | HLS_CHANGE_CURSOR | HLS_UNDERLINE_HIGHLIGHT\
								| HLS_TRUNCATE_LINK
//////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// Four possible states for hyperlinks:
enum hlState
{
	hlNormal			= 0,	// Unvisited hyperlink, not highlighted
	hlHighlight			= 1,	// Highlighted, unvisited hyperlink
	hlNormalVisited		= 2,	// Visited hyperlink, not highlighted
	hlHighlightVisited	= 3		// Highlighted, visited hyperlink
};
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CHyperLink class
class CHyperLink : public CStatic
{
	DECLARE_MESSAGE_MAP()

public:
	// Constructor:
	CHyperLink();
	
	// Virtual destructor:
	virtual ~CHyperLink(){};

////////////////
// Methods:
////////////////

	// This method can be useful mainly in derived classes to find out what
	// state is supported by the object depending on two properties:
	// 'style' and 'state'
	hlState GetSupportedState();

	// The following two method access palette used by the object to draw
	// the hyperlink depending on its state:

	// Returns color for state 's' of the object
	COLORREF GetHLColor(hlState s);

	// Sets color for state 's' of the object
	void SetHLColor(hlState s, COLORREF color, bool bRedraw = true);

	////////////////////////////////////////////////////////////
	// The following two methods override that of class CWnd
	// in order to set up font used for hyperlinks:
	////////////////////////////////////////////////////////////

	// Sets a new font to be used for the hyperlink.
	// When pFont is NULL it sets up the default font
	// of the static control
	void SetFont(CFont * pFont, BOOL bRedraw = TRUE);

	// Returns the currently used font:
	CFont * GetFont();

	// Tries to open the URL.
	// Override this method to process the hyperlink
	// activation in a different way.
	virtual void OnActivate();

	// Updates the cursor, if the object is being highlighted,
	// and it uses style HLS_CHANGE_CURSOR.
	// It calls function GetHighlightingCursor() to get the cursor
	// handle. Of that function returns NULL the standard
	// Windows cursor is set.
	void UpdateCursor();

	// Tells the hyperlink that if it is highlighted it must
	// unhighlight immediately. Call this method when processing
	// a mouse click event that must bring up a dialog box or menu
	// just before displaying the object. When the object uses its
	// URL this method is called automatically.
	// The method returns ID of the static control. If the object
	// hasn't been created yet it does nothing, and returns 0.
	UINT ReleaseLink();

	// Performs an optimized redrawing operation for the control
	// Use this function instead of calling Invalidate or UpdateWindow
	// functions.
	void UpdateLink();

	// Sets new text in the static control. Use this function instead
	// of calling CStatic::SetWindowText directly.
	void SetWindowText(LPCTSTR lpszString);

///////////////////
// Attributes:
///////////////////

	// Hyperlink state: four possible values declared in type hlState.
	// This is a Read-Only property
	__declspec(property(get=GetState, put=PutState)) hlState state;

	// Hyperlink style: a combination of HLS_xxx flags declared above
	// When changing style of the hyperlink in a derived class it is
	// recommended to do so in the constructor of the class.
	// However, it is possible to do dynamically as well.
	__declspec(property(get=GetHLStyle, put=PutHLStyle)) long style;

	// URL of the hyperlink. When this attribute is not specified
	// the class will get text of the static control, and try to
	// use it as the URL.
	__declspec(property(get=GetURL, put=PutURL)) CString url;

	// Indicates whether the hyperlink has been visited.
	// Note: Changing value of this variable won't effect the
	// highlighted state of the hyperlink
	__declspec(property(get=GetVisited, put=PutVisited)) bool bVisited;

	// Indicates whether the mouse is being hovering over the hyperlink;
	// It is a Read-Only property.
	// Reading it's value can be useful, for instance, in a derived class
	// that handles a mouse right button click to display a context menu
	// for the hyperlink. Such handler would first make sure that
	// (bHighlighted == True) before displaying any menu.
	__declspec(property(get=GetHighlighted, put=PutHighlighted)) bool bHighlighted;

	// Contains True (default), if the text is to be displayed with
	// transparent background. When the value is False it draws background
	// using color bgColor.
	__declspec(property(get=GetTransparent, put=PutTransparent)) bool bTransparent;

	__declspec(property(get=GetSunken, put=PutSunken)) bool bSunken;

	// Background color for the font when bTransparent is False.
	__declspec(property(get=GetBGColor, put=PutBGColor)) COLORREF bgColor;

// Overrides:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperLink)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd * pOldWnd);
	afx_msg void OnKillFocus(CWnd * pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_VIRTUAL

	// Overriding this method is necessary to initialize the class,
	// if it is created using DDX methods in a dialog box.
	// Instead of overriding this method you can override method
	// OnInitialUpdate(). If you do override this method, make sure
	// you always call CHyperLink::PreSubclassWindow();
	virtual void PreSubclassWindow();

/////////////////////////////////////////////////////////
// Access methods
// There's no need in using any of these methods directly;
// Instead, use properties(attributes) declared above.
//////////////////////////////////////////////////////////
	hlState GetState(){return m_state;}
	long GetHLStyle(){return m_style;}
	void PutHLStyle(long NewStyle){m_style = NewStyle;}
	CString GetURL(){return m_url;}
	void PutURL(CString strNewURL){m_url = strNewURL;}
	bool GetVisited();
	void PutVisited(bool bVis);
	bool GetHighlighted();
	bool GetTransparent(){return m_bTransparent;};
	void PutTransparent(bool NewVal);
	COLORREF GetBGColor(){return m_bgColor;};
	void PutBGColor(COLORREF NewColor);
	void PutSunken(bool bNewVal);
	bool GetSunken();

///////////////////////////////////////////////////////////

protected:

/////////////////////////////////
// Virtual methods
/////////////////////////////////

	// Returns true, if clicking on the hyperlink will
	// make it "visited" automatically
	virtual bool AutoVisit(){return true;};

	// If style HLS_CHANGE_CURSOR is used this method is called
	// to get a handle of a mouse cursor icon depending on the state
	// of the hyperlink object.
	// Note: when overriding this method in a derived class you also
	// can analyze the value of property bVisited to support two
	// different cursors depending on whether or not the hyperlink
	// has been visited.
	virtual HCURSOR GetHighlightingCursor(){return NULL;};

	// This method must return the size of the current contents
	// of the output for the current 'state'
	virtual CSize GetSize();

	// Called when the hyperlink needs to be painted
	// depending on property 'state'
	// Override this method to draw the hyperlink any way
	// you want.
	virtual void OnDrawLink(CDC & dc, int x, int y);

	// Called when the hyperlink recieves the input focus,
	// and it wants a focus rectangle to be drawn around it.
	// Overriding this method allows to draw a custom focus rectangle.
	virtual void OnDrawFocus(CDC & dc, RECT & r);

	// This virtual method is called to notify a derived class that
	// the control has been created successfully, and it is time
	// to do any initialization necessary.
	// For instance, you can set font here, or calculate visual representation
	// of the object to save on the time to display it,...etc
	virtual void OnInitialUpdate(){};

	// This function is called every time the hyperlink
	// is highlighted. The parameter indicates whether
	// the link was highlighted or unhighlighted.
	virtual void OnHighlight(bool /*bHighlight*/){};

	// When function ::ShellExecute(...) that opens the URL
	// fails it returns a error code <= 32, which is then
	// passed to this function (read MSDN for details).
	virtual void OnURLError(int /*ErrorCode*/){};

	//////////////////////////////////////////////////////////////
	// The following four functions allow to redifine in derived
	// class all parameters passed to function ::ShellExecute(...)
	// when opening URL of the object. Read MSDN for details.
	virtual LPCTSTR GetVerb(){return NULL;}
	virtual LPCTSTR GetParams(){return NULL;}
	virtual LPCTSTR GetDirectory(){return NULL;}
	virtual int GetShowCmd(){return 0;}
	///////////////////////////////////////////////////////////////

private:

//////////////////////
// Access methods:
//////////////////////

	void PutHighlighted(bool bHov);
	void PutState(enum hlState NewState);

//////////////////////
// Other methods:
//////////////////////

	// This callback procedure is to draw the hyperlink when it is disabled
	// through function ::DrawState(...)
	static BOOL CALLBACK DrawStateProc(HDC hdc, LPARAM lData, WPARAM wData, int cx, int cy);

	// Processes mouse events WM_LBUTTONUP and WM_LBUTTONDBLCLK;
	// It returns True, if the object was highlighted when the event
	// occured, of False otherwise.
	bool HandleMouseClick();

	// Initializes the object, and calls OnInitialUpdate()
	void Initialize();

	// Returns a window rectangle that corresponds exactly to the
	// area taked by contents of the static control. It is called when updating
	// background of the control
	CRect GetUpdateRect();

	// Returns a window rectangle that corresponds exactly to the
	// area taked by the static control. It is called when updating
	// background of the control
	CRect GetTabRect();

	// Handles events from the keyboard:
	void HandleKeyEvent();

//////////////////
// Properties:
//////////////////

	CString m_url;			// Hyperlink URL

	bool m_bInitalized;		// This flag tells whether the object has been initialized

	bool m_bTransparent;	// Flag to use transparent font

	CFont m_normalFont;		// Font to be used for normal text
	CFont m_highlightFont;	// Font to be used for highlighted text

	long m_style;			// Hyperlink style

	hlState m_state;		// Hyperlink state

	COLORREF m_colors[4];	// Hyperlink palette

	CSize m_LastSize;		// Last calculated size returned by function GetSize()
	bool m_bSizeValid;		// Indicates whether contents of m_LastSize is valid,
							// or if it needs to be recalculated

	COLORREF m_bgColor;		// Background color of the text

	bool m_bFocusKilled;	// Indicates that the control is about to lose input focus

	bool m_bSunken;			// When True the hyperlink is drawn with a special effect.
};

#endif
