#include "StdAfx.h"
#include "TranslationMemory.h"
#include "StringConversions.h"
#include "record_local.h"
#include "output_device.h"
#include "logging.h"
#include "input_device_file.h"

static const int ONEKB = 1024 ;
static const int ONEMB = (ONEKB*ONEKB) ;
static const int TENMB = (10*ONEMB) ;

using namespace except;


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

namespace mem_engine
{
	// ************************************
	// *
	// * memory member functions
	// *
	// *************************************

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	CTranslationMemory::CTranslationMemory(app_props::props_ptr props,
										double min_score) : 
		m_props(props),
		m_mem_properties(&props->m_mem_props),
		m_gloss_properties(&props->m_gloss_props),
		m_algo_properties(&props->m_alg_props),
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

		m_cmp_maker.m_ignore_case = !! m_mem_properties->m_data.m_ignore_case ;
		m_cmp_maker.m_ignore_hira_kata = !! m_mem_properties->m_data.m_ignore_hir_kat ;
		m_cmp_maker.m_ignore_width = !! m_mem_properties->m_data.m_ignore_width ;

		this->set_minimum_score( m_mem_properties->get_min_score() ) ;

	}

	CTranslationMemory::~CTranslationMemory()
	{
	}

	//////////////////////////////////////////////////////////////////////
	// Finding matches
	//////////////////////////////////////////////////////////////////////

	//! find matches in TM (source)
	bool CTranslationMemory::find_matches( trans_match_container &matches, const search_query_params &params )
	{
		if ( ! m_is_active )
		{
			return true ;
		}

		// set the plain text query
		m_match_maker.set_minimum_score((double)m_props->m_mem_props.get_min_score() / 100.0) ;
		m_match_maker.set_assess_format_penalty(params.m_assess_format_penalty) ;

		set_cmp_params(params);
		const double min_score = m_match_maker.get_minimum_score() ;
		Segment query(&m_cmp_maker, params.get_source_rich()) ;

		int match_algo = params.m_match_algo ;
		if ( match_algo == IDC_ALGO_AUTO )
		{
			match_algo = detect_match_algo(query.cmp()) ;
		}

		trans_set candidates ;
		this->get_match_candidates(candidates, params.get_source_rich(), min_score) ;

		// check each of the records for a match
		FOREACH ( record_pointer record, candidates )
		{
			search_match_ptr match_test(this->make_match(record)) ;
			Segment source(&m_cmp_maker, record->get_source_rich()) ;

			if ( m_match_maker.get_score( query,
				source,
				match_algo,
				match_test ) )
			{
				matches.insert( match_test ) ;
			}
		}

		return true ;
	}



	//! Find matches in translations
	bool CTranslationMemory::find_trans_matches( trans_match_container &matches, const search_query_params &params )
	{
		if( ! m_is_active )
		{
			return true ;
		}

		try
		{
			// we allow the same minimum score as with source searches
			m_match_maker.set_minimum_score((double)m_props->m_mem_props.get_min_score() / 100.0) ;
			m_match_maker.set_assess_format_penalty(params.m_assess_format_penalty) ;

			set_cmp_params(params);
			const double min_score = m_match_maker.get_minimum_score() ;
			Segment query(&m_cmp_maker, params.get_source_rich()) ;

			int	match_algo = params.m_match_algo ;
			if ( match_algo == IDC_ALGO_AUTO )
			{
				match_algo = detect_match_algo(query.cmp()) ;
			}

			trans_set candidates ;
			this->get_rmatch_candidates(candidates, params.get_source_rich(), min_score) ;

			FOREACH ( record_pointer record, candidates )
			{
				search_match_ptr match_test(this->make_match(record)) ;
				Segment source(&m_cmp_maker, record->get_trans_rich()) ;

				if ( m_match_maker.get_score_trans( query,
					source,
					match_algo,
					match_test ) )
				{

					// ok, stick it in the list now
					matches.insert( match_test ) ;
					// clear the match...
				}
			}
		}
		catch( CException &e )
		{
			e.add_to_message( R2TS( IDS_TRANS_LOOKUP_FAILED ) ) ;
			throw except::CException( e ) ;
		}

		return true ;
	}


	// ************************************
	// *
	// * searching glossary style
	// *
	// *************************************

	/** Sets up a fuzzy glossary match
	*/
	void CTranslationMemory::set_gloss_fuzzy_match(search_match_container& matches, 
		const search_query_params& params, 
		search_match_ptr& match_test)
	{
		markup_ptr markup = match_test->get_markup() ;
		record_pointer record = match_test->get_record() ;
		markup->SetQuery( params.m_source ) ;
		markup->SetTrans( record->get_trans_rich() ) ;
		markup->SetContext( record->get_context_rich() );

		matches.insert( match_test ) ;
	}

	//! Dirty flag. Returns whether the memory is saved. 
	bool CTranslationMemory::is_saved() const
	{
		return m_is_saved ;
	}

	//! Sets the is_saved flag. Used to tell whether the
	//! tm is saved
	bool CTranslationMemory::set_saved_flag(bool flag)
	{
		m_is_saved = flag ;
		return m_is_saved ;
	}


	//! Sets the minimum score for determining that a translation
	//! record is a match.
	void CTranslationMemory::set_minimum_score(const size_t score)
	{
		ATLASSERT( score <= 100 ) ;

		m_match_maker.set_minimum_score( static_cast< double >( score ) / 100.f ) ;
	}
	
	//! This is left over from the DRM days. Should be removed.
	//! TODO: Remove this code
	void CTranslationMemory::refresh_status()
	{
		m_is_demo = false ;
	}


	//! This is left over from the DRM days. Should be removed.
	//! TODO: Remove this code
	bool CTranslationMemory::is_demo() const
	{
		return false ;
	}

	//! Set the memory properties
	void CTranslationMemory::set_properties_memory( app_props::properties_memory *props)
	{
		m_mem_properties = props ;
		set_minimum_score( m_mem_properties->get_min_score() ) ;
	}
	//! Set the glossary properties
	void CTranslationMemory::set_properties_glossary( app_props::properties_glossary *props )
	{
		m_gloss_properties = props ;
	}
	//! Set the algorithm properties
	void CTranslationMemory::set_properties_algo( app_props::properties_algorithm *props )
	{
		m_algo_properties = props ;
	}

	//! Finds the number of records in an unparsed TM file
	//! First tries to parse the header, and get the record count from there.
	//! If there was no record count, counts each <record> entry in the file.
	//! We do this so we can show the progress bar on loading.
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

	//! Gets the number of glossary matches for `query`
	long CTranslationMemory::get_number_of_glossary_matches( const wstring query )
	{
		search_query_params params ;

		Segment query_seg(&m_cmp_maker, query) ;
		params.m_ignore_case = !! m_gloss_properties->m_data.m_ignore_case ;
		params.m_assess_format_penalty = false ;
		//		params.m_match_algo =  m_gloss_properties->m_data.m_match_algo ;
		params.m_rich_source = query ;
		params.m_source = query_seg.cmp() ;
		params.m_ignore_width = !! m_gloss_properties->m_data.m_ignore_width ;
		params.m_ignore_hira_kata = !! m_gloss_properties->m_data.m_ignore_hir_kat ;

		search_match_container matches ;

		this->get_glossary_matches(matches, params) ;
		return static_cast<long>(matches.size()) ;
	}

	//! Leftover code from DRM
	//! TODO: Delete
	void CTranslationMemory::do_demo_check()
	{
		refresh_status() ;
	}

	//! Sets the creator of the TM to the current user. The current
	//! user is either the username provided by the user, or the
	//! user's login name.
	void CTranslationMemory::set_creator_to_current_user()
	{
		this->get_memory_info()->set_creator_to_current_user() ;
	}

	//! Get the current user name from the memory info
	void CTranslationMemory::refresh_user_name(void)
	{
		// refresh user name 
		this->get_memory_info()->get_current_user() ;
	}

	//////////////////////////////////////////////////////////////////////////
	// extra strings interface
	// Extra strings are used to add arbitrary notes to a TM.
	//////////////////////////////////////////////////////////////////////////

	//! Remove an extra string from the TM
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

	//! Add an extra string to the TM.
	//! Extra strings are key/value pairs.
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

	//! Retrieve an extra string for the TM by key
	const wstring CTranslationMemory::get_extra_string( const wstring key ) 
	{
		extra_strings_type::const_iterator value = m_extra_strings.find( key ) ;
		if ( value != m_extra_strings.end() )
		{
			return value->second ;
		}

		return wstring() ;
	}

	//! Sets the date-created value to the file's creation date.
	void CTranslationMemory::detect_date_created( const CString &location, input_device_ptr input )
	{
		// get the size of the file and date created
		input->ensure_file_exists(location);

		// Get created on from file creation date (but the memory could be older than
		// this if it has been copied, etc.)
		this->get_memory_info()->set_created_on( input->get_creation_time(location) ) ;
	}

	/**
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

	//! Get the encoding from the XML encoding declaration
	UINT CTranslationMemory::encoding_from_encoding_string(textstream_reader< char > & xml_reader)
	{
		xml_reader.jump_to_first_of( "\"'", true ) ;
		xml_reader.eat_whitespace() ;
		textstream_reader<char>::bookmark_type start = xml_reader.get_current_pos() ;
		xml_reader.jump_to_first_of( "\"'", false ) ;
		string charset_str(start, xml_reader.get_current_pos()) ;
		return sci_encoding_from_encoding_string( charset_str ) ;
	}

	//! Use the CMultiLanguage class to guess the text encoding
	UINT CTranslationMemory::encoding_from_multi_lang(const char* text, UINT text_size)
	{
		CMultiLanguage multi ;

		const DWORD flag = MLDETECTCP_HTML ;
		const DetectEncodingInfo detect = multi.detect_encoding( text, text_size, flag ) ;

		return detect.nCodePage ;
	}

	//! translate a std::exception into an except::CException if we are going
	//! to rethrow
	void CTranslationMemory::handle_stdexception_on_load(   bool was_saved, const CString& file_name, std::exception &e  ) 
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
			throw except::CException( exception_message ) ;
		}
	}

	//! set memory type: true (TM) or false (glossary)
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

	//! is it a TM or glossary?
	bool CTranslationMemory::get_is_memory() const 
	{
		return this->get_memory_info_const()->is_memory() ;
	}

	//! If we have a listener, get UI interaction from there.
	//! Otherwise, prompt the user ourselves.
	// TODO: We want to get rid of this!
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

	//! find out whether to continue processing, or rethrow the exception
	void CTranslationMemory::continue_or_throw( CException &e )
	{
		// see if the user wants to continue saving
		CException local( e ) ;
		local.set_bottom_message( IDS_PROMPT_SKIP_CURRENT ) ;
		CString exception_msg ;
		exception_msg.FormatMessage( IDS_SAVE_FAILED, resource_string(IDS_RECORD) ) ;
		if ( IDNO == local.notify_user( exception_msg, MB_YESNO ) ) 
		{
			throw except::CException( e ) ;
		}
	}

	//! Check with the listener (which can prompt user) whether to bail on action
	bool CTranslationMemory::ListenerSaysBail()
	{
		return m_listener != NULL && m_listener->ShouldBailFromException();
	}

	//! See if we should send a load update to our progress listener.
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

	//! Convenience method for creating a match object that properly
	//! points back to this TM.
	mem_engine::search_match_ptr CTranslationMemory::make_match( record_pointer record )
	{
		search_match_ptr match(new search_match) ;
		match->set_memory_id(this->get_id()) ;
		match->set_memory_location(get_fname_from_loc((LPCWSTR)this->get_location())) ;
		if (record)
		{
			match->set_record(record) ;
		}
		return match ;
	}
}