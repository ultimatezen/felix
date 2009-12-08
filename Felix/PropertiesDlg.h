#pragma once

#include <atlapp.h>	// must be included before atldlgs.h
#include "atldlgs.h"
#include "atlddx.h"
#include "atlctrls.h" // CStatic

#include "Felix_properties.h"


/** @class CMemoryPage */
class CMemoryPage :	public CPropertyPageImpl<CMemoryPage>,
	public CWinDataExchange< CMemoryPage >
{

	// prop values
	app_props::properties_memory	*m_properties ;

public:

	enum { IDD = IDD_PROPPAGE_MEMORY };
	DECLARE_SENSING_VAR ;

	void set_props( app_props::properties_memory *props );

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	int OnApply();


	BEGIN_MSG_MAP(CMemoryPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CMemoryPage>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CMemoryPage)
		DDX_CHECK( IDC_IGNORE_CASE_CHECK,		m_properties->m_data.m_ignore_case)
		DDX_CHECK( IDC_PLAINTEXT_CHECK,			m_properties->m_data.m_plaintext)
		DDX_CHECK( IDC_FORMATTING_CHECK,		m_properties->m_data.m_assess_format_penalty)
		DDX_CHECK( IDC_IGNORE_WIDTH_CHECK,		m_properties->m_data.m_ignore_width)
		DDX_CHECK( IDC_IGNORE_HIR_KAT_CHECK,	m_properties->m_data.m_ignore_hir_kat)
		DDX_CHECK( IDC_PLACE_NUMBERS_CHECK,		m_properties->m_data.m_place_numbers)
		DDX_CHECK( IDC_PLACE_GLOSS_CHECK,		m_properties->m_data.m_place_gloss)
	END_DDX_MAP()
};

/**
@class CGeneralPage 
@brief Property sheet General page.
*/
class CGeneralPage :	
	public CPropertyPageImpl< CGeneralPage >,
	public CWinDataExchange< CGeneralPage >
{

	// prop values
	app_props::properties_general	*m_properties ;

public:

	enum { IDD = IDD_PROPPAGE_GENERAL };
	DECLARE_SENSING_VAR ;

	void set_props( app_props::properties_general *props );

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	int OnApply();


	BEGIN_MSG_MAP(CGeneralPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CGeneralPage>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CGeneralPage)
		DDX_CHECK( IDC_LOAD_MEMS,		m_properties->m_data.m_load_prev_mem_on_startup)
		DDX_CHECK( IDC_LOAD_GLOSS,		m_properties->m_data.m_load_prev_gloss_on_startup)
		DDX_CHECK( IDC_ASK_MERGE,		m_properties->m_data.m_query_merge)
		DDX_TEXT_LEN( IDC_USER_NAME,	m_properties->m_data.m_user_name, MAX_PATH)
	END_DDX_MAP()
};


