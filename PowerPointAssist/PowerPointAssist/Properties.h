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
#include "dispatchwrapper.h" // For COM dialogs

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
		DDX_CHECK( IDC_FONT_COLOR,		m_properties->m_data.m_font_color )
		DDX_CHECK( IDC_FONT_BOLD,		m_properties->m_data.m_font_bold )
		DDX_CHECK( IDC_FONT_ITALIC,		m_properties->m_data.m_font_italic )
		DDX_CHECK( IDC_FONT_UNDERLINE,	m_properties->m_data.m_font_underline )
	END_DDX_MAP()
};

// ======================
// class CPageView 
// ======================
template< int id_type >
class CPageView :	
	public CPropertyPageImpl< CPageView<id_type>  >,
	public CWinDataExchange< CPageView<id_type> >
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
		SendDlgItemMessage( m_properties->m_data.m_preferred_gui_lang, BM_SETCHECK, TRUE, 0 ) ;
		SendDlgItemMessage( m_properties->m_data.m_navigation_type, BM_SETCHECK, TRUE, 0 ) ;
		DoDataExchange(FALSE); // FALSE means copy TO the dialog

		return TRUE;
	}
	int OnApply()
	{
		BANNER("CPageView::OnApply") ;
		DoDataExchange(TRUE);
		// FALSE = allow deactivate
		// TRUE = prevent deactivation
		return PSNRET_NOERROR  ;
	}

	BOOL OnKillActive()
	{
		BANNER("CPageView::OnKillActive") ;
		return FALSE;
	}
	LRESULT OnLangCmd(WORD, WORD id, HWND, BOOL& )
	{
		ATLASSERT(id == IDC_LANG_ENG || id == IDC_LANG_JAPANESE) ;
		m_properties->m_data.m_preferred_gui_lang = id ;
		return 0;
	}
	LRESULT OnNavCmd(WORD, WORD id, HWND, BOOL& )
	{
		ATLASSERT(id == IDC_NAV_SLIDE || 
				  id == IDC_NAV_TOP ||
				  id == IDC_NAV_ASK) ;
		m_properties->m_data.m_navigation_type = id ;
		return 0;
	}
	LRESULT OnShortcuts(WORD, WORD, HWND, BOOL& )
	{
		try
		{
			CDispatchWrapper wrapper(L"Felix.Preferences"); 
			CComVariant language = L"English";
			CComVariant prog = L"ppt" ;
			if (IDD == IDD_PROPPAGE_VIEW_J)
			{
				language = L"Japanese" ;
			}
			wrapper.method(L"EditShortcuts", prog, language) ;
			wrapper.m_app = NULL ;
		}
		catch (_com_error& err)
		{
			ATLASSERT(FALSE && "Raised exception in file_logger") ;
			except::CComException ce(err) ;
			ce.notify_user(_T("Abbreviations Error")) ;
		}		
		catch(except::CException &e)
		{
			logging::log_error("Failed to edit shortcuts") ;
			logging::log_exception(e) ;
			e.notify_user(_T("Shortcuts Error")) ;
		}
		return 0;
	}
	BEGIN_MSG_MAP(CPageView)
		COMMAND_HANDLER(IDC_LANG_ENG, BN_CLICKED, OnLangCmd)
		COMMAND_HANDLER(IDC_LANG_JAPANESE, BN_CLICKED, OnLangCmd)

		COMMAND_HANDLER(IDC_NAV_SLIDE, BN_CLICKED, OnNavCmd)
		COMMAND_HANDLER(IDC_NAV_TOP, BN_CLICKED, OnNavCmd)
		COMMAND_HANDLER(IDC_NAV_ASK, BN_CLICKED, OnNavCmd)

		COMMAND_HANDLER(IDC_KEYBOARD_SHORTCUTS, BN_CLICKED, OnShortcuts)

		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		CHAIN_MSG_MAP( CPropertyPageImpl<CPageView> )
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageView)
		DDX_CHECK(IDC_RAISE_FELIX, m_properties->m_data.m_raise_felix )
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
	enum { IDD = id_type } ;

	void set_props( app_state *props )
	{
		m_properties = props ;
	}

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
	{
		BANNER("CPageSegmentation::OnInitDialog") ;

		DoDataExchange(FALSE); // FALSE means copy TO the dialog
		SendDlgItemMessage( m_properties->m_data.m_skipJ, BM_SETCHECK, TRUE, 0 ) ;

		CWindow edit = GetDlgItem(IDC_SEG_END_EDIT) ;
		edit.SetWindowText( m_properties->m_data.m_segChars ) ;

		return TRUE;
	}
	void fill_seg_text()
	{
		CWindow edit = GetDlgItem(IDC_SEG_END_EDIT) ;
		CString text ;
		edit.GetWindowText(text) ;
		if (text == _T("refresh"))
		{
			m_properties->m_data.set_segchars_to_defaults() ;
		}
		else
		{
			_tcscpy_s( m_properties->m_data.m_segChars, (LPCTSTR)text ) ;
		}
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

		this->fill_seg_text() ;

		// FALSE = allow deactivate
		// TRUE = prevent deactivation
		return FALSE;
	}

	LRESULT OnAbbreviations(WORD, WORD, HWND, BOOL& )
	{
		try
		{
			CDispatchWrapper wrapper(L"Felix.Preferences"); 
			CComVariant language = L"English";
			if (IDD == IDD_PROPPAGE_SEGMENTATION_J)
			{
				language = L"Japanese" ;
			}
			wrapper.method(L"EditAbbreviations", language) ;
			wrapper.m_app = NULL ;
		}
		catch (_com_error& err)
		{
			ATLASSERT(FALSE && "Raised exception in file_logger") ;
			except::CComException ce(err) ;
			ce.notify_user(_T("Abbreviations Error")) ;
		}	
		catch(except::CException &e)
		{
			logging::log_error("Failed to edit abbreviations") ;
			logging::log_exception(e) ;
			e.notify_user(_T("Abbreviations Error")) ;
		}

		return 0;
	}
	//LRESULT OnSkipJpn(WORD, WORD id, HWND, BOOL& )
	//{
	//	m_properties->m_skipJ = id ;
	//	return 0;
	//}

	BEGIN_MSG_MAP(CPageSegmentation)

		//COMMAND_HANDLER(IDC_SKIP_HASJ, BN_CLICKED, OnSkipJpn)
		//COMMAND_HANDLER(IDC_SKIP_NOJ, BN_CLICKED, OnSkipJpn)
		//COMMAND_HANDLER(IDC_NOSKIP, BN_CLICKED, OnSkipJpn)
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )

		COMMAND_HANDLER(IDC_ABBREVIATIONS, BN_CLICKED, OnAbbreviations)

		CHAIN_MSG_MAP( CPropertyPageImpl<CPageSegmentation> )

	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageSegmentation)
		DDX_CHECK(IDC_SKIP_NUMS_CHECK, m_properties->m_data.m_skipNumbers)
		DDX_CHECK(IDC_SELECT_SPACES, m_properties->m_data.m_select_spaces)
		DDX_RADIO(IDC_SKIP_HASJ, m_properties->m_data.m_skipJ)
	END_DDX_MAP()

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

	CPageSegmentE  m_page3 ;

	CString			m_title ;

public:

	app_state get_properties() { return m_properties ; }

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

	CPageSegmentJ  m_page3 ;

	CString			m_title ;

public:
	
	app_state get_properties() { return m_properties ; }
	
	enum { IDD = IDD_PROPSHEET_J };
	
	CPropertiesDlgJ( const app_state &props );
	
	BEGIN_MSG_MAP(CPropertiesDlgJ)
		CHAIN_MSG_MAP	  ( CPropertySheetImpl<CPropertiesDlgJ> )
	END_MSG_MAP()
		
};

