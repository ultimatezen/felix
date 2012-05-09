#include "stdafx.h"
#include "memory_local.h"
#include "logging.h"
#include "record_local.h"
#include "DemoException.h"
#include "input_device_file.h"

namespace mem_engine
{
	using namespace except ;
	using ATL::CString ;

	void copy_mem_info(memory_pointer from, memory_pointer to)
	{
		if (! from->is_new())
		{
			to->set_location(from->get_location()) ;
		}
		MemoryInfo *from_info = from->get_memory_info() ;
		MemoryInfo *to_info = to->get_memory_info() ;

		to_info->set_creator( from_info->get_creator() ) ;
		to_info->set_field( from_info->get_field() ) ;
		to_info->set_created_on( from_info->get_created_on() ) ;
		to_info->set_source_language( from_info->get_source_language() ) ;
		to_info->set_target_language( from_info->get_target_language() ) ;
		to_info->set_client( from_info->get_client() ) ;
		to_info->set_count( static_cast<long>(from->size()) ) ;
		if (from_info->is_memory())
		{
			to_info->set_is_memory_on() ;
		}
		else
		{
			to_info->set_is_memory_off() ;
		}
		if (from_info->is_locked())
		{
			to_info->set_locked_on() ;
		}
		else
		{
			to_info->set_locked_off() ;
		}

		to_info->set_creation_tool( from_info->get_creation_tool() ) ;
		to_info->set_creation_tool_version( from_info->get_creation_tool_version() ) ;

		to_info->set_modified_by( from_info->get_modified_by() ) ;
		to_info->set_modified_on( from_info->get_modified_on() ) ;
	}

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
	void memory_local::perform_add( key_type &key, record_pointer record  )
	{
		record->set_cmp_maker(&m_cmp_maker) ;
		m_records[key] = record ;

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
		make_dirty() ;
	}
	bool memory_local::should_add(key_type &key, record_pointer record)
	{
		if (m_records.find(key) == m_records.end())
		{
			return true ;
		}

		if (record->get_modified() > m_records[key]->get_modified())
		{
			return true ;
		}
		return false;
	}

	bool memory_local::add_record(record_pointer record)
	{
		if ( m_header.is_locked() )
		{
			return false ;
		}
		if ( ! record->is_valid_record() ) 
		{
			throw except::CBadRecordException( IDS_TRANS_OR_SOURCE_EMPTY ) ;
		}

		if (record->get_context_rich().empty() && ! m_default_context.empty())
		{
			record->set_context(m_default_context) ;
		}


		// Only check when conditions are right
		if (should_check_for_demo())
		{
			do_demo_check() ;
		}

		key_type key = this->get_key(record) ;
		const bool was_added = this->should_add(key, record);

		if (was_added)
		{
			perform_add(key, record);
		}

		return was_added ;
	}

