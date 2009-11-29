// GeneralProperties.h : Declaration of the CGeneralProperties

#ifndef __GENERALPROPERTIES_H_
#define __GENERALPROPERTIES_H_

#include "resource.h"       // main symbols
#include "mainfrm.h"

#include "PropertiesDlg.h"
#include "AutomationExceptionHandler.h"

/////////////////////////////////////////////////////////////////////////////
// CGeneralProperties
class ATL_NO_VTABLE CGeneralProperties : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CGeneralProperties, &CLSID_GeneralProperties>,
	public ISupportErrorInfo,
	public IDispatchImpl<IGeneralProperties, &IID_IGeneralProperties, &LIBID_FelixLib>
{
	app_props::properties_general m_props ;

public:
	CGeneralProperties()
	{
		m_props.read_from_registry() ;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GENERALPROPERTIES)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGeneralProperties)
	COM_INTERFACE_ENTRY(IGeneralProperties)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IGeneralProperties
public:
	STDMETHOD(get_Language)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_Language)(/*[in]*/ short newVal);

private:
	
	// Function name	: CApplication::get_mainframe
	// Description	    : 
	// Return type		: CMainFrame& 
	CMainFrame& get_mainframe()
	{
		return app::get_app() ;
	}
	

};

#endif //__GENERALPROPERTIES_H_
