#include "stdafx.h"
#include "AddIn.h"
#include "Connect.h"
#include "MenuAdder.h"
#include "appstrings.h"

#include "properties.h"
#include "ErrorTranslator.h"
#include <exception>

#include "Hooker.h"

#include "excel_application.h"
#include <boost/algorithm/string.hpp>
#include "ClipboardBackup.h"
#include <boost/bind.hpp>
#include "logging.h"
#include "input_device_file.h"

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
	MessageBox(NULL, _T("An error occurred while performing an operation in the Felix interface\r\rIf the problem persists, please contact technical support."), _T("Felix Internal Error"), MB_OK ) ;  \
}

using namespace except ;

extern CAddInModule _AtlModule;

// 0x00000009
_ATL_FUNC_INFO SheetActivationInfo = 
{CC_STDCALL,VT_EMPTY,1,{VT_DISPATCH | VT_BYREF}} ;

_ATL_FUNC_INFO BeforeCloseInfo =
{CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH | VT_BYREF, VT_BYREF | VT_BOOL}};

CConnect::CConnect():
	m_keyboard_shortcuts(&m_mapper)
{
}

/*!
* \brief
* Takes the localized seed text, and creates a glossary menu item with shortcut.
* 
\param txt
The seed text

\param num
The glossary entry number

* The shortcut is the same as the entry number. The exception is entry 10, which has a shortcut of 0.
* 
* \param num - must be between 1 and 10.
*/
_bstr_t glossCap( _bstr_t txt, int num )
{
	CStringW tmp ;
	int altNum = num ;

	if ( altNum == 10 )
	{
		tmp.Format( L"%ls 1&0   (Alt+0)", (LPCWSTR)txt ) ;
		return _bstr_t( tmp ) ;
	}

	// else:
	tmp.Format( L"%ls &%d    (Alt+%d)", (LPCWSTR)txt, num, altNum ) ;
	return _bstr_t( tmp ) ;
}


/*!
* \brief
* Formats the description of the glossary item menu.
* 
* \param txt -- localized seed text
* \param num -- the glossary number. Must be between 1 and 10
* 
*/
_bstr_t glossDesc( _bstr_t txt, int num )
{
	CStringW tmp ;
	tmp.Format( L"%ls %i", (LPCWSTR)txt, num ) ;

	return _bstr_t( tmp ) ;
}

/*!
* \brief
* Excel will call this when loading the Addin.
* 
* \param pApplication - This is the Excel application. Grab it.
* 
* Lots of setup work done here.
- Exception handling
- menu
- installs keyboard hook
* 
*/
STDMETHODIMP CConnect::OnConnection(IDispatch *pApplication, 
									AddInDesignerObjects::ext_ConnectMode /*ConnectMode*/, 
									IDispatch *pAddInInst, 
									SAFEARRAY ** /*custom*/ )
{
	try
	{
		logging::log_debug("CConnect::OnConnection") ;
		try
		{
			set_up_exception_handling() ;
		}
		catch( ... )
		{
			logging::log_error("Unknown error setting up exception handling") ;
			MessageBox( NULL, _T("There was an error initializing ExcelAssist.\r\n")
				_T("(Failed to set up exception handling.)\r\n\r\n")
				_T("The Excel interface may not function properly.\r\nOnConnection"), _T("Initialization Error"), MB_OK ) ;
		}
		m_properties.read_from_registry() ;
		// pass IDispatch pointer to interface
		try
		{
			m_excelIF.set_app_ptr(pApplication) ;
		}
		catch (_com_error &e)
		{
			logging::log_error("Error setting application") ;
			CComException ce(e) ;
			logging::log_exception(ce) ;
		}
		// Back up clipboard info -- will restore on destruction
		CClipboardBackup backup ;

		m_pApplication = pApplication ;
		m_pAddInInstance = pAddInInst ;

		try
		{
			killOldMenu() ;
		}
		catch( ... )
		{
			logging::log_error("Unknown error removing old menu") ;
			MessageBox( NULL, _T("There was an error initializing ExcelAssist.\r\n")
				_T("(Failed to remove old menu items.)\r\n\r\n")
				_T("The Excel interface may not function properly.\r\nOnConnection"), _T("Initialization Error"), MB_OK ) ;
		}

		excel::app_ptr excel_app = excel::app_ptr( new excel::application ) ;
		try
		{
			excel_app->set_app( m_pApplication ) ; 
			m_excelIF.setApp( excel_app ) ;
		}
		catch (_com_error &e)
		{
			ATLTRACE("Error setting application\n") ;
			CComException ce(e) ;
			logging::log_error("Error setting application") ;
			logging::log_exception(ce) ;
		}
		m_excelIF.setProperties(&m_properties) ;

		MSOffice::_CommandBarsPtr spCmdBars = m_pApplication->GetCommandBars( ) ;
		ATLASSERT(spCmdBars);

		// attach old created
		_variant_t vName("Felix Interface");
		HRESULT hr = spCmdBars->get_Item(vName, &m_toolbar);

		if ( FAILED(hr) || ! m_toolbar || m_toolbar->Controls->Count != 8)
		{
			if (m_toolbar)
			{
				m_toolbar->Delete() ;
			}
			add_toolbar( spCmdBars ) ;
		}
		int i = 0 ;
		try
		{
			m_button_lookup = m_toolbar->Controls->Item[++i] ;
		}
		catch (_com_error &)
		{
			logging::log_warn("Toolbar items empty. Adding") ;
			add_toolbar_items(m_toolbar->Controls);
			m_button_lookup = m_toolbar->Controls->Item[i] ;
		}
		m_button_lookup_next  = m_toolbar->Controls->Item[++i] ;
		m_button_get		  = m_toolbar->Controls->Item[++i] ;
		m_button_get_and_next = m_toolbar->Controls->Item[++i] ;
		m_button_set		  = m_toolbar->Controls->Item[++i] ;
		m_button_set_and_next = m_toolbar->Controls->Item[++i] ;
		m_button_switch		  = m_toolbar->Controls->Item[++i] ;
		m_button_help		  = m_toolbar->Controls->Item[++i] ;

		if (is_2007_version())
		{
			m_toolbar->Visible = VARIANT_TRUE ;
		}

		advise_button_events();

		try
		{
			createMenu() ;
		}
		catch(except::CException &e)
		{
			CString message = _T("Failed to create menu") ;
			e.add_to_message(message) ;
			logging::log_error("Failed to create menu") ;
			throw CException(e) ;
		}
		catch( ... )
		{
			logging::log_error("Unknown error creating menu") ;
			MessageBox( NULL, _T("There was an error initializing ExcelAssist.\r\n")
				_T("(Failed to create Felix menu.)\r\n\r\n")
				_T("The Excel interface may not function properly.\r\nOnConnection"), _T("Initialization Error"), MB_OK ) ;
		}

		setMenuStrings( m_properties.get_preferred_gui_lang() ) ;

		input_device_ptr input(new InputDeviceFile) ;
		m_keyboard_shortcuts.load(get_shortcuts_text(SHORTCUTS_FILE, input)) ;
		m_mapper.m_target = &m_excelIF ;

		installhook( &m_keyboard_shortcuts ) ;

		m_keyboard_shortcuts.m_on_toggle_shortcuts = boost::bind(&CConnect::on_toggle_shortcuts, this, _1) ;
		logging::log_debug("connection complete") ;

	}
	catch(except::CException &e)
	{
		handle_base_exception(e, _T("Error initializing Felix addin for Excel. The Excel interface may not function properly.")) ;
	}
	catch(_com_error &com_err)
	{
		handle_exception(com_err, _T("A COM error occured while initialing the Felix addin.")) ;
	}
	catch( ... )
	{
		logging::log_error("An unknown exception occurred while initializing the Felix addin.") ;
		MessageBox( NULL, _T("There was an error initializing ExcelAssist.\r\nThe Excel interface may not function properly.\r\nOnConnection"), _T("Initialization Error"), MB_OK ) ;
	}
	return S_OK;
}

