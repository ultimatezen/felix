/*
	Fake memory subclass used for testing
*/
#pragma once
#include "TranslationMemory.h"
#include "memory_header.h"

namespace mem_engine
{
	typedef std::pair<wstring, wstring> key_type ;
	typedef std::map<key_type, record_pointer> map_type ;

	typedef map_type record_collection_type ;

	class memory_fake : public CTranslationMemory
	{
		VISIBLE_TO_TESTS

		memory_header		m_header ;
		size_t				m_next_id ;
		std::set<size_t>	m_ids ;
		record_collection_type			m_records ;
		CString				m_file_location ;
		bool				m_is_loading ;
	public:
		bool m_is_local ;

		memory_fake(double min_score=0.5f);

		  // MemoryInfo
		  MemoryInfo* get_memory_info();
		  const MemoryInfo* get_memory_info_const() const;
		  void set_locked_off();
		  void set_locked_on( );
		  bool is_locked();

		  // map stuff
		  key_type get_key(record_pointer rec);

			// loading
		  void load_header_raw_text(const char *raw_text, size_t file_len);
		  bool load_text( char * raw_text, const CString& file_name, unsigned int file_len );
		  void loadWideBuffer(const char* raw_text, int file_len, CStringW& wide_buffer);
		  void postLoadCleanup(const ATL::CString& file_name, bool was_saved, size_t original_num_records);
		  void loadRecords(const ATL::CString& file_name, textstream_reader< wchar_t >& reader, int progress_interval, bool was_saved);
		  bool load( const CString &file_name ) ;
		  int get_progress_interval(int num_records) ;
		  void handle_cexception_on_load( const CString& file_name, bool was_saved, except::CException& e ) ;

		  void check_bail_from_load( bool was_saved, except::CException &e, const CString& file_name );
		  // TranslationMemory
		  bool add_record(record_pointer record) ;

		  void perform_add( key_type &key, record_pointer record );
		  bool should_add(key_type &key, record_pointer record);
		  bool should_check_for_demo() const ;
		  record_collection_type& get_records() {return m_records ; }

		  size_t get_next_id();
		  size_t size() ;
		  bool empty();
		  bool erase( const record_pointer record ) ;
		  CString get_location( );
		  void set_mem_location(CString location);
		  void set_location( CString location ) ;
		  CString get_fullpath();
		  bool is_new();
		  bool save_memory( )  ;
		  bool is_local()
		  {
			  return m_is_local ;
		  }
		  void set_cmp_params( const search_query_params &params ) ;

		  void get_match_candidates(trans_set &candidates, 
			  const wstring query,
			  double min_score);
		  void get_rmatch_candidates(trans_set &candidates, 
			  const wstring query,
			  double min_score);
		  double get_best_match_score(const wstring query);
		  bool get_glossary_matches( search_match_container &matches, 
			  const search_query_params &params ) ;
		  bool perform_search( search_match_container &matches, 
			  const search_query_params &params ) ;
		  record_pointer add_by_id(size_t recid, const wstring source, const wstring trans);
		  void replace(const record_pointer old_rec, record_pointer new_rec);

		  void make_dirty();
		  void get_gloss_100(search_match_container& matches, 
			  const search_query_params& params);
		  void get_gloss_fuzzy(search_match_container& matches, 
			  const search_query_params& params);
		  bool clear_memory();
		  bool record_exists( record_pointer rec );
		  record_pointer get_record_at( const size_t index );
		  void tabulate_fonts( font_tabulator &tabulator );
		  rel_tuple get_reliability_stats();
		  wstring get_validated_percent();
		  void search_no_regex(const search_query_params & params, search_match_container &matches);
		  void batch_set_reliability( size_t rel );
		  void batch_set_validation( bool val );

		  size_t get_perfect_matches(search_match_container &matches, const wstring &query);

	};

	void copy_mem_info(memory_pointer from, memory_pointer to) ;
}