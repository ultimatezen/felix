// App.cpp : Implementation of CApp

#include "stdafx.h"
#include "App.h"

#include "Felix.h"
#include "Exceptions.h"					// exception support

#include "FelixApp.h"
#include <comdef.h> // _com_error
#include "atlscintilla.h"
#include "background_processor.h"

// To use this macro, derive from CAutomationExceptionHandler
#define TA_CATCH( func_str ) \
	catch( _com_error &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }  \
	catch( CWinException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CComException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CSWException &e )    { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CException &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch ( std::exception &e ) { CString Func( func_str ) ; return handle_exception( e, Func ) ; }

// CApp
using namespace except ;
using namespace mem_engine ;

STDMETHODIMP CApp::get_App2(IApp2 **pVal)
{
	try
	{
		if ( m_App2 )
		{
			return m_App2->QueryInterface(pVal) ;
		}
	}
	TA_CATCH("App2")
	return E_FAIL ;
}

STDMETHODIMP CApp::get_Visible(VARIANT_BOOL* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		if ( ! MainFrame.IsWindow() )
		{
			*pVal = VARIANT_FALSE ;
			return S_OK ;
		}

		if ( MainFrame.IsWindowVisible() )
		{
			*pVal = VARIANT_TRUE ;
		}
		else
		{
			*pVal = VARIANT_FALSE ;
		}
	}
	TA_CATCH( "Visible" ) ;

	return S_OK;
}

STDMETHODIMP CApp::put_Visible(VARIANT_BOOL newVal)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		if ( ! MainFrame.IsWindow() )
		{
			if ( newVal == VARIANT_FALSE )
			{
				return S_OK ;
			}
			if ( ! MainFrame.CreateEx() )
			{
				return E_FAIL ;
			}
		}
		ATLASSERT( MainFrame.IsWindow() ) ;

		MainFrame.put_visible( newVal ? SW_SHOW : SW_HIDE ) ;
	}
	TA_CATCH( "Visible" ) ;

	return S_OK;
}

STDMETHODIMP CApp::Quit(void)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		if ( ! MainFrame.IsWindow() )
		{
			::PostQuitMessage( 0 ) ;
		}
		else
		{
			MainFrame.exit_silently() ;
		}
	}
	TA_CATCH( "Quit" ) ;

	return S_OK;
}

STDMETHODIMP CApp::Lookup(BSTR Query)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.lookup(BSTR2wstring( Query )) ;

		//MainFrame.m_deferred_query = BSTR2wstring( Query ) ;
		//MainFrame.PostMessage( UWM_USER_MESSAGE, USER_LOOKUP_SOURCE, USER_LOOKUP_SOURCE ) ;
	}
	TA_CATCH( "Lookup" ) ;

	return S_OK;
}

STDMETHODIMP CApp::LookupTrans(BSTR Trans)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.lookup_trans(BSTR2wstring( Trans )) ;

		//MainFrame.m_deferred_query = BSTR2wstring( Trans ) ;
		//MainFrame.PostMessage( UWM_USER_MESSAGE, USER_LOOKUP_TRANS, USER_LOOKUP_TRANS ) ;
	}
	TA_CATCH( "LookupTrans" ) ;

	return S_OK;
}

STDMETHODIMP CApp::LookupDeferred(BSTR Query)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.m_deferred_query = BSTR2wstring( Query ) ;
		MainFrame.PostMessage( UWM_USER_MESSAGE, USER_LOOKUP_SOURCE, USER_LOOKUP_SOURCE ) ;
	}
	TA_CATCH( "Lookup" ) ;

	return S_OK;
}

STDMETHODIMP CApp::LookupTransDeferred(BSTR Trans)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.m_deferred_query = BSTR2wstring( Trans ) ;
		MainFrame.PostMessage( UWM_USER_MESSAGE, USER_LOOKUP_TRANS, USER_LOOKUP_TRANS ) ;
	}
	TA_CATCH( "LookupTrans" ) ;

	return S_OK;
}

STDMETHODIMP CApp::Concordance(BSTR Query)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.get_concordances( BSTR2wstring( Query ) ) ;
	}
	TA_CATCH( "Concordance" ) ;

	return S_OK;
}

STDMETHODIMP CApp::TransConcordance(BSTR Trans)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.get_translation_concordances( BSTR2wstring( Trans ) ) ;
	}
	TA_CATCH( "TransConcordance" ) ;

	return S_OK;
}

