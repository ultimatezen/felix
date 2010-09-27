// Word2HtmlParser.h : Declaration of the CWord2HtmlParser

#pragma once
#include "resource.h"       // main symbols

#include "AddIn.h"
#include "app_state.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CWord2HtmlParser

class ATL_NO_VTABLE CWord2HtmlParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWord2HtmlParser, &CLSID_Word2HtmlParser>,
	public IDispatchImpl<IWord2HtmlParser, &IID_IWord2HtmlParser, &LIBID_WordAssistLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	bool m_plaintext ;
	app_state	*m_properties ;
	Abbreviations		m_abbreviations ;
public:
	void set_properties(app_state *properties)
	{
		m_properties = properties ;
	}
	void set_plaintext(bool plaintext)
	{
		m_plaintext = plaintext ;
	}
	CWord2HtmlParser() :
		m_plaintext(false)
	{
		m_abbreviations.load(get_config_text(_T("abbreviations.txt"))) ;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_WORD2HTMLPARSER)


BEGIN_COM_MAP(CWord2HtmlParser)
	COM_INTERFACE_ENTRY(IWord2HtmlParser)
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
	STDMETHOD(RangeToHtml)(/*[in]*/ IDispatch *word_range, /*[out, retval]*/ BSTR* html_text);

};

OBJECT_ENTRY_AUTO(__uuidof(Word2HtmlParser), CWord2HtmlParser)