/*!
* Callback from Excel.
*/
STDMETHODIMP CConnect::OnDisconnection(AddInDesignerObjects::ext_DisconnectMode /*RemoveMode*/, SAFEARRAY ** /*custom*/ )
{
	try
	{
		logging::log_debug("CConnect::OnDisconnection") ;
		m_toolbar = NULL ;
		m_menu = NULL ;
		m_pApplication = NULL;
		m_pAddInInstance = NULL;
		logging::set_logger(logger_ptr()) ;
	}
	catch( _com_error &e ) 
	{
		logging::log_error("Error on OnDisconnection (_com_error)") ;
		CComException ce(e) ;
		logging::log_exception(ce) ;
	}
	catch( CException &myException )
	{
		logging::log_error("Error on OnDisconnection (CException)") ;
		logging::log_exception(myException) ;
	}
	catch ( std::exception &stdE )
	{
		logging::log_error("Error on OnDisconnection (std::exception)") ;
		logging::log_error(stdE.what()) ;
	}
	catch( CAtlException &atlE )
	{
		CComException msg(atlE.m_hr) ;
		logging::log_error("Error on OnDisconnection (CAtlException)") ;
		logging::log_exception(msg) ;
	}
	catch( ... )
	{
		logging::log_error("Unknown exception on OnDisconnection") ;
		BANNER("CConnect::OnDisconnection - other exception") ;
	}
	return S_OK;
}

/*!
* Callback from Excel.
*/
STDMETHODIMP CConnect::OnAddInsUpdate (SAFEARRAY ** /*custom*/ )
{
	return S_OK;
}

/*!
* Callback from Excel.
*/
STDMETHODIMP CConnect::OnStartupComplete (SAFEARRAY ** /*custom*/ )
{
	return S_OK;
}

/*!
* Respond to OnBeginShutdown call from Excel.
*/
STDMETHODIMP CConnect::OnBeginShutdown (SAFEARRAY ** /*custom*/ )
{
	try
	{
		logging::log_debug("CConnect::OnBeginShutdown") ;

		unadvise_app_methods();
		// toolbar buttons
		unadvise_toolbar_methods();
		// menu items
		unadvise_menu_methods();

		uninstallhook( &m_keyboard_shortcuts ) ;
		m_excelIF.shut_down() ;

		logging::log_debug("shutdown complete") ;
	}
	catch( _com_error &e ) 
	{
		logging::log_error("Error on OnBeginShutdown (_com_error)") ;
		CComException ce(e) ;
		logging::log_exception(ce) ;
	}
	catch( CException &myException )
	{
		logging::log_error("Error on OnBeginShutdown (CException)") ;
		logging::log_exception(myException) ;
	}
	catch ( std::exception &stdE )
	{
		logging::log_error("Error on OnBeginShutdown (std::exception)") ;
		logging::log_error(stdE.what()) ;
	}
	catch( CAtlException &atlE )
	{
		CComException msg(atlE.m_hr) ;
		logging::log_error("Error on OnBeginShutdown (CAtlException)") ;
		logging::log_exception(msg) ;
	}
	catch( ... )
	{
		logging::log_error("Unknown exception on OnBeginShutdown") ;
		BANNER("CConnect::OnBeginShutdown - other exception") ;
	}

	return S_OK;
}

