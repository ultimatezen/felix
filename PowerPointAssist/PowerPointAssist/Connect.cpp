// Connect.cpp : Implementation of CConnect
#include "stdafx.h"
#include "AddIn.h"
#include "Connect.h"
#include "Hooker.h"
#include "ErrorTranslator.h"
#include <exception>

#include "DispatchWrapper.h"

#include "atlstr.h"

#include "globalApp.h"
#include "ClipboardBackup.h"
#include "menu_manip.h"
#include "Properties.h"
#include <boost/bind.hpp>
#include "logging.h"

extern CAddInModule _AtlModule;
using namespace except ;

_ATL_FUNC_INFO OnClickButtonInfo =
{CC_STDCALL,VT_EMPTY,2,{VT_DISPATCH,VT_BYREF | VT_BOOL}};

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
	MessageBox(NULL, _T("An error occurred while initializing the Felix interface\r\rIf the problem persists, please contact technical support."), _T("Felix PowerPointAssist"), MB_OK ) ;  \
}


// CConnect
STDMETHODIMP CConnect::OnConnection(IDispatch *pApplication, AddInDesignerObjects::ext_ConnectMode /*ConnectMode*/, IDispatch *pAddInInst, SAFEARRAY ** /*custom*/ )
{
	try
	{
		logging::log_debug("PowerPoint Assist :: OnConnection") ;


		// Windows structured exceptions as C++ exceptions
		//set_up_exception_handling() ;
		m_pApplication = pApplication ;

		globalApp::set( pApplication ) ;

		ATLASSERT( NULL != m_pApplication ) ;

		m_pAddInInstance = pAddInInst ;

		m_properties.read_from_registry() ;

		// Back up clipboard info -- will restore on destruction
		const CClipboardBackup backup ;

		killOldMenu() ;
		m_interface.setApp( m_pApplication ) ;
		m_interface.setProperties(&m_properties) ;

		Office::_CommandBarsPtr spCmdBars = m_pApplication->GetCommandBars( ) ;
		ATLASSERT(spCmdBars);

		// attach old created
		init_toolbar(spCmdBars);


		logging::log_debug("adding menu") ;
		add_menu( spCmdBars ) ;

		init_properties();

		installhook( static_cast<CPowerPointInterface *>(&m_interface) ) ;

		set_shortcuts_callback(boost::bind(&CConnect::on_toggle_shortcuts, this, _1), 
			m_properties.get_shortcuts_active()) ;

	}
	CATCH_ALL(_T("Error connecting to PowerPoint")) ;

	return S_OK;
}
void CConnect::init_properties()
{
	logging::log_debug("initializing properties") ;

	m_properties.read_from_registry() ;

	if ( m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE )
	{
		gui_to_japanese() ;
	}
}

void CConnect::init_toolbar( Office::_CommandBarsPtr spCmdBars )
{
	logging::log_debug("initializing toolbar") ;

	const _variant_t vName("Felix Interface");
	HRESULT hr = spCmdBars->get_Item(vName, &m_toolbar);

	if ( FAILED(hr) || ! m_toolbar)
	{
		add_toolbar( spCmdBars ) ;
	}
	int i = 0 ;
	try
	{
		m_button_lookup		  = m_toolbar->Controls->Item[++i] ;
	}
	catch (_com_error &)
	{
		logging::log_debug("Failed to retrieve first toolbar item; adding") ;
		add_toolbar_items(m_toolbar->Controls);
		m_button_lookup = m_toolbar->Controls->Item[i] ;
	}
	m_button_lookup_next  = m_toolbar->Controls->Item[++i] ;
	m_button_get		  = m_toolbar->Controls->Item[++i] ;
	m_button_get_and_next = m_toolbar->Controls->Item[++i] ;
	m_button_set		  = m_toolbar->Controls->Item[++i] ;
	m_button_set_and_next = m_toolbar->Controls->Item[++i] ;
	m_button_gloss_n	  = m_toolbar->Controls->Item[++i] ;
	m_button_help		  = m_toolbar->Controls->Item[++i] ;


	logging::log_debug("advising toolbar items") ;

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
	COM_ENFORCE(ButtonGlossNEventImpl::DispEventAdvise( (IUnknown*)m_button_gloss_n ),
		"Failed to configure button (gloss)");
	COM_ENFORCE(ButtonHelpEventImpl::DispEventAdvise ( (IUnknown*)m_button_help ),
		"Failed to configure button (help)");

	logging::log_debug("Completed toolbar setup") ;

}

STDMETHODIMP CConnect::OnDisconnection(AddInDesignerObjects::ext_DisconnectMode /*RemoveMode*/, SAFEARRAY ** /*custom*/ )
{
	try
	{
		logging::log_debug("PowerPoint Assist :: OnDisconnection") ;
		m_pApplication = NULL;
		m_pAddInInstance = NULL;
		logging::set_logger(logger_ptr()) ;
	}
	catch( _com_error &e ) 
	{
		logging::log_error("CConnect::OnDisconnection - _com_error") ;
		CComException ce(e) ;
		logging::log_exception(ce) ;
	}
	catch( CException &myException )
	{
		myException ;
		logging::log_error("CConnect::OnDisconnection - CException") ;
		TRACE(myException.format_message_for_message_box()) ;
		logging::log_exception(myException) ;
	}
	catch ( std::exception &stdE )
	{
		logging::log_error("CConnect::OnDisconnection - std::exception") ;
		CStringA msg = stdE.what() ;
		TRACE(msg) ;
	}
	catch( CAtlException &atlE )
	{
		logging::log_error("CConnect::OnDisconnection - CAtlException") ;
		CComException msg(atlE.m_hr) ;
		TRACE(msg.format_message_for_message_box()) ;
		logging::log_exception(msg) ;
	}
	catch( ... )
	{
		logging::log_error("CConnect::OnDisconnection - other exception") ;
	}

	return S_OK;
}

STDMETHODIMP CConnect::OnAddInsUpdate (SAFEARRAY ** /*custom*/ )
{
	logging::log_debug("PowerPoint Assist :: OnAddInsUpdate") ;
	return S_OK;
}

STDMETHODIMP CConnect::OnStartupComplete (SAFEARRAY ** /*custom*/ )
{
	logging::log_debug("PowerPoint Assist :: OnStartupComplete") ;
	return S_OK;
}

