#include "stdafx.h"
#include "AddIn.h"
#include "Connect.h"

extern CAddInModule _AtlModule;

#include "hooker.h"
#include "File.h"
#include "Path.h"

#include "ErrorTranslator.h"

#include "WordControllerHtml.h"
#include "WordControllerWord.h"

#include "ocidl.h"

#include "ErrorTranslator.h"

#include "resource.h"

#include "ClipboardBackup.h"
#include "input_device_file.h"
#include "stringconversions.h"

#pragma warning( disable : 4702 ) // Unreachable code
#include "LuaState.h"

using namespace except ;

#define _HR( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) \
{ \
	CStringA err_msg ; err_msg.Format("COM failure: %s(%d)", __FILE__, __LINE__) ; \
	logging::log_error((LPCSTR)err_msg); \
	throw CComException( _T("COM Failure"), _hr, _T(__FILE__), __LINE__ ) ; \
} \
}

#define _HR_RET( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) \
{ \
	CStringA err_msg ; err_msg.Format("COM failure: %s(%d)", __FILE__, __LINE__) ; \
	logging::log_warn((LPCSTR)err_msg); \
	return _hr ; \
} \
}

#define _HR_V( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) \
{ \
	CStringA err_msg ; err_msg.Format("COM failure: %s(%d)", __FILE__, __LINE__) ; \
	logging::log_warn((LPCSTR)err_msg); \
	return  ; \
} \
}

#define _HR_NULL( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) \
{ \
	CStringA err_msg ; err_msg.Format("COM failure: %s(%d)", __FILE__, __LINE__) ; \
	logging::log_warn((LPCSTR)err_msg); \
	return NULL ; \
} \
}

#define _HR_FALSE( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( _hr == S_FALSE ) return _hr ;\
	if ( FAILED( _hr ) ) \
{ \
	CStringA err_msg ; err_msg.Format("COM failure: %s(%d)", __FILE__, __LINE__) ; \
	logging::log_warn((LPCSTR)err_msg); \
	throw CComException( _T("COM Failure"), _hr, _T(__FILE__), __LINE__ ) ; \
} \
}

#define _HRB( x ) \
{ \
	HRESULT _hr = (x) ; \
	ASSERT_HRESULT( _hr ) ; \
	if ( FAILED( _hr ) ) \
{ \
	CStringA err_msg ; err_msg.Format("COM failure: %s(%d)", __FILE__, __LINE__) ; \
	logging::log_warn((LPCSTR)err_msg); \
	return false ; \
} \
}



#define CATCH_C_EXCEPTIONS(_msg) catch ( std::exception &e ) { handle_exception(e, _msg) ; }
#define CATCH_COM_EXCEPTIONS(_msg) catch ( CComException &e ) { handle_exception(e, _msg) ; }
#define CATCH_WIN_EXCEPTIONS(_msg) catch ( CWinException &e ) { handle_exception(e, _msg) ; }
#define CATCH_SW_EXCEPTIONS(_msg) catch ( CSWException &e ) { handle_exception(e, _msg) ; }
#define CATCH_EXCEPTIONS(_msg) catch ( CException &e ) { handle_base_exception(e, _msg) ; }
#define CATCH_COM_ERRORS(_msg) catch (_com_error &e ) { handle_exception(e, _msg) ; }
#define CATCH_ATL_EXCEPTIONS(_msg) catch (CAtlException &e ) { handle_exception(e, _msg) ; }

#define CATCH_ALL(msg) CATCH_C_EXCEPTIONS(msg) CATCH_COM_EXCEPTIONS(msg) CATCH_WIN_EXCEPTIONS(msg) CATCH_SW_EXCEPTIONS(msg) CATCH_EXCEPTIONS(msg) CATCH_COM_ERRORS(msg) \
	catch(...)\
{\
	logging::log_error("An unknown error occurred while initializing the Felix interface.") ; \
	MessageBox(NULL, _T("An error occurred while initializing the Felix interface\r\rIf the problem persists, please contact technical support."), _T("Felix WordAssist"), MB_OK ) ;  \
}

#define JAPANESE_OFFSET ( IDS_MENU_JAPANESE_FIRST - IDS_MENU_ENGLISH_FIRST )

_ATL_FUNC_INFO OnClickButtonInfo =
{CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_BYREF | VT_BOOL}};

_ATL_FUNC_INFO WindowActivateInfo = 
{CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH | VT_BYREF, VT_DISPATCH | VT_BYREF }};
_ATL_FUNC_INFO WindowDeactivateInfo = 
{CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH | VT_BYREF, VT_DISPATCH | VT_BYREF }};

// 0x00000003
_ATL_FUNC_INFO DocumentChangeInfo = 
{CC_STDCALL, VT_EMPTY, 0, VT_EMPTY};

// 0x00000004
_ATL_FUNC_INFO DocumentOpenInfo = 
{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH | VT_BYREF}} ;

// 0x00000009
_ATL_FUNC_INFO NewDocumentInfo = 
{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH | VT_BYREF}} ;

// 0x0000000C
_ATL_FUNC_INFO WindowSelectionChangeInfo = 
{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH | VT_BYREF}} ;

_ATL_FUNC_INFO OnBeforeCloseInfo =
{CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH | VT_BYREF, VT_BYREF | VT_BOOL}};



/*!
* \brief
* Callback from Word. We hook this and set up our addin.
* 
* \remarks
* One of the methods we must implement for the AddInDesignerObjects::_IDTExtensibility2 interface.
* 
* \see
* AddInDesignerObjects::_IDTExtensibility2
*/
STDMETHODIMP CConnect::OnConnection(IDispatch *pApplication, 
									AddInDesignerObjects::ext_ConnectMode /*ConnectMode*/, 
									IDispatch *pAddInInst, 
									SAFEARRAY ** /*custom*/ )
{
	setup_stuff() ;

	try 
	{
		logging::set_logger(logger_ptr(new file_logger));
		logging::log_debug("OnConnection") ;

		luawrapper::LuaState lua_state ;
		if (! lua_state.is_valid())
		{
			logging::log_error("Failed to initialize Lua VM.") ;
			logging::log_error(lua_state.get_string(-1)) ;
		}


		// store/restore the clipboard contents
		CClipboardBackup cbb ;

		m_app = pApplication ;
		m_pAddInInstance = pAddInInst ;

		m_properties.read_from_registry() ;

		setUpController();
		try
		{
			logging::log_debug("Setting application") ;
			m_controller->set_app(pApplication) ;
		}
		catch (_com_error& e)
		{
			logging::log_error("Error setting Word application data") ;
			logging::log_exception(e) ;
		}

		setUpCommandBars();
		switch_to_translation_toolbar() ;

		advise_document_events() ;

		load_keyboard_shortcuts() ;
		m_mapper.m_target = m_controller ;
		installhook( &m_keyboard_shortcuts ) ;
		m_keyboard_shortcuts.m_on_toggle_shortcuts = boost::bind(&CConnect::on_toggle_shortcuts, this, _1) ;

		logging::log_debug("connection complete") ;
	}
	CATCH_ALL( _T("En error occured loading WordAssist Addin (OnConnection)") ) ;



	return S_OK;
}


bool CConnect::is_2007_version()
{
	return string2double(wstring(static_cast< LPCWSTR >( this->m_app->Version ))) >= 12.0f ;
}

