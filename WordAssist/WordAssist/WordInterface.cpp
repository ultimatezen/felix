/********************************************************************
	Date:	    2004/11/09
	Time:   	16:12:39
	filename: 	C:\Programming\Programming Projects\TransAssist Interface for Word\WordInterface.cpp
	file path:	C:\Programming\Programming Projects\TransAssist Interface for Word
	file base:	WordInterface
	file ext:	cpp
	author:		Ryan Ginstrom
	
	purpose:	Implementation of CWordInterface
*********************************************************************/

// Code walkthrough [2004-11-9]

#include "stdafx.h"
#include ".\wordinterface.h"

#include "TransAssist Interface for Word.h"

#include "WordInterface.h"
#include "hooker.h"
#include "File.h"
#include "Path.h"

#include <stdio.h>
#include <new.h>

#include "ErrorTranslator.h"


#include "WordControllerHtml.h"
#include "WordControllerWord.h"

#include "ocidl.h"

#include "TestResultsPrinter.h"
#include "ProgressBarTestRunner.h"

#ifdef _DEBUG

using namespace easyunit ;


#endif // #ifdef _DEBUG

#define _HR( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) \
	{ \
		throw CComException( _T("COM Failure"), _hr, _T(__FILE__), __LINE__ ) ; \
	} \
}

#define _HR_RET( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) \
	{ \
		return _hr ; \
	} \
}

#define _HR_V( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) \
	{ \
		return  ; \
	} \
}

#define _HR_NULL( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) return NULL ; \
}

#define _HR_FALSE( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( _hr == S_FALSE ) return _hr ;\
	if ( FAILED( _hr ) ) \
	{ \
		throw CComException( _T("COM Failure"), _hr, _T(__FILE__), __LINE__ ) ; \
	} \
}

#define _HRB( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) return false ; \
}

#define CATCH_C_EXCEPTIONS(_msg) catch ( std::exception &e ) { label_name msg( _msg ) ; return handle_exception( e, msg ) ; }
#define CATCH_COM_EXCEPTIONS(_msg) catch ( CComException &e ) { label_name msg( _msg ) ; return handle_exception( e, msg ) ; }
#define CATCH_WIN_EXCEPTIONS(_msg) catch ( CWinException &e ) { label_name msg( _msg ) ; return handle_exception( e, msg ) ; }
#define CATCH_SW_EXCEPTIONS(_msg) catch ( CSWException &e ) { label_name msg( _msg ) ; return handle_exception( e, msg ) ; }
#define CATCH_EXCEPTIONS(_msg) catch ( CException &e ) { label_name msg( _msg ) ; return handle_exception( e, msg ) ; }
#define CATCH_COM_ERRORS(_msg) catch (_com_error &e ) { label_name msg( _msg ) ; return handle_exception( e, msg ) ; }

#define CATCH_ALL(msg) } CATCH_C_EXCEPTIONS(msg) CATCH_COM_EXCEPTIONS(msg) CATCH_WIN_EXCEPTIONS(msg) CATCH_SW_EXCEPTIONS(msg) CATCH_EXCEPTIONS(msg) CATCH_COM_ERRORS(msg)

#define JAPANESE_OFFSET ( IDS_MENU_JAPANESE_FIRST - IDS_MENU_ENGLISH_FIRST )

_ATL_FUNC_INFO OnClickButtonInfo =
{CC_STDCALL,VT_EMPTY,2,{VT_DISPATCH ,VT_BYREF | VT_BOOL}};

_ATL_FUNC_INFO WindowActivateInfo = 
{CC_STDCALL,VT_EMPTY,2,{VT_DISPATCH | VT_BYREF ,VT_DISPATCH | VT_BYREF }};
_ATL_FUNC_INFO WindowDeactivateInfo = 
{CC_STDCALL,VT_EMPTY,2,{VT_DISPATCH | VT_BYREF ,VT_DISPATCH | VT_BYREF }};

// 0x00000003
_ATL_FUNC_INFO DocumentChangeInfo = 
{CC_STDCALL,VT_EMPTY,0,VT_EMPTY};

// 0x00000004
_ATL_FUNC_INFO DocumentOpenInfo = 
{CC_STDCALL,VT_EMPTY,1,{VT_DISPATCH | VT_BYREF}} ;

// 0x00000009
_ATL_FUNC_INFO NewDocumentInfo = 
{CC_STDCALL,VT_EMPTY,1,{VT_DISPATCH | VT_BYREF}} ;

// 0x0000000C
_ATL_FUNC_INFO WindowSelectionChangeInfo = 
{CC_STDCALL,VT_EMPTY,1,{VT_DISPATCH | VT_BYREF}} ;

/////////////////////////////////////////////////////////////////////////////
// CWordInterface

STDMETHODIMP CWordInterface::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (
		( (PLONG) &IID_IWordInterface ) [0] == ( (PLONG) &riid ) [0] &&
		( (PLONG) &IID_IWordInterface ) [1] == ( (PLONG) &riid ) [1] &&
		( (PLONG) &IID_IWordInterface ) [2] == ( (PLONG) &riid ) [2] &&
		( (PLONG) &IID_IWordInterface ) [3] == ( (PLONG) &riid ) [3] )
		return S_OK ;

	return S_FALSE;
}



// ===================================
// _IDTExtensibility2
// ===================================


// Function name	: CWordInterface::OnConnection
// Description	    : 
// Return type		: STDMETHODIMP 
// Argument         : IDispatch * Application
// Argument         : ext_ConnectMode ConnectMode
// Argument         : IDispatch * AddInInst
// Argument         : SAFEARRAY * * custom
STDMETHODIMP CWordInterface::OnConnection(IDispatch * Application, 
										  ext_ConnectMode ConnectMode, 
										  IDispatch * AddInInst, 
										  SAFEARRAY * * custom )
{
	ConnectMode ;
	custom ;

	BANNER( "CWordInterface::OnConnection" ) ;

	setup_stuff() ;
	
	AUTO_TRY

		COM_ENFORCE( Application->QueryInterface(__uuidof(MSWord::_Application), out_ptr( &m_app ) ), "Failed to QI Word app" );
		COM_ENFORCE( AddInInst->QueryInterface(__uuidof(IDispatch), out_ptr( &m_pAddInInstance ) ), "Failed to QI Word instance" ) ;

	CATCH_ALL( _T("OnConnection") )

	return S_OK ;
}

// Function name	: CWordInterface::OnDisconnection
// Description	    : 
// Return type		: STDMETHODIMP 
// Argument         : ext_DisconnectMode RemoveMode
// Argument         : SAFEARRAY * * custom
STDMETHODIMP CWordInterface::OnDisconnection(ext_DisconnectMode RemoveMode, SAFEARRAY * * custom)
{
	custom ;
	RemoveMode ;

	BANNER( "CWordInterface::OnDisconnection" ) ;

	AUTO_TRY

		m_app = NULL ;
		m_pAddInInstance = NULL ;

	CATCH_ALL( _T("OnDisconnection") )

	return S_OK ;
}

// Function name	: CWordInterface::OnAddInsUpdate
// Description	    : 
// Return type		: STDMETHODIMP 
// Argument         : SAFEARRAY * * custom
STDMETHODIMP CWordInterface::OnAddInsUpdate(SAFEARRAY * * custom)
{
	custom ;

	BANNER( "CWordInterface::OnAddInsUpdate" ) ;

	AUTO_TRY
		

	CATCH_ALL(_T("OnAddInsUpdate"))
	
	return S_OK ;
}
// Function name	: CWordInterface::OnStartupComplete
// Description	    : 
// Return type		: STDMETHODIMP 
// Argument         : SAFEARRAY * * custom
STDMETHODIMP CWordInterface::OnStartupComplete(SAFEARRAY * * custom)
{
	custom ;

	BANNER( "CWordInterface::OnStartupComplete" ) ;

	AUTO_TRY

		m_properties.read_from_registry() ;
		m_controller = get_listener( m_properties.get_segmentation_type() ) ;

		ATLASSERT(m_app);
		m_controller->set_app( m_app ) ;

		// Get the CommandBars interface that represents Word's
		//toolbars & menu items    

		office_cmd_bars spCmdBars = m_app->GetCommandBars( ) ;
		ATLASSERT(spCmdBars);

		HRESULT hr = S_OK ;
		hr = add_toolbar( spCmdBars ) ;
		ASSERT_HRESULT( hr ) ;

		if ( m_properties.get_classic_if() ) 
		{
			hr = add_classic_menu( spCmdBars ) ;
			ASSERT_HRESULT( hr ) ;

		}
		else
		{
			hr = add_menu( spCmdBars ) ;
			ASSERT_HRESULT( hr ) ;
			switch_to_translation_toolbar() ;
		}

		advise_document_events() ;

		installhook( static_cast<CKeyboardListener *>(m_controller) ) ;

		m_controller->refresh_properties( ) ;

	#ifdef _DEBUG

		try
		{
			CATLResultsPrinter printer ;
			CProgressBarTestRunner runner ;
			TestRegistry::runAndPrint( 
				static_cast< TestPrinter* >( &printer ), 
				static_cast< TestRunner* >( &runner ) ) ;
		}
		catch( ... )
		{
			ATLASSERT( "OOPS!" && false ) ;
		}

	#endif // #ifdef _DEBUG

	CATCH_ALL( _T("OnStartupComplete") )
	
	return S_OK ;
}
// Function name	: CWordInterface::OnBeginShutdown
// Description	    : 
// Return type		: STDMETHODIMP 
// Argument         : SAFEARRAY * * custom
STDMETHODIMP CWordInterface::OnBeginShutdown(SAFEARRAY * * custom)
{
	custom ;

	BANNER( "CWordInterface::OnBeginShutdown" ) ;

	AUTO_TRY
		
		persist_app_state() ;

		//m_gloss_menu_popup->Delete() ;
		//m_trans_assist_menu_popup->Delete() ;

		unadvise_document_events() ;

		unadvise_button_items() ;

		unadvise_menu_items() ;

		uninstallhook( static_cast<CKeyboardListener *>(m_controller) ) ;

	CATCH_ALL( _T("OnBeginShutdown") )
	
	return S_OK ;
}