STDMETHODIMP CConnect::OnBeginShutdown (SAFEARRAY ** /*custom*/ )
{
	try
	{
		logging::log_debug("PowerPoint Assist :: OnBeginShutdown") ;

		MenuAutoTransFuzzyEventImpl	::DispEventUnadvise( (IUnknown*)m_menu_auto_trans_fuzzy ) ;
		MenuGetEventImpl::DispEventUnadvise( (IUnknown*)m_menu_get );
		MenuSetEventImpl::DispEventUnadvise( (IUnknown*)m_menu_set );
		MenuGetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_menu_get_and_next );
		MenuSetAndNextEventImpl::DispEventUnadvise( (IUnknown*)m_menu_set_and_next );
		MenuLookupEventImpl::DispEventUnadvise( (IUnknown*)m_menu_lookup );
		MenuLookupNextEventImpl::DispEventUnadvise( (IUnknown*)m_menu_lookup_next );
		MenuExtendLookupEventImpl::DispEventUnadvise( (IUnknown*)m_menu_extend_lookup ) ;
		MenuConcordanceEventImpl::DispEventUnadvise( (IUnknown*)m_menu_concordance ) ;
		MenuRegisterGlossEventImpl::DispEventUnadvise( (IUnknown*)m_menu_register_gloss ) ;
		MenuDeleteEventImpl::DispEventUnadvise( (IUnknown*)m_menu_delete ) ;
		MenuNextEventImpl::DispEventUnadvise( (IUnknown*)m_menu_next ) ;
		MenuPrevEventImpl::DispEventUnadvise( (IUnknown*)m_menu_prev ) ;
		MenuEntry0EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_0 ) ;
		MenuEntry1EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_1 ) ;
		MenuEntry2EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_2 ) ;
		MenuEntry3EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_3 ) ;
		MenuEntry4EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_4 ) ;
		MenuEntry5EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_5 ) ;
		MenuEntry6EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_6 ) ;
		MenuEntry7EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_7 ) ;
		MenuEntry8EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_8 ) ;
		MenuEntry9EventImpl::DispEventUnadvise( (IUnknown*)m_menu_entry_9 ) ;
		MenuGlossNEventImpl::DispEventUnadvise( (IUnknown*)m_menu_gloss_n );
		MenuLookupTransEventImpl::DispEventUnadvise( (IUnknown*)m_menu_lookup_trans ) ;
		MenuLookupNextTransEventImpl::DispEventUnadvise( (IUnknown*)m_menu_lookup_next_trans ) ;
		MenuTransConcordanceEventImpl::DispEventUnadvise( (IUnknown*)m_menu_trans_concordance ) ;
		MenuCorrectTransEventImpl::DispEventUnadvise( (IUnknown*)m_menu_correct_trans ) ;
		MenuExtendTransLookupEventImpl::DispEventUnadvise( (IUnknown*)m_menu_extend_trans_lookup ) ;
		MenuSaveEventImpl::DispEventUnadvise( (IUnknown*)m_menu_save );
		MenuHelpEventImpl::DispEventUnadvise( (IUnknown*)m_menu_help );
		MenuGuiEventImpl::DispEventUnadvise( (IUnknown*)m_menu_gui ) ;
		MenuPreferencesEventImpl::DispEventUnadvise( (IUnknown*)m_menu_preferences ) ;

		logging::log_debug("PowerPoint Assist :: OnBeginShutdown complete") ;
	}
	catch( _com_error &e ) 
	{
		TRACE(e.ErrorMessage()) ;
		TRACE(e.Description()) ;
		CComException ce(e) ;
		logging::log_error("Failed to complete shutdown (_com_error)") ;
		logging::log_exception(ce) ;
	}
	catch( CException &myException )
	{
		TRACE(myException.format_message_for_message_box()) ;
		logging::log_error("Failed to complete shutdown (CException)") ;
		logging::log_exception(myException) ;
	}
	catch ( std::exception &stdE )
	{
		CStringA msg = stdE.what() ;
		TRACE(msg) ;
		logging::log_error("Failed to complete shutdown (std::exception)") ;
		logging::log_error((LPCSTR)msg) ;
	}
	catch( CAtlException &atlE )
	{
		CComException msg(atlE.m_hr) ;
		TRACE(msg.format_message_for_message_box()) ;
		logging::log_error("Failed to complete shutdown (CAtlException)") ;
		logging::log_exception(msg) ;
	}
	catch( ... )
	{
		logging::log_error("Failed to complete shutdown (unknown exception)") ;
	}
	return S_OK;
}
wstring CConnect::get_menu_caption()
{
	const wstring caption = L"Feli&x" ;

	if (! m_properties.get_shortcuts_active())
	{
		return caption + L" [-]" ;
	}
	return caption ;
}
void CConnect::on_toggle_shortcuts(BOOL shortcuts_enabled)
{
	try
	{
		m_properties.read_from_registry() ;
		m_properties.set_shortcuts_active(shortcuts_enabled) ;
		m_properties.write_to_registry() ;
		logging::log_debug("toggle shortcuts") ;

		m_felix_menu->Caption = get_menu_caption().c_str() ;
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
void CConnect::killOldMenu(void)
{
	logging::log_debug("Removing old menu") ;

	/*
	*	PowerPoint XP will throw a _com_error somewhere inside the loop (invalid pointer).
	*  We need to catch it here instead of propagating it, in order to avoid
	*  failing in our initialization code.
	*/
	try
	{
		Office::CommandBarControlsPtr controls = m_pApplication->GetCommandBars()->GetActiveMenuBar()->GetControls() ;

		int menuCount = controls->Count ;
		for ( int i = 1 ; i < menuCount ; ) 
		{
			Office::CommandBarControlPtr control = controls->Item[i] ;

			if ( item_needs_killing( control->Caption ) )
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
	catch ( _com_error &e ) 
	{
		CComException ce( e ) ;
		ce.add_to_message(_T("Failed to remove old menu")) ;
		logging::log_error("Failed to remove old menu") ;
		logging::log_exception(ce) ;
#ifdef _DEBUG
		ce.notify_user( _T("Failure in KillOldMenu") ) ;
#endif
		return ;
	}
}


// ===========================


// Function name	: CConnect::add_menu
// Description	    : 
// Return type		: void 
// Argument         : Office::_CommandBars> &spCmdBars
// TODO: Change pointers to built-in smart pointer types
HRESULT CConnect::add_menu(Office::_CommandBarsPtr &spCmdBars)
{
	try
	{
		logging::log_debug("CConnect::add_menu") ;

		m_felix_menu = spCmdBars->ActiveMenuBar->Controls->Add(
			_variant_t( Office::msoControlPopup ),	// type
			vtMissing,															// id
			vtMissing,															// parameter
			vtMissing,															// before
			_variant_t(VARIANT_TRUE)) ;						// temporary

		m_felix_menu->Caption = string2BSTR(get_menu_caption()) ;

		Office::CommandBarPopupPtr popup = m_felix_menu->Control ;
		popup->Visible = VARIANT_TRUE ;

		Office::CommandBarControlsPtr ta_menu_controls = popup->GetControls() ;

		CComBSTR caption, tooltip ;

		//	caption = L"&Auto Translate Selection" ;
		//	tooltip =  L"Auto translate current selection" ;
		//	m_menu_auto_trans = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		//	MenuAutoTransEventImpl		::DispEventAdvise( (IUnknown*)m_menu_auto_trans ) ;

		caption = L"Translate to Fu&zzy (ALT + Z)" ;
		tooltip = L"Auto translate until a fuzzy match is reached" ;
		m_menu_auto_trans_fuzzy = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuAutoTransFuzzyEventImpl	::DispEventAdvise( (IUnknown*)m_menu_auto_trans_fuzzy ) ;

		caption.LoadString( IDB_GET ) ;
		tooltip.LoadString( IDB_GET ) ;
		m_menu_get = add_menu_item( ta_menu_controls, IDB_GET, caption, tooltip ) ;
		//	m_menu_get->put_BeginGroup( VARIANT_TRUE ) ;
		MenuGetEventImpl::DispEventAdvise( (IUnknown*)m_menu_get );

		caption.LoadString( IDB_SET ) ;
		tooltip.LoadString( IDB_SET ) ;
		m_menu_set= add_menu_item( ta_menu_controls, IDB_SET, caption, tooltip  ) ;
		MenuSetEventImpl::DispEventAdvise( (IUnknown*)m_menu_set );

		caption.LoadString( IDB_GET_AND_NEXT ) ;
		tooltip.LoadString( IDB_GET_AND_NEXT ) ;
		m_menu_get_and_next= add_menu_item( ta_menu_controls, IDB_GET_AND_NEXT, caption, tooltip  ) ;
		m_menu_set->put_BeginGroup( VARIANT_TRUE ) ;
		MenuGetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_menu_get_and_next );

		caption.LoadString( IDB_SET_AND_NEXT ) ;
		tooltip.LoadString( IDB_SET_AND_NEXT ) ;
		m_menu_set_and_next = add_menu_item( ta_menu_controls, IDB_SET_AND_NEXT, caption, tooltip  ) ;
		MenuSetAndNextEventImpl::DispEventAdvise( (IUnknown*)m_menu_set_and_next );

		caption.LoadString( IDB_LOOKUP ) ;
		tooltip.LoadString( IDB_LOOKUP ) ;
		m_menu_lookup = add_menu_item( ta_menu_controls, IDB_LOOKUP, caption, tooltip  ) ;
		m_menu_lookup->put_BeginGroup( VARIANT_TRUE ) ;
		MenuLookupEventImpl::DispEventAdvise( (IUnknown*)m_menu_lookup );

		caption.LoadString( IDB_LOOKUP_NEXT ) ;
		tooltip.LoadString( IDB_LOOKUP_NEXT ) ;
		m_menu_lookup_next = add_menu_item( ta_menu_controls, IDB_LOOKUP_NEXT, caption, tooltip  ) ;
		MenuLookupNextEventImpl::DispEventAdvise( (IUnknown*)m_menu_lookup_next );

		caption = L"&Extend Lookup Sentence (CTL + R ARR)" ;
		tooltip = L"Extend the current translation lookup" ;
		m_menu_extend_lookup = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuExtendLookupEventImpl	::DispEventAdvise( (IUnknown*)m_menu_extend_lookup ) ;

		caption = L"Find C&oncordance (ALT + C)" ;
		tooltip = L"Find concordance for the current selection in the memory" ;
		m_menu_concordance = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuConcordanceEventImpl	::DispEventAdvise( (IUnknown*)m_menu_concordance ) ;

		caption = L"Add Glossary En&try" ;
		tooltip = L"Add a glossary entry with the selection as the source" ;
		m_menu_register_gloss = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		m_menu_register_gloss->put_BeginGroup( VARIANT_TRUE ) ;
		MenuRegisterGlossEventImpl	::DispEventAdvise( (IUnknown*)m_menu_register_gloss ) ;


		caption = L"&Delete Translation (ALT + D)" ;
		tooltip = L"Delete the currently displayed translation" ;
		m_menu_delete = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuDeleteEventImpl			::DispEventAdvise( (IUnknown*)m_menu_delete ) ;

		caption = L"&Next Translation (ALT + N)" ;
		tooltip = L"Show the next translation in memory, if any" ;
		m_menu_next = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuNextEventImpl			::DispEventAdvise( (IUnknown*)m_menu_next ) ;

		caption = L"&Previous Translation (ALT + P)" ;
		tooltip = L"Show the previous translation in memory, if any" ;
		m_menu_prev = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuPrevEventImpl			::DispEventAdvise( (IUnknown*)m_menu_prev ) ;

		COM_ENFORCE( ta_menu_controls->raw_Add(
			CComVariant( Office::msoControlPopup ),
			vtMissing,
			vtMissing,
			vtMissing,
			CComVariant( VARIANT_TRUE ),
			&m_gloss_menu_popup
			),
			_T("Failed to get controls for menu item") 
			) ;

		m_gloss_menu_popup->Caption = L"Glossar&y..." ;
		m_gloss_menu_popup->put_BeginGroup( VARIANT_TRUE ) ;

		Office::CommandBarPopupPtr spGlossPopup = m_gloss_menu_popup->Control ;

		spGlossPopup->Visible = VARIANT_TRUE  ;

		Office::CommandBarControlsPtr spGlossMenuControls;
		COM_ENFORCE( spGlossPopup->get_Controls(&spGlossMenuControls), _T("Failed to get menu bar controls") ) ;

		caption = L"Entry &0 (ALT + 0)" ;
		tooltip = L"Get entry 0" ;
		m_menu_entry_0 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry0EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_0 ) ;

		caption = L"Entry &1 (ALT + 1)" ;
		tooltip = L"Get entry " ;
		m_menu_entry_1 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry1EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_1 ) ;

		caption = L"Entry &2 (ALT + 2)" ;
		tooltip = L"Get entry " ;
		m_menu_entry_2 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry2EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_2 ) ;

		caption = L"Entry &3 (ALT + 3)" ;
		tooltip = L"Get entry " ;
		m_menu_entry_3 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry3EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_3 ) ;

		caption = L"Entry &4 (ALT + 4)" ;
		tooltip = L"Get entry " ;
		m_menu_entry_4 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry4EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_4 ) ;

		caption = L"Entry &5 (ALT + 5)" ;
		tooltip = L"Get entry " ;
		m_menu_entry_5 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry5EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_5 ) ;

		caption = L"Entry &6 (ALT + 6)" ;
		tooltip = L"Get entry " ;
		m_menu_entry_6 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry6EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_6 ) ;

		caption = L"Entry &7 (ALT + 7)" ;
		tooltip = L"Get entry " ;
		m_menu_entry_7 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry7EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_7 ) ;

		caption = L"Entry &8 (ALT + 8)" ;
		tooltip = L"Get entry " ;
		m_menu_entry_8 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry8EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_8 ) ;

		caption = L"Entry &9 (ALT + 9)" ;
		tooltip = L"Get entry " ;
		m_menu_entry_9 = add_menu_item( spGlossMenuControls, 0, caption, tooltip  ) ;
		MenuEntry9EventImpl::DispEventAdvise( (IUnknown*)m_menu_entry_9 ) ;

		caption.LoadString( IDB_GLOSS_N ) ;
		tooltip.LoadString( IDB_GLOSS_N ) ;
		m_menu_gloss_n = add_menu_item( spGlossMenuControls, IDB_GLOSS_N, caption, tooltip  ) ;
		MenuGlossNEventImpl::DispEventAdvise( (IUnknown*)m_menu_gloss_n );

		caption = L"&Find Current Translation (CTL + ALT + L)" ;
		tooltip = L"Look up current translation" ;
		m_menu_lookup_trans = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		m_menu_lookup_trans->put_BeginGroup( VARIANT_TRUE ) ;
		MenuLookupTransEventImpl::DispEventAdvise( (IUnknown*)m_menu_lookup_trans ) ;

		caption = L"Look up Ne&xt Translation (CTL + ALT + R ARR)" ;
		tooltip = L"Select and look up next translation" ;
		m_menu_lookup_next_trans = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuLookupNextTransEventImpl::DispEventAdvise( (IUnknown*)m_menu_lookup_next_trans ) ;

		caption = L"Translat&ion Concordance (CTL + ALT + C)" ;
		tooltip = L"Find concordance in translations for current selection" ;
		m_menu_trans_concordance = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuTransConcordanceEventImpl::DispEventAdvise( (IUnknown*)m_menu_trans_concordance ) ;

		caption = L"Correct Translation (&B) (CTL + ALT + UP ARR)" ;
		tooltip = L"Correct current translation" ;
		m_menu_correct_trans = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuCorrectTransEventImpl::DispEventAdvise( (IUnknown*)m_menu_correct_trans ) ;

		caption = L"Extend Translation Loo&kup Sentence (CTL + ALT + E)" ;
		tooltip = L"Extend the translation lookup sentence" ;
		m_menu_extend_trans_lookup = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuExtendTransLookupEventImpl::DispEventAdvise( (IUnknown*)m_menu_extend_trans_lookup ) ;

		caption.LoadString( IDB_SAVE ) ;
		tooltip.LoadString( IDB_SAVE ) ;
		m_menu_save = add_menu_item( ta_menu_controls, IDB_SAVE, caption, tooltip  )	;
		m_menu_save->put_BeginGroup( VARIANT_TRUE ) ;
		MenuSaveEventImpl::DispEventAdvise( (IUnknown*)m_menu_save );

		//	caption.LoadString( IDB_ALIGN ) ;
		//	tooltip.LoadString( IDB_ALIGN ) ;
		//	m_menu_align = add_menu_item( ta_menu_controls, IDB_ALIGN, caption, tooltip  )	;
		//	MenuAlignEventImpl::DispEventAdvise( (IUnknown*)m_menu_align );

		caption.LoadString( IDB_HELP ) ;
		tooltip.LoadString( IDB_HELP ) ;
		m_menu_help = add_menu_item( ta_menu_controls, IDB_HELP, caption, tooltip  )	;
		MenuHelpEventImpl::DispEventAdvise( (IUnknown*)m_menu_help );

		caption = L"S&with Menu to Japanese" ;
		tooltip = L"Show Japanese GUI" ;
		m_menu_gui = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuGuiEventImpl				::DispEventAdvise( (IUnknown*)m_menu_gui ) ;

		caption.LoadString( IDS_USER_PREFERENCES ) ;
		tooltip.LoadString( IDS_USER_PREFERENCES ) ;
		m_menu_preferences = add_menu_item( ta_menu_controls, 0, caption, tooltip  ) ;
		MenuPreferencesEventImpl::DispEventAdvise( (IUnknown*)m_menu_preferences ) ;


	}
	catch (_com_error& e)
	{
		CComException com_e(e) ;
		com_e.add_to_message(_T("Error adding menu items")) ;
		throw com_e ;
	}

	return S_OK ;
}


