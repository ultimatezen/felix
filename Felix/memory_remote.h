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

		void batch_set_reliability( size_t rel );
		void batch_set_validation( bool val );
		wstring get_validated_percent();
		void get_reliability_stats( size_t &low, size_t &high, double &ave );


		trans_set& get_records();
	    bool connect(CString conn_str);
		bool add_record ( record_pointer record );
		size_t size();
		bool empty();
		CString get_location( );
		void set_location( CString location );

		CString get_fullpath();
		bool is_new();
		bool save_memory( );
		bool is_local();
		bool load( const CString &);
		void load_header( const CString &);
		void tabulate_fonts( font_tabulator &);
		record_pointer get_record_at( const size_t index );
		void set_cmp_params( const search_query_params &);
		bool record_exists( record_pointer rec );
		bool clear_memory();

		bool erase( const record_pointer record );
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
		void replace(const record_pointer old_rec, record_pointer new_rec);
	};

}