// PropertiesDlg.cpp: implementation of the CPropertiesDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/************************************************************************/
/* CPropertiesDlg                                                       */
/************************************************************************/

CPropertiesDlg::CPropertiesDlg() : 
	m_title( R2T( IDS_PREFERENCES_TITLE ) ),
	m_properties(new app_props::properties)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW  ;
	m_psh.pszCaption = (LPCTSTR)m_title ;

	AddPage(m_page1);
	AddPage(m_page2);
	AddPage(m_page3);
	AddPage(m_page4);
	AddPage(m_page5);

	m_properties->read_from_registry() ;

	m_page1.set_props( &m_properties->m_gen_props ) ;
	m_page2.set_props( &m_properties->m_view_props ) ;
	m_page3.set_props( &m_properties->m_mem_props ) ;
	m_page4.set_props( &m_properties->m_gloss_props ) ;
	m_page5.set_props( &m_properties->m_alg_props ) ;

	SetActivePage(0);
}

CPropertiesDlg::~CPropertiesDlg()
{

}

CString CPropertiesDlg::get_title()
{
	return m_title ;
}

app_props::props_ptr CPropertiesDlg::get_properties()
{
	return m_properties ;
}

/************************************************************************/
/* CMemoryPage                                                          */
/************************************************************************/

void CMemoryPage::set_props( app_props::properties_memory *props )
{
	m_properties = props ;
}

LRESULT CMemoryPage::OnInitDialog( UINT, WPARAM, LPARAM, BOOL& )
{
	SENSE( "OnInitDialog" ) ;

#ifdef UNIT_TEST
	return TRUE;
#else

	CUpDownCtrl min_spinner = GetDlgItem( IDC_MIN_MATCH_SPIN ) ;

	const int MINRANGE = 0 ;
	const int MAXRANGE = 100 ;
	min_spinner.SetRange(MINRANGE, MAXRANGE) ;
	min_spinner.SetPos( (int) m_properties->get_min_score() ) ;

	DoDataExchange(FALSE); // FALSE means copy TO the dialog

	return TRUE;
#endif	
}

int CMemoryPage::OnApply()
{
	DoDataExchange(TRUE) ;
	CUpDownCtrl min_spinner = GetDlgItem( IDC_MIN_MATCH_SPIN ) ;

	BOOL error = FALSE ;
	int res = min_spinner.GetPos32( &error ) ;
	ATLASSERT ( error == FALSE ) ; 

	m_properties->set_min_score( static_cast< size_t >( res ) ) ;
	return PSNRET_NOERROR  ;
}

/************************************************************************/
/* CGeneralPage                                                         */
/************************************************************************/

void CGeneralPage::set_props( app_props::properties_general *props )
{
	m_properties = props ;
}

LRESULT CGeneralPage::OnInitDialog( UINT, WPARAM, LPARAM, BOOL& )
{
	SENSE( "OnInitDialog" ) ;

#ifndef UNIT_TEST
	DoDataExchange(FALSE)  ; // FALSE means copy TO the dialog
#endif	

	return TRUE;
}

int CGeneralPage::OnApply()
{
	DoDataExchange(TRUE) ;
	return PSNRET_NOERROR  ;
}

/************************************************************************/
/* CAlgorithmPage                                                       */
/************************************************************************/

void CAlgorithmPage::set_props( app_props::properties_algorithm *props )
{
	m_properties = props ;
}

LRESULT CAlgorithmPage::OnInitDialog( UINT, WPARAM, LPARAM, BOOL& )
{
	SENSE( "OnInitDialog" ) ;

	ATLASSERT( m_properties->m_data.m_match_algo == IDC_ALGO_CHAR || 
		m_properties->m_data.m_match_algo == IDC_ALGO_WORD ||
		m_properties->m_data.m_match_algo == IDC_ALGO_AUTO) ;

#ifdef UNIT_TEST
	return TRUE ;
#else

	SendDlgItemMessage( m_properties->m_data.m_match_algo, BM_SETCHECK, TRUE, 0 ) ;

	return TRUE;
#endif
}

int CAlgorithmPage::OnApply()
{
	return PSNRET_NOERROR  ;
}

LRESULT CAlgorithmPage::OnAlgoChar( WORD, WORD, HWND, BOOL& )
{
	if ( BST_CHECKED == SendDlgItemMessage( IDC_ALGO_CHAR, BM_GETCHECK, 0, 0 ) )
	{
		m_properties->m_data.m_match_algo = IDC_ALGO_CHAR ;
	}
	return 0;
}

