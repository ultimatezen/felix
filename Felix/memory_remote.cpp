#include "StdAfx.h"
#include "memory_remote.h"
#include "DemoException.h"

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
		int count = matches.prop_get(L"Count").iVal ;

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
			double score = distance.edist_score(query_cmp, record->get_source_cmp()) ;
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

		double min_score = static_cast< double >( m_gloss_properties.get_min_score() ) / 100.0f ;
		Segment query(m_cmp_maker, params.m_rich_source) ;
		const wstring query_cmp = query.cmp() ;

		CComVariant com_matches = this->m_engine.method(L"Gloss", query_cmp.c_str(), min_score) ;
		trans_set candidates ;
		this->convert_candidates(candidates, com_matches) ;

		if ( m_gloss_properties.get_min_score() < 100 )
		{
			set_minimum_score( m_gloss_properties.get_min_score() ) ;
			Segment haystack(m_cmp_maker, params.m_rich_source) ;

			search_match_ptr match(new search_match) ;
			foreach(record_pointer record, candidates)
			{
				Segment needle(m_cmp_maker, record->get_source_rich()) ;
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
			const wstring query_cmp = m_cmp_maker.make_cmp(params.m_source) ;
			CComVariant com_matches = this->m_engine.method(L"Concordance", query_cmp.c_str()) ;
			this->convert_candidates(candidates, com_matches) ;
		}
		else if (! params.m_trans.empty())
		{
			const wstring query_cmp = m_cmp_maker.make_cmp(params.m_trans) ;
			CComVariant com_matches = this->m_engine.method(L"ReverseConcordance", query_cmp.c_str()) ;
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

	bool memory_remote::erase( record_pointer record )
	{
		try
		{
			record_remote rec(record) ;
			this->m_engine.method(L"DeleteRecord", rec.get_engine()) ;
		}
		catch (_com_error& e)
		{
			BANNER("Error in memory_remote::erase") ;
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;

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
		if ( is_demo() )
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
				remrec->set_id(recid) ;
				remrec->set_source(source) ;
				remrec->set_trans(trans) ;
				this->m_engine.method(L"AddRecord", remrec->get_engine()) ;
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
}
