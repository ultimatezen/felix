// GlossaryWindow.h : Declaration of the CGlossaryDialog

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



//! This class will make the GlossaryWindowFrame class automatable via COM
class ATL_NO_VTABLE CGlossaryWindow :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGlossaryWindow, &CLSID_GlossaryWindow>,
	public IDispatchImpl<IGlossaryWindow, &IID_IGlossaryWindow, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CGlossaryWindow()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GLOSSARYWINDOW)


BEGIN_COM_MAP(CGlossaryWindow)
	COM_INTERFACE_ENTRY(IGlossaryWindow)
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

};

OBJECT_ENTRY_AUTO(__uuidof(GlossaryWindow), CGlossaryWindow)
