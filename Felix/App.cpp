// App.cpp : Implementation of CApp

#include "stdafx.h"
#include "App.h"

#include "Felix.h"
#include "Exceptions.h"					// exception support

#include "FelixApp.h"
#include "atlscintilla.h"
#include "background_processor.h"

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
	FELIX_AUTO_CATCH("App2")
	return E_FAIL ;
}

STDMETHODIMP CApp::get_Visible(VARIANT_BOOL* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
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
	FELIX_AUTO_CATCH( "Visible" ) ;

	return S_OK;
}

STDMETHODIMP CApp::put_Visible(VARIANT_BOOL newVal)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
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
	FELIX_AUTO_CATCH( "Visible" ) ;

	return S_OK;
}

STDMETHODIMP CApp::Quit(void)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		if ( ! MainFrame.IsWindow() )
		{
			::PostQuitMessage( 0 ) ;
		}
		else
		{
			MainFrame.exit_silently() ;
		}
	}
	FELIX_AUTO_CATCH( "Quit" ) ;

	return S_OK;
}

STDMETHODIMP CApp::Lookup(BSTR Query)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.lookup(BSTR2wstring( Query )) ;

		//MainFrame.m_deferred_query = BSTR2wstring( Query ) ;
		//MainFrame.PostMessage( UWM_USER_MESSAGE, USER_LOOKUP_SOURCE, USER_LOOKUP_SOURCE ) ;
	}
	FELIX_AUTO_CATCH( "Lookup" ) ;

	return S_OK;
}

STDMETHODIMP CApp::LookupTrans(BSTR Trans)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.lookup_trans(BSTR2wstring( Trans )) ;

		//MainFrame.m_deferred_query = BSTR2wstring( Trans ) ;
		//MainFrame.PostMessage( UWM_USER_MESSAGE, USER_LOOKUP_TRANS, USER_LOOKUP_TRANS ) ;
	}
	FELIX_AUTO_CATCH( "LookupTrans" ) ;

	return S_OK;
}

STDMETHODIMP CApp::LookupDeferred(BSTR Query)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.m_deferred_query = BSTR2wstring( Query ) ;
		MainFrame.PostMessage( UWM_USER_MESSAGE, USER_LOOKUP_SOURCE, USER_LOOKUP_SOURCE ) ;
	}
	FELIX_AUTO_CATCH( "Lookup" ) ;

	return S_OK;
}

STDMETHODIMP CApp::LookupTransDeferred(BSTR Trans)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.m_deferred_query = BSTR2wstring( Trans ) ;
		MainFrame.PostMessage( UWM_USER_MESSAGE, USER_LOOKUP_TRANS, USER_LOOKUP_TRANS ) ;
	}
	FELIX_AUTO_CATCH( "LookupTrans" ) ;

	return S_OK;
}

STDMETHODIMP CApp::Concordance(BSTR Query)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.get_concordances( BSTR2wstring( Query ) ) ;
	}
	FELIX_AUTO_CATCH( "Concordance" ) ;

	return S_OK;
}

STDMETHODIMP CApp::TransConcordance(BSTR Trans)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.get_translation_concordances( BSTR2wstring( Trans ) ) ;
	}
	FELIX_AUTO_CATCH( "TransConcordance" ) ;

	return S_OK;
}

