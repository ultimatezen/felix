// MemoryProperties.h : Declaration of the CMemoryProperties

#ifndef __MEMORYPROPERTIES_H_
#define __MEMORYPROPERTIES_H_

#include "resource.h"       // main symbols
#include "PropertiesDlg.h"
#include "mainfrm.h"
#include "AutomationExceptionHandler.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoryProperties
class ATL_NO_VTABLE CMemoryProperties : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CMemoryProperties, &CLSID_MemoryProperties>,
	public ISupportErrorInfo,
	public IDispatchImpl<IMemoryProperties, &IID_IMemoryProperties, &LIBID_FelixLib>
{
		app_props::properties_memory		m_props ;
		app_props::properties_algorithm	m_algo_props ;
public:
	CMemoryProperties()
	{
		m_props.read_from_registry() ;
		m_algo_props.read_from_registry() ;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MEMORYPROPERTIES)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMemoryProperties)
	COM_INTERFACE_ENTRY(IMemoryProperties)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IMemoryProperties
public:
	STDMETHOD(get_MatchingAlgorithm)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_MatchingAlgorithm)(/*[in]*/ short newVal);
	
private:
	// Function name	: CApplication::get_mainframe
	// Description	    : 
	// Return type		: CMainFrame& 
	CMainFrame& get_mainframe()
	{
		return app::get_app() ;
	}
};

#endif //__MEMORYPROPERTIES_H_
