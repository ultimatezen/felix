// Properties.h : Declaration of the CProperties

#ifndef __PROPERTIES_H_
#define __PROPERTIES_H_

#include "resource.h"       // main symbols

#include "MemoryProperties.h"
#include "GlossaryProperties.h"
#include "GeneralProperties.h"
#include "AutomationExceptionHandler.h"

/////////////////////////////////////////////////////////////////////////////
// CProperties
class ATL_NO_VTABLE CProperties : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CProperties, &CLSID_Properties>,
	public ISupportErrorInfo,
	public IDispatchImpl<IProperties, &IID_IProperties, &LIBID_FelixLib>
{
public:
	CProperties()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROPERTIES)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CProperties)
	COM_INTERFACE_ENTRY(IProperties)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IProperties
public:
	STDMETHOD(get_GeneralProperties)(/*[out, retval]*/ IGeneralProperties* *pVal);
	STDMETHOD(get_GlossaryProperties)(/*[out, retval]*/ IGlossaryProperties* *pVal);
	STDMETHOD(get_MemoryProperties)(/*[out, retval]*/ IMemoryProperties* *pVal);

private:
	
};

#endif //__PROPERTIES_H_
