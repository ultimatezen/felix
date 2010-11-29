// ToolBarXP.cpp: implementation of the CToolBarXP class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ToolBarXP.h"

//#include "AppTooltips.h"

DWORD CToolBarXP::OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
{
	if (*this != lpNMCustomDraw->hdr.hwndFrom)
	{
		SetMsgHandled(FALSE);
		return CDRF_DODEFAULT;
	}
	return CDRF_NOTIFYITEMDRAW;
}

DWORD CToolBarXP::OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
{
	if (*this != lpNMCustomDraw->hdr.hwndFrom)
	{
		SetMsgHandled(FALSE);
		return CDRF_DODEFAULT;
	}

	NMLVCUSTOMDRAW* lpTBCD = reinterpret_cast<NMLVCUSTOMDRAW*>( lpNMCustomDraw ) ;
	
	PaintHotButton(lpNMCustomDraw);
	PaintTBButton(&lpTBCD->nmcd);

	ATLTRACE("(%d,%d,%d,%d)\n", lpNMCustomDraw->rc.left, 
		lpNMCustomDraw->rc.top, lpNMCustomDraw->rc.right, lpNMCustomDraw->rc.bottom);
	
    // Tell Windows to paint the control itself.
    return CDRF_SKIPDEFAULT;
}

HWND CToolBarXP::Create24BitToolBarCtrl( HWND hWndParent, std::vector< int > &commands, BOOL bInitialSeparator /*= FALSE */ , DWORD style) 
{
	HINSTANCE hInst = _Module.GetResourceInstance();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(IDR_MAINFRAME), RT_TOOLBAR);
	if (hRsrc == NULL)
		return NULL;

	HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return NULL;

	_AtlToolBarData* pData = (_AtlToolBarData*)::LockResource(hGlobal);
	if (pData == NULL)
		return NULL;
	ATLASSERT(pData->wVersion == 1);

	int nItems = commands.size() + (bInitialSeparator ? 1 : 0);
	TBBUTTON* pTBBtn = (TBBUTTON*)_alloca(nItems * sizeof(TBBUTTON));

	// set initial separator (half width)
	if(bInitialSeparator)
	{
		pTBBtn[0].iBitmap = 4;
		pTBBtn[0].idCommand = 0;
		pTBBtn[0].fsState = 0;
		pTBBtn[0].fsStyle = TBSTYLE_SEP;
		pTBBtn[0].dwData = 0;
		pTBBtn[0].iString = 0;
	}

	int nBmp = 0;
	for(size_t i = 0, j = bInitialSeparator ? 1 : 0; i < commands.size(); i++, j++)
	{
		if(commands[i] != 0)
		{
			pTBBtn[j].iBitmap = nBmp++;
			pTBBtn[j].idCommand = commands[i];
			pTBBtn[j].fsState = TBSTATE_ENABLED;
			pTBBtn[j].fsStyle = TBSTYLE_BUTTON;
			pTBBtn[j].dwData = 0;
			pTBBtn[j].iString = 0;
		}
		else
		{
			pTBBtn[j].iBitmap = 8;
			pTBBtn[j].idCommand = 0;
			pTBBtn[j].fsState = 0;
			pTBBtn[j].fsStyle = TBSTYLE_SEP;
			pTBBtn[j].dwData = 0;
			pTBBtn[j].iString = 0;
		}
	}

	HWND hWnd = ::CreateWindowEx(0, TOOLBARCLASSNAME, NULL, style, 0,0,100,100,
		hWndParent, (HMENU)LongToHandle(ATL_IDW_TOOLBAR), _Module.GetModuleInstance(), NULL);

	::SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0L);

	// check if font is taller than our bitmaps
	CFontHandle font = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0L);
	if(font.IsNull())
		font = AtlGetDefaultGuiFont();
	LOGFONT lf = { 0 };
	font.GetLogFont(lf);
	WORD cyFontHeight = (WORD)abs(lf.lfHeight);

	::SendMessage(hWnd, TB_ADDBUTTONS, nItems, (LPARAM)pTBBtn);
	::SendMessage(hWnd, TB_SETBITMAPSIZE, 0, 
		MAKELONG(pData->wWidth, max(pData->wHeight, cyFontHeight)));
	const int cxyButtonMargin = 7;
	::SendMessage(hWnd, TB_SETBUTTONSIZE, 0, 
		MAKELONG(pData->wWidth + cxyButtonMargin, max(pData->wHeight, cyFontHeight) + cxyButtonMargin));

	return hWnd;
}
//------------------------------------------------------------------------------
// File    : NewToolBar.h & NewToolBar.cpp 
// Version : 1.10
// Date    : 30. November 2003
// Author  : Bruno Podetti
// Email   : Podetti@gmx.net
// Web     : www.podetti.com/NewMenu 
// Systems : VC6.0/7.0 and VC7.1 (Run under (Window 98/ME), Windows Nt 2000/XP)
//
// For bugreport please add following informations
// - The CNewToolBar version number (Example CNewToolBar 1.10)
// - Operating system Win95 / WinXP and language (English / Japanese / German etc.)
// - Intalled service packs
// - Short description how to reproduce the bug
// - Pictures/Sample are wellcome too
// - You can write in English or German to the above email-address.
// - Have my compiled examples the same effect?
//------------------------------------------------------------------------------
//
// ToDo's
// - Supporting comboboxes
// - Seperator drawing
// - Adding shade to icons
// - Correcting checked state
// - Adding text
//
// 23. November 2003 (1.10)
// - Work with Themes (manifest)
// - Expanded office 2003 look
// - Expanded ICY-Style
// - work also with 16-bit colored toolbar
//
// 12. July 2003 Version 1.0
// - Thanks to Andreas Schärer for some hints
// - It works with IE 4.0 and above
// - only a minimal implementation not supported all styles
// - support more than 16 colors for toolbarbitmaps
//
// You are free to use/modify this code but leave this header intact.
// This class is public domain so you are free to use it any of your 
// applications (Freeware, Shareware, Commercial). 
// All I ask is that you let me know so that if you have a real winner I can
// brag to my buddies that some of my code is in your app. I also wouldn't 
// mind if you sent me a copy of your application since I like to play with
// new stuff.
//------------------------------------------------------------------------------
// 
// ToolBarCtrlXP.h: interface for the CToolBarXP class.
// File    : ToolBarCtrlXP.h & ToolBarCtrlXP.cpp 
// Version : 1.0
// Date    : 10. March 2004
// Author  : Do Young Kim
// Email   : jackslack@neomtel.co.kr
// Web     : jackslack.x-y.net
// Comment : Originally developed by author above, converted into WTL version by doyoung KIM.
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COLORREF CToolBarXP::DarkenColorXP(COLORREF color)
{
  return RGB( MulDiv(GetRValue(color),7,10),
              MulDiv(GetGValue(color),7,10),
              MulDiv(GetBValue(color)+55,7,10));
}