// Function name	: CWordInterface::add_menu
// Description	    : 
// Return type		: void 
// Argument         : office_cmd_bars &spCmdBars
HRESULT CWordInterface::add_menu( office_cmd_bars &spCmdBars )
{
	BANNER( "CWordInterface::add_menu" ) ;

	MSOffice::CommandBarPtr spActiveMenuBar = spCmdBars->GetActiveMenuBar() ;
	
	office_cmd_bar_ctls spCmdBarCtrls = spActiveMenuBar->GetControls() ;

	kill_old_menu( spCmdBarCtrls ) ;

	set_customization_context() ;	

	_variant_t vType( MSOffice::msoControlPopup ) ;
	
	_variant_t vTemp(VARIANT_FALSE) ; // menu is temporary        

	m_trans_assist_menu_popup = spCmdBarCtrls->Add(
			vType,					// type
			vtMissing,				// id
			vtMissing, 				// parameter
			vtMissing, 				// before
			vTemp					// temporary
			) ;

	m_trans_assist_menu_popup->Caption = L"T&ransAssist" ;

	CComPtr< IDispatch > spDispatch ;
	_HR_RET( m_trans_assist_menu_popup->get_Control( &spDispatch ) ) ;

	CComQIPtr< MSOffice::CommandBarPopup > spCmdBarPopup( spDispatch ) ;
	ATLASSERT( spCmdBarPopup) ;
	_HR_RET( spCmdBarPopup->put_Visible( VARIANT_TRUE ) ) ;

	office_cmd_bar_ctls spMenuControls;
	COM_ENFORCE( spCmdBarPopup->get_Controls(&spMenuControls), _T("Failed to get menu bar controls") ) ;

	int string_offset = 0 ;

	if ( m_properties.get_preferred_gui_lang() == LANG_JAPANESE )
	{
		string_offset += JAPANESE_OFFSET ;
	}

	if ( m_properties.get_classic_if() ) 
	{
		string_offset += ( IDS_CLASSIC_MENU_ENGLISH_FIRST - IDS_MENU_ENGLISH_FIRST ) ;
	}

	m_menu_auto_trans = add_menu_item( spMenuControls, 0, IDS_MENU_AUTO_TRANS_SEL + string_offset) ;
	ATLASSERT( m_menu_auto_trans ) ;
	
	m_menu_auto_trans_fuzzy = add_menu_item( spMenuControls, 0, IDS_MENU_AUTO_TRANS_FUZZY + string_offset) ;
	ATLASSERT( m_menu_auto_trans_fuzzy ) ;
	
	m_menu_get = add_menu_item( spMenuControls, IDB_GET, IDS_MENU_GET + string_offset) ;
	ATLASSERT( m_menu_get ) ;
	m_menu_get->put_BeginGroup( VARIANT_TRUE ) ;

	m_menu_get_and_next = add_menu_item( spMenuControls, IDB_GET_AND_NEXT, IDS_MENU_GET_AND_NEXT + string_offset) ;
	ATLASSERT( m_menu_get_and_next ) ;
	
	m_menu_set = add_menu_item( spMenuControls, IDB_SET, IDS_MENU_REGISTER + string_offset) ;
	ATLASSERT( m_menu_set ) ;
	m_menu_set->put_BeginGroup( VARIANT_TRUE ) ;
	
	m_menu_set_and_next = add_menu_item( spMenuControls, IDB_SET_AND_NEXT, IDS_MENU_SET_AND_NEXT + string_offset) ;
	ATLASSERT( m_menu_set_and_next ) ;
	
	m_menu_lookup = add_menu_item( spMenuControls, IDB_LOOKUP, IDS_MENU_LOOKUP + string_offset ) ;
	ATLASSERT( m_menu_lookup ) ;
	m_menu_lookup->put_BeginGroup( VARIANT_TRUE ) ;
	
	m_menu_lookup_next = add_menu_item( spMenuControls, IDB_LOOKUP_NEXT, IDS_MENU_LOOKUP_NEXT + string_offset ) ;
	ATLASSERT( m_menu_lookup_next ) ;
	
	m_menu_extend_lookup = add_menu_item( spMenuControls, 0, IDS_MENU_EXTEND + string_offset ) ;
	ATLASSERT( m_menu_extend_lookup ) ;
	
	m_menu_concordance = add_menu_item( spMenuControls, 0, IDS_MENU_CONCORDANCE + string_offset ) ;
	ATLASSERT( m_menu_concordance ) ;
	
	m_menu_register_gloss = add_menu_item( spMenuControls, 0, IDS_MENU_REGISTER_GLOSS + string_offset ) ;
	ATLASSERT( m_menu_register_gloss ) ;
	m_menu_register_gloss->put_BeginGroup( VARIANT_TRUE ) ;
	
	m_menu_delete = add_menu_item( spMenuControls, IDB_DELETE, IDS_MENU_DELETE + string_offset ) ;
	ATLASSERT( m_menu_delete ) ;
	
	m_menu_next = add_menu_item( spMenuControls, 0, IDS_MENU_NEXT + string_offset ) ;
	ATLASSERT( m_menu_next ) ;
	
	m_menu_prev = add_menu_item( spMenuControls, 0, IDS_MENU_PREV + string_offset ) ;
	ATLASSERT( m_menu_prev ) ;
	
	COM_ENFORCE( spMenuControls->raw_Add(
		vType,
		vtMissing,
		vtMissing,
		vtMissing,
		vTemp,
		&m_gloss_menu_popup
		),
		_T("Failed to get controls for menu item") 
		) ;
	
	CComBSTR caption ; caption.LoadString( IDS_MENU_GET_GLOSS + string_offset ) ;
	CComBSTR tooltip ; tooltip.LoadString( IDS_MENU_GET_GLOSS_TT + string_offset ) ;
	_HR_RET( m_gloss_menu_popup->put_Caption( caption ) ) ;
	_HR_RET( m_gloss_menu_popup->put_DescriptionText( tooltip ) );
	_HR_RET( m_gloss_menu_popup->put_BeginGroup( VARIANT_TRUE ) ) ;
	
	CComPtr< IDispatch > spGlossDispatch ;
	_HR_RET( m_gloss_menu_popup->get_Control( &spGlossDispatch ) ) ;
	
	CComQIPtr< MSOffice::CommandBarPopup > spGlossPopup( spGlossDispatch ) ;
	_HR_RET( spCmdBarPopup->put_Visible( VARIANT_TRUE ) ) ;
	
	office_cmd_bar_ctls spGlossMenuControls;
	COM_ENFORCE( spGlossPopup->get_Controls(&spGlossMenuControls), _T("Failed to get menu bar controls") ) ;
	
		m_menu_entry_0 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_0 + string_offset ) ;
		ATLASSERT( m_menu_entry_0 ) ;
		
		m_menu_entry_1 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_1 + string_offset ) ;
		ATLASSERT( m_menu_entry_1 ) ;
		
		m_menu_entry_2 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_2 + string_offset ) ;
		ATLASSERT( m_menu_entry_2 ) ;
		
		m_menu_entry_3 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_3 + string_offset ) ;
		ATLASSERT( m_menu_entry_3 ) ;
		
		m_menu_entry_4 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_4 + string_offset ) ;
		ATLASSERT( m_menu_entry_4 ) ;
		
		m_menu_entry_5 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_5 + string_offset ) ;
		ATLASSERT( m_menu_entry_5 ) ;
		
		m_menu_entry_6 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_6 + string_offset ) ;
		ATLASSERT( m_menu_entry_6 ) ;
		
		m_menu_entry_7 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_7 + string_offset ) ;
		ATLASSERT( m_menu_entry_7 ) ;
		
		m_menu_entry_8 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_8 + string_offset ) ;
		ATLASSERT( m_menu_entry_8 ) ;
		
		m_menu_entry_9 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_9 + string_offset ) ;
		ATLASSERT( m_menu_entry_9 ) ;
		
		m_menu_gloss_n = add_menu_item( spGlossMenuControls, IDB_GLOSS_N, IDS_MENU_GLOSS_N + string_offset ) ;
		ATLASSERT( m_menu_gloss_n ) ;

	m_menu_save = add_menu_item( spMenuControls, IDB_SAVE, IDS_MENU_SAVE + string_offset )	;
	ATLASSERT( m_menu_save ) ;
	m_menu_save->put_BeginGroup( VARIANT_TRUE ) ;

	m_menu_switch = add_menu_item( spMenuControls, IDB_SWITCH_TO_REVIEW, IDS_MENU_TO_REVIEW_MODE + string_offset ) ;
	ATLASSERT( m_menu_switch ) ;

	m_menu_preferences = add_menu_item( spMenuControls, IDB_PROPERTIES, IDS_MENU_PREFERENCES + string_offset ) ;
	ATLASSERT( m_menu_preferences ) ;

	advise_menu_items() ;

	block_save_in_normal_dot() ;

	return S_OK ;
}


// Function name	: CWordInterface::add_menu_item
// Description	    : 
// Return type		: command_button_ptr 
// Argument         : office_cmd_bar_ctls &controls
// Argument         : int button_id
command_button_ptr CWordInterface::add_menu_item(office_cmd_bar_ctls &controls, int button_id, int string_id )
{
	BANNER( "CWordInterface::add_menu_item" ) ;

	office_cmd_bar_ctl spMenuItemCtrl ;
	CComVariant vTemp(VARIANT_FALSE); // menu is not temporary        
	COM_ENFORCE( controls->raw_Add(
		CComVariant( MSOffice::msoControlButton ),
		vtMissing,
		vtMissing,
		vtMissing,
		vTemp,
		&spMenuItemCtrl
		),
		_T("Failed to get controls for menu item") 
		) ;
	
	CComPtr< IDispatch > spDispatch ;
	_HR_NULL( spMenuItemCtrl->get_Control( &spDispatch ) ) ;
	command_button_ptr menu_button ;
	_HR_NULL( spDispatch->QueryInterface( __uuidof(MSOffice::_CommandBarButton ), out_ptr( &menu_button ) ) ) ;
	
	// to set a bitmap to a button, load a 32x32 bitmap
	// and copy it to clipboard. Call CommandBarButton's PasteFace()
	// to copy the bitmap to the button face. to use
	// PowerPoint's set of predefined bitmap, set button's FaceId to  the
	// button whose bitmap you want to use
	
	// set style before setting bitmap
	
	if (  button_id != 0 && set_button_image( menu_button, button_id) ) 
	{
		_HR_NULL( menu_button->put_Style( MSOffice::msoButtonIconAndCaption ) ) ;
		
	}
	else
	{
		_HR_NULL( menu_button->put_Style( MSOffice::msoButtonCaption ) ) ;
		
	}

	set_menu_text( menu_button, string_id ) ;

	CComBSTR tag_text ;
	tag_text.LoadString( string_id ) ;
	tag_text += L"Menu" ;
	set_tag_text( menu_button, tag_text ) ;

	// show the menu 
	
	_HR_NULL( menu_button->put_Visible( VARIANT_TRUE ) ) ;

	return menu_button ;
}


