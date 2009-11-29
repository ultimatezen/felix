/*!
	@brief interface for the CTranslationMemory class.
	@author Ryan Ginstrom
 */

#pragma once

#include "atlstr.h"
#include "record.h"					// translation_record
#include "data_converter.h"		// data conversion (eg to/from xml)

#include "MiscWrappers.h"			// for Date class

#include "font_tabulator.h"			// gets all the fonts in a memory

#include "ui.h"							// windows_ui

#include "Felix_properties.h"	// properties_memory

#include "Scintilla_encodings.h"
#include "MultiLanguage.h"
#include "memory_header.h"

#include "ProgressListener.h"

#include "match_maker.h"

#include "UserName.h"

#include "match_tester.h"
#include "segment.h"

void ensure_file_exists( const CString & file_name );
bool is_vista_or_later() ;
CString get_load_failure_msg( const CString & file_name ) ;

namespace memory_engine
{

/**
	@struct translation_record_compare 	
	@brief Compares translation matches.
 */
struct translation_record_compare 
	: std::binary_function< record_pointer, record_pointer, bool >
{
	bool operator() ( const record_pointer &lhs, const record_pointer &rhs  ) const
	{
		if ( lhs->get_source_rich() < rhs->get_source_rich()  ) 
			return true ;
		if ( lhs->get_source_rich()  > rhs->get_source_rich()  ) 
			return false ;
		return ( lhs->get_trans_rich()  < rhs->get_trans_rich() ) ;
	}
} ;


typedef std::set < record_pointer, translation_record_compare > trans_set ;
typedef std::set < record_pointer, translation_record_compare >:: iterator record_iterator ;

#define ALL_MEMORIES 1

/** Gives each memory a unique id (even if they have the same location).
 */
inline int get_unique_memory_id()
{
	static int current_id = ALL_MEMORIES ;

	current_id++ ;

	return current_id ;
}


/** The Felix memory class.
	@class CTranslationMemory
	This is the central engine of the entire application
	Everything is pretty much just a UI/COM interface running on top
*/
class CTranslationMemory
{
	typedef std::map< wstring, wstring >	extra_strings_type ;

	int							m_id ;
public:
	bool					m_is_demo ;
	extra_strings_type		m_extra_strings ;
	CProgressListener		*m_listener ;
	
	CString					m_file_location ;

	app_props::properties_memory		m_properties ;
	app_props::properties_glossary		m_gloss_properties ;
	app_props::properties_algorithm		m_algo_properties ;

	match_maker			m_match_maker ;
	bool				m_is_saved;

	memory_header		m_header ;

	bool				m_is_active ;

	CmpMaker			m_cmp_maker ;
	trans_set				m_records ;

public:
#ifdef UNIT_TEST
	bool m_throw_exception ;
#endif
	// construction / destruction
	CTranslationMemory( double min_score = 0.5 )  ;
	virtual ~CTranslationMemory();

	trans_set& get_records() {return m_records ; }

	void set_gloss_props(app_props::properties_glossary &props);
	int	get_id() 
	{ 
		return m_id ;
	}

	void set_active_on() { m_is_active = true ; }
	void set_active_off() { m_is_active = false ; }
	bool is_active() { return m_is_active ; }

	virtual bool is_local() = 0 ;
	void set_creator_to_current_user();
	void do_demo_check( int *cookie );
	long get_number_of_glossary_matches( const wstring &query );
	void load_header( const CString &location );

	void load_header_raw_text( char * raw_text, int file_len );
	void set_properties_memory( const app_props::properties_memory &props ) ;
	void set_properties_glossary( const app_props::properties_glossary &props ) ;
	void set_properties_algo( const app_props::properties_algorithm &props ) ;
	void refresh_properties() ;

	bool remove_extra_string( const wstring &key ) ;
	void set_extra_string( const wstring &key, const wstring &value ) ;
	const wstring get_extra_string( const wstring &key ) ;

	memory_header &get_header() { return m_header ; }
	void set_header( const memory_header &header );
	void set_is_memory( const bool setting );
	bool get_is_memory();

	void batch_set_reliability( size_t rel );
	void batch_set_validation( bool val );
	wstring get_validated_percent();
	void get_reliability_stats( size_t &low, size_t &high, double &ave );

	void set_locked_off();
	void set_locked_on( );
	bool is_locked()
	{
		return m_header.is_locked() ;
	}

	void set_listener( CProgressListener *listener )
	{
		m_listener = listener ;
	}
	size_t get_num_records( LPCSTR file_text );

	bool is_demo();
	void refresh_status();
	typedef trans_set::iterator	iterator_type ;

	void set_minimum_score(const size_t score);

