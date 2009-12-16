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

namespace mem_engine
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

/************************************************************************/
/* translation_record                                                   */
/************************************************************************/

// set_validated
void translation_record::set_validated( const bool setting )
{
	setting ? this->set_validated_on() : this->set_validated_off() ;
}

// increment_reliability
void translation_record::increment_reliability()
{
	this->set_reliability( this->get_reliability() + 1u ) ;
}

// increment_refcount
void translation_record::increment_refcount()
{
	this->set_refcount(this->get_refcount() + 1u) ;
}

// reset_refcount
// If we actually change the refcount, then we update
// the last-modified value.
void translation_record::reset_refcount()
{
	if ( 0u < this->get_refcount() ) 
	{
		this->modify() ;
		this->set_refcount(0u) ;
	}
}

}
