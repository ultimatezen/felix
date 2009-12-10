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
#include "DemoException.h"

#include "output_device.h"
#include "logging.h"

static const int ONEKB = 1024 ;
static const int ONEMB = (ONEKB*ONEKB) ;
static const int TENMB = (10*ONEMB) ;

const static int CP_UNICODE = 1200 ;
const static int CP_UNICODE_BE = 1201 ;

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
			const wstring query_cmp = query.cmp() ;

			int	match_algo = params.m_match_algo ;
			if ( match_algo == IDC_ALGO_AUTO )
			{
				match_algo = detect_match_algo(query.cmp()) ;
			}

			trans_set candidates ;
			this->get_rmatch_candidates(candidates, query_cmp, min_score) ;

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


	// =============
	// file io
	// =============

	// load

	// Function name	: memory::load
	// Description	    : 
	// Return type		: bool 
	// Argument         : const tstring& file_name

	void CTranslationMemory::loadRecords( const ATL::CString& file_name, textstream_reader< wchar_t >& reader, int progress_interval, bool was_saved ) 
	{
		typedef textstream_reader< wchar_t >::bookmark_type bm_type ;

		CXml2RecordConverter converter  ;

		if ( ! reader.find( L"<records", true ) )
		{
			throw CException( IDS_CORRUPT_FILE ) ;
		}

		while ( reader.find( L"<record", true ) )
		{
			try
			{
				// we don't make this a bookmark type because we are
				// going to assign to it (bookmark type is const)
				LPWSTR bookmark_start = (LPWSTR)reader.get_current_pos() ;
				if ( ! reader.find( L"</record>", true ) )
				{
					throw CException( IDS_CORRUPT_FILE ) ;
				}

				bm_type bookmark_end = reader.get_current_pos() ;

				ATLASSERT( bookmark_start != bookmark_end ) ;
				ATLASSERT( bookmark_start != NULL ) ;
				ATLASSERT( bookmark_end != NULL ) ;		
				if ( bookmark_start != bookmark_end )
				{
					const size_t len = str::generic_strdist( bookmark_start, bookmark_end ) ;
					bookmark_start[len-1]=0 ;

					record_pointer record = converter.convert_from_xml_node( bookmark_start ) ;
					add_record( record ) ;
				}

				check_progress_update(progress_interval);
			}
			catch ( CException &exception )
			{
				handleCExceptionOnLoad( file_name, was_saved, exception);
			}
			catch( std::exception &std_exception )
			{
				handleStdExceptionOnLoad( was_saved, file_name, std_exception);
			}
		}

	}

	void CTranslationMemory::postLoadCleanup( const ATL::CString& file_name, bool was_saved, size_t original_num_records ) 
	{
		if ( m_listener != NULL )
		{
			m_listener->OnProgressDoneLoad(size() - original_num_records ) ;
		}

		set_saved_flag( was_saved ) ;

		if ( is_new() ) 
		{
			set_location( file_name ) ;
		}

	}

	void CTranslationMemory::loadWideBuffer(const char* raw_text, int file_len, CStringW& wide_buffer)
	{
		ATLASSERT( raw_text != NULL ) ;
		if (!raw_text)
		{
			return ;
		}
		const UINT code_page = get_correct_encoding( raw_text, file_len ) ;

		if ( code_page == CP_UNICODE ) // our work is done for us!
		{
			ATLASSERT( file_len % 2 == 0 && "File length must not be odd" ) ;
			size_t destSize = static_cast< size_t >( file_len / 2 + 1 ) ;
			size_t srcSize = static_cast< size_t >( file_len / 2 ) ;
			LPCWSTR src = (LPCWSTR)( raw_text ) ;
			LPWSTR dest = wide_buffer.GetBufferSetLength( destSize+1 ) ;
			ATLVERIFY( SUCCEEDED( StringCbCopyNW(          
				dest,
				destSize,
				src,
				srcSize
				) ) ) ;
			wide_buffer.ReleaseBuffer(destSize) ;
			return ;

		}

		// convert to Unicode (from code_page)
		CLEAR_WINERRORS ;
		const UINT len_needed = ::MultiByteToWideChar( code_page, NULL, raw_text, file_len, NULL, 0 ) ;
		ATLASSERT( ERROR_SUCCESS == GetLastError() ) ;

		// we need to create a new buffer here, because we may be loading
		// more than one memory at the same time.
		// If we do that, then we will wreck our current buffer, causing all kinds of havoc!!!
		LPWSTR dest = wide_buffer.GetBufferSetLength( len_needed+1 ) ;
		::MultiByteToWideChar( code_page, NULL, raw_text, file_len, dest, len_needed+1 ) ;
		ATLASSERT( ERROR_SUCCESS == GetLastError() ) ;

		// MultiByteToWideChar doesn't null-terminate the string
		wide_buffer.GetBuffer()[len_needed] = 0 ;
		wide_buffer.ReleaseBuffer(len_needed) ;
	}

	int CTranslationMemory::setProgressInterval(int num_records)
	{
		int progress_interval(10) ;
		if ( 1000 < num_records ) 
		{
			progress_interval = num_records / 200 ;
		}

		return progress_interval;
	}

	void CTranslationMemory::handleCExceptionOnLoad( const ATL::CString& file_name, bool was_saved, CException& e ) 
	{
		if ( this->is_demo() )
		{
			// If the exception was due to an attempt to load too many
			// entries, we'll pass it up, cancelling the load.
			if ( size() >= MAX_MEMORY_SIZE_FOR_DEMO )
			{
				set_saved_flag( was_saved ) ;
				throw CDemoException() ;
			}
		}

		if ( UserSaysBail() )
		{
			m_listener->OnProgressDoneLoad(0) ;
			set_saved_flag( was_saved ) ;
			e.set_bottom_message( get_load_failure_msg(file_name) ) ;
			throw CException( e ) ;
		}

		e.set_bottom_message( IDS_PROMPT_DISCARD_LOAD ) ;
		if ( IDNO == e.notify_user( IDS_LOAD_RECORD_FAILED, MB_YESNO ) )
		{
			if ( m_listener != NULL )
			{
				m_listener->OnProgressDoneLoad(0) ;
			}

			set_saved_flag( was_saved ) ;
			CString exception_message ;
			exception_message.FormatMessage( IDS_LOAD_FAILED, file_name ) ;
			throw CException( exception_message ) ;
		}

	}



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

		const int count = m_header.get_count( ) ;
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
		m_header.set_creator_to_current_user() ;
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
		m_header.get_current_user() ;
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
		m_header.set_created_on( string2wstring( created_date.get_date_time_string( ) ) );
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

	void CTranslationMemory::set_header( const memory_header &header )
	{
		m_header = header ;
	}

	void CTranslationMemory::set_is_memory( const bool setting )
	{
		if ( setting ) 
		{
			m_header.set_is_memory_on() ;
		}
		else
		{
			m_header.set_is_memory_off() ;
		}
	}

	bool CTranslationMemory::get_is_memory() const 
	{
		return m_header.is_memory() ;
	}

	void CTranslationMemory::set_locked_off()
	{
		m_header.set_locked_off() ;
	}

	void CTranslationMemory::set_locked_on()
	{
		m_header.set_locked_on() ;
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

	bool CTranslationMemory::load_text( char * raw_text, const CString& file_name, unsigned int file_len )
	{
		// read the header, if any
		this->load_header_raw_text(raw_text, file_len) ;

		bool was_locked = this->is_locked() ;
		this->set_locked_off() ;

		int num_records = get_num_records( raw_text ) ;

		if ( m_listener != NULL ) 
		{
			m_listener->OnProgressInit( file_name, size(), size() + num_records ) ;
		}

		int progress_interval = setProgressInterval(num_records);

		bool was_saved = is_saved() ;

		// now convert to utf-16
		CStringW wide_buffer ;
		loadWideBuffer(raw_text, file_len, wide_buffer) ;

		textstream_reader< wchar_t > reader ;

		reader.set_buffer( wide_buffer.GetBuffer() ) ;

		loadRecords(file_name, reader, progress_interval, was_saved);
		wide_buffer.ReleaseBuffer() ;
		if (was_locked)
		{
			this->set_locked_on() ;
		}
		return was_saved ;
	}

	void CTranslationMemory::load_header_raw_text( char * raw_text, int file_len )
	{
		textstream_reader< char > reader ;
		reader.set_buffer( raw_text ) ;

		if ( ! reader.find( "<head>", false ) )
		{
			return ;
		}
		reader.rewind() ;
		textstream_reader< char >::bookmark_type text_start = reader.get_current_pos() ;

		if ( ! reader.find( "</head>", true ) )
		{
			return ;
		}
		textstream_reader< char >::bookmark_type text_end = reader.get_current_pos() ;

		string text( text_start, text_end ) ;

		str::wbuffer wide_buffer ;

		const UINT code_page = get_correct_encoding( raw_text, file_len ) ;

		// convert to Unicode (from code_page)
		const int len_needed = ::MultiByteToWideChar( code_page, NULL, text.c_str(), static_cast< int >( text.size() ), NULL, 0 ) + 1 ;
		::MultiByteToWideChar( code_page, 
			NULL, 
			text.c_str(), 
			static_cast< int >( text.size() ), 
			wide_buffer.buffer( static_cast< size_t >( len_needed ), false ), 
			len_needed ) ;

		wide_buffer.null_terminate( static_cast< size_t >( len_needed - 1 ) ) ;

		m_header.set_count( -1 ) ;
		wstring header_text = wide_buffer.str();
		// read the header, if any
		m_header.read_header( header_text.c_str() ) ;
	}

	// Function name	: 
	// Description	    : 
	// Return type		: bool memory::find_matches 
	// Argument         :  translation_match_multiset &matches
	// Argument         : search_query_params &params
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
		const wstring query_cmp = query.cmp() ;

		int match_algo = params.m_match_algo ;
		if ( match_algo == IDC_ALGO_AUTO )
		{
			match_algo = detect_match_algo(query.cmp()) ;
		}

		trans_set candidates ;
		this->get_match_candidates(candidates, query_cmp, min_score) ;

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
	//////////////////////////////////////////////////////////////////////////
	// memory_local methods
	//////////////////////////////////////////////////////////////////////////

	size_t memory_local::size()
	{
		return m_records.size() ;
	}
	bool memory_local::empty()
	{
		return m_records.empty() ;
	}


	bool memory_local::add_record(record_pointer record)
	{
		if ( m_header.is_locked() )
		{
			return false ;
		}
		if ( ! record->is_valid_record() ) 
		{

			ATLTRACE( "*** FAILED TO ADD RECORD ***\n" ) ;
			TRACE( record->get_source_plain()  ) ;
			TRACE( record->get_trans_plain()  ) ;
			TRACE( record->get_context_plain()  ) ;
			TRACE( record->get_created().get_date_time_string() ) ;
			TRACE( record->get_modified().get_date_string() ) ;

			throw CException( IDS_TRANS_OR_SOURCE_EMPTY ) ;
		}


		// check for demo status
#ifndef _DEBUG
		int cookie = 0xDECAFBAD ;
		try
		{
			do_demo_check( &cookie ) ;
		}
		catch( ... )
		{
			TRACE( cookie ) ;
			cookie = 0 ;
			throw ;
		}
#endif
		ATLASSERT( record->get_source_plain().empty() == false ) ;
		ATLASSERT( record->get_trans_plain().empty() == false ) ;

		// ensure creator and modified-by are set...
		record->get_creator() ;
		record->get_modified_by() ;

		record->set_cmp_maker(m_cmp_maker) ;

		// returns iterator for next pos, and whether record was inserted.
		std::pair< iterator_type, bool > res = m_records.insert( record ) ;

		if ( res.second ) 
		{
			size_t recid = record->get_id() ;
			if (! recid || m_ids.find(recid) != m_ids.end())
			{
				record->set_id(get_next_id()) ;
			}
			m_ids.insert(record->get_id()) ;

			// memory is now dirty
			this->set_saved_flag(false) ;
		}

		return res.second ;
	}

	void memory_local::get_match_candidates( trans_set &candidates, const wstring &query_cmp, double min_score )
	{
		Distance distance ;
		distance.set_minscore(min_score) ;

		foreach ( record_pointer record, m_records )
		{
			if ( distance.edist_score(query_cmp, record->get_source_cmp()) >= min_score)
			{
				candidates.insert(record) ;
			}
		}
	}

	void memory_local::get_rmatch_candidates( trans_set &candidates, const wstring &query_cmp, double min_score )
	{
		Distance distance ;
		distance.set_minscore(min_score) ;

		foreach ( record_pointer record, m_records )
		{
			if ( distance.edist_score(query_cmp, record->get_trans_cmp()) >= min_score)
			{
				candidates.insert(record) ;
			}
		}
	}
	double memory_local::get_best_match_score(const wstring &query)
	{
		Distance distance ;
		const wstring query_cmp = m_cmp_maker.make_cmp(query) ;

		double best_score = 0.0f ;
		// check each of the records for a match
		foreach ( record_pointer record, m_records )
		{
			const double score = distance.edist_score(query_cmp, record->get_source_cmp()) ;
			if (score > best_score)
			{
				best_score = score ;
			}
		}

		return best_score ;
	}
	bool memory_local::get_glossary_matches( search_match_multiset &matches, const search_query_params &params )
	{

		if ( ! m_is_active )
		{
			return true ;
		}
		if ( params.m_source.empty() ) 
		{
			return true ;
		}

		set_cmp_params(params);
		const size_t max_score = 100 ;
		if ( m_gloss_properties.get_min_score() < max_score )
		{
			get_gloss_fuzzy(matches, params) ;
		}
		else
		{
			get_gloss_100(matches, params) ;
		}
		return ( false == matches.empty() ) ;
	}

	bool memory_local::perform_search( search_match_multiset &matches, 
										const search_query_params &params )
	{
		if( ! m_is_active )
		{
			return true ;
		}

		// If we are not using regular expressions...
		if ( ! params.m_use_regex )
		{
			search_no_regex(params, matches);

			return ( false == matches.empty() ) ;
		}

		// we are using regular expressions
		search_match_tester_regex tester( params ) ;
		foreach( record_pointer record, m_records)
		{
			if ( tester.is_match( record ) )
			{
				search_match_ptr match = tester.get_search_match( record ) ;
				const wstring location = static_cast<LPCWSTR>( m_file_location ) ;
				match->set_memory_location( location ) ;
				match->set_memory_id( get_id() ) ;
				matches.insert( match ) ;
			}
		}
		return ( false == matches.empty() ) ;

	}

	// Function name	: memory::erase
	bool memory_local::erase( record_pointer record )
	{
		if ( m_header.is_locked() )
		{
			return false ;
		}

		size_t num_erased = m_records.erase( record ) ;

		// if we didn't erase anything, then don't mark
		// the memory as dirty. 
		if ( num_erased == 0 )
		{
			return false ;	// didn't erase anything.
		}

		// mark memory as dirty
		m_is_saved = false ;

		return true ;
	}

	// Function name	: memory::get_location
	// Description	    : 
	// Return type		: tstring & 
	CString memory_local::get_location()
	{
		if ( m_file_location.IsEmpty() )
		{
			return resource_string(IDS_NEW) ;
		}
		return m_file_location ;
	}


// save_memory

bool memory_local::save_memory()
{
	// make sure we have a valid directory
	// But don't be too quick to throw an exception
	// Throw it when we have no other choice only!
	file::CPath check_path = m_file_location ;
	check_path.RemoveFileSpec() ;
	ATLASSERT( check_path.IsDirectory() ) ;

	logging::log_debug("Saving memory") ;
	// =============
	// save the file
	// =============
	OutputDeviceFile output_device ;
	output_device.open(m_file_location) ;

	try
	{
		ATLASSERT ( m_listener != NULL ) ; 

		if ( m_listener != NULL ) 
		{
			m_listener->OnProgressInit(m_file_location, 0, static_cast< int >( size() ) ) ;
		}

		// create the xml document
		const string rec_start =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
			"<!DOCTYPE memory >\r\n"
			"<!-- Created by Felix v " VERSION " -->\r\n"
			"<memory>\r\n";
		output_device.write( rec_start ) ;

		m_header.set_count( static_cast< long >( size() ) ) ;
		m_header.set_modified_now() ;
		m_header.modified_by_current_user() ;
		m_header.write_header( &output_device ) ;

		output_device.write( "<records>\r\n" ) ;

		CRecord2XmlConverter converter( &output_device ) ;

		ATLTRACE( "Saving records...\n" ) ;

		int current_record = 0 ;
		// loop through each of the records
		foreach(record_pointer record, m_records)
		{
			try // Failing to save a single record should not make
				// the entire save fail!
			{
				converter.convert_from_record( record ) ;
			}
			catch ( CException &e )
			{
				continue_or_throw(e);
			}

			++current_record ;
			const int update_interval = 10 ;
			if( m_listener != NULL && 0 == ( current_record % update_interval ) )
			{
				m_listener->OnProgressWriteUpdate( current_record ) ;
			}
		}

		output_device.write( "</records>\r\n</memory>" ) ;
		output_device.m_file.write_eof() ;
		output_device.m_file.close() ;

		if (m_listener != NULL ) 
		{
			m_listener->OnProgressDoneWrite(current_record) ;
		}
	}
	catch ( CException &e ) 
	{
		if (m_listener != NULL ) 
		{
			m_listener->OnProgressDoneWrite(0) ;
		}
		CString exception_message ;
		exception_message.FormatMessage( IDS_SAVE_FAILED, get_location() ) ;
		e.add_to_message( exception_message ) ;
		throw CException( e ) ;
	}

	ATLTRACE( "...Save complete!\n" ) ;

	// the document is saved
	set_saved_flag( true ) ;

	ensure_file_exists(get_location());

	return is_saved() ;
}

memory_engine::record_pointer memory_local::add_by_id( size_t recid, const wstring source, const wstring trans )
{
	foreach(record_pointer rec, m_records)
	{
		if (recid == rec->get_id())
		{
			this->erase(rec) ;
			rec->set_source(source) ;
			rec->set_trans(trans) ;
			this->add_record(rec) ;
			return rec ;
		}
	}
	record_pointer rec(new record_local) ;
	rec->set_id(recid) ;
	rec->set_source(source) ;
	rec->set_trans(trans) ;
	this->add_record(rec) ;
	return rec ;
}

void memory_local::replace(record_pointer old_rec, record_pointer new_rec)
{
	this->erase(old_rec) ;
	// ensure creator and modified-by are set...
	new_rec->get_creator() ;
	new_rec->get_modified_by() ;

	new_rec->set_cmp_maker(m_cmp_maker) ;
	m_records.insert(new_rec) ;

	// memory is now dirty
	this->set_saved_flag(false) ;
}

size_t memory_local::get_next_id()
{
	while (m_ids.find(m_next_id) != m_ids.end())
	{
		++m_next_id ;
	}
	return m_next_id ;
}

void memory_local::set_cmp_params( const search_query_params &params )
{
	bool params_changed = false ;
	if (m_cmp_maker.m_ignore_case != params.m_ignore_case)
	{
		params_changed = true ;
		m_cmp_maker.m_ignore_case = params.m_ignore_case ;
	}
	if (m_cmp_maker.m_ignore_hira_kata != params.m_ignore_hira_kata)
	{
		params_changed = true ;
		m_cmp_maker.m_ignore_hira_kata = params.m_ignore_hira_kata ;
	}
	if (m_cmp_maker.m_ignore_width != params.m_ignore_width)
	{
		params_changed = true ;
		m_cmp_maker.m_ignore_width = params.m_ignore_width ;
	}
	if (params_changed)
	{
		foreach(record_pointer record, m_records)
		{
			record->set_cmp_maker(m_cmp_maker) ;
		}
	}
}
void memory_local::get_gloss_100(search_match_multiset& matches, const search_query_params& params)
{
	Segment query(m_cmp_maker, params.m_source) ;
	gloss_match_tester tester(query.cmp()) ;

	foreach( record_pointer record, m_records)
	{
		if ( tester.test_source( record ) )
		{
			set_gloss_100_char(matches, tester, record);
		}
	}
}

void memory_local::get_gloss_fuzzy(search_match_multiset& matches, 
										 const search_query_params& params)
{
	set_minimum_score( m_gloss_properties.get_min_score() ) ;
	Distance distance ;
	const double min_score = m_match_maker.get_minimum_score() ;
	distance.set_minscore(min_score) ;
	Segment haystack(m_cmp_maker, params.m_rich_source) ;

	search_match_ptr match(new search_match) ;
	foreach(record_pointer record, m_records)
	{
		if (distance.subdist_score(record->get_source_cmp(), haystack.cmp()) >= min_score)
		{
			Segment needle(m_cmp_maker, record->get_source_rich()) ;
			match->set_record(record) ;
			if(m_match_maker.fuzzy_gloss_score(needle, haystack, match))
			{
				set_gloss_fuzzy_match(matches, params, match);	
				match = search_match_ptr(new search_match) ;
			}
		}
	}

	set_minimum_score( m_properties.get_min_score() ) ;
}
bool memory_local::load( const CString& file_name )
{

	if ( m_header.is_locked() )
	{
		return false ;
	}
	// input assumptions
	ATLASSERT( file_name.IsEmpty() == false ) ;
	ensure_file_exists(file_name);

	// see if we are a demo...
	refresh_status() ;

	refresh_user_name() ;

	get_date_created(file_name) ;

	const unsigned int file_len = file::file::size( file_name ) ;

	if ( file_len == 0 )
	{
		throw CException( IDS_CORRUPT_FILE ) ;
	}

	const size_t original_num_records = size() ;
	// create a view of the xml document
	file::view memory_view ;
	char *raw_text = (char *)memory_view.create_view( file_name ) ;

	bool was_saved = load_text(raw_text, file_name, file_len);

	postLoadCleanup(file_name, was_saved, original_num_records);

	set_location(file_name) ;
	ensure_file_exists(get_location());

	return true ;
}
bool memory_local::clear_memory()
{
	m_is_saved = m_records.empty() ;
	m_file_location.Empty() ;
	m_records.clear() ;
	return true ;
}
bool memory_local::record_exists(record_pointer rec)
{
	return ( m_records.find( rec ) != m_records.end() ) ;
}
record_pointer memory_local::get_record_at(const size_t index)
{
	if (index >= m_records.size())
	{
		throw CProgramException(_T("Requested record index is out of bounds")) ;
	}

	trans_set::iterator pos = m_records.begin() ;
	std::advance( pos, index ) ;
	return *pos ;
}
// Function name	: CTranslationMemory::tabulate_fonts
void memory_local::tabulate_fonts(font_tabulator &tabulator)
{
	foreach(record_pointer record, m_records)
	{
		// clear doc text

		const wstring combined = record->get_source_rich() + 
			record->get_trans_rich() + 
			record->get_context_rich() ;

		tabulator.tabulate_fonts( boost::to_lower_copy(combined) ) ;
	}
}
// get_reliability_stats
void memory_local::get_reliability_stats(size_t &low, size_t &high, double &ave)
{
	// initialize the three variables
	low = 100 ; 
	high = 0 ;
	ave = 0 ;

	foreach ( record_pointer record, m_records )
	{
		const size_t rel = record->get_reliability() ;
		if ( rel < low )
			low = rel ;

		if ( rel > high )
			high = rel ;

		ave += rel ;
	}

	if ( low == 100 )
	{
		low = 0 ;
	}

	if ( m_records.empty() == false )
	{
		ave /= (double)m_records.size() ;
	}

}

wstring memory_local::get_validated_percent()
{
	if ( m_records.empty() )
	{
		return L"0%" ;
	}
	size_t num_validated = 0 ;

	foreach ( record_pointer record, m_records )
	{
		if ( record->is_validated() )
		{
			++num_validated ;
		}
	}

	return double2percent_wstring( (double)num_validated / (double)m_records.size()  ) ;

}

void memory_local::load_header(const CString &location)
{
	m_records.clear() ;

	// refresh user name 
	m_header.set_creator_to_current_user() ;

	// get the size of the file and date created
	file::file memory_file ;
	ensure_file_exists(location);
	try
	{
		memory_file.open( location ) ;
	}
	catch( CFileException &e )
	{
		CString err_msg ;
		err_msg.FormatMessage( IDS_FILE_NOT_OPEN_FOR_WRITING, location ) ;
		e.add_to_message(err_msg) ;
		throw CException(e) ;
	}
	ATLASSERT(memory_file.is_open()) ;
	if ( ! memory_file.is_open() )
	{
		throw CProgramException(IDS_FILE_NOT_OPEN_FOR_WRITING) ;
	}
	const int file_len = memory_file.size() ;
	FILETIME file_time ;
	memory_file.get_creation_time( &file_time ) ;
	memory_file.close() ;

	// Get created on from file creation date (but the memory could be older than
	// this if it has been copied, etc.)
	misc_wrappers::date created_date( file_time ) ;
	wstring created_on = string2wstring( created_date.get_date_time_string( ) ) ;
	m_header.set_created_on( created_on ) ;

	// create a view of the xml document
	file::view memory_view ;
	char *raw_text = (char *)memory_view.create_view( location ) ;

	load_header_raw_text(raw_text, file_len);
	return;
}
void memory_local::search_no_regex( const search_query_params & params, search_match_multiset &matches ) 
{
	search_match_tester tester( params ) ;
	foreach( record_pointer record, m_records )
	{
		if ( tester.is_match( record ) )
		{
			search_match_ptr match = tester.get_search_match( record ) ;
			const wstring location = static_cast<LPCWSTR>( m_file_location ) ;
			match->set_memory_location( location ) ;
			match->set_memory_id( get_id() ) ;
			matches.insert( match ) ;
		}
	}
}

void memory_local::batch_set_reliability( const size_t rel )
{
	if ( m_header.is_locked() )
	{
		logging::log_warn("Attempted to batch set reliability, but memory is locked.") ;
		return ;
	}
	const size_t setting = min(rel, MAX_RELIABILITY) ;
	foreach(record_pointer record, m_records)
	{
		record->set_reliability(setting) ;
	}
}

void memory_local::batch_set_validation( bool val )
{
	if ( m_header.is_locked() )
	{
		return ;
	}
	foreach( record_pointer record, m_records )
	{
		if ( val ) 
		{
			record->set_validated_on() ;
		}
		else
		{
			record->set_validated_off() ;
		}
	}
}

bool memory_local::is_new()
{
	return !! m_file_location.IsEmpty() ;
}

// Function name	: memory::set_location
void memory_local::set_location(const CString location)
{
	m_file_location = location ;
}



}
