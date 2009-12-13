// ComRecord.h : Declaration of the CRecord

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"
#include "_IRecordEvents_CP.h"
#include "record.h"
#include "record_local.h"

#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CRecord

class ATL_NO_VTABLE CRecord :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CRecord, &CLSID_Record>,
//	public CComCoClass<CRecord, &CLSID_Record>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CRecord>,
	public CProxy_IRecordEvents<CRecord>,
	public IDispatchImpl<IRecord, &IID_IRecord, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	memory_engine::record_pointer m_record ;
public:

	void set_record(memory_engine::record_pointer record)
	{
		m_record = record ;
	}
	CRecord() : m_record(new memory_engine::record_local)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_RECORD)


BEGIN_COM_MAP(CRecord)
	COM_INTERFACE_ENTRY(IRecord)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CRecord)
	CONNECTION_POINT_ENTRY(__uuidof(_IRecordEvents))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(get_Source)(BSTR* pVal);
	STDMETHOD(put_Source)(BSTR pVal);

	STDMETHOD(get_Trans)(BSTR* pVal);
	STDMETHOD(put_Trans)(BSTR pVal);

	STDMETHOD(get_Id)(ULONG* pVal);
	STDMETHOD(put_Id)(ULONG pVal);

};

OBJECT_ENTRY_AUTO(__uuidof(Record), CRecord)
