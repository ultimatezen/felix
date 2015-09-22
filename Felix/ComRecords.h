// ComRecords.h : Declaration of the CRecords

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"
#include "ComRecord.h"
#include "com2stl_coll.h"
#include "TranslationMemory.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


typedef CAdapt< CComPtr<IRecord> > ComRecordPtr ;

typedef IRecords						RecordCollectionInterface;
typedef std::vector< ComRecordPtr >		RecordContainerType;
typedef IEnumVARIANT						EnumeratorInterface;
typedef VARIANT								EnumeratorExposedType;
typedef _CopyVariantFromAdaptItf<IRecord>	RecordEnumeratorCopyType;
typedef CComEnumOnSTL
	<     
	EnumeratorInterface,
	&__uuidof(EnumeratorInterface),
	EnumeratorExposedType,
	RecordEnumeratorCopyType,
	RecordContainerType
	> 
	RecordEnumeratorType;

typedef IRecord*						RecordCollectionExposedType;
typedef _CopyItfFromAdaptItf<IRecord>	RecordCollectionCopyType;

typedef ICollectionOnSTLImpl
	<     
	RecordCollectionInterface,
	RecordContainerType,
	RecordCollectionExposedType,
	RecordCollectionCopyType,
	RecordEnumeratorType
	> 
	RecordCollectionType;

// CRecords

class ATL_NO_VTABLE CRecords :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CRecords, &CLSID_ComRecords>,
	public IDispatchImpl<RecordCollectionType, &IID_IRecords, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CRecords()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_COMRECORDS)


BEGIN_COM_MAP(CRecords)
	COM_INTERFACE_ENTRY(IRecords)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	HRESULT set_records(mem_engine::record_collection_type& records);

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

OBJECT_ENTRY_AUTO(__uuidof(ComRecords), CRecords)
