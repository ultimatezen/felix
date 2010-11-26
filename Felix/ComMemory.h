// ComMemory.h : Declaration of the CComMemory

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"
#include "ComRecords.h"
#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler
#include "TranslationMemory.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// CComMemory

class ATL_NO_VTABLE CComMemory :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CComMemory, &CLSID_ComMemory>,
//	public CComCoClass<CComMemory, &CLSID_ComMemory>,
	public IDispatchImpl<IMemory, &IID_IMemory, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	typedef CComObject< CRecords > RecordsObject ;

	RecordsObject *m_records ;
	mem_engine::memory_pointer m_memory ;

public:
	CComMemory();

DECLARE_REGISTRY_RESOURCEID(IDR_COMMEMORY)

BEGIN_COM_MAP(CComMemory)
	COM_INTERFACE_ENTRY(IMemory)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	void set_memory(mem_engine::memory_pointer mem);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		HRESULT hr = RecordsObject::CreateInstance( &m_records ) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		m_records->AddRef() ;
		return hr;
	}

	void FinalRelease()
	{
		if ( m_records )
		{
			m_records->Release() ;
		}
	}

public:
	STDMETHOD(get_Records)(IRecords **pVal);
	STDMETHOD(AddRecord)(IRecord *pVal);

	STDMETHOD(Save)();
	STDMETHOD(SaveAs)(BSTR location);

	// Creator
	STDMETHOD(get_Creator)(BSTR* pVal);
	STDMETHOD(put_Creator)(BSTR source);
	// modified_by
	STDMETHOD(get_ModifiedBy)(BSTR* pVal);
	STDMETHOD(put_ModifiedBy)(BSTR source);
	// field
	STDMETHOD(get_Field)(BSTR* pVal);
	STDMETHOD(put_Field)(BSTR source);
	// client
	STDMETHOD(get_Client)(BSTR* pVal);
	STDMETHOD(put_Client)(BSTR source);
	// source_lang
	STDMETHOD(get_SourceLanguage)(BSTR* pVal);
	STDMETHOD(put_SourceLanguage)(BSTR source);
	// target_lang
	STDMETHOD(get_TargetLanguage)(BSTR* pVal);
	STDMETHOD(put_TargetLanguage)(BSTR source);
	// created_on
	STDMETHOD(get_CreatedOn)(DATE* pVal);
	STDMETHOD(put_CreatedOn)(DATE pVal);
	// ModifiedOn
	STDMETHOD(get_ModifiedOn)(DATE* pVal);
	STDMETHOD(put_ModifiedOn)(DATE pVal);

};

OBJECT_ENTRY_AUTO(__uuidof(ComMemory), CComMemory)
