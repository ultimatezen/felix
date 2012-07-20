#pragma once

#include "TranslationMemory.h"
#include "DispatchWrapper.h"
#include "record_remote.h"
#include "memory_info.h"

namespace mem_engine
{

	class memory_remote : 
			public CTranslationMemory,
			public MemoryInfo
	{
	public:
		CDispatchWrapper m_engine ;
		CString m_conn_str ;

		memory_remote(app_props::props_ptr props,
					  double min_score=0.5f, 
					  LPCWSTR server_name = L"Felix.RemoteMemory") : 
		  CTranslationMemory(props, min_score), m_engine(server_name)
		{
			m_match_maker.set_minimum_score(min_score) ;
			m_is_saved = true ;
		}
	
		  // We really need to plug this leak, but for right now I can't find it!
	    ~memory_remote()
		{
		}

		bool connect(CString conn_str, CString username = _T(""), CString password = _T(""));
		bool login(CString username, CString password);

		/************************************************************************/
		/* MemoryInfo implementation                                            */
		/************************************************************************/

		MemoryInfo* get_memory_info()
		{
			return static_cast<MemoryInfo*>(this) ;
		}

		const MemoryInfo* get_memory_info_const() const
		{
			return static_cast<const MemoryInfo*>(this) ;
		}
		// getters
		bool is_locked() const;
		bool is_locked() ;

		wstring get_current_user();

		wstring get_creator() const;
		wstring get_field() const;
		wstring get_created_on() const;

		wstring get_modified_by() const;
		wstring get_modified_on() const;

		wstring get_source_language() const;
		wstring get_target_language() const;
		wstring get_client() const;
		wstring get_creation_tool() const;
		wstring get_creation_tool_version() const;
		long get_count() const;
		bool is_memory() const;

		// setters
		void set_creator( const wstring setting );
		void set_field( const wstring setting );
		void set_created_on( const wstring setting );
		void set_source_language( const wstring setting );
		void set_target_language( const wstring setting );
		void set_client( const wstring setting );
		void set_count( const long setting );
		void set_is_memory_on();
		void set_is_memory_off();
		void set_creation_tool( const wstring setting );
		void set_creation_tool_version( const wstring setting );

		void set_modified_by(const wstring modified_by);
		void set_modified_on(const wstring modified_on);
		void set_modified_now();
		void modified_by_current_user();

		void set_locked_off();
		void set_locked_on();
		void set_creator_to_current_user();
		void set_is_memory(bool setting);
		void set_is_locked(bool setting);

		/************************************************************************/
		/* TranslationMemory implementation                                     */
		/************************************************************************/

		// memory name
		CString get_location( );
		void set_location( CString location );
		void set_mem_location(CString location);
		CString get_fullpath();
		bool is_new();

		// batch/stats ops
		void batch_set_reliability( size_t rel );
		void batch_set_validation( bool val );
		wstring get_validated_percent();
		rel_tuple get_reliability_stats();

		record_collection_type& get_records();
		bool add_record ( record_pointer record );
		void replace(const record_pointer old_rec, record_pointer new_rec);
		size_t size();
		bool empty();
		bool save_memory();
		bool is_local();
		void tabulate_fonts( font_tabulator &);
		record_pointer get_record_at( const size_t index );
		void set_cmp_params( const search_query_params &);
		bool record_exists( record_pointer rec );
		bool clear_memory();

		bool erase( const record_pointer record );
		void get_match_candidates(trans_set &candidates, 
			const wstring query,
			double min_score);
		void get_rmatch_candidates(trans_set &candidates, 
			const wstring query,
			double min_score);

		void convert_candidates(trans_set &candidates, CComVariant disp);
		double get_best_match_score( const wstring query );
		bool get_glossary_matches( search_match_container &matches, 
									const search_query_params &params );
		bool perform_search( search_match_container &matches, 
			const search_query_params &params );

		record_pointer add_by_id(size_t recid, const wstring source, const wstring trans);

		// load
		bool load( const CString &file_name );
		size_t get_perfect_matches(trans_set &records, const wstring &query);
	};

}