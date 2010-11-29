///////////////////////////////////////////////////////////////////////////////////////
//                      WtlGrid 2.1                                                  //
///////////////////////////////////////////////////////////////////////////////////////
//   (c) GISTEK 2001/2002/2003/20040 -  											 //
//    Written by Noel Frankinet nfr@gistek.net                                       //
//    The grid is a vector of vector of cell containing a CString !                  //
///////////////////////////////////////////////////////////////////////////////////////                                            
//    This is free software                                                          //
//    it can be used in any software free or commercial provided that                //
//    1-This copyright is remains intact                                             //
//    2-Bugs,correction and patch are sent back to me for the next release           //        
//    3-You acknoledge the use of this software in your about, doc and licence.      //
//    Thank you                                                                      // 
//    DISCLAIMER                                                                     //
//    Use at your own risk, no warranty whatsoever                                   //
//    Only the property module is well tested, other mode (multi-cell)               //
//    are experimental                                                               //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __cplusplus
  #error ATL requires C++ compilation (use a .cpp suffix)
#endif

#if !defined(WTLGRID_INCLUDED_)
	#define WTLGRID_INCLUDED_
#pragma once
#if !defined(IDC_SIZE_LR)
#pragma message("warning : define a IDC_SIZE_LR CURSOR")
#endif
#ifndef __ATLCTRLS_H__
  #error WtlGrid.h requires atlscrl.h to be included first
#endif


#pragma once
#ifndef _COLOR_INCLUDED_
class Ccolor {
public  :
   enum color_name{red,green,blue,black,yellow,white,magenta,light_blue}; 
   Ccolor():m_R(0),m_G(0),m_B(0),m_A(0){};
   Ccolor(unsigned char R,unsigned char G,unsigned char B,unsigned char A):
   m_R(R),
   m_G(G),
   m_B(B),
   m_A(A) {
   }
   
   Ccolor(const char *desc) //0,0,0,0
   {
	 char b[255];
	 strncpy(b,desc,254);
	 char *p = strtok(b,",");
	 m_R = p != NULL ? atoi(p):0;
	       p = strtok(NULL,",");
	 m_G = p != NULL ? atoi(p):0;
	       p = strtok(NULL,",");
	 m_B = p != NULL ? atoi(p):0;
	       p = strtok(NULL,", ");
	 m_A = p != NULL ? atoi(p):0;  
   }

   Ccolor(color_name cname)
   { m_A=0;
     switch(cname)
     {
       case red   : m_R=255;m_G=0;m_B=0;break;
       case blue  : m_R=0;m_G=0;m_B=255;break;
       case green : m_R=0;m_G=255;m_B=0;break;
       case yellow : m_R=255;m_G=255;m_B=0;break;
       case black : m_R=0;m_G=0;m_B=0;break;
       case white : m_R=255;m_G=255;m_B=255;break;
       case magenta   : m_R=255;m_G=0;m_B=255;break;
       case light_blue : m_R=0;m_G=255;m_B=255;break;
     }
   }

   void rotate()
   {static int k=0;
    switch(k)
    {case 0: m_R = 255;m_G=0;m_B=0;break;
       case 1  : m_R=0;m_G=0;m_B=255;break;
       case 2 : m_R=0;m_G=255;m_B=0;break;
       case 3 : m_R=255;m_G=255;m_B=0;break;
       case 4: m_R=128;m_G=128;m_B=128;break;
    }
    k++;
    if (k>4) k=0;
   }
   unsigned char R()const {return m_R;}
   unsigned char G()const {return m_G;}
   unsigned char B()const {return m_B;}
   unsigned char A()const {return m_A;}
   void R(unsigned char r) {m_R = r;}
   void G(unsigned char g) {m_G = g;}
   void B(unsigned char b) {m_B = b;}
   void A(unsigned char a) {m_A = a;}
   bool operator == (const Ccolor other) 
   {return  m_R == other.m_R &&
            m_G == other.m_G &&
            m_B == other.m_B &&
            m_A == other.m_A;}
   Ccolor operator ~()
   {Ccolor c(~m_R,~m_G,~m_B,m_A);
    return c;
   } 
   virtual ~Ccolor()
   {
   }
private :
  unsigned char m_R,m_G,m_B,m_A;
};

#endif

////////////////////////////////////////////////////////////////////////////////////////
class WtlGridWrapper;

///////////////////////////////////////////////////////////////////////////////////////////
//The grid itself derived from a scrolling window
///////////////////////////////////////////////////////////////////////////////////////////
class WtlGrid : public CScrollWindowImpl<WtlGrid> 
{
	friend class WtlGridWrapper;
	typedef CScrollWindowImpl<WtlGrid>    baseClass;
public : 
class WtlDC 
{
public :
	enum hjust
	{
		left,right,center
	}; 
	enum vjust
	{
		top,vcenter,bottom
	};
	enum blook
	{
		flat,raised,sunked
	};


    WtlDC(CDCHandle dc):
		m_dc(dc)
    {
    }

	void TextDraw(const int xx,
		          const int yy,
				  const int width,
				  const int height,
				  const CString &str,
				  const hjust hj=left,
				  const vjust vj=vcenter)
    {
	int old = m_dc.SetBkMode(TRANSPARENT);
	int x = xx;
	int y = yy;
	//m_dc.SetTextColor(RGB(0,0,0));
	int len = str.GetLength();
	const int lmarg(3);
	switch ( vj )
	{
	case top     : break;
	case vcenter : y += height/2-TextHeight(str)/2;break;
	case bottom  : y += height - TextHeight(str);  break;
	}
	int twidth = TextWidth(str,len);
	if ( twidth > width )
	{CString alt(str);
		while ( TextWidth(str,--len)  > width  && len > 3 );
		wchar_t *buf;
		buf = alt.GetBuffer(0);
		for(int i=0;i<3;++i)
		   buf[len-i] = '.';
		alt.ReleaseBuffer();
		m_dc.TextOut(x+lmarg,y,alt,len);
	}
	else
	{
		switch ( hj )
		{
		case left   : x += lmarg;                 break;
		case center : x += width/2-twidth/2;      break;
		case right  : x += width - (twidth +3);   break;
		}

		m_dc.TextOut(x,y,str,len);
	}
	m_dc.SetBkMode(old);
    }

	void DrawEmptyButton(const int x,
		                 const int y,
						 const int width,
						 const int height,
						 const blook look=raised) 
    {
	RECT rc = { x, y, x + width, y + height};
	m_dc.FillRect(&rc, ::GetSysColorBrush(COLOR_3DFACE));

	switch ( look )
	{
	case flat :
			{CPen pen;
	         CPen old = pen.CreatePen(0,0,RGB(128,128,128));
	         m_dc.SelectPen(pen);
			 m_dc.MoveTo(x,y);
			 m_dc.LineTo(x+width,y);
			 m_dc.LineTo(x+width,y+height-1);
			 m_dc.LineTo(x,y+height-1);
			 m_dc.LineTo(x,y);
			 m_dc.SelectPen(old);
			}
		break;
	case sunked : 
		m_dc.Draw3dRect(x,y,width,height,
						::GetSysColor(COLOR_3DSHADOW),
						::GetSysColor(COLOR_3DHILIGHT));
		break;
	case raised : 
		m_dc.Draw3dRect(x,y,width,height,
						::GetSysColor(COLOR_3DHILIGHT),
						::GetSysColor(COLOR_3DSHADOW));
		break;
	}
    }

	void DrawButton(const int x,
		            const int y,
					const int width,
					const int height,
					const CString &str,
					const blook look=raised,
					const hjust hj=left,
					const vjust vj=vcenter)

    {
	DrawEmptyButton(x,y,width,height,look);
	TextDraw(x,y,width,height,str,hj,vj);
    }

	void DrawButton(const int xx,
		            const int yy,
					const int width,
					const int height,
					const int resource,
					const blook look =flat,
					const bool disabled=false)

    {
	int x = xx;
	int y = yy;
	DrawEmptyButton(x,y,width,height,look);
	CBitmap bmp;
	bmp.LoadBitmap(resource);
	if ( bmp.m_hBitmap == NULL )
		return;
	SIZE size;
	bmp.GetSize(size);

	//should center the bitmap
	if ( width > size.cx )
		x += width/2 - size.cx/2;
	else
		x +=2;
	if ( height > size.cy )
		y += height/2 - size.cy/2;
	else
		y +=2;
	//offset image
	if ( look == sunked )
	{
		x+=1;
		y+=1;
	}

	if ( disabled )
		m_dc.DitherBlt(x,y,size.cx,size.cy,NULL,bmp,0,0);
	else
	{
		CDC dcMem;

		dcMem.CreateCompatibleDC(m_dc);
		HBITMAP hBmpOld = dcMem.SelectBitmap(bmp);
		m_dc.BitBlt(x, y, size.cx, size.cy, dcMem, 0, 0, SRCCOPY);
		dcMem.SelectBitmap(hBmpOld);
	}   
    }

	void DrawCheck(const int x,
		           const int y,
				   const int width,
				   const int height,
				   const int sel=-1,
				   const int unsel=-1,
				   const bool selected=false,
				   const bool disabled=false)

    {
    enum {chsize =9 };
	if ( width < chsize || height < chsize )
		return;
	int posx,posy;
	posx = x + width/2 - chsize/2;
	posy = y + height/2 - chsize/2;
	DrawButton(posx,posy,chsize,chsize,selected?sel:unsel,
			   selected?sunked:raised,disabled);
    }

    enum {chsize =9 };

	void DrawTreeCheck(const int x,
		               const int y,
					   const int width,
					   const int height,
					   const int sel=-1,
					   const int unsel=-1,
					   const bool selected=false,
					   const bool empty = true,
					   const bool carry = false)

    {

		m_dc.SetBkMode(TRANSPARENT);
	    CPen pen;
	    pen.CreatePen(2,0,RGB(64,64,64));
		CPen pen2;
		pen2.CreatePen(0,0,RGB(0,0,0));
	    CPen old_pen = m_dc.SelectPen(pen);


	    int posx,posy;
		int half_chsize = chsize/2;
	    posx = x + width/2  - half_chsize;
	    posy = y + height/2 - half_chsize;

#ifdef CARRY_LINE
	    if ( carry )
	    {
		m_dc.MoveTo(x-1,posy+half_chsize);
		if ( empty )
			m_dc.LineTo(posx+half_chsize,posy+half_chsize);
		else
			m_dc.LineTo(posx,posy+half_chsize);
	    }

	    if ( empty )
	    {
		m_dc.MoveTo(posx+half_chsize,y);
		m_dc.LineTo(posx+half_chsize,y+height);
	    }
	    else
	    {
		m_dc.MoveTo(posx+half_chsize,y);
		m_dc.LineTo(posx+half_chsize,posy);
#else
	    if ( empty )
        {CRect rc(x,y,x+width,y+height);
	     m_dc.FillRect(&rc, ::GetSysColorBrush(COLOR_3DFACE));
        }
		else
		{
#endif

		//draw box
		m_dc.SelectPen(pen2);
		m_dc.MoveTo(posx,posy);
		m_dc.LineTo(posx,posy+chsize-1);
		m_dc.LineTo(posx+chsize-1,posy+chsize-1);
		m_dc.LineTo(posx+chsize-1,posy);
		m_dc.LineTo(posx,posy);
#ifdef CARRY_LINE
		m_dc.SelectPen(pen);
		m_dc.MoveTo(posx+chsize,posy+half_chsize);
		m_dc.LineTo(width,posy+half_chsize);
		m_dc.MoveTo(posx+half_chsize,posy+chsize);
		m_dc.LineTo(posx+half_chsize,y+height);
		m_dc.SelectPen(pen2);
#endif

		//draw small "-" or "+"

		m_dc.MoveTo(posx+2,posy+half_chsize);
		m_dc.LineTo(posx+(chsize-2),posy+half_chsize);

		if ( selected )
		    {
			m_dc.MoveTo(posx+half_chsize,posy+2);
			m_dc.LineTo(posx+half_chsize,posy+(chsize-2));
		    }
	    }

		m_dc.SelectPen(old_pen);
    }

    int TextWidth(const CString &str,int len=-1)
    {
	    SIZE   size;
	    if ( len < 0 )
		    len = str.GetLength();
	    m_dc.GetTextExtent(str,len,&size);
	    return size.cx;
    }

    int TextHeight(const CString &str)
    {
	    SIZE   size;
	    m_dc.GetTextExtent(str,str.GetLength(),&size);
	    return size.cy;
    }


private :
	CDCHandle m_dc;
};

////////////////////////////////////////////////////////////////////////////////////////
//toolbar button                                                                      //
////////////////////////////////////////////////////////////////////////////////////////
class WtlGridButton
{
public :
    WtlGridButton(int id,bool enabled) :
    m_id(id),
    m_enabled(enabled)
    {
    }

    void disable()
    {
	m_enabled = false;
    }
    void enable()
    {
	m_enabled = true;
    }

    CString &tip()
    {
	    return m_tip;
    }


	int      m_id;
	bool     m_enabled;
    CString  m_tip;
};
                                          

///////////////////////////////////////////////////////////////////////////////////////////
// Small toolbar code, can be used in any window                                         //
///////////////////////////////////////////////////////////////////////////////////////////
class WtlGridToolbar
{
    enum{toolbar_height =23};
public : 
    WtlGridToolbar():
#ifndef _WIN32_WCE
    m_tip(NULL),
#endif
    m_cur(-1),
    m_height(toolbar_height),
    m_right(true),
    m_hwnd(NULL),
    m_width(0)
    {
    }

    void render(WtlDC &wdc)
    {
	int left;
	int width = m_width;
	if ( m_right )
	{
		left =  width - (m_buttons.size()*(m_height));
		if ( left < 10 )
			left = 10;
	}
	else
		left = 10;
	int x      = left;
	int margin = 3;
	int bsize  = m_height-2*margin;
	int k      = 0;
	wdc.DrawEmptyButton(0,0,width,m_height);
	for ( vector<WtlGridButton>::iterator i = m_buttons.begin(); 
		i!=m_buttons.end();++i )
	    {
		    wdc.DrawButton(x,margin,bsize,bsize,i->m_id,
					   (k==m_cur)?WtlDC::raised:WtlDC::flat,!i->m_enabled);
		    x += m_height;
		    k++;
		    if ( x > width )
			    break;
	    }
    }

    void width(const int w)
    {
	    m_width =  w;
    }

    int width()	const 
    {
	    return m_width;
    }


    CRect &button_rect(const int button)
    {                                                    
	    static CRect r;
	    r.left = 0;
	    r.right = m_width;
	    r.top = 0;
	    r.bottom = m_height;
	    return r;

    }