/**
@class CAlgorithmPage	
@brief Property Sheet.
*/
class CAlgorithmPage :	
	public CPropertyPageImpl<CAlgorithmPage>
	// 	, public CWinDataExchange< CAlgorithmPage >
{

	// prop values
	app_props::properties_algorithm	*m_properties ;

public:

	enum { IDD = IDD_PROPPAGE_ALGORITHM };
	DECLARE_SENSING_VAR ;

	void set_props( app_props::properties_algorithm *props );

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	int OnApply();


	LRESULT OnAlgoChar(WORD, WORD, HWND, BOOL& );
	LRESULT OnAlgoWord(WORD, WORD, HWND, BOOL& );
	LRESULT OnAlgoAuto(WORD, WORD, HWND, BOOL& );

	BEGIN_MSG_MAP(CAlgorithmPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

		COMMAND_HANDLER(IDC_ALGO_AUTO, BN_CLICKED, OnAlgoAuto)
		COMMAND_HANDLER(IDC_ALGO_CHAR, BN_CLICKED, OnAlgoChar)
		COMMAND_HANDLER(IDC_ALGO_WORD, BN_CLICKED, OnAlgoWord)

		CHAIN_MSG_MAP(CPropertyPageImpl<CAlgorithmPage>)
	END_MSG_MAP()

};

/**
@class CGlossaryPage 
@brief Glossary Property Sheet page.
*/
class CGlossaryPage :	public CPropertyPageImpl<CGlossaryPage>,
	public CWinDataExchange< CGlossaryPage>
{

	// prop values
	app_props::properties_glossary	*m_properties ;

public:

	enum { IDD = IDD_PROPPAGE_GLOSSARY };
	DECLARE_SENSING_VAR ;

	void set_props( app_props::properties_glossary *props );

	void init_max_add_spin();
	void init_fuzzy_spin();

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);

	size_t get_spin_pos( UINT ctl_id );
	void apply_max_add_spin();
	void apply_fuzzy_spin();
	int OnApply();

	LRESULT OnGlossNum(WORD wNotifyCode, WORD wID, HWND, BOOL& );

	BEGIN_DDX_MAP(CGlossaryPage)
		DDX_CHECK( IDC_GLOSS_IGNORE_CASE_CHECK,		m_properties->m_data.m_ignore_case)
		DDX_CHECK( IDC_GLOSS_PLAINTEXT_CHECK,		m_properties->m_data.m_plaintext)
		DDX_CHECK( IDC_GLOSS_TO_LOWER_CHECK,		m_properties->m_data.m_to_lower)
		DDX_CHECK( IDC_GLOSS_IGNORE_WIDTH_CHECK,	m_properties->m_data.m_ignore_width)
		DDX_CHECK( IDC_GLOSS_IGNORE_HIR_KAT_CHECK,	m_properties->m_data.m_ignore_hir_kat)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CGlossaryPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CGlossaryPage>)

		COMMAND_HANDLER(IDC_GLOSS_NUM_0, BN_CLICKED, OnGlossNum)
		COMMAND_HANDLER(IDC_GLOSS_NUM_1, BN_CLICKED, OnGlossNum)
	END_MSG_MAP()

};

/**
@class CViewPage :	
@brief View Property Sheet.
*/
class CViewPage :	
	public CPropertyPageImpl< CViewPage >,
	public CWinDataExchange< CViewPage >
{
	CStaticT<TWindow>			m_back_color_box ;
	CStaticT<TWindow>			m_query_color_box ;
	CStaticT<TWindow>			m_source_bkcolor_box ;
	CStaticT<TWindow>			m_trans_bkcolor_box ;

	// prop values
	app_props::properties_view *m_properties ;

public:

	enum { IDD = IDD_PROPPAGE_VIEW };
	DECLARE_SENSING_VAR ;

	void set_props( app_props::properties_view *props );

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	int OnApply();

	BEGIN_DDX_MAP(CViewPage)
		DDX_CHECK( IDC_ONE_SCREEN_MATCH,	m_properties->m_data.m_single_screen_matches)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CViewPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)

		COMMAND_HANDLER(IDC_BACK_BTN,   BN_CLICKED, OnBack)
		COMMAND_HANDLER(IDC_QUERY_BTN,  BN_CLICKED, OnQuery)
		COMMAND_HANDLER(IDC_SOURCE_BTN, BN_CLICKED, OnSource)
		COMMAND_HANDLER(IDC_TRANS_BTN,  BN_CLICKED, OnTrans)

		CHAIN_MSG_MAP(CPropertyPageImpl<CViewPage>)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled );

	void fill_rect( CDC &dc, CStaticT<TWindow> &static_control, int color );
	void Rect2Points( RECT &rc, POINT *points );

	LRESULT OnBack(WORD, WORD, HWND, BOOL& );
	LRESULT OnQuery(WORD, WORD, HWND, BOOL& );
	LRESULT OnSource(WORD, WORD, HWND, BOOL& );

	void invalidate_client_rect();
	LRESULT OnTrans(WORD, WORD, HWND, BOOL& );
};

/**
@class CPropertiesDlg  
@brief Property Sheet for app properties.
*/
class CPropertiesDlg  
	: public CPropertySheetImpl<CPropertiesDlg>
{
	app_props::props_ptr	m_properties ;
	CString					m_title ;

	CGeneralPage	m_page1 ;
	CViewPage		m_page2 ;
	CMemoryPage		m_page3 ;
	CGlossaryPage	m_page4 ;
	CAlgorithmPage	m_page5 ;

public:
	enum { IDD = IDD_PROPSHEET };
	DECLARE_SENSING_VAR ;

	CString get_title();
	app_props::props_ptr get_properties();

	CPropertiesDlg( );
	virtual ~CPropertiesDlg();

	BEGIN_MSG_MAP(CPropertiesDlg)
		CHAIN_MSG_MAP	  ( CPropertySheetImpl<CPropertiesDlg> )
	END_MSG_MAP()
};
