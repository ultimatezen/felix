// GlossaryProperties.h : Declaration of the CGlossaryProperties

#ifndef __GLOSSARYPROPERTIES_H_
#define __GLOSSARYPROPERTIES_H_

#include "resource.h"       // main symbols
#include "PropertiesDlg.h"
#include "mainfrm.h"
#include "AutomationExceptionHandler.h"

/////////////////////////////////////////////////////////////////////////////
// CGlossaryProperties
class ATL_NO_VTABLE CGlossaryProperties : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CGlossaryProperties, &CLSID_GlossaryProperties>,
	public ISupportErrorInfo,
	public IDispatchImpl<IGlossaryProperties, &IID_IGlossaryProperties, &LIBID_FelixLib>
{
		app_props::properties_glossary m_props ;
public:
	CGlossaryProperties()
	{
		m_props.read_from_registry() ;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GLOSSARYPROPERTIES)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGlossaryProperties)
	COM_INTERFACE_ENTRY(IGlossaryProperties)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// IGlossaryProperties
public:
	STDMETHOD(get_GetEntriesAsPlaintext)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_GetEntriesAsPlaintext)(/*[in]*/ BOOL newVal);

	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	
	// Function name	: CApplication::get_mainframe
	// Description	    : 
	// Return type		: CMainFrame& 
	CMainFrame& get_mainframe()
	{
		return app::get_app() ;
	}

};

inline STDMETHODIMP CGlossaryProperties::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (
		( (PLONG) &IID_IGlossaryProperties ) [0] == ( (PLONG) &riid ) [0] &&
		( (PLONG) &IID_IGlossaryProperties ) [1] == ( (PLONG) &riid ) [1] &&
		( (PLONG) &IID_IGlossaryProperties ) [2] == ( (PLONG) &riid ) [2] &&
		( (PLONG) &IID_IGlossaryProperties ) [3] == ( (PLONG) &riid ) [3] )
		return S_OK ;
	
	return S_FALSE ;
}

#endif //__GLOSSARYPROPERTIES_H_