// Function name	: CConnect::add_menu_item
// Description	    : 
// Return type		: command_button_ptr 
// Argument         : Office::CommandBarControlsPtr &controls
// Argument         : int button_id
Office::_CommandBarButtonPtr CConnect::add_menu_item( Office::CommandBarControlsPtr &controls, int button_id, const CComBSTR caption, const CComBSTR tooltip )
{

	Office::CommandBarControlPtr spMenuItemCtrl = controls->Add(
		CComVariant( Office::msoControlButton ),
		vtMissing,
		vtMissing,
		vtMissing,
		CComVariant( VARIANT_FALSE )
		) ;

	Office::_CommandBarButtonPtr menu_item = spMenuItemCtrl->GetControl() ;

	// to set a bitmap to a button, load a 32x32 bitmap
	// and copy it to clipboard. Call CommandBarButton's PasteFace()
	// to copy the bitmap to the button face. to use
	// PowerPoint's set of predefined bitmap, set button's FaceId to     //the
	// button whose bitmap you want to use
	if ( button_id != 0 )
	{
		try
		{
			if ( ! load_picture( menu_item, button_id + 100 ) ) 
			{
				HBITMAP hBmp  =(HBITMAP)::LoadImage(_AtlModule.GetResourceInstance(),
					MAKEINTRESOURCE(button_id),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS);

				if ( hBmp )
				{
					// put bitmap into Clipboard
					::OpenClipboard(NULL) ;
					::EmptyClipboard() ;
					::SetClipboardData(CF_BITMAP, (HANDLE)hBmp) ;
					::CloseClipboard() ;

					HRESULT hr = menu_item->PasteFace() ;

					::DeleteObject(hBmp) ;
					::OpenClipboard(NULL) ;
					::EmptyClipboard() ;
					::CloseClipboard() ;

					if ( FAILED( hr ) ) 
					{
						throw CComException( _T("Failed to paste image into toolbar button"), hr, menu_item, _T(__FILE__), __LINE__ ) ;
					}		
					menu_item->put_Style( Office::msoButtonIconAndCaption );
				}			
				else
				{
					menu_item->put_Style( Office::msoButtonCaption ) ;
				}
			}
			else
			{
				menu_item->put_Style( Office::msoButtonIconAndCaption );
			}
		}
		catch (std::exception& e)
		{
			logging::log_error("Failed to set image for menu item (std error): " + int2string(button_id)) ;
			logging::log_error(e.what()) ;
		}
		catch (CException& e)
		{
			logging::log_error("Failed to set image for menu item: " + int2string(button_id)) ;
			logging::log_exception(e) ;
		}
		catch (_com_error& e)
		{
			logging::log_error("Failed to set image for menu item (_com_error): " + int2string(button_id)) ;
			CComException ce(e) ;
			logging::log_exception(ce) ;
		}
	}

	menu_item->put_Caption( caption ) ;
	menu_item->put_TooltipText( tooltip ) ;
	CComBSTR tag_name = caption ;
	tag_name += L"menu" ;
	menu_item->put_Tag( tag_name ) ;

	// show the menu 

	menu_item->put_Visible(VARIANT_TRUE); 

	return menu_item ;
}


