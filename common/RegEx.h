/*!
	@file RegEx.h
	@brief Interface file for CMatchCollection class.
	@date 2005/06/25
	Time: 13:19:34
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

#include "Exceptions.h"
#include "stringconversions.h"

#import "C:/dev/cpp/Common DLLs/vbscript.tlb" \
	no_namespace \
	named_guids

#pragma warning( disable:4310 ) // cast truncates constant value

/**
	@class CMatchCollection
	@brief Wraps a collection of matches (regex).
 */
class CMatchCollection
{
	typedef CComPtr< IDispatch > dispatch_ptr ;
	typedef CComPtr< IMatchCollection2 > match_collection_ptr ;
	typedef CComPtr< IMatch2 > match_ptr ;

	match_collection_ptr m_collection ;

public:
	CMatchCollection() {}
	CMatchCollection( const CMatchCollection &copy ) : m_collection( copy.m_collection ) {}
 	CMatchCollection( const match_collection_ptr &copy ) : m_collection( copy ) {}
	CMatchCollection& operator=( const CMatchCollection &copy ) 
	{ 
		m_collection = ( copy.m_collection ) ;
	}
	CMatchCollection& operator=( const match_collection_ptr &copy ) 
	{ 
		m_collection = ( copy ) ;
	}
	match_ptr get_item( int i )
	{
		dispatch_ptr dispatch ;
		COM_ENFORCE( m_collection->get_Item ( i, &dispatch ), CString("Failed to get item ") + int2tstring( i ).c_str() ) ;
		match_ptr match ;
		COM_ENFORCE( dispatch->QueryInterface( &match ), _T("Failed to query interface for IMatch pointer") ) ;
		return match ;
	}
	long get_count( )
	{
		long count ;
		COM_ENFORCE( m_collection->get_Count ( &count ), _T("Failed to get count for match collection") ) ;
		return count ;
	}

} ;

/**
	@class CRegEx  
	@brief Wrapper for VB Regex COM component.
 */
class CRegEx  
{
	typedef CComPtr< IDispatch > dispatch_ptr ;
	typedef CComPtr< IMatchCollection2 > match_collection_ptr ;

	CComPtr<IRegExp2> spRegExp ;

public:

	// Function name	: CRegEx
	// Description	    : 
	// Return type		: 
	CRegEx()
	{
		COM_ENFORCE( spRegExp.CoCreateInstance(CLSID_RegExp), TEXT("Failed to initialize regular expression component") ) ;
		if ( spRegExp.p == NULL ) // interface failed
			throw except::CComException( TEXT("Regular expression component not found"), E_NOINTERFACE ) ;

	}


	// Function name	: set_multiline
	// Description	    : 
	// Return type		: void 
	// Argument         : bool Multiline
	void set_multiline( bool Multiline ) 
	{
		BANNER( "CRegEx::set_multiline" ) ;

		VARIANT_BOOL vbMultiline = ( Multiline ? VARIANT_TRUE : VARIANT_FALSE ) ;
		COM_ENFORCE( spRegExp->put_Multiline( vbMultiline ), _T("Failed to set Multiline parameter") ) ;

	}

	// Function name	: get_multiline
	// Description	    : 
	// Return type		: bool 
	bool get_multiline(  ) 
	{
		BANNER( "CRegEx::get_multiline" ) ;

		VARIANT_BOOL vbMultiline ;
		COM_ENFORCE( spRegExp->get_Multiline( &vbMultiline ), _T("Failed to get Multiline parameter") ) ;
		return !! vbMultiline ;
	}

	// Function name	: set_global
	// Description	    : 
	// Return type		: void 
	// Argument         : bool global
	void set_global( bool global ) 
	{

		BANNER( "CRegEx::set_global" ) ;

		VARIANT_BOOL vbGlobal = ( global ? VARIANT_TRUE : VARIANT_FALSE ) ;
		COM_ENFORCE( spRegExp->put_Global( vbGlobal ), _T("Failed to set global parameter") ) ;

	}

