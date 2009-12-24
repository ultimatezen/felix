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
	static const int IDD = id_type ;
	CPageView();
	void set_props( app_state *props );

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	int OnApply();


	BEGIN_MSG_MAP(CPageView)

		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		CHAIN_MSG_MAP( CPropertyPageImpl<CPageView> )

	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageView)
		DDX_RADIO(IDC_LANG_ENG, m_properties->m_preferred_gui_lang )
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
	static const int IDD = id_type ;

	void set_props( app_state *props );

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);

	int OnApply();
	BOOL OnKillActive();


	BEGIN_MSG_MAP(CPageSegmentation)

		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )

		COMMAND_HANDLER(IDC_SKIP_HASJ, BN_CLICKED, OnSkip)
		COMMAND_HANDLER(IDC_SKIP_NOJ, BN_CLICKED, OnSkip)
		COMMAND_HANDLER(IDC_NOSKIP, BN_CLICKED, OnSkip)

		CHAIN_MSG_MAP( CPropertyPageImpl<CPageSegmentation> )

	END_MSG_MAP()


	LRESULT OnSkip(WORD, WORD skip_code, HWND, BOOL& );

	BEGIN_DDX_MAP(CSegmentation)
		DDX_CHECK(IDC_SKIP_NUMS_CHECK, m_properties->m_skipNumbers)
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
	static const int IDD = id_type ;

	void set_props( app_state *props );

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);

	int OnApply();
	BOOL OnKillActive();


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
	CPageSegmentE	m_page2 ;
	CPageTransHistE m_page3 ;

	CString			m_title ;

public:

	app_state get_properties() { return m_properties ; }
	static const int IDD = IDD_PROPSHEET_E ;

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
	CPageSegmentJ	m_page2 ;
	CPageTransHistJ m_page3 ;

	CString			m_title ;

public:

	app_state get_properties() { return m_properties ; }

	static const int IDD = IDD_PROPSHEET_J ;

	CPropertiesDlgJ( const app_state &props );

	BEGIN_MSG_MAP(CPropertiesDlgJ)
		CHAIN_MSG_MAP	  ( CPropertySheetImpl<CPropertiesDlgJ> )
	END_MSG_MAP()

};

