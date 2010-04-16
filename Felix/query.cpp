
#include "StdAfx.h"
#include "query.h"
#include "logging.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

namespace mem_engine
{

	using namespace text_tmpl ;

	wstring get_fname_from_loc(const wstring loc)
	{
		return fs::wpath(loc).leaf() ;
	}

felix_query::felix_query( const size_t pos /* =0 */ ) : m_pos( pos ) 
{}

// destruction
felix_query::~felix_query() {}

// ====================
// get and set params
// ====================

// query
// these are just for convenience, they all are just aliases for the source routines

const wstring felix_query::get_query_plain() const 
{ 
	return get_source_plain() ; 
}

void felix_query::set_query_plain ( const wstring source  ) 
{ 
	set_source_plain( source ) ; 
}

const wstring felix_query::get_query_rich() const 
{
	return get_source_rich()  ; 
}

void felix_query::set_query_rich( const wstring rich_source  ) 
{ 
	set_source( rich_source ) ; 
}

// source

const wstring felix_query::get_source_plain() const 
{ 
	return m_params.m_source ; 
}

void felix_query::set_source_plain ( const wstring source  ) 
{
	m_params.m_source = source ; 
}

const wstring felix_query::get_source_rich() const 
{ 
	return m_params.m_rich_source  ; 
}
void felix_query::set_source ( const wstring rich_source  ) 
{
	m_params.m_rich_source = rich_source ; 
	set_source_plain( strip_tags( rich_source ) ) ;
}

// trans
const wstring felix_query::get_trans_plain() const 
{ 
	return m_params.m_trans ; 
}
void felix_query::set_plain_trans ( const wstring trans  ) 
{ 
	m_params.m_trans = trans ; 
}

const wstring felix_query::get_trans_rich() const 
{ 
	return m_params.m_rich_trans  ; 
}
void felix_query::set_trans ( const wstring rich_trans  ) 
{ 
	m_params.m_rich_trans = rich_trans ; 
	set_plain_trans( strip_tags( rich_trans ) ) ;
}

// context
const wstring felix_query::get_context_plain() const 
{ 
	return m_params.m_context ; 
}
void felix_query::set_plain_context ( const wstring context  ) 
{ 
	m_params.m_context = context ; 
}

const wstring felix_query::get_context_rich() const 
{ 
	return m_params.m_rich_context  ; 
}
void felix_query::set_context ( const wstring rich_context  ) 
{
	m_params.m_rich_context = rich_context ; 
	set_plain_context( strip_tags( rich_context ) ) ;
}

// ====================
// m_match_list interface 
// ====================
size_t felix_query::current_pos() 
{ 
	return m_pos ; 
}
size_t felix_query::prev_match_pos()
{
	size_t prev_pos = m_pos ;
	// get the current match
	if ( prev_pos == 0 )
	{
		prev_pos = size() ;
	}
	--prev_pos ;
	return prev_pos ;
}
size_t felix_query::next_match_pos()
{
	size_t next_pos = m_pos + 1;
	// get the current match
	if ( next_pos >= size() )
	{
		next_pos = 0 ;
	}
	return next_pos ;
}
size_t felix_query::size() 
{ 
	return m_match_list.size() ; 
}
search_match_ptr &felix_query::at( size_t pos )
{
	return m_match_list[pos] ;
}
search_match_ptr &felix_query::current()
{
	return at( m_pos ) ;
}
bool felix_query::empty() 
{
	return m_match_list.empty() ;
}
void felix_query::clear()
{
	m_match_list.clear() ;
	m_pos = 0 ;
	bool show_markup = m_params.m_show_marking ;
	m_params = search_query_params() ;
	m_params.m_show_marking = show_markup ;
}
void felix_query::set_matches( const search_match_container &matches ) 
{
	m_match_list.clear() ;
	std::copy( matches.begin(), matches.end(), std::back_inserter( m_match_list ) ) ;
	m_pos = 0 ;
}
void felix_query::set_matches( const trans_match_container &matches ) 
{
	m_match_list.clear() ;
	std::copy( matches.begin(), matches.end(), std::back_inserter( m_match_list ) ) ;
	m_pos = 0 ;
}
void felix_query::set_matches( const match_list &matches ) 
{
	m_match_list.clear() ;
	std::copy( matches.begin(), matches.end(), std::back_inserter( m_match_list ) ) ;
	m_pos = 0 ;
}
void  felix_query::forward( )
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
void  felix_query::back( )
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
void felix_query::erase_current()
{
	erase_at( m_pos ) ;
}
void felix_query::erase_at( size_t pos )
{
	if (pos >= size())
	{
		logging::log_warn("Tried to erase from out of bounds. Size: " + 
								ulong2string(size()) + 
								"; Pos: " + 
								ulong2string(pos)) ;
		return ;
	}

	match_list::iterator iter = m_match_list.begin() ;
	std::advance( iter, pos ) ;
	m_match_list.erase( iter ) ;
	if ( m_pos >= size() )
	{
		m_pos = 0 ;
	}
}
void felix_query::set_current( size_t pos )
{
	if (pos < size())
	{
		m_pos = pos ;
	}
	else
	{
		m_pos = 0 ;
	}
}

wstring felix_query::make_id_cell( int id, const wstring val )
{
	return make_id_cell( R2WSTR(id), val ) ;
}
wstring felix_query::make_id_cell( const wstring id, const wstring val )
{
	return (wformat(L"<td class=\"match_content\" id=\"%1%\">%2%</td>\n") 
		% id 
		% val).str() ;
}
wstring felix_query::make_table_header()
{
	return wstring(L"<table class=\"base\">\n") ;
}

bool felix_query::show_marking()
{
	return m_params.m_show_marking ;
}

void felix_query::set_marking_on()
{
	m_params.m_show_marking = true ;
}

void felix_query::set_marking_off()
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
	if (m_match_list.empty())
	{
		return wstring(L"(0%)") ;
	}
	else
	{
		search_match_ptr prev_match = m_match_list[prev_match_pos()] ;
		return L"(" + get_score_text(prev_match) + L")" ;

	}
}
// wstring next_score()
wstring translation_match_query::next_score()
{
	if (m_match_list.empty())
	{
		return wstring(L"(0%)") ;
	}
	else
	{
		search_match_ptr next_match = m_match_list[next_match_pos()] ;
		return L"(" + get_score_text(next_match) + L")" ;
	}
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

	text_tmpl::DictListPtr items = engine.CreateDictList();

	size_t pos = this->current_pos() ;

	for( size_t i = 0 ; i < size() ; ++i )
	{
		match_ptr match = this->at(i) ;
		record_pointer current_rec = match->get_record() ;
		text_tmpl::DictPtr item = engine.CreateDict() ;


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
		score_text << L" <span class=\"format_penalty\">[F]</span>" ;
	}
	if ( match->HasPlacement() ) 
	{
		score_text << L" <span class=\"format_penalty\">[P]</span>" ;
	}
	return score_text ;
}