void CConnect::on_toggle_shortcuts(BOOL shortcuts_enabled)
{
	try
	{
		m_properties.read_from_registry() ;
		m_properties.set_shortcuts_active(shortcuts_enabled) ;
		m_properties.write_to_registry() ;
		logging::log_debug("toggle shortcuts") ;

		m_menu->Caption = get_menu_caption().c_str() ;
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
/*!
* Puts up an error box if the HRESULT is failure.
*/
void CConnect::checkHR(HRESULT hr)
{
	if ( FAILED( hr ) ) 
	{
		const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM ;
		LPVOID buff(0)  ;

		if ( 0 != 
			::FormatMessage( flags, NULL, hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &buff,	0, NULL )
			)
		{
			if ( buff != NULL )
			{
				tstring _msg((LPCTSTR)buff) ;
				logging::log_error(string("COM Exception: ") + string2string(_msg, CP_UTF8)) ;
				MessageBox( NULL, (LPCTSTR) buff, _T("COM Error"), MB_OK ) ;
				::LocalFree( buff ) ;
			}
		}

	}
}

/*!
* Switches GUI languages.
*/
void CConnect::switchGuiLang(void)
{
	try
	{
		logging::log_debug("Switching GUI languages") ;
		m_properties.read_from_registry() ;

		if ( PREF_LANG_ENGLISH == m_properties.get_preferred_gui_lang() ) 
		{
			m_properties.set_preferred_gui_lang( PREF_LANG_JAPANESE ) ;
		}
		else
		{
			m_properties.set_preferred_gui_lang( PREF_LANG_ENGLISH ) ;
		}
		m_properties.write_to_registry() ;

		setMenuStrings( m_properties.get_preferred_gui_lang() ) ;
	}
	catch( _com_error &e ) 
	{
		handle_exception(e, _T("There was a COM error changing the Felix menu language.")) ;
	}
	catch( CException &myException )
	{
		handle_base_exception(myException, _T("There was an error changing the Felix menu language.")) ;
	}
	catch ( std::exception &stdE )
	{
		handle_exception(stdE, _T("There was a runtime error changing the Felix menu language.")) ;
	}
	catch( CAtlException &atlE )
	{
		handle_exception(atlE, _T("There was an internal COM error changing the Felix menu language.")) ;
	}
	catch( ... )
	{
		logging::log_error("Unknown error switching Felix GUI language") ;
		CString errMsg = _T("There was an error changing the Felix menu language.\r\nThe program may not function properly.") ;
		errMsg << _T("\r\nUnknown Error") ;
		errMsg << _T("The program may not function properly. Please restart Excel.\r\n") ;
		errMsg << _T("If the problem persists, contact technical support.\r\n\r\n") ;
		MessageBox( NULL, errMsg, _T("Initialization Error"), MB_OK ) ;
	}
}

/*!
* Deletes the old menu.
*/
void CConnect::killOldMenu(void)
{
	MSOffice::CommandBarControlsPtr controls = m_pApplication->GetCommandBars()->GetActiveMenuBar()->GetControls() ;

	wstring marked_for_death(L"felix") ;

	int menuCount = controls->Count ;
	for ( int i = 1 ; i < menuCount ; ) 
	{
		MSOffice::CommandBarControlPtr control = controls->Item[i] ;
		wstring candidate_for_death = (LPCWSTR)control->Caption ;
		boost::replace_all( candidate_for_death, L"&", L"" ) ;
		boost::to_lower(candidate_for_death) ;
		if ( candidate_for_death.find( L"felix" ) != wstring::npos  )
		{
			ATLTRACE( "YEEHAW!!\n" ) ;
			control->Delete() ;
			--menuCount ;
		}
		else
		{
			++i ;
		}
	}	
}

/*!
* Sets the menu strings (for localization).
*/
void CConnect::setMenuStrings(int lang_id)
{
	try
	{
		CAppStrings& appStr = app_str::get( ) ;
		appStr.set_trans_mode(m_excelIF.is_trans_mode()) ;
		appStr.load( lang_id == PREF_LANG_JAPANESE ? LANG_JAPANESE : LANG_ENGLISH) ;

		// captions
		m_menuAutoAddGloss->Caption =  appStr.get_add_gloss() ;
		m_menuAutoAddMem->Caption =  appStr.get_add_memory() ;
		m_menuLookup->Caption =  appStr.get_lookup() ;
		m_menuLookupNext->Caption =  appStr.get_lookup_next() ;
		m_menuSet->Caption =  appStr.get_set() ;
		m_menuSetAndNext->Caption =  appStr.get_set_and_next() ;
		m_menuGet->Caption =  appStr.get_get() ;
		m_menuGetAndNext->Caption =  appStr.get_get_and_next() ;
		m_menuSwitchLang->Caption =  appStr.get_switch_lang() ;

		m_menuAuto->Caption = appStr.get_string(L"auto_trans") ;

		m_menuAutoTransSel->Caption =  appStr.get_auto_trans_sel() ;
		m_menuAutoTransSheet->Caption =  appStr.get_auto_sheet() ;
		m_menuAutoTransWorkbook->Caption =  appStr.get_auto_workbook() ;

		m_menuGloss->Caption = appStr.get_string(L"gloss") ;

		_bstr_t seedText = appStr.get_string(L"entry") ;
		m_menuGloss1->Caption = glossCap( seedText, 1) ;
		m_menuGloss2->Caption = glossCap( seedText, 2)  ;
		m_menuGloss3->Caption = glossCap( seedText, 3) ;
		m_menuGloss4->Caption = glossCap( seedText, 4) ;
		m_menuGloss5->Caption = glossCap( seedText, 5) ;
		m_menuGloss6->Caption = glossCap( seedText, 6) ;
		m_menuGloss7->Caption = glossCap( seedText, 7) ;
		m_menuGloss8->Caption = glossCap( seedText, 8) ;
		m_menuGloss9->Caption = glossCap( seedText, 9) ;
		m_menuGloss10->Caption = glossCap( seedText, 10) ;

		m_menuNextTrans->Caption = appStr.get_string(L"next") ;
		m_menuPrevTrans->Caption = appStr.get_string(L"prev") ;
		m_menuPreferences->Caption = appStr.get_properties() ;

		// tooltips
		m_menuAutoAddGloss->TooltipText =  appStr.get_add_gloss_tt() ;
		m_menuAutoAddMem->TooltipText =  appStr.get_add_memory_tt() ;
		m_menuLookup->TooltipText =  appStr.get_lookup_tt() ;
		m_menuLookupNext->TooltipText =  appStr.get_lookup_next_tt() ;
		m_menuSet->TooltipText =  appStr.get_set_tt() ;
		m_menuSetAndNext->TooltipText =  appStr.get_set_and_next_tt() ;
		m_menuGet->TooltipText =  appStr.get_get_tt() ;
		m_menuGetAndNext->TooltipText =  appStr.get_get_and_next_tt() ;
		m_menuSwitchLang->TooltipText =  appStr.get_switch_lang_tt() ;

		m_menuAutoTransSel->TooltipText =  appStr.get_auto_trans_sel_tt() ;
		m_menuAutoTransSheet->TooltipText =  appStr.get_auto_sheet_tt() ;
		m_menuAutoTransWorkbook->TooltipText =  appStr.get_auto_workbook_tt() ;

		seedText = appStr.get_string(L"entry") ;
		m_menuGloss1->TooltipText = glossDesc( seedText, 1) ;
		m_menuGloss2->TooltipText = glossDesc( seedText, 2)  ;
		m_menuGloss3->TooltipText = glossDesc( seedText, 3) ;
		m_menuGloss4->TooltipText = glossDesc( seedText, 4) ;
		m_menuGloss5->TooltipText = glossDesc( seedText, 5) ;
		m_menuGloss6->TooltipText = glossDesc( seedText, 6) ;
		m_menuGloss7->TooltipText = glossDesc( seedText, 7) ;
		m_menuGloss8->TooltipText = glossDesc( seedText, 8) ;
		m_menuGloss9->TooltipText = glossDesc( seedText, 9) ;
		m_menuGloss10->TooltipText = glossDesc( seedText, 10) ;

		m_menuNextTrans->TooltipText = appStr.get_tt_string(L"next") ;
		m_menuPrevTrans->TooltipText = appStr.get_tt_string(L"prev") ;
		m_menuPreferences->TooltipText = appStr.get_properties_tt() ;

	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("Menu Setup Error"), MB_OK ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Initialization Error") ) ;
	}
	catch ( std::exception &stdE )
	{
		CStringA msg = stdE.what() ;
		MessageBoxA( NULL, msg, "C Runtime Error", MB_OK ) ;
	}
	catch( CAtlException &atlE )
	{
		checkHR( atlE.m_hr ) ;
	}
	catch( ... )
	{
		MessageBox( NULL, _T("There was an error changing the Felix menu language.\r\nThe program may not function properly."), _T("Initialization Error"), MB_OK ) ;
	}
}

/*!
* Creates the menu, adding the menu items.
*/
void CConnect::createMenu()
{
	m_menu = m_pApplication->GetCommandBars()->GetActiveMenuBar()->GetControls()->Add
		(
		_variant_t( MSOffice::msoControlPopup ), // type
		vtMissing,								// id
		vtMissing, 								// parameter
		vtMissing, 								// before
		_variant_t( VARIANT_TRUE )				// temporary
		) ;

	m_menu->Caption = get_menu_caption().c_str() ;

	CMenuAdder adder( m_menu ) ;

	CAppStrings& appStr = app_str::get( ) ;
	appStr.load( LANG_ENGLISH ) ;

	m_menuAutoAddGloss = adder.add_menu( appStr.get_add_gloss(), appStr.get_add_gloss_tt() ) ;
	MenuAutoAddGlossEventImpl::DispEventAdvise( (IUnknown*)m_menuAutoAddGloss );

	m_menuAutoAddMem = adder.add_menu( appStr.get_add_memory(), appStr.get_add_memory_tt() ) ;
	MenuAutoAddMemEventImpl::DispEventAdvise( (IUnknown*)m_menuAutoAddMem );

	m_menuLookup = adder.add_menu( appStr.get_lookup(), appStr.get_lookup_tt(), true ) ;
	MenuLookupEventImpl::DispEventAdvise( (IUnknown*)m_menuLookup );

	m_menuLookupNext = adder.add_menu( appStr.get_lookup_next(), appStr.get_lookup_next_tt() ) ;
	MenuLookupNextEventImpl::DispEventAdvise( (IUnknown*)m_menuLookupNext );

	m_menuSet = adder.add_menu( appStr.get_set(), appStr.get_set_tt(), true ) ;
	MenuSetEventImpl::DispEventAdvise( (IUnknown*)m_menuSet );

	m_menuSetAndNext = adder.add_menu( appStr.get_set_and_next(), appStr.get_set_and_next_tt() ) ;
	MenuSetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_menuSetAndNext );

	m_menuGet = adder.add_menu( appStr.get_get(), appStr.get_get_tt(), true ) ;
	MenuGetEventImpl::DispEventAdvise( (IUnknown*)m_menuGet );

	m_menuGetAndNext = adder.add_menu( appStr.get_get_and_next(), appStr.get_get_and_next_tt() ) ;
	MenuGetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_menuGetAndNext );

	m_menuNextTrans = adder.add_menu( appStr.get_string( L"next" ), appStr.get_tt_string( L"next" ), true ) ;
	MenuNextTrans::DispEventAdvise( (IUnknown*)m_menuNextTrans );

	m_menuPrevTrans = adder.add_menu(  appStr.get_string( L"prev" ), appStr.get_tt_string( L"prev" ) ) ;
	MenuPrevTrans::DispEventAdvise( (IUnknown*)m_menuPrevTrans );

	// ==============================
	// [1/7/2007 Ryan Ginstrom]
	// Glossary entry menus
	// ==============================

	m_menuGloss = adder.add_popup(  appStr.get_string( L"gloss" ), true ) ;

	CMenuAdder glossAdder( m_menuGloss ) ;

	_bstr_t entryText = appStr.get_string( L"entry" ) ;

	m_menuGloss1 = glossAdder.add_menu( glossCap(entryText, 1), glossDesc(entryText, 1) ) ;
	MenuGloss1::DispEventAdvise( (IUnknown*)m_menuGloss1 );

	m_menuGloss2 = glossAdder.add_menu( glossCap(entryText, 2), glossDesc(entryText, 2) ) ;
	MenuGloss2::DispEventAdvise( (IUnknown*)m_menuGloss2 );

	m_menuGloss3 = glossAdder.add_menu( glossCap(entryText, 3), glossDesc(entryText, 3) ) ;
	MenuGloss3::DispEventAdvise( (IUnknown*)m_menuGloss3 );

	m_menuGloss4 = glossAdder.add_menu( glossCap(entryText, 4), glossDesc(entryText, 4) ) ;
	MenuGloss4::DispEventAdvise( (IUnknown*)m_menuGloss4 );

	m_menuGloss5 = glossAdder.add_menu( glossCap(entryText, 5), glossDesc(entryText, 5) ) ;
	MenuGloss5::DispEventAdvise( (IUnknown*)m_menuGloss5 );

	m_menuGloss6 = glossAdder.add_menu( glossCap(entryText, 6), glossDesc(entryText, 6) ) ;
	MenuGloss6::DispEventAdvise( (IUnknown*)m_menuGloss6 );

	m_menuGloss7 = glossAdder.add_menu( glossCap(entryText, 7), glossDesc(entryText, 7) ) ;
	MenuGloss7::DispEventAdvise( (IUnknown*)m_menuGloss7 );

	m_menuGloss8 = glossAdder.add_menu( glossCap(entryText, 8), glossDesc(entryText, 8) ) ;
	MenuGloss8::DispEventAdvise( (IUnknown*)m_menuGloss8 );

	m_menuGloss9 = glossAdder.add_menu( glossCap(entryText, 9), glossDesc(entryText, 9) ) ;
	MenuGloss9::DispEventAdvise( (IUnknown*)m_menuGloss9 );

	m_menuGloss10 = glossAdder.add_menu( glossCap(entryText, 10), glossDesc(entryText, 10) ) ;
	MenuGloss10::DispEventAdvise( (IUnknown*)m_menuGloss10 );

	m_menuAuto = adder.add_popup(  appStr.get_string( L"auto_trans" ), true ) ;

	CMenuAdder autoAdder( m_menuAuto ) ;

	m_menuAutoTransSel = autoAdder.add_menu( appStr.get_auto_trans_sel(), appStr.get_auto_trans_sel_tt() ) ;
	MenuAutoTransSelEventImpl::DispEventAdvise( (IUnknown*)m_menuAutoTransSel );

	m_menuAutoTransSheet = autoAdder.add_menu( appStr.get_auto_sheet(), appStr.get_auto_sheet_tt() ) ;
	MenuAutoTransSheetEventImpl::DispEventAdvise( (IUnknown*)m_menuAutoTransSheet );

	m_menuAutoTransWorkbook = autoAdder.add_menu( appStr.get_auto_workbook(), appStr.get_auto_workbook_tt() ) ;
	MenuAutoTransWorkbookEventImpl::DispEventAdvise( (IUnknown*)m_menuAutoTransWorkbook );

	// =====================
	m_menuSwitchLang = adder.add_menu( appStr.get_switch_lang(), appStr.get_switch_lang_tt(), true ) ;
	MenuSwitchLangEventImpl::DispEventAdvise( (IUnknown*)m_menuSwitchLang );

	m_menuPreferences = adder.add_menu( appStr.get_properties(), appStr.get_properties_tt(), true ) ;
	MenuPreferencesEventImpl::DispEventAdvise( (IUnknown*)m_menuPreferences );

}

