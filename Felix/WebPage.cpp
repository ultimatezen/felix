/////////////////////////////////////////////////////////////////
//             By Eugene Khodakovsky                           //
//                  April,2002                                 //
//             Eugene@cpplab.com                               //
//            Last Update: April, 2002                         //
/////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WebPage.h"

namespace html
{

#ifndef CHECK_POINTER
#define CHECK_POINTER(p)\
	ATLASSERT(p != NULL);\
	if(p == NULL)\
	{\
	ShowError("NULL pointer");\
	return false;\
}
#endif
	const CString GetSystemErrorMessage(DWORD dwError)
	{
		CString strError;
		LPTSTR lpBuffer;

		if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,  dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR) &lpBuffer, 0, NULL))

		{
			strError = "FormatMessage Native Error" ;
		}
		else
		{
			strError = lpBuffer;
			LocalFree(lpBuffer);
		}
		return strError;
	}

	CString GetNextToken(CString& strSrc, const CString strDelim,bool bTrim, bool bFindOneOf)
	{
		CString strToken;
		const int idx = bFindOneOf? strSrc.FindOneOf(strDelim) : strSrc.Find(strDelim);
		if(idx != -1)
		{
			strToken  = strSrc.Left(idx);
			strSrc = strSrc.Right(strSrc.GetLength() - (idx + 1) );
		}
		else
		{
			strToken = strSrc;
			strSrc.Empty();
		}
		if(bTrim)
		{
			strToken.TrimLeft();
			strToken.TrimRight();
		}
		return strToken;
	}

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	CWebPage::CWebPage()
	{
	}

	CWebPage::~CWebPage()
	{

	}

	void CWebPage::SetDocument(document_ptr doc)
	{
		m_spDoc = doc ;
	}

	bool CWebPage::GetJScript(CComPtr<IDispatch>& spDisp)
	{

		CHECK_POINTER(m_spDoc);
		HRESULT hr = m_spDoc->get_Script(&spDisp);
		ATLASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	bool CWebPage::GetJScripts(collection_ptr& spColl)
	{
		CHECK_POINTER(m_spDoc);
		HRESULT hr = m_spDoc->get_scripts(&spColl);
		ATLASSERT(SUCCEEDED(hr));
		return SUCCEEDED(hr);
	}

	bool CWebPage::CallJScript(const CString strFunc,CComVariant* pVarResult)
	{
		std::vector<CString> paramArray;
		return CallJScript(strFunc,paramArray,pVarResult);
	}

	bool CWebPage::CallJScript(const CString strFunc,const CString strArg1,CComVariant* pVarResult)
	{
		std::vector<CString> paramArray;
		paramArray.push_back(strArg1);
		return CallJScript(strFunc,paramArray,pVarResult);
	}

	bool CWebPage::CallJScript(const CString strFunc,const CString strArg1,const CString strArg2,CComVariant* pVarResult)
	{
		std::vector<CString> paramArray;
		paramArray.push_back(strArg1);
		paramArray.push_back(strArg2);
		return CallJScript(strFunc,paramArray,pVarResult);
	}

	bool CWebPage::CallJScript(const CString strFunc,const CString strArg1,const CString strArg2,const CString strArg3,CComVariant* pVarResult)
	{
		std::vector<CString> paramArray;
		paramArray.push_back(strArg1);
		paramArray.push_back(strArg2);
		paramArray.push_back(strArg3);
		return CallJScript(strFunc,paramArray,pVarResult);
	}

	bool CWebPage::CallJScript(const CString strFunc, const std::vector<CString>& paramArray,CComVariant* pVarResult)
	{

		CComPtr<IDispatch> spScript;
		if(!GetJScript(spScript))
		{
			ShowError("Cannot GetScript");
			return false;
		}
		CComBSTR bstrMember(strFunc);
		DISPID dispid = NULL;
		HRESULT hr = spScript->GetIDsOfNames(IID_NULL,&bstrMember,1,
			LOCALE_SYSTEM_DEFAULT,&dispid);
		if(FAILED(hr))
		{
			ShowError(GetSystemErrorMessage(hr));
			return false;
		}

		const int arraySize = paramArray.size();

		DISPPARAMS dispparams;
		memset(&dispparams, 0, sizeof dispparams);
		dispparams.cArgs = arraySize;
		dispparams.rgvarg = new VARIANT[dispparams.cArgs];

		for( int i = 0; i < arraySize; i++)
		{
			CComBSTR bstr = paramArray.at(arraySize - 1 - i); // back reading
			if(! SUCCEEDED(bstr.CopyTo(&dispparams.rgvarg[i].bstrVal)))
			{
				logging::log_warn("CWebPage::CallJScript -- Failed to copy BSTR value") ;
			}
			dispparams.rgvarg[i].vt = VT_BSTR;
		}
		dispparams.cNamedArgs = 0;

		EXCEPINFO excepInfo;
		memset(&excepInfo, 0, sizeof excepInfo);
		CComVariant vaResult;
		UINT nArgErr = (UINT)-1;  // initialize to invalid arg

		hr = spScript->Invoke(dispid,IID_NULL,0,
			DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

		delete [] dispparams.rgvarg;
		if(FAILED(hr))
		{
			ShowError(GetSystemErrorMessage(hr));
			return false;
		}

		if(pVarResult)
		{
			*pVarResult = vaResult;
		}
		return true;
	}

	// returned java script function name, input string is truncating
	CString CWebPage::ScanJScript(CString& strAText, std::vector<CString>& args)
	{
		args.clear();
		CString strDelim(" \n\r\t"),strSrc(strAText);
		bool bFound = false;
		while(!strSrc.IsEmpty())
		{
			CString strStart = GetNextToken(strSrc,strDelim);
			if(strStart == "function")
			{
				bFound = true;
				break;
			}
			if(strStart == "/*")
			{
				// Skip comments
				while(!strSrc.IsEmpty())
				{
					CString strStop = GetNextToken(strSrc,strDelim);
					if(strStop == "*/")
					{
						break;
					}
				}
			}
		}

		if(!bFound)
		{
			return "";
		}

		CString strFunc = GetNextToken(strSrc,"(",true);
		CString strArgs = GetNextToken(strSrc,")",true);

		// Parse arguments
		CString strArg;
		while(!(strArg = GetNextToken(strArgs,",")).IsEmpty())
			args.push_back(strArg);

		strAText= strSrc;
		return strFunc;
	}

	void CWebPage::ShowError( CString lpszText )
	{
		m_strError = "JSCall Error:\n" + CString(lpszText);
	}

	LPDISPATCH CWebPage::GetHtmlDocument() const
	{
		return m_spDoc;
	}

	const CString CWebPage::GetLastError() const
	{
		return m_strError;
	}
}