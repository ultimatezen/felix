#if !defined(AFX_CHOICEBAR_H__20010710_3C6D_A924_0A99_0080AD509054__INCLUDED_)
#define AFX_CHOICEBAR_H__20010710_3C6D_A924_0A99_0080AD509054__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// ChoiceBar - A Choice Bar control
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2001 Bjarke Viksoe.
//
// Add the following macro to the parent's message map:
//   REFLECT_NOTIFICATIONS()
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __cplusplus
  #error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
  #error ChoiceBar.h requires atlapp.h to be included first
#endif

#ifndef __ATLCTRLS_H__
  #error ChoiceBar.h requires atlctrls.h to be included first
#endif

#include <resource.h>

#define CHBN_FIRST               (0U-1880U)       // choice bar
#define CHBN_LAST                (0U-1899U)

#define CHBN_INITIALIZE          (CHBN_FIRST - 0)
#define CHBN_BUTTONCLICK         (CHBN_FIRST - 1)
#define CHBN_ITEMSELECTING       (CHBN_FIRST - 2)
#define CHBN_ITEMSELECTED        (CHBN_FIRST - 3)
#define CHBN_CLOSEUP             (CHBN_FIRST - 4)
#define CHBN_CUSTOMIZE           (CHBN_FIRST - 5)

// Defined in Platform SDK on > Win5
#ifndef ODS_HOTLIGHT
#define ODS_DEFAULT         0x0020
#define ODS_HOTLIGHT        0x0040
#endif

typedef struct tagNMCHOICEBAR
{
   NMHDR hdr;
   int   iItem;
} NMCHOICEBAR, FAR* LPNMCHOICEBAR;


#define CHB_EX_DEFAULT   0x00000001  // Bar has default button
#define CHB_EX_CUSTOM    0x00000002  // Bar has custom button
#define CHB_EX_TOOLTIP   0x00000004  // Tooltips for buttons enabled
#define CHB_EX_FLOAT     0x00000008  // Bar is a floating window
#define CHB_EX_FLAT      0x00000010  // Flat button look

#define CHB_ITEM_DEFAULT (int)0x4000
#define CHB_ITEM_CUSTOM  (int)0x4001

#ifndef IDS_CUSTOMIZE 
  #error define a string with IDS_CUSTOMIZE value
#endif
template <class T, class TBase = CWindow, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CChoiceBarCtrl : 
   public CWindowImpl< T, TBase, TWinTraits >
{
public:
   int   m_nButtons;    // Number of buttons
   int   m_iCurFocus;   // The hot-tracked button
   int   m_iCurSel;     // The selected button (not including DEFAULT and CUSTOM button)
   int   m_iCurBtnSel;  // The pressed button (all buttons included)
   DWORD m_dwExtStyle;  // Extended style
   //
   SIZE  m_sizeButton;  // Size of buttons
   SIZE  m_sizeMargin;  // Gap between button border and ownerdraw area
   SIZE  m_sizePadding; // Padding between buttons
   SIZE  m_sizeMatrix;  // Rows and columns defintion
   //
   CSimpleArray<RECT> m_rcButtons;
   RECT m_rcDefaultButton;
   RECT m_rcCustomButton;
   CFont m_font;
   bool m_bMouseTracked;
#ifndef _WIN32_WCE
   CToolTipCtrl m_tip;
#endif

   CChoiceBarCtrl()
   {
      m_dwExtStyle = 0;
      m_nButtons = 1;
      m_sizeMatrix.cx = m_sizeMatrix.cy = 1;
      m_sizeButton.cx = m_sizeButton.cy = 20;
      m_sizeMargin.cx = m_sizeMargin.cy = 3;
      m_sizePadding.cx = m_sizePadding.cy = 1;
   }

   BEGIN_MSG_MAP(CChoiceBarCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
      MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
      MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
#ifndef _WIN32_WCE
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
      MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
      NOTIFY_CODE_HANDLER(TTN_GETDISPINFO, OnToolTipDispInfo);
#endif
   END_MSG_MAP()

   // Operations

   int SetCurSel(int iIndex)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      int iOldSel = m_iCurSel;
      _SelectButton(iIndex, false);
      return iOldSel;
   }

   int GetCurSel() const
   {
      return m_iCurSel;
   }

   void SetExtStyle(DWORD dwStyle)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_dwExtStyle = dwStyle;
#ifndef _WIN32_WCE

      // Create tooltip if nessecary
      if( m_dwExtStyle & CHB_EX_TOOLTIP && !m_tip.IsWindow() ) {
         m_tip.Create(m_hWnd);
         ATLASSERT(m_tip.IsWindow());
         m_tip.Activate(TRUE);
      }
#endif
      // Resize control
      _AutoSize();
   }

   DWORD GetExtStyle() const
   {
      return m_dwExtStyle;
   }

   void SetButtonCount(int nButtons)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(nButtons>0);
      if( nButtons<=0 ) m_nButtons = 1;
      m_nButtons = nButtons;
      m_sizeMatrix.cy = (m_nButtons / m_sizeMatrix.cx) + (((m_sizeMatrix.cx % m_nButtons)!=0) & 1);
#ifndef _WIN32_WCE

      // Recreate tooltip buttons
      if( m_dwExtStyle & CHB_EX_TOOLTIP ) {
         TOOLINFO ti = { 0 };
         ti.cbSize = sizeof(ti);
         ti.hwnd = m_hWnd;
         int nCount = m_tip.GetToolCount();
         while( nCount-- >=0 ) {
            ti.uId = nCount;
            m_tip.DelTool(&ti);
         }
         RECT rc = { 0,0,0,0 };
         nCount = nButtons;
         while( nCount-- >=0 ) {
            ti.uId = nCount;
            ti.uFlags = TTF_SUBCLASS;
            ti.rect = rc;
            ti.lpszText = LPSTR_TEXTCALLBACK;
            m_tip.AddTool(&ti);
         }
      }