    int OnMouse(WtlDC &wdc,const int mx,const int my,const UINT u)
    { 
	    if ( my > m_height )
		    return -1;
	bool show_tip = false;
	int left;
	int width = m_width;
	if ( m_right )
	{
		left =  width - (m_buttons.size()*m_height);
		if ( left < 10 )
			left = 10;
	}
	else
		left = 10;

	int x      = left;
	int margin = 3;
	int bsize  = m_height-2*margin;
	int k      = 0;
	for ( vector<WtlGridButton>::iterator i = m_buttons.begin(); 
		i!= m_buttons.end();++i )
	{
		if ( mx > x && mx < x+bsize && i->m_enabled )
		{
			if ( m_cur >= 0 )
				wdc.DrawButton(left + m_cur*m_height,margin,
							   bsize,bsize,m_buttons[m_cur].m_id,WtlDC::flat,!m_buttons[m_cur].m_enabled);
			m_cur = k;
			WtlDC::blook mode;
			switch ( u )
			{
			case WM_LBUTTONUP    : mode = WtlDC::raised; break;
			case WM_LBUTTONDOWN  : mode = WtlDC::sunked; break;
			default              : mode = WtlDC::raised; show_tip = true;break;
			}
			wdc.DrawButton(left + m_cur*m_height,margin,
						   bsize,bsize,m_buttons[m_cur].m_id,mode);
#ifndef _WIN32_WCE
			if ( show_tip && m_tip != NULL && m_tip->IsWindow() )
			{
				m_tip->AddTool(m_hwnd,i->tip().GetBuffer(0));
				m_tip->Activate(TRUE);
			}
#endif
			if ( u==WM_LBUTTONUP )
				return k;
			else
				return -1;
		}
		x += m_height;
		k++;
		if ( x> width )
			break;
	}
#ifndef _WIN32_WCE
	m_tip->Activate(FALSE);   
#endif
	return -1;
    }

    void AddButton(const int id,const bool enabled)
    {WtlGridButton b(id,enabled);
	    m_buttons.push_back(b);
    }

    int height() const  
    {
	    return m_height;
    }

#ifndef _WIN32_WCE
    void set_tip(CToolTipCtrl *tip,HWND hwnd)
    {
	    m_tip = tip;
	    m_hwnd = hwnd;
    }
#endif

    void enable_button(const unsigned int button,const bool enabled)
	{
	if ( button < 0 || button > m_buttons.size() )
		return;
	if ( enabled == m_buttons[button].m_enabled )
		return;

	if ( enabled )
		m_buttons[button].enable();
	else
		m_buttons[button].disable();
	::InvalidateRect(m_hwnd,button_rect(button),TRUE);
	}

private :
	int m_width;
	vector<WtlGridButton> m_buttons;
	int               m_cur;
	int               m_height;
	bool              m_right;
    HWND              m_hwnd;
#ifndef _WIN32_WCE
    CToolTipCtrl      *m_tip;
#endif
};


//////////////////////////////////////////////////////////////////////////////////////////
// a grid value is a CString, it could be optimised                                     //
//////////////////////////////////////////////////////////////////////////////////////////
class WtlGridValue
{
public :
	enum tree_val
	{
		tree_opened,tree_closed,tree_empty
	};

    WtlGridValue()
    {
    }

    WtlGridValue(const CString &value) :
    m_svalue(value)
    {

    }

    bool bvalue() const 
    {
	return(m_svalue.Left(1) == _T("T"));
    }

    void bvalue(const bool new_val) 
    {
	if ( new_val )
		m_svalue = _T("T");
	else
		m_svalue = _T("F");
    }

    int ivalue() const
    {
	return _wtoi(m_svalue);
    }

    void ivalue(const int new_val)
	{
	m_svalue.Format(_T("%d"),new_val);
    }

	double fvalue()	
    {char buf[200];
	 wcstombs(buf,m_svalue.GetBuffer(0),199);

	return atof(buf);
    }

    void fvalue(const double new_val)
	{
	m_svalue.Format(_T("%f"),new_val);
    }


    tree_val tree_value() const 
    {
	if ( m_svalue == _T("T") )
		return tree_opened;
	if ( m_svalue == _T("F") )
		return tree_closed;
	return  tree_empty;
    }

    void tree_value(const tree_val new_val)
    {
	 switch ( new_val )
	    {
	    case tree_opened : m_svalue = _T("T"); break;
	    case tree_closed : m_svalue = _T("F"); break;
	    default          : m_svalue = _T("0"); break;
	    }
    }

    CString &value() 
    {
	return m_svalue;
    }

    CString value() const
    {
	return m_svalue;
    }

	void value(const CString &val)
	{
		m_svalue = val;
	}
 private :
    CString m_svalue;
};

/////////////////////////////////////////////////////////////////////////////////////////
// A grid cell hold a value and has a render mode                                      //
/////////////////////////////////////////////////////////////////////////////////////////
class WtlGridCell
{
public : 
	enum render_mode
	{
		rcheck,        //render like a checkbox
        rstring,       //normal string
        rtree,         //collapsible tree
        rownerdrawn    //call back
	};
	enum e_edit_mode
	{
		none,
        edit,
        combo,
		combolist,
        check,
        tree,
        threestate,
        extended,
        color,
		pattern,
		penstyle,
		pathname,
		fontdialog,
		symbol,
		extcall
	};

    WtlGridCell():
    m_value(),
    m_selected(false),
    m_user_data(NULL)
    {
    }

	void render(CDCHandle dc,CRect &rc,
                const render_mode r,
				WtlGrid &grid,
                const int bsel=-1,
				const int bunsel=-1,
                const bool carry=false);

    void value(const CString &val) 
    {
	    m_value.value(val);
    }

    CString &value()
    {
	    return m_value.value();
    } 

    CString value() const
    {
	    return m_value.value();
    } 


    void bvalue(const bool val) 
    {
	    m_value.bvalue(val);
    }

    bool bvalue() const 
    {
	    return m_value.bvalue();
    }

    int  ivalue() const 
    {
	    return m_value.ivalue();
    }

    void ivalue(const int val)
    {
	    m_value.ivalue(val);
    }

	double fvalue()	
	{
		return m_value.fvalue();
	}

	void fvalue(const double val)
	{
		m_value.fvalue(val);
	}

    WtlGridValue::tree_val tree_value() const
    {
	    return m_value.tree_value();
    }

    void tree_value (const WtlGridValue::tree_val new_val)
    {
	    m_value.tree_value(new_val);
    }

    void select(const bool sel)
    {
	    m_selected = sel;
    }

    bool select() const
    {
	    return m_selected;
    }

    void *user_data()
    {
	    return m_user_data;
    }

    void user_data(void *data) 
    {
	    m_user_data = data;
    }

private :
	WtlGridValue m_value;
	bool         m_selected;
    void *       m_user_data;
};

typedef vector<WtlGridCell> WtlGridCells;

//////////////////////////////////////////////////////////////////////////////////////////
// column is just a size and a header, info are in rows
//////////////////////////////////////////////////////////////////////////////////////////
class WtlGridColumn
{
public  :
	WtlGridColumn(const CString &header,
				  const int  width      = -1,
                  const WtlDC::hjust align = WtlDC::left,
				  const WtlGridCell::render_mode render_mode = WtlGridCell::rstring,
				  const WtlGridCell::e_edit_mode edit_mode = WtlGridCell::none,
				  const int  header_res = -1,
				  const bool growable   = false,
				  const bool clickable  = false,
                  const int  sel_res = -1,
                  const int  unsel_res = -1,
                  void *user_data =NULL
				 ):

    m_header(header),
    m_width(width),
    m_selected(false),
    m_down(false),
    m_edit_mode(edit_mode),
    m_render_mode(render_mode),
    m_sel_res(sel_res),
    m_unsel_res(unsel_res),
    m_growable(growable),
    m_clickable(clickable),
    m_header_halign(WtlDC::left),
    m_header_res(header_res),
    m_user_data(user_data)

    {
	if ( m_header_res >= 0 )
		m_header_halign = WtlDC::right;

    }

    void render (CDCHandle dc,const int x,const int y,const int height)
    {WtlDC wdc(dc);
	if (m_edit_mode != WtlGridCell::tree) wdc.DrawButton(x,y,m_width,height,m_header,
				   (m_down)?WtlDC::sunked:WtlDC::flat,m_header_halign);
	if ( m_header_res > 0 )
	{
		wdc.DrawButton(x+2,y+2,height-4,height-4,m_header_res);
	}
    } 

    void render_mode(const WtlGridCell::render_mode new_mode)
    {
	m_render_mode = new_mode;
    }

    WtlGridCell::render_mode render_mode() const 
    {
	return m_render_mode;
    }

    bool OnEdge(const int x,const int cx)
    {bool ret = false;
	if ( !m_growable )
		return false;
	if ( cx > x + m_width - 4 && cx < x + m_width  )
		ret = true;

	return ret; 
    }

    bool OnHeader(const int x,const int cx)
    {bool ret = false;
	if ( !m_clickable )
		return false;
	if ( cx > x + 2 && cx < x + m_width - 2 )
		ret = true;
	return ret; 
    }


    void  down(const bool new_pos)
    {
	m_down = new_pos;
    }

    int   width()    const
    {
	return m_width;
    }

    void  width(const int new_width)
    {
	m_width = new_width;
    }

    CString header()  const 
    {
	return m_header;
    }

    WtlGridCell::e_edit_mode  edit_mode() const
    {
	return m_edit_mode;
    }

    void  edit_mode(const WtlGridCell::e_edit_mode new_mode)
    {
	m_edit_mode = new_mode;
    }

    vector<CString> &choices()                        
    {
	return m_choices;
    }

    void  choices(vector<CString> &new_choices) 
    {
	m_choices.clear();
	for ( vector<CString>::iterator i = new_choices.begin();i!=new_choices.end();++i )
		m_choices.push_back(*i);
    }               


    bool  growable()  const         
    {
	return m_growable;
    }

    void  growable(const bool new_grow)   
    {
	m_growable = new_grow;
    }

    bool  clickable() const         
    {
	return m_clickable;
    }

    void clickable(const bool new_click) 
    {
	m_clickable = new_click;
    }

    WtlDC::hjust  header_halign() const     
    {
	return m_header_halign;
    }

    void header_halign(const WtlDC::hjust new_align) 
    {
	m_header_halign = new_align;
    }

    int sel_res() const                
    {
	return m_sel_res;
    }

    int unsel_res()	const
    {
	return m_unsel_res;
    }

    void sel_res(const int new_res)      
    {
	m_sel_res = new_res;
    }

    void unsel_res(const int new_res)    
    {
	m_unsel_res = new_res;
    }

    int header_res() const        
    {
	return m_header_res;
    }

    void header_res(const int new_res)   
    {
	m_header_res = new_res;
	if ( m_header_res >= 0 )
		m_header_halign = WtlDC::right;
    }


    void * user_data() 	const 
    {
	return m_user_data;
    }

    void   user_data(void *user_data) 
    {
	m_user_data = user_data;
    }

    void  clear()         
    {
    }

    ~WtlGridColumn() 
    {
    }

private :
	CString                  m_header;
	int                      m_width;
	bool                     m_selected;
	bool                     m_down;
	WtlGridCell::e_edit_mode m_edit_mode;
	WtlGridCell::render_mode m_render_mode; 
	int                      m_sel_res;
	int                      m_unsel_res;
	vector<CString>          m_choices;
	bool                     m_growable;
	bool                     m_clickable;
	WtlDC::hjust             m_header_halign;
	int                      m_header_res;
    void *                   m_user_data;

};

//////////////////////////////////////////////////////////////////////////////////////////
//columns : a vector of column                                                          //
//////////////////////////////////////////////////////////////////////////////////////////
class WtlGridColumns : public vector<WtlGridColumn>
{
public : 

    int width()	const
    {int w = 0;
	 for ( const_iterator i = begin();i !=end();++i )
	    {
		    w += i->width();
	    }
	 return w;
    }

    int column_from_pos(const int x) const
    {
	 int w = 0;
	 int k = 0;
	 for ( const_iterator i = begin();i !=end();++i )
	 {
		if ( (x > w) && (x <( w + i->width())) )
			return k;
		w += i->width();
		k++;
	 }
	 return k;
    }

    void column_left_right(int c,long &left,long &right)
    {
     int k = 0;
	 int w = 0;
	 for ( const_iterator i = begin();i !=end();++i )
	 {
		if ( k == c )
		{
			left  = w;
			right = w + i->width();
			return;
		}
		k++;
		w += i->width();
	 }
    }

    void render(CDCHandle dc,int &x,const int y,const int height)
    {
	 for ( iterator i = begin();i != end();++i )
	 {
		i->render(dc,x,y,height);
		x += i->width();
	 }
    }

};

//////////////////////////////////////////////////////////////////////////////////////////
//row : hold a vector of cell                                                           //
//////////////////////////////////////////////////////////////////////////////////////////
class WtlGridRow
{
public : 
	WtlGridRow(CString header=_T(""),int height=-1,bool visible = true,void *user_data = NULL):
    m_selected(false),
    m_visible (visible),
    m_height(height),
    m_user_data(user_data),
    m_bold(false)
    {
    }

    void push_cell(WtlGridCell cell)
    {
	    m_cells.push_back(cell);
    }

    void select(const bool s)
    {
	    m_selected = s;
    }

    bool select() const
    {
	    return m_selected;
    }

    bool visible() const
    {
	    return m_visible;
    }

    void visible(const bool new_vis)
    {
	    m_visible = new_vis;
    }

    bool contains(const int y,const int cur_y)
    {
	return m_visible && y >= cur_y && y < cur_y +m_height;
    }

    WtlGridCell  * cell(const unsigned int index)
    {
	if ( index < 0 || index > m_cells.size() )
		return NULL;
	return &m_cells[index];
    }

    int  height() const 
    {
	return m_height;
    }

    void height(const int new_height) 
    {
	m_height = new_height;
    }

    void *user_data() const
    {
	return m_user_data;
    }

    void user_data(void *data) 
    {    
	m_user_data = data;
    }

    bool bold() const 
    {
	return m_bold;
    }

    void bold(const bool b) 
    {
	m_bold = b;
    }

    void render(CDCHandle dc,const int x,int &y,WtlGridColumns &columns,CRect &rc,
						bool focus,WtlGrid &grid);

	bool          m_selected;
	bool          m_visible;
	WtlGridCells  m_cells;
	int           m_height;
    bool          m_bold;
    void          *m_user_data;
};

//////////////////////////////////////////////////////////////////////////////////////////
// rows : a vector of row                                                               //
//////////////////////////////////////////////////////////////////////////////////////////
class  WtlGridRows : public vector<WtlGridRow>
{
public :

    void unselect()
    {
	for ( iterator i = begin();i != end();++i )
		i->select(false);
    }

    void unselect_cells(const int column)
    {

	for ( iterator i = begin();i != end();++i )
		i->m_cells[column].select(false);
    }

    void change_value(const int column,const CString &value)
    {

	for ( iterator i = begin();i != end();++i )
		i->m_cells[column].value(value);
    }

    bool single_selection()	const
    {
	int k = 0;
	for ( const_iterator i = begin();i != end();++i )
		if ( i->select() ) k++;
	return(k==1);
    }