void CConnect::load_keyboard_shortcuts()
{
	input_device_ptr input(new InputDeviceFile) ;
	output_device_ptr output(new OutputDeviceFile) ;
	m_keyboard_shortcuts.load(get_shortcuts_text(SHORTCUTS_FILE, input, output)) ;
}
void __stdcall CConnect::OnMenuSwitchMode( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnMenuSwitchMode") ;
		CClipboardBackup cbb ;

		m_controller->OnSwitchModeAction() ;

		if ( m_controller->is_translation_mode() ) 
		{
			switch_to_translation_toolbar() ;
			switch_to_translation_menu() ;
			m_menu_auto_trans->Enabled = VARIANT_TRUE ;
		}
		else 
		{
			switch_to_review_toolbar() ;
			switch_to_review_menu() ;
			m_menu_auto_trans->Enabled = VARIANT_FALSE ;
		}
	}

	CATCH_ALL(_T("Switching menu modes")) ;
}

/*!
\brief
Creates the Felix toolbar and menu.

\remarks
We save and then restore the clipboard state
*/
void CConnect::setUpCommandBars()
{
	logging::log_debug("Initializing Felix toolbar") ;
	// Get the CommandBars interface that represents Word's
	//toolbars & menu items    

	office_cmd_bars spCmdBars = m_app->GetCommandBars( ) ;

	try
	{
		load_toolbar(spCmdBars);


		if ( this->is_2007_version())
		{
			m_toolbar->Visible = VARIANT_TRUE ;
		}
	}
	catch (CException& e)
	{
		logging::log_error("Failed to initialize Felix toolbar") ;
		logging::log_exception(e) ;
		CString message(_T("Failed to initialize toolbar")) ;
		e.add_to_description(message) ;
		throw CException(e) ;
	}


	addCorrectMenu( spCmdBars );
}

/*!
\brief
Adds the classic or new menu, depending on the preferences.

\remarks
We will stop offering the new menu in 2.0
*/
void CConnect::addCorrectMenu(office_cmd_bars& spCmdBars)
{
	logging::log_debug("addCorrectMenu") ;
	if ( m_properties.get_classic_if() ) 
	{
		add_classic_menu( spCmdBars ) ;
	}
	else
	{
		add_menu( spCmdBars ) ;
	}

}

/*!
* Initializes m_controller.
*/
void CConnect::setUpController()
{
	logging::log_debug("setUpController") ;
	m_controller = get_listener( m_properties.get_segmentation_type() ) ;
	m_mapper.m_target = m_controller ;
	set_listener(&m_keyboard_shortcuts) ;
}

STDMETHODIMP CConnect::OnDisconnection(AddInDesignerObjects::ext_DisconnectMode /*RemoveMode*/, SAFEARRAY ** /*custom*/ )
{
	try
	{
		logging::log_debug("OnDisconnection") ;
		m_app = NULL;
		m_pAddInInstance = NULL;
		logging::log_debug("Cleaning up logger") ;
		logging::set_logger(logger_ptr()) ;
	}
	catch( _com_error &e ) 
	{
		logging::log_error("CConnect::OnDisconnection - _com_error") ;
		logging::log_exception(e) ;
	}
	catch( CException &myException )
	{
		logging::log_error("CConnect::OnDisconnection - CException") ;
		logging::log_exception(myException) ;
	}
	catch ( std::exception &stdE )
	{
		logging::log_error("CConnect::OnDisconnection - std::exception") ;
		logging::log_error(stdE.what()) ;
	}
	catch( CAtlException &atlE )
	{
		logging::log_error("CConnect::OnDisconnection - CAtlException") ;
		CComException msg(atlE.m_hr) ;
		logging::log_exception(msg) ;
	}
	catch( ... )
	{
		logging::log_error("CConnect::OnDisconnection - other exception") ;
	}

	return S_OK;
}

/*!
* \brief
* Callback from Word.
* 
* \remarks
* One of the methods we must implement for the AddInDesignerObjects::_IDTExtensibility2 interface.
* 
* \see
* AddInDesignerObjects::_IDTExtensibility2
*/
STDMETHODIMP CConnect::OnAddInsUpdate (SAFEARRAY ** /*custom*/ )
{
	return S_OK;
}

/*!
* \brief
* Callback from Word.
* 
* \remarks
* One of the methods we must implement for the AddInDesignerObjects::_IDTExtensibility2 interface.
* 
* \see
* AddInDesignerObjects::_IDTExtensibility2
*/
STDMETHODIMP CConnect::OnStartupComplete (SAFEARRAY ** /*custom*/ )
{
	return S_OK;
}

/*!
* \brief
* Time to do cleanup.
* 
* \remarks
* One of the methods we must implement for the AddInDesignerObjects::_IDTExtensibility2 interface.
* 
* \see
* AddInDesignerObjects::_IDTExtensibility2
*/
STDMETHODIMP CConnect::OnBeginShutdown (SAFEARRAY ** /*custom*/ )
{
	try
	{
		logging::log_debug("OnBeginShutdown") ;
		persist_app_state() ;

		logging::log_debug("Cleaning up document events") ;
		unadvise_document_events() ;

		logging::log_debug("Cleaning up buttons") ;
		unadvise_button_items() ;

		logging::log_debug("Cleaning up menu items") ;
		const HRESULT unadvise_menu_items_result = unadvise_menu_items() ;
		if (FAILED(unadvise_menu_items_result))
		{
			logging::log_error("Failed to unadvise menu items") ;
			_com_error err(unadvise_menu_items_result) ;
			logging::log_exception(err) ;
		}

		logging::log_debug("Cleaning up keyboard listener") ;
		uninstallhook( &m_keyboard_shortcuts ) ;

		m_controller->shut_down() ;
		logging::log_debug("shutdown complete") ;
	}
	catch( _com_error &e ) 
	{
		logging::log_error("CConnect::OnBeginShutdown - _com_error") ;
		CComException ce(e) ;
		logging::log_exception(ce) ;
	}
	catch( CException &myException )
	{
		logging::log_error("CConnect::OnBeginShutdown - CException") ;
		logging::log_exception(myException) ;
	}
	catch ( std::exception &stdE )
	{
		logging::log_error("CConnect::OnBeginShutdown - std::exception") ;
		CStringA msg = stdE.what() ;
		logging::log_error(stdE.what()) ;
	}
	catch( CAtlException &atlE )
	{
		logging::log_error("CConnect::OnBeginShutdown - std::exception") ;
		CComException msg(atlE.m_hr) ;
		logging::log_exception(msg) ;
	}
	catch( ... )
	{
		logging::log_error("CConnect::OnBeginShutdown - other exception") ;
	}

	return S_OK;
}

/*!
* \brief
* Adds the Felix menu.
*
* \todo
* Add separate menu class (CMenuListener)
*/
HRESULT CConnect::add_menu( office_cmd_bars &spCmdBars )
{
	logging::log_debug( "CConnect::add_menu" ) ;

	MSOffice::CommandBarPtr spActiveMenuBar = spCmdBars->GetActiveMenuBar() ;

	office_cmd_bar_ctls spCmdBarCtrls = spActiveMenuBar->GetControls() ;

	try
	{
		kill_old_menu( spCmdBarCtrls ) ;
	}
	catch (CException& e)
	{
		logging::log_error("Failed to remove old menu") ;
		CString message(_T("Failed to remove old menu")) ;
		e.add_to_description(message) ;
		throw CException(e) ;
	}

	_variant_t vType( MSOffice::msoControlPopup ) ;
	_variant_t vTemp(VARIANT_FALSE) ; // menu is temporary        

	m_trans_assist_menu_popup = spCmdBarCtrls->Add(
		vType,					// type
		vtMissing,				// id
		vtMissing, 				// parameter
		vtMissing, 				// before
		vTemp					// temporary
		) ;


	wstring felix_caption = L"Feli&x" ;

	if ( ! m_properties.get_shortcuts_active() ) 
	{
		felix_caption += L" [-]" ;
	}

	m_trans_assist_menu_popup->Caption = string2BSTR(felix_caption) ;

	CComPtr< IDispatch > spDispatch ;
	_HR_RET( m_trans_assist_menu_popup->get_Control( &spDispatch ) ) ;

	CComQIPtr< MSOffice::CommandBarPopup > spCmdBarPopup( spDispatch ) ;
	ATLASSERT( spCmdBarPopup) ;
	_HR_RET( spCmdBarPopup->put_Visible( VARIANT_TRUE ) ) ;

	office_cmd_bar_ctls spMenuControls;
	COM_ENFORCE( spCmdBarPopup->get_Controls(&spMenuControls), _T("Failed to get menu bar controls") ) ;

	int string_offset = 0 ;

	if ( m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE )
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

	m_gloss_menu_popup->Caption = R2W(IDS_MENU_GET_GLOSS + string_offset) ;
	m_gloss_menu_popup->DescriptionText = R2W(IDS_MENU_GET_GLOSS_TT + string_offset) ; 
	m_gloss_menu_popup->BeginGroup = VARIANT_TRUE ;

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

	//m_MenuAnalyze = add_menu_item( spMenuControls, 0, IDS_MENU_ANALYZE ) ;
	//ATLASSERT( m_MenuAnalyze ) ;

	advise_menu_items() ;

	return S_OK ;
}