// Function name	: CWordInterface::add_toolbar
// Description	    : 
// Return type		: void 
// Argument         : office_cmd_bars &spCmdBars
HRESULT CWordInterface::add_toolbar( office_cmd_bars &spCmdBars )
{
	BANNER( "CWordInterface::add_toolbar" ) ;

	// now we add a new toolband to Word
	// to which we'll add our buttons
	CComVariant vName("TransAssist Interface");

	// position it below all toolbands
	//MsoBarPosition::msoBarTop = 1
	CComVariant vPos(1); 

	CComVariant vTemp(VARIANT_TRUE); // menu is temporary        
	//Add a new toolband through Add method
	// vMenuTemp holds an unspecified parameter
	//m_toolbar points to the newly created toolband
	_HR_RET( spCmdBars->raw_Add( vName, vPos, vtMissing, vTemp, &m_toolbar ) ) ;

	if ( m_properties.get_toolbar_visible() )
	{
		_HR_RET( m_toolbar->put_Visible( VARIANT_TRUE ) ) ;
	}
	else
	{
		_HR_RET( m_toolbar->put_Visible( VARIANT_FALSE ) ) ;
	}

	//now get the toolband's CommandBarControls
	
	// connect to event source in OnConnection
	// m_spButton member variable is a smart pointer to _CommandBarButton
	// that is used to cache the pointer to the first toolbar button.
	
	office_cmd_bar_ctls spBarControls;
	_HR_RET( m_toolbar->get_Controls( &spBarControls ) ) ;
	ATLASSERT( spBarControls ) ;

	int string_offset = 0 ;
	
	if ( m_properties.get_preferred_gui_lang() == LANG_JAPANESE )
	{
		string_offset = JAPANESE_OFFSET ;
	}
	

	// add buttons
	m_button_lookup = add_toolbar_item( spBarControls, IDB_LOOKUP, IDS_MENU_LOOKUP + string_offset ) ;
	ATLASSERT( m_button_lookup ) ;
	_HR_RET( ButtonLookupEventImpl::DispEventAdvise( (IUnknown*)m_button_lookup ) ) ;
	
	m_button_lookup_next = add_toolbar_item( spBarControls, IDB_LOOKUP_NEXT, IDS_MENU_LOOKUP_NEXT + string_offset ) ;
	ATLASSERT( m_button_lookup_next ) ;
	_HR_RET( ButtonLookupNextEventImpl::DispEventAdvise( (IUnknown*)m_button_lookup_next ) ) ;
	
	m_button_get = add_toolbar_item( spBarControls, IDB_GET, IDS_MENU_GET + string_offset ) ;
	ATLASSERT( m_button_get ) ;
	_HR_RET( ButtonGetEventImpl::DispEventAdvise( (IUnknown*)m_button_get ) ) ;
	
	m_button_get_and_next = add_toolbar_item( spBarControls, IDB_GET_AND_NEXT, IDS_MENU_GET_AND_NEXT + string_offset ) ;
	ATLASSERT( m_button_get_and_next ) ;
	_HR_RET( ButtonGetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_button_get_and_next ) ) ;
	
	m_button_set = add_toolbar_item( spBarControls, IDB_SET, IDS_MENU_REGISTER + string_offset ) ;
	ATLASSERT( m_button_set ) ;
	_HR_RET( ButtonSetEventImpl::DispEventAdvise( (IUnknown*)m_button_set ) ) ;
	
	m_button_set_and_next = add_toolbar_item( spBarControls, IDB_SET_AND_NEXT, IDS_MENU_SET_AND_NEXT + string_offset ) ;
	ATLASSERT( m_button_set_and_next ) ;
	_HR_RET( ButtonSetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_button_set_and_next ) ) ;

	m_button_gloss_n = add_toolbar_item( spBarControls, IDB_GLOSS_N, IDS_MENU_GLOSS_N + string_offset ) ;
	ATLASSERT( m_button_gloss_n ) ;
	_HR_RET( ButtonGlossNEventImpl::DispEventAdvise( (IUnknown*)m_button_gloss_n ) ) ;

	m_button_help = add_toolbar_item( spBarControls, IDB_HELP, IDS_MENU_HELP + string_offset ) ;
	ATLASSERT( m_button_help ) ;
	_HR_RET( ButtonHelpEventImpl::DispEventAdvise( (IUnknown*)m_button_help ) ) ;
   
	return S_OK ;
}


// Function name	: CWordInterface::add_toolbar_item
// Description	    : 
// Return type		: command_button_ptr 
// Argument         : office_cmd_bar_ctls &controls
// Argument         : int button_id
command_button_ptr CWordInterface::add_toolbar_item(office_cmd_bar_ctls &controls, int button_id, int string_id)
{
	BANNER( "CWordInterface::add_toolbar_item" ) ;

	//MsoControlType::msoControlButton = 1
	CComVariant vToolBarType(1);
	//show the toolbar?
	CComVariant vShow(VARIANT_TRUE);
	
	// add buttons
	office_cmd_bar_ctl spNewBar; 
	CComVariant vEmpty(DISP_E_PARAMNOTFOUND, VT_ERROR);            
	_HR_NULL( controls->raw_Add(vToolBarType,vEmpty,vEmpty,vEmpty,vShow, &spNewBar) ) ;


	CComPtr< IDispatch > spDispatch ;
	_HR_NULL( spNewBar->get_Control( &spDispatch ) ) ;
	command_button_ptr button ;
	_HR_NULL( spDispatch->QueryInterface( __uuidof(MSOffice::_CommandBarButton), out_ptr( &button ) ) ) ;

	// to set a bitmap to a button, load a 32x32 bitmap
	// and copy it to clipboard. Call CommandBarButton's PasteFace()
	// to copy the bitmap to the button face. to use
	// PowerPoint's set of predefined bitmap, set button's FaceId to     //the
	// button whose bitmap you want to use
	set_button_image( button, button_id ) ;

	// set style before setting bitmap
	_HR_NULL( button->put_Style( MSOffice::msoButtonIcon ) ) ;

	set_button_text( button, string_id ) ;

	CComBSTR tag_text ;
	tag_text.LoadString( string_id ) ;
	tag_text += L"Toolbar" ;
	set_tag_text( button, tag_text ) ;
	
	button->put_Enabled(VARIANT_TRUE);
	button->put_Visible(VARIANT_TRUE); 
	
	return button ;
}

void CWordInterface::gui_to_english()
{
	gui_to_language( 0 ) ;
}

void CWordInterface::gui_to_japanese()
{
	gui_to_language( JAPANESE_OFFSET ) ;
}

void CWordInterface::kill_old_menu(office_cmd_bar_ctls controls)
{
	int menu_count = 0 ;
	_HR( controls->get_Count( &menu_count ) ) ;
	
	wstring marked_for_death(L"transassist") ;
	
	// 1-based index for COM
	int i = 1 ;
	while ( i <= menu_count )
	{
		office_cmd_bar_ctl current_command_bar ;
		_HR( controls->get_Item( CComVariant( i ), &current_command_bar ) );

		CComBSTR caption ;
		_HR( current_command_bar->get_Caption( &caption ) ) ;
		
		TRACE( caption ) ;
		
		wstring candidate_for_death = BSTR2wstring( caption ) ;
		str::replace_all( candidate_for_death, L"&", L"" ) ;
		size_t len = marked_for_death.size() ;
		if ( str::equal_nocase( str::left( candidate_for_death, len), marked_for_death ) )
		{
			ATLTRACE( "YEEHAW!!\n" ) ;
			current_command_bar->Delete() ;
			--menu_count ;
		}
		else
		{
			++i ;
		}
		
		caption.Empty() ;
		current_command_bar.Release() ;
	}

	unadvise_menu_items() ;
}

void CWordInterface::persist_app_state()
{
	m_properties.read_from_registry() ;

	// toolbar state
	VARIANT_BOOL is_visible ;
	m_toolbar->get_Visible( &is_visible ) ;
	if ( is_visible == VARIANT_FALSE )
	{
		m_properties.set_toolbar_visible( FALSE ) ;
	}
	else
	{
		m_properties.set_toolbar_visible( TRUE ) ;
	}

	m_properties.write_to_registry() ;
}

HRESULT CWordInterface::advise_menu_items()
{
	_HR_RET( MenuAutoTransEventImpl::DispEventAdvise( (IUnknown*)m_menu_auto_trans ) ) ;
	_HR_RET( MenuAutoTransFuzzyEventImpl::DispEventAdvise( (IUnknown*)m_menu_auto_trans_fuzzy ) ) ;
	_HR_RET( MenuGetEventImpl::DispEventAdvise( (IUnknown*)m_menu_get ) ) ;
	_HR_RET( MenuGetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_menu_get_and_next ) ) ;
	_HR_RET( MenuSetEventImpl::DispEventAdvise( (IUnknown*)m_menu_set ) ) ;
	_HR_RET( MenuSetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_menu_set_and_next ) ) ;
	_HR_RET( MenuLookupEventImpl::DispEventAdvise( (IUnknown*)m_menu_lookup ) ) ;
	_HR_RET( MenuLookupNextEventImpl::DispEventAdvise( (IUnknown*)m_menu_lookup_next ) ) ;
	_HR_RET( MenuExtendLookupEventImpl::DispEventAdvise( (IUnknown*)m_menu_extend_lookup ) ) ;
	_HR_RET( MenuConcordanceEventImpl::DispEventAdvise( (IUnknown*)m_menu_concordance ) ) ;
	_HR_RET( MenuRegisterGlossEventImpl::DispEventAdvise( (IUnknown*)m_menu_register_gloss ) ) ;
	_HR_RET( MenuDeleteEventImpl::DispEventAdvise( (IUnknown*)m_menu_delete ) ) ;
	_HR_RET( MenuNextEventImpl::DispEventAdvise( (IUnknown*)m_menu_next ) ) ;
	_HR_RET( MenuPrevEventImpl::DispEventAdvise( (IUnknown*)m_menu_prev ) ) ;
		_HR_RET( MenuEntry0EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_0 ) ) ;
		_HR_RET( MenuEntry1EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_1 ) ) ;
		_HR_RET( MenuEntry2EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_2 ) ) ;
		_HR_RET( MenuEntry3EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_3 ) ) ;
		_HR_RET( MenuEntry4EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_4 ) ) ;
		_HR_RET( MenuEntry5EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_5 ) ) ;
		_HR_RET( MenuEntry6EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_6 ) ) ;
		_HR_RET( MenuEntry7EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_7 ) ) ;
		_HR_RET( MenuEntry8EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_8 ) ) ;
		_HR_RET( MenuEntry9EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_9 ) ) ;
		_HR_RET( MenuGlossNEventImpl::DispEventAdvise( (IUnknown*)m_menu_gloss_n ) ) ;

	if ( m_menu_lookup_trans ) 
	{
		_HR_RET( MenuLookupTransEventImpl::DispEventAdvise( (IUnknown*)m_menu_lookup_trans ) ) ;
		_HR_RET( MenuLookupNextTransEventImpl::DispEventAdvise( (IUnknown*)m_menu_lookup_next_trans ) ) ;
		_HR_RET( MenuTransConcordanceEventImpl::DispEventAdvise( (IUnknown*)m_menu_trans_concordance ) ) ;
		_HR_RET( MenuCorrectTransEventImpl::DispEventAdvise( (IUnknown*)m_menu_correct_trans ) ) ;
		_HR_RET( MenuExtendTransLookupEventImpl::DispEventAdvise( (IUnknown*)m_menu_extend_trans_lookup ) ) ;
	}

	_HR_RET( MenuSaveEventImpl::DispEventAdvise( (IUnknown*)m_menu_save ) ) ;
	_HR_RET( MenuPreferencesEventImpl::DispEventAdvise( (IUnknown*)m_menu_preferences ) ) ;

	if ( m_menu_switch ) 
	{
		_HR_RET( MenuSwitchEventImpl::DispEventAdvise( (IUnknown*)m_menu_switch ) ) ;
	}
	return S_OK ;
}