    bool single_cell_selection(const int column)
    {
	int k = 0;
	for ( const_iterator i = begin();i != end();++i )
		if ( i->m_cells[column].select() ) k++;
	return(k==1);
    }

	int visible_rows() 
	{ int k(0);
	  for ( const_iterator i = begin();i != end();++i )
		  if (i->visible()) k++;
	  return k;
	}

    void render(CDCHandle dc,int left_margin,WtlGridColumns columns,
						 int init,const CRect &rec,int m_cur_row,int default_height,WtlGrid &grid)
    {int x  = left_margin;
	 int y = 0;
	 CRect rc   = rec;
	 for ( iterator i = begin();i != end();++i )
	 {
		if ( i->height() < 0 )
			i->height(default_height);
		rc.top    = y;
		rc.bottom = y + i->height();
		i->render(dc,x,y,columns,rc,false,grid);
		if ( y > init+rec.bottom )
			break;
	 }
    }
};


//////////////////////////////////////////////////////////////////////////////////////////
//edit as a combobox
//////////////////////////////////////////////////////////////////////////////////////////
class WtlGridDropDown : public CWindowImpl<WtlGridDropDown,CComboBox >
{
public :
	DECLARE_WND_CLASS(NULL)
	BEGIN_MSG_MAP(WtlGridDropDown)
	MESSAGE_HANDLER(WM_KEYDOWN,      OnKeyDown)
	END_MSG_MAP()
	WtlGridDropDown(CWindow &master):
	m_master(master)
	{
	}
    LRESULT OnKeyDown(UINT	/*uMsg*/, WPARAM wParam, 
					  LPARAM lParam,  BOOL& bHandled)
    {
	 bHandled = FALSE;
	 if ( wParam == VK_ESCAPE || wParam == VK_RETURN )
	 {
		m_escape = (wParam == VK_ESCAPE);
		ShowWindow(SW_HIDE); 
		m_master.SetFocus();
		bHandled = TRUE;
	 }
	 return 0;
    }

    void escape(const bool new_escape)
    {
	m_escape = new_escape;
    }

    bool escape() const
    {  
	return m_escape;
    }


private :
	CWindow &m_master;
	bool     m_escape;
};

//////////////////////////////////////////////////////////////////////////////////////////
//edit as an editbox
//////////////////////////////////////////////////////////////////////////////////////////
class WtlGridTextEdit : public CWindowImpl<WtlGridTextEdit, CEdit>
{
public :
	DECLARE_WND_CLASS(NULL)
	BEGIN_MSG_MAP(WtlGridTextEdit)
	MESSAGE_HANDLER(WM_KEYDOWN,          OnKeyDown)
	MESSAGE_HANDLER(WM_SETFOCUS,         OnFocus)
	MESSAGE_HANDLER(WM_KILLFOCUS,        OnKillFocus)
	END_MSG_MAP()

	WtlGridTextEdit(CWindow &master):
	m_master(master)
	{
	}

    LRESULT OnFocus(UINT	/*uMsg*/, WPARAM wParam, 
								   LPARAM lParam, BOOL& bHandled)
    {
	bHandled = FALSE;
#ifdef _WIN32_WCE
    SHSipPreference(m_hWnd, SIP_UP);   //CE only
#endif
	return 0;
    }

    LRESULT OnKillFocus(UINT	/*uMsg*/, WPARAM wParam, 
								   LPARAM lParam, BOOL& bHandled)
    {
#ifdef _WIN32_WCE
    SHSipPreference(m_hWnd, SIP_DOWN); //CE only
#endif
	bHandled = FALSE;
	return 0;
    }


    LRESULT OnKeyDown(UINT	/*uMsg*/, WPARAM wParam, 
								   LPARAM lParam, BOOL& bHandled)
    {

	bHandled = FALSE;
	if ( wParam == VK_ESCAPE || wParam == VK_RETURN )
	{
		m_escape = ( wParam == VK_ESCAPE );
		ShowWindow(SW_HIDE); 
		m_master.SetFocus();
		bHandled = FALSE;
	}
	return 0;
    }

    void escape(const bool new_escape)
    {
	m_escape = new_escape;
    }

    bool escape() const 
    {
	return m_escape;
    }

private :
	CWindow &m_master;
	bool     m_escape;
}; 



//////////////////////////////////////////////////////////////////////////////////////////

class WtlGridColor : public CPaletteSelectCtrl 
{
 public:
	 WtlGridColor(WtlGrid &master):
	   CPaletteSelectCtrl(),
	   m_master(master)
	   {
	   }
    virtual void OnItemSelected(PALETTEENTRY color);
private :
    WtlGrid   &m_master;
};

class WtlGridPattern : public CFillStyleCtrl 
{
 public:
	 WtlGridPattern(WtlGrid &master):
	   CFillStyleCtrl(),
	   m_master(master)
	   {
	   }
    virtual void OnItemSelected(int pattern);
private :
    WtlGrid   &m_master;
};

class WtlGridPenStyle : public CLineStyleCtrl 
{
 public:
	 WtlGridPenStyle(WtlGrid &master):
	   CLineStyleCtrl(),
	   m_master(master)
	   {
	   }
    virtual void OnItemSelected(int style);
private :
    WtlGrid   &m_master;
};

class WtlGridSymbol : public CSymbolCtrl
{
public :
	WtlGridSymbol(const CString & font_name,WtlGrid &master):
	   CSymbolCtrl(font_name),
	   m_master(master)
	   {
	   }
    virtual void OnItemSelected(int style);
private :
    WtlGrid   &m_master;
};

//////////////////////////////////////////////////////////////////////////////////////////


public  :
	enum drag_mode
	{
		start,dodrag,stop
	}; 

	enum select_mode
	{
		mono_row,multi_row,mono_cell,multi_cell
	};

    enum master_mode 
    {
       from_col,from_row
    };
//////////////////////////////////////////////////////////////////////////////////////////
//outgoing interface (events)

    virtual bool OnRender(CDCHandle dc,CRect &rc,const WtlGridRow &row,const WtlGridCell &cell,void *user)
    {
    return true;
    }

    virtual WtlGridCell::e_edit_mode OnBeforeEdit(const int row_nb,const int column_nb,void *user)
    { 
	return _get_edit_mode(column_nb);  
    }

    virtual bool OnAfterEdit(const CString &old_value,const int row,const int column,const void *user,const bool add)
	{
	return true;
	}

    virtual bool OnAfterSelect(const int row_nb,const int column_nb,const int button,const int x,const int y,void *user)
    {
    return true;
    }
    virtual bool OnToolbarButton(const int button_nb,void *user)
    {
    return true;
    }
    virtual bool OnHeaderSelect(const int column_nb,void *user)
    {
    return true;
    }

    virtual bool OnCustomButton(const int row_nb,const int column_nb)
    {
    return true;
    }
/////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32_WCE
    static CWndClassInfo& GetWndClassInfo() 
    { 
	static CWndClassInfo wc = 
	{ 
		{ CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS , StartWindowProc, 
		  0, 0, 0, 0, 0, (HBRUSH)(COLOR_WINDOW+1), 0, _T("WtlGrid") }, 
		NULL, NULL, NULL, TRUE, 0, _T("") 
	}; 
      return wc;
    }
#else	
	DECLARE_WND_CLASS_EX(NULL,CS_VREDRAW|CS_DBLCLKS,-1)
#endif
	BEGIN_MSG_MAP(WtlGrid)
#ifndef _WIN32_WCE
	MESSAGE_HANDLER(WM_ERASEBKGND,   OnEraseBackground)
#endif
	MESSAGE_HANDLER(WM_LBUTTONUP,    OnLButtonUp)
	MESSAGE_HANDLER(WM_RBUTTONUP,    OnRButtonUp)
	MESSAGE_HANDLER(WM_HSCROLL,      OnHScroll)
	MESSAGE_HANDLER(WM_KEYDOWN,      OnKeyDown)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK,OnDblClck)
	MESSAGE_HANDLER(WM_SETFOCUS,     OnFocus)
	MESSAGE_HANDLER(WM_GETDLGCODE,   OnGetDlgCode)

	MESSAGE_HANDLER(WM_DRAWITEM,     OnDrawItem)


	CHAIN_MSG_MAP(baseClass);
    CHAIN_MSG_MAP_ALT(baseClass, 1)


    DEFAULT_REFLECTION_HANDLER()
    COMMAND_CODE_HANDLER(BN_CLICKED, OnButtonClicked)
    REFLECT_NOTIFICATIONS()

	END_MSG_MAP()

    WtlGrid():
    m_color(NULL),
	m_pattern(NULL),
	m_penstyle(NULL),
	m_symbol(NULL),
    m_drop(false),
    m_button(-1),
    m_tentative(false),
    m_old_x(-1),
    m_left_margin(0),
    m_column_padding(30),
    m_row_height(-1),
    m_selection_mode(mono_cell),
    m_cur_row(-1),
    m_cur_column(-1),
    m_in_edit(false),
    m_edit_mode(WtlGridCell::e_edit_mode::none),
    m_vertical_lines(false),
    m_horizontal_lines(false),
    m_masterwnd(NULL),
    m_user_data(NULL),
    m_master_mode(from_col),
	m_edit(*this),
	m_combo(*this),
	m_combolist(*this)
    {
    }

    BOOL PreTranslateMessage(MSG* pMsg)
    {
	pMsg;
	return FALSE;
    }

    LRESULT OnGetDlgCode(UINT /*uMsg*/,      WPARAM wParam, 
					LPARAM	/*lParam*/, BOOL& bHandled)
	{
		return DLGC_WANTALLKEYS;
	}

    LRESULT OnFocus(UINT /*uMsg*/,      WPARAM wParam, 
					LPARAM	/*lParam*/, BOOL& bHandled)
    {
	bHandled = TRUE;
	if ( !m_in_edit )
		return 0;
	bool do_fire = false;
	CString old_value;
	int last_key=0;
	switch ( m_edit_mode )
	{
	case WtlGridCell::edit :
		if ( !m_edit.escape() )
		{
			int nLen = m_edit.GetWindowTextLength();
			if ( nLen > 0 )
			{
				old_value = m_rows[m_cur_row].m_cells[m_cur_column].value();
				LPTSTR lpszText = (LPTSTR)malloc((nLen+1)*sizeof(TCHAR));
				if ( m_edit.GetWindowText(lpszText, nLen+1) )
					change_cell(lpszText);
				free(lpszText);
				do_fire = true;
			}

		}
		else if ( m_tentative )
		{
			m_rows.pop_back();
			m_cur_row--;
			if ( m_cur_row <0 )
				m_cur_row = 0;
			compute_scroll();
			InvalidateRect(NULL,true);
		}

		break;
	case WtlGridCell::combolist :
		if ( !m_combolist.escape() )
		{
			int nLen = m_combolist.GetWindowTextLength();
			if ( nLen > 0 )
			{
				LPTSTR lpszText = (LPTSTR)malloc((nLen+1)*sizeof(TCHAR));
				if ( m_combolist.GetWindowText(lpszText, nLen+1) )
					change_cell(lpszText);
				free(lpszText);
				do_fire = true;
			}
		}
		break;
	case WtlGridCell::combo :
		if ( !m_combo.escape() )
		{
			int nLen = m_combo.GetWindowTextLength();
			if ( nLen > 0 )
			{
				LPTSTR lpszText = (LPTSTR)malloc((nLen+1)*sizeof(TCHAR));
				if ( m_combo.GetWindowText(lpszText, nLen+1) )
					change_cell(lpszText);
				free(lpszText);
				do_fire = true;
			}
		}

		break;
	}  

	m_in_edit   = false;
	m_edit_mode = WtlGridCell::none;

	InvalidateRect(cell_rect());
	if ( do_fire )
		OnAfterEdit(old_value,m_cur_row,m_cur_column,m_user_data,m_tentative);
	m_tentative = false;
	return 0;
    }

    LRESULT OnEraseBackground(UINT	/*uMsg*/, WPARAM wParam, 
							  LPARAM /*lParam*/, BOOL&	/*bHandled*/)
    {
	RECT rect;
	GetClientRect(&rect);
	CDCHandle dc = (HDC)wParam;
	dc.FillRect(&rect, (HBRUSH)(COLOR_WINDOW + 1));
	return 0;
    }

    LRESULT OnHScroll(UINT	/*uMsg*/, WPARAM wParam, 
						   LPARAM /*lParam*/, BOOL& bHandled);

    void left_right_selection(unsigned int new_sel)
    {bool vis;
	if ( new_sel < 0 || new_sel == (unsigned int)m_cur_column || new_sel >= m_columns.size() )
		return;
	if ( m_selection_mode == mono_cell )
	{
		m_rows[m_cur_row].m_cells[m_cur_column].select(false);
		InvalidateRect(cell_rect(),FALSE);
	}
	else
	{
		m_rows.unselect_cells(m_cur_column);
		column_rect(m_cur_column);
		InvalidateRect(m_rc,FALSE);
	}

	m_cur_column = new_sel;
	m_rows[m_cur_row].m_cells[m_cur_column].select(true);
	cell_rect(m_cur_row,m_cur_column,vis);
	if ( vis )
		InvalidateRect(m_rc,TRUE);
    }

    void  show_button()
    {
     CRect rc = cell_rect();
     rc.left = rc.right - (rc.Height()-5);
     if (m_in_edit)
       m_wndButton.ShowWindow(FALSE);
     else
	 {
      switch(get_edit_mode())
        {   
        case WtlGridCell::combo :
        case WtlGridCell::combolist :
        m_drop = true;
        break;
		case WtlGridCell::extcall     :
		case WtlGridCell::pathname    :
        case WtlGridCell::color       :
        case WtlGridCell::pattern     :
        case WtlGridCell::extended    :
		case WtlGridCell::penstyle    :
		case WtlGridCell::fontdialog  :
		case WtlGridCell::symbol      :
        m_drop = false;
        break;
        default :
        m_wndButton.ShowWindow(FALSE);
        return;
        }
     m_wndButton.SetWindowPos(HWND_TOP, &rc, 0);
     m_wndButton.ShowWindow(TRUE);
	 }
    }

   LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
      if( m_wndButton!=lpdis->hwndItem ) return 0;
      CDCHandle dc(lpdis->hDC);
      if( m_drop ) {
         dc.DrawFrameControl(&lpdis->rcItem, DFC_SCROLL, (lpdis->itemState & ODS_SELECTED) ? DFCS_SCROLLDOWN|DFCS_PUSHED : DFCS_SCROLLDOWN);
      }
      else {
         dc.DrawFrameControl(&lpdis->rcItem, DFC_BUTTON, (lpdis->itemState & ODS_SELECTED) ? DFCS_BUTTONPUSH|DFCS_PUSHED : DFCS_BUTTONPUSH);
         dc.SetBkMode(TRANSPARENT);
         dc.DrawText(_T("..."), 3, &lpdis->rcItem, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
      }
      return 0;
   }           