/*!
* Auto-adds a glossary.
*/
void __stdcall CConnect::OnAutoAddGloss( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;
		CancelDefault ;

		m_excelIF.OnAutoAddGloss() ;
	}
	CATCH_ALL(_T("OnAutoAddGloss")) ;

}	

void __stdcall CConnect::OnWorkbookBeforeClose( IDispatch * workbook, VARIANT_BOOL * )
{
	if(m_properties.m_use_trans_hist)
	{
		try
		{
			ATLTRACE("OnWorkbookBeforeClose\n") ;
			m_excelIF.close_workbook(workbook) ;
		}
		catch (_com_error& e)
		{
			logging::log_warn("Failed to handle workbook close event.") ;
			logging::log_exception(e) ;
		}
	}
}

wstring CConnect::get_menu_caption()
{
	wstring caption = L"Feli&x" ;

	if ( ! m_excelIF.is_trans_mode() ) 
	{
		caption += L" [*]" ;
	}
	if (! m_properties.get_shortcuts_active())
	{
		caption += L" [-]" ;
	}
	return caption ;
}

void CConnect::unadvise_menu_methods()
{
	MenuAutoAddGlossEventImpl::DispEventUnadvise( (IUnknown*)m_menuAutoAddGloss ) ;       
	MenuAutoAddMemEventImpl::DispEventUnadvise( (IUnknown*)m_menuAutoAddMem );
	MenuLookupEventImpl::DispEventUnadvise( (IUnknown*)m_menuLookup );
	MenuLookupNextEventImpl::DispEventUnadvise( (IUnknown*)m_menuLookupNext );
	MenuSetEventImpl::DispEventUnadvise( (IUnknown*)m_menuSet );
	MenuSetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_menuSetAndNext );
	MenuGetEventImpl::DispEventUnadvise( (IUnknown*)m_menuGet );
	MenuGetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_menuGetAndNext );
	MenuSwitchLangEventImpl::DispEventUnadvise( (IUnknown*)m_menuSwitchLang );

	MenuAutoTransSelEventImpl::DispEventUnadvise( (IUnknown*)m_menuAutoTransSel );
	MenuAutoTransSheetEventImpl::DispEventUnadvise( (IUnknown*)m_menuAutoTransSheet );
	MenuAutoTransWorkbookEventImpl::DispEventUnadvise( (IUnknown*)m_menuAutoTransWorkbook );

	MenuNextTrans::DispEventUnadvise( (IUnknown*)m_menuNextTrans );
	MenuPrevTrans::DispEventUnadvise( (IUnknown*)m_menuPrevTrans );

	MenuGloss1::DispEventUnadvise( (IUnknown*)m_menuGloss1 );
	MenuGloss2::DispEventUnadvise( (IUnknown*)m_menuGloss2 );
	MenuGloss3::DispEventUnadvise( (IUnknown*)m_menuGloss3 );
	MenuGloss4::DispEventUnadvise( (IUnknown*)m_menuGloss4 );
	MenuGloss5::DispEventUnadvise( (IUnknown*)m_menuGloss5 );
	MenuGloss6::DispEventUnadvise( (IUnknown*)m_menuGloss6 );
	MenuGloss7::DispEventUnadvise( (IUnknown*)m_menuGloss7 );
	MenuGloss8::DispEventUnadvise( (IUnknown*)m_menuGloss8 );
	MenuGloss9::DispEventUnadvise( (IUnknown*)m_menuGloss9 );
	MenuGloss10::DispEventUnadvise( (IUnknown*)m_menuGloss10 );
}


