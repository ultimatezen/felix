/*
 Copyright (c) 2000 
 Author: Ramon Smits
 E-mail: ramons@xs4all.nl
 w3    : http://www.exyll.com
 Created: 08-08-2000
 Version: 1.0.0

 Font selection combo with font preview in dropdown

 Portions Copyright:
	Girish Bharadwaj (Girish_Bharadwaj@smtpnotes.pictel.com) - http://codeguru.earthweb.com/combobox/font_selection_combo.shtml
	Norm Almond of Paramax Technology Limited (nalmond@hotmail.com) - http://codeguru.earthweb.com/combobox/fontcombo.shtml
	Konstantin Boukreev (konstantin@mail.primorye.ru)
*/
#ifndef _kFontCombo_0690d7cc_49b3_4d9a_9390_978bc2839650
#error kFontCombo.h should be included first
#endif // kFontCombo_0690d7cc_49b3_4d9a_9390_978bc2839650

#ifndef _rsFontPreviewCombo_0690d7cc_49b3_4d9a_9390_978bc2839650
#define _rsFontPreviewCombo_0690d7cc_49b3_4d9a_9390_978bc2839650

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

//#include "kFontCombo.h"

namespace WTL
{

class rsFontPreviewCombo : public kFontCombo
{	

public:	
	rsFontPreviewCombo() { }

	BEGIN_MSG_MAP(rsFontPreviewCombo)
		MESSAGE_HANDLER(WM_CREATE, OnCreate);
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem);
		MESSAGE_HANDLER(OCM_MEASUREITEM, OnMeasureItem);
		CHAIN_MSG_MAP(kFontCombo)		
		ALT_MSG_MAP(1)
			CHAIN_MSG_MAP_ALT(kFontCombo, 1)
	END_MSG_MAP()


protected:

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRet = kFontCombo::OnCreate(uMsg, wParam, lParam, bHandled);
		
		SetDroppedWidth(300);
		
		return lRet;
	}

	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		DRAWITEMSTRUCT *lpDIS = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);

		ATLASSERT(lpDIS->CtlType == ODT_COMBOBOX); 
	
		CDCHandle dc = lpDIS->hDC;		
		RECT rc = lpDIS->rcItem;
	

		if (lpDIS->itemState & ODS_FOCUS)
			dc.DrawFocusRect(&rc);
			
		int nIndexDC = dc.SaveDC();

		CBrush br;
		
		if (lpDIS->itemState & ODS_SELECTED)
		{
			br.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
			dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			br.CreateSolidBrush(dc.GetBkColor());
		}

		dc.SetBkMode(TRANSPARENT);
		dc.FillRect(&rc, br);
		

		int nLen = GetLBTextLen(lpDIS->itemID);
		TCHAR* psFont = (TCHAR *)_alloca(sizeof TCHAR * (nLen + 1));
		GetLBText(lpDIS->itemID, psFont);
		
		DWORD dwData = GetItemData(lpDIS->itemID);

		if (dwData & TRUETYPE_FONTTYPE)
			m_img.Draw(dc, 0, rc.left+5, rc.top+4,ILD_TRANSPARENT);
	
		rc.left += GLYPH_WIDTH + 2;

		CFont cf;

		cf.CreateFont(20,0,0,0,FW_NORMAL,FALSE, FALSE, FALSE,DEFAULT_CHARSET ,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH,psFont);
		HFONT hf = dc.SelectFont(cf);
		dc.TextOut(rc.left+10, rc.top, psFont);
		dc.SelectFont(hf);

		dc.RestoreDC(nIndexDC);
		return 0;
	}

	LRESULT OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		MEASUREITEMSTRUCT* pmis = reinterpret_cast<MEASUREITEMSTRUCT*>(lParam);
		ATLASSERT(pmis->CtlType == ODT_COMBOBOX);

		if (m_cyItem == -1)
		{
			// calculate height
			CClientDC dc(m_hWnd);
			HFONT hFont = ((HFONT)GetStockObject( DEFAULT_GUI_FONT ));
			dc.SelectFont( hFont ); //GetFont()
			TEXTMETRIC tm;
			dc.GetTextMetrics(&tm); 

			m_cyItem = tm.tmHeight + tm.tmInternalLeading;
		}else
			m_cyItem = 20;
		pmis->itemHeight = m_cyItem;
		return TRUE;			
	}
};

class CCoolCombo : public rsFontPreviewCombo
{
public:
	HWND CreateEx(HWND hWnd, RECT& rc, int cyItem = -1, int ID = 0)
	{
		m_cyItem = cyItem;
		return Create(hWnd, rc, 0, WS_CHILD|WS_VSCROLL|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWN|CBS_OWNERDRAWFIXED|CBS_HASSTRINGS,
			0, ID, 0);		
	}
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRet = kFlatCombo::OnCreate(uMsg, wParam, lParam, bHandled);
		HFONT hFont = ((HFONT)GetStockObject( DEFAULT_GUI_FONT ));
		SetFont(hFont);
		
		return lRet;
	}
	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		DRAWITEMSTRUCT *lpDIS = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);
		
		ATLASSERT(lpDIS->CtlType == ODT_COMBOBOX); 
		
		CDCHandle dc = lpDIS->hDC;		
		RECT rc = lpDIS->rcItem;
		
		
		if (lpDIS->itemState & ODS_FOCUS)
			dc.DrawFocusRect(&rc);
		
		int nIndexDC = dc.SaveDC();
		
		CBrush br;
		
		if (lpDIS->itemState & ODS_SELECTED)
		{
			br.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
			dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			br.CreateSolidBrush(dc.GetBkColor());
		}
		
		dc.SetBkMode(TRANSPARENT);
		dc.FillRect(&rc, br);
		
		int nLen = GetLBTextLen(lpDIS->itemID);
		TCHAR* psFont = (TCHAR *)_alloca(sizeof TCHAR * (nLen + 1));
		GetLBText(lpDIS->itemID, psFont);
		
		// Not referenced -- should it be?
		//DWORD dwData = GetItemData(lpDIS->itemID);
		
		CFont cf;
		
		dc.TextOut(rc.left, rc.top, psFont);
		
		dc.RestoreDC(nIndexDC);
		return 0;
	}
	
	LRESULT OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
	{
		MEASUREITEMSTRUCT* pmis = reinterpret_cast<MEASUREITEMSTRUCT*>(lParam);
		ATLASSERT(pmis->CtlType == ODT_COMBOBOX);

		if (m_cyItem == -1)
		{
			// calculate height
			CClientDC dc(m_hWnd);
			HFONT hFont = ((HFONT)GetStockObject( DEFAULT_GUI_FONT ));
			dc.SelectFont( hFont ); //GetFont()
			TEXTMETRIC tm;
			dc.GetTextMetrics(&tm); 

			m_cyItem = tm.tmHeight + tm.tmInternalLeading;
		}else
			m_cyItem = 20;
		pmis->itemHeight = m_cyItem;
		return TRUE;			
	}

	BEGIN_MSG_MAP(CCoolCombo)
		MESSAGE_HANDLER(WM_CREATE, OnCreate);
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem);
		MESSAGE_HANDLER(OCM_MEASUREITEM, OnMeasureItem);
		CHAIN_MSG_MAP(rsFontPreviewCombo)		
			ALT_MSG_MAP(1)
		CHAIN_MSG_MAP_ALT(rsFontPreviewCombo, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

};

}

#endif //_rsFontPreviewCombo_0690d7cc_49b3_4d9a_9390_978bc2839650