   LRESULT OnButtonClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& bHandled)
   {
      bHandled = TRUE;
      start_edit();
      return 0;
   }

   bool find(CString &what,int &row, const unsigned int col) 
   {int k =0;
	if ( col < 0 || col > m_columns.size() )
		return false;
	for ( WtlGridRows::iterator i = m_rows.begin(); i!= m_rows.end();++i )
	{
		if ( i->m_cells[col].value() == what )
		{
			row = k;
			return true;
		}
		k++;
	}
	return false;
   }

   void up_down_selection(const unsigned int new_sel)
   {bool vis;
	if ( new_sel < 0 || new_sel == (unsigned int)m_cur_row || new_sel >= m_rows.size() )
		return;
    m_button = -1;
	switch ( m_selection_mode )
	{
	case multi_row : 
		{
			if ( !m_rows.single_selection() )
			{
				m_rows.unselect();
				InvalidateRect(NULL,TRUE);
			}
			else
			{
				m_rows[m_cur_row].select(false);
				CRect rc = row_rect(m_cur_row,vis);
				if ( vis )
					InvalidateRect(rc,FALSE);
			} 
			m_cur_row = new_sel;
			m_rows[m_cur_row].select(true);

			CRect rc = row_rect(m_cur_row,vis);
			if ( vis )
				InvalidateRect(rc,TRUE);
		}
		break;
	case mono_row : 
		{
			m_rows[m_cur_row].select(false);
			CRect rc = row_rect(m_cur_row,vis);
			if ( vis )
				InvalidateRect(rc,FALSE);
			m_cur_row = new_sel;
			m_rows[m_cur_row].select(true);

			rc = row_rect(m_cur_row,vis);
			if ( vis )
				InvalidateRect(rc,TRUE);
		}
		break;
	case mono_cell :
		{
			m_rows[m_cur_row].m_cells[m_cur_column].select(false);
			InvalidateRect(cell_rect(),FALSE);
			m_cur_row = new_sel;
			m_rows[m_cur_row].m_cells[m_cur_column].select(true);
		    InvalidateRect(cell_rect(),TRUE);
		}
		break;
	case multi_cell : 
		{
			if ( m_cur_column < 0 || (unsigned int)m_cur_column > m_columns.size() )
				break;
			if ( !m_rows.single_cell_selection(m_cur_column) )
			{

				m_rows.unselect_cells(m_cur_column);
				column_rect(m_cur_column);
				InvalidateRect(&m_rc,TRUE);
			}
			else
			{
				m_rows[m_cur_row].m_cells[m_cur_column].select(false);
			    InvalidateRect(cell_rect(),FALSE);
			} 
			m_cur_row = new_sel;
			m_rows[m_cur_row].m_cells[m_cur_column].select(true);

			InvalidateRect(cell_rect());
		}
		break;
	} 
	OnAfterSelect(m_cur_row,m_cur_column,-1,0,0,m_user_data);
    }        

    LRESULT OnKeyDown(UINT	/*uMsg*/, WPARAM wParam, 
					  LPARAM lParam, BOOL& bHandled)
    {
	switch ( wParam )
	{
	case VK_RETURN :
		start_edit();
		break;
	case VK_PRIOR : ScrollPageUp();
		break;
	case VK_NEXT  : ScrollPageDown();
		break;
	case VK_END   : ScrollBottom();
		break;
	case VK_HOME  : ScrollTop();
		break;
	case VK_LEFT :
		if ( m_selection_mode == mono_cell || m_selection_mode == multi_cell && m_cur_column > 0)
        { WtlGridCell::e_edit_mode mode = get_edit_mode(-1,m_cur_column-1);
		    if (mode  != WtlGridCell::none && mode != WtlGridCell::tree) 
			    left_right_selection(m_cur_column - 1);
		}
		else
			ScrollLineLeft();
		break;
	case VK_UP : //up
		{int new_row = m_cur_row-1;
			while ( new_row >= 0 && (!m_rows[new_row].visible() || get_edit_mode(new_row)==WtlGridCell::none) )
				new_row--;
			if ( new_row < first_row() )
			    for(int k=0;k<m_cur_row-new_row;k++)
				   ScrollLineUp();
			up_down_selection(new_row);
		}
		break;
	case VK_RIGHT :
		if ( m_selection_mode == mono_cell || m_selection_mode == multi_cell && (unsigned int)m_cur_column < m_columns.size()-1)
		{   WtlGridCell::e_edit_mode mode = get_edit_mode(-1,m_cur_column+1);
		    if (mode  != WtlGridCell::none && mode != WtlGridCell::tree) 
			   left_right_selection(m_cur_column + 1);
		}
		else
			ScrollLineRight();
		break;  
	case VK_DOWN : //down
		{unsigned int new_row = m_cur_row+1;
			while ( new_row < m_rows.size() && (!m_rows[new_row].visible() || get_edit_mode(new_row)==WtlGridCell::none) )
				new_row++;
			if ( new_row >= last_row() )
			    for(unsigned int k=0;k<new_row-m_cur_row;k++)
				   ScrollLineDown();
			up_down_selection(new_row);
		}
		break; 
	default	: start_edit(wParam);break;
	}
	return 0;
    }

    void change_value(const int column,const CString &value)
    {
	m_rows.change_value(column,value);
	bool vis;
	CRect rc,rc2;
	rc = cell_rect(0,column,vis);
	GetClientRect(rc2);
	rc2.left = rc.left;
	rc2.right = rc.right;
	InvalidateRect(rc2,true);
    }

    WtlGridCell::e_edit_mode _get_edit_mode(const unsigned int col)	const
    {
	if ( col < 0 || col > m_columns.size() )
		return WtlGridCell::none;

	if ( m_master_mode == from_col )
		return m_columns[col].edit_mode();
	return WtlGridCell::none;

    }


    WtlGridCell::e_edit_mode get_edit_mode(int row=-1,int col=-1) 
    {
	  if (row <0) row= m_cur_row;
	  if (col <0) col = m_cur_column;
    //the default is to return the column edit mode
	return OnBeforeEdit(row,col,m_user_data);
    }

    int select(const int x,const int y,const bool shift,const bool ctrl)
    {
    bool vis;
	if ( m_in_edit )
		end_edit();
	SetFocus();

	switch ( m_selection_mode )
	{
	case mono_cell :
		{
			if ( m_cur_row >= 0 && m_cur_column >= 0 )
			{
				m_rows[m_cur_row].m_cells[m_cur_column].select(false);
				InvalidateRect(cell_rect());
			}
			//compute new position
			m_cur_row    = y;
			m_cur_column = x-(m_left_margin+1);
			cell_from_pos(m_cur_column,m_cur_row);

			//select out of range
			if ( m_cur_column < 0 || m_cur_row < 0 || get_edit_mode()==WtlGridCell::none)
			{
				OnAfterSelect(-1,-1,m_button,x,y,m_user_data);
				return 0;
			}
			
			m_rows[m_cur_row].m_cells[m_cur_column].select(true);

			//special case
			switch ( get_edit_mode() )
			{
		 	
			case WtlGridCell::check :
				{//invert value
					CString old_value =  m_rows[m_cur_row].m_cells[m_cur_column].value();
					m_rows[m_cur_row].m_cells[m_cur_column].bvalue(!m_rows[m_cur_row].m_cells[m_cur_column].bvalue());
					OnAfterEdit(old_value,m_cur_row,m_cur_column,m_user_data,false);
				}
				break;

			case  WtlGridCell::tree :	//show or hide part of the grid.
				if ( m_rows[m_cur_row].m_cells[m_cur_column].tree_value() != WtlGridValue::tree_empty )
				{bool visible;
                    m_wndButton.ShowWindow(FALSE);
					if ( m_rows[m_cur_row].m_cells[m_cur_column].tree_value() == WtlGridValue::tree_opened )
					{m_rows[m_cur_row].m_cells[m_cur_column].tree_value(WtlGridValue::tree_closed);
					 visible = false;
					} 
					else
					{	m_rows[m_cur_row].m_cells[m_cur_column].tree_value(WtlGridValue::tree_opened);	  
					 visible = true;
					}
					for ( WtlGridRows::iterator i = m_rows.begin()+m_cur_row+1;
						i != m_rows.end() && i->m_cells[m_cur_column].tree_value()==WtlGridValue::tree_empty ;++i )
						i->visible(visible);
					InvalidateRect(NULL,TRUE);
					return 0;
				}
				break;
              default :show_button();
                break;
			}
			InvalidateRect(cell_rect());
		}
		break; 
	case multi_cell :
		{
			unsigned int old_row      = m_cur_row;
			unsigned int old_column   = m_cur_column;
			m_cur_row        = y;
			m_cur_column     = x - (m_left_margin+1);

			cell_from_pos(m_cur_column,m_cur_row);
			if ( m_cur_column < 0 || m_cur_row < 0 )
			{
				OnAfterSelect(-1,-1,m_button,x,y,m_user_data);
				return 0;
			}


			if ( 
			   (!shift         && 
				!ctrl          &&
				old_column < m_columns.size() && 
				old_column >= 0)
			   ||
			   (old_column != (unsigned int)m_cur_column    &&
				old_column < m_columns.size() && 
				old_column >= 0)
			   )
			{
				m_rows.unselect_cells(old_column);
				column_rect(old_column);
				InvalidateRect(m_rc,FALSE);
			}

			//if shift - select from last cur to new cur
			if ( shift && (unsigned int)m_cur_column == old_column )
			{
				int start,end;
				if ( old_row < (unsigned int)m_cur_row )
				{
					start = old_row;
					end   = m_cur_row+1;
				}
				else
				{
					start = m_cur_row;
					end   = old_row;
				}

				for ( int i=start;i < end;i++ )
					m_rows[i].m_cells[m_cur_column].select(true);
				column_rect(m_cur_column);
				InvalidateRect(m_rc,TRUE); 
			}
			else //otherwise select or unselect
			switch ( get_edit_mode() )
			{
			case WtlGridCell::check :
				{//invert value
					CString old_value =  m_rows[m_cur_row].m_cells[m_cur_column].value();
					m_rows[m_cur_row].m_cells[m_cur_column].bvalue(!m_rows[m_cur_row].m_cells[m_cur_column].bvalue());
                    OnAfterEdit(old_value,m_cur_row,m_cur_column,m_user_data,false);
				}
				break;

			case  WtlGridCell::tree :	//show or hide part of the grid.
				if ( m_rows[m_cur_row].m_cells[m_cur_column].tree_value() != WtlGridValue::tree_empty )
				{
					if ( m_rows[m_cur_row].m_cells[m_cur_column].tree_value() == WtlGridValue::tree_opened )
						m_rows[m_cur_row].m_cells[m_cur_column].tree_value(WtlGridValue::tree_closed);
					else
						m_rows[m_cur_row].m_cells[m_cur_column].tree_value(WtlGridValue::tree_opened);
					for ( WtlGridRows::iterator i = m_rows.begin()+m_cur_row+1;
						i != m_rows.end() && i->m_cells[m_cur_column].tree_value() == WtlGridValue::tree_empty ;++i )
						i->visible(!i->visible());
					InvalidateRect(NULL,TRUE);
					return 0;
				}
				break;
            default :
			{
			   
				if ( m_cur_row < 0 || m_cur_column < 0 ) break;
				m_rows[m_cur_row].m_cells[m_cur_column].select(!m_rows[m_cur_row].m_cells[m_cur_column].select());
				if ( get_edit_mode() == WtlGridCell::check )
				{
					CString old_value = m_rows[m_cur_row].m_cells[m_cur_column].value();

					m_rows[m_cur_row].m_cells[m_cur_column].bvalue(!m_rows[m_cur_row].m_cells[m_cur_column].bvalue());
					OnAfterEdit(old_value,m_cur_row,m_cur_column,m_user_data,false);
				}
				InvalidateRect(cell_rect()); 
			}
		}

		}
		break; 

	case mono_row :
		{
			//unselect old
			if ( m_cur_row >= 0 )
			{
				m_rows[m_cur_row].select(false);
				CRect rc = row_rect(m_cur_row,vis);
				if ( vis )
					InvalidateRect(rc,FALSE);
			}
			m_cur_row = row_from_pos(y);
			m_rows[m_cur_row].select(true);
			CRect rc = row_rect(m_cur_row,vis);
			if ( vis )
				InvalidateRect(rc,TRUE);
		}
		break;
	case multi_row :
		//unselect
		if ( !shift && !ctrl )
		{
			m_rows.unselect();
			InvalidateRect(NULL,FALSE);
		}

		unsigned int old_row = m_cur_row;
		m_cur_row   = row_from_pos(y);

		//if shift - select from last cut to new cur
		if ( shift)
		{
			int start,end;
			if ( old_row < (unsigned int)m_cur_row )
			{
				start = old_row;
				end   = m_cur_row+1;
			}
			else
			{
				start = m_cur_row;
				end   = old_row;
			}

			for ( int i=start;i < end;i++ )
				m_rows[i].select(true);
			InvalidateRect(NULL,TRUE); 
		}
		else //otherwise select or unselect
		{
			m_rows[m_cur_row].select(!m_rows[m_cur_row].select());
			InvalidateRect(NULL,TRUE); 
		}
		break;      
	}
	OnAfterSelect(m_cur_row,m_cur_column,m_button,x,y,m_user_data);
    return 0;
    }


    LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, 
							 LPARAM lParam, BOOL& bHandled)
    {
    m_button = 2;
    select(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),(wParam& MK_SHIFT)==MK_SHIFT,(wParam& MK_CONTROL)==MK_CONTROL);
    return 0;
    }
    LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, 
							 LPARAM lParam, BOOL& /*bHandled*/)
    {
    m_button = 0;
    select(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),(wParam& MK_SHIFT)==MK_SHIFT,(wParam& MK_CONTROL)==MK_CONTROL);
	return 0;
    }

    LRESULT OnDblClck(UINT	/*uMsg*/, WPARAM wParam, 
						   LPARAM lParam, BOOL& bHandled)
    {
	start_edit();
	return 0;
    }

    void DoPaint(CDCHandle dc)
    {int x = m_left_margin-1;
	POINT pt;
	CRect  rc;
	int cur_row= (m_selection_mode == mono_cell || m_selection_mode == multi_cell)?-1:m_cur_row;
	GetClientRect(&rc);



	GetScrollOffset(pt);
	rc.left  += pt.x;
	rc.right += pt.x;
	//rows
	dc.SelectFont(AtlGetDefaultGuiFont());
	m_rows.render(dc,x,m_columns,pt.y,rc,cur_row,m_row_height,*this);

	CPen pen;
	pen.CreatePen(0,0,RGB(128,128,128));
	dc.SelectPen(pen);

	//vertical lines
	if ( m_vertical_lines )
	{
		int row_height = (m_rows.size()==0)?20:m_rows.visible_rows()*m_rows[0].height();
		int bottom = rc.bottom /*> rc.top+row_height)?rc.top+row_height:rc.bottom*/;
		for ( WtlGridColumns::iterator i=m_columns.begin();
			i!= m_columns.end();++i )
		{
			dc.MoveTo(x,pt.y);
			dc.LineTo(x,pt.y+bottom);
			x += i->width();
			if (x > rc.Width()+pt.x) break;
		}
	}

	//horizontal lines
	//skip column if tree
	if (m_columns[0].edit_mode()==WtlGridCell::tree)
		pt.x += m_columns[0].width();
	if ( m_horizontal_lines )
	{
		int y = m_row_height;

		for (unsigned int k=0;k<m_rows.size();++k )
		{
			if (m_rows[k].visible())
			{
			dc.MoveTo(pt.x,y);
			dc.LineTo(pt.x+rc.right,y);
			y+= m_rows[k].height();
			if (y > rc.Height()+pt.y) break;
			}
		}
	}
    }

    void tentative_add()
    {
	m_tentative = true;
	WtlGridRow row(_T(""),m_rows[m_cur_row].height()-1,true);
	for (unsigned int i=0;i < m_columns.size();i++ )
	{
		WtlGridCell cell;
		row.m_cells.push_back(cell);
	}
	add_row(row,false);
	m_cur_row = m_rows.size()-1;
	m_cur_column = 0;
	ScrollBottom();

	start_edit();

    }

   COLORREF GetColor(const CString &val1)
   {TCHAR buf[200];
    int R,G,B;
	COLORREF  ret;
	CString val(val1);
    _tcsncpy(buf,val.GetBuffer(0),200);
     swscanf(buf,L"&%d&%d&%d",&R,&G,&B);
	 ret = RGB(R,G,B);
	 return ret;
   }