#endif
      _AutoSize();
   }

   int GetButtonCount() const
   {
      return m_nButtons;
   }

   void SetButtonSize(LONG cx, LONG cy)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(cx>0 && cy>0);
      m_sizeButton.cx = cx;
      m_sizeButton.cy = cy;
      _AutoSize();
   }

   void SetPadding(LONG cx, LONG cy)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(cx>=0 && cy>=0);
      m_sizePadding.cx = cx;
      m_sizePadding.cy = cy;
      _AutoSize();
   }

   void SetColumns(UINT nCols)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(nCols!=0);
      m_sizeMatrix.cx = nCols;
      m_sizeMatrix.cy = (m_nButtons / nCols) + (((nCols % m_nButtons)!=0) & 1);
      _AutoSize();
   }

   UINT GetColumns() const
   {
      return m_sizeMatrix.cx;
   }

   int HitTest(POINT pt) const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      for( int i=0; i<m_nButtons; i++ ) {
         if( ::PtInRect(&m_rcButtons[i], pt) ) return i;
      }
      if( ::PtInRect(&m_rcDefaultButton, pt) ) return CHB_ITEM_DEFAULT;
      if( ::PtInRect(&m_rcCustomButton, pt) ) return CHB_ITEM_CUSTOM;
      return -1;
   }

   // Implementation

   void _Init()
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ModifyStyle(0, SS_NOTIFY); // We need this for mouse-clicks

      m_iCurFocus = m_iCurSel = m_iCurBtnSel = -1;
      m_bMouseTracked = false;
#ifndef _WIN32_WCE
      //if( !m_font.IsNull() ) m_font.DeleteObject();
      //NONCLIENTMETRICS ncm;
      //ncm.cbSize = sizeof(ncm);
      //::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
      //ncm.lfMenuFont.lfHeight = 13; // To match the button size
      //m_font.CreateFontIndirect(&ncm.lfMenuFont);
#endif
      m_font = AtlGetDefaultGuiFont();
      NMHDR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_INITIALIZE };
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
   }

   void _AutoSize()
   {
      T* pT = static_cast<T*>(this);
      pT->UpdateLayout();
   }

   void _InvalidateRect(int iIndex)
   {
      if( iIndex>=0 && iIndex<m_nButtons ) {
         InvalidateRect(&m_rcButtons[iIndex]);
      }
      else if( (iIndex==CHB_ITEM_DEFAULT) && (m_dwExtStyle & CHB_EX_DEFAULT) ) {
         InvalidateRect(&m_rcDefaultButton);
      }
      else if( (iIndex==CHB_ITEM_CUSTOM) && (m_dwExtStyle & CHB_EX_CUSTOM) ) {
         InvalidateRect(&m_rcCustomButton);
      }
   }

   void _DrawButton(HDC hdc, RECT &rc, int i, DRAWITEMSTRUCT &dis) const
   {
      bool bSelected = (i==m_iCurSel) || (i==m_iCurBtnSel);
      bool bFocus = i==m_iCurFocus;
      // Draw borders
      if( m_dwExtStyle & CHB_EX_FLAT ) {
         if( bSelected || bFocus ) {
            DWORD hiCol = ::GetSysColor(!bSelected ? COLOR_3DHIGHLIGHT : COLOR_3DSHADOW); 
            DWORD loCol = ::GetSysColor(bSelected ? COLOR_3DHIGHLIGHT : COLOR_3DSHADOW); 
            CDCHandle dc(hdc);
            dc.Draw3dRect(&rc, hiCol, loCol);
         }
      }
      else {
         UINT iFlags = 0;
         if( bSelected ) iFlags = EDGE_SUNKEN;
         else if( bFocus ) iFlags = EDGE_RAISED;
         if( iFlags!=0 ) ::DrawEdge(hdc, &rc, iFlags, BF_RECT);
#ifndef _WIN32_WCE
         iFlags = DFCS_BUTTONPUSH | DFCS_FLAT;
#else
         iFlags = DFCS_BUTTONPUSH ;
#endif
         if( i==m_iCurSel ) iFlags |= DFCS_PUSHED;
         ::DrawFrameControl(hdc, &rc, DFC_BUTTON, iFlags);
      }
      // Shrink rectangle so it doesn't include borders
      ::InflateRect(&rc, -m_sizeMargin.cx, -m_sizeMargin.cy);
      // Update DRAWITEMSTRUCT
      dis.itemID = i;
      dis.rcItem = rc;
      dis.itemAction = ODA_SELECT|ODA_FOCUS;
      dis.itemState = 0;
      if( i==CHB_ITEM_DEFAULT ) dis.itemState |= ODS_DEFAULT;
      if( bSelected ) dis.itemState |= ODS_SELECTED;
      if( bFocus ) dis.itemState |= ODS_HOTLIGHT;
   }

   void _SelectButton(int iIndex, bool bPostProcess=true)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(::IsWindow(GetParent()));
      // Custom button selected?
      if( iIndex==CHB_ITEM_CUSTOM ) {
         NMHDR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_CUSTOMIZE };
         ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
         return;
      }
      // Ask parent
      NMCHOICEBAR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_ITEMSELECTING, iIndex };
      if( ::SendMessage(GetParent(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh)!=0 ) return;;
      // Update buttons
      _InvalidateRect(iIndex);
      if( iIndex>=0 && iIndex<m_nButtons ) {
         _InvalidateRect(m_iCurBtnSel);
         _InvalidateRect(m_iCurSel);
         _InvalidateRect(CHB_ITEM_DEFAULT); // The Default button is likely to reflect the selected button
         m_iCurSel = m_iCurBtnSel = iIndex;
      }
      // Notify parent
      nmh.hdr.code = CHBN_ITEMSELECTED;
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
      if( bPostProcess ) {
         // Destroy bar?
         if( (m_dwExtStyle & CHB_EX_FLOAT)==0 ) PostMessage(WM_CLOSE);
      }
   }

   UINT _GetDlgCtrlID() const
   {
      // HACK: WTL messaging requires a control ID
      //       and Window Identifiers are not officially supported by
      //       top-level windows. We need to fake one.
      UINT ID = GetDlgCtrlID();
      if( ID==0 ) ID = 1234; 
      return ID;
   }

   // Message Handlers

   LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
      _Init();
      return lRes;
   }

   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      NMHDR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_CLOSEUP };
      ::SendMessage(GetParent(), WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Click outside a bar and it closes...
      if( (m_dwExtStyle & CHB_EX_FLOAT)==0 ) PostMessage(WM_CLOSE);
      ::SetFocus(GetParent());
      return 0;
   }

   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CDCHandle dc((HDC)wParam);
      RECT rc;
      GetClientRect(&rc);