// Function splits a color into its RGB components and
// transforms the color using a scale 0..255
COLORREF CToolBarXP::DarkenColor( long lScale, COLORREF lColor)
{ 
  long red   = MulDiv(GetRValue(lColor),(255-lScale),255);
  long green = MulDiv(GetGValue(lColor),(255-lScale),255);
  long blue  = MulDiv(GetBValue(lColor),(255-lScale),255);

  return RGB(red, green, blue); 
}

// Function splits a color into its RGB components and
// transforms the color using a scale 0..255
COLORREF CToolBarXP::LightenColor( long lScale, COLORREF lColor)
{ 
  long R = MulDiv(255-GetRValue(lColor),lScale,255)+GetRValue(lColor);
  long G = MulDiv(255-GetGValue(lColor),lScale,255)+GetGValue(lColor);
  long B = MulDiv(255-GetBValue(lColor),lScale,255)+GetBValue(lColor);

  return RGB(R, G, B); 
}

BOOL CToolBarXP::PaintHotButton(LPNMCUSTOMDRAW lpNMCustomDraw)
{
	CDCT<false> dc(lpNMCustomDraw->hdc);

	{
      if (lpNMCustomDraw->uItemState&CDIS_CHECKED ||
          lpNMCustomDraw->uItemState&CDIS_HOT)
      {
        COLORREF colorSel = GetXpHighlightColor();
        COLORREF colorBorder = GetSysColor(COLOR_HIGHLIGHT);
        dc.FillSolidRect( &(lpNMCustomDraw->rc),colorSel);
        dc.Draw3dRect(&(lpNMCustomDraw->rc),colorBorder,colorBorder);
      }
	}

	return TRUE;
}

