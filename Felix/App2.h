// App2.h : Declaration of the CApp2

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"
#include "_IApp2Events_CP.h"
#include "SearchResult.h"
#include "SearchResults.h"
#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler
#include "ComMemory.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// CApp2

class ATL_NO_VTABLE CApp2 :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CApp2, &CLSID_App2>,
//	public CComCoClass<CApp2, &CLSID_App2>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CApp2>,
	public CProxy_IApp2Events<CApp2>,
	public IDispatchImpl<IApp2, &IID_IApp2, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	typedef CComObject< CSearchResult > MatchObject ;

	MatchObject *m_current_match ;

	CComObject<CSearchResults> *m_current_matches ;
	CComObject<CSearchResults> *m_current_gloss_matches ;

	CComObject<CComMemory> *m_active_memory ;
	CComObject<CComMemory> *m_active_glossary ;

public:
	CApp2() :
	  m_current_match(NULL),
	  m_current_matches(NULL),
	  m_current_gloss_matches(NULL),
	  m_active_memory(NULL),
	  m_active_glossary(NULL)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_APP2)


BEGIN_COM_MAP(CApp2)
	COM_INTERFACE_ENTRY(IApp2)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CApp2)
	CONNECTION_POINT_ENTRY(__uuidof(_IApp2Events))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		HRESULT hr = MatchObject::CreateInstance( &m_current_match ) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		m_current_match->AddRef() ;

		hr = CComObject<CSearchResults>::CreateInstance(&m_current_matches) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		m_current_matches->AddRef() ;

		hr = CComObject<CSearchResults>::CreateInstance(&m_current_gloss_matches) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		m_current_gloss_matches->AddRef() ;

		hr = CComObject<CComMemory>::CreateInstance(&m_active_memory) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		m_active_memory->AddRef() ;

		hr = CComObject<CComMemory>::CreateInstance(&m_active_glossary) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		m_active_glossary->AddRef() ;

		return hr ;
	}

	void FinalRelease()
	{
		if ( m_current_match )
		{
			m_current_match->Release() ;
		}
		if ( m_current_matches )
		{
			m_current_matches->Release() ;
		}
		if ( m_current_gloss_matches )
		{
			m_current_gloss_matches->Release() ;
		}
	}

public:
	STDMETHOD(get_ActiveMemory)(IComMemory **pVal);
	STDMETHOD(get_ActiveGlossary)(IComMemory **pVal);

	STDMETHOD(get_CurrentMatches)(ISearchResults **pVal);
	STDMETHOD(get_CurrentGlossMatches)(ISearchResults **pVal);

	STDMETHOD(get_CurrentMatch)(ISearchResult **pVal);
	STDMETHOD(put_CurrentMatch)(ISearchResult *pVal);
	STDMETHOD(ReflectChanges)(ULONG RecId, BSTR Source, BSTR Trans);
	STDMETHOD(ReviewTranslation)(ULONG RecId, BSTR Source, BSTR Trans);
};

OBJECT_ENTRY_AUTO(__uuidof(App2), CApp2)
