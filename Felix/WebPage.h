// WebPage.h: interface for the CWebPage class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <atlbase.h>
#include <Mshtml.h>
#include "HtmlDocument.h"	// wrappers for html doc & others

namespace html
{

	class CWebPage  
	{
	public:
		CWebPage();
		virtual ~CWebPage();

		void SetDocument(document_ptr doc);
		LPDISPATCH GetHtmlDocument() const;
		const CString GetLastError() const;
		bool GetJScript(CComPtr<IDispatch>& spDisp);
		bool GetJScripts(collection_ptr& spColl);

		CString ScanJScript(CString& strAText, std::vector<CString>& args);

		bool CallJScript(const CString strFunc,CComVariant* pVarResult = NULL);
		bool CallJScript(const CString strFunc,const CString strArg1,CComVariant* pVarResult = NULL);
		bool CallJScript(const CString strFunc,const CString strArg1,const CString strArg2,CComVariant* pVarResult = NULL);
		bool CallJScript(const CString strFunc,const CString strArg1,const CString strArg2,const CString strArg3,CComVariant* pVarResult = NULL);
		bool CallJScript(const CString strFunc,const std::vector<CString>& paramArray,CComVariant* pVarResult = NULL);

	protected:
		void ShowError(CString lpszText);

	protected:

		document_ptr	m_spDoc;
		CString	m_strError;

	};

	CString GetNextToken(CString& strSrc, 
		const CString strDelim,
		bool bTrim = false, 
		bool bFindOneOf = true);
	const CString GetSystemErrorMessage(DWORD dwError) ;
}