void CConnect::unadvise_toolbar_methods()
{
	ButtonGetEventImpl::DispEventUnadvise( (IUnknown*)m_button_get ) ;       
	ButtonSetEventImpl::DispEventUnadvise( (IUnknown*)m_button_set ) ;       
	ButtonGetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_button_get_and_next ) ;       
	ButtonSetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_button_set_and_next ) ;       
	ButtonLookupEventImpl::DispEventUnadvise( (IUnknown*)m_button_lookup ) ;       
	ButtonLookupNextEventImpl::DispEventUnadvise( (IUnknown*)m_button_lookup_next ) ;       
	ButtonSwitchEventImpl::DispEventUnadvise( (IUnknown*)m_button_switch ) ;       
	ButtonHelpEventImpl::DispEventUnadvise( (IUnknown*)m_button_help ) ;
}
void CConnect::unadvise_app_methods()
{
	COM_ENFORCE(ExcelSheetActivateImpl::DispEventUnadvise( (IUnknown*)m_pApplication ),
		"Failed to unhook sheet activation event");
	COM_ENFORCE(ExcelSheetDeactivateImpl::DispEventUnadvise( (IUnknown*)m_pApplication ),
		"Failed to unhook sheet deactivation event");
	COM_ENFORCE(ExcelWbBeforeCloseImpl::DispEventUnadvise( (IUnknown*)m_pApplication ),
		"Failed to hook workbook close event");
}