#ifndef _WIN32_WCE
      dc.FillRect(&rc, (HBRUSH)(COLOR_BTNFACE+1));
#else
      dc.FillRect(&rc, (HBRUSH)(COLOR_BTNFACE));
#endif

      return 1;
   }

   LRESULT OnGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      return DefWindowProc(uMsg, wParam, lParam) | DLGC_WANTARROWS;
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      T* pT = static_cast<T*>(this);
      if( wParam != NULL ) {
         pT->DoPaint((HDC)wParam);
      }
      else {
         PAINTSTRUCT m_ps;
         HDC hDC = ::BeginPaint(m_hWnd, &m_ps);
         pT->DoPaint(hDC);
      }
      return 0;
   }

   LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      int iIndex = HitTest(pt);
      if( iIndex==-1 ) return 0;
      // Update button to show as pressed
      _InvalidateRect(iIndex);
      _InvalidateRect(m_iCurBtnSel);
      m_iCurBtnSel = iIndex;
      // Capture the mouse so we can be sure to get WM_LBUTTONUP
      SetCapture();
      return 0;
   }

   LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      if( ::GetCapture()==m_hWnd ) ReleaseCapture();

      int iCurBtnSel = m_iCurBtnSel;
      _InvalidateRect(m_iCurBtnSel);
      m_iCurBtnSel = -1;
      
      POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      int iIndex = HitTest(pt);
      if( iCurBtnSel==iIndex ) {
         // Notify parent
         NMCHOICEBAR nmh = { m_hWnd, _GetDlgCtrlID(), CHBN_BUTTONCLICK, iIndex };
         ::SendMessage(GetParent(), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM)&nmh);
         // Select item
         _SelectButton(iIndex);
      }
      return 0;
   }

   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      if( ::GetCapture()==m_hWnd ) return 0; // Not as long as we are tracking

      POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      int iIndex = HitTest(pt);
      if( iIndex != m_iCurFocus ) {
         _InvalidateRect(iIndex);
         _InvalidateRect(m_iCurFocus);
         m_iCurFocus = iIndex;
      }

#ifndef _WIN32_WCE
  if( !m_bMouseTracked ) {
         TRACKMOUSEEVENT tme = { 0 };
         tme.cbSize = sizeof(tme);
         tme.hwndTrack = m_hWnd;
         tme.dwFlags = TME_LEAVE;
         tme.dwHoverTime = HOVER_DEFAULT;
         _TrackMouseEvent(&tme);
         m_bMouseTracked = true;
      } 
