// Properties.h: interface for the CProperties class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "atlapp.h"
#include "CStringInterface.h"
#include "atlwin.h"
#include "atldlgs.h"
#include "atlddx.h"
#include "resource.h"
#include "StringConversions.h"	// convert to/from strings
#include "DebugUtilities.h"	// extra debugging stuff
#include "app_state.h"

// ======================
// class CPageFormat 
// ======================
template< int id_type >
class CPageFormat :	
	public CPropertyPageImpl<CPageFormat<id_type> >,
	public CWinDataExchange< CPageFormat<id_type> >
{
	// prop values
	app_state	*m_properties ;

public:
	enum { IDD = id_type } ;

	void set_props( app_state *props )
	{
		m_properties = props ;
	}

	LRESULT OnInitDialog( UINT, WPARAM, LPARAM, BOOL& )
	{
		BANNER( "CMemoryPage::OnInitDialog" ) ;

		DoDataExchange(FALSE); // FALSE means copy TO the dialog

		return TRUE;
	}

	int OnApply()
	{
		DoDataExchange(TRUE) ;

		return PSNRET_NOERROR  ;
	}

	BOOL OnKillActive()
	{
		BANNER("CPageSegmentation::OnKillActive") ;
		DoDataExchange(TRUE);
		// FALSE = allow deactivate
		// TRUE = prevent deactivation
		return FALSE;
	}

	BEGIN_MSG_MAP(CPageFormat)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl< CPageFormat >)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageFormat)
		DDX_CHECK( IDC_FONT_FACE,		m_properties->m_font_face )
		DDX_CHECK( IDC_FONT_COLOR,		m_properties->m_font_color )
		DDX_CHECK( IDC_FONT_BOLD,		m_properties->m_font_bold )
		DDX_CHECK( IDC_FONT_ITALIC,		m_properties->m_font_italic )
		DDX_CHECK( IDC_FONT_UNDERLINE,	m_properties->m_font_underline )
		DDX_CHECK( IDC_FONT_SUPER_SUB,	m_properties->m_font_super_sub )
	END_DDX_MAP()
};

// ======================
// class CPageView 
// ======================
template< int id_type >
class CPageView :	
	public CPropertyPageImpl< CPageView<id_type>  >,
	public CWinDataExchange< CPageView<id_type>  >
{
	
	// prop values
	app_state	*m_properties ;

public:
	enum { IDD = id_type } ;
	CPageView()
	{

	}
	void set_props( app_state *props )
	{
		m_properties = props ;
	}
	
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
	{
		DoDataExchange(FALSE); // FALSE means copy TO the dialog

		return TRUE;
	}
	int OnApply()
	{
		DoDataExchange(TRUE) ;
		return PSNRET_NOERROR  ; 
	}
	
	BEGIN_MSG_MAP(CPageView)
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		CHAIN_MSG_MAP( CPropertyPageImpl<CPageView> )
	END_MSG_MAP()
		
	BEGIN_DDX_MAP(CPageView)
		DDX_CHECK( IDC_CLASSIC_INTERFACE, m_properties->m_classic_if )
		DDX_RADIO(IDC_LANG_ENG,m_properties->m_preferred_gui_lang )
	END_DDX_MAP()

};

