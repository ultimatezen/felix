/*!
	@file RegistryStuff.h
	@brief Header file for registry map class
	@date 2004/12/29
	Time: 11:01:42
	@author Ryan Ginstrom
 */

#pragma once

#include "DebugUtilities.h"

namespace props
{

/**
	@class CRegMap
	@brief Registry map taken from concept of DDX map.
 */
class CRegMap
{
	DWORD	res ;
	CRegKey	rkParent ;
	CRegKey	rk ;

public:
	virtual ~CRegMap(){}
	bool open_reg_for_read( HKEY reg_store, LPCTSTR reg_entry, LPCTSTR specific_entry )
	{
		res = rkParent.Open(reg_store, reg_entry ) ;						
		if ( res != ERROR_SUCCESS ) 
		{ 
			return false ; 
		}	
		res = rk.Open(rkParent, specific_entry ) ;							
		if ( res != ERROR_SUCCESS ) 
		{ 
			return false ; 
		}	
		return true ;
	}
	bool open_reg_for_write( HKEY reg_store, LPCTSTR reg_entry, LPCTSTR specific_entry )
	{
		res = rkParent.Create(reg_store, reg_entry ) ;						
		ATLASSERT( res == ERROR_SUCCESS  ) ;
		if ( res != ERROR_SUCCESS ) 
		{ 
			return false ; 
		}	
		res = rk.Create(rkParent, specific_entry ) ;							
		ATLASSERT( res == ERROR_SUCCESS  ) ;
		if ( res != ERROR_SUCCESS ) 
		{ 
			return false ; 
		}	
		return true ;
	}

	bool query_value( LPCTSTR reg_name, DWORD &dwRet )
	{
		DWORD res = 0 ;
		res = rk.QueryDWORDValue( reg_name, dwRet ) ;
		return ( res == ERROR_SUCCESS ) ;
	}
	bool query_value_string( LPCTSTR reg_name, LPTSTR ret_val, ULONG len )
	{
		DWORD res = 0 ;
		ULONG len_ret = len ;
		res = rk.QueryStringValue( reg_name, ret_val, &len_ret ) ;
		return ( res == ERROR_SUCCESS ) ;
	}

	bool write_value( LPCTSTR reg_name, DWORD dwRet )
	{
		DWORD res = 0 ;
		res = rk.SetDWORDValue( reg_name, dwRet ) ;
		ATLASSERT( res == ERROR_SUCCESS  ) ;
		return ( res == ERROR_SUCCESS ) ;
	}
	bool write_value_string( LPCTSTR reg_name, LPCTSTR val )
	{
		DWORD res = 0 ;
		res = rk.SetStringValue( reg_name, val ) ;
		ATLASSERT( res == ERROR_SUCCESS  ) ;
		return ( res == ERROR_SUCCESS ) ;
	}

	bool close_registry()
	{
		LONG success = ERROR_SUCCESS ;
		
		success = rk.Close() ;
		ATLASSERT( success == ERROR_SUCCESS  ) ;

		success = rkParent.Close() ;
		ATLASSERT( success == ERROR_SUCCESS  ) ;

		return ( success == ERROR_SUCCESS ) ;
	}

protected:
	bool registry_failure()
	{
		close_registry() ;
		return false ;
	}
};

}

#define BEGIN_REGISTRY_MAP( reg_store, reg_entry, specific_entry ) \
	bool read_from_registry() { return DO_REG_DATA_EXCHANGE( true ) ; } \
	bool write_to_registry() { return DO_REG_DATA_EXCHANGE( false ) ; } \
	bool DO_REG_DATA_EXCHANGE( bool is_read ) { \
	if ( is_read ) { if ( ! open_reg_for_read( reg_store, reg_entry, specific_entry ) ) return false ; } \
	else { if ( ! open_reg_for_write( reg_store, reg_entry, specific_entry ) ) return false ; } 	\
	DWORD dwRet ; dwRet = 0 ;

#define REG_ENTRY_BOOL( reg_name, val )	\
	if( is_read ) { if ( ! query_value( reg_name, dwRet ) ) return registry_failure() ; val = (BOOL)dwRet ; } \
	else { if ( ! write_value( reg_name, (DWORD)val ) ) return registry_failure() ;  }

#define REG_ENTRY_INT( reg_name, val ) \
	if( is_read ) { if ( ! query_value( reg_name, dwRet ) ) return registry_failure() ;  val = (int)dwRet ; } \
	else { if ( ! write_value( reg_name, (DWORD)val ) ) return registry_failure() ;  }

#define REG_ENTRY_STRING( reg_name, val, sz ) \
	if( is_read ) { if ( ! query_value_string( reg_name, val, sz ) ) return registry_failure() ;   } \
	else { if ( ! write_value_string( reg_name, val ) ) return registry_failure() ;  }

#define END_REGISTRY_MAP return close_registry() ; }
	
