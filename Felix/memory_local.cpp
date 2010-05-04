#include "stdafx.h"
#include "memory_local.h"
#include "logging.h"
#include "record_local.h"
#include "DemoException.h"

namespace mem_engine
{
	using namespace except ;
	using ATL::CString ;

	size_t memory_local::size()
	{
		return m_records.size() ;
	}
	bool memory_local::empty()
	{
		return m_records.empty() ;
	}

	/************************************************************************/
	/* MemoryInfo                                                           */
	/************************************************************************/

	void memory_local::set_locked_off()
	{
		m_header.set_locked_off() ;
	}

	void memory_local::set_locked_on()
	{
		m_header.set_locked_on() ;
	}

	bool memory_local::is_locked()
	{
		return m_header.is_locked() ;
	}
	// TranslationMemory
	bool memory_local::add_record(record_pointer record)
	{
		if ( m_header.is_locked() )
		{
			return false ;
		}
		if ( ! record->is_valid_record() ) 
		{
			throw except::CException( IDS_TRANS_OR_SOURCE_EMPTY ) ;
		}


		// Only check when conditions are right
		if (should_check_for_demo())
		{
		// check for demo status
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
		}

		record->set_cmp_maker(m_cmp_maker) ;

		// returns iterator for next pos, and whether record was inserted.
		const std::pair< iterator_type, bool > res = m_records.insert( record ) ;

		const bool was_added = res.second ;
		if ( was_added ) 
		{
			// ensure creator and modified-by are set...
			record->get_creator() ;
			record->get_modified_by() ;

			const size_t recid = record->get_id() ;
			if (! recid || m_ids.find(recid) != m_ids.end())
			{
				record->set_id(get_next_id()) ;
			}
			m_ids.insert(record->get_id()) ;

			// memory is now dirty
			this->set_saved_flag(false) ;
		}

		return was_added ;
	}

	void memory_local::get_match_candidates( trans_set &candidates, const wstring query, double min_score )
	{
		Distance distance ;
		distance.set_minscore(min_score) ;
		Segment segment(m_cmp_maker, query) ;
		const wstring query_cmp = segment.cmp() ;

		foreach ( record_pointer record, m_records )
		{
			if ( distance.edist_score(query_cmp, record->get_source_cmp()) >= min_score)
			{
				candidates.insert(record) ;
			}
		}
	}

	void memory_local::get_rmatch_candidates( trans_set &candidates, const wstring query, double min_score )
	{
		Distance distance ;
		distance.set_minscore(min_score) ;
		Segment segment(m_cmp_maker, query) ;
		const wstring query_cmp = segment.cmp() ;

		foreach ( record_pointer record, m_records )
		{
			if ( distance.edist_score(query_cmp, record->get_trans_cmp()) >= min_score)
			{
				candidates.insert(record) ;
			}
		}
	}
	double memory_local::get_best_match_score(const wstring query)
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
	bool memory_local::get_glossary_matches( search_match_container &matches, const search_query_params &params )
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

	bool memory_local::perform_search( search_match_container &matches, 
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
	bool memory_local::erase( const record_pointer record )
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
			throw except::CException( e ) ;
		}

		ATLTRACE( "...Save complete!\n" ) ;

		// the document is saved
		set_saved_flag( true ) ;

		ensure_file_exists(get_location());

