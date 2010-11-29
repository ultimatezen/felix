// FelixObj.cpp: implementation of the CFelix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FelixObj.h"
#include "Exceptions.h"
#include "StringConversions.h"	// convert to/from std strings

#ifdef _DEBUG

#include "easyunit/testharness.h"

using namespace easyunit;

#endif // #ifdef _DEBUG


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif



CFelix::CFelix() :
	COleAutoWrapper(L"Felix.App" )
{
	put_visible_on() ;
}

bool CFelix::is_visible(void)
{
	_variant_t vis = prop_get( L"Visible" ) ;
	return VARIANT_FALSE != vis.boolVal ;
}

void CFelix::put_visible_on(void)
{
	_variant_t vis( VARIANT_TRUE ) ;
	prop_put( L"Visible", vis ) ; 
}

void CFelix::put_visible_off(void)
{
	_variant_t vis( VARIANT_FALSE ) ;
	prop_put( L"Visible", vis ) ; 
}

int CFelix::get_num_gloss_hits()
{
	_variant_t num = prop_get( L"NumGlossMatches" ) ;
	return num.intVal ;
}

_bstr_t CFelix::get_gloss_entry( short index )
{
	_variant_t i(index) ;
	_variant_t entry = prop_get( L"GlossMatch", i ) ;
	return entry.bstrVal ;
}

void CFelix::save()
{
	method( L"Save" ) ;
}
void CFelix::quit()
{
	method( L"Quit" ) ;
}

void CFelix::next_translation() 
{
	method( L"NextTrans" ) ;
}
void CFelix::previous_translation() 
{
	method( L"PrevTrans" ) ;
}

void CFelix::delete_trans()
{
	method( L"DeleteMemEntry" ) ;
}

void CFelix::concordance( _bstr_t query ) 
{
	_variant_t q( query ) ;
	method( L"Concordance", q ) ;
}
void CFelix::translation_concordance( _bstr_t query )
{
	_variant_t q( query ) ;
	method( L"TransConcordance", q ) ;
}

void CFelix::lookup( _bstr_t query )
{
	_variant_t q( query ) ;
	method( L"Lookup", q ) ;
}
void CFelix::lookup_trans( _bstr_t query )
{
	_variant_t q( query ) ;
	method( L"LookupTrans", q ) ;
}

_bstr_t CFelix::get_trans()
{
	_variant_t trans = prop_get( L"Trans" ) ;
	return trans.bstrVal ;
}
_bstr_t CFelix::get_query()
{
	_variant_t query = prop_get( L"Query" ) ;
	return query.bstrVal ;
}

double CFelix::get_score()
{
	_variant_t score = prop_get( L"Score" ) ;
	return score.dblVal ;
}

void CFelix::set_trans(_bstr_t setting)
{
	_variant_t val = setting ;
	prop_put( L"Trans", val ) ;
}
void CFelix::set_query(_bstr_t setting)
{
	_variant_t val = setting ;
	prop_put( L"Query", val ) ;
}

void CFelix::correct_trans( _bstr_t trans )
{
	_variant_t t( trans ) ;
	method( L"CorrectTrans", t ) ;
}