HRESULT CWordInterface::unadvise_menu_items()
{
	// disconnect menu items
	if ( ! m_menu_auto_trans ) 
	{
		return S_OK ;
	}
	_HR_RET( MenuAutoTransEventImpl::DispEventUnadvise( (IUnknown*)m_menu_auto_trans  ) ) ;
	_HR_RET( MenuAutoTransFuzzyEventImpl::DispEventUnadvise( (IUnknown*)m_menu_auto_trans_fuzzy  ) ) ;
	
	_HR_RET( MenuGetEventImpl::DispEventUnadvise( (IUnknown*)m_menu_get  ) ) ;         
	_HR_RET( MenuGetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_menu_get_and_next  ) ) ;  
	_HR_RET( MenuSetEventImpl::DispEventUnadvise( (IUnknown*)m_menu_set  ) ) ;         
	_HR_RET( MenuSetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_menu_set_and_next  ) ) ;  
	_HR_RET( MenuLookupEventImpl::DispEventUnadvise( (IUnknown*)m_menu_lookup  ) ) ;
	_HR_RET( MenuLookupNextEventImpl::DispEventUnadvise( (IUnknown*)m_menu_lookup_next  ) ) ;
	
	_HR_RET( MenuConcordanceEventImpl::DispEventUnadvise( (IUnknown*)m_menu_concordance  ) ) ;
	_HR_RET( MenuRegisterGlossEventImpl::DispEventUnadvise( (IUnknown*)m_menu_register_gloss  ) ) ;
	_HR_RET( MenuNextEventImpl::DispEventUnadvise( (IUnknown*)m_menu_next  ) ) ;
	_HR_RET( MenuPrevEventImpl::DispEventUnadvise( (IUnknown*)m_menu_prev  ) ) ;
	_HR_RET( MenuExtendLookupEventImpl::DispEventUnadvise( (IUnknown*)m_menu_extend_lookup  ) ) ;
	_HR_RET( MenuDeleteEventImpl::DispEventUnadvise( (IUnknown*)m_menu_delete  ) ) ;
	
	_HR_RET( MenuEntry0EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_0  ) ) ;
	_HR_RET( MenuEntry1EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_1  ) ) ;
	_HR_RET( MenuEntry2EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_2  ) ) ;
	_HR_RET( MenuEntry3EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_3  ) ) ;
	_HR_RET( MenuEntry4EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_4  ) ) ;
	_HR_RET( MenuEntry5EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_5  ) ) ;
	_HR_RET( MenuEntry6EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_6  ) ) ;
	_HR_RET( MenuEntry7EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_7  ) ) ;
	_HR_RET( MenuEntry8EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_8  ) ) ;
	_HR_RET( MenuEntry9EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_9  ) ) ;
	_HR_RET( MenuGlossNEventImpl::DispEventUnadvise( (IUnknown*)m_menu_gloss_n  ) ) ;
	
	if ( m_menu_lookup_trans ) 
	{
		_HR_RET( MenuLookupTransEventImpl::DispEventUnadvise( (IUnknown*)m_menu_lookup_trans ) ) ;
		_HR_RET( MenuLookupNextTransEventImpl::DispEventUnadvise( (IUnknown*)m_menu_lookup_next_trans ) ) ;
		_HR_RET( MenuTransConcordanceEventImpl::DispEventUnadvise( (IUnknown*)m_menu_trans_concordance ) ) ;
		_HR_RET( MenuCorrectTransEventImpl::DispEventUnadvise( (IUnknown*)m_menu_correct_trans ) ) ;
		_HR_RET( MenuExtendTransLookupEventImpl::DispEventUnadvise( (IUnknown*)m_menu_extend_trans_lookup ) ) ;
	}
	
	_HR_RET( MenuSaveEventImpl::DispEventUnadvise( (IUnknown*)m_menu_save ) ) ;
	_HR_RET( MenuPreferencesEventImpl::DispEventUnadvise( (IUnknown*)m_menu_preferences ) ) ;
	
	if ( m_menu_switch ) 
	{
		_HR_RET( MenuSwitchEventImpl::DispEventUnadvise( (IUnknown*)m_menu_switch ) ) ;
	}

	// set them to null
		m_menu_get = NULL ;
		m_menu_set = NULL ;
		m_menu_get_and_next = NULL ;
		m_menu_set_and_next = NULL ;
		m_menu_lookup = NULL ;
		m_menu_lookup_next = NULL ;
		m_menu_gloss_n = NULL ;
		m_menu_save = NULL ;
		m_menu_help = NULL ;
	
		m_menu_auto_trans = NULL ;
		m_menu_auto_trans_fuzzy = NULL ;
		m_menu_concordance = NULL ;
		m_menu_extend_lookup = NULL ;
		m_menu_register_gloss = NULL ;
		m_menu_delete = NULL ;
		m_menu_next = NULL ;
		m_menu_prev = NULL ;
		m_menu_entry_0 = NULL ;
		m_menu_entry_1 = NULL ;
		m_menu_entry_2 = NULL ;
		m_menu_entry_3 = NULL ;
		m_menu_entry_4 = NULL ;
		m_menu_entry_5 = NULL ;
		m_menu_entry_6 = NULL ;
		m_menu_entry_7 = NULL ;
		m_menu_entry_8 = NULL ;
		m_menu_entry_9 = NULL ;
		m_menu_lookup_trans = NULL ;
		m_menu_lookup_next_trans = NULL ;
		m_menu_trans_concordance = NULL ;
		m_menu_correct_trans = NULL ;
		m_menu_extend_trans_lookup = NULL ;
		m_menu_preferences = NULL ;
	
	m_trans_assist_menu_popup = NULL ;
	m_gloss_menu_popup = NULL ;
	
	return S_OK ;
}

void CWordInterface::block_save_in_normal_dot()
{
	return ;

//	CComPtr< MSWord::Templates > templates ;
//	_HR_V( m_app->get_Templates( &templates ) ) ;
//	if ( ! templates )
//	{
//		return ;
//	}
//	
//	long template_count = 0 ;
//	_HR_V( templates->get_Count( &template_count ) ) ;
//	
//	for ( long i = 1 ; i <= template_count ; ++i )
//	{
//		CComPtr< MSWord::Template > active_template ;
//		CComVariant index( i ) ;
//		_HR_V( templates->Item( &index, &active_template ) ) ;
//
//		CComBSTR template_name ;
//		_HR_V( active_template->get_Name( &template_name ) ) ;
//		
//		if ( template_name == L"Normal.dot" )
//		{
//			_HR_V( active_template->put_Saved( VARIANT_TRUE ) );
////			_HR_V( active_template->Save( ) );
//			return ; // leave the loop
//		}
//	}

}

void CWordInterface::set_button_text( command_button_ptr &button, int string_id )
{
	set_command_bar_text( button, string_id ) ;

}

void CWordInterface::set_menu_text(command_button_ptr &button, int string_id)
{
	set_command_bar_text( button, string_id ) ;

	CComBSTR description_text ;
	bool success = false ;
	success = description_text.LoadString( string_id+1 ) ;
	ATLASSERT( success ) ;

	_HR_V( button->put_DescriptionText( description_text ) ) ;
}

void CWordInterface::set_command_bar_text(command_button_ptr &button, int string_id)
{
	bool success = false ;
	
	CComBSTR caption_text ;
	success = caption_text.LoadString( string_id ) ;
	ATLASSERT( success ) ;

	_HR_V( button->put_Caption( caption_text ) ) ;
	
	CComBSTR tooltip_text ;
	success = tooltip_text.LoadString( string_id+1 ) ;
	ATLASSERT( success ) ;
	
	_HR_V( button->put_TooltipText( tooltip_text ) ) ;

}

