// record.cpp: implementation of the record class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "record.h"

namespace mem_engine
{

static const wstring date_created_tag	= L"date_created" ;	
static const wstring last_modified_tag	= L"last_modified" ;	
static const wstring context_tag		= L"context" ;		

static const wstring refcount_tag		= L"ref_count" ;

static wstring m_user_name ;

void set_record_username(const wstring username)
{
	m_user_name = username ;
}
wstring get_record_username()
{
	return m_user_name ;
}



}
