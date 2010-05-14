#pragma once

#include "StringEx.h"			// string extensions
#include "StringConversions.h"	// convert to/from wstrings/strings/tstrings
#include "resource.h"			// string ids
#include "ArmadilloStuff.h"		// get them crackers!

#include "PropertiesDlg.h"

#include "ColorRef.h"

#include "search_match.h"
#include "TextTemplate.h"
#include "text_templates.h"

#define RES_INST _Module.GetResourceInstance()

namespace mem_engine
{
	wstring get_fname_from_loc(const wstring loc);


/** Use this for user searches, when source/trans/context etc can be specified. */
struct search_query_params
{
	wstring				m_rich_source ;
	wstring				m_rich_trans ;
	wstring				m_rich_context ;
	wstring				m_source ;
	wstring				m_trans ;
	wstring				m_context ;
	bool				m_assess_format_penalty ;
	bool				m_ignore_case ;
	bool				m_use_regex ;
	bool				m_only_validated ;
	size_t				m_min_reliability ;
	bool				m_ignore_width ;
	bool				m_ignore_hira_kata ;

	bool				m_place_numbers ;
	bool				m_place_gloss ;

	int					m_match_algo ;
	
	bool				m_show_marking ;
	// to be implemented
	bool				m_show_details ;

	search_query_params( )
			// init those bad boys!
		:	m_show_details( true ),
			m_ignore_case( false ),
			m_use_regex( false ),
			m_only_validated( false ),
			m_min_reliability( 0 ),
			m_assess_format_penalty( false ),
			m_ignore_width( false ),
			m_ignore_hira_kata( false ),
			m_match_algo( IDC_ALGO_CHAR ),
			m_show_marking(true),
			m_place_numbers(false),
			m_place_gloss(false)
	{}

	search_query_params( const search_query_params &rhs );
	search_query_params &operator = ( const search_query_params &rhs )
	{
		internal_copy( rhs ) ;
		return *this ;
	}
	void internal_copy( const search_query_params &rhs );
} ;

/** struct felix_query. */
struct felix_query
{
	// typedefs
	typedef mem_engine::search_match_ptr match_ptr ;
	typedef match_ptr&					reference ;
	typedef const match_ptr&			const_reference ;
	typedef std::vector< match_ptr >	match_list ;

	// list of match pointers
	match_list				m_match_list ;

	// member variables
	search_query_params		m_params ;
	size_t					m_pos ;

	// member functions

	// construction
	felix_query( const size_t pos=0 ) ;
	// destruction
	virtual ~felix_query() ;

	// ====================
	// get and set params
	// ====================
	bool show_marking();
	void set_marking_on();
	void set_marking_off();

	// query
	// these are just for convenience, they all are just aliases for the source routines
	const wstring get_query_plain() const ;
	const wstring get_query_rich() const ;

	void set_query_plain ( const wstring source  ) ;
	void set_query_rich ( const wstring rich_source  ) ;

	// source
	const wstring get_source_plain() const ;
	const wstring get_source_rich() const ;

	void set_source_plain ( const wstring source  ) ;
	void set_source ( const wstring rich_source  ) ;

	// trans
	const wstring get_trans_plain() const ;
	const wstring get_trans_rich() const ;

	void set_plain_trans ( const wstring trans  ) ;
	void set_trans ( const wstring rich_trans  ) ;

	// context
	const wstring get_context_plain() const ;
	const wstring get_context_rich() const ;

	void set_plain_context ( const wstring context  ) ;
	void set_context ( const wstring rich_context  ) ;

	// ====================
	// m_match_list interface 
	// ====================
	size_t current_pos() ;
	size_t prev_match_pos();
	size_t next_match_pos();
	size_t size() ;
	match_ptr &at( size_t pos );
	match_ptr &current();
	bool empty() ;
	void clear();
	void set_matches( const mem_engine::search_match_container &matches ) ;
	void set_matches( const mem_engine::trans_match_container &matches ) ;
	void set_matches( const match_list &matches ) ;
	void add_match(mem_engine::search_match_ptr match);
	void  forward( );
	void  back( );
	void erase_current();
	void erase_at( size_t pos );
	void set_current( size_t pos );

	wstring make_id_cell( int id, const wstring val );
	wstring make_id_cell( const wstring id, const wstring val );
	wstring make_table_header();
	// =======================
	// pure virtual functions
	// =======================
	virtual void set_start_numbering(size_t start) = 0 ;
	virtual wstring get_html_short() = 0 ;
	virtual wstring get_html_long() = 0 ;
	virtual wstring get_html_all() = 0 ;
	virtual void fill_match_template_params( text_tmpl::CTextTemplate &engine, 
											 match_ptr match ) = 0 ;

} ;

/** struct search_query. */
struct search_query 
	: public felix_query
{

	size_t m_start_numbering ;
	// construction
	search_query() : m_start_numbering(1) {}
	virtual ~search_query(){}

	void set_start_numbering(size_t start)
	{
		ATLASSERT(start == 0 || start == 1) ;
		m_start_numbering = start ;
	}
	wstring get_mem_name(match_ptr match) ;
	wstring get_html_long() ;
	wstring get_html_all()
	{
		return this->get_html_long() ;
	}
	void fill_match_template_params( text_tmpl::CTextTemplate &engine, match_ptr match );
} ;


/** This is for concordance searches in the main memory. */
struct search_query_mainframe : public search_query
{
	size_t m_start_numbering ;
	search_query_mainframe() : m_start_numbering(1) {}
	virtual ~search_query_mainframe() {}

	void set_start_numbering(size_t start)
	{
		ATLASSERT(start == 0 || start == 1) ;
		m_start_numbering = start ;
	}
	wstring get_html_short();
} ;


/** this is used for glossary searches and concordance searches in glossary. */
struct search_query_glossary : public search_query
{
	search_query_glossary(){}
	virtual ~search_query_glossary(){}

	wstring get_html_short() ;
} ;

/** struct translation_match_query. */
struct translation_match_query : public felix_query
{
	CColorRef m_query_color ;
	CColorRef m_source_color ;
	CColorRef m_trans_color ;
	size_t m_start_numbering ;

	// construction
	translation_match_query()  ;

	void set_start_numbering(size_t start)
	{
		ATLASSERT(start == 0 || start == 1) ;
		m_start_numbering = start ;
	}
	// =====================
	// creating html content
	// =====================

	wstring create_dummy_match( ) ;

	wstring prev_score() ;

	wstring get_score_text( search_match_ptr prev_match );
	wstring next_score() ;
	
	wstring get_html_all();

	wstring get_html_short()  ;

	wstring get_mem_name( wstring loc );
	wstring get_html_long()  ;

	bool is_perfect_match(match_ptr match);
	void fill_match_template_params( text_tmpl::CTextTemplate &engine, match_ptr match );
} ;


}