/*!
* \brief
* Adds a specific menu item.
*/
command_button_ptr CConnect::add_menu_item(office_cmd_bar_ctls &controls, int button_id, int string_id )
{
	office_cmd_bar_ctl spMenuItemCtrl ;
	_variant_t vTemp(VARIANT_FALSE); // menu is not temporary        
	COM_ENFORCE( controls->raw_Add(
		_variant_t( MSOffice::msoControlButton ),
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

	CStringW tag_text ;
	tag_text.LoadString( string_id ) ;
	tag_text += L"Menu" ;
	set_tag_text( menu_button, _bstr_t( (LPCWSTR)tag_text ) ) ;

	// show the menu 

	_HR_NULL( menu_button->put_Visible( VARIANT_TRUE ) ) ;

	return menu_button ;
}


/*!
* Adds our toolbar.
*/
HRESULT CConnect::add_toolbar( office_cmd_bars &spCmdBars )
{
	logging::log_debug("CConnect::add_toolbar") ;

	//Add a new toolband through Add method
	//m_toolbar points to the newly created toolband
	_variant_t vName(L"Felix Interface");
	// position it below all toolbands
	CComVariant vPos(1); //MsoBarPosition::msoBarTop = 1

	_variant_t vTemp(VARIANT_FALSE); // menu IS NOT temporary        
	//Add a new toolband through Add method
	// vMenuTemp holds an unspecified parameter
	m_toolbar = spCmdBars->Add( vName, vPos, vtMissing, vTemp) ;

	//now get the toolband's CommandBarControls
	office_cmd_bar_ctls spBarControls = m_toolbar->Controls ;

	add_toolbar_items(spBarControls);

	m_toolbar->Visible = VARIANT_TRUE ;
	return S_OK ;
}

// Add the buttons to the toolbar
void CConnect::add_toolbar_items( office_cmd_bar_ctls spBarControls )
{
	int string_offset = 0 ;

	m_properties.read_from_registry();
	if ( m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE )
	{
		string_offset = JAPANESE_OFFSET ;
	}

	// add buttons
	m_button_lookup = add_toolbar_item( spBarControls, IDB_LOOKUP, IDS_MENU_LOOKUP + string_offset ) ;
	m_button_lookup_next = add_toolbar_item( spBarControls, IDB_LOOKUP_NEXT, IDS_MENU_LOOKUP_NEXT + string_offset ) ;
	m_button_get = add_toolbar_item( spBarControls, IDB_GET, IDS_MENU_GET + string_offset ) ;
	m_button_get_and_next = add_toolbar_item( spBarControls, IDB_GET_AND_NEXT, IDS_MENU_GET_AND_NEXT + string_offset ) ;
	m_button_set = add_toolbar_item( spBarControls, IDB_SET, IDS_MENU_REGISTER + string_offset ) ;
	m_button_set_and_next = add_toolbar_item( spBarControls, IDB_SET_AND_NEXT, IDS_MENU_SET_AND_NEXT + string_offset ) ;
	m_button_switch_modes = add_toolbar_item( spBarControls, IDB_SWITCH_TO_REVIEW, IDS_MENU_TO_REVIEW_MODE + string_offset ) ;
	m_button_help = add_toolbar_item( spBarControls, IDB_HELP, IDS_MENU_HELP + string_offset ) ;
}

/*!
* Adds an individual toolbar item.
*/
command_button_ptr CConnect::add_toolbar_item(office_cmd_bar_ctls &controls, int button_id, int string_id)
{
	//MsoControlType::msoControlButton = 1
	_variant_t vToolBarType(1);
	//show the toolbar?
	_variant_t vShow(VARIANT_TRUE);

	// add buttons
	office_cmd_bar_ctl spNewBar; 
	_variant_t vEmpty(DISP_E_PARAMNOTFOUND, VT_ERROR);            
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

	set_command_bar_text( button, string_id ) ;

	CStringW tag_text ;
	tag_text.LoadString( string_id ) ;
	tag_text += L"Toolbar" ;
	set_tag_text( button, (LPCWSTR)tag_text ) ;

	button->Enabled = VARIANT_TRUE;
	button->Visible = VARIANT_TRUE; 

	return button ;
}

/*!
* \brief
* Sets the GUI language to English.
*/
void CConnect::gui_to_english()
{
	logging::log_debug("gui_to_english") ;
	gui_to_language( 0 ) ;
}

/*!
* \brief
* Sets the GUI language to Japanese.
*/
void CConnect::gui_to_japanese()
{
	logging::log_debug("gui_to_japanese") ;
	gui_to_language( JAPANESE_OFFSET ) ;
}

/*!
* Removes the old Felix menu.
*/
void CConnect::kill_old_menu(office_cmd_bar_ctls controls)
{
	logging::log_debug("kill_old_menu") ;
	int menu_count = 0 ;
	_HR( controls->get_Count( &menu_count ) ) ;

	// 1-based index for COM
	int i = 1 ;
	while ( i <= menu_count )
	{
		office_cmd_bar_ctl current_command_bar = controls->Item[ i ] ;

		_bstr_t caption = current_command_bar->Caption ;

		TRACE( caption ) ;

		wstring candidate_for_death = BSTR2wstring( caption ) ;
		boost::replace_all( candidate_for_death, L"&", L"" ) ;
		boost::to_lower(candidate_for_death) ;
		if ( candidate_for_death.find( L"felix" ) != wstring::npos ||
			candidate_for_death.find( L"transassist" ) != wstring::npos  )
		{
			ATLTRACE( "YEEHAW!!\n" ) ;
			current_command_bar->Delete() ;
			--menu_count ;
		}
		else
		{
			++i ;
		}

		caption = L"" ;
	}

	unadvise_menu_items() ;
}

/*!
* Saves our app state to the registry.
*/
void CConnect::persist_app_state()
{
	logging::log_debug("persist_app_state") ;
	m_properties.read_from_registry() ;

	// toolbar state
	if ( m_toolbar->Visible )
	{
		m_properties.set_toolbar_visible( FALSE ) ;
	}
	else
	{
		m_properties.set_toolbar_visible( TRUE ) ;
	}
	m_properties.set_is_not_fresh_install() ;

	m_properties.write_to_registry() ;
}

/*!
* Sink events for our menu items.
*/
HRESULT CConnect::advise_menu_items()
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

/*!
* Frees up our menu items before shutdown.
*/
HRESULT CConnect::unadvise_menu_items()
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

	//_HR_RET( MenuAnalyzeEventImpl::DispEventUnadvise( (IUnknown*)m_MenuAnalyze ) ) ;

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

	//m_MenuAnalyze = NULL ;

	return S_OK ;
}


void CConnect::set_menu_text(command_button_ptr &button, int string_id)
{
	set_command_bar_text( button, string_id ) ;

	CStringW description_text ;
	bool success = !! description_text.LoadString( string_id+1 ) ;
	ATLASSERT( success ) ;
	if (! success)
	{
		logging::log_error("Failed to load button text with id " + int2string(string_id)) ;
		description_text = L"Button" ;
	}

	button->DescriptionText = (LPCWSTR)description_text ;
}

void CConnect::set_command_bar_text(command_button_ptr &button, int string_id)
{
	CStringW caption_text ;
	bool success = !! caption_text.LoadString( string_id ) ;
	ATLASSERT( success ) ;

	button->Caption = (LPCWSTR)caption_text ;

	CStringW tooltip_text ;
	success = !! tooltip_text.LoadString( string_id+1 ) ;
	ATLASSERT( success ) ;

	button->TooltipText = (LPCWSTR)tooltip_text ;

}

void CConnect::gui_to_language(int lang_offset)
{
	int string_offset = lang_offset ;

	if ( m_properties.get_classic_if() ) 
	{
		string_offset += ( IDS_CLASSIC_MENU_ENGLISH_FIRST - IDS_MENU_ENGLISH_FIRST ) ;
	}

	ATLASSERT( R2TS( IDS_MENU_HELP +1 ) == R2TS( IDS_MENU_HELP_TT) ) ;
	ATLASSERT( R2TS( IDS_MENU_HELP_J +1 ) == R2TS( IDS_MENU_HELP_TT_J) ) ;

	int string_id = IDS_MENU_HELP + string_offset ;
	set_command_bar_text( m_button_help, string_id ) ;

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
	set_command_bar_text( m_button_get, string_id ) ;

	ATLASSERT( R2TS( IDS_MENU_GET_AND_NEXT +1 ) == R2TS( IDS_MENU_GET_AND_NEXT_TT) ) ;
	ATLASSERT( R2TS( IDS_MENU_GET_AND_NEXT_J +1 ) == R2TS( IDS_MENU_GET_AND_NEXT_TT_J) ) ;

	string_id = IDS_MENU_GET_AND_NEXT + string_offset ;
	set_menu_text( m_menu_get_and_next, string_id ) ;
	set_command_bar_text( m_button_get_and_next, string_id ) ;

	string_id = IDS_MENU_REGISTER + string_offset ;
	set_menu_text( m_menu_set, string_id ) ;
	set_command_bar_text( m_button_set, string_id ) ;

	string_id = IDS_MENU_SET_AND_NEXT + string_offset ;
	set_menu_text( m_menu_set_and_next, string_id ) ;
	set_command_bar_text( m_button_set_and_next, string_id ) ;

	string_id = IDS_MENU_LOOKUP + string_offset ;
	set_menu_text( m_menu_lookup, string_id ) ;
	set_command_bar_text( m_button_lookup, string_id ) ;

	string_id = IDS_MENU_LOOKUP_NEXT + string_offset ;
	set_menu_text( m_menu_lookup_next, string_id ) ;
	set_command_bar_text( m_button_lookup_next, string_id ) ;

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

	CStringW caption ; caption.LoadString( IDS_MENU_GET_GLOSS + string_offset ) ;
	CStringW tooltip ; tooltip.LoadString( IDS_MENU_GET_GLOSS_TT + string_offset ) ;
	m_gloss_menu_popup->Caption = (LPCWSTR)caption ;
	m_gloss_menu_popup->DescriptionText = (LPCWSTR)tooltip ;

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
		set_command_bar_text( m_button_switch_modes, string_id ) ;
	}
	else
	{
		set_command_bar_text( m_button_switch_modes, IDS_MENU_TO_REVIEW_MODE + lang_offset ) ;
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
}


/*!
* \brief
* Set the button's image using the Picture property.
* 
* Tries setting the image of the picture using the undocumented Picture property.
* This is supposed to work with Office XP and above, but I don't think I've gotten it
* to work yet. 
*/
bool CConnect::load_picture(command_button_ptr &button, int button_id)
{
	try
	{
		HINSTANCE hInst = _AtlModule.GetResourceInstance() ;

		CComPtr< IPicture > iPicture ;
		PICTDESC *pd = new PICTDESC;
		pd->cbSizeofstruct = sizeof(PICTDESC);
		pd->picType = PICTYPE_BITMAP;
		pd->bmp.hbitmap = LoadBitmap( hInst, MAKEINTRESOURCE(button_id));
		pd->bmp.hpal = 0;
		HRESULT hr = OleCreatePictureIndirect( pd, IID_IPictureDisp, FALSE, (void**)(&iPicture));
		delete pd;
		if ( FAILED( hr ) ) 
		{
			if ( hr == CTL_E_INVALIDPICTURE ) 
			{
				ATLTRACE( "%s(%d): 0x%0X Invalid picture\n", __FILE__, __LINE__, hr ) ;
				TRACE( button_id ) ;
			}
			else
			{
				TRACE_HRESULT( hr ) ;
			}
			return false ;
		}

		CDispatchWrapper but( CComPtr<IDispatch>( (IDispatch*)button ) ) ;
		_variant_t vPicture ( (IUnknown*)iPicture ) ;
		but.prop_put( L"Picture", vPicture ) ;
	}
	catch( CComException &e )
	{
		TRACE( e.what() ) ;
		TRACE( e.description() ) ;
		logging::log_error("Failed to load picture") ;
		logging::log_exception(e) ;
		return false ;

	}
	catch ( _com_error &e ) 
	{
		TRACE( e.ErrorMessage() ) ;
		TRACE_HRESULT( e.Error() ) ;
		logging::log_error("Failed to load picture") ;
		logging::log_exception(e) ;
		return false ;
	}

	return true ;

}

void CConnect::set_tag_text(command_button_ptr &button, const _bstr_t &tag_text )
{
	_HR_V( button->put_Tag( tag_text ) ) ;
}


bool CConnect::set_button_image(command_button_ptr &button, const int image_id)
{
	HRESULT hr = S_OK ;
	try
	{
		if ( load_picture( button, image_id ) ) 
		{
			return true ;
		}

		CBitmap bMp ;
		bMp.LoadBitmap( image_id ) ;
		//=(HBITMAP)::LoadImage(_AtlModule.GetResourceInstance(),
		//MAKEINTRESOURCE(image_id),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS);

		// put bitmap into Clipboard
		::OpenClipboard(NULL) ;
		::EmptyClipboard() ;
		::SetClipboardData(CF_BITMAP, (HANDLE)(HBITMAP)bMp) ;
		::CloseClipboard() ;

		hr = button->PasteFace() ;
		ATLASSERT( SUCCEEDED( hr ) && "Failed to paste image into command button object" )  ;
		if (! SUCCEEDED(hr))
		{
			CComException e(hr) ;
			logging::log_error("Failed to set image for button id " + int2string(image_id)) ;
			logging::log_exception(e) ;
		}

		// clear clipboard
		::OpenClipboard(NULL) ;
		::EmptyClipboard() ;
		::CloseClipboard() ;
	}
	catch ( CException &e ) 
	{
		logging::log_error("exception thrown in set_button_image!") ;
		logging::log_exception(e) ;
		return false ;
	}
	catch ( _com_error &er ) 
	{
		CComException e(er) ;
		logging::log_error("_com_error thrown in set_button_image") ;
		logging::log_exception(e) ;
		return false ;
	}
	catch ( ... ) 
	{
		logging::log_error("Unknown exception thrown in set_button_image!") ;
		ATLASSERT( FALSE && "Exception thrown in set_button_image!" ) ;
		return false ;
	}

	return ( ! FAILED( hr ) ) ;
}

void CConnect::switch_to_classic_toolbar()
{
	logging::log_debug("switch_to_classic_toolbar") ;
	int string_offset = IDS_CLASSIC_MENU_ENGLISH_FIRST - IDS_MENU_ENGLISH_FIRST ;

	if ( m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE )
	{
		string_offset += JAPANESE_OFFSET ;
	}

	this->unadvise_button_items() ;
	load_toolbar(m_app->GetCommandBars( )) ;

	// add buttons
	set_button_image( m_button_lookup, IDB_LOOKUP ) ;
	set_command_bar_text( m_button_lookup, IDS_MENU_LOOKUP + string_offset ) ;

	set_button_image( m_button_lookup_next, IDB_LOOKUP_NEXT ) ;
	set_command_bar_text( m_button_lookup_next, IDS_MENU_LOOKUP_NEXT + string_offset ) ;

	set_button_image( m_button_get, IDB_GET ) ;
	set_command_bar_text( m_button_get, IDS_MENU_GET + string_offset ) ;

	set_button_image( m_button_get_and_next, IDB_GET_AND_NEXT ) ;
	set_command_bar_text( m_button_get_and_next, IDS_MENU_GET_AND_NEXT + string_offset ) ;

	set_button_image( m_button_set, IDB_SET ) ;
	set_command_bar_text( m_button_set, IDS_MENU_REGISTER + string_offset ) ;

	set_button_image( m_button_set_and_next, IDB_SET_AND_NEXT ) ;
	set_command_bar_text( m_button_set_and_next, IDS_MENU_SET_AND_NEXT + string_offset ) ;

	set_button_image( m_button_switch_modes, IDB_GLOSS_N ) ;
	set_command_bar_text( m_button_switch_modes, IDS_MENU_GLOSS_N + string_offset ) ;

}


void CConnect::switch_to_translation_toolbar()
{
	logging::log_debug("switch_to_translation_toolbar") ;
	int string_offset = 0 ;

	if ( m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE )
	{
		string_offset += JAPANESE_OFFSET ;
	}

	// add buttons
	set_button_image( m_button_lookup, IDB_LOOKUP ) ;
	set_command_bar_text( m_button_lookup, IDS_MENU_LOOKUP + string_offset ) ;

	set_button_image( m_button_lookup_next, IDB_LOOKUP_NEXT ) ;
	set_command_bar_text( m_button_lookup_next, IDS_MENU_LOOKUP_NEXT + string_offset ) ;

	set_button_image( m_button_get, IDB_GET ) ;
	set_command_bar_text( m_button_get, IDS_MENU_GET + string_offset ) ;

	set_button_image( m_button_get_and_next, IDB_GET_AND_NEXT ) ;
	set_command_bar_text( m_button_get_and_next, IDS_MENU_GET_AND_NEXT + string_offset ) ;

	set_button_image( m_button_set, IDB_SET ) ;
	set_command_bar_text( m_button_set, IDS_MENU_REGISTER + string_offset ) ;

	set_button_image( m_button_set_and_next, IDB_SET_AND_NEXT ) ;
	set_command_bar_text( m_button_set_and_next, IDS_MENU_SET_AND_NEXT + string_offset ) ;

	set_button_image( m_button_switch_modes, IDB_SWITCH_TO_REVIEW ) ;
	set_command_bar_text( m_button_switch_modes, IDS_MENU_TO_REVIEW_MODE + string_offset ) ;
}


void CConnect::switch_to_review_toolbar( )
{
	logging::log_debug("switch_to_review_toolbar") ;
	int string_offset = 0 ;

	if ( m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE )
	{
		string_offset = IDS_MENU_JAPANESE_FIRST - IDS_MENU_ENGLISH_FIRST ;
	}


	// add buttons
	set_button_image( m_button_lookup, IDB_LOOKUP_TRANS ) ;
	set_command_bar_text( m_button_lookup, IDS_MENU_LOOKUP_TRANS + string_offset ) ;

	set_button_image( m_button_lookup_next, IDB_LOOKUP_NEXT_TRANS ) ;
	set_command_bar_text( m_button_lookup_next, IDS_MENU_LOOKUP_NEXT_TRANS + string_offset ) ;

	set_button_image( m_button_get, IDB_RESTORE ) ;
	set_command_bar_text( m_button_get, IDS_MENU_RESTORE + string_offset ) ;

	set_button_image( m_button_get_and_next, IDB_RESTORE_AND_NEXT ) ;
	set_command_bar_text( m_button_get_and_next, IDS_MENU_RESTORE_AND_NEXT + string_offset ) ;

	set_button_image( m_button_set, IDB_CORRECT ) ;
	set_command_bar_text( m_button_set, IDS_MENU_CORRECT_TRANS + string_offset ) ;

	set_button_image( m_button_set_and_next, IDB_CORRECT_AND_NEXT ) ;
	set_command_bar_text( m_button_set_and_next, IDS_MENU_CORRECT_AND_NEXT + string_offset ) ;

	set_button_image( m_button_switch_modes, IDB_SWITCH_TO_TRANS ) ;
	set_command_bar_text( m_button_switch_modes, IDS_MENU_TO_TRANS_MODE + string_offset ) ;
}


void CConnect::switch_to_new_menu()
{
	logging::log_debug("switch_to_new_menu") ;
	office_cmd_bars spCmdBars; 
	COM_ENFORCE( m_app->get_CommandBars( &spCmdBars ), _T("Failed to retrieve command bars collection from app") ) ;
	ATLASSERT(spCmdBars);

	add_menu( spCmdBars ) ;
}

void CConnect::switch_to_classic_menu()
{
	logging::log_debug("switch_to_classic_menu") ;
	office_cmd_bars spCmdBars; 
	COM_ENFORCE( m_app->get_CommandBars( &spCmdBars ), _T("Failed to retrieve command bars collection from app") ) ;
	ATLASSERT(spCmdBars);

	add_classic_menu( spCmdBars ) ;
}

void CConnect::switch_to_translation_menu()
{
	logging::log_debug("switch_to_translation_menu") ;
	office_cmd_bars spCmdBars; 
	COM_ENFORCE( m_app->get_CommandBars( &spCmdBars ), _T("Failed to retrieve command bars collection from app") ) ; ;
	ATLASSERT(spCmdBars);

	add_menu( spCmdBars ) ;
}

void CConnect::switch_to_review_menu()
{
	logging::log_debug("switch_to_review_menu") ;
	office_cmd_bars spCmdBars; 
	COM_ENFORCE( m_app->get_CommandBars( &spCmdBars ), _T("Failed to retrieve command bars collection from app") ) ; ;
	ATLASSERT(spCmdBars);

	add_menu_review( spCmdBars ) ;
}

HRESULT CConnect::add_classic_menu( office_cmd_bars &spCmdBars )
{
	CComPtr< MSOffice::CommandBar >   spActiveMenuBar;
	COM_ENFORCE( spCmdBars->get_ActiveMenuBar(&spActiveMenuBar), _T("Failed to get menu bar") ) ;

	office_cmd_bar_ctls spCmdBarCtrls;
	COM_ENFORCE( spActiveMenuBar->get_Controls(&spCmdBarCtrls), _T("Failed to get menu bar controls") ) ;

	kill_old_menu( spCmdBarCtrls ) ;

	_variant_t vType( MSOffice::msoControlPopup ) ;
	_variant_t vTemp(VARIANT_FALSE) ; // menu is temporary        
	COM_ENFORCE( spCmdBarCtrls->raw_Add( 
		vType,									// type
		vtMissing,								// id
		vtMissing, 								// parameter
		vtMissing, 								// before
		vTemp, 			// temporary
		&m_trans_assist_menu_popup ), 			// the control
		_T("Failed to add popup menu") ) ;

	_HR_RET( m_trans_assist_menu_popup->put_Caption( _bstr_t( L"Feli&x" ) ) ) ;

	CComPtr< IDispatch > spDispatch ;
	_HR_RET( m_trans_assist_menu_popup->get_Control( &spDispatch ) ) ;

	CComQIPtr< MSOffice::CommandBarPopup > spCmdBarPopup( spDispatch ) ;
	ATLASSERT( spCmdBarPopup) ;
	_HR_RET( spCmdBarPopup->put_Visible( VARIANT_TRUE ) ) ;

	office_cmd_bar_ctls spMenuControls;
	COM_ENFORCE( spCmdBarPopup->get_Controls(&spMenuControls), _T("Failed to get menu bar controls") ) ;

	int string_offset = 0 ;

	if ( m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE )
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

	CStringW caption ; caption.LoadString( IDS_MENU_GET_GLOSS + string_offset ) ;
	CStringW tooltip ; tooltip.LoadString( IDS_MENU_GET_GLOSS_TT + string_offset ) ;
	m_gloss_menu_popup->Caption = (LPCWSTR)( caption ) ;
	m_gloss_menu_popup->DescriptionText = (LPCWSTR)( tooltip );
	m_gloss_menu_popup->BeginGroup = ( VARIANT_TRUE ) ;

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

	return S_OK ;

}

void CConnect::advise_document_events()
{
	HRESULT hr = ActivateDocumentEventImpl::DispEventAdvise( (IUnknown*)m_app );
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
	hr = BeforeDocumentCloseImpl::DispEventAdvise( (IUnknown*)m_app );
	ASSERT_HRESULT( hr ) ;
}

void CConnect::unadvise_document_events()
{
	HRESULT hr = ActivateDocumentEventImpl::DispEventUnadvise( (IUnknown*)m_app );
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
	hr = BeforeDocumentCloseImpl::DispEventUnadvise( (IUnknown*)m_app );	
	ASSERT_HRESULT( hr ) ;

}

void CConnect::unadvise_button_items()
{
	// disconnect buttons
	HRESULT hr = ButtonGetEventImpl::DispEventUnadvise( (IUnknown*)m_button_get  ) ;       
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
	hr = ButtonGlossNEventImpl::DispEventUnadvise( (IUnknown*)m_button_switch_modes  ) ;
	ASSERT_HRESULT( hr ) ;
	hr = ButtonHelpEventImpl::DispEventUnadvise( (IUnknown*)m_button_help  ) ;
	ASSERT_HRESULT( hr ) ;
}


void CConnect::setup_stuff()
{
	set_up_exception_handling() ;

	BOOL success = AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_INTERNET_CLASSES | ICC_USEREX_CLASSES );	// add flags to support other controls
	if (! success)
	{
		logging::log_warn("Failed to initialize common controls.") ;
	}
	ATLASSERT( success ) ;
}
void CConnect::on_toggle_shortcuts(BOOL shortcuts_enabled)
{
	try
	{
		m_properties.read_from_registry() ;
		m_properties.set_shortcuts_active(shortcuts_enabled) ;
		m_properties.write_to_registry() ;
		logging::log_debug("toggle shortcuts") ;

		if ( m_controller->is_translation_mode() ) 
		{
			this->switch_to_translation_menu() ;
		}
		else
		{
			this->switch_to_review_menu() ;
		}
	}
	catch (CException& e)
	{
		logging::log_error("Failed to handle toggle-shortcut command") ;
		logging::log_exception(e) ;
		ATLASSERT("Failed to handle toggle shortcut" && FALSE) ;
	}
	catch (_com_error& e)
	{
		logging::log_error("Failed to handle toggle-shortcut command (_com_error)") ;
		logging::log_exception(e) ;
		ATLASSERT("Failed to handle toggle shortcut" && FALSE) ;
	}
}