void CWordInterface::gui_to_language(int lang_offset)
{
	int string_offset = lang_offset ;

	if ( m_properties.get_classic_if() ) 
	{
		string_offset += ( IDS_CLASSIC_MENU_ENGLISH_FIRST - IDS_MENU_ENGLISH_FIRST ) ;
	}
	
	ATLASSERT( R2TS( IDS_MENU_HELP +1 ) == R2TS( IDS_MENU_HELP_TT) ) ;
	ATLASSERT( R2TS( IDS_MENU_HELP_J +1 ) == R2TS( IDS_MENU_HELP_TT_J) ) ;
	
	int string_id = IDS_MENU_HELP + string_offset ;
	set_button_text( m_button_help, string_id ) ;

	if ( m_controller->is_translation_mode() ) 
	{
		string_id = IDS_MENU_AUTO_TRANS_SEL + string_offset ;
		set_menu_text( m_menu_auto_trans, string_id ) ;
		
		string_id = IDS_MENU_AUTO_TRANS_FUZZY + string_offset ;
		set_menu_text( m_menu_auto_trans_fuzzy, string_id ) ;
	}
	else
	{
		string_id = IDS_MENU_MEMORY_TO_TRANS + string_offset ;
		set_menu_text( m_menu_auto_trans, string_id ) ;
		
		string_id = IDS_MENU_TRANS_TO_MEMORY + string_offset ;
		set_menu_text( m_menu_auto_trans_fuzzy, string_id ) ;
	}
	
	string_id = IDS_MENU_GET + string_offset ;
	set_menu_text( m_menu_get, string_id ) ;
	set_button_text( m_button_get, string_id ) ;
	
	ATLASSERT( R2TS( IDS_MENU_GET_AND_NEXT +1 ) == R2TS( IDS_MENU_GET_AND_NEXT_TT) ) ;
	ATLASSERT( R2TS( IDS_MENU_GET_AND_NEXT_J +1 ) == R2TS( IDS_MENU_GET_AND_NEXT_TT_J) ) ;

	string_id = IDS_MENU_GET_AND_NEXT + string_offset ;
	set_menu_text( m_menu_get_and_next, string_id ) ;
	set_button_text( m_button_get_and_next, string_id ) ;

	string_id = IDS_MENU_REGISTER + string_offset ;
	set_menu_text( m_menu_set, string_id ) ;
	set_button_text( m_button_set, string_id ) ;

	string_id = IDS_MENU_SET_AND_NEXT + string_offset ;
	set_menu_text( m_menu_set_and_next, string_id ) ;
	set_button_text( m_button_set_and_next, string_id ) ;
	
	string_id = IDS_MENU_LOOKUP + string_offset ;
	set_menu_text( m_menu_lookup, string_id ) ;
	set_button_text( m_button_lookup, string_id ) ;
	
	string_id = IDS_MENU_LOOKUP_NEXT + string_offset ;
	set_menu_text( m_menu_lookup_next, string_id ) ;
	set_button_text( m_button_lookup_next, string_id ) ;
	
	string_id = IDS_MENU_EXTEND + string_offset ;
	set_menu_text( m_menu_extend_lookup, string_id ) ;

	string_id = IDS_MENU_CONCORDANCE + string_offset ;
	set_menu_text( m_menu_concordance, string_id ) ;
	
	string_id = IDS_MENU_REGISTER_GLOSS + string_offset ;
	set_menu_text( m_menu_register_gloss, string_id ) ;
	
	string_id = IDS_MENU_DELETE + string_offset ;
	set_menu_text( m_menu_delete, string_id ) ;
	
	string_id = IDS_MENU_NEXT + string_offset ;
	set_menu_text( m_menu_next, string_id ) ;
	
	string_id = IDS_MENU_PREV + string_offset ;
	set_menu_text( m_menu_prev, string_id ) ;
	
	CComBSTR caption ; caption.LoadString( IDS_MENU_GET_GLOSS + string_offset ) ;
	CComBSTR tooltip ; tooltip.LoadString( IDS_MENU_GET_GLOSS_TT + string_offset ) ;
	_HR_V( m_gloss_menu_popup->put_Caption( caption ) ) ;
	_HR_V( m_gloss_menu_popup->put_DescriptionText( tooltip ) );
	
		string_id = IDS_MENU_ENTRY_0 + string_offset ;
		set_menu_text( m_menu_entry_0, string_id ) ;
		
		string_id = IDS_MENU_ENTRY_1 + string_offset ;
		set_menu_text( m_menu_entry_1, string_id ) ;
		
		string_id = IDS_MENU_ENTRY_2 + string_offset ;
		set_menu_text( m_menu_entry_2, string_id ) ;
		
		ATLASSERT( R2TS( IDS_MENU_ENTRY_3 +1 ) == R2TS( IDS_MENU_ENTRY_3_TT) ) ;
		ATLASSERT( R2TS( IDS_MENU_ENTRY_3_J +1 ) == R2TS( IDS_MENU_ENTRY_3_TT_J) ) ;
		
		string_id = IDS_MENU_ENTRY_3 + string_offset ;
		set_menu_text( m_menu_entry_3, string_id ) ;
		
		string_id = IDS_MENU_ENTRY_4 + string_offset ;
		set_menu_text( m_menu_entry_4, string_id ) ;
		
		string_id = IDS_MENU_ENTRY_5 + string_offset ;
		set_menu_text( m_menu_entry_5, string_id ) ;
		
		string_id = IDS_MENU_ENTRY_6 + string_offset ;
		set_menu_text( m_menu_entry_6, string_id ) ;
		
		string_id = IDS_MENU_ENTRY_7 + string_offset ;
		set_menu_text( m_menu_entry_7, string_id ) ;
		
		string_id = IDS_MENU_ENTRY_8 + string_offset ;
		set_menu_text( m_menu_entry_8, string_id ) ;
		
		string_id = IDS_MENU_ENTRY_9 + string_offset ;
		set_menu_text( m_menu_entry_9, string_id ) ;
		
		string_id = IDS_MENU_GLOSS_N + string_offset ;
		set_menu_text( m_menu_gloss_n, string_id ) ;

		if ( m_properties.get_classic_if() ) 
		{
			set_button_text( m_button_gloss_n, string_id ) ;
		}
		else
		{
			set_button_text( m_button_gloss_n, IDS_MENU_TO_REVIEW_MODE + lang_offset ) ;
		}
		
	if ( m_menu_lookup_trans && m_properties.get_classic_if() ) 
	{
		string_id = IDS_MENU_LOOKUP_TRANS + string_offset ;
		set_menu_text( m_menu_lookup_trans, string_id ) ;
		
		string_id = IDS_MENU_LOOKUP_NEXT_TRANS + string_offset ;
		set_menu_text( m_menu_lookup_next_trans, string_id ) ;
		
		string_id = IDS_MENU_CONCORDANCE_TRANS + string_offset ;
		set_menu_text( m_menu_trans_concordance, string_id ) ;
		
		string_id = IDS_MENU_CORRECT_TRANS + string_offset ;
		set_menu_text( m_menu_correct_trans, string_id ) ;
		
		string_id = IDS_MENU_EXTEND_TRANS + string_offset ;
		set_menu_text( m_menu_extend_trans_lookup, string_id ) ;
	}
	
	string_id = IDS_MENU_SAVE + string_offset ;
	set_menu_text( m_menu_save, string_id ) ;
	
	string_id = IDS_MENU_PREFERENCES + string_offset ;
	set_menu_text( m_menu_preferences, string_id ) ;

	if ( m_menu_switch && ! m_properties.get_classic_if() ) 
	{
		if ( m_controller->is_translation_mode() ) 
		{
			string_id = IDS_MENU_TO_REVIEW_MODE + string_offset ;
		}
		else
		{
			string_id = IDS_MENU_TO_TRANS_MODE + string_offset ;
		}
		set_menu_text( m_menu_switch, string_id ) ;
	}

	// Revision toolbar
//	string_id = IDS_MENU_LOOKUP_TRANS + string_offset ;
//	set_menu_text( m_button_lookup_trans, string_id ) ;
//	
//	string_id = IDS_MENU_LOOKUP_NEXT_TRANS + string_offset ;
//	set_menu_text( m_button_lookup_next_trans, string_id ) ;
//	
//	string_id = IDS_MENU_RESTORE + string_offset ;
//	set_menu_text( m_button_restore, string_id ) ;
//	
//	string_id = IDS_MENU_RESTORE_AND_NEXT + string_offset ;
//	set_menu_text( m_button_restore_and_next, string_id ) ;
//	
//	string_id = IDS_MENU_CORRECT_TRANS + string_offset ;
//	set_menu_text( m_button_correct, string_id ) ;
//	
//	string_id = IDS_MENU_CORRECT_AND_NEXT + string_offset ;
//	set_menu_text( m_button_correct_and_next, string_id ) ;
	
}

void CWordInterface::set_customization_context()
{

}

bool CWordInterface::load_picture(command_button_ptr &button, int button_id)
{
	HINSTANCE hInst = _Module.GetResourceInstance() ;

	HRSRC hrsrc = FindResource(hInst,MAKEINTRESOURCE(button_id),_T("IMAGE"));	// リソースに組み込んだ時の名前が「jpg」の場合
	DWORD dwFileSize = SizeofResource(hInst, hrsrc);
	
	HANDLE hres = LoadResource(hInst,hrsrc);
	char* pcPos = (char*)LockResource(hres);
	
	HANDLE hGlobal = GlobalAlloc(GPTR, dwFileSize);	// グローバルに取り直さないと以下が行えない
	memcpy(hGlobal,pcPos,dwFileSize);

	HRESULT hr = S_OK ;

	CComPtr< IStream >  iStream ;
	hr = CreateStreamOnHGlobal(hGlobal, TRUE, &iStream);
	if ( FAILED( hr ) ) 
	{
		return false ;
	}

	CComPtr< IPicture > iPicture ;
	hr = OleLoadPicture(iStream, dwFileSize, TRUE, IID_IPicture, (LPVOID*)&iPicture);
	if ( FAILED( hr ) ) 
	{
		if ( hr == CTL_E_INVALIDPICTURE ) 
		{
			ATLTRACE( "%s(%d): 0x%0X Invalid picture\n", __FILE__, __LINE__, hr ) ;
		}
		else
		{
			TRACE_HRESULT( hr ) ;
		}
		return false ;
	}

	try
	{
		COleAutoWrapper but( (IDispatch*)button ) ;
		CComVariant vPicture ( (IUnknown*)iPicture ) ;
		but.prop_put( L"Picture", vPicture ) ;
	}
	catch( CComException &e )
	{
		e ;
		TRACE( e.what() ) ;
		TRACE( e.description() ) ;
		
		return false ;
		
	}
	
	return true ;

}

void CWordInterface::set_tag_text(command_button_ptr &button, const CComBSTR &tag_text )
{
	_HR_V( button->put_Tag( tag_text ) ) ;
}


bool CWordInterface::set_button_image(command_button_ptr &button, const int image_id)
{
	if ( ! load_picture( button, image_id + 300 ) ) 
	{
		HBITMAP hBmp  =(HBITMAP)::LoadImage(_Module.GetResourceInstance(),
			MAKEINTRESOURCE(image_id),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS);
		
		// put bitmap into Clipboard
		::OpenClipboard(NULL) ;
		::EmptyClipboard() ;
		::SetClipboardData(CF_BITMAP, (HANDLE)hBmp) ;
		::CloseClipboard() ;
		::DeleteObject(hBmp) ;
		
		HRESULT hr = button->PasteFace() ;
		ATLASSERT( SUCCEEDED( hr ) && "Failed to paste image into command button object" )  ;
		
		::OpenClipboard(NULL) ;
		::EmptyClipboard() ;
		::CloseClipboard() ;

		if ( FAILED( hr ) ) 
		{
			return false ;
		}
	}

	return true ;
}

