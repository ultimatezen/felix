// GlossaryWindow.h : Declaration of the CGlossaryWindow

#ifndef __GLOSSARYWINDOW_H_
#define __GLOSSARYWINDOW_H_

#include "resource.h"       // main symbols
#include "TranslationMemory.h"
#include "GlossaryDialog.h"
#include "AutomationExceptionHandler.h"
#include "Memories.h"


/////////////////////////////////////////////////////////////////////////////
// CGlossaryWindow
class ATL_NO_VTABLE CGlossaryWindow : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CGlossaryWindow, &CLSID_GlossaryWindow>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CGlossaryWindow>,
	public IDispatchImpl<IGlossaryWindow, &IID_IGlossaryWindow, &LIBID_FelixLib>
{
	gloss_window_pointer	m_glossary_window ;

public:
	CGlossaryWindow()
	{
	}
	void set_glossary_window( gloss_window_pointer glossary_window )
	{
	   m_glossary_window = glossary_window ;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GLOSSARYWINDOW)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGlossaryWindow)
	COM_INTERFACE_ENTRY(IGlossaryWindow)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CGlossaryWindow)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IGlossaryWindow
public:
	STDMETHOD(get_Visible)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_Visible)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_Glossaries)(/*[out, retval]*/ IMemories * *pVal);
	STDMETHOD(get_HWND)(/*[out, retval]*/ long *pVal);
private:

	void create_memories(ATL::CComObject< CMemories >* the_memories);
};

#endif //__GLOSSARYWINDOW_H_