// Function name	: CConnect::add_toolbar
// Description	    : 
// Return type		: void 
// Argument         : Office::_CommandBars> &spCmdBars
HRESULT CConnect::add_toolbar( Office::_CommandBarsPtr &spCmdBars )
{
	logging::log_debug( "CConnect::add_toolbar" ) ;

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
	Office::CommandBarControlsPtr spBarControls = m_toolbar->Controls;

	add_toolbar_items(spBarControls);

	return S_OK ;
}


// Function name	: CConnect::add_toolbar_item
// Description	    : 
// Return type		: command_button_ptr 
// Argument         : Office::CommandBarControlsPtr &controls
// Argument         : int button_id
Office::_CommandBarButtonPtr CConnect::add_toolbar_item(Office::CommandBarControlsPtr &controls, int button_id )
{

	// add button
	Office::_CommandBarButtonPtr toolbarItem = addControl(controls) ;

	if ( ! load_picture( toolbarItem, button_id + 100 ) ) 
	{
		pastePicture(button_id, toolbarItem);
	}

	// set style before setting bitmap
	toolbarItem->Style = ( Office::msoButtonIcon );

	setToolbarItemText(button_id, toolbarItem);

	toolbarItem->put_Enabled(VARIANT_TRUE);
	toolbarItem->put_Visible(VARIANT_TRUE); 

	return toolbarItem ;
}

void CConnect::setToolbarItemText(int button_id, Office::_CommandBarButtonPtr& toolbarItem)
{
	CComBSTR caption ;
	caption.LoadString( button_id ) ;

	toolbarItem->put_Caption( caption );
	toolbarItem->put_TooltipText(caption); 

	CComBSTR tag_name = caption ;
	tag_name += L"toolbar" ;
	toolbarItem->put_Tag(tag_name); 

}

void CConnect::pastePicture(int button_id, Office::_CommandBarButtonPtr& toolbarItem)
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

