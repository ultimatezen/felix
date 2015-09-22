/*!
	@file LocalizedServerAppModule.h
	@brief interface for the CLocalizedServerAppModule class.
	@date 2004/12/23
	Time: 11:37:36
	@author Ryan Ginstrom
	@dir C:/Programming/Programming Projects/Felix
 */

#pragma once

#include "stringex.h"

/**
	@struct appstate_mainframe
	@brief Persists the state of the MainFrame in the registry.
 */
struct appstate_mainframe
{
	BOOL m_is_toolbar_visible ;
	BOOL m_is_statusbar_visible ;
	WORD m_preferred_gui_lang ;
	BOOL m_rebar_has_linebreak ;
	 
	appstate_mainframe() : 
		m_is_toolbar_visible( TRUE ),
		m_is_statusbar_visible( TRUE ),
		m_rebar_has_linebreak(TRUE)
	{
		LANGID lid = ::GetUserDefaultLangID() ;
		m_preferred_gui_lang = (WORD)PRIMARYLANGID( lid ) ;
	}
	bool read_from_registry()
	{
		CRegKey rkParent;
		CRegKey rk;
		
		DWORD res = rkParent.Open(HKEY_CURRENT_USER, _T("Software\\AssistantSuite\\Felix") ) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		
		res = rk.Open(rkParent, _T("APPSTATE_MAINFRAME")) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		
		DWORD dwRet = 0 ;
		
		res = rk.QueryDWORDValue(_T("TOOLBAR_VISIBLE"), dwRet ) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		m_is_toolbar_visible = (BOOL)dwRet ;

		res = rk.QueryDWORDValue( _T("STATUSBAR_VISIBLE"), dwRet ) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		m_is_statusbar_visible = (BOOL)dwRet ;

		res = rk.QueryDWORDValue( _T("PREFERRED_GUILANG"), dwRet ) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		m_preferred_gui_lang = (WORD)dwRet ;


		res = rk.QueryDWORDValue(_T("REBAR_HAS_LINEBREAK"), dwRet ) ;
		if ( res == ERROR_SUCCESS )
		{
			m_rebar_has_linebreak = (BOOL)dwRet ;
		}

		rk.Close();
		rkParent.Close();
		return true ;
	}

	bool write_to_registry()
	{
		CRegKey rkParent;
		CRegKey rk;
		
		DWORD res =  rkParent.Create(HKEY_CURRENT_USER, _T("Software\\AssistantSuite\\Felix") ) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		res =  rk.Create(rkParent, _T("APPSTATE_MAINFRAME")) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		
		DWORD val;
		
		val = (DWORD)m_is_toolbar_visible ;
		res = rk.SetDWORDValue(  _T("TOOLBAR_VISIBLE"), val) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		
		val = (DWORD)m_is_statusbar_visible ;
		res = rk.SetDWORDValue(  _T("STATUSBAR_VISIBLE"), val) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		
		val = (DWORD)m_preferred_gui_lang ;
		res = rk.SetDWORDValue( _T("PREFERRED_GUILANG"), val) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
	
		val = (DWORD)m_rebar_has_linebreak ;
		res = rk.SetDWORDValue(  _T("REBAR_HAS_LINEBREAK"), val) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}

		rk.Close();
		rkParent.Close();
		return true ;
	}
};

/**
	@struct appstate_glossary
	@brief Persists application state in the registry.
 */
struct appstate_glossary
{
	BOOL m_is_statusbar_visible ;
	BOOL m_is_toolbar_visible ;

	appstate_glossary() : 
		m_is_statusbar_visible( TRUE ),
		m_is_toolbar_visible( TRUE )
	{}
	bool read_from_registry()
	{
		CRegKey rkParent;
		CRegKey rk;
		
		DWORD res = rkParent.Open(HKEY_CURRENT_USER, _T("Software\\AssistantSuite\\Felix") ) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		
		res = rk.Open(rkParent, _T("APPSTATE_GLOSSARY")) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		
		DWORD dwRet;
		
		res = rk.QueryDWORDValue( _T("STATUSBAR_VISIBLE"), dwRet ) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		m_is_statusbar_visible = (BOOL)dwRet ;
		
		res = rk.QueryDWORDValue( _T("TOOLBAR_VISIBLE"), dwRet ) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		m_is_toolbar_visible = (BOOL)dwRet ;

		rk.Close();
		rkParent.Close();
		return true ;
	}
	bool write_to_registry()
	{
		CRegKey rkParent;
		CRegKey rk;
		
		DWORD res =  rkParent.Create(HKEY_CURRENT_USER, _T("Software\\AssistantSuite\\Felix") ) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		res =  rk.Create(rkParent, _T("APPSTATE_GLOSSARY")) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}