LRESULT CAlgorithmPage::OnAlgoWord( WORD, WORD, HWND, BOOL& )
{
	m_properties->m_data.m_match_algo = IDC_ALGO_WORD ;
	return 0;
}

LRESULT CAlgorithmPage::OnAlgoAuto( WORD, WORD, HWND, BOOL& )
{
	m_properties->m_data.m_match_algo = IDC_ALGO_AUTO;
	return 0;
}

/************************************************************************/
/* CGlossaryPage                                                        */
/************************************************************************/

void CGlossaryPage::set_props( app_props::properties_glossary *props )
{
	m_properties = props ;
}

void CGlossaryPage::init_max_add_spin()
{
	CUpDownCtrl max_add = GetDlgItem( IDC_MAX_ADD_SPIN ) ;
	const int MINRANGE = 0 ;
	const int MAXRANGE = 50 ;
	max_add.SetRange(MINRANGE, MAXRANGE) ;
	max_add.SetPos( (int) m_properties->get_max_add() ) ;
}

void CGlossaryPage::init_fuzzy_spin()
{
	CUpDownCtrl fuzzy_spin = GetDlgItem( IDC_FUZZY_SPIN ) ;
	const int MINRANGE = 0 ;
	const int MAXRANGE = 100 ;
	fuzzy_spin.SetRange(MINRANGE, MAXRANGE) ;
	fuzzy_spin.SetPos( (int) m_properties->get_min_score() ) ;
}

LRESULT CGlossaryPage::OnInitDialog( UINT, WPARAM, LPARAM, BOOL& )
{
	SENSE( "OnInitDialog" ) ;


#ifndef UNIT_TEST
	DoDataExchange(FALSE)  ; // FALSE means copy TO the dialog

	init_max_add_spin() ;
	init_fuzzy_spin() ;
#endif	

	ATLASSERT( m_properties->m_data.m_numbering == 1 || m_properties->m_data.m_numbering == 0 ) ;

#ifdef UNIT_TEST
	return TRUE;
#else

	if ( m_properties->m_data.m_numbering == 1)
	{
		SendDlgItemMessage( IDC_GLOSS_NUM_1, BM_SETCHECK, TRUE, 0 ) ;
	}
	else
	{
		SendDlgItemMessage( IDC_GLOSS_NUM_0, BM_SETCHECK, TRUE, 0 ) ;
	}


	return TRUE;
#endif
}

size_t CGlossaryPage::get_spin_pos( UINT ctl_id )
{
	CUpDownCtrl spin = GetDlgItem( ctl_id ) ;

	BOOL error = FALSE ;

	int spin_val = spin.GetPos32( &error ) ;
	ATLASSERT ( error == FALSE ) ; 

	return static_cast< size_t >( spin_val ) ;
}

void CGlossaryPage::apply_max_add_spin()
{
	// max add
	m_properties->set_max_add( get_spin_pos( IDC_MAX_ADD_SPIN ) ) ;
}

void CGlossaryPage::apply_fuzzy_spin()
{
	// min score
	m_properties->set_min_score( get_spin_pos( IDC_FUZZY_SPIN ) ) ;
}

int CGlossaryPage::OnApply()
{
	DoDataExchange(TRUE)  ;

	apply_max_add_spin() ;
	apply_fuzzy_spin() ;

	return PSNRET_NOERROR  ;
}

LRESULT CGlossaryPage::OnGlossNum( WORD wNotifyCode, WORD wID, HWND, BOOL& )
{
	wNotifyCode ;

	ATLASSERT( wID == IDC_GLOSS_NUM_1 || wID == IDC_GLOSS_NUM_0 ) ;

	if ( wID == IDC_GLOSS_NUM_1 )
	{
		m_properties->m_data.m_numbering = 1 ;
	}
	else
	{
		m_properties->m_data.m_numbering = 0 ;
	}
	return 0L;
}

/************************************************************************/
/* CViewPage                                                            */
/************************************************************************/

void CViewPage::set_props( app_props::properties_view *props )
{
	m_properties = props ;
}