void CConnect::advise_button_events()
{
	COM_ENFORCE(ExcelSheetActivateImpl::DispEventAdvise( (IUnknown*)m_pApplication ),
		"Failed to hook sheet activation event");
	COM_ENFORCE(ExcelSheetDeactivateImpl::DispEventAdvise( (IUnknown*)m_pApplication ),
		"Failed to hook sheet deactivation event");
	COM_ENFORCE(ExcelWbBeforeCloseImpl::DispEventAdvise( (IUnknown*)m_pApplication ),
		"Failed to hook workbook close event");
	
	// connect to event source in OnConnection
	// m_spButton member variable is a smart pointer to _CommandBarButton
	// that is used to cache the pointer to the first toolbar button.
	COM_ENFORCE(ButtonLookupEventImpl::DispEventAdvise( (IUnknown*)m_button_lookup ),
		"Failed to configure button (lookup)");
	COM_ENFORCE(ButtonGetEventImpl::DispEventAdvise( (IUnknown*)m_button_get ),
		"Failed to configure button (get)");
	COM_ENFORCE(ButtonSetEventImpl::DispEventAdvise( (IUnknown*)m_button_set ),
		"Failed to configure button (set)");
	COM_ENFORCE(ButtonGetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_button_get_and_next ),
		"Failed to configure button (get and next)");
	COM_ENFORCE(ButtonSetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_button_set_and_next ),
		"Failed to configure button (set and next)");
	COM_ENFORCE(ButtonLookupNextEventImpl::DispEventAdvise( (IUnknown*)m_button_lookup_next ),
		"Failed to configure button (lookup next)");
	COM_ENFORCE(ButtonSwitchEventImpl::DispEventAdvise( (IUnknown*)m_button_switch ),
		"Failed to configure button (lookup next)");
	COM_ENFORCE(ButtonHelpEventImpl::DispEventAdvise ( (IUnknown*)m_button_help ),
		"Failed to configure button (help)");
}

void __stdcall CConnect::OnSwitchModes( IDispatch * /*Ctrl*/, VARIANT_BOOL * /*CancelDefault*/ )
{
	if (m_excelIF.is_trans_mode()) // go to review mode
	{
		m_excelIF.set_review_mode() ;
		set_tb_img_and_text( m_button_lookup, IDB_LOOKUP_TRANS ) ;
		set_tb_img_and_text( m_button_lookup_next, IDB_LOOKUP_NEXT_TRANS ) ;
		set_tb_img_and_text( m_button_get, IDB_RESTORE ) ;
		set_tb_img_and_text( m_button_get_and_next, IDB_RESTORE_AND_NEXT ) ;
		set_tb_img_and_text( m_button_set, IDB_CORRECT ) ;
		set_tb_img_and_text( m_button_set_and_next, IDB_CORRECT_AND_NEXT ) ;
		set_tb_img_and_text( m_button_switch, IDB_SWITCH_TO_TRANS ) ;
		set_tb_img_and_text( m_button_help, IDB_HELP ) ;
		m_menu->Caption = get_menu_caption().c_str() ;
		m_menuAutoTransSel->Enabled = VARIANT_FALSE ;
	}
	else // go to trans mode
	{
		m_excelIF.set_trans_mode() ;
		set_tb_img_and_text( m_button_lookup, IDB_LOOKUP ) ;
		set_tb_img_and_text( m_button_lookup_next, IDB_LOOKUP_NEXT ) ;
		set_tb_img_and_text( m_button_get, IDB_GET ) ;
		set_tb_img_and_text( m_button_get_and_next, IDB_GET_AND_NEXT ) ;
		set_tb_img_and_text( m_button_set, IDB_SET ) ;
		set_tb_img_and_text( m_button_set_and_next, IDB_SET_AND_NEXT ) ;
		set_tb_img_and_text( m_button_switch, IDB_SWITCH_TO_REVIEW ) ;
		set_tb_img_and_text( m_button_help, IDB_HELP ) ;
		m_menu->Caption = get_menu_caption().c_str() ;
		m_menuAutoTransSel->Enabled = VARIANT_TRUE ;
	}
	setMenuStrings(m_properties.get_preferred_gui_lang()) ;
}
void CConnect::set_tb_img_and_text( MSOffice::_CommandBarButtonPtr toolbarItem, int button_id )
{
	if ( ! load_picture( toolbarItem, button_id + 100 ) ) 
	{
		pastePicture(button_id, toolbarItem);
	}

	setToolbarItemText(button_id, toolbarItem);
}
/*!
* Auto-adds a memory.
*/
void __stdcall CConnect::OnAutoAddMem( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnAutoAddMem() ;
	}
	CATCH_ALL(_T("OnAutoAddMem")) ;
}