void CWordInterface::switch_to_classic_toolbar()
{
	int string_offset = IDS_CLASSIC_MENU_ENGLISH_FIRST - IDS_MENU_ENGLISH_FIRST ;
	
	if ( m_properties.get_preferred_gui_lang() == LANG_JAPANESE )
	{
		string_offset += JAPANESE_OFFSET ;
	}
	
	// add buttons
	set_button_image( m_button_lookup, IDB_LOOKUP ) ;
	set_button_text( m_button_lookup, IDS_MENU_LOOKUP + string_offset ) ;
	
	set_button_image( m_button_lookup_next, IDB_LOOKUP_NEXT ) ;
	set_button_text( m_button_lookup_next, IDS_MENU_LOOKUP_NEXT + string_offset ) ;
	
	set_button_image( m_button_get, IDB_GET ) ;
	set_button_text( m_button_get, IDS_MENU_GET + string_offset ) ;
	
	set_button_image( m_button_get_and_next, IDB_GET_AND_NEXT ) ;
	set_button_text( m_button_get_and_next, IDS_MENU_GET_AND_NEXT + string_offset ) ;
	
	set_button_image( m_button_set, IDB_SET ) ;
	set_button_text( m_button_set, IDS_MENU_REGISTER + string_offset ) ;
	
	set_button_image( m_button_set_and_next, IDB_SET_AND_NEXT ) ;
	set_button_text( m_button_set_and_next, IDS_MENU_SET_AND_NEXT + string_offset ) ;

	set_button_image( m_button_gloss_n, IDB_GLOSS_N ) ;
	set_button_text( m_button_gloss_n, IDS_MENU_GLOSS_N + string_offset ) ;
	
}



void CWordInterface::switch_to_translation_toolbar()
{
	int string_offset = 0 ;

	if ( m_properties.get_preferred_gui_lang() == LANG_JAPANESE )
	{
		string_offset += JAPANESE_OFFSET ;
	}
	
	// add buttons
	set_button_image( m_button_lookup, IDB_LOOKUP ) ;
	set_button_text( m_button_lookup, IDS_MENU_LOOKUP + string_offset ) ;
	
	set_button_image( m_button_lookup_next, IDB_LOOKUP_NEXT ) ;
	set_button_text( m_button_lookup_next, IDS_MENU_LOOKUP_NEXT + string_offset ) ;
	
	set_button_image( m_button_get, IDB_GET ) ;
	set_button_text( m_button_get, IDS_MENU_GET + string_offset ) ;
	
	set_button_image( m_button_get_and_next, IDB_GET_AND_NEXT ) ;
	set_button_text( m_button_get_and_next, IDS_MENU_GET_AND_NEXT + string_offset ) ;
	
	set_button_image( m_button_set, IDB_SET ) ;
	set_button_text( m_button_set, IDS_MENU_REGISTER + string_offset ) ;
	
	set_button_image( m_button_set_and_next, IDB_SET_AND_NEXT ) ;
	set_button_text( m_button_set_and_next, IDS_MENU_SET_AND_NEXT + string_offset ) ;
	
	set_button_image( m_button_gloss_n, IDB_SWITCH_TO_REVIEW ) ;
	set_button_text( m_button_gloss_n, IDS_MENU_TO_REVIEW_MODE + string_offset ) ;
	
}




	void CWordInterface::switch_to_review_toolbar( )
	{
		int string_offset = 0 ;
		
		if ( m_properties.get_preferred_gui_lang() == LANG_JAPANESE )
		{
			string_offset = IDS_MENU_JAPANESE_FIRST - IDS_MENU_ENGLISH_FIRST ;
		}
		
		// add buttons
		set_button_image( m_button_lookup, IDB_LOOKUP_TRANS ) ;
		set_button_text( m_button_lookup, IDS_MENU_LOOKUP_TRANS + string_offset ) ;
		
		set_button_image( m_button_lookup_next, IDB_LOOKUP_NEXT_TRANS ) ;
		set_button_text( m_button_lookup_next, IDS_MENU_LOOKUP_NEXT_TRANS + string_offset ) ;
		
		set_button_image( m_button_get, IDB_RESTORE ) ;
		set_button_text( m_button_get, IDS_MENU_RESTORE + string_offset ) ;
		
		set_button_image( m_button_get_and_next, IDB_RESTORE_AND_NEXT ) ;
		set_button_text( m_button_get_and_next, IDS_MENU_RESTORE_AND_NEXT + string_offset ) ;
		
		set_button_image( m_button_set, IDB_CORRECT ) ;
		set_button_text( m_button_set, IDS_MENU_CORRECT_TRANS + string_offset ) ;
		
		set_button_image( m_button_set_and_next, IDB_CORRECT_AND_NEXT ) ;
		set_button_text( m_button_set_and_next, IDS_MENU_CORRECT_AND_NEXT + string_offset ) ;
		
		set_button_image( m_button_gloss_n, IDB_SWITCH_TO_TRANS ) ;
		set_button_text( m_button_gloss_n, IDS_MENU_TO_TRANS_MODE + string_offset ) ;
	}

	

void CWordInterface::switch_to_new_menu()
{
	office_cmd_bars spCmdBars; 
	COM_ENFORCE( m_app->get_CommandBars( &spCmdBars ), _T("Failed to retrieve command bars collection from app") ) ;
	ATLASSERT(spCmdBars);
	
	add_menu( spCmdBars ) ;
}

void CWordInterface::switch_to_classic_menu()
{
	office_cmd_bars spCmdBars; 
	COM_ENFORCE( m_app->get_CommandBars( &spCmdBars ), _T("Failed to retrieve command bars collection from app") ) ;
	ATLASSERT(spCmdBars);
	
	add_classic_menu( spCmdBars ) ;
}

void CWordInterface::switch_to_translation_menu()
{
	office_cmd_bars spCmdBars; 
	COM_ENFORCE( m_app->get_CommandBars( &spCmdBars ), _T("Failed to retrieve command bars collection from app") ) ;
	ATLASSERT(spCmdBars);

	add_menu( spCmdBars ) ;
}

void CWordInterface::switch_to_review_menu()
{
	office_cmd_bars spCmdBars; 
	COM_ENFORCE( m_app->get_CommandBars( &spCmdBars ), _T("Failed to retrieve command bars collection from app") ) ;
	ATLASSERT(spCmdBars);
	
	add_menu_review( spCmdBars ) ;
}

