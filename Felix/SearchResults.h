// SearchResults.h : Declaration of the CSearchResults

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"
#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler
#include "SearchResult.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// Store the data in a vector of std::strings
template <typename T> 
struct _CopyVariantFromAdaptItf : public _Copy<VARIANT>
{
	static HRESULT copy(VARIANT* p1, const CAdapt< CComPtr<T> >* p2)
	{
		HRESULT hr = p2->m_T->QueryInterface(IID_IDispatch, (void**)&p1->pdispVal);

		if (SUCCEEDED(hr))
		{
			p1->vt = VT_DISPATCH;
			return hr ;
		}

		hr = p2->m_T->QueryInterface(IID_IUnknown, (void**)&p1->punkVal);
		if (SUCCEEDED(hr))
		{
			p1->vt = VT_UNKNOWN;
		}

		return hr;
	}
};

template <typename T> 
struct _CopyItfFromAdaptItf : public _CopyInterface< T >
{
	static HRESULT copy(T** p1, const CAdapt< CComPtr<T> >* p2)
	{
		*p1 = p2->m_T ;
		if (*p1) return (*p1)->AddRef(), S_OK;
		return E_POINTER;
	}
};

typedef CAdapt< CComPtr<ISearchResult> > SearchResultPtr ;

typedef ISearchResults						SearchResultCollectionInterface;
typedef std::vector< SearchResultPtr >		SearchResultContainerType;
typedef IEnumVARIANT						EnumeratorInterface;
typedef VARIANT								EnumeratorExposedType;
typedef _CopyVariantFromAdaptItf<ISearchResult>	SearchResultEnumeratorCopyType;
typedef CComEnumOnSTL
	<     
		EnumeratorInterface,
		&__uuidof(EnumeratorInterface),
		EnumeratorExposedType,
		SearchResultEnumeratorCopyType,
		SearchResultContainerType
	> 
	SearchResultEnumeratorType;

typedef ISearchResult*						SearchResultCollectionExposedType;
typedef _CopyItfFromAdaptItf<ISearchResult>	SearchResultCollectionCopyType;

typedef ICollectionOnSTLImpl
	<     
		SearchResultCollectionInterface,
		SearchResultContainerType,
		SearchResultCollectionExposedType,
		SearchResultCollectionCopyType,
		SearchResultEnumeratorType
	> 
	SearchResultCollectionType;


// CSearchResults

class ATL_NO_VTABLE CSearchResults :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CSearchResults, &CLSID_SearchResults>,
//	public CComCoClass<CSearchResults, &CLSID_SearchResults>,
	public IDispatchImpl<SearchResultCollectionType, &IID_ISearchResults, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:

	CSearchResults()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SEARCHRESULTS)


BEGIN_COM_MAP(CSearchResults)
	COM_INTERFACE_ENTRY(ISearchResults)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	HRESULT set_matches(mem_engine::felix_query *matches);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

};

OBJECT_ENTRY_AUTO(__uuidof(SearchResults), CSearchResults)
