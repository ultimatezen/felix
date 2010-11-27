// MemoryWindow.h : Declaration of the CMemoryWindow

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"
#include "window_interface.h"

#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CMemoryWindow

class ATL_NO_VTABLE CMemoryWindow :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CMemoryWindow, &CLSID_MemoryWindow>,
	public IDispatchImpl<IMemoryWindow, &IID_IMemoryWindow, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	WindowInterface *m_window ;
	CMemoryWindow() : m_window(NULL)
	{
	}
	void set_window(WindowInterface *window)
	{
		m_window = window ;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MEMORYWINDOW)


BEGIN_COM_MAP(CMemoryWindow)
	COM_INTERFACE_ENTRY(IMemoryWindow)
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

	// height
	STDMETHOD(get_Height)(LONG* pVal);
	STDMETHOD(put_Height)(LONG newVal);

	// width
	STDMETHOD(get_Width)(LONG* pVal);
	STDMETHOD(put_Width)(LONG newVal);

	// left
	STDMETHOD(get_Left)(LONG* pVal);
	STDMETHOD(put_Left)(LONG newVal);

	// top
	STDMETHOD(get_Top)(LONG* pVal);
	STDMETHOD(put_Top)(LONG newVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MemoryWindow), CMemoryWindow)