HRESULT CConnect::add_menu_review(office_cmd_bars & spCmdBars)
{
	logging::log_debug("add_menu_review") ;

	CComPtr< MSOffice::CommandBar >   spActiveMenuBar;
	COM_ENFORCE( spCmdBars->get_ActiveMenuBar(&spActiveMenuBar), _T("Failed to get menu bar") ) ;

	office_cmd_bar_ctls spCmdBarCtrls;
	COM_ENFORCE( spActiveMenuBar->get_Controls(&spCmdBarCtrls), _T("Failed to get menu bar controls") ) ;

	kill_old_menu( spCmdBarCtrls ) ;

	_variant_t vType( MSOffice::msoControlPopup ) ;
	_variant_t vTemp(VARIANT_FALSE) ; // menu is temporary        
	COM_ENFORCE( spCmdBarCtrls->raw_Add( 
		vType,									// type
		vtMissing,								// id
		vtMissing, 								// parameter
		vtMissing, 								// before
		vTemp, 			// temporary
		&m_trans_assist_menu_popup ), 			// the control
		_T("Failed to add popup menu") ) ;


	wstring felix_caption = L"Feli&x [*]" ;

	if ( ! m_properties.get_shortcuts_active() ) 
	{
		felix_caption += L" [-]" ;
	}

	_HR_RET( m_trans_assist_menu_popup->put_Caption( string2BSTR( felix_caption ) ) ) ;

	CComPtr< IDispatch > spDispatch ;
	_HR_RET( m_trans_assist_menu_popup->get_Control( &spDispatch ) ) ;

	CComQIPtr< MSOffice::CommandBarPopup > spCmdBarPopup( spDispatch ) ;
	ATLASSERT( spCmdBarPopup) ;
	_HR_RET( spCmdBarPopup->put_Visible( VARIANT_TRUE ) ) ;

	office_cmd_bar_ctls spMenuControls;
	COM_ENFORCE( spCmdBarPopup->get_Controls(&spMenuControls), _T("Failed to get menu bar controls") ) ;

	int string_offset = 0 ;

	if ( m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE )
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

	CStringW caption ; caption.LoadString( IDS_MENU_GET_GLOSS + string_offset ) ;
	CStringW tooltip ; tooltip.LoadString( IDS_MENU_GET_GLOSS_TT + string_offset ) ;
	m_gloss_menu_popup->Caption = (LPCWSTR)( caption )  ;
	m_gloss_menu_popup->DescriptionText = (LPCWSTR)( tooltip ) ;
	m_gloss_menu_popup->BeginGroup = ( VARIANT_TRUE ) ;

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

	return S_OK ;
}