STDMETHODIMP CApp::get_Score(DOUBLE* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		*pVal = MainFrame.get_score() ;
	}
	FELIX_AUTO_CATCH( "get_Score" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::get_Query(BSTR* pVal)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		const wstring current_query = MainFrame.get_current_query( ) ;
		*pVal = ::SysAllocStringLen( current_query.c_str(), current_query.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_Query" ) ;

	return S_OK ;
}
STDMETHODIMP CApp::put_Query(BSTR Query)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.lookup(BSTR2wstring( Query )) ;
	}
	FELIX_AUTO_CATCH( "put_Query" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::get_Trans(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		const wstring trans = MainFrame.get_current_translation( ) ;
		*pVal = ::SysAllocStringLen( trans.c_str(), trans.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_Trans" ) ;

	return S_OK ;
}
STDMETHODIMP CApp::put_Trans(BSTR pVal)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.set_translation(  BSTR2wstring( pVal ) ) ;
	}
	FELIX_AUTO_CATCH( "put_Trans" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::Save(void)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.PostMessage( UWM_USER_MESSAGE, USER_SAVE_MEMORIES, USER_SAVE_MEMORIES ) ;
	}
	FELIX_AUTO_CATCH( "Save" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::NextTrans(void)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.next_match() ;
	}
	FELIX_AUTO_CATCH( "NextTrans" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::PrevTrans(void)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.prev_match() ;
	}
	FELIX_AUTO_CATCH( "PrevTrans" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::get_GlossMatch(SHORT Index, BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		const wstring entry = MainFrame.get_glossary_entry( Index ) ;
		*pVal = ::SysAllocStringLen( entry.c_str(), entry.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_GlossMatch" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::get_NumGlossMatches(SHORT* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		gloss_window_pointer gloss = MainFrame.get_glossary_window() ;
		*pVal = static_cast< SHORT >( gloss->num_matches() ) ;
	}
	FELIX_AUTO_CATCH( "get_NumGlossMatches" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::LoadMemory(BSTR MemoryName)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.load( MemoryName ) ;
	}
	FELIX_AUTO_CATCH( "LoadMemory" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::LoadGlossary(BSTR GlossaryName)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		gloss_window_pointer gloss = MainFrame.get_glossary_window() ;
		gloss->load( GlossaryName ) ;

	}
	FELIX_AUTO_CATCH( "LoadGlossary" ) ;

	return S_OK ;
}

STDMETHODIMP CApp::ClearMemories(void)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.clear_memory() ;
	}
	FELIX_AUTO_CATCH( "ClearMemories" ) ;

	return S_OK;
}

STDMETHODIMP CApp::ClearGlossaries(void)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		gloss_window_pointer gloss = MainFrame.get_glossary_window() ;
		gloss->clear_memory() ;
	}
	FELIX_AUTO_CATCH( "ClearGlossaries" ) ;

	return S_OK;
}

STDMETHODIMP CApp::get_ShowMarkup(VARIANT_BOOL* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		*pVal = MainFrame.get_show_marking() ;
	}
	FELIX_AUTO_CATCH( "get_ShowMarkup" ) ;

	return S_OK;
}

STDMETHODIMP CApp::put_ShowMarkup(VARIANT_BOOL newVal)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.put_show_marking( newVal ) ;
	}
	FELIX_AUTO_CATCH( "put_ShowMarkup" ) ;

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

		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.add_record( record ) ;
	}
	FELIX_AUTO_CATCH( "AddMemoryEntry" ) ;

	return S_OK;
}

STDMETHODIMP CApp::AddGlossaryEntry(BSTR Source, BSTR Trans, BSTR EntryContext)
{
	try
	{
		record_pointer record(new record_local) ;
		record->set_source( BSTR2wstring(Source) ) ;
		record->set_trans( BSTR2wstring(Trans) ) ;
		record->set_context( BSTR2wstring(EntryContext) ) ;

		MemoryWindowFrame &MainFrame = app::get_app() ;
		gloss_window_pointer gloss = MainFrame.get_glossary_window() ;
		gloss->add_record(record) ;

	}
	FELIX_AUTO_CATCH( "AddGlossaryEntry" )

	return S_OK;
}

STDMETHODIMP CApp::DeleteMemEntry(void)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.delete_current_translation( ) ;
	}
	FELIX_AUTO_CATCH( "DeleteMemEntry" ) ;

	return S_OK;
}

STDMETHODIMP CApp::CorrectTrans(BSTR Trans)
{
	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		MainFrame.correct_trans(BSTR2wstring(Trans)) ;
	}
	FELIX_AUTO_CATCH( _T("CorrectedTranslation") )

	return S_OK;
}

STDMETHODIMP CApp::get_NumMatches( SHORT *index )
{
	CHECK_OUT_PTR( index ) ;

	try
	{
		MemoryWindowFrame &MainFrame = app::get_app() ;
		*index = static_cast< SHORT >( MainFrame.get_num_matches() ) ;
	}
	FELIX_AUTO_CATCH( "get_NumMatches" ) ;

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
	MemoryWindowFrame &MainFrame = app::get_app() ;
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

HRESULT CApp::FinalConstruct()
{
	HRESULT hr = CComObject< CApp2 >::CreateInstance( &m_App2 ) ;
	if ( SUCCEEDED( hr ) )
	{
		m_App2->AddRef() ;
	}
	return hr ;
}

void CApp::FinalRelease()
{
	if ( m_App2 )
	{
		m_App2->Release() ;
	}
}