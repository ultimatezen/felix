// Memories.h : Declaration of the CMemories

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"
#include "TranslationMemory.h"
#include "com2stl_coll.h"
#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler
#include "FelixModelInterface.h"
#include "ComMemory.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

typedef CAdapt< CComPtr<IMemory> > ComMemoryPtr ;

typedef IMemories						MemoryCollectionInterface;
typedef std::vector< ComMemoryPtr >		MemoryContainerType;
typedef IEnumVARIANT						EnumeratorInterface;
typedef VARIANT								EnumeratorExposedType;
typedef _CopyVariantFromAdaptItf<IMemory>	MemoryEnumeratorCopyType;
typedef CComEnumOnSTL
<     
EnumeratorInterface,
&__uuidof(EnumeratorInterface),
EnumeratorExposedType,
MemoryEnumeratorCopyType,
MemoryContainerType
> 
MemoryEnumeratorType;

typedef IMemory*						MemoryCollectionExposedType;
typedef _CopyItfFromAdaptItf<IMemory>	MemoryCollectionCopyType;

typedef ICollectionOnSTLImpl
<     
MemoryCollectionInterface,
MemoryContainerType,
MemoryCollectionExposedType,
MemoryCollectionCopyType,
MemoryEnumeratorType
> 
MemoryCollectionType;


// CMemories

class ATL_NO_VTABLE CMemories :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CMemories, &CLSID_Memories>,
	public IDispatchImpl<MemoryCollectionType, &IID_IMemories, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	FelixModelInterface *m_mem_model ;

public:
	CMemories() : m_mem_model(NULL)
	{
	}
	HRESULT set_model(FelixModelInterface *model);
	HRESULT load_memories(FelixModelInterface *model);

	HRESULT add_memory( mem_engine::memory_pointer mem );
DECLARE_REGISTRY_RESOURCEID(IDR_MEMORIES)


BEGIN_COM_MAP(CMemories)
	COM_INTERFACE_ENTRY(IMemories)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(Add)(IMemory **pVal);
	STDMETHOD(Load)(BSTR location, IMemory **pVal);

};

OBJECT_ENTRY_AUTO(__uuidof(Memories), CMemories)