/************************************************************************/
/* search_query_mainframe                                               */
/************************************************************************/


wstring search_query_mainframe::get_html_short()
{
	throw std::logic_error("Not implemented") ;
}
/************************************************************************/
/* search_query_glossary                                                */
/************************************************************************/

wstring search_query_glossary::get_html_short()
{
	CTextTemplate engine ;

	text_tmpl::DictListPtr items = engine.CreateDictList();

	for( size_t i = 0 ; i < size() ; ++i )
	{
		match_ptr match = this->at(i) ;
		record_pointer current_rec = match->get_record() ;
		text_tmpl::DictPtr item = engine.CreateDict() ;

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

	text_tmpl::DictListPtr items = engine.CreateDictList();

	size_t pos = this->current_pos() ;

	for( size_t i = 0 ; i < size() ; ++i )
	{
		match_ptr match = this->at(i) ;
		record_pointer current_rec = match->get_record() ;
		text_tmpl::DictPtr item = engine.CreateDict() ;

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

void search_query::fill_match_template_params( text_tmpl::CTextTemplate &engine, match_ptr match )
{
	engine ;
	match ;
	logging::log_warn("fill_match_template_params not implemented for gloss/concordance queries") ;
	throw except::CProgramException(_T("method `fill_match_template_params` not implemented for gloss/concordance")) ;
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