#endif
      return 0;
   }

   LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      m_bMouseTracked = false;
      _InvalidateRect(m_iCurFocus);
      m_iCurFocus = -1;
      return 0;
   }

   LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      switch( wParam ) {
      case VK_RETURN:
      case VK_SPACE:
         _SelectButton(m_iCurBtnSel);
         return 0;
      case VK_ESCAPE:
         if( (m_dwExtStyle & CHB_EX_FLOAT)==0 ) PostMessage(WM_CLOSE);
         return 0;
      case VK_LEFT:
      case VK_RIGHT:
      case VK_UP:
      case VK_DOWN:
         // TODO: Add keyboard navigation
         return 0;
      }
      return 0;
   }

   LRESULT OnToolTipDispInfo(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      // Forward notification to my owner instead
      pnmh->hwndFrom = m_hWnd;
      pnmh->idFrom = _GetDlgCtrlID();
      return ::SendMessage(GetParent(), WM_NOTIFY, pnmh->idFrom, (LPARAM)pnmh);
   }

   // Overridables

   void DoPaint(HDC hdc)
   {
      HWND hwndParent = GetParent();
      ATLASSERT(::IsWindow(hwndParent));
      UINT iDlgCtrl = _GetDlgCtrlID();

	  CRect rc;
	  GetClientRect(rc);
	  InflateRect(rc,-1,-1);
	  Rectangle(hdc,rc.left,rc.top,rc.right,rc.bottom);
      // Save current DC selections
      int save = ::SaveDC(hdc);
      ATLASSERT(save!=0);

      // Prepare DC
      ::SelectObject(hdc, m_font);

      DRAWITEMSTRUCT dis = { 0 };
      dis.CtlType = ODT_BUTTON;
      dis.CtlID = iDlgCtrl;
      dis.hwndItem = m_hWnd;
      dis.hDC = hdc;

      if( m_dwExtStyle & CHB_EX_DEFAULT ) {
         RECT rc = m_rcDefaultButton;
         _DrawButton(hdc, rc, CHB_ITEM_DEFAULT, dis);
         ::SendMessage(hwndParent, WM_DRAWITEM, iDlgCtrl, (LPARAM)&dis);
      }
      int i=0;
      while( i<m_nButtons ) {
         RECT rc = m_rcButtons[i];
         _DrawButton(hdc, rc, i, dis);
         ::SendMessage(hwndParent, WM_DRAWITEM, iDlgCtrl, (LPARAM)&dis);
         i++;
      }
      if( m_dwExtStyle & CHB_EX_CUSTOM ) {
         RECT rc = m_rcCustomButton;
         _DrawButton(hdc, rc, CHB_ITEM_CUSTOM, dis);
         ::SendMessage(hwndParent, WM_DRAWITEM, iDlgCtrl, (LPARAM)&dis);
      }

      ::RestoreDC(hdc, save);
   }

   void UpdateLayout()
   {
      SIZE sizeInset = { 4, 4 };
      POINT ptOrg = { sizeInset.cx, sizeInset.cy };
      POINT pt = ptOrg;
      int cx = 0;
      int cy = 0;
      int iLargeButtonPadding = (m_sizePadding.cy>3)?m_sizePadding.cy:3;

      if( m_dwExtStyle & CHB_EX_DEFAULT ) {
         int iHeight = m_sizeButton.cy + (m_sizeMargin.cy*2);
         RECT rc = { ptOrg.x, ptOrg.y, ptOrg.x, ptOrg.y + iHeight };
         m_rcDefaultButton = rc;
         pt.y += iHeight + iLargeButtonPadding;
         cy += iHeight + iLargeButtonPadding;
      }

      m_rcButtons.RemoveAll();
      int i=0;
      int nRow=0;
      while( i<m_nButtons ) {
         RECT rc = { pt.x, pt.y, pt.x + m_sizeButton.cx, pt.y + m_sizeButton.cy };
         m_rcButtons.Add(rc);
         i++;
         pt.x += m_sizeButton.cx + m_sizePadding.cx;
         if( nRow==0 ) cx += m_sizeButton.cx + m_sizePadding.cx;
         if( (i<m_nButtons) && ((i % m_sizeMatrix.cx )==0) ) {
            pt.x = ptOrg.x;
            pt.y += m_sizeButton.cy + m_sizePadding.cy;
            cy += m_sizeButton.cy + m_sizePadding.cy;
            nRow++;
         }
      }
      pt.y += m_sizeButton.cy;
      cy += m_sizeButton.cy;
      cx -= m_sizePadding.cx;

      if( m_dwExtStyle & CHB_EX_DEFAULT ) {
         m_rcDefaultButton.right += cx;
      }
      if( m_dwExtStyle & CHB_EX_CUSTOM ) {
         pt.y += iLargeButtonPadding;
         RECT rc = { ptOrg.x, pt.y, ptOrg.x + cx, pt.y + m_sizeButton.cy };
         m_rcCustomButton = rc;
         cy += m_sizeButton.cy + iLargeButtonPadding;
      }
      cy += m_sizePadding.cy;

      // Resize window according to the new (cx,cy)
      RECT rcWin;
      GetWindowRect(&rcWin);
      rcWin.right = rcWin.left + cx + (sizeInset.cx*2);
      rcWin.bottom = rcWin.top + cy + (sizeInset.cy*2);
      ::AdjustWindowRectEx(&rcWin, GetStyle(), FALSE, GetExStyle());
      SetWindowPos(HWND_TOP, &rcWin, SWP_NOMOVE);

      // Set button rectangles for tooltip
#ifndef _WIN32_WCE
      if( m_dwExtStyle & CHB_EX_TOOLTIP && m_tip.IsWindow() ) {
         TOOLINFO ti = { 0 };
         ti.cbSize = sizeof(ti);
         ti.hwnd = m_hWnd;
         for( int i=0; i<m_nButtons; i++ ) {
            ti.uId = i;
            ti.rect = m_rcButtons[i];
            m_tip.SetToolRect(&ti);
         }
      } 
#endif
   }

};


/////////////////////////////////////////////////////////////////////////////
//
// The sample Choice Bar controls
//
// The follwing samples derive directly from CChoiceBarCtrl.
// This means that they can actually use the internal members
// of this class. But they will not! To keep the code clean, I'm only
// using public member methods.
//