LRESULT CViewPage::OnInitDialog( UINT, WPARAM, LPARAM, BOOL& )
{
	SENSE( "OnInitDialog" ) ;

#ifndef UNIT_TEST
	DoDataExchange(FALSE)  ;
#endif	

	m_back_color_box.Attach( GetDlgItem( IDC_BACK_COLOR_BOX ) ) ;
	m_query_color_box.Attach( GetDlgItem( IDC_QUERY_COLOR_BOX ) ) ;
	m_source_bkcolor_box.Attach( GetDlgItem( IDC_SOURCE_COLOR_BOX ) ) ;
	m_trans_bkcolor_box.Attach( GetDlgItem( IDC_TRANS_COLOR_BOX ) ) ;

	return TRUE;
}

int CViewPage::OnApply()
{
#ifndef UNIT_TEST
	DoDataExchange(TRUE)  ;
#endif	
	return PSNRET_NOERROR  ;
}

LRESULT CViewPage::OnPaint( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	SENSE( "OnPaint" ) ;
	bHandled = FALSE ;

	// ==============
	// Begin Paint
	// ==============
	PAINTSTRUCT ps ;
	CDC dc = BeginPaint( &ps ) ;

	// background color
	fill_rect( dc, m_back_color_box, m_properties->m_data.m_back_color ) ;
	// query color
	fill_rect( dc, m_query_color_box, m_properties->m_data.m_query_color ) ;
	// source color
	fill_rect( dc, m_source_bkcolor_box, m_properties->m_data.m_source_color ) ;
	// trans color
	fill_rect( dc, m_trans_bkcolor_box, m_properties->m_data.m_trans_color ) ;

	// ==============
	// End Paint
	// ==============
	EndPaint( &ps ) ;
	return 0L ;
}

void CViewPage::fill_rect( CDC &dc, CStaticT<TWindow> &static_control, int color )
{
	RECT rc = {0} ;
	static_control.GetWindowRect( &rc ) ;
	ScreenToClient( &rc ) ;

	dc.FillSolidRect( &rc, static_cast< COLORREF >( color ) ) ;

	const size_t POINTS_IN_RECT = 5;
	POINT points[POINTS_IN_RECT] ;
	Rect2Points(rc, points) ;
	dc.Polyline(points, POINTS_IN_RECT) ;
}

void CViewPage::Rect2Points( const RECT rc, POINT *points ) const
{
	points[0].y = rc.top ;
	points[0].x = rc.left ;

	points[1].y = rc.top ;
	points[1].x = rc.right ;

	points[2].y = rc.bottom ;
	points[2].x = rc.right ;

	points[3].y = rc.bottom ;
	points[3].x = rc.left ;

	points[4] = points[0] ;
}

LRESULT CViewPage::OnBack( WORD, WORD, HWND, BOOL& )
{
	// query user for color
	CColorDialog dialog( (COLORREF)m_properties->m_data.m_back_color ) ; // current color is default

	if ( dialog.DoModal() == IDCANCEL )
	{
		return 0 ;
	}

	// get the color the user picked
	m_properties->m_data.m_back_color = (int)dialog.GetColor() ;

	invalidate_client_rect();

	return 0;
}

LRESULT CViewPage::OnQuery( WORD, WORD, HWND, BOOL& )
{
	// query user for color
	CColorDialog dialog( (COLORREF)m_properties->m_data.m_query_color ) ; // current color is default

	if ( dialog.DoModal() == IDCANCEL )
	{
		return 0 ;
	}

	// get the color the user picked
	m_properties->m_data.m_query_color = (int)dialog.GetColor() ;

	invalidate_client_rect();

	return 0;
}

LRESULT CViewPage::OnSource( WORD, WORD, HWND, BOOL& )
{
	// query user for color
	CColorDialog dialog( (COLORREF)m_properties->m_data.m_source_color ) ; // current color is default

	if ( dialog.DoModal() == IDCANCEL )
	{
		return 0 ;
	}

	// get the color the user picked
	m_properties->m_data.m_source_color = (int)dialog.GetColor() ;

	invalidate_client_rect();

	return 0;
}

LRESULT CViewPage::OnTrans( WORD, WORD, HWND, BOOL& )
{
	// query user for color
	CColorDialog dialog( (COLORREF)m_properties->m_data.m_trans_color ) ; // current color is default

	if ( dialog.DoModal() == IDCANCEL )
	{
		return 0 ;
	}

	// get the color the user picked
	m_properties->m_data.m_trans_color = (int)dialog.GetColor() ;

	invalidate_client_rect();

	return 0;
}

void CViewPage::invalidate_client_rect()
{
	RECT rc = {0} ;
	GetClientRect( &rc ) ;
	InvalidateRect( &rc ) ;
}