	// Function name	: get_global
	// Description	    : 
	// Return type		: bool 
	bool get_global(  ) 
	{
		VARIANT_BOOL vbGlobal ;
		COM_ENFORCE( spRegExp->get_Global( &vbGlobal ), _T("Failed to get global parameter") ) ;
		return !! vbGlobal ;
	}

	// Function name	: set_ignore_case
	// Description	    : 
	// Return type		: void 
	// Argument         : bool ignore_case
	void set_ignore_case( bool ignore_case )
	{
		VARIANT_BOOL vbIgnoreCase = ( ignore_case ? VARIANT_TRUE : VARIANT_FALSE ) ;
		COM_ENFORCE( spRegExp->put_IgnoreCase( vbIgnoreCase ), _T("Failed to set case matching parameter") ) ;

	}

	// Function name	: get_ignore_case
	// Description	    : 
	// Return type		: bool 
	bool get_ignore_case(  ) 
	{
		BANNER( "CRegEx::get_ignore_case" ) ;

		VARIANT_BOOL vbIgnoreCase ;
		COM_ENFORCE( spRegExp->get_IgnoreCase( &vbIgnoreCase ), _T("Failed to get case matching parameter") ) ;
		return !! vbIgnoreCase ;
	}

	// Function name	: set_pattern
	// Description	    : 
	// Return type		: void 
	// Argument         : const wstring &pattern
	void set_pattern( const wstring &pattern )
	{
		_bstr_t pat = string2BSTR( pattern ) ;
		COM_ENFORCE( spRegExp->put_Pattern( pat ), _T("Failed to set search pattern in regular expression component") ) ;
	}

	// Function name	: get_pattern
	// Description	    : 
	// Return type		: wstring 
	wstring get_pattern( )
	{
		BANNER( "CRegEx::get_pattern" ) ;

		_bstr_t pattern ;
		COM_ENFORCE( spRegExp->get_Pattern( pattern.GetAddress() ), _T("Failed to set search pattern in regular expression component") ) ;

		return BSTR2wstring( pattern ) ;
	}

	// Function name	: execute
	// Description	    : 
	// Return type		: match_collection_ptr 
	// Argument         : const wstring &test_string
	match_collection_ptr execute( const wstring &test_string ) 
	{
		BANNER( "CRegEx::execute" ) ;

		_bstr_t sts( test_string.c_str() ) ;
		CComPtr< IDispatch > dispatch  ;
		COM_ENFORCE( spRegExp->raw_Execute(sts, &dispatch), _T("Search failed in regular expression component") ) ;
		match_collection_ptr matches ;
		COM_ENFORCE( dispatch->QueryInterface( &matches ), _T("Failed to query interface for pointer to IMatchCollection") ) ;

		return matches ;

	}

	// Function name	: find_in
	// Description	    : 
	// Return type		: bool 
	// Argument         : const wstring &string_to_search
	bool test( const wstring &string_to_search )
	{
		_bstr_t sts( string_to_search.c_str() ) ;
		VARIANT_BOOL vbMatch ;
		COM_ENFORCE( spRegExp->raw_Test(sts, &vbMatch), _T("Search failed in regular expression component") ) ;

		return !! vbMatch ;
	}

	// Function name	: replace
	// Description	    : 
	// Return type		: void 
	// Argument         :  const wstring &source_string
	// Argument         : const wstring &replace_string
	// Argument         : wstring &dest_string
	wstring replace( const wstring &source_string, const wstring &replace_string )
	{
		BANNER( "CRegEx::replace" ) ;

		_bstr_t source_BSTR( source_string.c_str() ) ;
		_variant_t replace_var( replace_string.c_str() ) ;

		_bstr_t dest_BSTR ;
		COM_ENFORCE( spRegExp->raw_Replace(source_BSTR, replace_var, (BSTR *)&dest_BSTR ), _T("Search failed in regular expression component") ) ;

		return BSTR2wstring( dest_BSTR ) ;
	}
};