class CLineStyleCtrl : 
   public CChoiceBarCtrl<CLineStyleCtrl>,
   public COwnerDraw<CLineStyleCtrl>
{
public:
   DECLARE_WND_CLASS(_T("WTL_LineStyle"))

   BEGIN_MSG_MAP(CLineStyleCtrl)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_INITIALIZE, OnInitialize)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_ITEMSELECTED, _OnItemSelected)

      CHAIN_MSG_MAP(CChoiceBarCtrl<CLineStyleCtrl>)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CLineStyleCtrl>, 1)
      DEFAULT_REFLECTION_HANDLER()
   END_MSG_MAP()

   LRESULT OnInitialize(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {
      SetExtStyle(CHB_EX_FLAT);
#ifndef _WIN32_WCE
      SetButtonCount(5);
#else
      SetButtonCount(2);
#endif
      SetColumns(1);
      SetButtonSize(120,18);
      return 0;
   }
   virtual void OnItemSelected(int pattern)
   {
   }

   LRESULT _OnItemSelected(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      //if (!m_init)  
           //OnItemSelected(m_colors[GetCurSel()]); 
	  OnItemSelected(GetCurSel());
      return 0;
   }


   void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
   {
      CDCHandle dc(lpDrawItemStruct->hDC);
      RECT &rc = lpDrawItemStruct->rcItem;

      dc.SetBkMode(TRANSPARENT);
      int iStyle = 0;
      switch( lpDrawItemStruct->itemID ) {
      case 0:
         iStyle = PS_SOLID; break;
      case 1:
         iStyle = PS_DASH; break;
#ifndef _WIN32_WCE
      case 2:
         iStyle = PS_DOT; break;
      case 3:
         iStyle = PS_DASHDOT; break;
      case 4:
         iStyle = PS_DASHDOTDOT; break;
      default:
         ATLASSERT(false);
#endif
      }
      // Create pen with style.
      // Styled pens are only supported at a 1-pixel size.
      CPen pen;
      pen.CreatePen(iStyle, 1, ::GetSysColor(COLOR_BTNTEXT));
      HPEN hOldPen = dc.SelectPen(pen);
      int y = rc.top + (((rc.bottom-rc.top)/2)-1);
      for( int i=0; i<3; i++ ) {
         dc.MoveTo(rc.left+3, y);
         dc.LineTo(rc.right-3, y);
         y++;
      }
      dc.SelectPen(hOldPen);
   }
};




class CBorderStyleCtrl : 
   public CChoiceBarCtrl<CBorderStyleCtrl>,
   public COwnerDraw<CBorderStyleCtrl>
{
public:
   DECLARE_WND_CLASS(_T("WTL_BorderStyle"))

   CBitmap m_bmDotted;
   CBrush  m_brDotted;

   BEGIN_MSG_MAP(CBorderStyleCtrl)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_INITIALIZE, OnInitialize)
      CHAIN_MSG_MAP(CChoiceBarCtrl<CBorderStyleCtrl>)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CBorderStyleCtrl>, 1)
      DEFAULT_REFLECTION_HANDLER()
   END_MSG_MAP()

   LRESULT OnInitialize(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {
      WORD bitmapbits[8] = { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA };
      if( !m_bmDotted.IsNull() ) m_bmDotted.DeleteObject();
      m_bmDotted.CreateBitmap(8,8,1,1, bitmapbits);
      if( !m_brDotted.IsNull() ) m_brDotted.DeleteObject();
      m_brDotted.CreatePatternBrush(m_bmDotted);

      SetExtStyle(CHB_EX_FLAT);
      SetButtonCount(10);
      SetColumns(5);
      SetButtonSize(19,19);
      return 0;
   }

   void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
   {
      CDCHandle dc(lpDrawItemStruct->hDC);
      RECT &rc = lpDrawItemStruct->rcItem;
      rc.right--;
      rc.bottom--;

      LOGBRUSH lb;
      ::GetObject(m_brDotted, sizeof(LOGBRUSH), &lb);
      CPen penDash;
      penDash.CreatePen(PS_DASH, 1, &lb, 0, NULL);
      CPen penSolid;
      penSolid.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNTEXT));

      dc.SetBkMode(TRANSPARENT);      
      dc.SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
      dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
      HPEN hOldPen = dc.SelectPen(penDash);
      int midx = rc.left + ((rc.right-rc.left)/2);
      int midy = rc.top + ((rc.bottom-rc.top)/2);
      //dc.Rectangle(&rc); // Requires a selected background brush so we don't bother right now
      dc.MoveTo(rc.left, rc.top);
      dc.LineTo(rc.right, rc.top);
      dc.LineTo(rc.right, rc.bottom);
      dc.LineTo(rc.left, rc.bottom);
      dc.LineTo(rc.left, rc.top);
      dc.MoveTo(midx, rc.top);
      dc.LineTo(midx, rc.bottom);
      dc.MoveTo(rc.left, midy);
      dc.LineTo(rc.right, midy);

      int iBits = 0; // bit 0..5: left, top, right, bottom, vertical, horizontal
      switch( lpDrawItemStruct->itemID ) {
      case 0:
         iBits = 0x0F; break;
      case 1:
         iBits = 0x02; break;
      case 2:
         iBits = 0x08; break;
      case 3:
         iBits = 0x04; break;
      case 4:
         iBits = 0x01; break;
      case 5:
         iBits = 0x3F; break;
      case 6:
         iBits = 0x30; break;
      case 7:
         iBits = 0x20; break;
      case 8:
         iBits = 0x10; break;
      case 9:
         iBits = 0x00; break;
      default:
         ATLASSERT(false);
      }

      // Draw a line for each of the enabled bits/line segments
      // (remembering that LineTo() doesn't include the end-point)
      dc.SelectPen(penSolid);
      if( iBits & 0x01 ) {
         dc.MoveTo(rc.left, rc.top);
         dc.LineTo(rc.right+1, rc.top);
      }
      if( iBits & 0x02 ) {
         dc.MoveTo(rc.right, rc.top);
         dc.LineTo(rc.right, rc.bottom+1);
      }
      if( iBits & 0x04 ) {
         dc.MoveTo(rc.right, rc.bottom);
         dc.LineTo(rc.left-1, rc.bottom);
      }
      if( iBits & 0x08 ) {
         dc.MoveTo(rc.left, rc.bottom);
         dc.LineTo(rc.left, rc.top-1);
      }
      if( iBits & 0x10 ) {
         dc.MoveTo(midx, rc.top);
         dc.LineTo(midx, rc.bottom+1);
      }
      if( iBits & 0x20 ) {
         dc.MoveTo(rc.left, midy);
         dc.LineTo(rc.right+1, midy);
      }

      dc.SelectPen(hOldPen);
   }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////

