#pragma once
#include "TranslationMemory.h"
#include "memory_header.h"

namespace memory_engine
{
	class memory_local : public CTranslationMemory
	{
		VISIBLE_TO_TESTS

		memory_header		m_header ;
		size_t				m_next_id ;
		std::set<size_t>	m_ids ;
		trans_set			m_records ;
		CString				m_file_location ;
	public:
		memory_local(double min_score=0.5f) : 
		  CTranslationMemory(min_score),
			  m_next_id(1)
		  {}

		  // MemoryInfo
		  MemoryInfo* get_memory_info()
		  {
			  return static_cast<MemoryInfo*>(&m_header) ;
		  }
		  const MemoryInfo* get_memory_info_const() const
		  {
			  return static_cast<const MemoryInfo*>(&m_header) ;
		  }
		  void set_locked_off();
		  void set_locked_on( );
		  bool is_locked();

			// loading
		  void load_header_raw_text( char * raw_text, int file_len );
		  bool load_text( char * raw_text, const CString& file_name, unsigned int file_len );
		  void loadWideBuffer(const char* raw_text, int file_len, CStringW& wide_buffer);
		  void postLoadCleanup(const ATL::CString& file_name, bool was_saved, size_t original_num_records);
		  void loadRecords(const ATL::CString& file_name, textstream_reader< wchar_t >& reader, int progress_interval, bool was_saved);
		  bool load( const CString &file_name ) ;
		  void load_header( const CString &location );
		  int setProgressInterval(int num_records) ;
		  void handleCExceptionOnLoad( const CString& file_name, bool was_saved, CException& e ) ;

		  // TranslationMemory
		  bool add_record(record_pointer record) ;
		  trans_set& get_records() {return m_records ; }
		  iterator_type begin( ) { return m_records.begin() ; }
		  iterator_type end( ) { return m_records.end() ; }

		  size_t get_next_id();
		  size_t size() ;
		  bool empty();
		  bool erase( const record_pointer record ) ;
		  CString get_location( );
		  void set_location( CString location ) ;
		  CString get_fullpath()
		  {
			  return this->get_location() ;
		  }
		  bool is_new();
		  bool save_memory( )  ;
		  bool is_local()
		  {
			  return true ;
		  }
		  void set_cmp_params( const search_query_params &params ) ;

		  void get_match_candidates(trans_set &candidates, 
			  const wstring &query,
			  double min_score);
		  void get_rmatch_candidates(trans_set &candidates, 
			  const wstring &query,
			  double min_score);
		  double get_best_match_score(const wstring &query);
		  bool get_glossary_matches( search_match_multiset &matches, 
			  const search_query_params &params ) ;
		  bool perform_search( search_match_multiset &matches, 
			  const search_query_params &params ) ;
		  record_pointer add_by_id(size_t recid, const wstring source, const wstring trans);
		  void replace(const record_pointer old_rec, record_pointer new_rec);
		  void get_gloss_100(search_match_multiset& matches, 
			  const search_query_params& params);
		  void get_gloss_fuzzy(search_match_multiset& matches, 
			  const search_query_params& params);
		  bool clear_memory();
		  bool record_exists( record_pointer rec );
		  record_pointer get_record_at( const size_t index );
		  void tabulate_fonts( font_tabulator &tabulator );
		  void get_reliability_stats( size_t &low, size_t &high, double &ave );
		  wstring get_validated_percent();
		  void search_no_regex(const search_query_params & params, search_match_multiset &matches);
		  void batch_set_reliability( size_t rel );
		  void batch_set_validation( bool val );

	};

}