Office::_CommandBarButtonPtr CConnect::addControl(Office::CommandBarControlsPtr& controls)
{
	//MsoControlType::msoControlButton = 1
	CComVariant vToolBarType(1);
	//show the toolbar?
	CComVariant vShow(VARIANT_TRUE);
	CComVariant vEmpty(DISP_E_PARAMNOTFOUND, VT_ERROR);            
	Office::CommandBarControlPtr spNewBar = controls->Add(vToolBarType,vEmpty,vEmpty,vEmpty,vShow ); 

	Office::_CommandBarButtonPtr toolbarItem = spNewBar->GetControl() ;

	return toolbarItem;
}

void CConnect::purgeClipboard()
{
	::OpenClipboard(NULL) ;
	::EmptyClipboard() ;
	::CloseClipboard() ;

}

void CConnect::putImageInClipboard(HBITMAP hBmp)
{
	::OpenClipboard(NULL) ;
	::EmptyClipboard() ;
	::SetClipboardData(CF_BITMAP, (HANDLE)hBmp) ;
	::CloseClipboard() ;
}



void CConnect::gui_to_english()
{
	logging::log_debug("CConnect::gui_to_english") ;
	//	caption = L"&Auto Translate Selection" ;
	//	tooltip =  L"Auto translate current selection" ;
	//	m_menu_auto_trans->put_Caption( caption ) ;
	//	m_menu_auto_trans->put_TooltipText( tooltip ) ;
	CComBSTR caption, tooltip ;

	caption = L"Translate to Fu&zzy (ALT + Z)" ;
	tooltip = L"Auto translate until a fuzzy match is reached" ;
	m_menu_auto_trans_fuzzy->put_Caption( caption ) ;
	m_menu_auto_trans_fuzzy->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_GET ) ;
	tooltip.LoadString( IDB_GET ) ;
	m_menu_get->put_Caption( caption ) ;
	m_menu_get->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_SET ) ;
	tooltip.LoadString( IDB_SET ) ;
	m_menu_get_and_next->put_Caption( caption ) ;
	m_menu_get_and_next->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_GET_AND_NEXT ) ;
	tooltip.LoadString( IDB_GET_AND_NEXT ) ;
	m_menu_set->put_Caption( caption ) ;
	m_menu_set->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_SET_AND_NEXT ) ;
	tooltip.LoadString( IDB_SET_AND_NEXT ) ;
	m_menu_set_and_next->put_Caption( caption ) ;
	m_menu_set_and_next->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_LOOKUP ) ;
	tooltip.LoadString( IDB_LOOKUP ) ;
	m_menu_lookup->put_Caption( caption ) ;
	m_menu_lookup->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_LOOKUP_NEXT ) ;
	tooltip.LoadString( IDB_LOOKUP_NEXT ) ;
	m_menu_lookup_next->put_Caption( caption ) ;
	m_menu_lookup_next->put_TooltipText( tooltip ) ;

	caption = L"&Extend Lookup Sentence (CTL + R ARR)" ;
	tooltip = L"Extend the current translation lookup" ;
	m_menu_extend_lookup->put_Caption( caption ) ;
	m_menu_extend_lookup->put_TooltipText( tooltip ) ;

	caption = L"Find C&oncordance (ALT + C)" ;
	tooltip = L"Find concordance for the current selection in the memory" ;
	m_menu_concordance->put_Caption( caption ) ;
	m_menu_concordance->put_TooltipText( tooltip ) ;

	caption = L"Add Glossary En&try" ;
	tooltip = L"Add a glossary entry with the selection as the source" ;
	m_menu_register_gloss->put_Caption( caption ) ;
	m_menu_register_gloss->put_TooltipText( tooltip ) ;

	caption = L"&Delete Translation (ALT + D)" ;
	tooltip = L"Delete the currently displayed translation" ;
	m_menu_delete->put_Caption( caption ) ;
	m_menu_delete->put_TooltipText( tooltip ) ;

	caption = L"&Next Translation (ALT + N)" ;
	tooltip = L"Show the next translation in memory, if any" ;
	m_menu_next->put_Caption( caption ) ;
	m_menu_next->put_TooltipText( tooltip ) ;

	caption = L"&Previous Translation (ALT + P)" ;
	tooltip = L"Show the previous translation in memory, if any" ;
	m_menu_prev->put_Caption( caption ) ;
	m_menu_prev->put_TooltipText( tooltip ) ;

	m_gloss_menu_popup->put_Caption( CComBSTR( L"Glossar&y..." ) ) ;

	caption = L"Entry &0 (ALT + 0)" ;
	tooltip = L"Get entry 0" ;
	m_menu_entry_0->put_Caption( caption ) ;
	m_menu_entry_0->put_TooltipText( tooltip ) ;

	caption = L"Entry &1 (ALT + 1)" ;
	tooltip = L"Get entry " ;
	m_menu_entry_1->put_Caption( caption ) ;
	m_menu_entry_1->put_TooltipText( tooltip ) ;

	caption = L"Entry &2 (ALT + 2)" ;
	tooltip = L"Get entry " ;
	m_menu_entry_2->put_Caption( caption ) ;
	m_menu_entry_2->put_TooltipText( tooltip ) ;

	caption = L"Entry &3 (ALT + 3)" ;
	tooltip = L"Get entry " ;
	m_menu_entry_3->put_Caption( caption ) ;
	m_menu_entry_3->put_TooltipText( tooltip ) ;

	caption = L"Entry &4 (ALT + 4)" ;
	tooltip = L"Get entry " ;
	m_menu_entry_4->put_Caption( caption ) ;
	m_menu_entry_4->put_TooltipText( tooltip ) ;

	caption = L"Entry &5 (ALT + 5)" ;
	tooltip = L"Get entry " ;
	m_menu_entry_5->put_Caption( caption ) ;
	m_menu_entry_5->put_TooltipText( tooltip ) ;

	caption = L"Entry &6 (ALT + 6)" ;
	tooltip = L"Get entry " ;
	m_menu_entry_6->put_Caption( caption ) ;
	m_menu_entry_6->put_TooltipText( tooltip ) ;

	caption = L"Entry &7 (ALT + 7)" ;
	tooltip = L"Get entry " ;
	m_menu_entry_7->put_Caption( caption ) ;
	m_menu_entry_7->put_TooltipText( tooltip ) ;

	caption = L"Entry &8 (ALT + 8)" ;
	tooltip = L"Get entry " ;
	m_menu_entry_8->put_Caption( caption ) ;
	m_menu_entry_8->put_TooltipText( tooltip ) ;

	caption = L"Entry &9 (ALT + 9)" ;
	tooltip = L"Get entry " ;
	m_menu_entry_9->put_Caption( caption ) ;
	m_menu_entry_9->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_GLOSS_N ) ;
	tooltip.LoadString( IDB_GLOSS_N ) ;
	m_menu_gloss_n->put_Caption( caption ) ;
	m_menu_gloss_n->put_TooltipText( tooltip ) ;

	caption = L"&Find Current Translation (CTL + ALT + L)" ;
	tooltip = L"Look up current translation" ;
	m_menu_lookup_trans->put_Caption( caption ) ;
	m_menu_lookup_trans->put_TooltipText( tooltip ) ;

	caption = L"Lookkup Ne&xt Translation (CTL + ALT + R ARR)" ;
	tooltip = L"Select and look up next translation" ;
	m_menu_lookup_next_trans->put_Caption( caption ) ;
	m_menu_lookup_next_trans->put_TooltipText( tooltip ) ;

	caption = L"Translat&ion Concordance (CTL + ALT + C)" ;
	tooltip = L"Find concordance in translations for current selection" ;
	m_menu_trans_concordance->put_Caption( caption ) ;
	m_menu_trans_concordance->put_TooltipText( tooltip ) ;

	caption = L"Correct Translation (&B) (CTL + ALT + UP ARR)" ;
	tooltip = L"Correct current translation" ;
	m_menu_correct_trans->put_Caption( caption ) ;
	m_menu_correct_trans->put_TooltipText( tooltip ) ;

	caption = L"Extend Translation Loo&kup Sentence (CTL + ALT + E)" ;
	tooltip = L"Extend the translation lookup sentence" ;
	m_menu_extend_trans_lookup->put_Caption( caption ) ;
	m_menu_extend_trans_lookup->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_SAVE ) ;
	tooltip.LoadString( IDB_SAVE ) ;
	m_menu_save->put_Caption( caption ) ;
	m_menu_save->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_HELP ) ;
	tooltip.LoadString( IDB_HELP ) ;
	m_menu_help->put_Caption( caption ) ;
	m_menu_help->put_TooltipText( tooltip ) ;

	caption = L"S&with Menu to Japanese" ;
	tooltip = L"Show Japanese GUI" ;
	m_menu_gui->put_Caption( caption ) ;
	m_menu_gui->put_TooltipText( tooltip ) ;

	caption.LoadString( IDB_GET ) ;
	tooltip.LoadString( IDB_GET ) ;
	m_button_get->put_Caption( caption ) ;
	m_button_get->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_USER_PREFERENCES ) ;
	tooltip.LoadString( IDS_USER_PREFERENCES ) ;
	m_menu_preferences->put_Caption( caption ) ;
	m_menu_preferences->put_TooltipText( tooltip ) ;


	caption.LoadString( IDB_SET ) ;
	tooltip.LoadString( IDB_SET ) ;
	m_button_set->put_TooltipText( tooltip ) ;
	m_button_set->put_Tag( caption ) ;

	caption.LoadString( IDB_GET_AND_NEXT ) ;
	tooltip.LoadString( IDB_GET_AND_NEXT ) ;
	m_button_get_and_next->put_TooltipText( tooltip ) ;
	m_button_get_and_next->put_Tag( caption ) ;

	caption.LoadString( IDB_SET_AND_NEXT ) ;
	tooltip.LoadString( IDB_SET_AND_NEXT ) ;
	m_button_set_and_next->put_TooltipText( tooltip ) ;
	m_button_set_and_next->put_Tag( caption ) ;

	caption.LoadString( IDB_LOOKUP ) ;
	tooltip.LoadString( IDB_LOOKUP ) ;
	m_button_lookup->put_TooltipText( tooltip ) ;
	m_button_lookup->put_Tag( caption ) ;

	caption.LoadString( IDB_LOOKUP_NEXT ) ;
	tooltip.LoadString( IDB_LOOKUP_NEXT ) ;
	m_button_lookup_next->put_TooltipText( tooltip ) ;
	m_button_lookup_next->put_Tag( caption ) ;

	caption.LoadString( IDB_GLOSS_N ) ;
	tooltip.LoadString( IDB_GLOSS_N ) ;
	m_button_gloss_n->put_TooltipText( tooltip ) ;
	m_button_gloss_n->put_Tag( caption ) ;

	caption.LoadString( IDB_HELP ) ;
	tooltip.LoadString( IDB_HELP ) ;
	m_button_help->put_TooltipText( tooltip ) ;
	m_button_help->put_Tag( caption ) ;

	m_properties.set_preferred_gui_lang(PREF_LANG_ENGLISH) ;
	m_properties.write_to_registry() ;
}