// ======================
// class CPageSegmentation 
// ======================
template< int id_type >
class CPageSegmentation :	
	public CPropertyPageImpl< CPageSegmentation<id_type>  >,
	public CWinDataExchange< CPageSegmentation<id_type>  >
{

	// prop values
	app_state	*m_properties ;

public:
	TCHAR m_segChars[SEG_CHAR_SIZE] ;
	const static int IDD = id_type ;

	bool m_wants_to_clear  ;

	CPageSegmentation() : m_wants_to_clear(false)
	{
		ZeroMemory(m_segChars, SEG_CHAR_SIZE*sizeof(TCHAR)) ;
	}
	void set_props( app_state *props )
	{
		m_properties = props ;
	}

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
	{
		BANNER("CPageSegmentation::OnInitDialog") ;

		DoDataExchange(FALSE); // FALSE means copy TO the dialog

		// we need to manually set the radio buttons, because the DDX
		// doesn't work for them.
		// The check boxes are handled in the DDX.
		switch(m_properties->m_skipJ)
		{
		case IDC_SKIP_HASJ:
		case IDC_SKIP_NOJ:
		case IDC_NOSKIP:
			break ;
		default:
			m_properties->m_skipJ = IDC_NOSKIP ;
		}
		SendDlgItemMessage( m_properties->m_skipJ, BM_SETCHECK, TRUE, 0 ) ;

		CWindow edit = GetDlgItem(IDC_SEG_END_EDIT) ;
		edit.SetWindowText( m_properties->m_segChars ) ;
		// Initialize the seg chars to the defaults, in case
		// our DDX isn't instantiated.
		_tcscpy_s(m_segChars, (LPCTSTR)m_properties->m_segChars ) ;

		return TRUE;
	}
	void fill_seg_text()
	{
		CString text = get_seg_text() ;
		if (! text.IsEmpty() || m_wants_to_clear)
		{
			_tcscpy_s(m_segChars, (LPCTSTR)text ) ;
		}
	}
	CString get_seg_text()
	{
		CWindow edit = GetDlgItem(IDC_SEG_END_EDIT) ;
		CString text ;
		edit.GetWindowText(text) ;
		return text ;
	}
	int OnApply()
	{
		BANNER("CPageSegmentation::OnApply") ;
		DoDataExchange(TRUE) ;
	
		this->fill_seg_text() ;

		// PSNRET_NOERROR = apply OK
		// PSNRET_INVALID = apply not OK, return to this page
		// PSNRET_INVALID_NOCHANGEPAGE = apply not OK, don't change focus
		return PSNRET_NOERROR  ; 
	}
	BOOL OnKillActive()
	{
		BANNER("CPageSegmentation::OnKillActive") ;
		DoDataExchange(TRUE);

		CString text = get_seg_text() ;
		if (! text.IsEmpty())
		{
			this->fill_seg_text() ;
		}

		// FALSE = allow deactivate
		// TRUE = prevent deactivation
		return FALSE;
	}

	BEGIN_MSG_MAP(CPageSegmentation)

		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )

		COMMAND_HANDLER(IDC_SKIP_HASJ, BN_CLICKED, OnSkipJpn)
		COMMAND_HANDLER(IDC_SKIP_NOJ, BN_CLICKED, OnSkipUnlessJpn)
		COMMAND_HANDLER(IDC_NOSKIP, BN_CLICKED, OnNoSkip)

		//COMMAND_HANDLER(IDC_RADIO_WORD_DOC, BN_CLICKED, OnWordDoc)
		//COMMAND_HANDLER(IDC_RADIO_HTML_DOC, BN_CLICKED, OnHtmlDoc)

		CHAIN_MSG_MAP( CPropertyPageImpl<CPageSegmentation> )

	END_MSG_MAP()
	//LRESULT OnWordDoc(WORD, WORD, HWND, BOOL& )
	//{
	//	m_properties->m_segmentation_type = IDC_RADIO_WORD_DOC ;
	//	return 0;
	//}
	//LRESULT OnHtmlDoc(WORD, WORD, HWND, BOOL& )
	//{
	//	m_properties->m_segmentation_type = IDC_RADIO_HTML_DOC ;
	//	return 0;
	//}


	LRESULT OnSkipJpn(WORD, WORD, HWND, BOOL& )
	{
		m_properties->m_skipJ = IDC_SKIP_HASJ ;
		return 0;
	}
	LRESULT OnSkipUnlessJpn(WORD, WORD, HWND, BOOL& )
	{
		m_properties->m_skipJ = IDC_SKIP_NOJ ;
		return 0;
	}
	LRESULT OnNoSkip(WORD, WORD, HWND, BOOL& )
	{
		m_properties->m_skipJ = IDC_NOSKIP ;
		return 0;
	}

	BEGIN_DDX_MAP(CPageSegmentation)
		DDX_CHECK(IDC_SKIP_NUMS_CHECK, m_properties->m_skipNumbers)
		DDX_CHECK(IDC_SELECT_SPACES, m_properties->m_select_spaces)
	END_DDX_MAP()

};

