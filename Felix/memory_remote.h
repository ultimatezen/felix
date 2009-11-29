#pragma once

#include "TranslationMemory.h"
#include "DispatchWrapper.h"
#include "record_remote.h"

namespace memory_engine
{

	class memory_remote : public CTranslationMemory
	{
	public:
		CDispatchWrapper m_engine ;
		CString m_conn_str ;

		memory_remote(double min_score=0.5f, LPCWSTR server_name = L"Felix.RemoteMemory") : 
		  CTranslationMemory(min_score), m_engine(server_name)
		{
			m_is_saved = true ;
		}
	
		  // We really need to plug this leak, but for right now I can't find it!
	    ~memory_remote()
		{
		}
	    bool connect(CString conn_str);
		bool add_record ( record_pointer record );
		size_t size()
		{
			return static_cast< size_t >( this->m_engine.method(L"GetInfo", L"size").lVal ) ;
		}
		bool empty()
		{
			return (this->size() == 0) ;
		}
		CString get_location( )
		{
			return CString(m_engine.method(L"GetInfo", L"name").bstrVal) ;
		}
		CString get_fullpath()
		{
			return this->m_conn_str ;
		}
		bool is_new()
		{
			return false ;
		}
		bool save_memory( ) 
		{
			return true ;
		}
		bool is_local()
		{
			return false ;
		}

		bool erase( record_pointer record );
		void get_match_candidates(trans_set &candidates, 
			const wstring &query_cmp,
			double min_score);
		void get_rmatch_candidates(trans_set &candidates, 
			const wstring &query_cmp,
			double min_score);

		void convert_candidates(trans_set &candidates, CComVariant disp);
		double get_best_match_score( const wstring &query );
		bool get_glossary_matches( search_match_multiset &matches, 
									const search_query_params &params );
		bool perform_search( search_match_multiset &matches, 
			const search_query_params &params );

		record_pointer add_by_id(size_t recid, const wstring source, const wstring trans);
		void replace(record_pointer old_rec, record_pointer new_rec)
		{
			this->add_record(new_rec) ;
		}
	};

}