void CConnect::gui_to_japanese()
{
	logging::log_debug("CConnect::gui_to_japanese") ;
	CComBSTR caption, tooltip ;

	//	caption.LoadString( IDS_AUTO_TRANS_SEL_J ) ;
	//	tooltip.LoadString( IDS_AUTO_TRANS_SEL_J ) ;
	//	m_menu_auto_trans->put_Caption( caption ) ;
	//	m_menu_auto_trans->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_AUTO_TRANS_FUZZY_J ) ;
	tooltip.LoadString( IDS_AUTO_TRANS_FUZZY_J ) ;
	m_menu_auto_trans_fuzzy->put_Caption( caption ) ;
	m_menu_auto_trans_fuzzy->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_GET_J ) ;
	tooltip.LoadString( IDS_GET_J ) ;
	m_menu_get->put_Caption( caption ) ;
	m_menu_get->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_GET_AND_NEXT_J ) ;
	tooltip.LoadString( IDS_GET_AND_NEXT_J ) ;
	m_menu_get_and_next->put_Caption( caption ) ;
	m_menu_get_and_next->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_SET_J ) ;
	tooltip.LoadString( IDS_SET_J ) ;
	m_menu_set->put_Caption( caption ) ;
	m_menu_set->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_SET_AND_NEXT_J ) ;
	tooltip.LoadString( IDS_SET_AND_NEXT_J ) ;
	m_menu_set_and_next->put_Caption( caption ) ;
	m_menu_set_and_next->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_LOOKUP_J ) ;
	tooltip.LoadString( IDS_LOOKUP_J ) ;
	m_menu_lookup->put_Caption( caption ) ;
	m_menu_lookup->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_LOOKUP_NEXT_J ) ;
	tooltip.LoadString( IDS_LOOKUP_NEXT_J ) ;
	m_menu_lookup_next->put_Caption( caption ) ;
	m_menu_lookup_next->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_EXTEND_LOOKUP_J ) ;
	tooltip.LoadString( IDS_EXTEND_LOOKUP_J ) ;
	m_menu_extend_lookup->put_Caption( caption ) ;
	m_menu_extend_lookup->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_CONCORDANCE_J ) ;
	tooltip.LoadString( IDS_CONCORDANCE_J ) ;
	m_menu_concordance->put_Caption( caption ) ;
	m_menu_concordance->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_REGISTER_GLOSS_J ) ;
	tooltip.LoadString( IDS_REGISTER_GLOSS_J ) ;
	m_menu_register_gloss->put_Caption( caption ) ;
	m_menu_register_gloss->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_DELETE_J ) ;
	tooltip.LoadString( IDS_DELETE_J ) ;
	m_menu_delete->put_Caption( caption ) ;
	m_menu_delete->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_NEXT_J ) ;
	tooltip.LoadString( IDS_NEXT_J ) ;
	m_menu_next->put_Caption( caption ) ;
	m_menu_next->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_PREV_J ) ;
	tooltip.LoadString( IDS_PREV_J ) ;
	m_menu_prev->put_Caption( caption ) ;
	m_menu_prev->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_GLOSS_POPUP_J ) ;
	m_gloss_menu_popup->put_Caption( caption ) ;

	caption.LoadString( IDS_ENTRY_0_J ) ;
	tooltip.LoadString( IDS_ENTRY_0_J ) ;
	m_menu_entry_0->put_Caption( caption ) ;
	m_menu_entry_0->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_ENTRY_1_J ) ;
	tooltip.LoadString( IDS_ENTRY_1_J ) ;
	m_menu_entry_1->put_Caption( caption ) ;
	m_menu_entry_1->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_ENTRY_2_J ) ;
	tooltip.LoadString( IDS_ENTRY_2_J ) ;
	m_menu_entry_2->put_Caption( caption ) ;
	m_menu_entry_2->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_ENTRY_3_J ) ;
	tooltip.LoadString( IDS_ENTRY_3_J ) ;
	m_menu_entry_3->put_Caption( caption ) ;
	m_menu_entry_3->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_ENTRY_4_J ) ;
	tooltip.LoadString( IDS_ENTRY_4_J ) ;
	m_menu_entry_4->put_Caption( caption ) ;
	m_menu_entry_4->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_ENTRY_5_J ) ;
	tooltip.LoadString( IDS_ENTRY_5_J ) ;
	m_menu_entry_5->put_Caption( caption ) ;
	m_menu_entry_5->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_ENTRY_6_J ) ;
	tooltip.LoadString( IDS_ENTRY_6_J ) ;
	m_menu_entry_6->put_Caption( caption ) ;
	m_menu_entry_6->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_ENTRY_7_J ) ;
	tooltip.LoadString( IDS_ENTRY_7_J ) ;
	m_menu_entry_7->put_Caption( caption ) ;
	m_menu_entry_7->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_ENTRY_8_J ) ;
	tooltip.LoadString( IDS_ENTRY_8_J ) ;
	m_menu_entry_8->put_Caption( caption ) ;
	m_menu_entry_8->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_ENTRY_9_J ) ;
	tooltip.LoadString( IDS_ENTRY_9_J ) ;
	m_menu_entry_9->put_Caption( caption ) ;
	m_menu_entry_9->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_GLOSS_N_J ) ;
	tooltip.LoadString( IDS_GLOSS_N_J ) ;
	m_menu_gloss_n->put_Caption( caption ) ;
	m_menu_gloss_n->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_LOOKUP_TRANS_J ) ;
	tooltip.LoadString( IDS_LOOKUP_TRANS_J ) ;
	m_menu_lookup_trans->put_Caption( caption ) ;
	m_menu_lookup_trans->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_LOOKUP_NEXT_TRANS_J ) ;
	tooltip.LoadString( IDS_LOOKUP_NEXT_TRANS_J ) ;
	m_menu_lookup_next_trans->put_Caption( caption ) ;
	m_menu_lookup_next_trans->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_TRANS_CONCORDANCE_J ) ;
	tooltip.LoadString( IDS_TRANS_CONCORDANCE_J ) ;
	m_menu_trans_concordance->put_Caption( caption ) ;
	m_menu_trans_concordance->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_CORRECT_TRANS_J ) ;
	tooltip.LoadString( IDS_CORRECT_TRANS_J ) ;
	m_menu_correct_trans->put_Caption( caption ) ;
	m_menu_correct_trans->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_EXTEND_LOOKUP_TRANS_J ) ;
	tooltip.LoadString( IDS_EXTEND_LOOKUP_TRANS_J ) ;
	m_menu_extend_trans_lookup->put_Caption( caption ) ;
	m_menu_extend_trans_lookup->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_SAVE_J ) ;
	tooltip.LoadString( IDS_SAVE_J ) ;
	m_menu_save->put_Caption( caption ) ;
	m_menu_save->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_HELP_J ) ;
	tooltip.LoadString( IDS_HELP_J ) ;
	m_menu_help->put_Caption( caption ) ;
	m_menu_help->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_MENU_GUI_J ) ;
	tooltip.LoadString( IDS_MENU_GUI_J ) ;
	m_menu_gui->put_Caption( caption ) ;
	m_menu_gui->put_TooltipText( tooltip ) ;

	caption.LoadString( IDS_USER_PREFERENCES_J ) ;
	tooltip.LoadString( IDS_USER_PREFERENCES_J ) ;
	m_menu_preferences->put_TooltipText( tooltip ) ;
	m_menu_preferences->put_Caption( caption ) ;

	caption.LoadString( IDS_GET_J ) ;
	tooltip.LoadString( IDS_GET_J ) ;
	m_button_get->put_TooltipText( tooltip ) ;
	m_button_get->put_Tag( caption ) ;

	caption.LoadString( IDS_SET_J ) ;
	tooltip.LoadString( IDS_SET_J ) ;
	m_button_set->put_TooltipText( tooltip ) ;
	m_button_set->put_Tag( caption ) ;

	caption.LoadString( IDS_GET_AND_NEXT_J ) ;
	tooltip.LoadString( IDS_GET_AND_NEXT_J ) ;
	m_button_get_and_next->put_TooltipText( tooltip ) ;
	m_button_get_and_next->put_Tag( caption ) ;

	caption.LoadString( IDS_SET_AND_NEXT_J ) ;
	tooltip.LoadString( IDS_SET_AND_NEXT_J ) ;
	m_button_set_and_next->put_TooltipText( tooltip ) ;
	m_button_set_and_next->put_Tag( caption ) ;

	caption.LoadString( IDS_LOOKUP_J ) ;
	tooltip.LoadString( IDS_LOOKUP_J ) ;
	m_button_lookup->put_TooltipText( tooltip ) ;
	m_button_lookup->put_Tag( caption ) ;

	caption.LoadString( IDS_LOOKUP_NEXT_J ) ;
	tooltip.LoadString( IDS_LOOKUP_NEXT_J ) ;
	m_button_lookup_next->put_TooltipText( tooltip ) ;
	m_button_lookup_next->put_Tag( caption ) ;

	caption.LoadString( IDS_GLOSS_N_J ) ;
	tooltip.LoadString( IDS_GLOSS_N_J ) ;
	m_button_gloss_n->put_TooltipText( tooltip ) ;
	m_button_gloss_n->put_Tag( caption ) ;

	caption.LoadString( IDS_HELP_J ) ;
	tooltip.LoadString( IDS_HELP_J ) ;
	m_button_help->put_TooltipText( tooltip ) ;
	m_button_help->put_Tag( caption ) ;

	m_properties.set_preferred_gui_lang(PREF_LANG_JAPANESE) ;
	m_properties.write_to_registry() ;
}

