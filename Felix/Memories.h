// Memories.h : Declaration of the CMemories

#ifndef __MEMORIES_H_
#define __MEMORIES_H_

#include "resource.h"       // main symbols
#include "TranslationMemory.h"
#include "AutomationExceptionHandler.h"

/////////////////////////////////////////////////////////////////////////////
// CMemories
class ATL_NO_VTABLE CMemories : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CMemories, &CLSID_Memories>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CMemories>,
	public IDispatchImpl<IMemories, &IID_IMemories, &LIBID_FelixLib>
{

	memory_list *m_memories ;

public:
	CMemories()
		: m_memories( NULL ) 
	{
	}

	void set_memories( memory_list &memories )
	{
		m_memories = &memories ;
	}
	bool remove_memory_by_name( const CString &name )
	{
		for ( memory_iterator pos = m_memories->begin() ; pos != m_memories->end() ; ++pos )
		{
			CString cmp_name = (*pos)->get_location(); 
			
			if ( 0 == name.CompareNoCase( cmp_name ) )
			{
				m_memories->erase( pos ) ;
				return true ;
			}
		}
		
		return false ;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MEMORIES)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMemories)
	COM_INTERFACE_ENTRY(IMemories)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CMemories)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IMemories
public:
	memory_list & get_memories();
	STDMETHOD(Add)(/*[out, retval]*/ IMemory **Item);
	STDMETHOD(Remove)(/*[in]*/ VARIANT item);
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ LPUNKNOWN *pVal);
	STDMETHOD(get_Item)(long index, /*[out, retval]*/ IMemory **pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

#endif //__MEMORIES_H_
