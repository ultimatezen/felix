#pragma once

// Store the data in a vector of std::strings
template <typename T> 
struct _CopyVariantFromAdaptItf : public _Copy<VARIANT>
{
	static HRESULT copy(VARIANT* p1, const CAdapt< CComPtr<T> >* p2)
	{
		HRESULT hr = p2->m_T->QueryInterface(IID_IDispatch, (void**)&p1->pdispVal);

		if (SUCCEEDED(hr))
		{
			p1->vt = VT_DISPATCH;
			return hr ;
		}

		hr = p2->m_T->QueryInterface(IID_IUnknown, (void**)&p1->punkVal);
		if (SUCCEEDED(hr))
		{
			p1->vt = VT_UNKNOWN;
		}

		return hr;
	}
};

template <typename T> 
struct _CopyItfFromAdaptItf : public _CopyInterface< T >
{
	static HRESULT copy(T** p1, const CAdapt< CComPtr<T> >* p2)
	{
		*p1 = p2->m_T ;
		if (*p1) return (*p1)->AddRef(), S_OK;
		return E_POINTER;
	}
};
