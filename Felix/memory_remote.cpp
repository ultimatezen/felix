#include "StdAfx.h"
#include "memory_remote.h"
#include "DemoException.h"
#include "logging.h"

namespace memory_engine
{
	void memory_remote::get_match_candidates( trans_set &candidates, const wstring &query_cmp, double min_score )
	{
		CComVariant matches = this->m_engine.method(L"Search", query_cmp.c_str(), min_score) ;
		this->convert_candidates(candidates, matches) ;
	}

	void memory_remote::get_rmatch_candidates( trans_set &candidates, const wstring &query_cmp, double min_score )
	{
		CComVariant matches = this->m_engine.method(L"ReverseSearch", query_cmp.c_str(), min_score) ;
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
			record->set_cmp_maker(m_cmp_maker) ;
			candidates.insert(record) ;
		}
	}

	double memory_remote::get_best_match_score( const wstring &query )
	{
		trans_set candidates ;
		const wstring query_cmp = m_cmp_maker.make_cmp(query) ;

		this->get_match_candidates(candidates, query_cmp, 0.5f); 

		Distance distance ;
		double best_score = 0.0f ;
		// check each of the records for a match
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

	bool memory_remote::get_glossary_matches( search_match_multiset &matches, 
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

		const double min_score = static_cast< double >( m_gloss_properties.get_min_score() ) / 100.0f ;

		CComVariant com_matches = this->m_engine.method(L"Gloss", params.m_rich_source.c_str(), min_score) ;
		trans_set candidates ;
		this->convert_candidates(candidates, com_matches) ;

		if ( m_gloss_properties.get_min_score() < 100 )
		{
			set_minimum_score( m_gloss_properties.get_min_score() ) ;
			const Segment haystack(m_cmp_maker, params.m_rich_source) ;

			search_match_ptr match(new search_match) ;
			foreach(record_pointer record, candidates)
			{
				const Segment needle(m_cmp_maker, record->get_source_rich()) ;
				match->set_record(record) ;
				if(m_match_maker.fuzzy_gloss_score(needle, haystack, match))
				{
					set_gloss_fuzzy_match(matches, params, match);	
					match = search_match_ptr(new search_match) ;
				}
			}

			set_minimum_score( m_properties.get_min_score() ) ;
		}
		else
		{
			const Segment segment(m_cmp_maker, params.m_rich_source) ;
			const wstring query_cmp = segment.cmp() ;
			gloss_match_tester tester(query_cmp) ;

			foreach( record_pointer record, candidates)
			{
				tester.test_source( record ) ;
				set_gloss_100_char(matches, tester, record);
			}
		}
		return ( false == matches.empty() ) ;
	}

	bool memory_remote::perform_search( search_match_multiset &matches, 
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
			search_match_ptr match(new search_match) ;
			match->set_record(record) ;
			match->set_values_to_record() ;
			match->set_memory_id( get_id() ) ;
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

			CComException com_e(e) ;
			com_e.add_to_message(_T("Error removing record from remote memory")) ;
			throw com_e ;
		}
		return true ;
	}

	bool memory_remote::add_record( record_pointer record )
	{
		try
		{
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

			CComException com_e(e) ;
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
			throw CException(CString(_T("Failed to connect to memory: ")) + conn_str) ;
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

	memory_engine::record_pointer memory_remote::add_by_id( size_t recid, const wstring source, const wstring trans )
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

	void memory_remote::get_reliability_stats( size_t &low, size_t &high, double &ave )
	{
		logging::log_warn("get_reliability_stats not implemented for remote memories/glossaries.") ;
		low = 0 ;
		high = 0 ;
		ave = 0.0 ;
	}

	bool memory_remote::load( const CString & )
	{
		logging::log_warn("load not implemented for remote memories/glossaries.") ;
		return false ;
	}

	void memory_remote::load_header( const CString & )
	{
		logging::log_warn("load_header not implemented for remote memories/glossaries.") ;
	}

	void memory_remote::tabulate_fonts( font_tabulator & )
	{
		logging::log_warn("tabulate_fonts not implemented for remote memories/glossaries.") ;
	}

	memory_engine::record_pointer memory_remote::get_record_at( const size_t )
	{
		logging::log_warn("get_record_at not implemented for remote memories/glossaries.") ;
		return record_pointer(new record_remote) ;
	}

	void memory_remote::set_cmp_params( const search_query_params & )
	{
		logging::log_warn("set_cmp_params not implemented for remote memories/glossaries.") ;
	}

	bool memory_remote::record_exists( record_pointer rec )
	{
		logging::log_warn("record_exists not implemented for remote memories/glossaries.") ;
		return false ;
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
		return CString(m_engine.method(L"GetInfo", L"name").bstrVal) ;
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

	trans_set& memory_remote::get_records()
	{
		throw CProgramException("get_records not implemented for remote memories") ;
	}
}