void __stdcall CConnect::OnMenuPreferences ( IDispatch *, VARIANT_BOOL *  )
{
	try 
	{
		logging::log_debug("OnMenuPreferences") ;
		BANNER("OnMenuPreferences") ;

		if ( ! m_controller->IsActive() ) 
		{
			return ;
		}

		if ( ! m_properties.read_from_registry() )
		{
			m_properties.setDefaults() ;
		}
		m_properties.set_is_not_fresh_install() ;

		BOOL old_classic_if = m_properties.get_classic_if() ;
		int old_preferred_gui = m_properties.get_preferred_gui_lang() ;
		int old_segmentation_type = m_properties.get_segmentation_type() ;

		if ( m_properties.get_preferred_gui_lang() == PREF_LANG_ENGLISH )
		{
			CPropertiesDlgE props_dlg(m_properties) ;
			INT_PTR result = props_dlg.DoModal( ) ;
			m_controller->load_abbreviations() ;
			load_keyboard_shortcuts() ;
			if ( result <= 0 || result == IDCANCEL ) 
			{
				ATLTRACE("User canceled.\n") ;
				return ;
			}

			ATLTRACE("User clicked 'OK'.\n") ;
			m_properties = props_dlg.get_properties() ;
		}
		else
		{
			CPropertiesDlgJ props_dlg(m_properties) ;
			INT_PTR result = props_dlg.DoModal( ) ;
			m_controller->load_abbreviations() ;
			load_keyboard_shortcuts() ;
			if ( result <= 0 || result == IDCANCEL ) 
			{
				ATLTRACE("User canceled.\n") ;
				return ;
			}

			ATLTRACE("User clicked 'OK'.\n") ;
			m_properties = props_dlg.get_properties() ;
		}

		m_properties.write_to_registry() ;

		m_controller->set_properties(m_properties) ;

		reflectSegType(old_segmentation_type) ;
		reflectClassicPref(old_classic_if);
		reflectPreferredLang(old_preferred_gui);

		logging::log_debug("finished setting user preferences") ;
	}
	CATCH_ALL(_T("OnMenuPreferences")) ;
}