STDMETHODIMP CApp::get_Score(DOUBLE* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		*pVal = MainFrame.get_score() ;
	}
	TA_CATCH( "get_Score" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::get_Query(BSTR* pVal)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		const wstring current_query = MainFrame.get_current_query( ) ;
		*pVal = ::SysAllocStringLen( current_query.c_str(), current_query.size() ) ;
	}
	TA_CATCH( "get_Query" ) ;

	return S_OK ;
}
STDMETHODIMP CApp::put_Query(BSTR Query)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.lookup(BSTR2wstring( Query )) ;
	}
	TA_CATCH( "put_Query" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::get_Trans(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		const wstring trans = MainFrame.get_current_translation( ) ;
		*pVal = ::SysAllocStringLen( trans.c_str(), trans.size() ) ;
	}
	TA_CATCH( "get_Trans" ) ;

	return S_OK ;
}
STDMETHODIMP CApp::put_Trans(BSTR pVal)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.set_translation(  BSTR2wstring( pVal ) ) ;
	}
	TA_CATCH( "put_Trans" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::Save(void)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.PostMessage( UWM_USER_MESSAGE, USER_SAVE_MEMORIES, USER_SAVE_MEMORIES ) ;
	}
	TA_CATCH( "Save" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::NextTrans(void)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.next_match() ;
	}
	TA_CATCH( "NextTrans" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::PrevTrans(void)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.prev_match() ;
	}
	TA_CATCH( "PrevTrans" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::get_GlossMatch(SHORT Index, BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		const wstring entry = MainFrame.get_glossary_entry( Index ) ;
		*pVal = ::SysAllocStringLen( entry.c_str(), entry.size() ) ;
	}
	TA_CATCH( "get_GlossMatch" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::get_NumGlossMatches(SHORT* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		gloss_window_pointer gloss = MainFrame.get_glossary_window() ;
		*pVal = static_cast< SHORT >( gloss->num_matches() ) ;
	}
	TA_CATCH( "get_NumGlossMatches" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::LoadMemory(BSTR MemoryName)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.load( MemoryName ) ;
	}
	TA_CATCH( "LoadMemory" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::LoadGlossary(BSTR GlossaryName)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		gloss_window_pointer gloss = MainFrame.get_glossary_window() ;
		gloss->load( GlossaryName ) ;

	}
	TA_CATCH( "LoadGlossary" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::ClearMemories(void)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.clear_memory() ;
	}
	TA_CATCH( "ClearMemories" ) ;

	return S_OK;
}

STDMETHODIMP CApp::ClearGlossaries(void)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		gloss_window_pointer gloss = MainFrame.get_glossary_window() ;
		gloss->clear_memory() ;
	}
	TA_CATCH( "ClearGlossaries" ) ;

	return S_OK;
}

STDMETHODIMP CApp::get_ShowMarkup(VARIANT_BOOL* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		*pVal = MainFrame.get_show_marking() ;
	}
	TA_CATCH( "get_ShowMarkup" ) ;

	return S_OK;
}

STDMETHODIMP CApp::put_ShowMarkup(VARIANT_BOOL newVal)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.put_show_marking( newVal ) ;
	}
	TA_CATCH( "put_ShowMarkup" ) ;

	return S_OK;
}

STDMETHODIMP CApp::AddMemoryEntry(BSTR Source, BSTR Trans, BSTR EntryContext)
{
	try
	{
		record_pointer record(new record_local()) ;
		record->set_source( BSTR2wstring(Source) ) ;
		record->set_trans( BSTR2wstring(Trans) ) ;
		record->set_context( BSTR2wstring(EntryContext) ) ;

		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.add_record( record ) ;
	}
	TA_CATCH( "AddMemoryEntry" ) ;

	return S_OK;
}

STDMETHODIMP CApp::AddGlossaryEntry(BSTR Source, BSTR Trans, BSTR EntryContext)
{
	try
	{

		record_pointer record(new record_local()) ;
		record->set_source( BSTR2wstring(Source) ) ;
		record->set_trans( BSTR2wstring(Trans) ) ;
		record->set_context( BSTR2wstring(EntryContext) ) ;

		CMainFrame &MainFrame = app::get_app() ;
		gloss_window_pointer gloss = MainFrame.get_glossary_window() ;
		gloss->add_record(record) ;

	}
	TA_CATCH( "AddGlossaryEntry" )

	return S_OK;
}

STDMETHODIMP CApp::DeleteMemEntry(void)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.delete_current_translation( ) ;
	}
	TA_CATCH( "DeleteMemEntry" ) ;

	return S_OK;
}

STDMETHODIMP CApp::CorrectTrans(BSTR Trans)
{
	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		MainFrame.correct_trans(BSTR2wstring(Trans)) ;
	}
	TA_CATCH( _T("CorrectedTranslation") )

	return S_OK;
}

STDMETHODIMP CApp::get_NumMatches( SHORT *index )
{
	CHECK_OUT_PTR( index ) ;

	try
	{
		CMainFrame &MainFrame = app::get_app() ;
		*index = static_cast< SHORT >( MainFrame.get_num_matches() ) ;
	}
	TA_CATCH( "get_NumMatches" ) ;

	return S_OK ;
}

CApp::CApp()
{
	// Initialize the Scintilla module here, because we once got an error that this wasn't
	// initialized.
	// Really need to track this down so we only need to initialize it once.
	WTL::ScintillaModule &scintilla_module = WTL::ScintillaModule::instance() ;
	scintilla_module ;
	ATLASSERT(scintilla_module.IsLoaded()) ;
}

void CApp::wait_for_query()
{
	CMainFrame &MainFrame = app::get_app() ;
	if(MainFrame.m_deferred_query.empty())
	{
		return ;
	}

	// I know that this is a cheesy hack...
	background_processor processor ;
	size_t iterations = 0 ;
	const size_t MAX_ITERATIONS = 50 ;
	while(!MainFrame.m_deferred_query.empty())
	{
		processor.perform_background_processing() ;
		++iterations;
		if (iterations > MAX_ITERATIONS)
		{
			MainFrame.lookup(MainFrame.m_deferred_query) ;
			MainFrame.m_deferred_query.clear() ;
			return ;
		}
	}
}