class CPaletteSelectCtrl : 
   public CChoiceBarCtrl<CPaletteSelectCtrl>,
   public COwnerDraw<CPaletteSelectCtrl>
{
public:
   DECLARE_WND_CLASS(_T("WTL_PaletteSelectCtrl"))

   enum { NUM_COLORS = 24 };
   PALETTEENTRY m_colors[NUM_COLORS];

   BEGIN_MSG_MAP(CPaletteSelectCtrl)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_INITIALIZE, OnInitialize)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_ITEMSELECTED, _OnItemSelected)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_CUSTOMIZE, _OnCustomize)


      CHAIN_MSG_MAP(CChoiceBarCtrl<CPaletteSelectCtrl>)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CPaletteSelectCtrl>, 1)
      DEFAULT_REFLECTION_HANDLER()
   END_MSG_MAP()

   COLORREF GetColor(int iIndex) const
   {
      ATLASSERT(iIndex>=0 && iIndex<NUM_COLORS);
      if( iIndex<0 || iIndex>=NUM_COLORS ) return RGB(0,0,0); 
      return RGB(m_colors[iIndex].peRed, m_colors[iIndex].peGreen, m_colors[iIndex].peBlue);
   }
   
   void SetColor(COLORREF c)
   {
     for (int i=0;i< NUM_COLORS;++i)
        if (m_colors[i].peRed == GetRValue(c) && m_colors[i].peBlue == GetBValue(c) && m_colors[i].peGreen == GetGValue(c))
          {SetCurSel(i);break;
          }
   }
   virtual void OnItemSelected(PALETTEENTRY color)
   {
   }

   LRESULT _OnItemSelected(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      
      if (!m_init)  
           OnItemSelected(m_colors[GetCurSel()]); 
      return 0;
   }

   LRESULT _OnCustomize(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      
#ifndef _WIN32_WCE
      CColorDialog c(0,0,GetParent());
	  c.DoModal();
	  DWORD ret = CommDlgExtendedError();
	  COLORREF color = c.GetColor();
      m_colors[GetCurSel()].peRed   = GetRValue(color);
      m_colors[GetCurSel()].peGreen = GetGValue(color);
      m_colors[GetCurSel()].peBlue  = GetBValue(color);
	 m_colors[GetCurSel()].peFlags        = 0;//PC_NOCOLLAPSE;
	  OnItemSelected(m_colors[GetCurSel()]);
#endif
	  return 0;
   }


	 void PalValue(BYTE index,BYTE r,BYTE g,BYTE b)
	{
	 m_colors[index].peRed   = r;
	 m_colors[index].peGreen = g;
	 m_colors[index].peBlue  = b;
	 m_colors[index].peFlags = 0;//PC_NOCOLLAPSE;
	}

   LRESULT OnInitialize(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {
      m_init = true;
      HPALETTE hPal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
      ATLASSERT(hPal!=NULL);
      ::GetPaletteEntries(hPal, 0, NUM_COLORS, m_colors);
#ifndef _WIN32_WCE
      SetExtStyle(CHB_EX_DEFAULT|CHB_EX_CUSTOM|CHB_EX_FLAT);
#else
      SetExtStyle(CHB_EX_DEFAULT|CHB_EX_FLAT);
	  PalValue(0,   0,   0,   0);
	  PalValue(1, 128,   0,   0);
	  PalValue(2,   0, 128,   0);
	  PalValue(3, 128, 128,   0);
	  PalValue(4,   0,   0, 128);
	  PalValue(5, 128,   0, 128);
	  PalValue(6,   0, 128, 128);
	  PalValue(7, 192, 192, 192);
	  PalValue(8, 192, 220, 192);
	  PalValue(9, 160, 200, 240);
	  PalValue(10, 255, 255, 255);
	  PalValue(11, 153, 153, 153);
	  PalValue(12, 153, 153, 153);
	  PalValue(13, 255,   0,   0);
	  PalValue(14,   0, 255,   0);
	  PalValue(15, 255, 255,   0);
	  PalValue(16,   0,   0, 255);
	  PalValue(17, 255,   0, 255);
	  PalValue(18,   0, 255, 255);
	  PalValue(19,   0, 255, 255);
	  PalValue(20,   0, 255, 255);
	  PalValue(21,   0, 255, 255);
	  PalValue(22,   0, 255, 255);
	  PalValue(23, 255, 255, 255);
#endif
      SetButtonCount(NUM_COLORS);
      SetButtonSize(18,18);
      SetPadding(0,0);
      SetColumns(6);
      SetCurSel(0);
      m_init = false;
      return 0;
   }

   void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
   {
      CDCHandle dc(lpDrawItemStruct->hDC);
      int iIndex = (int)lpDrawItemStruct->itemID;
      RECT &rc = lpDrawItemStruct->rcItem;

      if( iIndex >= 0 && iIndex<GetButtonCount() ) {
         ATLASSERT(GetButtonCount()<=NUM_COLORS);
         dc.FrameRect(&rc, (HBRUSH)::GetStockObject(GRAY_BRUSH));
         ::InflateRect(&rc, -1, -1);
         dc.FillSolidRect(&rc, GetColor(iIndex));
      }
      else if( iIndex==CHB_ITEM_DEFAULT ) {
         dc.FrameRect(&rc, (HBRUSH)::GetStockObject(GRAY_BRUSH));
         ::InflateRect(&rc, -3, -3);
         int i = GetCurSel();
         ATLASSERT(i>=0);
         dc.FillSolidRect(&rc, GetColor(i));
      }
      else if( iIndex==CHB_ITEM_CUSTOM ) {
         // If selected, move the text down a bit
         if( lpDrawItemStruct->itemState & ODS_SELECTED ) {
            rc.top += 2;
            rc.left += 2;
         }
         // Load button caption from resource
         TCHAR szText[64];
         ::LoadString(_Module.GetResourceInstance(), IDS_CUSTOMIZE, szText, sizeof(szText)/sizeof(TCHAR));
         dc.SetBkMode(TRANSPARENT);
         dc.DrawText(szText, -1, &rc, DT_CENTER | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);
      }
   }
private :
   bool m_init;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(IDC_SIZE_LR)
#pragma message("warning : define IDB_HATCH1 to IDB_HATCH1+25 patterns")
#define IDB_HATCH1 0
#endif

class CFillStyleCtrl : 
   public CChoiceBarCtrl<CFillStyleCtrl>,
   public COwnerDraw<CFillStyleCtrl>
{
public:
   DECLARE_WND_CLASS(_T("WTL_FillStyle"))

   BEGIN_MSG_MAP(CFillStyleCtrl)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_INITIALIZE, OnInitialize)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_ITEMSELECTED, _OnItemSelected)

      CHAIN_MSG_MAP(CChoiceBarCtrl<CFillStyleCtrl>)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CFillStyleCtrl>, 1)
      DEFAULT_REFLECTION_HANDLER()			   
   END_MSG_MAP()

   LRESULT OnInitialize(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {
     SetExtStyle(CHB_EX_DEFAULT|CHB_EX_FLAT);
     SetButtonCount(25);
     SetColumns(5);
     SetButtonSize(20,20);
     return 0;
   }

   virtual void OnItemSelected(int pattern)
   {
   }

   LRESULT _OnItemSelected(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      //if (!m_init)  
           //OnItemSelected(m_colors[GetCurSel()]); 
	  OnItemSelected(GetCurSel());
      return 0;
   }

   static void LocFillRect(HDC dc,const CRect &rc,const int iIndex)
   {
	  CBrush brush;
#ifndef _WIN32_WCE
      long iStyle = -1;
      switch(iIndex) 
	  {
      case 1:
         iStyle = HS_BDIAGONAL; break;
      case 2:
         iStyle = HS_CROSS;     break;
      case 3:
         iStyle = HS_DIAGCROSS; break;
      case 4:
         iStyle = HS_FDIAGONAL; break;
      case 5:
         iStyle = HS_HORIZONTAL; break;
      case 6:
         iStyle = HS_VERTICAL;  break;
      }   

    // Create brush with style.
      if (iIndex < 7 )
		brush.CreateHatchBrush(iStyle,RGB(0,0,0));
	  else
#else
	  if (iIndex < 6)								  //map all standard to solid
		brush.CreateSolidBrush(RGB(255,255,255));
	  else
#endif
	  {
		CBitmap b;
		b.LoadBitmap(IDB_HATCH1+iIndex-6);
        brush.CreatePatternBrush(b);
	  }
      FillRect(dc,&rc,brush);
   }

   void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
   {
      CDCHandle dc(lpDrawItemStruct->hDC);
      RECT &rc    = lpDrawItemStruct->rcItem;
	  int  bIndex = lpDrawItemStruct->itemID;
	  int  iIndex = bIndex == CHB_ITEM_DEFAULT? GetCurSel(): bIndex;
      dc.SetBkMode(TRANSPARENT);
      dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));

      if( bIndex  >= 0 && bIndex < GetButtonCount() ) 
	  {
		dc.Rectangle(&rc);
		::InflateRect(&rc, -1, -1);
		LocFillRect(dc,rc,iIndex);
	  }
      else if( bIndex == CHB_ITEM_DEFAULT ) 
	  {
        dc.FrameRect(&rc, (HBRUSH)::GetStockObject(GRAY_BRUSH));
        ::InflateRect(&rc, -3, -3);
		Rectangle(dc,rc.left,rc.top,rc.right,rc.bottom);
        ::InflateRect(&rc, -1, -1);
        LocFillRect(dc,rc,iIndex);
      }
   }
};


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSymbolCtrl : 
   public CChoiceBarCtrl<CSymbolCtrl>,
   public COwnerDraw<CSymbolCtrl>
{
private :
	HFONT   m_font;
	CString m_font_name;

public:
   CSymbolCtrl(const CString &font_name):m_font_name(font_name){	}
   DECLARE_WND_CLASS(_T("WTL_Symbol"))

   BEGIN_MSG_MAP(CSymbolCtrl)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_INITIALIZE,    OnInitialize)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_ITEMSELECTED, _OnItemSelected)

      CHAIN_MSG_MAP(CChoiceBarCtrl<CSymbolCtrl>)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CSymbolCtrl>, 1)
      DEFAULT_REFLECTION_HANDLER()			   
   END_MSG_MAP()

   LRESULT OnInitialize(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {
     SetExtStyle(CHB_EX_DEFAULT|CHB_EX_FLAT);
     SetButtonCount(49);
     SetColumns(7);
     SetButtonSize(30,30);
	 m_font          = 0;
     return 0;
   }

   virtual void OnItemSelected(int pattern)
   {
   }

   LRESULT _OnItemSelected(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
	  OnItemSelected(GetCurSel());
      return 0;
   }

   static HFONT CreateFont(HDC dc,long height,CString & font_name,long angle=0,bool bold=false,bool italic=false,bool underline=false,bool strikeout=false,bool opaque = false)
    {
      LOGFONT lf;
      memset(&lf,0,sizeof(lf));
      lf.lfWidth         = 0;
      lf.lfWeight        = bold?700:400;
      lf.lfItalic        = italic;
      lf.lfUnderline     = underline;
      lf.lfHeight        = height;
      lf.lfEscapement    = angle;
      lf.lfOrientation   = angle;
      lf.lfStrikeOut     = strikeout; 
      lf.lfCharSet       = ANSI_CHARSET; 
      lf.lfOutPrecision  = OUT_DEFAULT_PRECIS; 
      lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 

      lf.lfQuality    = DEFAULT_QUALITY;
      _tcsncpy(lf.lfFaceName,font_name.GetBuffer(0),LF_FACESIZE);
	  HFONT font =  ::CreateFontIndirect(&lf);

      if (font == NULL)
		  ::MessageBox(NULL,L"error creating font",L"",MB_OK);
	  return font;
    }

   void LocDrawItem(HDC dc,const CRect &rc,const int iIndex)
   {
	 wchar_t buf[2];
	 buf[1]=0;
	 buf[0]='A'+iIndex;
	 if (m_font == 0)
		 m_font = CreateFont(dc,rc.Height(),m_font_name);

     HFONT    old_font        = (HFONT) SelectObject(dc,m_font);
     int      old_bkmode      = SetBkMode   (dc,TRANSPARENT);
	 COLORREF old_bkcolor     = SetBkColor  (dc,RGB(255,255,255));
     COLORREF old_textcolor   = SetTextColor(dc,RGB(  0,  0,  0));
	 ExtTextOut  (dc,rc.left+2,rc.top+2,ETO_OPAQUE,NULL,buf,1,NULL);
	 SelectObject(dc,old_font);
     SetBkMode   (dc,old_bkmode);
	 SetBkColor  (dc,old_bkcolor);
	 SetTextColor(dc,old_textcolor);
   }

   void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
   {
      CDCHandle  dc(lpDrawItemStruct->hDC);
      RECT &rc    = lpDrawItemStruct->rcItem;
	  int  bIndex = lpDrawItemStruct->itemID;
	  int  iIndex = bIndex == CHB_ITEM_DEFAULT? GetCurSel(): bIndex;
      int      old_mode = dc.SetBkMode(TRANSPARENT);
      COLORREF old_bkcolor = dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));

      if( bIndex  >= 0 && bIndex < GetButtonCount() ) 
	  {
		dc.Rectangle(&rc);
		::InflateRect(&rc, -1, -1);
		LocDrawItem(dc,rc,iIndex);
	  }
      else if( bIndex == CHB_ITEM_DEFAULT ) 
	  {
        dc.FrameRect(&rc, (HBRUSH)::GetStockObject(GRAY_BRUSH));
        ::InflateRect(&rc, -3, -3);
		Rectangle(dc,rc.left,rc.top,rc.right,rc.bottom);
        ::InflateRect(&rc, -1, -1);
        LocDrawItem(dc,rc,iIndex);
      }
   }

   ~CSymbolCtrl()
   {
     if (m_font) DeleteObject(m_font);

   }
};