/*!
* Looks up the current cell.
*/
void __stdcall CConnect::OnLookup( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnLookup() ;
	}
	CATCH_ALL(_T("OnLookup")) ;
}

/*!
* Looks up the next cell.
*/
void __stdcall CConnect::OnLookupNext( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnLookupNext() ;
	}
	CATCH_ALL(_T("OnLookupNext")) ;
}

/*!
* Sets the translation.
*/
void __stdcall CConnect::OnSet( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnSet() ;
	}
	CATCH_ALL(_T("OnSet")) ;
}

/*!
* Sets the translation and looks up the next cell.
*/
void __stdcall CConnect::OnSetAndNext( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnSetAndNext() ;
	}
	CATCH_ALL(_T("OnSetAndNext")) ;
}

/*!
* Get the current translation.
*/
void __stdcall CConnect::OnGet( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnGet() ;
	}
	CATCH_ALL(_T("OnGet")) ;
}

/*!
* Get the translation and look up the next cell.
*/
void __stdcall CConnect::OnGetAndNext( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnGetAndNext() ;
	}
	CATCH_ALL(_T("OnGetAndNext")) ;
}

/*!
* Switch GUI languages.
*/
void __stdcall CConnect::OnSwitch( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		switchGuiLang() ;
	}
	CATCH_ALL(_T("OnSwitch")) ;
}

/*!
* Auto translate the selected range.
*/
void __stdcall CConnect::OnAutoTransSel( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnAutoTransSel() ;
	}
	CATCH_ALL(_T("OnAutoTransSel")) ;
}

/*!
* Auto translate the current worksheet.
*/
void __stdcall CConnect::OnAutoTransSheet( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnAutoTransSheet() ;
	}
	CATCH_ALL(_T("OnAutoTransSheet")) ;
}

/*!
* Auto translate the entire workbook.
*/
void __stdcall CConnect::OnAutoTransWorkbook( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnAutoTransWorkbook() ;
	}
	CATCH_ALL(_T("OnAutoTransWorkbook")) ;
}

/*!
* Navigates to the next translation.
*/
void __stdcall CConnect::OnNextTrans( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnNext() ;
	}
	CATCH_ALL(_T("OnNextTrans")) ;
}

/*!
* Navigates to the previous translation.
*/
void __stdcall CConnect::OnPrevTrans( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnPrev() ;
	}
	CATCH_ALL(_T("OnPrevTrans")) ;
}

/*!
* Gets glossary match #1.
*/
void __stdcall CConnect::OnGloss1( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 1 ) ;
	}
	CATCH_ALL(_T("OnGloss1")) ;
}

/*!
* Gets glossary match #2.
*/
void __stdcall CConnect::OnGloss2( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 2 ) ;
	}
	CATCH_ALL(_T("OnGloss2")) ;
}

/*!
* Gets glossary match #3.
*/
void __stdcall CConnect::OnGloss3( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 3 ) ;
	}
	CATCH_ALL(_T("OnGloss3")) ;
}

/*!
* Gets glossary match #4.
*/
void __stdcall CConnect::OnGloss4( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 4 ) ;
	}
	CATCH_ALL(_T("OnGloss4")) ;
}

/*!
* Gets glossary match #5.
*/
void __stdcall CConnect::OnGloss5( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 5 ) ;
	}
	CATCH_ALL(_T("OnGloss5")) ;
}

/*!
* Gets glossary match #6.
*/
void __stdcall CConnect::OnGloss6( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 6 ) ;
	}
	CATCH_ALL(_T("OnGloss6")) ;
}

/*!
* Gets glossary match #7.
*/
void __stdcall CConnect::OnGloss7( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 7 ) ;
	}
	CATCH_ALL(_T("OnGloss7")) ;
}

/*!
* Gets glossary match #8.
*/

void __stdcall CConnect::OnGloss8( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 8 ) ;
	}
	CATCH_ALL(_T("OnGloss8")) ;
}

/*!
* Gets glossary match #9.
*/
void __stdcall CConnect::OnGloss9( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 9 ) ;
	}
	CATCH_ALL(_T("OnGloss9")) ;
}

/*!
* Gets glossary match #10.
*/
void __stdcall CConnect::OnGloss10( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault )
{
	try
	{
		Ctrl ;		CancelDefault ;
		m_excelIF.OnEntry( 0 ) ;
	}
	CATCH_ALL(_T("OnGloss10")) ;
}

HRESULT CConnect::add_toolbar( MSOffice::_CommandBarsPtr &spCmdBars )
{
	BANNER( "CConnect::add_toolbar" ) ;

	// now we add a new toolband to PowerPoint
	// to which we'll add our buttons
	CComVariant vName("Felix Interface");

	// position it below all toolbands
	//MsoBarPosition::msoBarTop = 1
	CComVariant vPos(1); 

	CComVariant vTemp(VARIANT_FALSE); // menu is not temporary        
	//Add a new toolband through Add method
	// vMenuTemp holds an unspecified parameter
	m_toolbar = spCmdBars->Add( vName, vPos, vtMissing, vTemp) ;

	m_toolbar->Visible = VARIANT_TRUE ;

	//now get the toolband's CommandBarControls
	MSOffice::CommandBarControlsPtr spBarControls = m_toolbar->Controls;

	add_toolbar_items(spBarControls);

	return S_OK ;
}