void __stdcall CConnect::OnHelp( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnHelp") ;
		file::CPath path ;
		path.GetModulePath( _AtlModule.GetResourceInstance() ) ;

		CString helpfile_name = _T("http://felix-cat.com/media/manuals/felix/7.html") ;

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
	CATCH_ALL(_T("Error showing help for Felix PowerPoint interface")) ;
}

void __stdcall CConnect::OnMenuPreferences( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnMenuPreferences") ;
		BANNER("OnMenuPreferences") ;
		if ( ! m_properties.read_from_registry() )
		{
			m_properties.setDefaults() ;
		}

		TRACE(m_properties.get_preferred_gui_lang()) ;
		if ( m_properties.get_preferred_gui_lang() == PREF_LANG_ENGLISH )
		{
			CPropertiesDlgE props_dlg(m_properties) ;
			const INT_PTR result = props_dlg.DoModal( ) ;
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
			if ( result <= 0 || result == IDCANCEL ) 
			{
				ATLTRACE("User canceled.\n") ;
				return ;
			}

			ATLTRACE("User clicked 'OK'.\n") ;
			m_properties = props_dlg.get_properties() ;
		}
		const int props[] = {SKIP_IF_J, SKIP_UNLESS_J, NO_SKIP} ;
		const size_t index = static_cast<size_t>(m_properties.m_data.m_skipJ) ;
		ATLASSERT(index < sizeof(props)) ;
		m_properties.m_data.m_skipJ = props[index] ;

		TRACE(m_properties.get_preferred_gui_lang()) ;
		if ( m_properties.get_preferred_gui_lang() == PREF_LANG_ENGLISH)
		{
			gui_to_english() ;
		}
		else
		{
			gui_to_japanese() ;
		}
		m_properties.write_to_registry() ;

	}
	CATCH_ALL(_T("Error showing Felix preferences")) ;

}