/*!
* \brief
* Get a keyboard listener of the desired type.
*/
CKeyboardListener* CConnect::get_listener(int /*listener_type*/)
{
	logging::log_debug("Initializing listener") ;
//	static CWordControllerHtml htmlController ;
	static CWordControllerWord word_controller ;
	return static_cast< CKeyboardListener* >(&word_controller) ;
	//switch( listener_type ) 
	//{
	//case SEG_TYPE_WORD_DOC:
	//	return static_cast< CKeyboardListener* >( &word_controller ) ;
	//	break;
	//case SEG_TYPE_HTML_DOC:
	//	return static_cast< CKeyboardListener* >( &htmlController ) ;
	//	break;
	//default:
	//	ATLASSERT( "CConnect::get_listener -- Unknown segmentation type!" && NULL ) ;
	//	return static_cast< CKeyboardListener* >( &word_controller ) ;
	//}
}

CConnect::CConnect() :
	m_controller( NULL ),
	m_keyboard_shortcuts(&m_mapper)
{
}

HRESULT CConnect::FinalConstruct()
{
	return S_OK;
}

void CConnect::FinalRelease()
{

}

void __stdcall CConnect::OnAutoTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		const bool shift_key_pressed = !!( ::GetKeyState(VK_SHIFT) & 0x8000 ) ;

		m_controller->OnAutoTransAction(shift_key_pressed) ;
	}
	CATCH_ALL(_T("OnAutoTrans")) ;
}