		DWORD val = (DWORD)m_is_statusbar_visible ;
		res = rk.SetDWORDValue( _T("STATUSBAR_VISIBLE"), val) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}
		
		val = (DWORD)m_is_toolbar_visible ;
		res = rk.SetDWORDValue( _T("TOOLBAR_VISIBLE"), val) ;
		if ( res != ERROR_SUCCESS )
		{
			return false ;
		}

		rk.Close();
		rkParent.Close();
		return true ;
	}
};

/** A server app module that allows for switching between libraries of localized resources */
class CLocalizedServerAppModule : public CServerAppModule  
{
	TCHAR m_lib_name[_MAX_PATH] ; ///< Holds the name of the current resource library (DLL)

public:
	CLocalizedServerAppModule()
	{
		ZeroMemory(m_lib_name, sizeof(m_lib_name)) ;
	}

	/** Set the name of the resource library (DLL). 
	@return bool description. 
	@param lib_name parameter. 
	*/
	bool set_library(const tstring &lib_name) ;

	/** Initialize the module. 
	@return HRESULT description. 
	@param pObjMap parameter. 
	@param hInstance parameter. 
	@param pLibID parameter. 
	*/
	HRESULT Init(_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID = NULL) ;


	/** Get the name of the current resource library (DLL). 
	@return const tstring description. 
	*/
	const tstring get_library( )  ;

} ;

inline bool CLocalizedServerAppModule::set_library(const tstring &lib_name)
{
	// set the name
	_tcsncpy_s(m_lib_name, _MAX_PATH, (LPCTSTR)lib_name.c_str(), lib_name.size() ) ;

	// load the library
	HINSTANCE localized_instance = NULL ;
	localized_instance = ::LoadLibrary( m_lib_name ) ;


	ATLASSERT( localized_instance != NULL ) ;

	// Be defensive...
	if ( ! localized_instance )
		return false ;

	// Be defensive...
	if ( m_hInstResource  != NULL )
		::FreeLibrary( m_hInstResource  ) ;

	// Finally, we set the instance of the new resource library
	m_hInstResource  = localized_instance ;

	// we already returned false if localized_instance was NULL
	return true ;

}


inline HRESULT CLocalizedServerAppModule::Init(_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID )
{
	appstate_mainframe appstate ;
	
	appstate.read_from_registry() ;

	/** @todo Snarf up all the dlls in lang directory, get the language supported by each, and automatically select the best match.
	*/

	HINSTANCE localized_instance( NULL ) ;

	TCHAR dir[_MAX_PATH] ;
	::GetModuleFileName( hInstance, dir, MAX_PATH ) ;
	::PathRemoveFileSpec( dir ) ;

	if ( appstate.m_preferred_gui_lang == LANG_JAPANESE )
	{
		::PathCombine( m_lib_name, dir, _T("lang\\JpnResource.dll") );	
		localized_instance = ::LoadLibrary( m_lib_name ) ;
	}
	
	if ( localized_instance == NULL )
	{
		::PathCombine( m_lib_name, dir, _T("lang\\EngResource.dll") );	
		localized_instance = ::LoadLibrary( m_lib_name ) ;
	}

	ATLASSERT( localized_instance != NULL ) ;
	
	HRESULT hr = CServerAppModule::Init( pObjMap, hInstance, pLibID ) ;
	if ( localized_instance != NULL )
		m_hInstResource = localized_instance ;
	return hr ;
}


inline const tstring CLocalizedServerAppModule::get_library( ) 
{ 
	return tstring( m_lib_name ) ; 
}

