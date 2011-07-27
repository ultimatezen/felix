// ***************************************************************
//  DispatchWrapper   version:  1.0	date: 09/23/2007
//  -------------------------------------------------------------
//  Wraps an IDispatch pointer for use in late binding
//  -------------------------------------------------------------
//  Copyright (C) 2007 Ryan Ginstrom - All Rights Reserved
// ***************************************************************
//  MIT License
// ***************************************************************
#pragma once

#include "atlcomcli.h" // CComPtr etc.
#include "comdef.h" // _com_error
#include <vector>
#include <map>
#include <string>
#include "Exceptions.h"

class CDispatchWrapper
{
	typedef std::vector< VARIANT > var_vec ;
	typedef std::map< std::wstring, DISPID > map_type ;

	// cache DISPIDs for performance
	map_type m_dispids ;
	wstring  m_server_name ;
public:
	// We wrap this
	IDispatch *m_app ;


	// constructors
	CDispatchWrapper( LPCWSTR app_name ) : 
		m_server_name(app_name),
		m_app(NULL)
	{
		CLSID clsid;
		ATLVERIFY(SUCCEEDED(CLSIDFromProgID(app_name, &clsid)));
		this->create(clsid) ;
	}
	CDispatchWrapper(const CDispatchWrapper &app) : 
		m_app(NULL)
	{
		app.m_app->QueryInterface(IID_IDispatch, 
			CDispatchWrapper::out_ptr( &m_app ));
	}
	CDispatchWrapper(IDispatch *app) : 
		m_app(NULL)
	{
		m_app = app ;
		m_app->AddRef() ;
	}
	

	~CDispatchWrapper()
	{
		try
		{
			if(m_app)
			{
				m_app->Release() ;
				m_app = NULL ;
			}
		}
		catch (except::CSWException &e) 
		{
			e ;
			TRACE(e.format_message_for_message_box()) ;
			ATLTRACE("*** CDispatchWrapper :: Failed to set m_app to NULL! (%p)\n", this) ;
		}
		catch(...)
		{
			TRACE(m_server_name) ;
			ATLTRACE("*** CDispatchWrapper :: Failed to set m_app to NULL! (%p)\n", this) ;
		}
	}

	CComPtr< IDispatch > get_dispatch()
	{
		return m_app ;
	}

	// properties
	void prop_put( LPCWSTR NAME,
					const CComVariant PROP )
	{
		Wrap( DISPATCH_PROPERTYPUT, NULL, NAME, PROP ) ;
	}

	CComVariant prop_get( LPCWSTR NAME )
	{
		CComVariant out_arg ;
		Wrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &out_arg, NAME ) ;
		return out_arg ;
	}
	CComVariant prop_get( LPCWSTR NAME, const CComVariant in_arg )
	{
		CComVariant out_arg ;
		Wrap( DISPATCH_PROPERTYGET|DISPATCH_METHOD, &out_arg, NAME,  in_arg ) ;
		return out_arg ;
	}

	// methods
	CComVariant method( LPCWSTR NAME ) 
	{
		CComVariant out_arg ;
		Wrap( DISPATCH_METHOD, &out_arg, NAME ) ;
		return out_arg ;
	}
	CComVariant method( LPCWSTR NAME, 
				const CComVariant in_arg ) 
	{
		CComVariant out_arg ;
		Wrap( DISPATCH_METHOD, &out_arg, NAME,  in_arg ) ;
		return out_arg ;
	}
	CComVariant method( LPCWSTR NAME, 
				const CComVariant in_arg1, 
				const CComVariant in_arg2 ) 
	{
		CComVariant out_arg ;
		Wrap( DISPATCH_METHOD, &out_arg, NAME,  in_arg1, in_arg2 ) ;
		return out_arg ;
	}
	CComVariant method( LPCWSTR NAME, 
				const CComVariant in_arg1, 
				const CComVariant in_arg2, 
				const CComVariant in_arg3 ) 
	{
		CComVariant out_arg ;
		Wrap( DISPATCH_METHOD, &out_arg, NAME,  in_arg1, in_arg2, in_arg3 ) ;
		return out_arg ;
	}

	DISPID get_dispid(LPCWSTR name)
	{
		// Cache this for performance
		map_type::iterator pos = m_dispids.find(name) ;
		if ( pos != m_dispids.end())
		{
			return pos->second ;
		}

		DISPID dispID ;

		// Get DISPID for name passed...
		HRESULT hr = m_app->GetIDsOfNames(IID_NULL, 
											const_cast<LPOLESTR*>(&name), 
											1, 
											LOCALE_USER_DEFAULT, 
											&dispID) ;
		if ( FAILED(hr))
		{
			throw _com_error(hr) ;
		}

		m_dispids[name] = dispID ;
		return dispID ;
	}