BOOL CToolBarXP::SubclassWindow(HWND hWnd, LPCTSTR /*lpszResourceName*/, COLORREF /*transparentColor*/)
{
	BOOL bRes = baseClass::SubclassWindow( hWnd);

	ATLASSERT( m_pfnSuperWindowProc);

	return bRes;
}

HBITMAP CToolBarXP::LoadColorBitmap(LPCTSTR lpszResourceName, HMODULE hInst, int* pNumcol)
{
	if(hInst==0)
	{
		hInst = _Module.GetResourceInstance();
	}
	HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(lpszResourceName),RT_BITMAP);
	if (hRsrc == NULL)
	return NULL;

	// determine how many colors in the bitmap
	HGLOBAL hglb;
	if ((hglb = LoadResource(hInst, hRsrc)) == NULL)
	return NULL;

	LPBITMAPINFOHEADER lpBitmap = (LPBITMAPINFOHEADER)LockResource(hglb);
	if (lpBitmap == NULL)
	return NULL;
	WORD numcol = NumBitmapColors(lpBitmap);
	if(pNumcol)
	{
		*pNumcol = numcol;
	}

	UnlockResource(hglb);
	FreeResource(hglb);

	return LoadBitmap(hInst,lpszResourceName);
}

WORD CToolBarXP::NumBitmapColors(LPBITMAPINFOHEADER lpBitmap)
{
  if ( lpBitmap->biClrUsed != 0)
    return (WORD)lpBitmap->biClrUsed;
  
  switch (lpBitmap->biBitCount)
  {
  case 1:
    return 2;
  case 4:
    return 16;
  case 8:
    return 256;
  }
  return 0;
}


COLORREF CToolBarXP::GetXpHighlightColor()
{
	bool bHighContrast = false;
  if(bHighContrast)
  {
    return GetSysColor(COLOR_HIGHLIGHT);
  }

  if (NumScreenColors() > 256) 
  {
    //return DarkenColor(30,MidColor(GetSysColor(COLOR_WINDOW),GetSysColor(COLOR_HIGHLIGHT)));
    return MidColor(GetSysColor(COLOR_WINDOW),GetSysColor(COLOR_HIGHLIGHT));
  }
  return GetSysColor(COLOR_WINDOW);
}

int CToolBarXP::NumScreenColors()
{
  int nColors = 0;
  if (!nColors)
  {
    // DC of the desktop
    CClientDC myDC(NULL);
    nColors = myDC.GetDeviceCaps(NUMCOLORS);
    if (nColors == -1)
    {
      nColors = 64000;
    }
  }
  return nColors;
}  

COLORREF CToolBarXP::MidColor(COLORREF colorA,COLORREF colorB)
{ 
  // (7a + 3b)/10
  int red   = MulDiv(7,GetRValue(colorA),10) + MulDiv(3,GetRValue(colorB),10);
  int green = MulDiv(7,GetGValue(colorA),10) + MulDiv(3,GetGValue(colorB),10);
  int blue  = MulDiv(7,GetBValue(colorA),10) + MulDiv(3,GetBValue(colorB),10);

  return RGB( red, green, blue);
}

void CToolBarXP::_GetButton(int nIndex, TBBUTTON* pButton)
{	
	SendMessage(TB_GETBUTTON, nIndex, (LPARAM)pButton);	
	pButton->fsState ^= TBSTATE_ENABLED;
}