void __stdcall CConnect::OnAutoTransFuzzy( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		const bool shift_key_pressed = !!( ::GetKeyState(VK_SHIFT) & 0x8000 ) ;

		m_controller->OnAutoTransFuzzyAction(shift_key_pressed) ;
	}
	CATCH_ALL(_T("OnAutoTransFuzzy")) ;
}

void __stdcall CConnect::OnConcordance( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnConcordanceAction( false ) ;
	}
	CATCH_ALL(_T("OnConcordance")) ;
}

void __stdcall CConnect::OnExtendLookup( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnExtendLookupAction( false ) ;
	}
	CATCH_ALL(_T("OnExtendLookup")) ;
}

void __stdcall CConnect::OnRegisterGloss( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnRegisterGlossAction( false ) ;
	}
	CATCH_ALL(_T("OnRegisterGloss")) ;
}

void __stdcall CConnect::OnNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnNextAction() ;
	}
	CATCH_ALL(_T("OnNext")) ;
}

void __stdcall CConnect::OnDelete( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnDeleteAction() ;
	}
	CATCH_ALL(_T("OnDelete")) ;
}

void __stdcall CConnect::OnPrev( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnPrevAction() ;
	}
	CATCH_ALL(_T("OnPrev")) ;
}

void __stdcall CConnect::OnEntry0( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnEntry0Action(shift_key_is_pressed()) ;
	}
	CATCH_ALL(_T("OnEntry0")) ;
}

void __stdcall CConnect::OnEntry1( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnEntry1Action( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry1")) ;
}

void __stdcall CConnect::OnEntry2( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnEntry2Action( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry2")) ;
}

void __stdcall CConnect::OnEntry3( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnEntry3Action( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry3")) ;
}

void __stdcall CConnect::OnEntry4( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnEntry4Action( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry4")) ;
}

void __stdcall CConnect::OnEntry5( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnEntry5Action( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry5")) ;
}

void __stdcall CConnect::OnEntry6( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnEntry6Action( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry6")) ;
}

void __stdcall CConnect::OnEntry7( IDispatch *, VARIANT_BOOL * )
{
	try
	{

		m_controller->OnEntry7Action( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry7")) ;
}

void __stdcall CConnect::OnEntry8( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnEntry8Action( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry8")) ;
}

void __stdcall CConnect::OnEntry9( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnEntry9Action( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry9")) ;
}

void __stdcall CConnect::OnLookupTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnLookupTransAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnLookupTrans")) ;
}

void __stdcall CConnect::OnLookupNextTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnLookupNextTransAction(shift_key_is_pressed()) ;
	}
	CATCH_ALL(_T("OnLookupNextTrans")) ;
}

void __stdcall CConnect::OnTransConcordance( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnTransConcordanceAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnTransConcordance")) ;
}

void __stdcall CConnect::OnExtendTransLookup( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnExtendTransLookupAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnExtendTransLookup")) ;
}

void __stdcall CConnect::OnCorrect( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnCorrectAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnCorrect")) ;
}

void __stdcall CConnect::OnCorrectAndNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnCorrectAndNextAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnCorrectAndNext")) ;
}

void __stdcall CConnect::OnRestore( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnRestoreAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnRestore")) ;
}

void __stdcall CConnect::OnRestoreAndNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnRestoreAndNextAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnRestoreAndNext")) ;
}

void __stdcall CConnect::OnGet( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnGetAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnGet")) ;
}

void __stdcall CConnect::OnSet( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnSetAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnSet")) ;
}

void __stdcall CConnect::OnGetAndNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnGetAndNextAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnGetAndNext")) ;
}

void __stdcall CConnect::OnSetAndNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnSetAndNextAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnSetAndNext")) ;
}

void __stdcall CConnect::OnLookup( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnLookupAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnLookup")) ;
}

void __stdcall CConnect::OnLookupNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->set_auto_off() ;
		m_controller->OnLookupNextAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnLookupNext")) ;
}

void __stdcall CConnect::OnGlossN( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnGlossNAction( shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnGlossN")) ;
}

void __stdcall CConnect::OnButtonGlossN( IDispatch *Ctrl, VARIANT_BOOL *CancelDefault )
{
	try
	{
		if ( ! m_properties.get_classic_if() ) 
		{
			OnMenuSwitchMode( Ctrl, CancelDefault ) ;
		}
		else
		{
			m_controller->OnGlossNAction( shift_key_is_pressed() ) ;
		}
	}
	CATCH_ALL(_T("OnButtonGlossN")) ;
}

void __stdcall CConnect::OnSave( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnSaveMemoryAction( ) ;
	}
	CATCH_ALL(_T("OnSave")) ;
}

void __stdcall CConnect::OnHelp( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_controller->OnHelpAction() ;
	}
	CATCH_ALL(_T("OnHelp")) ;
}

void __stdcall CConnect::OnMenuAnalyze( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		ATLTRACE( "OnMenuAnalyze\n" ) ;

		m_controller->OnAnalyze() ;
	}
	CATCH_ALL(_T("OnMenuAnalyze")) ;
}