MSOffice::_CommandBarButtonPtr CConnect::add_toolbar_item( MSOffice::CommandBarControlsPtr &controls, int button_id )
{
	// add button
	MSOffice::_CommandBarButtonPtr toolbarItem = addControl(controls) ;

	// set style before setting bitmap
	toolbarItem->Style = ( MSOffice::msoButtonIcon );

	set_tb_img_and_text(toolbarItem, button_id);

	toolbarItem->put_Enabled(VARIANT_TRUE);
	toolbarItem->put_Visible(VARIANT_TRUE); 

	return toolbarItem ;
}

void CConnect::add_toolbar_items( MSOffice::CommandBarControlsPtr spBarControls )
{
	// add buttons

	add_toolbar_item( spBarControls, IDB_LOOKUP ) ;
	add_toolbar_item( spBarControls, IDB_LOOKUP_NEXT ) ;
	add_toolbar_item( spBarControls, IDB_GET ) ;
	add_toolbar_item( spBarControls, IDB_GET_AND_NEXT ) ;
	add_toolbar_item( spBarControls, IDB_SET ) ;
	add_toolbar_item( spBarControls, IDB_SET_AND_NEXT ) ;
	add_toolbar_item( spBarControls, IDB_SWITCH_TO_REVIEW ) ;
	add_toolbar_item( spBarControls, IDB_HELP ) ;
}

MSOffice::_CommandBarButtonPtr CConnect::addControl( MSOffice::CommandBarControlsPtr& controls )
{
	//MsoControlType::msoControlButton = 1
	CComVariant vToolBarType(1);
	//show the toolbar?
	CComVariant vShow(VARIANT_TRUE);
	CComVariant vEmpty(DISP_E_PARAMNOTFOUND, VT_ERROR);            
	MSOffice::CommandBarControlPtr spNewBar = controls->Add(vToolBarType,vEmpty,vEmpty,vEmpty,vShow ); 

	MSOffice::_CommandBarButtonPtr toolbarItem = spNewBar->GetControl() ;

	return toolbarItem;
}

void CConnect::setToolbarItemText( int button_id, MSOffice::_CommandBarButtonPtr& toolbarItem )
{
	CComBSTR caption ;
	caption.LoadString( button_id ) ;

	toolbarItem->put_Caption( caption );
	toolbarItem->put_TooltipText(caption); 

	CComBSTR tag_name = caption ;
	tag_name += L"toolbar" ;
	toolbarItem->put_Tag(tag_name);
}

void CConnect::pastePicture( int button_id, MSOffice::_CommandBarButtonPtr& toolbarItem )
{
	HBITMAP hBmp  =(HBITMAP)::LoadImage(_AtlModule.GetResourceInstance(),
		MAKEINTRESOURCE(button_id),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS);

	// put bitmap into Clipboard
	putImageInClipboard(hBmp);

	HRESULT hr = toolbarItem->PasteFace() ;

	::DeleteObject(hBmp) ;

	purgeClipboard();

	if ( FAILED( hr ) ) 
	{
		throw CComException( _T("Failed to paste image into toolbar button"), hr, toolbarItem, _T(__FILE__), __LINE__ ) ;
	}
}

bool CConnect::load_picture( MSOffice::_CommandBarButtonPtr &button, int button_id )
{
	BANNER("CConnect::load_picture") ;
	try
	{
		HINSTANCE hInst = _AtlModule.GetResourceInstance() ;

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
		e ;
		TRACE( e.what() ) ;
		TRACE( e.description() ) ;

		return false ;

	}
	catch ( _com_error &e ) 
	{
		e ;
		TRACE( e.ErrorMessage() ) ;
		TRACE_HRESULT( e.Error() ) ;

		return false ;
	}

	return true ;
}

void CConnect::purgeClipboard()
{
	::OpenClipboard(NULL) ;
	::EmptyClipboard() ;
	::CloseClipboard() ;
}

void CConnect::putImageInClipboard( HBITMAP hBmp )
{
	::OpenClipboard(NULL) ;
	::EmptyClipboard() ;
	::SetClipboardData(CF_BITMAP, (HANDLE)hBmp) ;
	::CloseClipboard() ;
}

void __stdcall CConnect::OnPreferences( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		BANNER("OnMenuPreferences") ;
		m_properties.read_from_registry() ;
		int old_preferred_lang = m_properties.get_preferred_gui_lang() ;
		if ( m_properties.get_preferred_gui_lang() == PREF_LANG_ENGLISH )
		{
			CPropertiesDlgE props_dlg(m_properties) ;
			const INT_PTR result = props_dlg.DoModal( ) ;
			input_device_ptr input(new InputDeviceFile) ;
			m_keyboard_shortcuts.load(get_shortcuts_text(SHORTCUTS_FILE, input)) ;
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
			const INT_PTR result = props_dlg.DoModal( ) ;
			input_device_ptr input(new InputDeviceFile) ;
			m_keyboard_shortcuts.load(get_shortcuts_text(SHORTCUTS_FILE, input)) ;
			if ( result <= 0 || result == IDCANCEL ) 
			{
				ATLTRACE("User canceled.\n") ;
				return ;
			}

			ATLTRACE("User clicked 'OK'.\n") ;
			m_properties = props_dlg.get_properties() ;
		}

		if ( old_preferred_lang != m_properties.get_preferred_gui_lang() ) 
		{
			setMenuStrings( m_properties.get_preferred_gui_lang() ) ;
		}

		m_properties.write_to_registry() ;
	}
	CATCH_ALL(_T("OnPreferences")) ;

}

void __stdcall CConnect::OnHelp( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		file::CPath path ;
		path.GetModulePath( _AtlModule.GetResourceInstance() ) ;

		CString helpfile_name = _T("http://felix-cat.com/media/manuals/felix/6.html") ;

		HINSTANCE result = ::ShellExecute(
			::GetDesktopWindow(),	//	HWND hwnd, 
			_T("open"),		// LPCTSTR lpOperation,
			helpfile_name,	// LPCTSTR lpFile, 
			NULL,			// LPCTSTR lpParameters, 
			path.Path(),	// LPCTSTR lpDirectory,
			SW_SHOW			// INT nShowCmd
			);	

		ATLASSERT ( result != NULL ) ; 
		result ;
	}
	CATCH_ALL(_T("OnShowHelp")) ;
}