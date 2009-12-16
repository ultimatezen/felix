// SearchResult.h : Declaration of the CSearchResult

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"
#include "ComRecord.h"
#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler
#include "record.h"
#include "query.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CSearchResult

class ATL_NO_VTABLE CSearchResult :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CSearchResult, &CLSID_SearchResult>,
//	public CComCoClass<CSearchResult, &CLSID_SearchResult>,
	public IDispatchImpl<ISearchResult, &IID_ISearchResult, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	typedef CComObject< CRecord > RecordObject ;

	RecordObject *m_record ;
	mem_engine::search_match_ptr m_match ;
public:

	void set_match(mem_engine::search_match_ptr match)
	{
		m_match = match ;
		this->set_record(m_match->get_record()) ;
	}
	void set_record(mem_engine::record_pointer record)
	{
		if (m_record)
		{
			m_record->set_record(record) ;
		}
	}

	CSearchResult() : m_record(NULL), m_match(new mem_engine::search_match)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SEARCHRESULT)


BEGIN_COM_MAP(CSearchResult)
	COM_INTERFACE_ENTRY(ISearchResult)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		HRESULT hr = RecordObject::CreateInstance( &m_record ) ;
		if ( SUCCEEDED( hr ) )
		{
			m_record->AddRef() ;
		}
		return hr ;
	}

	void FinalRelease()
	{
		if ( m_record )
		{
			m_record->Release() ;
		}
	}

public:
	STDMETHOD(get_Record)(IRecord **pVal);
	STDMETHOD(put_Record)(IRecord *pVal);
	STDMETHOD(get_Score)(DOUBLE* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(SearchResult), CSearchResult)