#ifndef _WIN32_WCE
	CString &encode_font(CFontDialog &f)
	{static CString s;
	 COLORREF c =  f.GetColor();
	 
	  s.Format(L"&%1d&%1d&%1d&%1d&%3d&%3d&%3d&%3d&%3d&%s",f.IsBold(),f.IsItalic(),f.IsStrikeOut(),f.IsUnderline(),f.GetWeight(),f.GetSize()/10,GetRValue(c),GetGValue(c),GetBValue(c),f.GetFaceName());
	  return s;
	}
#else
	CString &encode_font(CFontDialog &f)
	{static CString s;
	  return s;
	}
#endif

	LPLOGFONT decode_font(const CString &ss,int &r,int &g,int &b)
	{
	 static LOGFONT lg;
	 CString s(ss);
	 memset(&lg,0,sizeof(lg));
#ifndef _WIN32_WCE
	 int bold,italic,strikeout,weight,underline,si;
	 int ret = swscanf(s.GetBuffer(0),L"&%1d&%1d&%1d&%1d&%3d&%3d&%3d&%3d&%3d",&bold,&italic,&strikeout,&underline,&weight,&si,&r,&g,&b);
	 CString font_name = s.Right(s.GetLength()-29);
	 if (ret== 9)
	 {
	 wcscpy(lg.lfFaceName,font_name.GetBuffer(0));
	 lg.lfWeight    = weight;
	 lg.lfItalic    = italic;
	 lg.lfStrikeOut = strikeout;
	 lg.lfUnderline = underline;
	 HDC dc = ::GetDC(m_hWnd);
	 lg.lfHeight    = -MulDiv(si, GetDeviceCaps(dc, LOGPIXELSY), 72);
	 ::ReleaseDC(m_hWnd,dc);
	 }
#endif
	 return &lg;
	}

    void start_edit(int key=0)
    {
	if ( (m_selection_mode == mono_cell || m_selection_mode == multi_cell) && m_cur_row >=0 && m_cur_column >= 0 && m_columns[m_cur_column].edit_mode()!=WtlGridCell::none)
	{
		CRect rc = cell_rect();
		CWindow w;
		switch ( get_edit_mode() )
		{
		
		case WtlGridCell::check :
			{
				m_rows[m_cur_row].m_cells[m_cur_column].bvalue(!m_rows[m_cur_row].m_cells[m_cur_column].bvalue());
				InvalidateRect(rc,TRUE);
				return;
			}
			break;
		case WtlGridCell::edit :
			{
				w = m_edit;
				w.SetWindowPos(m_hWnd,rc.left,rc.top+1,rc.Width()-1,rc.Height()-1,SWP_NOZORDER);

				m_edit.SetWindowText(m_rows[m_cur_row].m_cells[m_cur_column].value());
				m_edit.SetSelAll(TRUE);
				m_edit.SendMessage(WM_KEYDOWN,key,0);
				m_edit.escape(false);
			}
			break;

		case WtlGridCell::combo :
			{

				w = m_combo;
				w.SetWindowPos(m_hWnd,rc.left,rc.top,rc.Width()-1,rc.Height()+100,SWP_NOZORDER);
              
				m_combo.ResetContent();
				for ( vector<CString>::iterator i = m_columns[m_cur_column].choices().begin();
					i !=  m_columns[m_cur_column].choices().end();++i )
					m_combo.AddString(*i);
				m_combo.SelectString(0,m_rows[m_cur_row].m_cells[m_cur_column].value());
				m_combo.escape(false);
                m_combo.SendMessage(CB_SHOWDROPDOWN,TRUE);
			}
			break;
		case WtlGridCell::combolist :
			{

				w = m_combolist;
				w.SetWindowPos(m_hWnd,rc.left,rc.top,rc.Width()-1,rc.Height()+100,SWP_NOZORDER);
              
				m_combolist.ResetContent();
				for ( vector<CString>::iterator i = m_columns[m_cur_column].choices().begin();
					i !=  m_columns[m_cur_column].choices().end();++i )
					m_combolist.AddString(*i);
				m_combolist.SelectString(0,m_rows[m_cur_row].m_cells[m_cur_column].value());
				m_combolist.escape(false);
                m_combolist.SendMessage(CB_SHOWDROPDOWN,TRUE);
			}
			break;
        case WtlGridCell::color :
            {
              CRect rc2(rc);
			  CRect rcw;
              ClientToScreen(&rc2);
			  GetWindowRect(rcw);
			  if (rc2.top < 0)
				  rc2.top = 0;
              rc2.left   = rc2.right-120;
			  if (rc2.left < 0)
				  rc2.left = 0;
              rc2.right  = rc2.left +100;
              rc2.bottom = rc2.top  +100;
			  if (rc2.bottom > rcw.bottom)
			  {
				rc2.bottom = rcw.bottom;
				rc2.top    = rc2.bottom -100;
			  }
              if (m_color != NULL)  
                 delete m_color;
              m_color =new  WtlGridColor(*this) ;
              m_color->Create(*this, rc2,NULL, WS_POPUP | WS_DLGFRAME);
			  m_color->SetColor( GetColor(m_rows[m_cur_row].m_cells[m_cur_column].value()) );
              w = *m_color;
			  m_color->SetFocus();
              ATLASSERT(m_color->IsWindow());
            }
            break;
        case WtlGridCell::pattern :
            {
              CRect rc2(rc);
			  CRect rcw;
              ClientToScreen(&rc2);
			  GetWindowRect(rcw);
              rc2.left   = rc2.right-120;
              rc2.right  = rc2.left +100;
              rc2.bottom = rc2.top  +100;
			  if (rc2.bottom > rcw.bottom)
			  {
				rc2.bottom = rcw.bottom;
				rc2.top    = rc2.bottom -100;
			  }
              if (m_pattern != NULL)
                 delete m_pattern;
              m_pattern = new  WtlGridPattern(*this) ;
              m_pattern->Create(*this, rc2,NULL, WS_POPUP | WS_DLGFRAME);
			  m_pattern->SetCurSel(m_rows[m_cur_row].m_cells[m_cur_column].ivalue());
              w = *m_pattern;
			  m_pattern->SetFocus();
              ATLASSERT(m_pattern->IsWindow());
            }
            break;

        case WtlGridCell::penstyle :
            {
              CRect rc2(rc);
			  CRect rcw;
              ClientToScreen(&rc2);
			  GetWindowRect(rcw);
              rc2.left   = rc2.right-120;
              rc2.right  = rc2.left +100;
              rc2.bottom = rc2.top  +100;
			  if (rc2.bottom > rcw.bottom)
			  {
				rc2.bottom = rcw.bottom;
				rc2.top    = rc2.bottom -100;
			  }
              if (m_penstyle != NULL)
                 delete m_penstyle;
              m_penstyle = new  WtlGridPenStyle(*this) ;
              m_penstyle->Create(*this, rc2,NULL, WS_POPUP | WS_DLGFRAME);
			  m_penstyle->SetCurSel(m_rows[m_cur_row].m_cells[m_cur_column].ivalue());
              w = *m_penstyle;
			  m_penstyle->SetFocus();
              ATLASSERT(m_penstyle->IsWindow());
            }
            break;
        case WtlGridCell::symbol :
            {
              CRect rc2(rc);
			  CRect rcw;
              ClientToScreen(&rc2);
			  GetWindowRect(rcw);
#ifdef _WIN32_WCE
			  rc2=rcw;
#else
              rc2.left   = rc2.right-120;
              rc2.right  = rc2.left +100;
              rc2.bottom = rc2.top  +100;
			  if (rc2.bottom > rcw.bottom)
			  {
				rc2.bottom = rcw.bottom;
				rc2.top    = rc2.bottom -100;
			  }
#endif
              if (m_symbol != NULL)
                 delete m_symbol;
              m_symbol = new  WtlGridSymbol(m_font_name,*this) ;
              m_symbol->Create(*this, rc2,NULL, WS_POPUP | WS_DLGFRAME);
			  m_symbol->SetCurSel(m_rows[m_cur_row].m_cells[m_cur_column].ivalue());
              w = *m_symbol;
			  m_symbol->SetFocus();
              ATLASSERT(m_symbol->IsWindow());
            }
            break;

		case WtlGridCell::pathname :
			{ 
	          CFileDialog dlg(TRUE, _T("Open file"), m_rows[m_cur_row].m_cells[m_cur_column].value(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All Files (*.*)\0*.*\0"), m_hWnd);
			  CString s((dlg.DoModal() == IDOK)?dlg.m_szFileName:L"");
			  change_cell(s);
		      OnAfterEdit(s,m_cur_row,m_cur_column,m_user_data,m_tentative);
			}
			return;
		case WtlGridCell::fontdialog :
#ifndef _WIN32_WCE
			{ int r(0),g(0),b(0);
			  LPLOGFONT     lf = decode_font(m_rows[m_cur_row].m_cells[m_cur_column].value(),r,g,b);
	          CFontDialog dlg(lf);
			  dlg.m_cf.rgbColors = RGB(r,g,b);
	          if ( dlg.DoModal() == IDOK )
			  {
				change_cell(encode_font(dlg));
				{CString s;
		            OnAfterEdit(s,m_cur_row,m_cur_column,m_user_data,m_tentative);
				}
				return;
			  }
			}
#endif
			return;
		case WtlGridCell::extcall :
			OnCustomButton(m_cur_row,m_cur_column);
			return;
		default : return;
		}

        m_wndButton.ShowWindow(SW_HIDE);
		w.ShowWindow(SW_SHOW);
		w.SetFocus();

		m_in_edit   = true;
		m_edit_mode =  get_edit_mode();
	}
    }

    void end_edit()
    {
	 int b;
    if (!m_in_edit)
        return;
	switch ( m_edit_mode )
	{
	case WtlGridCell::edit :    
		m_edit.ShowWindow(SW_HIDE);
		break;
	case WtlGridCell::combo :
		m_combo.ShowWindow(SW_HIDE);
		OnFocus(0,0,0,b);
		break;
	case WtlGridCell::combolist :
		m_combolist.ShowWindow(SW_HIDE);
		break;
	}
	m_in_edit = false;
	m_edit_mode = WtlGridCell::none;
    }

    void drag(int mx,drag_mode mode)
    {CDCHandle dc = GetDC();
	RECT rc;
	GetClientRect(&rc);

	int old = dc.SetROP2(R2_NOT);
	if ( mode == dodrag )
	{
		dc.MoveTo(m_old_x,0);
		dc.LineTo(m_old_x,rc.bottom);
	}
	m_old_x = mx;

	dc.MoveTo(mx,0);
	dc.LineTo(mx,rc.bottom);
	dc.SetROP2(R2_COPYPEN);
	ReleaseDC(dc);
    }

    int row_from_pos(const int yy)
    {
	POINT pt;
	int   y=yy;
	GetScrollOffset(pt);
	y += pt.y;
	int k      = 0;
	if ( m_rows.size() == 0 )
		return -1;
	int height = 0;
	for ( WtlGridRows::iterator i = m_rows.begin();i!= m_rows.end();++i )
	{
		if ( i->contains(y,height) )
			return k;
		k++;
		if ( i->visible() )
			height += i->height();
	} 

	return -1;
    }

    void cell_from_pos(int &x,int &y)
    {
	POINT pt;

	y = row_from_pos(y);
	GetScrollOffset(pt);
	if ( m_columns.size() == 0 )
	{
		x = -1; 
		return;
	}
	x = m_columns.column_from_pos(x+pt.x);
    }

    int first_row() const
    {POINT pt;
	GetScrollOffset(pt);
	int k= 0;
	int height = 0;
	for ( WtlGridRows::const_iterator i = m_rows.begin();i!= m_rows.end();++i )
	{
		if ( pt.y >= height && pt.y < height + i->height() )
			return k;
		k++;
		if ( i->visible() )
			height += i->height();
	} 
	return k;
    }

    unsigned int last_row() const 
    {RECT rc;
	POINT pt;
	GetScrollOffset(pt);
	GetClientRect(&rc);
	pt.y += rc.bottom;
	unsigned int k= 0;
	int height = 0;
	for ( WtlGridRows::const_iterator i = m_rows.begin();i!= m_rows.end();++i )
	{
		if ( pt.y >= height && pt.y < height + i->height() )
			return k;
		k++;
		if ( i->visible() )
			height += i->height();
	} 
	return k;
    }

    CRect &column_rect(const int col)
    {
	POINT pt;
	GetScrollOffset(pt);
	GetClientRect(&m_rc);
	m_columns.column_left_right(col,m_rc.left,m_rc.right);
	m_rc.left  += m_left_margin-pt.x;
	m_rc.right += m_left_margin-pt.x;
	return m_rc;
    }

    CRect &row_rect(const unsigned int ii,bool &visible)	 //return a rect from a row
    {
	POINT pt;

	visible = false;
	GetScrollOffset(pt);
	GetClientRect(&m_rc);
	unsigned int k      = 0;
	unsigned int height = 0;
	if ( ii < 0  || ii >= m_rows.size() )
		return m_rc;
	for ( WtlGridRows::const_iterator i = m_rows.begin();
		i!= m_rows.end()&&  k != ii;++i )
	{
		k++;
		if ( i->visible() )
			height += i->height();
	} 
	m_rc.top    = height-pt.y;
	m_rc.bottom = height + i->height()-pt.y;
	visible   = true;
	return m_rc;
    }

    CRect &cell_rect(const int y,const int x,bool &visible)	 //return a rect from a cell
    {   
	m_rc = row_rect(y,visible);
	m_columns.column_left_right(x,m_rc.left,m_rc.right);
	POINT pt;
	GetScrollOffset(pt);
	m_rc.left  += m_left_margin-pt.x;
	m_rc.right += m_left_margin-pt.x;
	return m_rc;
    }

    CRect & cell_rect() 
	{bool v; 
	 return cell_rect(m_cur_row,m_cur_column,v);
	}
   
    void add_column(WtlGridColumn &column)
    {
	    m_columns.push_back(column);
	    for ( WtlGridRows::iterator i= m_rows.begin();i != m_rows.end();++i )
	    {
		WtlGridCell cell;
		i->m_cells.push_back(cell);
	    }    
	compute_scroll();
    }

	void    add_column(
		            const CString                     &header     = _T(""),
                    const int                         w           = -1,
                    const WtlDC::hjust                centering  = WtlDC::left,
                    const int                         bmp_res    = -1,
                    const WtlGridCell::render_mode    render_mode = WtlGridCell::rstring,
					const WtlGridCell:: e_edit_mode   edit_mode  = WtlGridCell::none,
                    const bool                        growable   = true,
                    const bool                        clickable  = true,
                    const int                         sel_res    = -1,
                    const int                         unsel_res  = -1
				   )
    {int width = w;
	if ( width < 0 )
		width = cell_size(header).cx + m_column_padding;

	WtlGridColumn col(header,
					  width,
					  centering,
					  render_mode,
					  edit_mode,
					  bmp_res,
					  growable,
					  clickable,
					  sel_res,
					  unsel_res
					 );
	add_column(col);
    }

    void add_row(WtlGridRow &row,bool dummy)
    {
	m_rows.push_back(row);
	compute_scroll();
    }

    void clear_rows()
    {
	if ( m_in_edit )
		end_edit();
	m_rows.clear();
	compute_scroll();
    m_cur_row = m_cur_column = -1;
    m_wndButton.ShowWindow(SW_HIDE);
    }

    void compute_scroll()
    {

	CSize size(m_columns.width(),m_rows.size()*m_row_height);
	if ( size.cx <= 0 )	size.cx = 1;
	if ( size.cy <= 0 )	size.cy = 1;
	SetScrollOffset(0, 0);
    //SetScrollSize(0, 0, FALSE);	
	SetScrollSize(size);								    
	SIZE sizeline;
	sizeline.cx = 1;
	sizeline.cy = m_row_height;
	SetScrollLine(sizeline);
    }

    void wrapper(WtlGridWrapper *masterwnd)
    {                                                                   
	//if ( m_masterwnd != NULL )
	//	return;
	m_masterwnd = masterwnd;
	cell_size(_T("HH"));
    
	m_edit.Create(*this,rcDefault,NULL,WS_CHILD|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN);
	m_edit.SetFont(AtlGetDefaultGuiFont());
	m_combo.Create(*this,rcDefault,NULL,WS_CHILD |  CBS_DROPDOWN/*LIST*/ |WS_VSCROLL);
	m_combo.SetFont(AtlGetDefaultGuiFont());
	m_combolist.Create(*this,rcDefault,NULL,WS_CHILD |  CBS_DROPDOWNLIST |WS_VSCROLL);
	m_combolist.SetFont(AtlGetDefaultGuiFont());
    m_wndButton.Create(m_hWnd, rcDefault, NULL, WS_CHILD|BS_PUSHBUTTON|BS_OWNERDRAW);
    ATLASSERT(m_wndButton.IsWindow());
    m_wndButton.SetFont(AtlGetDefaultGuiFont());
	compute_scroll();							    
    }

    WtlGridWrapper &wrapper()
    {
	return *m_masterwnd;
    }

    void change_cell(const CString &buf)
    {
	 m_rows[m_cur_row].m_cells[m_cur_column].value(buf);
     InvalidateRect(cell_rect());
    } 


    bool vertical_lines()   const             {return m_vertical_lines;}
    bool horizontal_lines() const             {return m_horizontal_lines;}
    void vertical_lines(const bool new_ver)   {m_vertical_lines   = new_ver;}
    void horizontal_lines(const bool new_hor) {m_horizontal_lines = new_hor;}
    int  left_margin()  const                 {return m_left_margin;}
    void left_margin(const int new_margin)    {m_left_margin = new_margin;}

    WtlGridColumn *column(const unsigned int index)
    {if ( index >m_columns.size() )
		return NULL;
	return &m_columns[index];
    }

    WtlGridRow *row(const unsigned int index)
    {if (index >m_rows.size() )
		return NULL;
	return &m_rows[index];
    }

    WtlGridCell *cell(const unsigned int row_index,const unsigned int column_index)
    {WtlGridRow *r = row(row_index);
	if ( r == NULL )
		return NULL;
	if ( column_index < 0 || column_index > r->m_cells.size() )
		return NULL;
	return &r->m_cells[column_index];
    }


    SIZE cell_size(const CString &cont)
    {CDC dc = GetDC();
	 dc.SelectFont(AtlGetDefaultGuiFont());

	 SIZE   size;
	 dc.GetTextExtent(cont,cont.GetLength(),&size);
	 if ( m_row_height < 0 )
		m_row_height = size.cy + 4;
	 ReleaseDC(dc);
	 return size;
    }


    void  column_change(const int column,CString &value)
    {
	 m_rows.change_value(column,value);
    }

    bool vertical_delimiter() const
    {
	return m_vertical_lines;
    }

    bool horizontal_delimiter() const 
    {
	return m_horizontal_lines;
    }

    void vertical_delimiter(const bool new_delimiter) 
    {
	m_vertical_lines = new_delimiter;
    }

    void horizontal_delimiter(const bool new_delimiter) 
    {
	m_horizontal_lines = new_delimiter;
    }

    void *user_data() 
    {
	return m_user_data;
    }

    void  user_data(void *new_data) 
    {
	m_user_data = new_data;
    }

    int cur_row() const 
    {
	return m_cur_row;
    }

    int cur_col() const
    {
	return m_cur_column;
    }

    void Create(HWND hWndParent,CRect &rc,const bool toolbar=false,const int style = WS_CHILD|WS_VISIBLE,const int exstyle=0,const int id=0);

    void add_column(CString                  &header,
					const int                 w,
					const WtlDC::hjust        centering,
					const int                 bmp_res,
					const WtlGridCell::render_mode render_mode,
					const WtlGridCell:: e_edit_mode edit_mode,
					const bool                growable,
					const bool                clickable,
					const int                 sel_res,
					const int                 unsel_res,
					void                     *user_data
				   )
    {int width = w;
	if ( width < 0 )
		width = cell_size(header).cx+m_column_padding;

	WtlGridColumn col(header,
					  width,
					  centering,
					  render_mode,
					  edit_mode,
					  bmp_res,
					  growable,
					  clickable,
					  sel_res,
					  unsel_res,
					  user_data
					 );
	add_column(col);
    }

    void add_row(vector<CString> &strings,void *user_data=NULL)
    {WtlGridRow row(_T(""),-1,true,user_data);
	unsigned int k = 0; 
	for ( vector<CString>::iterator i = strings.begin();k < column_count();k++ )
	{
		WtlGridCell cell;
		if ( k < strings.size() )
		{
			cell.value(*i);
			++i;
		}
		row.m_cells.push_back(cell);
	}
	add_row(row,false);
    }


    unsigned int column_count() const 
    {
	return m_columns.size();
    }

    unsigned int row_count() const 
    {
	return m_rows.size();
    }


    select_mode selection_mode() const 
    {
	return m_selection_mode;
    }

    void selection_mode(const select_mode new_mode) 
    {
	m_selection_mode = new_mode;
    }

    bool adjust_last_row() const
	{
		return true;
	}

    WtlGridWrapper & WtlGrid::master(); //forward
    WtlGridToolbar & WtlGrid::toolbar();
    ~WtlGrid()
    {
	//if ( m_font_bold )
	//	m_font_bold.DeleteObject();

	for ( WtlGridColumns::iterator i= m_columns.begin();i!=m_columns.end();++i )
		i->clear();

    } 

	void font_name(const CString & fon) 
	{
		m_font_name =fon;
	}

    virtual void OnFinalMessage(HWND /*hWnd*/)
	{
      delete m_color;
      delete m_pattern;
      delete m_penstyle;
      delete m_symbol;
	}


protected :
	WtlGridWrapper             *m_masterwnd;
	int                        m_old_x;
	WtlGridColumns             m_columns;
	WtlGridRows                m_rows;
	int                        m_left_margin;
	int                        m_column_padding;
	int                        m_row_height;

public :
    int                        m_cur_row;
	int                        m_cur_column;
	bool                       m_tentative;
    void                       *m_user_data;

protected :
	select_mode                m_selection_mode;
	//edition
	bool                       m_in_edit;
	WtlGridCell::e_edit_mode   m_edit_mode;
	WtlGridTextEdit            m_edit;
	WtlGridDropDown            m_combo;
	WtlGridDropDown            m_combolist;
    WtlGridColor               *m_color;
	WtlGridPattern             *m_pattern;
	WtlGridPenStyle            *m_penstyle;
	WtlGridSymbol              *m_symbol;
    CButton                    m_wndButton;

	bool                       m_vertical_lines;
	bool                       m_horizontal_lines;

	CRect                      m_rc;
    master_mode                m_master_mode;
    int                        m_button;
    bool                       m_drop;
	CString                    m_font_name;
};

///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//a Grid wrapper with an optional toolbar and the column title
//////////////////////////////////////////////////////////////////////////////////////////
class WtlGridWrapper : public CWindowImpl<WtlGridWrapper>
{
public  :

#ifdef _WIN32_WCE
    static CWndClassInfo& GetWndClassInfo() 
    { 
	static CWndClassInfo wc = 
	{ 
		{ CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS , StartWindowProc, 
		  0, 0, 0, 0, 0, (HBRUSH)(COLOR_WINDOW+1), 0, _T("GridWrapper") }, 
		NULL, NULL, NULL, TRUE, 0, _T("") 
	}; 
	return wc; 
    }
#else
	DECLARE_WND_CLASS_EX(_T("GridWrapper"),CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS,-1)
#endif
	BEGIN_MSG_MAP(WtlGridWrapper)
	MESSAGE_HANDLER(WM_PAINT     ,   OnPaint)
	MESSAGE_HANDLER(WM_ERASEBKGND,   OnEraseBackground)


	MESSAGE_HANDLER(WM_MOUSEMOVE  ,  OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONDOWN,  OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONUP,    OnMouseMove)
	MESSAGE_HANDLER(WM_SIZE,         OnSize)
	MESSAGE_HANDLER(WM_DESTROY,      OnDestroy)
    REFLECT_NOTIFICATIONS()
	//CHAIN_MSG_MAP(CWindowImpl<WtlGridWrapper>);
	END_MSG_MAP()


    WtlGridWrapper(WtlGrid & inner) :
    m_inner(inner),
    m_has_toolbar(false),
    m_drag(-1),
    m_min(-1),
    m_top_margin(0),
    m_header_height(-1),
    m_has_column_header(true)
    {

    }


	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
	   return 0;
	}

    virtual void OnFinalMessage(HWND /*hWnd*/)
	{
      delete this;
	}

    BOOL PreTranslateMessage(MSG* pMsg)
    {
	    pMsg;
	    return FALSE;
    }

    LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, 
						LPARAM lParam, BOOL& /*bHandled*/)
    {
	int x          = m_inner.m_left_margin+2;
	int top_margin = m_top_margin;
	int mx = GET_X_LPARAM(lParam);
	int my = GET_Y_LPARAM(lParam);
	RECT rc;
	GetWindowRect(&rc);
#ifndef _WIN32_WCE
	if ( m_tip.IsWindow() )
	{
		MSG Msg = { m_hWnd, uMsg, wParam, lParam};
		m_tip.RelayEvent(&Msg);
	}
#endif
	if ( m_drag >= 0 )
	{
		if ( mx < m_min )
			mx = m_min;
		//else
		//if (mx > m_max) mx = m_max;

		WtlGrid::drag_mode mode = (uMsg==WM_MOUSEMOVE) ? WtlGrid::dodrag : 
								  WtlGrid::stop;
		m_inner.drag(mx,mode);
		if ( mode == WtlGrid::stop )
		{
			int min;
			if ( m_drag == 0 )
				min = m_min;
			else
				min	= m_min -2;

			SetCursor(LoadCursor(NULL,IDC_ARROW));
			m_inner.m_columns[m_drag].width(mx - min);
			ReleaseCapture();
			m_drag    = -1;
            if (m_inner.adjust_last_row())
            {
              CRect rc;
              m_inner.GetClientRect(rc);
            int total =  m_inner.m_columns.width();
            total -= m_inner.m_columns[m_inner.m_columns.size()-1].width();
            int new_width =  rc.Width() - total;
            m_inner.m_columns[m_inner.m_columns.size()-1].width(new_width);
            }
            m_inner.InvalidateRect(NULL,TRUE);
			InvalidateRect(NULL,TRUE);
		}
		return 0;
	}

	if ( m_has_toolbar )
	{
		CDCHandle dc = GetDC();
		WtlGrid::WtlDC wdc(dc);

		int handled = m_toolbar.OnMouse(wdc,mx,my,uMsg);
		top_margin += m_toolbar.height();
		ReleaseDC(dc);
		if ( handled  >= 0 )
		{
			m_inner.OnToolbarButton(handled,m_inner.m_user_data);
			return 0;
		}
	}

	if ( !m_has_column_header )
		return 0;
	int k = 0;
	POINT pt;
	m_inner.GetScrollOffset(pt);
	x -= pt.x;
	if ( my > top_margin && my < top_margin + m_header_height )
		for ( WtlGrid::WtlGridColumns::iterator i=m_inner.m_columns.begin();
			i!=m_inner.m_columns.end();++i )
		{
			if ( i->OnEdge(x,mx) )
			{
#ifdef IDC_SIZE_LR
				SetCursor(AtlLoadCursor(IDC_SIZE_LR));
#endif
				if ( uMsg == WM_LBUTTONDOWN )
				{
                    m_inner.end_edit();
					m_inner.drag(mx,WtlGrid::start);
					//restrict move down to previous column 
					m_drag = k;
					if ( k != 0 )
						m_min = x  + 2;
					else
						m_min = x; 
					SetCapture();
				}
				return 0;
			}
			else if ( i->OnHeader(x,mx) )
			{
				CDCHandle dc = GetDC();
				CFont old = dc.SelectFont(AtlGetDefaultGuiFont());
				switch ( uMsg )
				{
				case WM_LBUTTONDOWN :
					i->down(true);
					i->render(dc,x-2,top_margin,m_header_height);
					break;
				case WM_LBUTTONUP :
					i->down(false);
					i->render(dc,x-2,top_margin,m_header_height);
					m_inner.OnHeaderSelect(k,m_inner.m_user_data);

					break;

				}
				dc.SelectFont(old);
				ReleaseDC(dc);
				return 0;
			}
			x += i->width();
			k++;
		}

	SetCursor(LoadCursor(NULL,IDC_ARROW));

	return 0;
    }

    void Size(const int width,const int height)
    {
     int top=0;
	 if ( m_header_height < 0 &&  m_inner.IsWindow() )
	 {
		SIZE si =  m_inner.cell_size(_T("HH"));
		m_header_height = si.cy;
	 }

	 int vertical=height;
	 if ( m_has_toolbar )
	 {
		top += m_toolbar.height();
		m_toolbar.width(width);
	 }
	 if ( m_has_column_header )
		top += m_header_height;
	 vertical -= top;
	 vertical = (vertical/m_inner.m_row_height)*m_inner.m_row_height;
	 if ( m_inner.IsWindow() )
	 {
 	  m_inner.SetWindowPos(NULL,0,top,width,
							 vertical,0);
      if (m_inner.adjust_last_row() && m_inner.m_columns.size()>0)
	  {  
        CRect rc;
        m_inner.GetClientRect(rc);

        int total =  m_inner.m_columns.width();
        total -= m_inner.m_columns[m_inner.m_columns.size()-1].width();
        int new_width = rc.Width()-  total;
        m_inner.m_columns[m_inner.m_columns.size()-1].width(new_width);
      }
	 }
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
     Size(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));  
 	 bHandled = FALSE;
	 return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
     //FlatSB_Uninitialize();
 	 bHandled = FALSE;
	 return 0;
    }

    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