void __stdcall CConnect::OnSave( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnSave") ;
		m_interface.OnSaveAction() ;
	}
	CATCH_ALL(_T("OnSave")) ;
}

void __stdcall CConnect::OnGlossN( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnGlossN") ;
		m_interface.OnGlossNAction() ;
	}
	CATCH_ALL(_T("OnGlossN")) ;
}

void __stdcall CConnect::OnLookupNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnLookupNext") ;
		m_interface.OnLookupNextAction() ;
	}
	CATCH_ALL(_T("OnLookupNext")) ;
}

void __stdcall CConnect::OnLookup( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnLookup") ;
		m_interface.OnLookupAction() ;
	}
	CATCH_ALL(_T("OnLookup")) ;
}

void __stdcall CConnect::OnSetAndNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnSetAndNext") ;
		m_interface.OnSetAndNextAction() ;
	}
	CATCH_ALL(_T("OnSetAndNext")) ;
}

void __stdcall CConnect::OnGetAndNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnGetAndNext") ;
		m_interface.OnGetAndNextAction() ;
	}
	CATCH_ALL(_T("OnGetAndNext")) ;
}

void __stdcall CConnect::OnSet( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnSet") ;
		m_interface.OnSetAction() ;
	}
	CATCH_ALL(_T("OnSet")) ;
}

void __stdcall CConnect::OnMenuGui( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnMenuGui") ;
		m_properties.read_from_registry() ;
		if ( m_properties.get_preferred_gui_lang() == PREF_LANG_ENGLISH)
		{
			gui_to_japanese() ;
		}
		else
		{
			gui_to_english() ;
		}
	}
	CATCH_ALL( _T("Error switching GUI language") ) ;
}

void __stdcall CConnect::OnGet( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnGet") ;
		m_interface.OnGetAction() ;
	}
	CATCH_ALL(_T("OnGet")) ;
}

void __stdcall CConnect::OnRestoreAndNextTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnRestoreAndNextTrans") ;
		m_interface.OnRestoreAndNextTransAction() ;
	}
	CATCH_ALL(_T("OnRestoreAndNextTrans")) ;
}

void __stdcall CConnect::OnRestoreTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnRestoreTrans") ;
		m_interface.OnRestoreTransAction() ;
	}
	CATCH_ALL(_T("OnRestoreTrans")) ;
}

void __stdcall CConnect::OnCorrectAndNextTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnCorrectAndNextTrans") ;
		m_interface.OnCorrectAndNextTransAction() ;
	}
	CATCH_ALL(_T("OnCorrectAndNextTrans")) ;
}

void __stdcall CConnect::OnCorrectTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnCorrectTrans") ;
		m_interface.OnCorrectTransAction() ;
	}
	CATCH_ALL(_T("OnCorrectTrans")) ;
}

void __stdcall CConnect::OnExtendTransLookup( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnExtendTransLookup") ;
		m_interface.OnExtendTransLookupAction() ;
	}
	CATCH_ALL(_T("OnExtendTransLookup")) ;
}

void __stdcall CConnect::OnTransConcordance( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnTransConcordance") ;
		m_interface.OnTransConcordanceAction() ;
	}
	CATCH_ALL(_T("OnTransConcordance")) ;
}

void __stdcall CConnect::OnLookupNextTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnLookupNextTrans") ;
		m_interface.OnLookupNextTransAction() ;
	}
	CATCH_ALL(_T("OnLookupNextTrans")) ;
}

void __stdcall CConnect::OnLookupTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnLookupTrans") ;
		m_interface.OnLookupTransAction() ;
	}
	CATCH_ALL(_T("OnLookupTrans")) ;
}

void __stdcall CConnect::OnEntry9( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry9") ;
		m_interface.OnEntryAction( 9, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry9")) ;
}

void __stdcall CConnect::OnEntry8( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry8") ;
		m_interface.OnEntryAction( 8, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry8")) ;
}

void __stdcall CConnect::OnEntry7( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry7") ;
		m_interface.OnEntryAction( 7, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry7")) ;
}

void __stdcall CConnect::OnEntry6( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry6") ;
		m_interface.OnEntryAction( 6, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry6")) ;
}

void __stdcall CConnect::OnEntry5( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry5") ;
		m_interface.OnEntryAction( 5, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry5")) ;
}

void __stdcall CConnect::OnEntry4( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry4") ;
		m_interface.OnEntryAction( 4, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry4")) ;
}

void __stdcall CConnect::OnEntry3( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry3") ;
		m_interface.OnEntryAction( 3, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry3")) ;
}

void __stdcall CConnect::OnEntry2( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry2") ;
		m_interface.OnEntryAction( 2, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry2")) ;
}

void __stdcall CConnect::OnEntry1( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry1") ;
		m_interface.OnEntryAction( 1, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry1")) ;
}

void __stdcall CConnect::OnEntry0( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnEntry0") ;
		m_interface.OnEntryAction( 0, shift_key_is_pressed() ) ;
	}
	CATCH_ALL(_T("OnEntry0")) ;
}

void __stdcall CConnect::OnPrev( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnPrev") ;
		m_interface.OnPrevAction() ;
	}
	CATCH_ALL(_T("OnPrev")) ;
}

void __stdcall CConnect::OnNext( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnNext") ;
		m_interface.OnNextAction() ;
	}
	CATCH_ALL(_T("OnNext")) ;
}

void __stdcall CConnect::OnDelete( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnDelete") ;
		m_interface.OnDeleteAction() ;
	}
	CATCH_ALL(_T("OnDelete")) ;
}

void __stdcall CConnect::OnRegisterGloss( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnRegisterGloss") ;
		m_interface.OnRegisterGlossAction() ;
	}
	CATCH_ALL(_T("OnRegisterGloss")) ;
}

void __stdcall CConnect::OnExtendLookup( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		m_interface.m_is_auto = false ;
		logging::log_debug("CConnect::OnExtendLookup") ;
		m_interface.OnExtendLookupAction() ;
	}
	CATCH_ALL(_T("OnExtendLookup")) ;
}

void __stdcall CConnect::OnConcordance( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnConcordance") ;
		m_interface.OnConcordanceAction() ;
	}
	CATCH_ALL(_T("OnConcordance")) ;
}

void __stdcall CConnect::OnAutoTransFuzzy( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnAutoTransFuzzy") ;
		m_interface.OnAutoTransFuzzyAction() ;
	}
	CATCH_ALL(_T("OnAutoTransFuzzy")) ;
}

void __stdcall CConnect::OnAutoTrans( IDispatch *, VARIANT_BOOL * )
{
	try
	{
		logging::log_debug("CConnect::OnAutoTrans") ;
		m_interface.OnAutoTransAction() ;
	}
	CATCH_ALL(_T("OnAutoTrans")) ;
}