// ======================
// class CPageTransHist 
// ======================
template< int id_type >
class CPageTransHist :	
	public CPropertyPageImpl< CPageTransHist<id_type>  >,
	public CWinDataExchange< CPageTransHist<id_type>  >
{

	// prop values
	app_state	*m_properties ;

public:
	enum { IDD = id_type } ;

	void set_props( app_state *props )
	{
		m_properties = props ;
	}

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
	{
		BANNER("CPageTransHist::OnInitDialog") ;

		DoDataExchange(FALSE); // FALSE means copy TO the dialog

		return TRUE;
	}

	int OnApply()
	{
		BANNER("CPageTransHist::OnApply") ;
		DoDataExchange(TRUE) ;

		// PSNRET_NOERROR = apply OK
		// PSNRET_INVALID = apply not OK, return to this page
		// PSNRET_INVALID_NOCHANGEPAGE = apply not OK, don't change focus
		return PSNRET_NOERROR  ; 
	}
	BOOL OnKillActive()
	{
		BANNER("CPageTransHist::OnKillActive") ;
		DoDataExchange(TRUE);

		// FALSE = allow deactivate
		// TRUE = prevent deactivation
		return FALSE;
	}

	BEGIN_MSG_MAP(CPageTransHist)
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		CHAIN_MSG_MAP( CPropertyPageImpl<CPageTransHist> )
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageTransHist)
		DDX_CHECK(IDC_USE_TRANS_HIST, m_properties->m_use_trans_hist)
	END_DDX_MAP()

};

// translation history
class CPageTransHistE : public CPageTransHist<IDD_PROPPAGE_TRANS_HIST_E>
{
public:

};
class CPageTransHistJ : public CPageTransHist<IDD_PROPPAGE_TRANS_HIST_J>
{
public:

};

// format
class CPageFormatE : public CPageFormat<IDD_PROPPAGE_FORMAT_E>
{
public:

};
class CPageFormatJ : public CPageFormat<IDD_PROPPAGE_FORMAT_J>
{
public:

};

// view
class CPageViewJ : public CPageView<IDD_PROPPAGE_VIEW_J>
{
public:

};
class CPageViewE : public CPageView<IDD_PROPPAGE_VIEW_E>
{
public:

};

// segmentation
class CPageSegmentE : public CPageSegmentation<IDD_PROPPAGE_SEGMENTATION_E>
{
public:

};
class CPageSegmentJ : public CPageSegmentation<IDD_PROPPAGE_SEGMENTATION_J>
{
public:

};

// ======================
// class CPropertiesDlg 
// ======================
class CPropertiesDlgE  : public CPropertySheetImpl<CPropertiesDlgE>
{
	app_state		m_properties ;

	CPageViewE		m_page1 ;
	CPageFormatE	m_page2 ;
	CPageSegmentE	m_segmentation_page ;
	CPageTransHistE	m_page4 ;

	CString			m_title ;

public:
	app_state get_properties();

	enum { IDD = IDD_PROPSHEET_E };

	CPropertiesDlgE( const app_state &props );

	BEGIN_MSG_MAP(CPropertiesDlgE)
		CHAIN_MSG_MAP	  ( CPropertySheetImpl<CPropertiesDlgE> )
	END_MSG_MAP()
};

// ======================
// class CPropertiesDlgJ 
// ======================
class CPropertiesDlgJ  : public CPropertySheetImpl<CPropertiesDlgJ>
{
	app_state		m_properties ;
	
	CPageViewJ		m_page1 ;
	CPageFormatJ	m_page2 ;
	CPageSegmentJ	m_segmentation_page ;
	CPageTransHistJ	m_page4 ;

	CString			m_title ;

public:
	
	app_state get_properties();
	
	enum { IDD = IDD_PROPSHEET_J };
	
	CPropertiesDlgJ( const app_state &props );
	
	BEGIN_MSG_MAP(CPropertiesDlgJ)
		CHAIN_MSG_MAP	  ( CPropertySheetImpl<CPropertiesDlgJ> )
	END_MSG_MAP()
		
};
