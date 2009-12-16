
#include "StdAfx.h"
#include "query.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

namespace memory_engine
{
	wstring get_fname_from_loc(const wstring loc)
	{
		return fs::wpath(loc).leaf() ;
	}

base_presentation::base_presentation( const size_t pos /* =0 */ ) : m_pos( pos ) 
{}

// destruction
base_presentation::~base_presentation() {}

// ====================
// get and set params
// ====================

// query
// these are just for convenience, they all are just aliases for the source routines

const wstring &base_presentation::get_query_plain() const 
{ 
	return get_source_plain() ; 
}

void base_presentation::set_query_plain ( const wstring &source  ) 
{ 
	set_source_plain( source ) ; 
}

const wstring &base_presentation::get_query_rich() const 
{
	return get_source_rich()  ; 
}

void base_presentation::set_query_rich( const wstring &rich_source  ) 
{ 
	set_source( rich_source ) ; 
}

// source

const wstring &base_presentation::get_source_plain() const 
{ 
	return m_params.m_source ; 
}

void base_presentation::set_source_plain ( const wstring &source  ) 
{
	m_params.m_source = source ; 
}

const wstring &base_presentation::get_source_rich() const 
{ 
	return m_params.m_rich_source  ; 
}
void base_presentation::set_source ( const wstring &rich_source  ) 
{
	m_params.m_rich_source = rich_source ; 
	set_source_plain( strip_tags( rich_source ) ) ;
}

// trans
const wstring &base_presentation::get_trans_plain() const 
{ 
	return m_params.m_trans ; 
}
void base_presentation::set_plain_trans ( const wstring &trans  ) 
{ 
	m_params.m_trans = trans ; 
}

const wstring &base_presentation::get_trans_rich() const 
{ 
	return m_params.m_rich_trans  ; 
}
void base_presentation::set_trans ( const wstring &rich_trans  ) 
{ 
	m_params.m_rich_trans = rich_trans ; 
	set_plain_trans( strip_tags( rich_trans ) ) ;
}

// context
const wstring &base_presentation::get_context_plain() const 
{ 
	return m_params.m_context ; 
}
void base_presentation::set_plain_context ( const wstring &context  ) 
{ 
	m_params.m_context = context ; 
}

const wstring &base_presentation::get_context_rich() const 
{ 
	return m_params.m_rich_context  ; 
}
void base_presentation::set_context ( const wstring &rich_context  ) 
{
	m_params.m_rich_context = rich_context ; 
	set_plain_context( strip_tags( rich_context ) ) ;
}

// ====================
// m_match_list interface 
// ====================
size_t base_presentation::current_pos() 
{ 
	return m_pos ; 
}
size_t base_presentation::size() 
{ 
	return m_match_list.size() ; 
}
search_match_ptr &base_presentation::at( size_t pos )
{
	return m_match_list[pos] ;
}
search_match_ptr &base_presentation::current()
{
	return at( m_pos ) ;
}
bool base_presentation::empty() 
{
	return m_match_list.empty() ;
}
void base_presentation::clear()
{
	m_match_list.clear() ;
	m_pos = 0 ;
	bool show_markup = m_params.m_show_marking ;
	m_params = search_query_params() ;
	m_params.m_show_marking = show_markup ;
}
void base_presentation::set_matches( const search_match_container &matches ) 
{
	m_match_list.clear() ;
	std::copy( matches.begin(), matches.end(), std::back_inserter( m_match_list ) ) ;
	m_pos = 0 ;
}
void base_presentation::set_matches( const trans_match_container &matches ) 
{
	m_match_list.clear() ;
	std::copy( matches.begin(), matches.end(), std::back_inserter( m_match_list ) ) ;
	m_pos = 0 ;
}
void base_presentation::set_matches( const match_list &matches ) 
{
	m_match_list.clear() ;
	std::copy( matches.begin(), matches.end(), std::back_inserter( m_match_list ) ) ;
	m_pos = 0 ;
}
void  base_presentation::forward( )
{
	ATLASSERT( empty() || m_pos < size()  ) ;
	if ( empty() )
	{
		ATLASSERT( m_pos == 0 ) ;
		return ;
	}
	++m_pos ;
	if ( m_pos == size() )
		m_pos = 0 ;
}
void  base_presentation::back( )
{
	ATLASSERT( empty() || m_pos < size()  ) ;
	if ( empty() )
	{
		ATLASSERT( m_pos == 0 ) ;
		return ;
	}
	if ( m_pos == 0 )
		m_pos = size()-1 ;
	else
		--m_pos ;
}
void base_presentation::erase_current()
{
	erase_at( m_pos ) ;
}
void base_presentation::erase_at( size_t pos )
{
	ATLASSERT( pos < size() ) ;
	if ( empty() )
		return ;
	ATLASSERT( pos < size() ) ;

	match_list::iterator iter = m_match_list.begin() ;
	std::advance( iter, pos ) ;
	m_match_list.erase( iter ) ;
	if ( m_pos == size() )
		m_pos = 0 ;
}
void base_presentation::set_current( size_t pos )
{
	ATLASSERT( pos < size() ) ;
	m_pos = pos ;
}

wstring base_presentation::make_id_cell( int id, const wstring &val )
{
	return make_id_cell( R2WSTR(id), val ) ;
}
wstring base_presentation::make_id_cell( const wstring &id, const wstring &val )
{
	return (wformat(L"<td class=\"match_content\" id=\"%1%\">%2%</td>\n") 
		% id 
		% val).str() ;
}
wstring base_presentation::make_table_header()
{
	return wstring(L"<table class=\"base\">\n") ;
}

bool base_presentation::show_marking()
{
	return m_params.m_show_marking ;
}

void base_presentation::set_marking_on()
{
	m_params.m_show_marking = true ;
}

void base_presentation::set_marking_off()
{
	m_params.m_show_marking = false ;
}
//////////////////////////////////////////////////////////////////////////
// translation_match_query
//////////////////////////////////////////////////////////////////////////

translation_match_query::translation_match_query() 
{
}
// ===================================
// implemented pure virtual functions
// ===================================


// create_dummy_match
wstring translation_match_query::create_dummy_match( )
{
	wstring tpl_text = get_template_text("match_none.txt") ;
	CTextTemplate engine ;

	engine.Assign(L"color", L"#000") ;
	engine.Assign(L"query", get_query_rich()) ;

	return engine.Fetch(tpl_text) ;
}

// wstring prev_score()
wstring translation_match_query::prev_score()
{
	static wchar_t score_buf[128] ;

	size_t prev_match_pos = m_pos ;
	// get the current match
	if ( prev_match_pos == 0 )
	{
		prev_match_pos = size() ;
	}
	--prev_match_pos ;

	search_match_ptr prev_match = m_match_list[prev_match_pos] ;

	if ( m_params.m_assess_format_penalty && prev_match->get_formatting_penalty() > 0.0 ) 
	{
		StringCbPrintfW( score_buf, 128, L"(%s <font color=\"#888888\">[F]</font>)", double2percent_wstring( prev_match->get_score() ).c_str() ) ;
	}
	else
	{
		StringCbPrintfW( score_buf, 128, L"(%s)", double2percent_wstring( prev_match->get_score() ).c_str() ) ;
	}
	return wstring( score_buf ) ;

}
// wstring next_score()
wstring translation_match_query::next_score()
{
	static wchar_t score_buf[128] ;
	
	// get the current match
	size_t next_match_pos = m_pos ;

	ATLASSERT( next_match_pos != size() ) ;
	++next_match_pos ;
	if ( next_match_pos == size() )
	{
		next_match_pos = 0 ;
	}

	search_match_ptr next_match = m_match_list[next_match_pos] ;

	if ( m_params.m_assess_format_penalty && next_match->get_formatting_penalty() > 0.0 ) 
	{
		StringCbPrintfW( score_buf, 128, L"(%s <font color=\"#888888\">[F]</font>)", double2percent_wstring( next_match->get_score() ).c_str() ) ;
	}
	else
	{
		StringCbPrintfW( score_buf, 128, L"(%s)", double2percent_wstring( next_match->get_score() ).c_str() ) ;
		
	}
	return wstring( score_buf ) ;
}

// wstring get_html_short() 
wstring translation_match_query::get_html_short() 
{ 
	if ( empty() )
	{
		ATLASSERT( size() == 0 ) ;
		return create_dummy_match() ;
	}

	CTextTemplate engine ;

	fill_match_template_params(engine, current());

	// fill in the template
	wstring tpl_text ;
	if (is_perfect_match(current()))
	{
		tpl_text = get_template_text(_T("match_perfect.txt")) ;
	}
	else
	{
		tpl_text = get_template_text(_T("match_fuzzy.txt")) ;
	}
	return engine.Fetch(tpl_text) ;
}

// wstring get_html_long() 
wstring translation_match_query::get_html_long() 
{ 
	if ( empty() )
	{
		ATLASSERT( size() == 0 ) ;
		return create_dummy_match() ;
	}

	CTextTemplate engine ;

	fill_match_template_params(engine, current());

	// fill in the template
	wstring tpl_text ;
	if (is_perfect_match(current()))
	{
		tpl_text = get_template_text(_T("match_perfect_full.txt")) ;
	}
	else
	{
		tpl_text = get_template_text(_T("match_fuzzy_full.txt")) ;
	}
	return engine.Fetch(tpl_text) ;
}
void translation_match_query::fill_match_template_params(CTextTemplate &engine, match_ptr match)
{
	// colors
	engine.Assign(L"query_color", m_query_color.as_wstring()) ;
	engine.Assign(L"source_color", m_source_color.as_wstring()) ;
	engine.Assign(L"trans_color", m_trans_color.as_wstring()) ;
	engine.Assign(L"index", ulong2wstring(this->current_pos())) ;
	// match text
	if ( m_params.m_show_marking ) 
	{
		markup_ptr markup = match->get_markup() ;
		engine.Assign(L"query", markup->GetQuery()) ;
		engine.Assign(L"source", markup->GetSource()) ;
		engine.Assign(L"trans", markup->GetTrans()) ;
	}
	else
	{
		engine.Assign(L"query", get_query_rich()) ;
		engine.Assign(L"source", match->get_record()->get_source_rich()) ;
		engine.Assign(L"trans", match->get_record()->get_trans_rich()) ;
	}

	// score
	engine.Assign(L"score", get_score_text( match )) ;

	// num / total
	engine.Assign(L"num", ulong2wstring( m_pos+1 )) ;
	engine.Assign(L"total", ulong2wstring( size() )) ;

	// match navigation
	if ( size() > 1 ) 
	{
		wstring nav_text = get_template_text("next_nav.txt") ;
		CTextTemplate nav_engine ;
		nav_engine.Assign(L"prev_score", prev_score()) ;
		nav_engine.Assign(L"next_score", next_score()) ;
		engine.Assign(L"next_nav", nav_engine.Fetch(nav_text)) ;
	}
	else
	{
		engine.Assign(L"next_nav", L"") ;
	}

	record_pointer current_rec = match->get_record() ;

	engine.Assign(L"context", current_rec->get_context_rich()) ;
	engine.Assign(L"created", current_rec->get_created().get_date_time_string()) ;
	engine.Assign(L"modified", current_rec->get_modified().get_date_time_string()) ;
	engine.Assign(L"reliability", boost::lexical_cast<wstring>(current_rec->get_reliability())) ;
	engine.Assign(L"validated", bool2wstring(current_rec->is_validated())) ;

	engine.Assign(L"creator", current_rec->get_creator()) ;
	engine.Assign(L"modified_by", current_rec->get_modified_by()) ;
	// other info
	engine.Assign(L"mem", get_mem_name(match->get_memory_location())) ;
	wstring refcount = boost::lexical_cast<wstring>(match->get_record()->get_refcount()) ;
	engine.Assign(L"refcount", refcount) ;
	engine.Assign(L"ref_count", refcount) ;
}

bool translation_match_query::is_perfect_match( match_ptr match )
{
	return FLOAT_EQ(match->get_score(), 1.0) && ! match->HasPlacement();
}
wstring translation_match_query::get_html_all()
{
	if ( empty() )
	{
		ATLASSERT( size() == 0 ) ;
		return create_dummy_match() ;
	}

	CTextTemplate engine ;

	engine.Assign(L"query_color", m_query_color.as_wstring()) ;
	engine.Assign(L"source_color", m_source_color.as_wstring()) ;
	engine.Assign(L"trans_color", m_trans_color.as_wstring()) ;

	CTextTemplate::DictListPtr items = engine.CreateDictList();

	size_t pos = this->current_pos() ;

	for( size_t i = 0 ; i < size() ; ++i )
	{
		match_ptr match = this->at(i) ;
		record_pointer current_rec = match->get_record() ;
		CTextTemplate::DictPtr item = engine.CreateDict() ;


		if (pos == i)
		{
			item->insert(std::make_pair(L"active", L"&gt;&gt; ")) ;
		}
		else
		{
			item->insert(std::make_pair(L"active", L"")) ;
		}

		// match text
		if ( m_params.m_show_marking ) 
		{
			markup_ptr markup = match->get_markup() ;
			item->insert(std::make_pair( L"query", markup->GetQuery())) ;
			item->insert(std::make_pair( L"source", markup->GetSource())) ;
			item->insert(std::make_pair( L"trans", markup->GetTrans())) ;
		}
		else
		{
			item->insert(std::make_pair(L"query", get_query_rich())) ;
			item->insert(std::make_pair(L"source", current_rec->get_source_rich())) ;
			item->insert(std::make_pair(L"trans", current_rec->get_trans_rich())) ;
		}
		// score
		item->insert(std::make_pair(L"score", get_score_text(match))) ;

		item->insert(std::make_pair(L"context", current_rec->get_context_rich())) ;
		item->insert(std::make_pair(L"created", current_rec->get_created().get_date_time_string())) ;
		item->insert(std::make_pair(L"modified", current_rec->get_modified().get_date_time_string())) ;
		item->insert(std::make_pair(L"reliability", boost::lexical_cast<wstring>(current_rec->get_reliability()))) ;
		item->insert(std::make_pair(L"validated", bool2wstring(current_rec->is_validated()))) ;

		item->insert(std::make_pair(L"creator", current_rec->get_creator())) ;
		item->insert(std::make_pair(L"modified_by", current_rec->get_modified_by())) ;
		// other info
		item->insert( std::make_pair(L"mem", get_mem_name(match->get_memory_location()))) ;
		wstring refcount = boost::lexical_cast<wstring>(match->get_record()->get_refcount()) ;
		item->insert(std::make_pair(L"refcount", refcount)) ;
		item->insert(std::make_pair(L"ref_count", refcount)) ;

		items->push_back(item) ;
	}
	engine.Assign(L"records", items) ;

	// fill in the template
	wstring tpl_text = get_template_text(_T("matches_all.txt")) ;
	return engine.Fetch(tpl_text) ;
}


wstring translation_match_query::get_mem_name( wstring loc )
{
	if ( loc.empty() )
	{
		return R2WSTR( IDS_NEW ) ;
	}

	size_t backslash_pos = loc.find_last_of( L'\\' ) ;

	if ( backslash_pos != wstring::npos )
	{
		return loc.substr( backslash_pos+1 ) ;
	}

	return loc ;
}

wstring translation_match_query::get_score_text( match_ptr match )
{
	wstring score_text = double2percent_wstring( match->get_score() ) ;
	if ( m_params.m_assess_format_penalty && match->get_formatting_penalty() > 0.0 ) 
	{
		score_text << L" <font color=\"#888888\">[F]</font>" ;
	}
	if ( match->HasPlacement() ) 
	{
		score_text << L" <font color=\"#888888\">[P]</font>" ;
	}
	return score_text ;
}

/************************************************************************/
/* search_query_mainframe                                               */
/************************************************************************/


wstring search_query_mainframe::get_html_short()
{
	throw std::logic_error("Not implemented") ;
	return wstring() ;
}
/************************************************************************/
/* search_query_glossary                                                */
/************************************************************************/

wstring search_query_glossary::get_html_short()
{
	CTextTemplate engine ;

	CTextTemplate::DictListPtr items = engine.CreateDictList();

	for( size_t i = 0 ; i < size() ; ++i )
	{
		match_ptr match = this->at(i) ;
		record_pointer current_rec = match->get_record() ;
		CTextTemplate::DictPtr item = engine.CreateDict() ;

		// match text
		markup_ptr markup = match->get_markup() ;
		item->insert(std::make_pair(L"source", markup->GetSource())) ;
		item->insert(std::make_pair(L"trans", markup->GetTrans())) ;
		wstring context = markup->GetContext() ;
		item->insert(std::make_pair(L"context", context.empty() ? L"&nbsp;" : context)) ;

		item->insert(std::make_pair(L"created", current_rec->get_created().get_date_time_string())) ;
		item->insert(std::make_pair(L"modified", current_rec->get_modified().get_date_time_string())) ;
		item->insert(std::make_pair(L"reliability", boost::lexical_cast<wstring>(current_rec->get_reliability()))) ;
		item->insert(std::make_pair(L"validated", bool2wstring(current_rec->is_validated()))) ;

		item->insert(std::make_pair(L"creator", current_rec->get_creator())) ;
		item->insert(std::make_pair(L"modified_by", current_rec->get_modified_by())) ;
		// other info
		item->insert( std::make_pair(L"mem", get_mem_name(match))) ;
		wstring refcount = boost::lexical_cast<wstring>(match->get_record()->get_refcount()) ;
		item->insert(std::make_pair(L"refcount", refcount)) ;
		item->insert(std::make_pair(L"ref_count", refcount)) ;

		items->push_back(item) ;
	}
	engine.Assign(L"records", items) ;

	// fill in the template
	wstring tpl_text ;
	if (this->m_start_numbering)
	{
		tpl_text = get_template_text(_T("gloss_matches.txt")) ;
	}
	else
	{
		tpl_text = get_template_text(_T("gloss_matches0.txt")) ;
	}

	return engine.Fetch(tpl_text) ;
}



/************************************************************************/
/* search_query                                                         */
/************************************************************************/

wstring search_query::get_mem_name(match_ptr match)
{
	wstring loc = match->get_memory_location() ;

	if ( loc.empty() )
	{
		return R2WSTR( IDS_NEW ) ;
	}
	else
	{
		return get_fname_from_loc(loc);		
	}
}

wstring search_query::get_html_long()
{
	CTextTemplate engine ;

	CTextTemplate::DictListPtr items = engine.CreateDictList();

	size_t pos = this->current_pos() ;

	for( size_t i = 0 ; i < size() ; ++i )
	{
		match_ptr match = this->at(i) ;
		record_pointer current_rec = match->get_record() ;
		CTextTemplate::DictPtr item = engine.CreateDict() ;

		if (pos == i)
		{
			item->insert(std::make_pair(L"active", L"&gt;&gt; ")) ;
		}
		else
		{
			item->insert(std::make_pair(L"active", L"")) ;
		}


		// match text
		markup_ptr markup = match->get_markup() ;
		item->insert(std::make_pair(L"source", markup->GetSource())) ;
		item->insert(std::make_pair(L"trans", markup->GetTrans())) ;
		wstring context = markup->GetContext() ;
		item->insert(std::make_pair(L"context", context.empty() ? L"&nbsp;" : context)) ;

		item->insert(std::make_pair(L"created", current_rec->get_created().get_date_time_string())) ;
		item->insert(std::make_pair(L"modified", current_rec->get_modified().get_date_time_string())) ;
		item->insert(std::make_pair(L"reliability", boost::lexical_cast<wstring>(current_rec->get_reliability()))) ;
		item->insert(std::make_pair(L"validated", bool2wstring(current_rec->is_validated()))) ;

		item->insert(std::make_pair(L"creator", current_rec->get_creator())) ;
		item->insert(std::make_pair(L"modified_by", current_rec->get_modified_by())) ;
		// other info
		item->insert( std::make_pair(L"mem", get_mem_name(match))) ;
		wstring refcount = boost::lexical_cast<wstring>(match->get_record()->get_refcount()) ;
		item->insert(std::make_pair(L"refcount", refcount)) ;
		item->insert(std::make_pair(L"ref_count", refcount)) ;

		items->push_back(item) ;
	}
	engine.Assign(L"records", items) ;

	// fill in the template
	wstring tpl_text ;
	if (this->m_start_numbering)
	{
		tpl_text = get_template_text(_T("mem_concordance.txt")) ;
	}
	else
	{
		tpl_text = get_template_text(_T("mem_concordance0.txt")) ;
	}
	
	return engine.Fetch(tpl_text) ;
}


//////////////////////////////////////////////////////////////////////////
// html_formatter
//////////////////////////////////////////////////////////////////////////

wstring html_formatter::make_href_tag( const int key, const int val, UINT title_id )
{
	wstring href_tag ;
	href_tag << L"<a href=\"" 
		<< int2wstring( key ) 
		<< L":" 
		<< int2wstring( val ) 
		<< L"\"" ;

	if ( title_id != 0 ) 
	{
		wstring title_string = resource2wstring( title_id, RES_INST ) ;
		href_tag << L" title=\"" << title_string << L"\"" ;
	}

	href_tag << L">" ;
	return href_tag ;
}

wstring html_formatter::make_href( int key_id, int val_id, const wstring &link, UINT title_id /*= 0 */ )
{
	wstring href = make_href_tag( key_id, val_id, title_id ) ;
	href << link
		<< L"</a>" ;
	return href ;
}

//////////////////////////////////////////////////////////////////////////
// search_query_params
//////////////////////////////////////////////////////////////////////////

search_query_params::search_query_params( const search_query_params &rhs )
{
	internal_copy( rhs ) ;
}

void search_query_params::internal_copy( const search_query_params &rhs )
{
	m_rich_source			=	rhs.m_rich_source ;
	m_rich_trans			=	rhs.m_rich_trans ;		
	m_rich_context			=	rhs.m_rich_context ;		
	m_source				=	rhs.m_source ;
	m_trans					=	rhs.m_trans ;		
	m_context				=	rhs.m_context ;		
	m_show_details			=	rhs.m_show_details ;
	m_ignore_case			=	rhs.m_ignore_case ;		
	m_use_regex				=	rhs.m_use_regex ;		
	m_only_validated		=	rhs.m_only_validated ;		
	m_min_reliability		=	rhs.m_min_reliability ;		
	m_ignore_width			=	rhs.m_ignore_width ;
	m_ignore_hira_kata		=	rhs.m_ignore_hira_kata ;
	m_assess_format_penalty =   rhs.m_assess_format_penalty ;
	m_match_algo			=   rhs.m_match_algo ;
	m_show_marking			=	rhs.m_show_marking ;
	m_place_numbers			=	rhs.m_place_numbers ;
	m_place_gloss			=	rhs.m_place_gloss ;
}


}