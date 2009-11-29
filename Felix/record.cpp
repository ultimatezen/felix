// record.cpp: implementation of the record class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "record.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

namespace memory_engine
{

static const wstring date_created_tag	= L"date_created" ;	
static const wstring last_modified_tag	= L"last_modified" ;	
static const wstring context_tag		= L"context" ;		

static const wstring refcount_tag		= L"ref_count" ;

static wstring m_user_name ;

void set_record_username(const wstring &username)
{
	m_user_name = username ;
}
wstring get_record_username()
{
	return m_user_name ;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool record_local::remove_item(const wstring &key)
{

	for ( record_data_iterator pos = data_begin() ; pos != data_end() ; ++pos )
	{
		if ( pos->first == key )
		{
			m_record_data.erase( pos ) ;
			modify() ;
			return true ;
		}
	}

	return false ;
}

// ************************************
// *
// * record_local member functions
// *
// *************************************

// Constructors

record_local::record_local( ) : 
	m_is_validated(false), 
	m_reliability( 0u ),
	m_refcount( 0u ),
	m_id(0u)
{
	create() ;
} 



// set record information

// create
bool record_local::create(  )
{
	misc_wrappers::date local_time ;
	local_time.set_to_local_time() ;
	this->set_created(local_time) ;

	return true ;
}


// modify
// set the last-modified value to the current date-time
bool record_local::modify (  )
{
	misc_wrappers::date local_time ;
	local_time.set_to_local_time() ;
	this->set_modified(local_time) ;

	return true ;
}


// record_local::item_exists
bool record_local::item_exists( const wstring &key ) const
{
	return ( get_item( key ).empty() == false ) ;
}

// record_local::get_item
const wstring record_local::get_item( const wstring &key ) const
{
	TRUE_ENFORCE( key.empty() == false, "Key must not be empty" ) ;
	TRUE_ENFORCE( key != L"source", "Key must not be 'source'" ) ;
	TRUE_ENFORCE( key != L"trans", "Key must not be 'trans'" ) ;
	TRUE_ENFORCE( key != L"context", "Key must not be 'context'" ) ;

	record_data_map::const_iterator value = m_record_data.find( key ) ;
	if ( value != m_record_data.end() )
		return value->second ;

	return wstring() ;
}

/**
 *	get/set reliability
 */

size_t record_local::get_reliability() const
{
	return m_reliability ;
}

void record_local::set_reliability( const size_t reliability )  
{
	size_t rel = reliability ;
	if ( rel > 9 )
	{
		rel = 9 ;
	}
	if ( rel != m_reliability ) 
	{
		modify() ;
	}

	m_reliability = rel ;
}


/**
 *	get/set validation
 */


 bool record_local::is_validated() const 
{ 
	return m_is_validated ; 
}

// void record_local::set_validated( const bool validated ) 
void record_local::set_validated_on( ) 
{
	if( m_is_validated == false )
	{
		modify() ;
	}
	m_is_validated = true ;
}
void record_local::set_validated_off( ) 
{
	if( m_is_validated == true )
	{
		modify() ;
	}
	m_is_validated = false ;
}



// record_local::get_created
const misc_wrappers::date &record_local::get_created() const  
{
	return m_created ;
}

// record_local::get_modified
const misc_wrappers::date &record_local::get_modified() const  
{
	return m_modified ;
}


// void record_local::set_item( const wstring &key, const wstring &value )  
void record_local::set_item( const wstring &key, const wstring &value )  
{
	TRUE_ENFORCE( key.empty() == false, "Key must not be empty" ) ;
	TRUE_ENFORCE( key != L"source", "Key must not be 'source'" ) ;
	TRUE_ENFORCE( key != L"trans", "Key must not be 'trans'" ) ;
	TRUE_ENFORCE( key != L"context", "Key must not be 'context'" ) ;
	TRUE_ENFORCE( key != L"ref_count", "Key must not be 'ref_count'" ) ;

	// setting a key to an empty value is the same as
	// deleting it
	if ( value.empty() )
	{
		modify() ;
		remove_item( key ) ;
	}
	else
	{
		if ( ! item_exists( key ) || m_record_data[key] != value ) 
		{
			modify() ;
		}
		m_record_data[key] = value ;
	}
}

// void record_local::set_source( const wstring &rich_source )  
void record_local::set_source( const wstring source )  
{
	m_source.set_value(source) ;
	modify() ;
}

// void record_local::set_trans( const wstring &rich_trans )  
void record_local::set_trans( const wstring trans  )  
{
	m_trans.set_value(trans) ;
	modify() ;
}

// void record_local::set_context( const wstring &context ) 
void record_local::set_context( const wstring context ) 
{
	m_context.set_value(context) ;
	modify() ;
}


// void record_local::set_created( const wstring &created ) 
void record_local::set_created( const wstring &created ) 
{
	m_created = created ;
	if ( m_modified < m_created )
	{
		m_modified = created ;
	}
	ATLASSERT ( m_created.wYear < 2050 ) ; 
	ATLASSERT ( m_created.wYear > 2000 ) ; 
	ATLASSERT ( m_created.wMonth < 13 ) ; 
	ATLASSERT ( m_created.wDay < 366 ) ; 
	ATLASSERT ( m_created.wDayOfWeek < 8 ) ; 
	ATLASSERT ( m_created.wHour < 25 ) ; 
	ATLASSERT ( m_created.wMinute < 61 ) ; 
	ATLASSERT ( m_created.wSecond < 61 ) ; 
	ATLASSERT ( m_created.wMilliseconds < 1001 ) ; 
}

// void record_local::set_modified( const wstring &modified )  
void record_local::set_modified( const wstring &modified )  
{
	m_modified = modified ;
	if ( m_modified < m_created )
	{
		m_created = m_modified  ;
	}
	ATLASSERT ( m_modified.wYear < 2050 ) ; 
	ATLASSERT ( m_modified.wYear > 2000 ) ; 
	ATLASSERT ( m_modified.wMonth < 13 ) ; 
	ATLASSERT ( m_modified.wDay < 366 ) ; 
	ATLASSERT ( m_modified.wDayOfWeek < 8 ) ; 
	ATLASSERT ( m_modified.wHour < 25 ) ; 
	ATLASSERT ( m_modified.wMinute < 61 ) ; 
	ATLASSERT ( m_modified.wSecond < 61 ) ; 
	ATLASSERT ( m_modified.wMilliseconds < 1001 ) ; 
}

// void record_local::set_created( const misc_wrappers::date &created ) 
void record_local::set_created( const misc_wrappers::date &created ) 
{
	m_created = created ;
	this->set_modified(created) ;

	ATLASSERT ( m_created.wYear < 2050 ) ; 
	ATLASSERT ( m_created.wMonth < 13 ) ; 
	ATLASSERT ( m_created.wDay < 366 ) ; 
	ATLASSERT ( m_created.wDayOfWeek < 8 ) ; 
	ATLASSERT ( m_created.wHour < 25 ) ; 
	ATLASSERT ( m_created.wMinute < 61 ) ; 
	ATLASSERT ( m_created.wSecond < 61 ) ; 
	ATLASSERT ( m_created.wMilliseconds < 1001 ) ; 

	this->set_creator(get_record_username()) ;
}

// void record_local::set_modified( const misc_wrappers::date &modified )  
void record_local::set_modified( const misc_wrappers::date &modified )  
{
	m_modified = modified ;
	if ( m_modified < m_created )
	{
		m_created = m_modified  ;
	}
	ATLASSERT ( m_modified.wYear < 2050 ) ; 
	ATLASSERT ( m_modified.wMonth < 13 ) ; 
	ATLASSERT ( m_modified.wDay < 366 ) ; 
	ATLASSERT ( m_modified.wDayOfWeek < 8 ) ; 
	ATLASSERT ( m_modified.wHour < 25 ) ; 
	ATLASSERT ( m_modified.wMinute < 61 ) ; 
	ATLASSERT ( m_modified.wSecond < 61 ) ; 
	ATLASSERT ( m_modified.wMilliseconds < 1001 ) ; 

	this->set_modified_by(get_record_username()) ;

}

// void record_local::copy_from_self( const record_local &rec )
// commonizes copy code used for constructor and assignment
record_pointer record_local::copy_from_self( record_pointer rec )
{
	rec->set_source( this->get_source_rich() ) ;
	rec->set_trans( this->get_trans_rich() ) ;
	rec->set_context( this->get_context_rich() ) ;			

	// dates
	rec->set_created(this->m_created) ;
	rec->set_modified(this->m_modified) ;

	// other info
	rec->set_validated(this->is_validated()) ;
	rec->set_reliability(this->get_reliability()) ;
	rec->set_refcount(this->get_refcount()) ;

	// extra strings
	record_data_map tmp( this->m_record_data ) ;
	rec->get_data_map().swap( tmp ) ;

	return rec ;
}
void record_local::internal_copy( const record_pointer rec )
{
	set_source( rec->get_source_rich() ) ;
	set_trans( rec->get_trans_rich() ) ;
	set_context( rec->get_context_rich() ) ;			

	// dates
	m_created = rec->get_created() ;
	m_modified = rec->get_modified() ;

	// other info
	m_is_validated = rec->is_validated() ;
	m_reliability = rec->get_reliability() ;
	m_refcount = rec->get_refcount() ;
	set_id(rec->get_id()) ;

	// extra strings
	record_data_map tmp( rec->get_data_map() ) ;
	m_record_data.swap( tmp ) ;
}

// Function name	: record_local::is_valid_record
// Description	    : 
// Return type		: bool 
bool record_local::is_valid_record()
{
	if ( this->get_source_plain().empty() )
		return false ;
	if ( this->get_trans_plain().empty() )
		return false ;
	return true ;
}

// refcount related


size_t record_local::get_refcount() const 
{
	return m_refcount ;
}

void record_local::set_refcount(size_t ref_count)
{
	if ( ref_count != m_refcount ) 
	{
		modify() ;
	}
	m_refcount = ref_count ;
}



std::map< wstring, wstring >::iterator record_local::data_begin() 
{ 
	return m_record_data.begin() ; 
}
std::map< wstring, wstring >::iterator record_local::data_end() 
{
	return m_record_data.end() ;
}


void record_local::clear_user_strings()
{
	m_record_data.clear() ;
}

// source

const wstring & record_local::get_source_rich() const
{
	return m_source.rich() ;
}

const wstring record_local::get_source_plain()
{
	return m_source.plain() ;
}

const wstring & record_local::get_source_cmp() const
{
	return m_source.cmp() ;
}

// trans

const wstring & record_local::get_trans_rich() const
{
	return m_trans.rich() ;
}

const wstring record_local::get_trans_plain()
{
	return m_trans.plain() ;
}

const wstring & record_local::get_trans_cmp() const
{
	return m_trans.cmp() ;
}

// context 

const wstring & record_local::get_context_rich() const
{
	return m_context.rich() ;
}

const wstring record_local::get_context_plain()
{
	return m_context.plain() ;
}

const wstring & record_local::get_context_cmp() const
{
	return m_context.cmp() ;
}

void record_local::set_cmp_maker( const CmpMaker maker )
{
	m_source.set_cmp_maker(maker) ;
	m_trans.set_cmp_maker(maker) ;
	m_context.set_cmp_maker(maker) ;
}

void record_local::set_creator( const wstring &creator )
{
	m_creator = creator ;
}

void record_local::set_modified_by( const wstring &modified )
{
	m_modified_by = modified ;
}

wstring record_local::get_creator()
{
	if (m_creator.empty())
	{
		m_creator = get_record_username() ;
	}
	return m_creator ;
}

wstring record_local::get_modified_by()
{
	if (m_modified_by.empty())
	{
		m_modified_by = get_record_username() ;
	}
	return m_modified_by ;
}
}