#ifdef _WIN32_WCE
     static bool first = true;
      if (first)
      {CRect rc;
       GetClientRect(&rc);
       Size(rc.Width(),rc.Height());
       first=false;
      }
#endif

	 if ( wParam != NULL )
	   DoPaint((HDC)wParam);
	 else
	 {
	   CPaintDC dc(m_hWnd);
	   DoPaint(dc.m_hDC);
	 }
	 return 0;
    }

    void DoPaint(CDCHandle dc)
    {
	 POINT pt;
	 m_inner.GetScrollOffset(pt);
	 int x = (m_inner.m_left_margin-pt.x) /*+ 2*/;
	 int y = m_top_margin;
	 CRect  rc;

	 if ( m_header_height < 0 )
	 {
		SIZE si =  m_inner.cell_size(_T("HH"));
		m_header_height = si.cy-1;
	 }

	 CFont old = dc.SelectFont(AtlGetDefaultGuiFont());
	 GetClientRect(&rc);
	 dc.FillRect(&rc, ::GetSysColorBrush(COLOR_3DFACE));

	 GetWindowRect(&rc);
	 WtlGrid::WtlDC wdc(dc);

	 if ( m_has_toolbar )
	 {
	   m_toolbar.render(wdc);
	   y += m_toolbar.height();
	   dc.SelectClipRgn(NULL);
	 }

	 if ( m_has_column_header )
	 {
	  //fill left margin
	  //wdc.DrawEmptyButton(2,y,m_inner.m_left_margin-pt.x,m_header_height,WtlGrid::WtlDC::flat);
	  x-=1;
	  m_inner.m_columns.render(dc,x,y,m_header_height);

	  //fill right margin  
	  rc.right -= x;
	  //wdc.DrawEmptyButton(x,y,rc.right,m_header_height,WtlGrid::WtlDC::flat);
	 }
	 dc.SelectFont(old);
    }

    void __Create(HWND hWndParent, CRect &rc,bool has_toolbar=false,const int style=WS_CHILD|WS_VISIBLE,const int exstyle=0,const int id=0)
    {
     ATLASSERT(inner != NULL);
     Create(hWndParent,rc,NULL,style,exstyle,id);
#ifndef _WIN32_WCE

	 m_tip.Create(m_hWnd);
	 m_tip.Activate(TRUE);
	 m_toolbar.set_tip(&m_tip,m_hWnd);
#endif
     m_has_toolbar= has_toolbar;
    }


    WtlGrid & inner() 
    {
	 return m_inner;
    }

    void toolbar(bool t)
    {
	 m_has_toolbar = t;
    }

    bool toolbar() const              
    {
	 return m_has_toolbar;
    }

	WtlGrid::WtlGridToolbar & get_toolbar()
    {
	 return m_toolbar;
    }

    bool column_header()  const       
    {
	 return m_has_column_header;
    }

    void column_header(const bool new_head) 
    {
	 m_has_column_header = new_head;
    }

    void header_height(const int new_height)
    {
	 m_header_height = new_height;
    }

    int  header_height() const
    {
	 return m_header_height;
    }


	WtlGrid::WtlGridToolbar m_toolbar;
	void font_name(const CString & fon) 
	{
	 m_inner.font_name(fon);
	}

    ~WtlGridWrapper()
    {
    }  