	void tabulate_fonts( font_tabulator &tabulator );
	record_pointer get_record( size_t index );
	iterator_type begin( ) { return m_records.begin() ; }
	iterator_type end( ) { return m_records.end() ; }
	bool set_saved_flag( bool flag );
	bool is_saved();
	
	void search_no_regex(const search_query_params & params, search_match_multiset &matches);
	bool find_trans_matches ( TransMatchContainer &matches, const search_query_params &params ) ;

	void set_cmp_params( const search_query_params &params ) ;
	bool find_matches ( TransMatchContainer &matches, const search_query_params &params ) ;

	bool record_exists( record_pointer rec );
	bool clear_memory();
	// get/set functions
	bool set_location( const CString &location ) ;
	// export/import
	bool load( const CString &file_name ) ;

	bool load_text( char * raw_text, const CString& file_name, unsigned int file_len );
	void handleStdExceptionOnLoad(  bool was_saved, const CString& file_name, std::exception &e );

	bool UserSaysBail();
	bool ListenerSaysBail();

	void continue_or_throw(CException &e);

	void refresh_user_name(void);
	void get_date_created( const CString &location );

	UINT get_correct_encoding(char *text, UINT text_size ) ;


	UINT encoding_from_encoding_string(textstream_reader< char > & xml_reader);
	UINT encoding_from_multi_lang(char* text, UINT text_size);

	void handleCExceptionOnLoad(const ATL::CString& file_name, bool was_saved, CException& e);

	int setProgressInterval(int num_records);

	void loadWideBuffer(char* raw_text, int file_len, CStringW& wide_buffer);

	void postLoadCleanup(const ATL::CString& file_name, bool was_saved, size_t original_num_records);

	void loadRecords(const ATL::CString& file_name, textstream_reader< wchar_t >& reader, int progress_interval, bool was_saved);
	void loadRecords( const ATL::CString& file_name, c_reader& reader, int progress_interval, bool was_saved );

	void check_progress_update( int progress_interval );
	// glossary stuff
	void set_gloss_fuzzy_match(search_match_multiset& matches, 
							const search_query_params& params, 
							search_match_ptr& match_test);

	void get_gloss_fuzzy(search_match_multiset& matches, 
						 const search_query_params& params);
	void get_gloss_100(search_match_multiset& matches, 
					   const search_query_params& params);

	void set_gloss_100_char(search_match_multiset& matches, 
							gloss_match_tester& tester, 
							record_pointer& rec);

	std::wstring prep_pattern_string_gloss_100(const search_query_params& params, record_pointer& rec);

// pure virtual methods
	virtual bool add_record(record_pointer record) = 0;
	virtual record_pointer add_by_id(size_t recid, const wstring source, const wstring trans) = 0 ;
	virtual size_t size() = 0;
	virtual bool empty() = 0;
	virtual CString get_location( ) =0;
	virtual CString get_fullpath() = 0 ;
	virtual bool is_new() = 0;
	virtual bool save_memory( ) = 0;

	virtual bool erase( record_pointer record ) = 0;

	virtual void get_match_candidates(trans_set &candidates, 
										const wstring &query_cmp,
										double min_score) = 0 ;
	virtual void get_rmatch_candidates(trans_set &candidates, 
										const wstring &query_cmp,
										double min_score) = 0 ;
	virtual double get_best_match_score( const wstring &query ) = 0;
	virtual bool get_glossary_matches( search_match_multiset &matches, 
		const search_query_params &params ) = 0;
	virtual bool perform_search( search_match_multiset &matches, 
						const search_query_params &params ) = 0;

	virtual void replace(record_pointer old_rec, record_pointer new_rec) = 0 ;

} ;

typedef boost::shared_ptr< CTranslationMemory > memory_pointer ;
typedef std::list< memory_pointer > memory_list ;
typedef std::list< memory_pointer >::iterator memory_iterator ;

class memory_local : public CTranslationMemory
{
	size_t m_next_id ;
	std::set<size_t> m_ids ;
public:
	memory_local(double min_score=0.5f) : 
	  CTranslationMemory(min_score),
	  m_next_id(1)
	  {}

	bool add_record(record_pointer record) ;

	size_t get_next_id();
	size_t size();
	bool empty();
	bool erase( record_pointer record ) ;
	CString get_location( );
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

	void get_match_candidates(trans_set &candidates, 
								const wstring &query_cmp,
								double min_score);
	void get_rmatch_candidates(trans_set &candidates, 
								const wstring &query_cmp,
								double min_score);
	double get_best_match_score(const wstring &query);
	bool get_glossary_matches( search_match_multiset &matches, 
								const search_query_params &params ) ;
	bool perform_search( search_match_multiset &matches, 
								const search_query_params &params ) ;
	record_pointer add_by_id(size_t recid, const wstring source, const wstring trans);
	void replace(record_pointer old_rec, record_pointer new_rec);

};


}