// Word events

void __stdcall CConnect::OnWindowActivate( IDispatch *, IDispatch * )
{
	try
	{
		ATLTRACE( "OnWindowActivate\n" ) ;

		m_controller->OnWindowActivate() ;
	}
	CATCH_ALL(_T("OnWindowActivate")) ;
}

void __stdcall CConnect::OnWindowDeactivate( IDispatch *, IDispatch * )
{
	try
	{
		ATLTRACE( "OnWindowDeactivate\n" ) ;

		m_controller->OnWindowDeactivate() ;
	}
	CATCH_ALL(_T("OnWindowDeactivate")) ;
}

void __stdcall CConnect::OnDocumentChange()
{
	try
	{
		ATLTRACE( "OnDocumentChange\n" ) ;

		m_controller->OnWindowActivate() ;
	}
	CATCH_ALL(_T("OnDocumentChange")) ;
}

void __stdcall CConnect::OnDocumentOpen(IDispatch *doc)
{
	try
	{
		ATLTRACE( "OnDocumentOpen\n" ) ;

		m_controller->OnWindowActivate() ;
		m_controller->OnDocumentOpenAction(doc) ;
	}
	catch( _com_error &e ) 
	{
		logging::log_error("CConnect::OnDocumentOpen - _com_error") ;
		logging::log_exception(e) ;
	}
	catch( CException &myException )
	{
		logging::log_error("CConnect::OnDocumentOpen - CException") ;
		logging::log_exception(myException) ;
	}
	catch ( std::exception &stdE )
	{
		logging::log_error("CConnect::OnDocumentOpen - std::exception") ;
		logging::log_error(stdE.what()) ;
	}
	catch( CAtlException &atlE )
	{
		logging::log_error("CConnect::OnDocumentOpen - CAtlException") ;
		CComException msg(atlE.m_hr) ;
		logging::log_exception(msg) ;
	}
	catch( ... )
	{
		logging::log_error("CConnect::OnDocumentOpen - other exception") ;
	}
}
void __stdcall CConnect::OnBeforeDocumentClose( IDispatch* doc, VARIANT_BOOL * )
{
	if(m_properties.get_use_trans_hist())
	{
		try
		{
			try
			{
				logging::log_debug("OnBeforeDocumentClose") ;
				m_controller->close_doc(doc) ;
			}
			catch (_com_error& e)
			{
				logging::log_error("Failed to handle document close event.") ;
				logging::log_exception(e) ;

#ifdef _DEBUG
				CComException ce(e) ;
				ce.notify_user(_T("Failed to handle document close event.")) ;
#endif		
			}
		}
		CATCH_ALL(_T("OnBeforeDocumentClose")) ;
	}
}
void __stdcall CConnect::OnNewDocument(IDispatch *doc)
{
	try
	{
		logging::log_debug("OnNewDocument") ;

		m_controller->OnWindowActivate() ;
		m_controller->OnDocumentOpenAction(doc) ;
	}
	CATCH_ALL(_T("OnNewDocument")) ;
}

void __stdcall CConnect::OnWindowSelectionChange( IDispatch * )
{
	try
	{
		m_controller->OnWindowActivate() ;
	}
	CATCH_ALL(_T("OnWindowSelectionChange")) ;
}

void CConnect::reflectClassicPref( BOOL old_classic_if )
{
	if ( old_classic_if != m_properties.get_classic_if() ) 
	{
		// The user wants classic
		if ( m_properties.get_classic_if() ) 
		{
			// So switch to classic

			if ( ! m_controller->is_translation_mode() ) 
			{
				// No review mode in classic!
				m_controller->OnSwitchModeAction() ;
			}
			switch_to_classic_menu() ;
			switch_to_classic_toolbar() ;
		}
		// Or new
		else
		{
			// So switch to new

			ATLASSERT( m_controller->is_translation_mode() ) ;

			switch_to_new_menu() ;
			switch_to_translation_toolbar() ;
		}
	}
}

void CConnect::reflectPreferredLang( int old_preferred_gui )
{
	if ( old_preferred_gui != m_properties.get_preferred_gui_lang() ) 
	{
		if ( m_properties.get_preferred_gui_lang() == PREF_LANG_ENGLISH )
		{
			gui_to_english() ;
		}
		else
		{
			gui_to_japanese() ;
		}
	}
}

void CConnect::reflectSegType( int old_segmentation_type )
{
	if ( old_segmentation_type != m_properties.get_segmentation_type() )
	{
		// We remember if we were active...
		bool was_active = false ;
		if ( m_controller )
		{
			was_active = m_controller->IsActive() ;
		}
		m_controller = get_listener( m_properties.get_segmentation_type() ) ;
		set_listener(&m_keyboard_shortcuts) ;
		m_controller->set_app( m_app ) ;
		m_mapper.m_target = m_controller ;

		// In order to propagate that info to the new controller
		if ( was_active )
		{
			m_controller->OnWindowActivate() ;
		}
	}
}


void CConnect::load_toolbar( office_cmd_bars spCmdBars )
{
	// attach old created
	_variant_t vName("Felix Interface");
	CComPtr <MSOffice::CommandBar> toolbar ;
	HRESULT hr = spCmdBars->get_Item(vName, &toolbar);

	if ( FAILED(hr) || ! toolbar)
	{
		logging::log_debug("Adding Felix toolbar") ;
		add_toolbar( spCmdBars ) ;
	}
	else
	{
		m_toolbar = toolbar ;
		set_buttons_from_toolbar();
	}
	advise_toolbar_events() ;
}

void CConnect::set_buttons_from_toolbar()
{
	// add buttons
	int i = 0 ;
	m_button_lookup = m_toolbar->Controls->Item[++i] ;
	m_button_lookup_next = m_toolbar->Controls->Item[++i] ;
	m_button_get = m_toolbar->Controls->Item[++i] ;
	m_button_get_and_next = m_toolbar->Controls->Item[++i] ;
	m_button_set = m_toolbar->Controls->Item[++i] ;
	m_button_set_and_next = m_toolbar->Controls->Item[++i] ;
	m_button_switch_modes = m_toolbar->Controls->Item[++i] ;
	m_button_help = m_toolbar->Controls->Item[++i] ;
}
void CConnect::advise_toolbar_events()
{
	// advise toolbar button events
	COM_ENFORCE( ButtonLookupEventImpl::DispEventAdvise( (IUnknown*)m_button_lookup ), 
		"Failed to configure menu button (lookup)" ) ;
	COM_ENFORCE( ButtonLookupNextEventImpl::DispEventAdvise( (IUnknown*)m_button_lookup_next ), 
		"Failed to configure menu button (lookup next)" ) ;
	COM_ENFORCE( ButtonGetEventImpl::DispEventAdvise( (IUnknown*)m_button_get ), 
		"Failed to configure menu button (get)" ) ;
	COM_ENFORCE( ButtonGetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_button_get_and_next ), 
		"Failed to configure menu button (get and next)" ) ;
	COM_ENFORCE( ButtonSetEventImpl::DispEventAdvise( (IUnknown*)m_button_set ), 
		"Failed to configure menu button (set)" ) ;
	COM_ENFORCE( ButtonSetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_button_set_and_next ), 
		"Failed to configure menu button (set and next)" ) ;
	COM_ENFORCE( ButtonGlossNEventImpl::DispEventAdvise( (IUnknown*)m_button_switch_modes ), 
		"Failed to configure menu button (gloss)" ) ;
	COM_ENFORCE( ButtonHelpEventImpl::DispEventAdvise( (IUnknown*)m_button_help ), 
		"Failed to configure menu button (help)" ) ;
}

