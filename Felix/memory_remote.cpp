#include "StdAfx.h"
#include "memory_remote.h"
#include "DemoException.h"
#include "logging.h"

namespace mem_engine
{
	using namespace except ;

	void memory_remote::get_match_candidates( trans_set &candidates, const wstring query, double min_score )
	{
		CComVariant matches = this->m_engine.method(L"Search", query.c_str(), min_score) ;
		this->convert_candidates(candidates, matches) ;
	}

	void memory_remote::get_rmatch_candidates( trans_set &candidates, const wstring query, double min_score )
	{
		CComVariant matches = this->m_engine.method(L"ReverseSearch", query.c_str(), min_score) ;
		this->convert_candidates(candidates, matches) ;
	}

	void memory_remote::convert_candidates( trans_set &candidates, CComVariant disp )
	{
		CDispatchWrapper matches(disp.pdispVal) ;
		const int count = matches.prop_get(L"Count").iVal ;

		for (int i = 0 ; i < count ; ++i)
		{
			CComVariant match = matches.method(L"Item", i) ;
			record_pointer record(new record_remote(CDispatchWrapper(match.pdispVal))) ;
			record->set_cmp_maker(&m_cmp_maker) ;
			candidates.insert(record) ;
		}
	}

	double memory_remote::get_best_match_score( const wstring query )
	{
		trans_set candidates ;

		this->get_match_candidates(candidates, query, 0.5f); 

		Distance distance ;
		double best_score = 0.0f ;
		// check each of the records for a match
		Segment segment(&m_cmp_maker, query) ;
		const wstring query_cmp = segment.cmp() ;
		foreach ( record_pointer record, candidates )
		{
			const double score = distance.edist_score(query_cmp, record->get_source_rich()) ;
			if (score > best_score)
			{
				best_score = score ;
			}
		}

		return best_score ;
	}

	bool memory_remote::get_glossary_matches( search_match_container &matches, 
												const search_query_params &params )
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

		const double min_score = static_cast< double >( m_gloss_properties->get_min_score() ) / 100.0f ;

		CComVariant com_matches = this->m_engine.method(L"Gloss", params.m_rich_source.c_str(), min_score) ;
		trans_set candidates ;
		this->convert_candidates(candidates, com_matches) ;

