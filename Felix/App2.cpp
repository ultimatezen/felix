// App2.cpp : Implementation of CApp2

#include "stdafx.h"
#include "App2.h"
#include "FelixApp.h"
#include "query.h"
#include "TranslationMemory.h"

using namespace except ;

// CApp2

STDMETHODIMP CApp2::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IApp2
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

// memories/glossaries
STDMETHODIMP CApp2::get_Memories(IMemories **pVal)
{
	try
	{
		if (m_memories)
		{
			m_memories->set_model(app::get_app().get_model()) ;
			return m_memories->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_Memories")

	return E_FAIL ;
}
STDMETHODIMP CApp2::get_Glossaries(IMemories **pVal)
{
	try
	{
		if (m_glossaries)
		{
			m_memories->set_model(app::get_app().get_glossary_window()->get_model()) ;
			return m_glossaries->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_Glossaries")

	return E_FAIL ;
}

// memory/glossary windows
STDMETHODIMP CApp2::get_MemoryWindow(IMemoryWindow **pVal)
{
	try
	{
		if ( m_memory_window)
		{
			return m_memory_window->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_MemoryWindow")

	return E_FAIL ;
}
STDMETHODIMP CApp2::get_GlossaryWindow(IGlossaryWindow **pVal)
{
	try
	{
		if (m_glossary_window)
		{
			return m_glossary_window->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_GlossaryWindow")

	return E_FAIL ;
}

// active memory/glossary
STDMETHODIMP CApp2::get_ActiveMemory(IMemory **pVal)
{
	try
	{
		if ( m_active_memory)
		{
			return m_active_memory->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_ActiveMemory")

	return E_FAIL ;
}
STDMETHODIMP CApp2::get_ActiveGlossary(IMemory **pVal)
{
	try
	{
		if ( m_active_glossary)
		{
			return m_active_glossary->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_ActiveGlossary")

	return E_FAIL ;
}

// current match
STDMETHODIMP CApp2::get_CurrentMatch(ISearchResult **pVal)
{
	try
	{
		if ( m_current_match )
		{
			m_current_match->set_match(app::get_app().get_current_match());
			return m_current_match->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_CurrentMatch")
		
	return E_FAIL ;
}


STDMETHODIMP CApp2::put_CurrentMatch(ISearchResult *pVal)
{
	try
	{
		pVal ;
		return S_OK ;
	}
	FELIX_AUTO_CATCH("put_CurrentMatch")
}

// current matches
STDMETHODIMP CApp2::get_CurrentMatches(ISearchResults **pVal)
{
	try
	{
		if ( m_current_matches )
		{
			m_current_matches->set_matches(app::get_app().get_current_matches());
			return m_current_matches->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_CurrentMatches")

	return E_FAIL ;
}
STDMETHODIMP CApp2::get_CurrentGlossMatches(ISearchResults **pVal)
{
	try
	{
		if ( m_current_gloss_matches )
		{
			mem_engine::felix_query *matches = app::get_app().get_current_gloss_matches() ;
			if (matches)
			{
				m_current_gloss_matches->set_matches(matches);
			}
			return m_current_gloss_matches->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_CurrentGlossMatches")

	return E_FAIL ;
}


STDMETHODIMP CApp2::ReflectChanges(ULONG RecId, BSTR Source, BSTR Trans)
{
	try
	{
		app::get_app().add_by_id(static_cast<size_t>(RecId),
			BSTR2wstring(Source),
			BSTR2wstring(Trans)) ;
	}
	FELIX_AUTO_CATCH( "ReflectChanges" ) ;

	return S_OK;
}
STDMETHODIMP CApp2::ReviewTranslation(ULONG RecId, BSTR Source, BSTR Trans)
{
	try
	{
		app::get_app().view_by_id(static_cast<size_t>(RecId),
			BSTR2wstring(Source),
			BSTR2wstring(Trans)) ;
	}
	FELIX_AUTO_CATCH( "ReviewTranslation" ) ;

	return S_OK;
}

