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
	CDispatchWrapper( LPCWSTR app_name );
	CDispatchWrapper(const CDispatchWrapper &app);
	CDispatchWrapper(IDispatch *app);
	

	~CDispatchWrapper();

	CComPtr< IDispatch > get_dispatch();

	// properties
	void prop_put( LPCWSTR NAME,
					const CComVariant PROP );

	CComVariant prop_get( LPCWSTR NAME );
	CComVariant prop_get( LPCWSTR NAME, const CComVariant in_arg );

	// methods
	CComVariant method( LPCWSTR NAME );
	CComVariant method( LPCWSTR NAME, 
				const CComVariant in_arg );
	CComVariant method( LPCWSTR NAME, 
				const CComVariant in_arg1, 
				const CComVariant in_arg2 );
	CComVariant method( LPCWSTR NAME, 
				const CComVariant in_arg1, 
				const CComVariant in_arg2, 
				const CComVariant in_arg3 );

	DISPID get_dispid(LPCWSTR name);
protected:
	HRESULT Wrap( WORD autoType, VARIANT *result, LPCWSTR name );
	HRESULT Wrap( WORD autoType, 
						VARIANT *result, 
						LPCWSTR name, 
						const VARIANT &in_arg );
	HRESULT Wrap( WORD autoType, 
						VARIANT *result, 
						LPCWSTR name, 
						const VARIANT &in_arg1, 
						const VARIANT &in_arg2 );
	HRESULT Wrap( WORD autoType, 
						VARIANT *result, 
						LPCWSTR name, 
						const VARIANT &in_arg1, 
						const VARIANT &in_arg2, 
						const VARIANT &in_arg3 );

	// implementation details

	static void HandleSpecialCaseForPropertyPuts(WORD autoType, 
													DISPPARAMS *dp, 
													DISPID *dispidNamed);
	void GetDp(DISPPARAMS *dp, 
				WORD autoType, 
				var_vec &vars, 
				DISPID *dispidNamed);
	HRESULT Wrap(WORD autoType,
				VARIANT *result, 
				LPCWSTR name, 
				var_vec &vars );

	void create( CLSID clsid );

	// Safe cast of a pointer to type void**
	template< typename I >
	static void ** out_ptr( I **ppi );

};