private :

	WtlGrid        &m_inner;
	int            m_drag;
	int            m_min;
	int            m_top_margin;
	bool           m_has_column_header;
	int            m_header_height;
	bool           m_has_toolbar;
#ifndef _WIN32_WCE
	CToolTipCtrl   m_tip;
#endif
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WtlGrid::WtlGridCell::render(CDCHandle dc,
								  CRect &rc,
								  const render_mode r,
								  WtlGrid &grid,
								  const int  bsel,
								  const int  bunsel,
								  const bool carry)
{WtlGrid::WtlDC wdc(dc);

	switch ( r )
	{
	    case rownerdrawn :
	    case rstring :
			if ( m_selected )
			{
			rc.top +=1;
			dc.FillRect(&rc, ::GetSysColorBrush(COLOR_HIGHLIGHT));

			dc.DrawFocusRect(rc);  
			int old_color = dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
			wdc.TextDraw(rc.left,rc.top,rc.Width(),rc.Height(),m_value.value());
			dc.SetTextColor(old_color);
            grid.show_button();
			}
			else
				wdc.TextDraw(rc.left,rc.top,rc.Width(),rc.Height(),m_value.value());
		break;
	    case rtree  :
			if ( m_selected )
			{
			rc.top +=1;
			dc.DrawFocusRect(rc); 
			rc.top -=1; 
			}
			wdc.DrawTreeCheck(rc.left,rc.top,rc.Width(),rc.Height(),bsel,bunsel,m_value.tree_value()!= WtlGridValue::tree_opened,m_value.tree_value() == WtlGridValue::tree_empty,carry);
		break;
	    case rcheck :
			if ( m_selected )
			{
			rc.top +=1;
			dc.DrawFocusRect(rc); 
			rc.top -=1; 
			}
			wdc.DrawCheck(rc.left,rc.top,rc.Width(),rc.Height(),bsel,bunsel,m_value.bvalue());
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void WtlGrid::WtlGridRow::render(CDCHandle dc,const int xx,int &y,WtlGridColumns &columns,CRect &rc,
						         bool focus,WtlGrid &grid)
{   
	int		 k = 0;
	int		 x = xx;
	COLORREF old_color;

		if ( !m_visible )
			return;
		if ( m_selected )
			dc.FillRect(&rc, ::GetSysColorBrush(COLOR_HIGHLIGHT));

		if ( focus )
		{
		rc.top +=1;
		dc.DrawFocusRect(rc);  
		}

	    if ( m_selected )
			old_color = dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));

	    bool carry     = false;
	    CRect rc2      = rc;

		for ( WtlGridCells::iterator i = m_cells.begin();
			  i != m_cells.end();++i )
		{
		WtlGridColumn col = columns[k];
		rc2.left  = x+1;
		rc2.right = x + col.width();
		bool next = (   col.render_mode() == WtlGridCell::rtree && i->tree_value() != WtlGridValue::tree_empty );

		switch ( col.render_mode() )
		{
		case WtlGridCell::rownerdrawn : 
			if (grid.OnRender(dc,rc2,*this,(*i),grid.user_data()))
			 i->render(dc,rc2,col.render_mode(),grid,
					  col.sel_res(),
					  col.unsel_res(),
					  carry
                      );
			break;
   	    case WtlGridCell::rtree :
			//if ( i->tree_value() != WtlGridValue::tree_empty )
			//	dc.SelectFont(AtlGetDefaultGuiFont());
			    if ( i->tree_value() != WtlGridValue::tree_empty )
				{   CBrush brush;
		           dc.FillRect(&rc, brush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE)/*RGB(192,192,192)*/));
				}
		     	i->render(dc,rc2,col.render_mode(),grid,
					  col.sel_res(),
					  col.unsel_res(),
					  carry);
				break;
		default :
			i->render(dc,rc2,col.render_mode(),grid,
					  col.sel_res(),
					  col.unsel_res(),
					  carry);
			break;
		}
		carry = (   col.render_mode() == WtlGridCell::rtree && i->tree_value() != WtlGridValue::tree_empty );
		x = rc2.right;
		k++;
		}
		if ( m_selected )
			dc.SetTextColor(old_color);
		dc.SelectFont(AtlGetDefaultGuiFont());
		y += m_height;
}

WtlGridWrapper& WtlGrid::master() 
{
	return *m_masterwnd;
}

WtlGrid::WtlGridToolbar & WtlGrid::toolbar() 
{
	return m_masterwnd->get_toolbar();
}

/////////////////////////////////////////////////////////////////////////////////////////////

void WtlGrid::WtlGridPattern::OnItemSelected(int pattern)
{
 CString str;
     str.Format(_T("%d"),pattern);
     m_master.change_cell(str);
     m_master.end_edit();
     m_master.show_button();
	 m_master.OnAfterEdit(str,m_master.m_cur_row,m_master.m_cur_column,m_master.m_user_data,m_master.m_tentative);
}

void WtlGrid::WtlGridPenStyle::OnItemSelected(int style)
{
 CString str;
     str.Format(_T("%d"),style);
     m_master.change_cell(str);
     m_master.end_edit();
     m_master.show_button();
	 m_master.OnAfterEdit(str,m_master.m_cur_row,m_master.m_cur_column,m_master.m_user_data,m_master.m_tentative);
}

void WtlGrid::WtlGridSymbol::OnItemSelected(int style)
{
 CString str;
     str.Format(_T("%d"),style);
     m_master.change_cell(str);
     m_master.end_edit();
     m_master.show_button();
	 m_master.OnAfterEdit(str,m_master.m_cur_row,m_master.m_cur_column,m_master.m_user_data,m_master.m_tentative);
}

void WtlGrid::Create(HWND hWndParent,CRect &rc,const bool toolbar,const int style,const int exstyle,const int id)
{
  m_masterwnd = new WtlGridWrapper(*this);
  m_masterwnd->__Create(hWndParent,rc,toolbar,style,exstyle,id);
  baseClass::Create(*m_masterwnd,rc,NULL,style,exstyle);
  wrapper(m_masterwnd);
}

LRESULT WtlGrid::OnHScroll(UINT	/*uMsg*/, WPARAM wParam, 
						   LPARAM /*lParam*/, BOOL& bHandled)
{  
	m_masterwnd->InvalidateRect(NULL);
	bHandled = FALSE;
	return 0;
}