/////////////////////////////////////////////////////////////////////////////
//
// The sample Choice Bar controls
//
// The follwing samples derive directly from CChoiceBarCtrl.
// This means that they can actually use the internal members
// of this class. But they will not! To keep the code clean, I'm only
// using public member methods.
//

class CLineWidthCtrl : 
   public CChoiceBarCtrl<CLineWidthCtrl>,
   public COwnerDraw<CLineWidthCtrl>
{
public:
   DECLARE_WND_CLASS(_T("WTL_LineWidth"))

   BEGIN_MSG_MAP(CLineWidthCtrl)
      REFLECTED_NOTIFY_CODE_HANDLER(CHBN_INITIALIZE, OnInitialize)
      CHAIN_MSG_MAP(CChoiceBarCtrl<CLineWidthCtrl>)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CLineWidthCtrl>, 1)
      DEFAULT_REFLECTION_HANDLER()
   END_MSG_MAP()

   LRESULT OnInitialize(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {
      SetExtStyle(CHB_EX_FLAT);
      SetButtonCount(5);
      SetColumns(1);
      SetButtonSize(120,18);
      return 0;
   }

   void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
   {
      CDCHandle dc(lpDrawItemStruct->hDC);
      RECT &rc = lpDrawItemStruct->rcItem;

      dc.SetBkMode(TRANSPARENT);
      // Create pen with increasing width.
      CPen pen;
      pen.CreatePen(0, lpDrawItemStruct->itemID, ::GetSysColor(COLOR_BTNTEXT));
      HPEN hOldPen = dc.SelectPen(pen);
      int y = rc.top + (((rc.bottom-rc.top)/2)-1);
      for( int i=0; i<3; i++ ) {
         dc.MoveTo(rc.left+3, y);
         dc.LineTo(rc.right-3, y);
         y++;
      }
      dc.SelectPen(hOldPen);
   }
};



#endif // !defined(AFX_CHOICEBAR_H__20010710_3C6D_A924_0A99_0080AD509054__INCLUDED_)