protected:
	HRESULT Wrap( WORD autoType, VARIANT *result, LPCWSTR name ) 
	{
		var_vec vars ;
		return this->Wrap( autoType, result, name,  vars ) ;
	}
	HRESULT Wrap( WORD autoType, 
						VARIANT *result, 
						LPCWSTR name, 
						const VARIANT &in_arg ) 
	{
		var_vec vars ;
		vars.resize(1) ;
		vars[0] = in_arg ;
		return this->Wrap( autoType, result, name,  vars ) ;
	}
	HRESULT Wrap( WORD autoType, 
						VARIANT *result, 
						LPCWSTR name, 
						const VARIANT &in_arg1, 
						const VARIANT &in_arg2 ) 
	{
		var_vec vars ;
		vars.resize(2) ;
		vars[0] = in_arg2 ;
		vars[1] = in_arg1 ;
		return this->Wrap( autoType, result, name,  vars ) ;
	}
	HRESULT Wrap( WORD autoType, 
						VARIANT *result, 
						LPCWSTR name, 
						const VARIANT &in_arg1, 
						const VARIANT &in_arg2, 
						const VARIANT &in_arg3 ) 
	{
		var_vec vars ;
		vars.resize(3) ;
		vars[0] = in_arg3 ;
		vars[1] = in_arg2 ;
		vars[2] = in_arg1 ;
		return this->Wrap( autoType, result, name,  vars ) ;
	}

	// implementation details

	static void HandleSpecialCaseForPropertyPuts(WORD autoType, 
													DISPPARAMS *dp, 
													DISPID *dispidNamed)
	{
		if(autoType & DISPATCH_PROPERTYPUT)
		{
			dp->cNamedArgs = 1;
			dp->rgdispidNamedArgs = dispidNamed;
		}
	}
	void GetDp(DISPPARAMS *dp, 
				WORD autoType, 
				var_vec &vars, 
				DISPID *dispidNamed)
	{
		// Build DISPPARAMS
		dp->cArgs = static_cast< UINT >( vars.size() ) ;
		if ( ! vars.empty() )
		{
			dp->rgvarg = &vars[0] ;
		}

		HandleSpecialCaseForPropertyPuts(autoType, dp, dispidNamed);
	}
	HRESULT Wrap(WORD autoType,
				VARIANT *result, 
				LPCWSTR name, 
				var_vec &vars ) 
	{

		// Get DISPID for name passed...
		DISPID dispID = this->get_dispid(name) ;

		// Variables used...
		DISPPARAMS dp = { NULL, NULL, 0, 0 } ;
		DISPID dispidNamed = DISPID_PROPERTYPUT ;
		GetDp( &dp, autoType, vars, &dispidNamed );
		// Make the call
		HRESULT hr = S_OK ;
		EXCEPINFO except_info = {0} ;
		UINT err_num = 0 ;

		hr = m_app->Invoke(dispID, 
			IID_NULL, 
			LOCALE_SYSTEM_DEFAULT,
			autoType, 
			&dp, result, 
			&except_info, 
			&err_num ) ;

		if ( FAILED( hr ) ) 
		{
			throw _com_error(hr) ;
		}

		return hr ;
	}

	void create( CLSID clsid )
	{
		const HRESULT hr = ::CoCreateInstance(clsid, 
			NULL, CLSCTX_LOCAL_SERVER|CLSCTX_INPROC_SERVER, 
			IID_IDispatch, 
			CDispatchWrapper::out_ptr( &m_app ) ) ;

		if ( FAILED(hr))
		{
			throw _com_error(hr) ;
		}
	}

	// Safe cast of a pointer to type void**
	template< typename I >
	static void ** out_ptr( I **ppi )
	{
		ATLASSERT( ppi != NULL ) ;
		return reinterpret_cast< void ** >(ppi) ;
	}

};
