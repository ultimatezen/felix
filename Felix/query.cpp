
#include "StdAfx.h"
#include "query.h"
#include "logging.h"
#include "Path.h"
#include "cpptempl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

namespace mem_engine
{

	using namespace cpptempl;

	wstring get_fname_from_loc(const wstring loc)
	{
		file::CPath path(CString(loc.c_str())) ;
		path.StripPath() ;
		path.RemoveExtension() ;
		return wstring((LPCWSTR)path.Path()) ;
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
	m_match_list.assign(matches.begin(), matches.end()) ;
	m_pos = 0 ;
}

void felix_query::set_matches( const trans_match_container &matches ) 
{
	m_match_list.assign(matches.begin(), matches.end()) ;
	m_pos = 0 ;
}

void felix_query::set_matches( const match_list &matches ) 
{
	m_match_list.assign(matches.begin(), matches.end()) ;
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

void felix_query::add_match( mem_engine::search_match_ptr match )
{
	m_match_list.push_back(match) ;
}
//////////////////////////////////////////////////////////////////////////
// translation_match_query
//////////////////////////////////////////////////////////////////////////

translation_match_query::translation_match_query() : m_start_numbering(1)
{
}
// ===================================
// implemented pure virtual functions
// ===================================


// create_dummy_match
wstring translation_match_query::create_dummy_match( )
{
	wstring tpl_text = get_template_text("match_none.txt") ;
	cpptempl::data_map data ;


	data[L"color"] = cpptempl::make_data( L"#000") ;
	data[L"query"] = cpptempl::make_data( get_query_rich()) ;

	return cpptempl::parse(tpl_text, data) ;
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

	cpptempl::data_map data ;

	fill_match_template_params(data, current());

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
	return cpptempl::parse(tpl_text, data) ;
}

// wstring get_html_long() 
wstring translation_match_query::get_html_long() 
{ 
	if ( empty() )
	{
		ATLASSERT( size() == 0 ) ;
		return create_dummy_match() ;
	}

	cpptempl::data_map data ;

	fill_match_template_params(data, current());

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
	return cpptempl::parse(tpl_text, data) ;
}
void translation_match_query::fill_match_template_params(cpptempl::data_map &data, match_ptr match)
{
	// colors
	data[L"query_color"] = cpptempl::make_data( m_query_color.as_wstring()) ;
	data[L"source_color"] = cpptempl::make_data( m_source_color.as_wstring()) ;
	data[L"trans_color"] = cpptempl::make_data( m_trans_color.as_wstring()) ;
	data[L"index"] = cpptempl::make_data( ulong2wstring(this->current_pos())) ;
	// match text
	if ( m_params.m_show_marking ) 
	{
		markup_ptr markup = match->get_markup() ;
		data[L"query"] = cpptempl::make_data( markup->GetQuery()) ;
		data[L"source"] = cpptempl::make_data( markup->GetSource()) ;
		data[L"trans"] = cpptempl::make_data( markup->GetTrans()) ;
	}
	else
	{
		data[L"query"] = cpptempl::make_data( get_query_rich()) ;
		data[L"source"] = cpptempl::make_data( match->get_record()->get_source_rich()) ;
		data[L"trans"] = cpptempl::make_data( match->get_record()->get_trans_rich()) ;
	}

	// score
	data[L"score"] = cpptempl::make_data( get_score_text( match )) ;

	// num / total
	data[L"num"] = cpptempl::make_data( ulong2wstring( m_pos+1 )) ;
	data[L"total"] = cpptempl::make_data( ulong2wstring( size() )) ;

	// match navigation
	if ( size() > 1 ) 
	{
		wstring nav_text = get_template_text("next_nav.txt") ;
		cpptempl::data_map nav_data ;
		nav_data[L"prev_score"] = cpptempl::make_data( prev_score()) ;
		nav_data[L"next_score"] = cpptempl::make_data( next_score()) ;
		data[L"next_nav"] = cpptempl::make_data(cpptempl::parse(nav_text, nav_data)) ;
	}
	else
	{
		data[L"next_nav"] = cpptempl::make_data( L"") ;
	}

	record_pointer current_rec = match->get_record() ;

	data[L"context"] = cpptempl::make_data( current_rec->get_context_rich()) ;
	data[L"created"] = cpptempl::make_data( current_rec->get_created().get_date_time_string()) ;
	data[L"modified"] = cpptempl::make_data( current_rec->get_modified().get_date_time_string()) ;
	data[L"reliability"] = cpptempl::make_data( boost::lexical_cast<wstring>(current_rec->get_reliability())) ;
	data[L"validated"] = cpptempl::make_data( bool2wstring(current_rec->is_validated())) ;

	data[L"creator"] = cpptempl::make_data( current_rec->get_creator()) ;
	data[L"modified_by"] = cpptempl::make_data( current_rec->get_modified_by()) ;
	// other info
	data[L"mem"] = cpptempl::make_data( get_mem_name(match->get_memory_location())) ;
	wstring refcount = boost::lexical_cast<wstring>(match->get_record()->get_refcount()) ;
	data[L"refcount"] = cpptempl::make_data( refcount) ;
	data[L"ref_count"] = cpptempl::make_data( refcount) ;
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

	cpptempl::data_map data ;

	data[L"query_color"] = cpptempl::make_data( m_query_color.as_wstring()) ;
	data[L"source_color"] = cpptempl::make_data( m_source_color.as_wstring()) ;
	data[L"trans_color"] = cpptempl::make_data( m_trans_color.as_wstring()) ;

	cpptempl::data_list items ;

	size_t pos = this->current_pos() ;

	for( size_t i = 0 ; i < size() ; ++i )
	{
		match_ptr match = this->at(i) ;
		record_pointer current_rec = match->get_record() ;
		cpptempl::data_map item ;


		if (pos == i)
		{
			item[L"active"] = make_data(L"&gt;&gt; ") ;
		}
		else
		{
			item[L"active"] = make_data(L"") ;
		}

		// match text
		if ( m_params.m_show_marking ) 
		{
			markup_ptr markup = match->get_markup() ;
			item[L"query"] = make_data(markup->GetQuery()) ;
			item[L"source"] = make_data(markup->GetSource()) ;
			item[L"trans"] = make_data(markup->GetTrans()) ;
		}
		else
		{
			item[L"query"] = make_data(get_query_rich()) ;
			item[L"source"] = make_data(current_rec->get_source_rich()) ;
			item[L"trans"] = make_data(current_rec->get_trans_rich()) ;
		}
		// score
		item[L"score"] = make_data(get_score_text(match)) ;

		item[L"context"] = make_data(current_rec->get_context_rich()) ;
		item[L"created"] = make_data(current_rec->get_created().get_date_time_string()) ;
		item[L"modified"] = make_data(current_rec->get_modified().get_date_time_string()) ;
		item[L"reliability"] = make_data(boost::lexical_cast<wstring>(current_rec->get_reliability())) ;
		item[L"validated"] = make_data(bool2wstring(current_rec->is_validated())) ;

		item[L"creator"] = make_data(current_rec->get_creator()) ;
		item[L"modified_by"] = make_data(current_rec->get_modified_by()) ;
		// other info
		item[L"mem"] = make_data(get_mem_name(match->get_memory_location())) ;
		wstring refcount = boost::lexical_cast<wstring>(match->get_record()->get_refcount()) ;
		item[L"refcount"] = make_data(refcount) ;
		item[L"ref_count"] = make_data(refcount) ;

		items.push_back(cpptempl::make_data(item)) ;
	}
	data[L"records"] = cpptempl::make_data(items) ;

	// fill in the template
	wstring tpl_text = get_template_text(_T("matches_all.txt")) ;
	return cpptempl::parse(tpl_text, data) ;
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
	cpptempl::data_map data ;

	cpptempl::data_list items ;

	for( size_t i = 0 ; i < size() ; ++i )
	{
		match_ptr match = this->at(i) ;
		record_pointer current_rec = match->get_record() ;
		data_map item ;

		item[L"num0"] = cpptempl::make_data(boost::lexical_cast<wstring>(i)) ;
		item[L"num"] = cpptempl::make_data(boost::lexical_cast<wstring>(i+1)) ;

		// match text
		markup_ptr markup = match->get_markup() ;
		item[L"source"] = make_data(markup->GetSource()) ;
		item[L"trans"] = make_data(markup->GetTrans()) ;
		wstring context = markup->GetContext() ;
		item[L"context"] = make_data(context.empty() ? L"&nbsp;" : context) ;

		item[L"created"] = make_data(current_rec->get_created().get_date_time_string()) ;
		item[L"modified"] = make_data(current_rec->get_modified().get_date_time_string()) ;
		item[L"reliability"] = make_data(boost::lexical_cast<wstring>(current_rec->get_reliability())) ;
		item[L"validated"] = make_data(bool2wstring(current_rec->is_validated())) ;

		item[L"creator"] = make_data(current_rec->get_creator()) ;
		item[L"modified_by"] = make_data(current_rec->get_modified_by()) ;
		// other info
		item[L"mem"] = make_data(get_mem_name(match)) ;
		wstring refcount = boost::lexical_cast<wstring>(match->get_record()->get_refcount()) ;
		item[L"refcount"] = make_data(refcount) ;
		item[L"ref_count"] = make_data(refcount) ;

		items.push_back(cpptempl::make_data(item)) ;
	}
	data[L"records"] = cpptempl::make_data(items) ;

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

	return cpptempl::parse(tpl_text, data) ;
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
		return get_fname_from_loc(loc) ;
	}
}

wstring search_query::get_html_long()
{
	cpptempl::data_map data ;

	data_list items ;

	size_t pos = this->current_pos() ;

	for( size_t i = 0 ; i < size() ; ++i )
	{
		match_ptr match = this->at(i) ;
		record_pointer current_rec = match->get_record() ;
		data_map item ;

		if (pos == i)
		{
			item[L"active"] = make_data(L"&gt;&gt; ") ;
		}
		else
		{
			item[L"active"] = make_data(L"") ;
		}


		// match text
		markup_ptr markup = match->get_markup() ;
		item[L"source"] = make_data(markup->GetSource()) ;
		item[L"trans"] = make_data(markup->GetTrans()) ;
		wstring context = markup->GetContext() ;
		item[L"context"] = make_data(context.empty() ? L"&nbsp;" : context) ;

		item[L"created"] = make_data(current_rec->get_created().get_date_time_string()) ;
		item[L"modified"] = make_data(current_rec->get_modified().get_date_time_string()) ;
		item[L"reliability"] = make_data(boost::lexical_cast<wstring>(current_rec->get_reliability())) ;
		item[L"validated"] = make_data(bool2wstring(current_rec->is_validated())) ;

		item[L"creator"] = make_data(current_rec->get_creator()) ;
		item[L"modified_by"] = make_data(current_rec->get_modified_by()) ;
		// other info
		item[L"mem"] = make_data(get_mem_name(match)) ;
		wstring refcount = boost::lexical_cast<wstring>(match->get_record()->get_refcount()) ;
		item[L"refcount"] = make_data(refcount) ;
		item[L"ref_count"] = make_data(refcount) ;

		items.push_back(make_data(item)) ;
	}
	data[L"records"] = cpptempl::make_data(items) ;

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
	
	return cpptempl::parse(tpl_text, data) ;
}

void search_query::fill_match_template_params( cpptempl::data_map &, match_ptr )
{
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