void CFelix::add_entry_to_glossary( _bstr_t query, _bstr_t trans, _bstr_t context )
{
	_variant_t s( query ) ;
	_variant_t t( trans ) ;
	_variant_t c( context ) ;
	method( L"AddGlossaryEntry", c, t, s ) ;
}
/*

//////////////////////////////////////////////////////////////////////
// CMemory
//////////////////////////////////////////////////////////////////////

// size
long CMemory::size()
{
	long mem_size ;
	TRANS_ENFORCE( m_ta_obj->get_Count( &mem_size ), _T("Failed to set memory location") ) ;
	return mem_size ;
}

long CMemory::get_num_gloss_matches( const _bstr_t &query )
{
	long num_matches ;
	TRANS_ENFORCE( m_ta_obj->get_NumGlossaryMatches( query, &num_matches ), _T("Failed to set memory location") ) ;
	return num_matches ;
}
double CMemory::get_best_score( const _bstr_t &query )
{
	double best_score ;
	TRANS_ENFORCE( m_ta_obj->get_BestScore( query, &best_score ), _T("Failed to set memory location") ) ;
	return best_score ;
}

void CMemory::get_reliability_statistics( long *low, long *high, double *ave )
{
	TRANS_ENFORCE( m_ta_obj->GetReliabilityStatistics( high, low, ave ), _T("Failed to get reliability statistics") ) ;
}

_bstr_t CMemory::get_percent_validated_string()
{
	_bstr_t val_string ;
	TRANS_ENFORCE( m_ta_obj->get_PercentValidatedStr( &val_string ), _T("Failed to get percent validated string") ) ;
	return val_string ;
}
// source/target
_bstr_t CMemory::get_source_language()
{
	_bstr_t source_language ;
	TRANS_ENFORCE( m_ta_obj->get_SourceLanguage( &source_language ), _T("Failed to set get source language") ) ;
	return source_language ;
}
_bstr_t CMemory::get_target_language()
{
	_bstr_t target_language ;
	TRANS_ENFORCE( m_ta_obj->get_TargetLanguage( &target_language ), _T("Failed to get target language") ) ;
	return target_language ;
}
// name

// get
_bstr_t CMemory::get_name( )
{
	_bstr_t memory_filename ;
	TRANS_ENFORCE( m_ta_obj->get_Name( &memory_filename ), _T("Failed to set memory location") ) ;
	return memory_filename ;
}

// add
record_ptr CMemory::add_record( const _bstr_t src, const _bstr_t trans, const _bstr_t context )
{
	record_ptr rec ;
	TRANS_ENFORCE( m_ta_obj->raw_Add( src, trans, context, &rec ), _T("Failed to set memory location") ) ;
	return rec ;
}

// put
void CMemory::set_name( const _bstr_t &memory_filename )
{
	TRANS_ENFORCE( m_ta_obj->put_Name( memory_filename ), _T("Failed to set memory location") ) ;
}

// load

void CMemory::save( )
{
	TRANS_ENFORCE( m_ta_obj->Save( ), _T("Felix.Memory: Call to Load failed") ) ;
}

// load
void CMemory::load( const _bstr_t &memory_filename )
{
	TRANS_ENFORCE( m_ta_obj->Load( _bstr_t( memory_filename ) ), _T("Felix.Memory: Call to Load failed") ) ;
}

// load_header
void CMemory::load_header( const _bstr_t &memory_filename )
{
	TRANS_ENFORCE( m_ta_obj->LoadHeader( _bstr_t( memory_filename ) ), _T("Felix.Memory: Call to LoadHeader failed") ) ;
}

// date_created

date CMemory::get_date_created()
{
	DATE var_date ;
	TRANS_ENFORCE( m_ta_obj->get_DateCreated( &var_date ), _T("Felix.Memory: Failed to get date created") ) ;

	date sys_date( var_date ) ;
	return sys_date ;
}

// created_by

_bstr_t CMemory::get_created_by()
{
	_bstr_t created_by ;
	TRANS_ENFORCE( m_ta_obj->get_Creator( &created_by ), _T("Felix.Memory: Failed to get client name") ) ;
	
	return created_by ;
}

void CMemory::set_memory_on( )
{
	TRANS_ENFORCE( m_ta_obj->put_IsMemory( VARIANT_TRUE ), _T("Felix.Memory: Failed to set memory type") ) ;
}

void CMemory::set_memory_off( )
{
	TRANS_ENFORCE( m_ta_obj->put_IsMemory( VARIANT_FALSE ), _T("Felix.Memory: Failed to set memory type") ) ;
}

bool CMemory::is_memory()
{
	long is_mem ;
	TRANS_ENFORCE( m_ta_obj->get_IsMemory( &is_mem ), _T("Felix.Memory: Failed to get memory type") ) ;

	return ( !! is_mem ) ;
}


// client

// set 
void CMemory::set_client( const _bstr_t &client )
{
	TRANS_ENFORCE( m_ta_obj->put_Client( client ), _T("Felix.Memory: Failed to set client name") ) ;
}
// get
_bstr_t CMemory::get_client()
{
	_bstr_t client ;
	TRANS_ENFORCE( m_ta_obj->get_Client( &client ), _T("Felix.Memory: Failed to get client name") ) ;
	
	return client ;
}

// field

// set 
void CMemory::set_field( const _bstr_t &field )
{
	TRANS_ENFORCE( m_ta_obj->put_Field( field ), _T("Felix.Memory: Failed to set client name") ) ;
}
// get
_bstr_t CMemory::get_field()
{
	_bstr_t field ;
	TRANS_ENFORCE( m_ta_obj->get_Field( &field ), _T("Felix.Memory: Failed to get client name") ) ;
	
	return  field ;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Function name	: CFelix::lookup
// Description	    : 
// Return type		: void 
// Argument         : const wstring &query
void CFelix::lookup(const wstring &query)
{
	BANNER( "CFelix::lookup" ) ;

	TRACE( query ) ;
	_bstr_t bquery( query.c_str() ) ;

	lookup( bquery ) ;
}


// Function name	: CFelix::get_translation
// Description	    : 
// Return type		: wstring 
wstring CFelix::get_translation()
{
	BANNER( "CFelix::get_translation" ) ;

	ensure_instantiated() ;

	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	_bstr_t translation ;
	TRANS_ENFORCE( m_ta_obj->get_Translation( &translation ), _T("Failed to retrieve translation") ) ;

	return BSTR2wstring( translation ) ;
}

wstring CFelix::get_query()
{
	_bstr_t query ;

	get_query( query ) ;

	return BSTR2wstring( query ) ;
}

void CFelix::get_query( _bstr_t &query )
{
	BANNER( "CFelix::get_query" ) ;

	ensure_instantiated() ;

	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	TRANS_ENFORCE( m_ta_obj->get_Query( &query ), _T("Failed to retrieve query") ) ;
}

void CFelix::add_entry_to_glossary( const _bstr_t &source, const _bstr_t &trans, const _bstr_t &context )
{
	BANNER( "CFelix::get_query" ) ;

	ensure_instantiated() ;

	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	TRANS_ENFORCE( m_ta_obj->raw_AddEntryToGlossary( source, trans, context ), _T("Failed to add glossary entry") ) ;

}

// Function name	: CFelix::set_translation
// Description	    : 
// Return type		: void 
// Argument         : const wstring &translation
void CFelix::set_translation(const wstring &translation)
{
	BANNER( "CFelix::set_translation" ) ;

	ATLASSERT( translation.empty() == false ) ;

	_bstr_t btrans( translation.c_str() );

	set_translation( btrans ) ;

}


// Function name	: CFelix::get_gloss_entry
// Description	    : 
// Return type		: wstring 
// Argument         : short entry_num
wstring CFelix::get_gloss_entry(short entry_num)
{
	BANNER( "CFelix::get_gloss_entry" ) ;

	ATLASSERT( entry_num >= 0 ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	_bstr_t entry ;

	TRANS_ENFORCE( m_ta_obj->get_GlossEntry( entry_num, &entry ), _T("Failed to get glossary entry") ) ;

	return BSTR2wstring( entry ) ;
}


// Function name	: CFelix::get_num_gloss_matches
// Description	    : 
// Return type		: short 
short CFelix::get_num_gloss_matches()
{
	BANNER( "CFelix::get_num_gloss_matches" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	short num_matches ;

	TRANS_ENFORCE( m_ta_obj->get_NumGlosaryMatches( &num_matches ), _T("Failed to get number of glossary matches") ) ;

	return num_matches ;
}


// Function name	: CFelix::next_translation
// Description	    : 
// Return type		: void 
void CFelix::next_translation()
{
	BANNER( "CFelix::next_translation" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	TRANS_ENFORCE( m_ta_obj->NextTranslation(), _T("Failed to navigate to next translation") ) ;
}


// Function name	: CFelix::previous_translation
// Description	    : 
// Return type		: void 
void CFelix::previous_translation()
{
	BANNER( "CFelix::previous_translation" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	TRANS_ENFORCE( m_ta_obj->PrevTranslation(), _T("Failed to navigate to previous translation") ) ;
}


// Function name	: CFelix::save_memory
// Description	    : 
// Return type		: void 
void CFelix::save_memory()
{
	BANNER( "CFelix::save_memory" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	TRANS_ENFORCE( m_ta_obj->Save(), _T("Failed to save memory") ) ;
}


// Function name	: CFelix::add_gloss_entry
// Description	    : 
// Return type		: void 
// Argument         : const wstring &entry
void CFelix::add_gloss_entry(const wstring &entry)
{
	ATLASSERT( entry.empty() == false ) ;

	_bstr_t translation = string2BSTR( entry ) ;
	
	add_gloss_entry( translation ) ;
}


// Function name	: CFelix::delete_current_translation
// Description	    : 
// Return type		: void 
void CFelix::delete_current_translation()
{
	BANNER( "CFelix::delete_current_translation" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	TRANS_ENFORCE( m_ta_obj->DeleteTranslation(), _T("Failed to save memory") ) ;

}


// Function name	: CFelix::get_glossary_hits
// Description	    : 
// Return type		: void 
// Argument         : hit_set &hits
void CFelix::get_glossary_hits(std::set< string > &hits)
{
	BANNER( "CFelix::get_glossary_hits" ) ;

	TRACE( hits.size() ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	short num_gloss_matches = get_num_gloss_matches() ;

	wstring entry ;

	for( short i=0 ; i<num_gloss_matches ; ++i )
	{
		entry = get_gloss_entry( i ) ;
		hits.insert( string2string( entry ) ) ;
	}
}
void CFelix::get_glossary_hits(std::set< wstring > &hits)
{
	BANNER( "CFelix::get_glossary_hits" ) ;
	
	TRACE( hits.size() ) ;
	
	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}
	
	short num_gloss_matches = get_num_gloss_matches() ;
	
	wstring entry ;
	
	for( short i=0 ; i<num_gloss_matches ; ++i )
	{
		entry = get_gloss_entry( i ) ;
		hits.insert( entry ) ;
	}
}


// Function name	: CFelix::get_num_matches
// Description	    : 
// Return type		: long 
long CFelix::get_num_matches()
{
	BANNER( "CFelix::get_num_matches" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	long num_matches ;

	TRANS_ENFORCE( m_ta_obj->get_NumMatches( &num_matches ), _T("Failed to get number of memory matches") ) ;

	return num_matches ;
	
}


// Function name	: CFelix::get_concordance
// Description	    : 
// Return type		: void 
// Argument         : const wstring &query
void CFelix::get_concordance(const _bstr_t &query)
{
	BANNER( "CFelix::get_concordance" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	TRACE( query ) ;

	TRANS_ENFORCE( m_ta_obj->raw_Concordance( query ), _T("Failed to get concordance") ) ;
}


// Function name	: CFelix::get_trans_concordance
// Description	    : 
// Return type		: void 
// Argument         : const wstring &query
void CFelix::get_trans_concordance(const _bstr_t &query)
{
	BANNER( "CFelix::get_trans_concordance" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}
	
	TRACE( query ) ;
	
	TRANS_ENFORCE( m_ta_obj->raw_TranslationConcordance( query ), _T("Failed to get concordance") ) ;

}


// Function name	: CFelix::lookup_translation
// Description	    : 
// Return type		: void 
// Argument         : const wstring &query
void CFelix::lookup_translation(const wstring &query)
{
	_bstr_t bquery( query.c_str() ) ;

	lookup_translation( bquery ) ;
}


// Function name	: CFelix::correct_translation
// Description	    : 
// Return type		: void 
// Argument         : const _bstr_t &trans
void CFelix::correct_translation(const _bstr_t &trans)
{
	BANNER( "CFelix::correct_translation" ) ;

	ATLASSERT( trans.length() > 0 ) ;
	
	TRACE( trans ) ;
	
	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}
	
	TRANS_ENFORCE( m_ta_obj->put_CorrectedTranslation( trans ), _T("Failed to set translation") ) ;
}


// Function name	: CFelix::get_score
// Description	    : 
// Return type		: double 
double CFelix::get_score()
{
	BANNER( "CFelix::get_score" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	double score ;
	TRANS_ENFORCE( m_ta_obj->get_Score( &score ), _T("Failed to get score") ) ;
	TRACE( score ) ;
	return score ;
}


// Function name	: CFelix::lookup
// Description	    : 
// Return type		: void 
// Argument         : const _bstr_t &query
void CFelix::lookup(const _bstr_t &query)
{
	BANNER( "CFelix::lookup" ) ;

	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}

	TRANS_ENFORCE( m_ta_obj->put_Query( query ), _T("Failed to set query") ) ;
}


// Function name	: CFelix::set_translation
// Description	    : 
// Return type		: void 
// Argument         : const _bstr_t &translation
void CFelix::set_translation(const _bstr_t &translation)
{			   
	BANNER( "CFelix::set_translation" ) ;
				
	ATLASSERT( translation.length() > 0 ) ;

	TRACE( translation ) ;
	
	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}
	
	TRANS_ENFORCE( m_ta_obj->put_Translation( translation ), _T("Failed to set translation") ) ;
}



void CFelix::lookup_translation(const _bstr_t &query)
{
	BANNER( "CFelix::lookup_translation" ) ;
	
	TRACE( query ) ;
	
	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}
	TRANS_ENFORCE( m_ta_obj->put_TranslationForReview( query ), _T("Failed to set query") ) ;
}



void CFelix::quit_silently()
{
	TRANS_ENFORCE( m_ta_obj->QuitSilently( ), _T("Failed to set query") ) ;
}

bool CFelix::is_visible()
{
	long val ;
	TRANS_ENFORCE( m_ta_obj->get_IsVisible( &val ), _T("Failed to set query") ) ;
	
	if ( ! val )
	{
		return false ;
	}
	
	return true ;
}

void CFelix::set_visible_on()
{
	TRANS_ENFORCE( m_ta_obj->put_IsVisible( VARIANT_TRUE ), _T("Failed to set IsVisible to true" ) );
}

void CFelix::set_visible_off()
{
	TRANS_ENFORCE( m_ta_obj->put_IsVisible( VARIANT_FALSE ), _T("Failed to set IsVisible to false" ) );
}

void CFelix::add_gloss_entry(const _bstr_t &entry)
{
	ensure_instantiated() ;
	
	if ( ! is_visible() )
	{
		set_visible_on( ) ;
	}
	
	TRANS_ENFORCE( m_ta_obj->raw_AddGlossaryEntry( entry ), _T("Failed to save memory") ) ;
}
*/

// Tests

#ifdef _DEBUG

//DECLARE(CFelixTestCase)
//	CFelix app ;
//END_DECLARE
//
//SETUP(CFelixTestCase)
//{
//
//}
//
//TEARDOWN(CFelixTestCase)
//{
//	app.quit_silently() ;
//}
//
//TESTF(CFelixTestCase,Visible)
//{
//	app.put_visible_off() ;
//	ASSERT_TRUE ( app.is_visible() == false ) ;
//	app.put_visible_on() ;
//	ASSERT_TRUE ( app.is_visible() == true ) ; 
//	app.put_visible_off() ;
//	ASSERT_TRUE ( app.is_visible() == false ) ;
//}
//
//TESTF(CFelixTestCase,Query)
//{
//	app.set_query( L"Query" ) ;
//	ASSERT_TRUE(app.get_query() == L"Query" ) ;
//}
//
//
//TESTF(CFelixTestCase,Score)
//{
//	app.set_query( L"UniqueQueryString" ) ;
//	ASSERT_TRUE ( FLOAT_EQ( app.get_score(), 0.0f ) ) ; 
//
//}

#endif // #ifdef _DEBUG