void CToolBarXP::PaintTBButton(LPNMCUSTOMDRAW pInfo)
{	
	CImageList IL((HIMAGELIST)SendMessage(TB_GETIMAGELIST, 0, 0L));
	CImageList DIL((HIMAGELIST)SendMessage(TB_GETDISABLEDIMAGELIST, 0, 0L));

	CImageList* pImgList = (pInfo->uItemState&CDIS_DISABLED) ? &DIL: &IL;	

	CDCT<false> dc(pInfo->hdc);
	CPoint ptImage(pInfo->rc.left + 3,pInfo->rc.top + 3);
	
	int nIndex = SendMessage(TB_COMMANDTOINDEX, pInfo->dwItemSpec, 0);

	UINT nID = 0;
	UINT nStyle = 0;
	int iImage = 0;

	TBBUTTON tb;
	_GetButton( nIndex, &tb);
	nID = tb.idCommand;
	nStyle = MAKELONG(tb.fsStyle, tb.fsState);
	iImage = tb.iBitmap;

	if(pInfo->uItemState&CDIS_HOT && !(pInfo->uItemState&CDIS_SELECTED))
	{
	  if( *pImgList)
	  {
		  CSize imgSize;
		  pImgList->GetIconSize( imgSize);

		  ptImage.x = pInfo->rc.left+4;
		  ptImage.y = pInfo->rc.top+4;
		  // draws the icon blended
		  HICON hDrawIcon2 = pImgList->ExtractIcon(iImage);
		  CBrush br;
		  br.CreateSolidBrush( GetSysColor(COLOR_INACTIVECAPTION));
		  dc.DrawState(ptImage, imgSize, hDrawIcon2, DSS_MONO,(HBRUSH)br);
		  DestroyIcon(hDrawIcon2);

		  ptImage.x = pInfo->rc.left+2;
		  ptImage.y = pInfo->rc.top+2;
	  }

	}

	if(*pImgList)
	{
		pImgList->Draw(pInfo->hdc, iImage, ptImage, ILD_TRANSPARENT);
		ATLTRACE("Image #%d drawn at POINT(%d, %d)\r\n", iImage, ptImage.x, ptImage.y);
	}
}

COLORREF CToolBarXP::MixedColor(COLORREF colorA,COLORREF colorB)
{
	// ( 86a + 14b ) / 100
	int red   = MulDiv(86,GetRValue(colorA),100) + MulDiv(14,GetRValue(colorB),100);
	int green = MulDiv(86,GetGValue(colorA),100) + MulDiv(14,GetGValue(colorB),100);
	int blue  = MulDiv(86,GetBValue(colorA),100) + MulDiv(14,GetBValue(colorB),100);

	return RGB( red,green,blue);
}

COLORREF CToolBarXP::MakeGrayAlphablend(HBITMAP hBitmap, int weighting, COLORREF blendcolor)
{
	CDC myDC;
	// Create a compatible bitmap to the screen
	myDC.CreateCompatibleDC(0);
	// Select the bitmap into the DC
	HBITMAP hOldBitmap = myDC.SelectBitmap(hBitmap);

	BITMAP myInfo = {0};
	::GetObject( hBitmap, sizeof(myInfo), &myInfo);

	for (int nHIndex = 0; nHIndex < myInfo.bmHeight; nHIndex++)
	{
		for (int nWIndex = 0; nWIndex < myInfo.bmWidth; nWIndex++)
		{
			COLORREF ref = myDC.GetPixel(nWIndex,nHIndex);

			// make it gray
			DWORD nAvg =  (GetRValue(ref) + GetGValue(ref) + GetBValue(ref))/3;	

			// Algorithme for alphablending
			//dest' = ((weighting * source) + ((255-weighting) * dest)) / 256
			DWORD refR = ((weighting * nAvg) + ((255-weighting) * GetRValue(blendcolor))) / 256;
			DWORD refG = ((weighting * nAvg) + ((255-weighting) * GetGValue(blendcolor))) / 256;;
			DWORD refB = ((weighting * nAvg) + ((255-weighting) * GetBValue(blendcolor))) / 256;;

			myDC.SetPixel( nWIndex, nHIndex, RGB( refR, refG, refB));
		}
	}

	COLORREF topLeftColor = myDC.GetPixel( 0, 0);
	myDC.SelectBitmap(hOldBitmap);

	return topLeftColor;
}
