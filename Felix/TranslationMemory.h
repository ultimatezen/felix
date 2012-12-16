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
#include "memory_info.h"
#include "ProgressListener.h"

#include "match_maker.h"

#include "UserName.h"
#include "match_tester.h"
#include "segment.h"
#include "input_device.h"
#include <boost/range/adaptor/map.hpp>

bool is_vista_or_later() ;
const CString get_load_failure_msg( const CString & file_name ) ;

namespace mem_engine
{
	namespace ad = boost::adaptors ;

	const static int CP_UNICODE = 1200 ;
	const static int CP_UNICODE_BE = 1201 ;


	typedef std::pair<wstring, wstring> key_type ;
	typedef std::map<key_type, record_pointer> map_type ;

	typedef map_type record_collection_type ;

	typedef boost::tuple<size_t, size_t, double> rel_tuple ;
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
typedef std::set < record_pointer, translation_record_compare >::iterator record_iterator ;

static const size_t ALL_MEMORIES_ID = 1 ;
static const size_t RULE_MEMORY_ID = 2 ;

/** Gives each memory a unique id (even if they have the same location).
 */
inline size_t get_unique_memory_id()
{
	static size_t current_id = RULE_MEMORY_ID + 1 ;

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
protected:
VISIBLE_TO_TESTS
	typedef std::map< wstring, wstring >	extra_strings_type ;

	size_t							m_id ;

	bool					m_is_demo ;
	extra_strings_type		m_extra_strings ;
	CProgressListener		*m_listener ;

	app_props::props_ptr				m_props ;
	app_props::properties_memory		*m_mem_properties ;
	app_props::properties_glossary		*m_gloss_properties ;
	app_props::properties_algorithm		*m_algo_properties ;

	match_maker			m_match_maker ;
	bool				m_is_saved;

	bool				m_is_active ;

	CmpMaker			m_cmp_maker ;

	wstring m_default_context ;

public:

	DECLARE_SENSING_VAR ;

#ifdef UNIT_TEST
	bool m_throw_exception ;
#endif
	// construction / destruction
	CTranslationMemory(app_props::props_ptr props,
						double min_score = 0.5)  ;
	virtual ~CTranslationMemory();

	void set_default_context(wstring context)
	{
		m_default_context = context ;
	}
	wstring get_default_context()
	{
		return m_default_context ;
	}

	size_t get_id() 
	{ 
		return m_id ;
	}

	app_props::props_ptr get_properties()
	{
		return m_props ;
	}
	app_props::properties_memory *get_mem_properties()
	{
		return m_mem_properties ;
	}
	app_props::properties_glossary *get_gloss_properties()
	{
		return m_gloss_properties ;
	}
	app_props::properties_algorithm *get_algo_properties()
	{
		return m_algo_properties ;
	}

	void set_active_on() { m_is_active = true ; }
	void set_active_off() { m_is_active = false ; }
	bool is_active() const { return m_is_active ; }

	void set_creator_to_current_user();
	void do_demo_check(void);
	long get_number_of_glossary_matches( const wstring query );

	void set_properties_memory( app_props::properties_memory *props ) ;
	void set_properties_glossary( app_props::properties_glossary *props ) ;
	void set_properties_algo( app_props::properties_algorithm *props ) ;

	bool remove_extra_string(const wstring key) ;
	void set_extra_string( const wstring key, const wstring value ) ;
	const wstring get_extra_string( const wstring key ) ;

	void set_is_memory( const bool setting );
	bool get_is_memory() const ;

	void set_listener( CProgressListener *listener )
	{
		m_listener = listener ;
	}
	const size_t get_num_records( LPCSTR file_text );

	bool is_demo() const;
	void refresh_status();

	void set_minimum_score(const size_t score);

	bool set_saved_flag( bool flag );
	bool is_saved() const;
	
	bool find_trans_matches ( trans_match_container &matches, const search_query_params &params ) ;

	bool find_matches ( trans_match_container &matches, const search_query_params &params ) ;

	// get/set functions
	// export/import

	void handle_stdexception_on_load(  bool was_saved, const CString& file_name, std::exception &e );

	bool UserSaysBail();
	bool ListenerSaysBail();

	void continue_or_throw(except::CException &e);

	void refresh_user_name(void);
	void detect_date_created( const CString &location, input_device_ptr input );

	UINT get_correct_encoding(const char *text, UINT text_size ) ;


	UINT encoding_from_encoding_string(textstream_reader< char > & xml_reader);
	UINT encoding_from_multi_lang(const char* text, UINT text_size);

	void handle_cexception_on_load(const ATL::CString& file_name, bool was_saved, except::CException& e);

	void check_progress_update( int progress_interval );

	search_match_ptr make_match(record_pointer record = record_pointer());
	// glossary stuff
	void set_gloss_fuzzy_match(search_match_container& matches, 
							const search_query_params& params, 
							search_match_ptr& match_test);

	std::wstring prep_pattern_string_gloss_100(const search_query_params& params, record_pointer& rec);

	/************************************************************************/
	/* pure virtual methods                                                 */
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	// statistical info
	//////////////////////////////////////////////////////////////////////////
	virtual void batch_set_reliability( size_t rel ) = 0;
	virtual void batch_set_validation( bool val ) = 0 ;
	virtual wstring get_validated_percent(void) =  0 ;
	virtual rel_tuple get_reliability_stats(void) = 0 ;

	//////////////////////////////////////////////////////////////////////////
	// memory info
	//////////////////////////////////////////////////////////////////////////
	virtual MemoryInfo* get_memory_info(void) = 0 ;
	virtual const MemoryInfo* get_memory_info_const(void) const = 0 ;
	virtual void set_locked_off(void) = 0 ;
	virtual void set_locked_on(void) = 0 ;
	virtual bool is_locked(void) = 0 ;
	virtual bool is_new(void) = 0;
	virtual size_t size(void) = 0; // Can't make it const because the remote memory makes a COM call.
	virtual bool empty(void) = 0;

	//////////////////////////////////////////////////////////////////////////
	// load/save
	//////////////////////////////////////////////////////////////////////////
	virtual bool load( const CString &file_name ) = 0 ;
	virtual bool save_memory(void) = 0;

	virtual CString get_location(void) = 0 ;
	virtual void set_location( CString location ) = 0 ;
	virtual void set_mem_location(CString location) = 0 ;
	virtual CString get_fullpath(void) = 0 ;

	//////////////////////////////////////////////////////////////////////////
	// memory methods
	//////////////////////////////////////////////////////////////////////////
	virtual bool is_local(void) = 0 ;
	virtual record_collection_type& get_records(void) = 0 ;

	virtual void tabulate_fonts( font_tabulator &tabulator ) = 0;
	virtual record_pointer get_record_at( const size_t index ) = 0;
	virtual void set_cmp_params( const search_query_params &params ) = 0 ;
	virtual bool record_exists( record_pointer rec ) = 0 ;
	virtual  bool clear_memory(void) = 0;

	virtual bool add_record(record_pointer record) = 0;
	virtual record_pointer add_by_id(size_t recid, const wstring source, const wstring trans) = 0 ;

	virtual bool erase( const record_pointer record ) = 0;

	virtual void get_match_candidates(trans_set &candidates, 
										const wstring query,
										double min_score) = 0 ;
	virtual void get_rmatch_candidates(trans_set &candidates, 
										const wstring query,
										double min_score) = 0 ;
	virtual double get_best_match_score( const wstring query ) = 0;
	virtual bool get_glossary_matches( search_match_container &matches, 
		const search_query_params &params ) = 0;
	virtual bool perform_search( search_match_container &matches, 
						const search_query_params &params ) = 0;

	// Get records with exact match of query to plain source
	virtual size_t get_perfect_matches(search_match_container &matches, const wstring &query) = 0 ;

	virtual void replace(const record_pointer old_rec, record_pointer new_rec) = 0 ;

} ;

typedef boost::shared_ptr< CTranslationMemory > memory_pointer ;
typedef std::list< memory_pointer > memory_list ;
typedef std::list< memory_pointer >::iterator memory_iterator ;



}