	void memory_local::get_match_candidates( trans_set &candidates, const wstring query, double min_score )
	{
		Distance distance ;
		distance.set_minscore(min_score) ;
		Segment segment(&m_cmp_maker, query) ;
		const wstring query_cmp = segment.cmp() ;

		foreach ( record_pointer record, m_records | ad::map_values )
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
		Segment segment(&m_cmp_maker, query) ;
		const wstring query_cmp = segment.cmp() ;

		foreach ( record_pointer record, m_records | ad::map_values )
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
		foreach ( record_pointer record, m_records | ad::map_values )
		{
			const double score = distance.edist_score(query_cmp, record->get_source_cmp()) ;
			best_score = max(score, best_score) ;
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
		if ( m_gloss_properties->get_min_score() < max_score )
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
		tester.set_search_match(this->make_match()) ;
		foreach(record_pointer record, m_records | ad::map_values)
		{
			if ( tester.is_match( record ) )
			{
				search_match_ptr match = tester.get_search_match( record ) ;
				const wstring location = static_cast<LPCWSTR>( m_file_location ) ;
				match->set_memory_location( location ) ;
				match->set_memory_id( get_id() ) ;
				matches.insert( match ) ;
				tester.set_search_match(this->make_match()) ;
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

		// if we didn't erase anything, then don't mark
		// the memory as dirty. 

		const bool was_erased = record_exists(record) ;
		if ( was_erased )
		{
			key_type key = this->get_key(record) ;
			m_records.erase(key) ;
			make_dirty() ;
		}

		return was_erased ;	// didn't erase anything.
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
		m_is_loading = false ;
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
			if ( m_listener != NULL ) 
			{
				m_listener->OnProgressInit(m_file_location, 0, static_cast< int >( size() ) ) ;
			}

			// create the xml document
			const string rec_start =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
				"<!-- Created by Felix v " + string(VERSION) + " -->\r\n"
				"<memory>\r\n";
			output_device.write( rec_start ) ;

			m_header.set_count( static_cast< long >( size() ) ) ;
			m_header.write_header( &output_device ) ;

			output_device.write( "<records>\r\n" ) ;

			CRecord2XmlConverter converter( &output_device ) ;

			ATLTRACE( "Saving records...\n" ) ;

			int current_record = 0 ;
			// loop through each of the records
			foreach(record_pointer record, m_records | ad::map_values)
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

		input_device_ptr input(new InputDeviceFile) ;
		input->ensure_file_exists(get_location());

		return is_saved() ;
	}

	mem_engine::record_pointer memory_local::add_by_id( size_t recid, const wstring source, const wstring trans )
	{
		foreach(record_pointer rec, m_records | ad::map_values)
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

		new_rec->set_cmp_maker(&m_cmp_maker) ;
		m_records[get_key(new_rec)] = new_rec ;

		// memory is now dirty
		make_dirty();
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
			foreach(record_pointer record, m_records | ad::map_values)
			{
				record->set_cmp_maker(&m_cmp_maker) ;
			}
		}
	}
	void memory_local::get_gloss_100(search_match_container& matches, const search_query_params& params)
	{
		Segment query(&m_cmp_maker, params.m_source) ;
		gloss_match_tester tester(query.cmp()) ;
		tester.set_search_match(this->make_match()) ;

		foreach( record_pointer record, m_records | ad::map_values)
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
		set_minimum_score( m_gloss_properties->get_min_score() ) ;
		Distance distance ;
		const double min_score = m_match_maker.get_minimum_score() ;
		distance.set_minscore(min_score) ;
		Segment haystack(&m_cmp_maker, params.m_rich_source) ;

		search_match_ptr match(this->make_match()) ;
		foreach(record_pointer record, m_records | ad::map_values)
		{
			if (distance.subdist_score(record->get_source_cmp(), haystack.cmp()) >= min_score)
			{
				Segment needle(&m_cmp_maker, record->get_source_rich()) ;
				match->set_record(record) ;
				if(m_match_maker.fuzzy_gloss_score(needle, haystack, match))
				{
					set_gloss_fuzzy_match(matches, params, match);	
					match = this->make_match() ;
				}
			}
		}

		set_minimum_score( m_mem_properties->get_min_score() ) ;
	}
	bool memory_local::load( const CString& file_name )
	{
		try
		{
			if ( m_header.is_locked() )
			{
				logging::log_debug("Memory is locked; cannot load") ;
				return false ;
			}
			LOG_VERBOSE(string("Loading memory") + string((LPCSTR)CStringA(file_name))) ;

			m_is_loading = true ;
			// input assumptions
			ATLASSERT( file_name.IsEmpty() == false ) ;
			input_device_ptr input(new InputDeviceFile) ;
			input->ensure_file_exists(file_name);

			// see if we are a demo...
			refresh_status() ;

			// *** TODO: Consider deletion
			// Probably not needed, since we now set the user name on Felix startup,
			// and refresh it whenever the preferences are changed.
			refresh_user_name() ;

			detect_date_created(file_name, input) ;

			const unsigned int file_len = input->get_size(file_name) ;

			if ( file_len == 0 )
			{
				logging::log_warn("Trying to load empty file") ;
				throw except::CException( IDS_CORRUPT_FILE ) ;
			}

			const size_t original_num_records = size() ;
			// create a view of the xml document
			file::view memory_view ;
			char *raw_text = input->create_view_char( file_name ) ;

			bool was_saved = load_text(raw_text, file_name, file_len);

			postLoadCleanup(file_name, was_saved, original_num_records);

			set_location(file_name) ;
			input->ensure_file_exists(get_location());

		}
		catch (...)
		{
			m_is_loading = false ;
			throw ;
		}
		m_is_loading = false ;
		return true ;
	}
	bool memory_local::clear_memory()
	{
		m_is_saved = m_records.empty() ;
		m_file_location.Empty() ;
		m_records.clear() ;
		m_ids.clear() ;
		return true ;
	}
	bool memory_local::record_exists(record_pointer rec)
	{
		return ( m_records.find( get_key(rec) ) != m_records.end() ) ;
	}
	record_pointer memory_local::get_record_at(const size_t index)
	{
		if (index >= m_records.size())
		{
			throw CProgramException(_T("Requested record index is out of bounds")) ;
		}

		auto pos = m_records.begin() ;
		std::advance( pos, index ) ;
		return pos->second ;
	}
	// tabulate_fonts
	void memory_local::tabulate_fonts(font_tabulator &tabulator)
	{
		foreach(record_pointer record, m_records | ad::map_values)
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

		foreach ( record_pointer record, m_records | ad::map_values )
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
		const double num_validated = 
			(double)std::count_if(m_records.begin(),
							  m_records.end(),
							  [](record_collection_type::value_type const & r){ return r.second->is_validated(); }
							 ) ;
		return double2percent_wstring( num_validated / (double)m_records.size()  ) ;

	}

	void memory_local::search_no_regex( const search_query_params & params, search_match_container &matches ) 
	{
		search_match_tester tester( params ) ;
		tester.set_search_match(this->make_match()) ;
		foreach( record_pointer record, m_records | ad::map_values )
		{
			if ( tester.is_match( record ) )
			{
				search_match_ptr match = tester.get_search_match( record ) ;
				const wstring location = static_cast<LPCWSTR>( m_file_location ) ;
				match->set_memory_location( location ) ;
				match->set_memory_id( get_id() ) ;
				matches.insert( match ) ;
				tester.set_search_match(this->make_match()) ;
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
		foreach(record_pointer record, m_records | ad::map_values)
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
		foreach( record_pointer record, m_records | ad::map_values )
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
					throw except::CBadRecordException( IDS_CORRUPT_FILE ) ;
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
			catch ( CBadRecordException &exception )
			{
				continue_or_throw(exception);
			}			
			catch ( CException &exception )
			{
				handle_cexception_on_load( file_name, was_saved, exception);
			}
			catch( std::exception &std_exception )
			{
				handle_stdexception_on_load( was_saved, file_name, std_exception);
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
			const size_t destSize = static_cast< size_t >( file_len / 2 + 1 ) ;
			const size_t srcSize = static_cast< size_t >( file_len / 2 ) ;
			LPCWSTR src = (LPCWSTR)( raw_text ) ;
			LPWSTR dest = wide_buffer.GetBufferSetLength( destSize+1 ) ;

			_tcsncpy_s(dest, 
				destSize, 
				src, 
				srcSize ) ;
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

	void memory_local::handle_cexception_on_load( const CString& file_name, bool was_saved, CException& e ) 
	{
		if ( this->is_demo() )
		{
			// If the exception was due to an attempt to load too many
			// entries, we'll pass it up, cancelling the load.
			if ( size() >= MAX_MEMORY_SIZE_FOR_DEMO )
			{
				set_saved_flag( was_saved ) ;
				SENSE("CDemoException") ;
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

	MemoryInfo* memory_local::get_memory_info()
	{
		return static_cast<MemoryInfo*>(&m_header) ;
	}

	const MemoryInfo* memory_local::get_memory_info_const() const
	{
		return static_cast<const MemoryInfo*>(&m_header) ;
	}

	bool memory_local::is_local()
	{
		return true ;
	}

	CString memory_local::get_fullpath()
	{
		return this->get_location() ;
	}

	/* We call this after an action on the memory that
	   will change the saved file:

			- Adding a record
			- Removing a record
			- Changing memory properties
	*/
	void memory_local::make_dirty()
	{
		if (m_is_loading)
		{
			return ;
		}
		this->set_saved_flag(false) ;
		m_header.set_modified_now() ;
		m_header.modified_by_current_user() ;
	}

	void memory_local::set_mem_location( CString location )
	{
		this->set_location( location ) ;
		if ( ! this->empty() )
		{
			this->set_saved_flag( false ) ; // new location, so we have not saved to here before
		}
	}

}