		return is_saved() ;
	}

	mem_engine::record_pointer memory_local::add_by_id( size_t recid, const wstring source, const wstring trans )
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

	void memory_local::replace(const record_pointer old_rec, record_pointer new_rec)
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
	void memory_local::get_gloss_100(search_match_container& matches, const search_query_params& params)
	{
		Segment query(m_cmp_maker, params.m_source) ;
		gloss_match_tester tester(query.cmp()) ;
		tester.set_search_match(this->make_match()) ;

		foreach( record_pointer record, m_records)
		{
			if ( tester.test_source( record ) )
			{
				matches.insert( tester.get_search_match() ) ;
				tester.set_search_match(this->make_match()) ;
			}
		}
	}

	void memory_local::get_gloss_fuzzy(search_match_container& matches, 
		const search_query_params& params)
	{
		set_minimum_score( m_gloss_properties.get_min_score() ) ;
		Distance distance ;
		const double min_score = m_match_maker.get_minimum_score() ;
		distance.set_minscore(min_score) ;
		Segment haystack(m_cmp_maker, params.m_rich_source) ;

		search_match_ptr match(this->make_match()) ;
		foreach(record_pointer record, m_records)
		{
			if (distance.subdist_score(record->get_source_cmp(), haystack.cmp()) >= min_score)
			{
				Segment needle(m_cmp_maker, record->get_source_rich()) ;
				match->set_record(record) ;
				if(m_match_maker.fuzzy_gloss_score(needle, haystack, match))
				{
					set_gloss_fuzzy_match(matches, params, match);	
					match = this->make_match() ;
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

		// *** TODO: Consider deletion
		// Probably not needed, since we now set the user name on Felix startup,
		// and refresh it whenever the preferences are changed.
		refresh_user_name() ;

		get_date_created(file_name) ;

		const unsigned int file_len = file::file::size( file_name ) ;

		if ( file_len == 0 )
		{
			throw except::CException( IDS_CORRUPT_FILE ) ;
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
	// tabulate_fonts
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
	rel_tuple memory_local::get_reliability_stats()
	{
		// initialize the three variables
		size_t low = 100 ; 
		size_t high = 0 ;
		double ave = 0 ;

		foreach ( record_pointer record, m_records )
		{
			const size_t rel = record->get_reliability() ;
			low = min(low, rel) ;
			high = max(high, rel) ;

			ave += rel ;
		}

		low = min(low, high) ;

		if ( m_records.empty() == false )
		{
			ave /= (double)m_records.size() ;
		}
		return boost::make_tuple(low, high, ave) ;

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
			throw except::CException(e) ;
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
	void memory_local::search_no_regex( const search_query_params & params, search_match_container &matches ) 
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

	// =============
	// file io
	// =============

	bool memory_local::load_text( char * raw_text, const CString& file_name, unsigned int file_len )
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

		int progress_interval = get_progress_interval(num_records);

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

	void memory_local::load_header_raw_text(const char *raw_text, const size_t file_len)
	{
		this->get_memory_info()->set_count( -1 ) ;

		textstream_reader< char > reader ;
		reader.set_buffer(raw_text) ;

		if (! reader.find( "<head>", true ))
		{
			return ;
		}
		const textstream_reader< char >::bookmark_type text_start = reader.get_current_pos() ;

		if (! reader.find( "</head>", false ))
		{
			return ;
		}
		const textstream_reader< char >::bookmark_type text_end = reader.get_current_pos() ;

		const string text(text_start, text_end) ;
		const UINT code_page = get_correct_encoding(raw_text, file_len) ;
		const wstring header_text = string2wstring(text, code_page) ;
		// read the header, if any
		m_header.read_header( header_text.c_str() ) ;
	}

	void memory_local::loadRecords( const ATL::CString& file_name, 
									textstream_reader< wchar_t >& reader, 
									int progress_interval, 
									bool was_saved ) 
	{
		typedef textstream_reader< wchar_t >::bookmark_type bm_type ;

		CXml2RecordConverter converter  ;

		if ( ! reader.find( L"<records", true ) )
		{
			throw except::CException( IDS_CORRUPT_FILE ) ;
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
					throw except::CException( IDS_CORRUPT_FILE ) ;
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

	void memory_local::postLoadCleanup( const ATL::CString& file_name, bool was_saved, size_t original_num_records ) 
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

	void memory_local::loadWideBuffer(const char* raw_text, int file_len, CStringW& wide_buffer)
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

	int memory_local::get_progress_interval(int num_records)
	{
		const int SMALL_TM_SIZE = 1000 ;
		const int DEFAULT_PROGRESS_INTERVAL = 10 ;
		const int NUM_INTERVAL_STEPS = 200 ;

		if ( num_records > SMALL_TM_SIZE  ) 
		{
			return num_records / NUM_INTERVAL_STEPS ;
		}

		return DEFAULT_PROGRESS_INTERVAL;
	}

	void memory_local::handleCExceptionOnLoad( const CString& file_name, bool was_saved, CException& e ) 
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

		if (m_listener)
		{
			if ( UserSaysBail() )
			{
				m_listener->OnProgressDoneLoad(0) ;
				set_saved_flag( was_saved ) ;
				e.set_bottom_message( get_load_failure_msg(file_name) ) ;
				throw except::CException( e ) ;
			}
		}
		else
		{
			e.set_bottom_message( IDS_PROMPT_DISCARD_LOAD ) ;
			if ( IDNO == e.notify_user( IDS_LOAD_RECORD_FAILED, MB_YESNO ) )
			{
				set_saved_flag( was_saved ) ;
				CString exception_message ;
				exception_message.FormatMessage( IDS_LOAD_FAILED, file_name ) ;
				throw except::CException( exception_message ) ;
			}
		}


	}

	/*
	Find out if we should check whether this is a demo
	version while adding a record.
	*/
	bool memory_local::should_check_for_demo() const
	{
		const size_t DEMO_CHECK_INTERVAL = 100 ;

		if (m_records.size() < MAX_MEMORY_SIZE_FOR_DEMO)
		{
			return false ;
		}
		if (m_records.size() % DEMO_CHECK_INTERVAL)
		{
			return false ;
		}
		return true ;
	}


}