/*!
	@file App.h
	@brief Declaration of the CApp
	@date 2005/06/23
	Time: 15:48:40
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once
#include "resource.h"       // main symbols

#include "Felix.h"
#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler
#include "app2.h"
// CApp

/**
	@class CApp
	@brief COM interface for the application.
 */
class ATL_NO_VTABLE CApp : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CAutomationExceptionHandler<CApp, &CLSID_App>,
	public IDispatchImpl<IApp, &IID_IApp, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	CComObject< CApp2 > *m_App2 ;

public:
	CApp();

DECLARE_REGISTRY_RESOURCEID(IDR_APP)


BEGIN_COM_MAP(CApp)
	COM_INTERFACE_ENTRY(IApp)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		HRESULT hr = CComObject< CApp2 >::CreateInstance( &m_App2 ) ;
		if ( SUCCEEDED( hr ) )
		{
			m_App2->AddRef() ;
		}
		return hr ;
	}

	void FinalRelease()
	{
		if ( m_App2 )
		{
			m_App2->Release() ;
		}
	}

public:
	void wait_for_query();
	STDMETHOD(get_App2)(IApp2 **pVal);

	STDMETHOD(get_Visible)(VARIANT_BOOL* pVal);
	STDMETHOD(put_Visible)(VARIANT_BOOL newVal);

	STDMETHOD(Quit)(void);

	STDMETHOD(Lookup)(BSTR Query);
	STDMETHOD(LookupTrans)(BSTR Trans);

	STDMETHOD(Concordance)(BSTR Query);
	STDMETHOD(TransConcordance)(BSTR Trans);

	STDMETHOD(get_Score)(DOUBLE* pVal);

	STDMETHOD(get_Query)(BSTR* pVal);
	STDMETHOD(put_Query)(BSTR pVal);

	STDMETHOD(get_Trans)(BSTR* pVal);
	STDMETHOD(put_Trans)(BSTR pVal);

	STDMETHOD(Save)(void);
	STDMETHOD(NextTrans)(void);
	STDMETHOD(PrevTrans)(void);

	STDMETHOD(get_GlossMatch)(SHORT Index, BSTR* pVal);
	STDMETHOD(get_NumGlossMatches)(SHORT* pVal);

	STDMETHOD(LoadMemory)(BSTR MemoryName);
	STDMETHOD(LoadGlossary)(BSTR GlossaryName);
	STDMETHOD(ClearMemories)(void);
	STDMETHOD(ClearGlossaries)(void);

	STDMETHOD(get_ShowMarkup)(VARIANT_BOOL* pVal);
	STDMETHOD(put_ShowMarkup)(VARIANT_BOOL newVal);

	STDMETHOD(AddMemoryEntry)(BSTR Source, BSTR Trans, BSTR EntryContext);
	STDMETHOD(AddGlossaryEntry)(BSTR Source, BSTR Trans, BSTR EntryContext);
	STDMETHOD(DeleteMemEntry)(void);

	STDMETHOD(CorrectTrans)(BSTR Trans);
	STDMETHOD(get_NumMatches)( SHORT *index );

	STDMETHOD(LookupDeferred)(BSTR Query);
	STDMETHOD(LookupTransDeferred)(BSTR Trans);

};

OBJECT_ENTRY_AUTO(__uuidof(App), CApp)


//HKCR
//{
//	NoRemove AppID
//	{
//		{984A963D-5E88-442F-9239-BD34D2B695DE} = s 'Felix'
//			'Felix.EXE'
//		{
//			val AppID = s {984A963D-5E88-442F-9239-BD34D2B695DE}
//		}
//	}
//}