HRESULT CWordInterface::add_classic_menu( office_cmd_bars &spCmdBars )
{
	CComPtr< MSOffice::CommandBar >   spActiveMenuBar;
	COM_ENFORCE( spCmdBars->get_ActiveMenuBar(&spActiveMenuBar), _T("Failed to get menu bar") ) ;

	office_cmd_bar_ctls spCmdBarCtrls;
	COM_ENFORCE( spActiveMenuBar->get_Controls(&spCmdBarCtrls), _T("Failed to get menu bar controls") ) ;

	kill_old_menu( spCmdBarCtrls ) ;

	set_customization_context() ;	

	CComVariant vType( MSOffice::msoControlPopup ) ;
	CComVariant vTemp(VARIANT_FALSE) ; // menu is temporary        
	COM_ENFORCE( spCmdBarCtrls->raw_Add( 
		vType,									// type
		vtMissing,								// id
		vtMissing, 								// parameter
		vtMissing, 								// before
		vTemp, 			// temporary
		&m_trans_assist_menu_popup ), 			// the control
		_T("Failed to add popup menu") ) ;

	_HR_RET( m_trans_assist_menu_popup->put_Caption( CComBSTR( L"T&ransAssist" ) ) ) ;

	CComPtr< IDispatch > spDispatch ;
	_HR_RET( m_trans_assist_menu_popup->get_Control( &spDispatch ) ) ;

	CComQIPtr< MSOffice::CommandBarPopup > spCmdBarPopup( spDispatch ) ;
	ATLASSERT( spCmdBarPopup) ;
	_HR_RET( spCmdBarPopup->put_Visible( VARIANT_TRUE ) ) ;

	office_cmd_bar_ctls spMenuControls;
	COM_ENFORCE( spCmdBarPopup->get_Controls(&spMenuControls), _T("Failed to get menu bar controls") ) ;

	int string_offset = 0 ;

	if ( m_properties.get_preferred_gui_lang() == LANG_JAPANESE )
	{
		string_offset += ( IDS_MENU_JAPANESE_FIRST - IDS_MENU_ENGLISH_FIRST ) ;
	}

	if ( m_properties.get_classic_if() ) 
	{
		string_offset += ( IDS_CLASSIC_MENU_ENGLISH_FIRST - IDS_MENU_ENGLISH_FIRST ) ;
	}

	m_menu_auto_trans = add_menu_item( spMenuControls, 0, IDS_MENU_AUTO_TRANS_SEL + string_offset) ;
	ATLASSERT( m_menu_auto_trans ) ;
	
	m_menu_auto_trans_fuzzy = add_menu_item( spMenuControls, 0, IDS_MENU_AUTO_TRANS_FUZZY + string_offset) ;
	ATLASSERT( m_menu_auto_trans_fuzzy ) ;
	
	m_menu_get = add_menu_item( spMenuControls, IDB_GET, IDS_MENU_GET + string_offset) ;
	ATLASSERT( m_menu_get ) ;
	m_menu_get->put_BeginGroup( VARIANT_TRUE ) ;

	m_menu_get_and_next = add_menu_item( spMenuControls, IDB_GET_AND_NEXT, IDS_MENU_GET_AND_NEXT + string_offset) ;
	ATLASSERT( m_menu_get_and_next ) ;
	
	m_menu_set = add_menu_item( spMenuControls, IDB_SET, IDS_MENU_REGISTER + string_offset) ;
	ATLASSERT( m_menu_set ) ;
	m_menu_set->put_BeginGroup( VARIANT_TRUE ) ;
	
	m_menu_set_and_next = add_menu_item( spMenuControls, IDB_SET_AND_NEXT, IDS_MENU_SET_AND_NEXT + string_offset) ;
	ATLASSERT( m_menu_set_and_next ) ;
	
	m_menu_lookup = add_menu_item( spMenuControls, IDB_LOOKUP, IDS_MENU_LOOKUP + string_offset ) ;
	ATLASSERT( m_menu_lookup ) ;
	m_menu_lookup->put_BeginGroup( VARIANT_TRUE ) ;
	
	m_menu_lookup_next = add_menu_item( spMenuControls, IDB_LOOKUP_NEXT, IDS_MENU_LOOKUP_NEXT + string_offset ) ;
	ATLASSERT( m_menu_lookup_next ) ;
	
	m_menu_extend_lookup = add_menu_item( spMenuControls, 0, IDS_MENU_EXTEND + string_offset ) ;
	ATLASSERT( m_menu_extend_lookup ) ;
	
	m_menu_concordance = add_menu_item( spMenuControls, 0, IDS_MENU_CONCORDANCE + string_offset ) ;
	ATLASSERT( m_menu_concordance ) ;
	
	m_menu_register_gloss = add_menu_item( spMenuControls, 0, IDS_MENU_REGISTER_GLOSS + string_offset ) ;
	ATLASSERT( m_menu_register_gloss ) ;
	m_menu_register_gloss->put_BeginGroup( VARIANT_TRUE ) ;
	
	m_menu_delete = add_menu_item( spMenuControls, IDB_DELETE, IDS_MENU_DELETE + string_offset ) ;
	ATLASSERT( m_menu_delete ) ;
	
	m_menu_next = add_menu_item( spMenuControls, 0, IDS_MENU_NEXT + string_offset ) ;
	ATLASSERT( m_menu_next ) ;
	
	m_menu_prev = add_menu_item( spMenuControls, 0, IDS_MENU_PREV + string_offset ) ;
	ATLASSERT( m_menu_prev ) ;
	
	COM_ENFORCE( spMenuControls->raw_Add(
		vType,
		vtMissing,
		vtMissing,
		vtMissing,
		vTemp,
		&m_gloss_menu_popup
		),
		_T("Failed to get controls for menu item") 
		) ;
	
	CComBSTR caption ; caption.LoadString( IDS_MENU_GET_GLOSS + string_offset ) ;
	CComBSTR tooltip ; tooltip.LoadString( IDS_MENU_GET_GLOSS_TT + string_offset ) ;
	_HR_RET( m_gloss_menu_popup->put_Caption( caption ) ) ;
	_HR_RET( m_gloss_menu_popup->put_DescriptionText( tooltip ) );
	_HR_RET( m_gloss_menu_popup->put_BeginGroup( VARIANT_TRUE ) ) ;
	
	CComPtr< IDispatch > spGlossDispatch ;
	_HR_RET( m_gloss_menu_popup->get_Control( &spGlossDispatch ) ) ;
	
	CComQIPtr< MSOffice::CommandBarPopup > spGlossPopup( spGlossDispatch ) ;
	_HR_RET( spCmdBarPopup->put_Visible( VARIANT_TRUE ) ) ;
	
	office_cmd_bar_ctls spGlossMenuControls;
	COM_ENFORCE( spGlossPopup->get_Controls(&spGlossMenuControls), _T("Failed to get menu bar controls") ) ;
	
		m_menu_entry_0 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_0 + string_offset ) ;
		ATLASSERT( m_menu_entry_0 ) ;
		
		m_menu_entry_1 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_1 + string_offset ) ;
		ATLASSERT( m_menu_entry_1 ) ;
		
		m_menu_entry_2 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_2 + string_offset ) ;
		ATLASSERT( m_menu_entry_2 ) ;
		
		m_menu_entry_3 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_3 + string_offset ) ;
		ATLASSERT( m_menu_entry_3 ) ;
		
		m_menu_entry_4 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_4 + string_offset ) ;
		ATLASSERT( m_menu_entry_4 ) ;
		
		m_menu_entry_5 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_5 + string_offset ) ;
		ATLASSERT( m_menu_entry_5 ) ;
		
		m_menu_entry_6 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_6 + string_offset ) ;
		ATLASSERT( m_menu_entry_6 ) ;
		
		m_menu_entry_7 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_7 + string_offset ) ;
		ATLASSERT( m_menu_entry_7 ) ;
		
		m_menu_entry_8 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_8 + string_offset ) ;
		ATLASSERT( m_menu_entry_8 ) ;
		
		m_menu_entry_9 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_9 + string_offset ) ;
		ATLASSERT( m_menu_entry_9 ) ;
		
		m_menu_gloss_n = add_menu_item( spGlossMenuControls, IDB_GLOSS_N, IDS_MENU_GLOSS_N + string_offset ) ;
		ATLASSERT( m_menu_gloss_n ) ;
	
	m_menu_lookup_trans = add_menu_item( spMenuControls, IDB_LOOKUP_TRANS, IDS_MENU_LOOKUP_TRANS + string_offset ) ;
	ATLASSERT( m_menu_lookup_trans ) ;
	m_menu_lookup_trans->put_BeginGroup( VARIANT_TRUE ) ;
	
	m_menu_lookup_next_trans = add_menu_item( spMenuControls, IDB_LOOKUP_NEXT_TRANS, IDS_MENU_LOOKUP_NEXT_TRANS + string_offset ) ;
	ATLASSERT( m_menu_lookup_next_trans ) ;
	
	m_menu_trans_concordance = add_menu_item( spMenuControls, 0, IDS_MENU_CONCORDANCE_TRANS + string_offset ) ;
	ATLASSERT( m_menu_trans_concordance ) ;
	
	m_menu_correct_trans = add_menu_item( spMenuControls, IDB_CORRECT, IDS_MENU_CORRECT_TRANS + string_offset ) ;
	ATLASSERT( m_menu_correct_trans ) ;
	
	m_menu_extend_trans_lookup = add_menu_item( spMenuControls, 0, IDS_MENU_EXTEND_TRANS + string_offset ) ;
	ATLASSERT( m_menu_extend_trans_lookup ) ;
	
	m_menu_save = add_menu_item( spMenuControls, IDB_SAVE, IDS_MENU_SAVE + string_offset )	;
	ATLASSERT( m_menu_save ) ;
	m_menu_save->put_BeginGroup( VARIANT_TRUE ) ;
	
	m_menu_preferences = add_menu_item( spMenuControls, IDB_PROPERTIES, IDS_MENU_PREFERENCES + string_offset ) ;
	ATLASSERT( m_menu_preferences ) ;

	advise_menu_items() ;

	block_save_in_normal_dot() ;

	return S_OK ;

}

void CWordInterface::advise_document_events()
{
	HRESULT hr = S_OK ;
	
	hr = ActivateDocumentEventImpl::DispEventAdvise( (IUnknown*)m_app );
	ASSERT_HRESULT( hr ) ;
	
	hr = DeActivateDocumentEventImpl::DispEventAdvise( (IUnknown*)m_app );
	ASSERT_HRESULT( hr ) ;
	
	hr = DocumentChangeEventImpl::DispEventAdvise( (IUnknown*)m_app );
	ASSERT_HRESULT( hr ) ;
	
	hr = DocumentOpenEventImpl::DispEventAdvise( (IUnknown*)m_app );	
	ASSERT_HRESULT( hr ) ;
	
	hr = NewDocumentEventImpl::DispEventAdvise( (IUnknown*)m_app );	
	ASSERT_HRESULT( hr ) ;
	
	hr = WindowSelectionChangeEventImpl::DispEventAdvise( (IUnknown*)m_app );	
	ASSERT_HRESULT( hr ) ;

}

void CWordInterface::unadvise_document_events()
{
	HRESULT hr = S_OK ;
	hr = ActivateDocumentEventImpl::DispEventUnadvise( (IUnknown*)m_app );
	ASSERT_HRESULT( hr ) ;
	hr = DeActivateDocumentEventImpl::DispEventUnadvise( (IUnknown*)m_app );
	ASSERT_HRESULT( hr ) ;
	
	hr = DocumentChangeEventImpl::DispEventUnadvise( (IUnknown*)m_app );
	ASSERT_HRESULT( hr ) ;
	hr = DocumentOpenEventImpl::DispEventUnadvise( (IUnknown*)m_app );	
	ASSERT_HRESULT( hr ) ;
	hr = NewDocumentEventImpl::DispEventUnadvise( (IUnknown*)m_app );	
	ASSERT_HRESULT( hr ) ;
	hr = WindowSelectionChangeEventImpl::DispEventUnadvise( (IUnknown*)m_app );	
	ASSERT_HRESULT( hr ) ;

}

void CWordInterface::unadvise_button_items()
{
	HRESULT hr = S_OK ;
	
	// disconnect buttons
	hr = ButtonGetEventImpl::DispEventUnadvise( (IUnknown*)m_button_get  ) ;       
	ASSERT_HRESULT( hr ) ;
	hr = ButtonSetEventImpl::DispEventUnadvise( (IUnknown*)m_button_set  ) ;       
	ASSERT_HRESULT( hr ) ;
	hr = ButtonGetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_button_get_and_next  ) ;
	ASSERT_HRESULT( hr ) ;
	hr = ButtonSetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_button_set_and_next  ) ;
	ASSERT_HRESULT( hr ) ;
	hr = ButtonLookupEventImpl::DispEventUnadvise( (IUnknown*)m_button_lookup  ) ;
	ASSERT_HRESULT( hr ) ;
	hr = ButtonLookupNextEventImpl::DispEventUnadvise( (IUnknown*)m_button_lookup_next  ) ;
	ASSERT_HRESULT( hr ) ;
	hr = ButtonGlossNEventImpl::DispEventUnadvise( (IUnknown*)m_button_gloss_n  ) ;
	ASSERT_HRESULT( hr ) ;
	hr = ButtonHelpEventImpl::DispEventUnadvise( (IUnknown*)m_button_help  ) ;
	ASSERT_HRESULT( hr ) ;
}


