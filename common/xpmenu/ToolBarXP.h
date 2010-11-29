// ToolBarXP.h: interface for the CToolBarXP class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#define FRAME_TB_STYLE WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS

class CToolBarXP :	public CWindowImpl<CToolBarXP, CToolBarCtrl>,
					public CCustomDraw<CToolBarXP>										
{
	typedef CToolBarXP thisClass;
	typedef CWindowImpl<CToolBarXP, CToolBarCtrl> baseClass;

	struct _AtlToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;
		//WORD aItems[wItemCount]

		WORD* items()
		{ return (WORD*)(this+1); }
	};

public:
	DECLARE_WND_CLASS(_T("CToolBarXP"))
	BEGIN_MSG_MAP( thisClass)
	CHAIN_MSG_MAP(CCustomDraw<CToolBarXP>)
	END_MSG_MAP()

public:

#if (_WTL_VER >= 0x0700)
	BOOL m_bHandledCD;

	BOOL IsMsgHandled() const
	{
		return m_bHandledCD;
	}

	void SetMsgHandled(BOOL bHandled)
	{
		m_bHandledCD = bHandled;
	}
#endif //(_WTL_VER >= 0x0700)

    DWORD OnPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCustomDraw);
    DWORD OnItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCustomDraw);


	BOOL SubclassWindow(HWND hWnd, 
						LPCTSTR lpszResourceName, 
						COLORREF transparentColor = CLR_DEFAULT);	


	HWND Create24BitToolBarCtrl( HWND hWndParent, std::vector< int > &commands, BOOL bInitialSeparator = FALSE, DWORD style = FRAME_TB_STYLE );


protected:
	// Toolbar Helper APIs..
	HBITMAP		LoadColorBitmap(LPCTSTR lpszResourceName, HMODULE hInst, int* pNumcol);	
	BOOL		PaintHotButton(LPNMCUSTOMDRAW lpNMCustomDraw);
	void		PaintTBButton(LPNMCUSTOMDRAW pInfo);


	// Color modification related helpers..
	COLORREF	GetXpHighlightColor();
	COLORREF	MidColor(COLORREF colorA,COLORREF colorB);
	COLORREF	MixedColor(COLORREF colorA,COLORREF colorB);
	COLORREF	DarkenColorXP(COLORREF color);
	COLORREF	DarkenColor( long lScale, COLORREF lColor);
	COLORREF	LightenColor( long lScale, COLORREF lColor);
	COLORREF	MakeGrayAlphablend(HBITMAP hBitmap, int weighting, COLORREF blendcolor);
	int			NumScreenColors();
	WORD		NumBitmapColors(LPBITMAPINFOHEADER lpBitmap);	

	// implementation helpers
	void		_GetButton(int nIndex, TBBUTTON* pButton);
};