void WtlGrid::WtlGridColor::OnItemSelected(PALETTEENTRY color)
{
 CString str;
     //beforeedit ?
	 {
     str.Format(_T("&%d&%d&%d"),color.peRed,color.peGreen,color.peBlue);
     m_master.change_cell(str);
     m_master.end_edit();
     m_master.show_button();
	 m_master.OnAfterEdit(str,m_master.m_cur_row,m_master.m_cur_column,m_master.m_user_data,m_master.m_tentative);
	 }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//properties - grid relative
/////////////////////////////////////////////////////////////////////////////////////////////
class CPropertyDlg;

class WtlProperties : public WtlGrid
{

	enum prop_place {_property_ = 1,_value_ = 2};
	class WtlPropChoices 
	{
	public :
		WtlPropChoices(const int id,const vector<CString> &choices):
		m_id(id),
		m_choices(choices)
		{
		}
		void clear()
		{
		m_choices.clear();
		}
		int id() const {return m_id;}
		vector <CString> & GetChoices() {return m_choices;}
		void AddChoices(const vector<CString> &new_choices)
		{
		copy(new_choices.begin(),new_choices.end(),back_inserter(m_choices));
		}
	private :
		vector<CString> m_choices;
		int			    m_id;
	};

private :
	vector<WtlPropChoices> m_choices;
public  :
	friend CPropertyDlg;
    WtlProperties()
	{
    }

	bool HasChoices(const int row_nb)
	{
		for (vector<WtlPropChoices>::const_iterator i=m_choices.begin();
		                                      i!=m_choices.end();++i)
	       if (i->id()==row_nb)
			   return true;
		return false;
	}

	vector<CString> &GetChoices(const int row_nb) 
	{
		for (vector<WtlPropChoices>::iterator i=m_choices.begin();
		                                      i!=m_choices.end();++i)
	       if (i->id()==row_nb)
			   return i->GetChoices();
		static vector<CString> r;
		return r;
	}

	void AddChoices(const int row_nb,const vector<CString> &new_choice,bool reset=true)
	{
		for (vector<WtlPropChoices>::iterator i=m_choices.begin();
		                                      i!=m_choices.end();++i)
	    if (i->id()==row_nb)
		{
			if (reset) i->clear();
			i->AddChoices(new_choice);
		}
	}

	/////////////////////////////////////////////////
    enum prop_type{prop_title,prop_read,
                   prop_string,prop_bool,
                   prop_int,prop_color,
                   prop_fillcolor,
				   prop_pattern,
				   prop_penstyle,
				   prop_double,
				   prop_custom,
				   prop_customlist,
				   prop_path,
				   prop_font,
				   prop_symbol,
				   prop_font_list,
				   prop_custom_button
                  };

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
//outgoing interface 
	virtual bool OnPropertyChange(const CString & property,const int rown,const int column)
	{
		return true;
	}

	virtual bool OnBeforePropertyChange(const CString & property,const int rown,const int column)
	{
		return true;
	}

    virtual bool OnCustomButton(const int row_nb,const int column_nb)
    {
    return true;
    }

    virtual bool OnAfterEdit(const CString &old_value,const int row,const int column,const void *user,const bool add)
    {
		if (column == _value_)
          OnPropertyChange(m_rows[row].m_cells[column-1].value(),row,column);
    return true;
    }

	virtual vector<CString> &OnGetChoice(const int row_nb)
	{static vector<CString> ret;
	   return ret;
	}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Create(const CWindow &master,CRect &rc,const int style=WS_CHILD|WS_VISIBLE,const exstyle=0,int id=0)
    {
 	 WtlGrid::Create(master,rc,false,style,exstyle,id);
     vertical_delimiter(true);
     horizontal_delimiter(true);
     selection_mode(WtlGrid::mono_cell);
     add_column(L" ",15,WtlDC::left,-1,WtlGridCell::rtree,WtlGridCell::tree,false,true);
     add_column(L"Prop.",-1,WtlDC::left,-1,WtlGridCell::rstring,WtlGridCell::none,true,false);
     add_column(L"Value",-1,WtlDC::left,-1,WtlGridCell::rownerdrawn,WtlGridCell::edit,false,true);
	}

    void AddProperty(const CString &name,const CString  &value,void *user_data=NULL)
	{
     vector<CString> buf;
     buf.push_back(L" ");
     buf.push_back(name);
     buf.push_back(value);
     
     add_row(buf,user_data);
	}

    void AddProperty(const CString &name,const char * value,void *user_data=NULL)
	{ CString bvalue(value);
       if (user_data == NULL)
       AddProperty(name,bvalue,(void *)prop_string);
       else
       AddProperty(name,bvalue,user_data);
	}

    void AddProperty(const CString &name,bool value)
	{ 
     CString bvalue("FALSE");
     if (value)
       bvalue = "TRUE";
     AddProperty(name,bvalue,(void*)prop_bool);
	} 

    void AddProperty(const CString &name,double value)
	{ 
     CString bvalue;
	 char buf[200];
	 wchar_t wbuffer[200];
	 sprintf(buf,"%f",value);
     int i = mbstowcs( wbuffer, buf, 200 );
	 bvalue.Format(_T("%s"),wbuffer);
	 //bvalue.Format(_T("%f"),value);
     AddProperty(name,bvalue,(void*)prop_double);
	}

    void AddProperty(const CString &name,int value,prop_type type = prop_int)
	{ CString bvalue;
     bvalue.Format(L"%d",value);
       AddProperty(name,bvalue,(void *)type);
	}

    void AddPropertyInt(const CString &name,int value,prop_type type = prop_int)
	{ CString bvalue;
     bvalue.Format(L"%d",value);
       AddProperty(name,bvalue,(void *)type);
	}

    void AddPropertyButton(const CString &name,const CString & value)
	{
       AddProperty(name,value,(void *)prop_custom_button);
	}

    void AddPropertyCustom(const CString &name,const CString & value,prop_type type = prop_custom)
	{
       AddProperty(name,value,(void *)type);
	}

    void AddPropertyCustomList(const CString &name,const CString & value,prop_type type = prop_customlist)
	{
       AddProperty(name,value,(void *)type);
	}

    void AddComboProperty(const CString &name,const CString & value,const vector<CString> &choices)
	{ 
	   WtlPropChoices choc(m_rows.size(),choices);
	   m_choices.push_back(choc);
       AddProperty(name,value,(void *)prop_custom);
	}

    void AddComboProperty(const CString &name,const int value,const vector<CString> &choices)
	{ 
       CString bvalue;
       bvalue.Format(L"%d",value);
       AddComboProperty(name,bvalue,choices);
	}

    void AddComboProperty(const CString &name,const double value,const vector<CString> &choices)
	{ 
     CString bvalue;
	 char buf[200];
	 wchar_t wbuffer[200];
	 sprintf(buf,"%f",value);
     int i = mbstowcs( wbuffer, buf, 200 );
	 bvalue.Format(_T("%s"),wbuffer);
     AddComboProperty(name,bvalue,choices);
	}

    void AddComboListProperty(const CString &name,const CString & value,const vector<CString> &choices)
	{ 
	   WtlPropChoices choc(m_rows.size(),choices);
	   m_choices.push_back(choc);
       AddProperty(name,value,(void *)prop_customlist);
	}

    void AddComboListProperty(const CString &name,const int value,const vector<CString> &choices)
	{ 
       CString bvalue;
       bvalue.Format(L"%d",value);
       AddComboListProperty(name,bvalue,choices);
	}

    void AddComboListProperty(const CString &name,const double value,const vector<CString> &choices)
	{ 
     CString bvalue;
	 char buf[200];
	 wchar_t wbuffer[200];
	 sprintf(buf,"%f",value);
     int i = mbstowcs( wbuffer, buf, 200 );
	 bvalue.Format(_T("%s"),wbuffer);
     AddComboListProperty(name,bvalue,choices);
	}

    double Match(const double value,vector<CString> &choices)
	{
	 for (vector<CString>::iterator i=choices.begin();i!= choices.end();++i)
	 { 
      char buf[200];
	  wcstombs(buf,i->GetBuffer(0),200);
	  double a = atof(buf);
	  if (i+1== choices.end()) break;
	  wcstombs(buf,(i+1)->GetBuffer(0),200);
	  double b = atof(buf);
	  double dist = b -a ;
	  if (dist <= 0) continue;
	  if (value >=a && value < a+dist/2.0) return a;
	  else
	  if (value >= a +dist/2.0 && value <= b) return b;
	 }
	 return 0.0;
	}

   int IndexFromChoice(const int row_nb,const CString &choice)
   {  	int ret(0);
		if (!HasChoices(row_nb)) return -1;
		vector<CString> choices = GetChoices(row_nb);
		for (vector<CString>::const_iterator i=choices.begin();i!= choices.end();++i,++ret)
			if (*i == choice) return ret;
		return -1;
   }

   void AddProperty(const CString &name,int R,int G,int B,prop_type type = prop_color)
   {
    CString bvalue;
    bvalue.Format(L"&%d&%d&%d",R,G,B);
       AddProperty(name,bvalue,(void *)type);
   }

   void AddColorProperty(const CString &name,int R,int G,int B)
   {
       AddProperty(name,R,G,B);
   }

   void AddFillColorProperty(const CString &name,int R,int G,int B)
   {
       AddProperty(name,R,G,B,prop_fillcolor);
   }

   void AddPatternProperty(const CString &name,int pattern)
   {
       AddProperty(name,pattern,prop_pattern);
   }

   void AddPenStyleProperty(const CString &name,int style)
   {
       AddProperty(name,style,prop_penstyle);
   }

   void AddPathProperty(const CString &name,const char * value)
   { CString bvalue(value);
       AddProperty(name,bvalue,(void *)prop_path);
   }

   void AddFontProperty(const CString &name,const CString & value)
   { 
       AddProperty(name,value,(void *)prop_font);
   }

   static BOOL CALLBACK EnumFontProc (LPLOGFONT lplf, LPTEXTMETRIC /*lptm*/, DWORD dwType, LPARAM lpData)	
   {	
	    vector<CString> *font_list = reinterpret_cast< vector<CString> *>(lpData);
		font_list->push_back(lplf->lfFaceName);
		return TRUE;
   }

   void AddFontListProperty(const CString &name,const CString & value)
   { 
	 vector<CString> font_list;
	 CClientDC dc(m_hWnd);		

		EnumFonts (dc, 0,(FONTENUMPROC) EnumFontProc,(LPARAM)&font_list); //Enumerate font
        AddComboListProperty(name,value,font_list);
   }


   void AddSymbolProperty(const CString &name,const short & value,const CString & fn_name)
   { 
	   font_name(fn_name);
       AddPropertyInt(name,value,prop_symbol);
   }

   void AddPropertyTitle(const CString &title)
   {
     vector<CString> buf;
     buf.push_back(L"T");
     buf.push_back(title);
     buf.push_back(L"");
     add_row(buf,(void*)prop_title);
   }
   
   void Clear()
   {
      clear_rows();
   }

   int GetValueInt(const unsigned int row) const
   {
	  if (row >= m_rows.size()) return 0;
      return m_rows[row].m_cells[_value_].ivalue();
   }

   double GetValueDouble(const unsigned int row)	 
   {
	  if (row >= m_rows.size()) return 0.0;
      return m_rows[row].m_cells[_value_].fvalue();
   }

   bool GetValueBool(const unsigned int row)	const
   {
	  if (row >= m_rows.size()) return false;
      return m_rows[row].m_cells[_value_].bvalue();
   }

   CString & GetValue(const unsigned int row)
   {
	  static CString empty;
	  if (row >= m_rows.size()) return empty;
      return m_rows[row].m_cells[_value_].value();
   }

   void SetValue(const unsigned int row,const CString & value)
   {
	  if (row >= m_rows.size()) return;
      m_rows[row].m_cells[_value_].value(value);
   }

   CString & GetPropertyName(const unsigned int row)
   {   static CString empty;
	   if (row >= m_rows.size()) return empty;
	   return m_rows[row].m_cells[_property_].value();
   }

   void  ShowBoolChoice()
   {
    vector<CString> buf;
    buf.push_back(L"TRUE");
    buf.push_back(L"FALSE");
    column(_value_)->choices(buf);
   } 
  
   virtual WtlGridCell::e_edit_mode OnBeforeEdit(const int row_nb,const int column_nb,void *user)
   {
    if (column_nb == 0)
       return WtlGridCell::tree;
    else  if (column_nb == _property_)
       return WtlGridCell::none;
	if (column_nb == _value_)
       OnBeforePropertyChange(m_rows[row_nb].m_cells[column_nb-1].value(),row_nb,column_nb);
        
    int val = (int)row(row_nb)->user_data();
    switch(val)
    {
	case prop_custom_button :
		return WtlGridCell::extcall;
    case prop_string :
    case prop_int :
	case prop_double :
       return WtlGridCell::edit;
    case prop_bool :
       ShowBoolChoice();
       return WtlGridCell::combolist;
    case prop_color :
    case prop_fillcolor :
      return WtlGridCell::color;
    case prop_read    :
    case prop_title   :	break;
	case prop_penstyle :
		return WtlGridCell::penstyle;
    case prop_pattern : 
		return WtlGridCell::pattern;
	case prop_customlist : 
		if (HasChoices(row_nb))
			m_columns[_value_].choices(GetChoices(row_nb));
		else
			m_columns[_value_].choices(OnGetChoice(row_nb));
		return WtlGridCell::combolist;
	case prop_custom :
		if (HasChoices(row_nb))
			m_columns[_value_].choices(GetChoices(row_nb));
		else
			m_columns[_value_].choices(OnGetChoice(row_nb));
		return WtlGridCell::combo;
	case prop_path :
		return WtlGridCell::pathname;
	case prop_font :
		return WtlGridCell::fontdialog;
	case prop_symbol :
		return WtlGridCell::symbol;
    }
    return WtlGridCell::none;
   }

   void GetColor(const CString &val1,int &R,int &G,int &B)
   {TCHAR buf[200];
    CString val(val1);
    _tcsncpy(buf,val.GetBuffer(0),200);
     swscanf(buf,L"&%d&%d&%d",&R,&G,&B);
   }

   void GetRect(CRect &rc1,CString &val,HDC dc)
   {
    SIZE size;
	    GetTextExtentPoint32(dc,val,val.GetLength(),&size);
        rc1.top    +=2;
        rc1.bottom -=2;
        int w = rc1.Width()/2;
        if (w < size.cx+10)
          w = size.cx+10;
        rc1.left   += w;
        rc1.right  -= rc1.Height()+10;
        if (rc1.right < rc1.left) rc1.right = rc1.left;
   }

   int GetCount()
   {
	   return row_count();
   }

   int FindProperty(const CString& property) 
   {int row = -1;
	   find(const_cast<CString&>(property),row,1);
	   return row;
   }

   virtual bool OnRender(CDCHandle dc,CRect &rc1,const WtlGridRow &row,const WtlGridCell &cell,void *user_data)
   {
    int val = (int)row.user_data();
    switch(val)
    {
     case prop_fillcolor :
        {
        int R,G,B;
		CRect rc2(rc1);
		CString s(cell.value());
        GetColor(s,R,G,B);
		rc2.top    += 3;
		rc2.bottom -= 3;
		rc2.left   += 3;
		rc2.right   = rc2.left + rc2.Height();
		Rectangle(dc,rc2.left-1,rc2.top-1,rc2.right+1,rc2.bottom+1);
		CBrush brush;
        brush.CreateSolidBrush(RGB(R,G,B));
        FillRect(dc,&rc2,brush);
		rc1.left += rc2.Width()+6;
        }
	 break;
     case prop_pattern :
        {
		CRect rc2(rc1);
		rc2.top    += 3;
		rc2.bottom -= 3;
		rc2.left   += 3;
		rc2.right   = rc2.left + rc2.Height();
		Rectangle(dc,rc2.left-1,rc2.top-1,rc2.right+1,rc2.bottom+1);
		CFillStyleCtrl::LocFillRect(dc,&rc2,cell.ivalue());
		rc1.left += rc2.Width()+6;
        }
     break;
     case prop_color :
        {
        HPEN pen,old_pen;
        int R,G,B;
        GetColor(cell.value(),R,G,B);
        pen = CreatePen(0,2,RGB(R,G,B));
        old_pen = (HPEN) SelectObject(dc,pen);
		
        POINT p1[2];
        p1[0].x  = rc1.left+2;
        p1[0].y  = rc1.top+rc1.Height()/2;
        p1[1].x  = rc1.left+30;
        p1[1].y  = p1[0].y ;
        Polyline(dc,p1,2);
		rc1.left += 35;

        SelectObject(dc,old_pen);
        DeleteObject(pen);
        }
     break;
     case prop_penstyle :
        {
        HPEN pen,old_pen;
        pen = CreatePen(cell.ivalue(),1,RGB(0,0,0));
        old_pen = (HPEN) SelectObject(dc,pen);
		
        POINT p1[2];
        p1[0].x  = rc1.left+2;
        p1[0].y  = rc1.top+rc1.Height()/2;
        p1[1].x  = rc1.left+50;
        p1[1].y  = p1[0].y ;
        Polyline(dc,p1,2);
		rc1.left += 55;

        SelectObject(dc,old_pen);
        DeleteObject(pen);
        }
     break;
	 case prop_font :
	    {CString s(cell.value());
		 s= s.Right(s.GetLength()-29);
		 if (cell.select())
		 {CRect rc(rc1);
			rc.top +=1;
			dc.FillRect(&rc, ::GetSysColorBrush(COLOR_HIGHLIGHT));
			dc.DrawFocusRect(rc);  
		 }
         dc.TextOut(rc1.left+2,rc1.top+2,s,s.GetLength());
	    }
		return false;
	 case prop_symbol :
        {
	     wchar_t buf[2];
	     buf[0]='A'+cell.ivalue();
	     buf[1]=0;
		 HFONT     font            = CSymbolCtrl::CreateFont(dc,rc1.Height(),m_font_name);
         HFONT     old_font        = (HFONT) SelectObject(dc,font);
         int       old_bkmode      = SetBkMode   (dc,TRANSPARENT);
	     COLORREF  old_bkcolor     = SetBkColor  (dc,RGB(255,255,255));
         COLORREF  old_textcolor   = SetTextColor(dc,RGB(  0,  0,  0));
		 if (cell.select())
		 {CRect rc(rc1);
			rc.top +=1;
			dc.FillRect(&rc, ::GetSysColorBrush(COLOR_HIGHLIGHT));
			dc.DrawFocusRect(rc);  
		 }
	     ExtTextOut(dc,rc1.left+2,rc1.top+2,ETO_OPAQUE,NULL,buf,1,NULL);
		 SelectObject(dc,old_font);
		 DeleteObject(font);
         SetBkMode   (dc,old_bkmode);
		 SetBkColor  (dc,old_bkcolor);
		 SetTextColor(dc,old_textcolor);
        }
		return false;
    }
    return true;
   }   

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif 