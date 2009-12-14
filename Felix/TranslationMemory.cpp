// ***************************************************************
//  TranslationMemory   version:  1.0   ·  date: 05/29/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// TODO: NullProgressListener object so we don't have to check the progress listener for null
// ***************************************************************

#include "StdAfx.h"
#include "Windows.h"
#include "TranslationMemory.h"

#include "ArmadilloStuff.h"

#include "ConvertUTF.h"

#include "StringConversions.h"
#include ".\translationmemory.h"

#include "easyunit/testharness.h"
#include "record_local.h"

#include "output_device.h"
#include "logging.h"
#include "DemoException.h"

static const int ONEKB = 1024 ;
static const int ONEMB = (ONEKB*ONEKB) ;
static const int TENMB = (10*ONEMB) ;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

void ensure_file_exists( const CString & file_name ) 
{
	const file::CPath path(file_name) ;
	if ( ! path.FileExists() ) // whoops -- file not there
	{
		CString msg ;
		msg.FormatMessage( IDS_MSG_NO_FILE, file_name ) ;
		THROW_WIN_EXCEPTION( msg ) ;
	}
}

bool is_vista_or_later() 
{
	OSVERSIONINFO osvi;

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx (&osvi);
	return (osvi.dwMajorVersion >= 6);
}
const CString get_load_failure_msg( const CString & file_name )  
{
	CString failure_msg ;
	const file::CPath failure_path( file_name ) ;
	failure_msg.FormatMessage( IDS_LOAD_FAILED, failure_path.FindFileName() ) ;
	return failure_msg ;
}

namespace memory_engine
{
	// ************************************
	// *
	// * memory member functions
	// *
	// *************************************

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	CTranslationMemory::CTranslationMemory( double min_score ) : 
		m_is_saved( true ),
		m_match_maker( min_score ),
		m_is_active( true ),
		m_listener(NULL),
		m_id( get_unique_memory_id() ) 
#ifdef UNIT_TEST
		, m_throw_exception(false)
#endif	
		{
		refresh_status() ;

		m_properties.read_from_registry() ;
		m_gloss_properties.read_from_registry() ;

		m_cmp_maker.m_ignore_case = !! m_properties.m_data.m_ignore_case ;
		m_cmp_maker.m_ignore_hira_kata = !! m_properties.m_data.m_ignore_hir_kat ;
		m_cmp_maker.m_ignore_width = !! m_properties.m_data.m_ignore_width ;

		this->set_minimum_score( m_properties.m_data.m_min_score ) ;

	}

	CTranslationMemory::~CTranslationMemory()
	{
	}

	//////////////////////////////////////////////////////////////////////
	// Finding matches
	//////////////////////////////////////////////////////////////////////


	// Function name	: memory::find_trans_matches
	// Description	    : 
	// Return type		: bool 
	// Argument         :  translation_match_multiset &matches
	// Argument         : search_query_params &params
	bool CTranslationMemory::find_trans_matches( TransMatchContainer &matches, const search_query_params &params )
	{
		if( ! m_is_active )
		{
			return true ;
		}

		try
		{
			// we allow the same minimum score as with source searches
			m_match_maker.set_assess_format_penalty(params.m_assess_format_penalty) ;

			set_cmp_params(params);
			search_match_ptr match_test(new search_match) ;
			const double min_score = m_match_maker.get_minimum_score() ;
			Segment query(m_cmp_maker, params.m_rich_source) ;

			int	match_algo = params.m_match_algo ;
			if ( match_algo == IDC_ALGO_AUTO )
			{
				match_algo = detect_match_algo(query.cmp()) ;
			}

			trans_set candidates ;
			this->get_rmatch_candidates(candidates, params.m_rich_source, min_score) ;

			foreach ( record_pointer record, candidates )
			{
				match_test->set_record( record ) ;
				Segment source(m_cmp_maker, record->get_trans_rich()) ;

				if ( m_match_maker.get_trans_score( query,
					source,
					match_algo,
					match_test ) )
				{

					// ok, stick it in the list now
					const wstring loc = static_cast<LPCWSTR>( get_location() ) ;
					match_test->set_memory_location( loc ) ;
					match_test->set_memory_id( get_id() ) ;
					matches.insert( match_test ) ;
					// clear the match...
					match_test = search_match_ptr(new search_match) ;
				}
			}
		}
		catch( CException &e )
		{
			e.add_to_message( R2TS( IDS_TRANS_LOOKUP_FAILED ) ) ;
			throw CException( e ) ;
		}

		return true ;
	}

	// find_matches




	// ************************************
	// *
	// * searching glossary style
	// *
	// *************************************

	void CTranslationMemory::set_gloss_100_char(search_match_multiset& matches,
		gloss_match_tester& tester, 
		record_pointer& rec)
	{
		search_match_ptr match = tester.get_search_match() ;
		match->set_record(rec) ;
		match->get_markup()->SetTrans( rec->get_trans_rich() ) ;
		const wstring location = static_cast<LPCWSTR>( get_location() ) ;
		match->set_memory_location( location ) ;
		match->set_memory_id( get_id() ) ;
		matches.insert( match ) ;
	}

	/** Sets up a fuzzy glossary match
	*/
	void CTranslationMemory::set_gloss_fuzzy_match(search_match_multiset& matches, 
		const search_query_params& params, 
		search_match_ptr& match_test)
	{
		markup_ptr markup = match_test->get_markup() ;
		record_pointer record = match_test->get_record() ;
		markup->SetQuery( params.m_source ) ;
		markup->SetTrans( record->get_trans_rich() ) ;
		markup->SetContext( record->get_context_rich() );
		const wstring location = static_cast<LPCWSTR>( get_location( ) ) ;
		match_test->set_memory_location( location ) ;
		match_test->set_memory_id( get_id() ) ;

		matches.insert( match_test ) ;
	}




	// Function name	: CTranslationMemory::is_saved
	// Description	    : 
	// Return type		: bool 
	bool CTranslationMemory::is_saved() const
	{
		return m_is_saved ;
	}

	// Function name	: CTranslationMemory::set_saved_flag
	bool CTranslationMemory::set_saved_flag(bool flag)
	{
		m_is_saved = flag ;
		return m_is_saved ;
	}



	// Function name	: CTranslationMemory::set_minimum_score
	void CTranslationMemory::set_minimum_score(const size_t score)
	{
		ATLASSERT( score <= 100 ) ;

		m_match_maker.set_minimum_score( static_cast< double >( score ) / 100.f ) ;
	}


	// Function name	: CTranslationMemory::refresh_status
	void CTranslationMemory::refresh_status()
	{
		if ( armadillo::get_environment_var( R2A( IDS_USERKEY ) ).empty() || armadillo::get_environment_var( "EXPIRED" ).empty() == false )
		{
			m_is_demo = true ;
		}
		else 
		{
			m_is_demo = false ;
		}
	}


	// Function name	: CTranslationMemory::is_demo
	bool CTranslationMemory::is_demo() const
	{
		return m_is_demo ;
	}


	/*
	*	get/set properties
	*/
	void CTranslationMemory::set_properties_memory( const app_props::properties_memory &props)
	{
		m_properties = props ;
		set_minimum_score( m_properties.get_min_score() ) ;
	}
	void CTranslationMemory::set_properties_glossary( const app_props::properties_glossary &props )
	{
		m_gloss_properties = props ;
	}
	void CTranslationMemory::set_properties_algo( const app_props::properties_algorithm &props )
	{
		m_algo_properties = props ;
	}
	void CTranslationMemory::refresh_properties()
	{
		m_algo_properties.read_from_registry() ;
		m_gloss_properties.read_from_registry() ;
		m_properties.read_from_registry() ;
	}

	const size_t CTranslationMemory::get_num_records( LPCSTR file_text )
	{
		ATLASSERT( file_text != NULL ) ;

		const int count = this->get_memory_info()->get_count() ;
		if ( count >= 0 )
		{
			return static_cast< size_t >( count );
		}

		ATLTRACE("get_num_records: Record count not in header; counting manually.\n") ;
		c_reader reader ;
		reader.set_buffer( file_text ) ;

		size_t num_records(0) ;
		while ( reader.find( "<record>", true ) )
		{
			num_records++ ;
		}

		return num_records ;
	}




	long CTranslationMemory::get_number_of_glossary_matches( const wstring &query )
	{
		search_query_params params ;

		Segment query_seg(m_cmp_maker, query) ;
		params.m_ignore_case = !! m_gloss_properties.m_data.m_ignore_case ;
		params.m_assess_format_penalty = false ;
		//		params.m_match_algo =  m_gloss_properties.m_data.m_match_algo ;
		params.m_rich_source = query ;
		params.m_source = query_seg.cmp() ;
		params.m_ignore_width = !! m_gloss_properties.m_data.m_ignore_width ;
		params.m_ignore_hira_kata = !! m_gloss_properties.m_data.m_ignore_hir_kat ;

		search_match_multiset matches ;

		this->get_glossary_matches(matches, params) ;
		return static_cast<long>(matches.size()) ;
	}

	void CTranslationMemory::do_demo_check( int *cookie )
	{
		*cookie = 0 ;

		refresh_status() ;
		if ( this->is_demo() )
		{
			*cookie = 0xC0DEBEEF ;
			if ( size() > MAX_MEMORY_SIZE_FOR_DEMO + 100 )
			{
				if ( m_listener )
				{
					m_listener->OnProgressDoneLoad( 0 ) ;
				}
				throw CDemoException() ;
			}
		}
		else
		{
			*cookie = 0xDEADC0DE ;
		}

	}

	void CTranslationMemory::set_creator_to_current_user()
	{
		this->get_memory_info()->set_creator_to_current_user() ;
	}

	bool CTranslationMemory::remove_extra_string(const wstring key)
	{
		extra_strings_type::iterator pos = m_extra_strings.find( key ) ;
		if (pos == m_extra_strings.end())
		{
			return false ;
		}
		m_extra_strings.erase(pos) ;
		return true ;
	}

	void CTranslationMemory::set_extra_string( const wstring key, const wstring value )  
	{
		// setting a key to an empty value is the same as
		// deleting it
		if ( value.empty() )
		{
			remove_extra_string( key ) ;
		}
		else
		{
			m_extra_strings[key] = value ;
		}
	}

	const wstring CTranslationMemory::get_extra_string( const wstring &key ) 
	{
		extra_strings_type::const_iterator value = m_extra_strings.find( key ) ;
		if ( value != m_extra_strings.end() )
		{
			return value->second ;
		}

		return wstring() ;
	}

	void CTranslationMemory::refresh_user_name(void)
	{
		// refresh user name 
		this->get_memory_info()->get_current_user() ;
	}

	void CTranslationMemory::get_date_created( const CString &location )
	{
		// get the size of the file and date created
		file::file memory_file ;
		ensure_file_exists(location);
		memory_file.open( location ) ;
		FILETIME file_time ;
		memory_file.get_creation_time( &file_time ) ;

		// Get created on from file creation date (but the memory could be older than
		// this if it has been copied, etc.)
		misc_wrappers::date created_date( file_time ) ;
		this->get_memory_info()->set_created_on( string2wstring( created_date.get_date_time_string( ) ) );
	}

	/*
	First, try to find an encoding declaration in the first 256 chars.
	If that fails, use the MultiLang component to guess the correct encoding.
	*/
	UINT CTranslationMemory::get_correct_encoding(const char *text, UINT text_size )
	{
		const string xml_head = boost::to_lower_copy(string( text, 256 )) ;

		textstream_reader< char > xml_reader ;
		xml_reader.set_buffer( xml_head.c_str() ) ;

		if( xml_reader.find( "encoding" ) )
		{
			return encoding_from_encoding_string( xml_reader ) ;
		}

		return encoding_from_multi_lang( text, text_size ) ;
	}


	UINT CTranslationMemory::encoding_from_encoding_string(textstream_reader< char > & xml_reader)
	{
		xml_reader.find( "\"", true ) ;
		xml_reader.eat_whitespace() ;
		const string charset_str = xml_reader.getline('\"') ;
		return sci_encoding_from_encoding_string( charset_str ) ;
	}

	UINT CTranslationMemory::encoding_from_multi_lang(const char* text, UINT text_size)
	{
		CMultiLanguage multi ;

		const DWORD flag = MLDETECTCP_HTML ;
		const DetectEncodingInfo detect = multi.detect_encoding( text, text_size, flag, CP_UTF8 ) ;

		return detect.nCodePage ;
	}

	void CTranslationMemory::handleStdExceptionOnLoad(   bool was_saved, const CString& file_name, std::exception &e  ) 
	{
		if ( UserSaysBail() )
		{
			if ( m_listener != NULL )
			{
				m_listener->OnProgressDoneLoad(0) ;
			}

			set_saved_flag( was_saved ) ;
			CString exception_message ;
			exception_message.FormatMessage( IDS_LOAD_FAILED, file_name ) ;
			exception_message += _T("\r\n(") + CString(e.what()) + CString(")") ;
			throw CException( exception_message ) ;
		}
	}

	void CTranslationMemory::set_is_memory( const bool setting )
	{
		if ( setting ) 
		{
			this->get_memory_info()->set_is_memory_on() ;
		}
		else
		{
			this->get_memory_info()->set_is_memory_off() ;
		}
	}

	bool CTranslationMemory::get_is_memory() const 
	{
		return this->get_memory_info_const()->is_memory() ;
	}

	bool CTranslationMemory::UserSaysBail()
	{
		if (m_listener)
		{
			return this->ListenerSaysBail() ;
		}
		return IDNO == MessageBox( ::GetDesktopWindow(), 
			_T("Failed to load record. Continue loading?"),
			_T("Error"), 
			MB_YESNO | MB_SETFOREGROUND | MB_ICONHAND );
	}

	void CTranslationMemory::continue_or_throw( CException &e )
	{
		// see if the user wants to continue saving
		CException local( e ) ;
		local.set_bottom_message( IDS_PROMPT_SKIP_CURRENT ) ;
		CString exception_msg ;
		exception_msg.FormatMessage( IDS_SAVE_FAILED, resource_string(IDS_RECORD) ) ;
		if ( IDNO == local.notify_user( exception_msg, MB_YESNO ) ) 
		{
			throw CException( e ) ;
		}
	}

	bool CTranslationMemory::ListenerSaysBail()
	{
		return m_listener != NULL && ! m_listener->RecoverFromExceptions();
	}

	void CTranslationMemory::set_gloss_props( app_props::properties_glossary &props )
	{
		m_gloss_properties = props ;
	}

	void CTranslationMemory::check_progress_update( int progress_interval )
	{
		if( 0 == ( size() % progress_interval ) )
		{
			if ( m_listener != NULL )
			{
				m_listener->OnProgressLoadUpdate( static_cast< int >( size() ) ) ;
			}
		}
	}

	// find_matches
	bool CTranslationMemory::find_matches( TransMatchContainer &matches, const search_query_params &params )
	{
		if ( ! m_is_active )
		{
			return true ;
		}

		// set the plain text query
		m_match_maker.set_assess_format_penalty(params.m_assess_format_penalty) ;

		set_cmp_params(params);
		search_match_ptr match_test(new search_match) ;
		const double min_score = m_match_maker.get_minimum_score() ;
		Segment query(m_cmp_maker, params.m_rich_source) ;

		int match_algo = params.m_match_algo ;
		if ( match_algo == IDC_ALGO_AUTO )
		{
			match_algo = detect_match_algo(query.cmp()) ;
		}

		trans_set candidates ;
		this->get_match_candidates(candidates, params.m_rich_source, min_score) ;

		// check each of the records for a match
		foreach ( record_pointer record, candidates )
		{
			match_test->set_record( record ) ;
			Segment source(m_cmp_maker, record->get_source_rich()) ;

			if ( m_match_maker.get_score( query,
				source,
				match_algo,
				match_test ) )
			{
				const wstring location = static_cast<LPCWSTR>( get_location() ) ;
				match_test->set_memory_location( location ) ;
				match_test->set_memory_id( get_id() ) ;
				matches.insert( match_test ) ;
				match_test = search_match_ptr(new search_match) ;
			}
		}

		return true ;
	}

}