void CWordInterface::setup_stuff()
{
	set_up_exception_handling() ;
	
	BOOL success = FALSE ;
	success = AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_INTERNET_CLASSES | ICC_USEREX_CLASSES );	// add flags to support other controls
	ATLASSERT( success ) ;
}
HRESULT CWordInterface::add_menu_review(office_cmd_bars & spCmdBars)
{
	BANNER( "CWordInterface::add_menu" ) ;

	CComPtr< MSOffice::CommandBar >   spActiveMenuBar;
	COM_ENFORCE( spCmdBars->get_ActiveMenuBar(&spActiveMenuBar), _T("Failed to get menu bar") ) ;

	office_cmd_bar_ctls spCmdBarCtrls;
	COM_ENFORCE( spActiveMenuBar->get_Controls(&spCmdBarCtrls), _T("Failed to get menu bar controls") ) ;

	kill_old_menu( spCmdBarCtrls ) ;

	set_customization_context() ;	

	CComVariant vType( MSOffice::msoControlPopup ) ;
	CComVariant vTemp(VARIANT_FALSE) ; // menu is temporary        
	COM_ENFORCE( spCmdBarCtrls->raw_Add( 
		vType,									// type
		vtMissing,								// id
		vtMissing, 								// parameter
		vtMissing, 								// before
		vTemp, 			// temporary
		&m_trans_assist_menu_popup ), 			// the control
		_T("Failed to add popup menu") ) ;

	_HR_RET( m_trans_assist_menu_popup->put_Caption( CComBSTR( L"T&ransAssist [*]" ) ) ) ;

	CComPtr< IDispatch > spDispatch ;
	_HR_RET( m_trans_assist_menu_popup->get_Control( &spDispatch ) ) ;

	CComQIPtr< MSOffice::CommandBarPopup > spCmdBarPopup( spDispatch ) ;
	ATLASSERT( spCmdBarPopup) ;
	_HR_RET( spCmdBarPopup->put_Visible( VARIANT_TRUE ) ) ;

	office_cmd_bar_ctls spMenuControls;
	COM_ENFORCE( spCmdBarPopup->get_Controls(&spMenuControls), _T("Failed to get menu bar controls") ) ;

	int string_offset = 0 ;

	if ( m_properties.get_preferred_gui_lang() == LANG_JAPANESE )
	{
		string_offset += JAPANESE_OFFSET ;
	}

	if ( m_properties.get_classic_if() ) 
	{
		string_offset += ( IDS_CLASSIC_MENU_ENGLISH_FIRST - IDS_MENU_ENGLISH_FIRST ) ;
	}

	m_menu_auto_trans = add_menu_item( spMenuControls, 0, IDS_MENU_MEMORY_TO_TRANS + string_offset) ;
	ATLASSERT( m_menu_auto_trans ) ;

	m_menu_auto_trans_fuzzy = add_menu_item( spMenuControls, 0, IDS_MENU_TRANS_TO_MEMORY + string_offset) ;
	ATLASSERT( m_menu_auto_trans_fuzzy ) ;

	m_menu_get = add_menu_item( spMenuControls, IDB_RESTORE, IDS_MENU_RESTORE + string_offset) ;
	ATLASSERT( m_menu_get ) ;
	m_menu_get->put_BeginGroup( VARIANT_TRUE ) ;

	m_menu_get_and_next = add_menu_item( spMenuControls, IDB_RESTORE_AND_NEXT, IDS_MENU_RESTORE_AND_NEXT + string_offset) ;
	ATLASSERT( m_menu_get_and_next ) ;

	m_menu_set = add_menu_item( spMenuControls, IDB_CORRECT, IDS_MENU_CORRECT_TRANS + string_offset) ;
	ATLASSERT( m_menu_set ) ;
	m_menu_set->put_BeginGroup( VARIANT_TRUE ) ;

	m_menu_set_and_next = add_menu_item( spMenuControls, IDB_CORRECT_AND_NEXT, IDS_MENU_CORRECT_AND_NEXT + string_offset) ;
	ATLASSERT( m_menu_set_and_next ) ;

	m_menu_lookup = add_menu_item( spMenuControls, IDB_LOOKUP_TRANS, IDS_MENU_LOOKUP_TRANS + string_offset ) ;
	ATLASSERT( m_menu_lookup ) ;
	m_menu_lookup->put_BeginGroup( VARIANT_TRUE ) ;

	m_menu_lookup_next = add_menu_item( spMenuControls, IDB_LOOKUP_NEXT_TRANS, IDS_MENU_LOOKUP_NEXT_TRANS + string_offset ) ;
	ATLASSERT( m_menu_lookup_next ) ;

	m_menu_extend_lookup = add_menu_item( spMenuControls, 0, IDS_MENU_EXTEND + string_offset ) ;
	ATLASSERT( m_menu_extend_lookup ) ;

	m_menu_concordance = add_menu_item( spMenuControls, 0, IDS_MENU_CONCORDANCE + string_offset ) ;
	ATLASSERT( m_menu_concordance ) ;

	m_menu_register_gloss = add_menu_item( spMenuControls, 0, IDS_MENU_REGISTER_GLOSS + string_offset ) ;
	ATLASSERT( m_menu_register_gloss ) ;
	m_menu_register_gloss->put_BeginGroup( VARIANT_TRUE ) ;
	m_menu_register_gloss->put_Enabled( VARIANT_FALSE ) ;

	m_menu_delete = add_menu_item( spMenuControls, IDB_DELETE, IDS_MENU_DELETE + string_offset ) ;
	ATLASSERT( m_menu_delete ) ;

	m_menu_next = add_menu_item( spMenuControls, 0, IDS_MENU_NEXT + string_offset ) ;
	ATLASSERT( m_menu_next ) ;

	m_menu_prev = add_menu_item( spMenuControls, 0, IDS_MENU_PREV + string_offset ) ;
	ATLASSERT( m_menu_prev ) ;

	COM_ENFORCE( spMenuControls->raw_Add(
		vType,
		vtMissing,
		vtMissing,
		vtMissing,
		vTemp,
		&m_gloss_menu_popup
		),
		_T("Failed to get controls for menu item") 
		) ;

	CComBSTR caption ; caption.LoadString( IDS_MENU_GET_GLOSS + string_offset ) ;
	CComBSTR tooltip ; tooltip.LoadString( IDS_MENU_GET_GLOSS_TT + string_offset ) ;
	_HR_RET( m_gloss_menu_popup->put_Caption( caption ) ) ;
	_HR_RET( m_gloss_menu_popup->put_DescriptionText( tooltip ) );
	_HR_RET( m_gloss_menu_popup->put_BeginGroup( VARIANT_TRUE ) ) ;

	CComPtr< IDispatch > spGlossDispatch ;
	_HR_RET( m_gloss_menu_popup->get_Control( &spGlossDispatch ) ) ;

	CComQIPtr< MSOffice::CommandBarPopup > spGlossPopup( spGlossDispatch ) ;
	_HR_RET( spCmdBarPopup->put_Visible( VARIANT_TRUE ) ) ;

	office_cmd_bar_ctls spGlossMenuControls;
	COM_ENFORCE( spGlossPopup->get_Controls(&spGlossMenuControls), _T("Failed to get menu bar controls") ) ;

	m_menu_entry_0 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_0 + string_offset ) ;
	ATLASSERT( m_menu_entry_0 ) ;

	m_menu_entry_1 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_1 + string_offset ) ;
	ATLASSERT( m_menu_entry_1 ) ;

	m_menu_entry_2 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_2 + string_offset ) ;
	ATLASSERT( m_menu_entry_2 ) ;

	m_menu_entry_3 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_3 + string_offset ) ;
	ATLASSERT( m_menu_entry_3 ) ;

	m_menu_entry_4 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_4 + string_offset ) ;
	ATLASSERT( m_menu_entry_4 ) ;

	m_menu_entry_5 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_5 + string_offset ) ;
	ATLASSERT( m_menu_entry_5 ) ;

	m_menu_entry_6 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_6 + string_offset ) ;
	ATLASSERT( m_menu_entry_6 ) ;

	m_menu_entry_7 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_7 + string_offset ) ;
	ATLASSERT( m_menu_entry_7 ) ;

	m_menu_entry_8 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_8 + string_offset ) ;
	ATLASSERT( m_menu_entry_8 ) ;

	m_menu_entry_9 = add_menu_item( spGlossMenuControls, 0, IDS_MENU_ENTRY_9 + string_offset ) ;
	ATLASSERT( m_menu_entry_9 ) ;

	m_menu_gloss_n = add_menu_item( spGlossMenuControls, IDB_GLOSS_N, IDS_MENU_GLOSS_N + string_offset ) ;
	ATLASSERT( m_menu_gloss_n ) ;

	m_menu_save = add_menu_item( spMenuControls, IDB_SAVE, IDS_MENU_SAVE + string_offset )	;
	ATLASSERT( m_menu_save ) ;
	m_menu_save->put_BeginGroup( VARIANT_TRUE ) ;

	m_menu_switch = add_menu_item( spMenuControls, IDB_SWITCH_TO_TRANS, IDS_MENU_TO_TRANS_MODE + string_offset ) ;
	ATLASSERT( m_menu_switch ) ;

	m_menu_preferences = add_menu_item( spMenuControls, IDB_PROPERTIES, IDS_MENU_PREFERENCES + string_offset ) ;
	ATLASSERT( m_menu_preferences ) ;

	advise_menu_items() ;

	block_save_in_normal_dot() ;

	return S_OK ;


}

void __stdcall CWordInterface::OnMenuPreferences ( IDispatch *, VARIANT_BOOL *  )
{
	AUTO_TRY

		if ( ! m_controller->IsActive() ) 
		{
			return ;
		}

		m_properties.read_from_registry() ;

		BOOL old_classic_if = m_properties.get_classic_if() ;
		int old_preferred_gui = m_properties.get_preferred_gui_lang() ;
		int old_segmentation_type = m_properties.get_segmentation_type() ;

		if ( m_properties.get_preferred_gui_lang() == LANG_ENGLISH )
		{
			CPropertiesDlgE props_dlg(m_properties) ;
			if ( IDCANCEL == props_dlg.DoModal( ) ) 
			{
				return ;
			}

			m_properties = props_dlg.get_properties() ;
		}
		else
		{
			CPropertiesDlgJ props_dlg(m_properties) ;
			if ( IDCANCEL == props_dlg.DoModal( ) ) 
			{
				return ;
			}

			m_properties = props_dlg.get_properties() ;
		}

		m_properties.write_to_registry() ;
		m_controller->refresh_properties( ) ;

		if ( old_segmentation_type != m_properties.get_segmentation_type() )
		{
			delete m_controller ;
			m_controller = NULL ;

			m_controller = get_listener( m_properties.get_segmentation_type() ) ;
			m_controller->set_app( m_app ) ;
		}

		if ( old_classic_if != m_properties.get_classic_if() ) 
		{
			if ( m_properties.get_classic_if() ) 
			{
				if ( ! m_controller->is_translation_mode() ) 
				{
					m_controller->OnSwitchModeAction() ;
				}
				switch_to_classic_menu() ;
				switch_to_classic_toolbar() ;
				set_classic_if_on() ;
			}
			else
			{
				ATLASSERT( m_controller->is_translation_mode() ) ;

				switch_to_new_menu() ;
				switch_to_translation_toolbar() ;
				set_classic_if_off() ;
			}
		}

		if ( old_preferred_gui != m_properties.get_preferred_gui_lang() ) 
		{
			if ( m_properties.get_preferred_gui_lang() == LANG_ENGLISH )
			{
				gui_to_english() ;
			}
			else
			{
				gui_to_japanese() ;
			}
		}

		CATCH_ALL_VOID(_T("OnMenuPreferences"))
}



// Get a keyboard listener of the desired type
CKeyboardListener* CWordInterface::get_listener(int listener_type)
{
		switch( listener_type ) 
		{
		case SEG_TYPE_CUSTOM:
			return static_cast< CKeyboardListener* >( new CWordControllerWord() ) ;
			break;
		case SEG_TYPE_HTML_DOC:
			return static_cast< CKeyboardListener* >( new CWordControllerHtml() ) ;
			break;
		case SEG_TYPE_WORD_DOC:
			return static_cast< CKeyboardListener* >( new CWordControllerWord() ) ;
			break;
		default:
			return static_cast< CKeyboardListener* >( new CWordControllerWord() ) ;
		}
}