		if ( m_gloss_properties->get_min_score() < 100 )
		{
			set_minimum_score( m_gloss_properties->get_min_score() ) ;
			const Segment haystack(&m_cmp_maker, params.m_rich_source) ;

			search_match_ptr match(this->make_match()) ;
			foreach(record_pointer record, candidates)
			{
				const Segment needle(&m_cmp_maker, record->get_source_rich()) ;
				match->set_record(record) ;
				if(m_match_maker.fuzzy_gloss_score(needle, haystack, match))
				{
					set_gloss_fuzzy_match(matches, params, match);	
					match = this->make_match() ;
				}
			}

			set_minimum_score( m_properties->get_min_score() ) ;
		}
		else
		{
			const Segment segment(&m_cmp_maker, params.m_rich_source) ;
			const wstring query_cmp = segment.cmp() ;
			gloss_match_tester tester(query_cmp) ;

			foreach( record_pointer record, candidates)
			{
				search_match_ptr match(this->make_match()) ;
				match->set_record(record) ;
				match->set_values_to_record() ;
				matches.insert( match ) ;
			}
		}
		return ( false == matches.empty() ) ;
	}

	bool memory_remote::perform_search( search_match_container &matches, 
										const search_query_params &params )
	{
		if( ! m_is_active )
		{
			return true ;
		}

		// No support for regular expressions...
		if ( params.m_use_regex )
		{
			return false ;
		}

		trans_set candidates ;

		if (! params.m_source.empty())
		{
			CComVariant com_matches = this->m_engine.method(L"Concordance", params.m_source.c_str()) ;
			this->convert_candidates(candidates, com_matches) ;
		}
		else if (! params.m_trans.empty())
		{
			CComVariant com_matches = this->m_engine.method(L"ReverseConcordance", params.m_trans.c_str()) ;
			this->convert_candidates(candidates, com_matches) ;
		}

		foreach(record_pointer record, candidates)
		{
			search_match_ptr match(this->make_match()) ;
			match->set_record(record) ;
			match->set_values_to_record() ;
			matches.insert(match);
		}
		return ( ! matches.empty() ) ;
	}

	bool memory_remote::erase( const record_pointer record )
	{
		try
		{
			record_remote rec(record) ;
			this->m_engine.method(L"DeleteRecord", rec.get_engine()) ;
		}
		catch (_com_error& e)
		{
			BANNER("Error in memory_remote::erase") ;

			logging::log_error("Failed to remove record from remote memory") ;

			except::CComException com_e(e) ;
			com_e.add_to_message(_T("Error removing record from remote memory")) ;
			throw com_e ;
		}
		return true ;
	}

	bool memory_remote::add_record( record_pointer record )
	{
		try
		{

			if (record->get_context_rich().empty() && ! m_default_context.empty())
			{
				record->set_context(m_default_context) ;
			}
			record_remote rec(record) ;
			this->m_engine.method(L"AddRecord", rec.get_engine()) ;
			record->set_id(rec.get_id()) ;
		}
		catch (_com_error& e)
		{
			BANNER("Error in memory_remote::add_record") ;
			logging::log_error("Failed to add remote record") ;
			logging::log_exception(e) ;

			ATLASSERT("Failed to add remote record" && FALSE) ;

			CComException com_e(e) ;
			com_e.add_to_message(_T("Error adding record to remote memory")) ;
			throw com_e ;
		}
		return true ;
	}

	void memory_remote::replace( const record_pointer old_rec, record_pointer new_rec )
	{
		try
		{
			record_remote rec(new_rec) ;
			rec.set_id(old_rec->get_id()) ;
			this->m_engine.method(L"UpdateRecord", rec.get_engine()) ;
		}
		catch (_com_error& e)
		{
			BANNER("Error in memory_remote::add_record") ;
			logging::log_error("Failed to update remote record") ;
			logging::log_exception(e) ;

			ATLASSERT("Failed to add remote record" && FALSE) ;

			except::CComException com_e(e) ;
			com_e.add_to_message(_T("Error adding record to remote memory")) ;
			throw com_e ;
		}
	}

	bool memory_remote::connect( CString conn_str )
	{
		m_conn_str = conn_str ;
		bool success = (VARIANT_FALSE != m_engine.method(L"Connect", static_cast< LPCWSTR >( CT2W(conn_str) )).boolVal) ;
		if (! success)
		{
			throw except::CException(CString(_T("Failed to connect to memory: ")) + conn_str) ;
		}
		// check for demo status
		refresh_status() ;
		if ( this->is_demo() )
		{
			if ( size() > MAX_MEMORY_SIZE_FOR_DEMO + 100 )
			{
				throw CDemoException() ;
			}
		}
		return success ;
	}

	mem_engine::record_pointer memory_remote::add_by_id( size_t recid, const wstring source, const wstring trans )
	{
		try
		{
			CComVariant id((long)recid) ;
			CComVariant recvar = this->m_engine.method(L"RecordById", id) ;
			if (recvar.vt == VT_NULL)
			{
				record_remote *remrec = new record_remote(CDispatchWrapper(L"Felix.RemoteRecord")) ;
				// prevent memory leaks
				try
				{
					remrec->set_id(recid) ;
					remrec->set_source(source) ;
					remrec->set_trans(trans) ;
					this->m_engine.method(L"AddRecord", remrec->get_engine()) ;
				}
				catch (...)
				{
					if (remrec)
					{
						delete remrec ;
					}
					throw ;				
				}
				return record_pointer(remrec) ;
			}
			record_pointer record(new record_remote(CDispatchWrapper(recvar.pdispVal))) ;
			record->set_source(source) ;
			record->set_trans(trans) ;
			return record ;
		}
		catch (_com_error& e)
		{
			BANNER("Error in memory_remote::add_record") ;
			logging::log_error("Failed to retrieve remote record by id") ;
			logging::log_exception(e) ;

			ATLASSERT("Failed to retrieve remote record" && FALSE) ;
			CComException com_e(e) ;
			com_e.add_to_message(_T("Error adding record to remote memory")) ;
			throw com_e ;
		}
	}

	void memory_remote::batch_set_reliability( size_t )
	{
		logging::log_warn("batch_set_reliability not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::batch_set_validation( bool )
	{
		logging::log_warn("batch_set_validation not implemented for remote memories/glossaries.") ;
	}

	wstring memory_remote::get_validated_percent()
	{
		logging::log_warn("get_validated_percent not implemented for remote memories/glossaries.") ;
		return wstring() ;
	}

	rel_tuple memory_remote::get_reliability_stats()
	{
		logging::log_warn("get_reliability_stats not implemented for remote memories/glossaries.") ;
		return boost::make_tuple(0, 0, 0.0) ;
	}

	void memory_remote::tabulate_fonts( font_tabulator & )
	{
		logging::log_warn("tabulate_fonts not implemented for remote memories/glossaries.") ;
	}

	mem_engine::record_pointer memory_remote::get_record_at( const size_t )
	{
		logging::log_warn("get_record_at not implemented for remote memories/glossaries.") ;
		return record_pointer(new record_remote) ;
	}

	void memory_remote::set_cmp_params( const search_query_params &params )
	{
		if (m_cmp_maker.m_ignore_case != params.m_ignore_case)
		{
			m_cmp_maker.m_ignore_case = params.m_ignore_case ;
		}
		if (m_cmp_maker.m_ignore_hira_kata != params.m_ignore_hira_kata)
		{
			m_cmp_maker.m_ignore_hira_kata = params.m_ignore_hira_kata ;
		}
		if (m_cmp_maker.m_ignore_width != params.m_ignore_width)
		{
			m_cmp_maker.m_ignore_width = params.m_ignore_width ;
		}
	}

	bool memory_remote::record_exists( record_pointer rec )
	{
		try
		{
			CComVariant id((long)rec->get_id()) ;
			CComVariant recvar = this->m_engine.method(L"RecordById", id) ;
			return recvar.vt != VT_NULL;
		}
		catch (_com_error& e)
		{
			BANNER("Error in memory_remote::record_exists") ;
			logging::log_error("record_exists: Failed to retrieve remote record by id") ;
			logging::log_exception(e) ;

			return false ;
		}
	}

	bool memory_remote::clear_memory()
	{
		logging::log_warn("clear_memory not implemented for remote memories/glossaries.") ;
		return false ;
	}

	void memory_remote::set_location( CString )
	{
		logging::log_warn("set_location not implemented for remote memories/glossaries.") ;
	}

	size_t memory_remote::size()
	{
		return static_cast< size_t >( this->m_engine.method(L"GetInfo", L"size").lVal ) ;
	}

	bool memory_remote::empty()
	{
		return (this->size() == 0) ;
	}

	CString memory_remote::get_location()
	{
		try
		{
			return CString(m_engine.method(L"GetInfo", L"name").bstrVal) ;
		}
		catch(_com_error &e)
		{
			logging::log_error("memory_remote: failed to retrieve memory name") ;
			logging::log_exception(e) ;
			throw except::CComException(e) ;
		}

	}

	CString memory_remote::get_fullpath()
	{
		return this->m_conn_str ;
	}

	bool memory_remote::is_new()
	{
		return false ;
	}

	bool memory_remote::save_memory()
	{
		return true ;
	}

	bool memory_remote::is_local()
	{
		return false ;
	}

	record_collection_type& memory_remote::get_records()
	{
		throw CProgramException("get_records not implemented for remote memories") ;
	}

	bool memory_remote::load( const CString &file_name )
	{
		file_name ;
		throw CProgramException("load not implemented for remote memories") ;
	}

	/************************************************************************/
	/* MemoryInfo                                                           */
	/************************************************************************/

	// None of these are implemented yet...

	void memory_remote::set_locked_off()
	{
		logging::log_warn("set_locked_off not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_locked_on()
	{
		logging::log_warn("set_locked_on not implemented for remote memories/glossaries.") ;
	}

	bool memory_remote::is_locked()
	{
		logging::log_warn("is_locked not implemented for remote memories/glossaries.") ;
		return false;
	}
	bool memory_remote::is_locked() const
	{
		logging::log_warn("is_locked not implemented for remote memories/glossaries.") ;
		return false;
	}
	void memory_remote::set_is_memory(bool)
	{
		logging::log_warn("set_is_memory not implemented for remote memories/glossaries.") ;
	}
	void memory_remote::set_is_locked(bool)
	{
		logging::log_warn("set_is_locked not implemented for remote memories/glossaries.") ;
	}

	wstring memory_remote::get_current_user()
	{
		app_props::properties_general props ;
		props.read_from_registry() ;
		const wstring user_name = CT2W(props.m_data.m_user_name) ;
		set_record_username(user_name) ;
		return user_name ;
	}

	/*
	info fields:
		modified_by
		name
		creator
		notes
		source_language
		normalize_case
		normalize_hira
		created_on
		client
		normalize_width
		modified_on
		memtype
		size
		id
		target_language
	*/

	// getters

	wstring memory_remote::get_creator() const
	{
		logging::log_debug("memory_remote: retrieving memory creator") ;
		return BSTR2wstring(const_cast<memory_remote*>(this)->m_engine.method(L"GetInfo", L"creator").bstrVal) ;
	}

	wstring memory_remote::get_field() const
	{
		return wstring() ;
	}

	wstring memory_remote::get_created_on() const
	{
		logging::log_debug("memory_remote: retrieving memory creation date") ;
		CComVariant created = const_cast<memory_remote*>(this)->m_engine.method(L"GetInfo", L"created_on");
		created.ChangeType(VT_BSTR) ;
		return BSTR2wstring(created.bstrVal) ;
	}

	wstring memory_remote::get_modified_by() const
	{
		logging::log_debug("memory_remote: retrieving memory modifier name") ;
		return BSTR2wstring(const_cast<memory_remote*>(this)->m_engine.method(L"GetInfo", L"modified_by").bstrVal) ;
	}

	wstring memory_remote::get_modified_on() const
	{
		logging::log_debug("memory_remote: retrieving memory modified date") ;
		CComVariant modified = const_cast<memory_remote*>(this)->m_engine.method(L"GetInfo", L"modified_on");
		modified.ChangeType(VT_BSTR) ;
		return BSTR2wstring(modified.bstrVal) ;
	}

	wstring memory_remote::get_source_language() const
	{
		logging::log_debug("memory_remote: retrieving memory source language") ;
		return BSTR2wstring(const_cast<memory_remote*>(this)->m_engine.method(L"GetInfo", L"source_language").bstrVal) ;
	}

	wstring memory_remote::get_target_language() const
	{
		logging::log_debug("memory_remote: retrieving memory target language") ;
		return BSTR2wstring(const_cast<memory_remote*>(this)->m_engine.method(L"GetInfo", L"target_language").bstrVal) ;
	}

	wstring memory_remote::get_client() const
	{
		logging::log_debug("memory_remote: retrieving memory client") ;
		return BSTR2wstring(const_cast<memory_remote*>(this)->m_engine.method(L"GetInfo", L"client").bstrVal) ;
	}

	wstring memory_remote::get_creation_tool() const
	{
		return wstring(L"MemoryServes") ;
	}

	wstring memory_remote::get_creation_tool_version() const
	{
		return string2wstring(VERSION) ;
	}

	long memory_remote::get_count() const
	{
		return static_cast<long>(const_cast<memory_remote*>(this)->size()) ;
	}

	bool memory_remote::is_memory() const
	{
		logging::log_debug("memory_remote: retrieving memory type") ;
		return BSTR2wstring(const_cast<memory_remote*>(this)->m_engine.method(L"GetInfo", L"memtype").bstrVal) == L"m";
	}

	// setters

	void memory_remote::set_creator(const wstring )
	{
		logging::log_warn("set_creator not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_field(const wstring )
	{
		logging::log_warn("set_field not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_created_on(const wstring )
	{
		logging::log_warn("set_created_on not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_source_language(const wstring )
	{
		logging::log_warn("set_source_language not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_target_language(const wstring )
	{
		logging::log_warn("set_target_language not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_client(const wstring )
	{
		logging::log_warn("set_client not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_count(const long )
	{
		logging::log_warn("set_count not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_is_memory_on()
	{
		logging::log_warn("set_is_memory_on not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_is_memory_off()
	{
		logging::log_warn("set_is_memory_off not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_creation_tool(const wstring )
	{
		logging::log_warn("set_creation_tool not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_creation_tool_version(const wstring )
	{
		logging::log_warn("set_creation_tool_version not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_modified_by(const wstring)
	{
		logging::log_warn("set_modified_by not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_modified_on(const wstring)
	{
		logging::log_warn("set_modified_on not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_modified_now()
	{
		logging::log_warn("set_modified_now not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::modified_by_current_user()
	{
		logging::log_warn("modified_by_current_user not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::set_creator_to_current_user()
	{
		logging::log_warn("set_creator_to_current_user not implemented for remote memories/glossaries.") ;
	}
}
