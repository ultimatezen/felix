/**
 * Match up holes in strings for placement.
 */
#include "StdAfx.h"
#include "MatchStringPairing.h"

namespace mem_engine
{

namespace placement
{

/**
* Changes numbers to their narrow versions.
* Leaves other characters untouched.
*/
wchar_t narrow_num( wchar_t c )
{
	if ( str::is_fullwidth_number(c) )
	{
		return str::normalize_fullwidth_number(c);
	}
	return c ;
}

void fix_html_entities( wstring &segment )
{
	boost::replace_all(segment, L"&", L"&amp;") ;
	boost::replace_all(segment, L"<", L"&lt;") ;
	boost::replace_all(segment, L">", L"&gt;") ;
}

void fix_match_spans( wstring &segment )
{
	boost::replace_all(segment, L"&lt;span class=\"nomatch\"&gt;", L"<span class=\"nomatch\">") ;
	boost::replace_all(segment, L"&lt;span class=\"placement\"&gt;", L"<span class=\"placement\">") ;
	boost::replace_all(segment, L"&lt;/span&gt;", L"</span>") ;
}
/** Finds whether we are looking at a substitution text range.
 */
bool is_substitution(std::pair< std::wstring,
						std::wstring >& trans, 
						std::wstring& SourceNum, 
						size_t TransPos, 
						std::wstring& QueryNum)
{
	if ( ! is_num_rep(SourceNum) )
	{
		return false ;
	}
	if ( ! is_num_rep(QueryNum) )
	{
		return false ;
	}
	
	return ( TransPos != wstring::npos && trans.first.find( SourceNum, TransPos + SourceNum.size() ) == wstring::npos ) ;
}

/** Returns whether we are looking at a number representation.
 */
bool is_num_rep(std::wstring& PotentialNum)
{
	FOREACH(wchar_t c, PotentialNum)
	{
		if (! is_num_or_null(c))
		{
			return false ;
		}
	}
	return true ;
}


/** Returns whether the character is a number character or epsilon.
 */
int is_num_or_null( wchar_t c )
{
	return ( iswdigit( narrow_num( c ) ) || c == 0 ||  c == L'.' || c == L'-' || c == L'�D' || c == L',') ;
}

int is_num_pair(pairing_entity& pe)
{
	return iswdigit( narrow_num( pe.source() ) ) 
		&& iswdigit( narrow_num( pe.query() ) );
}


std::wstring mark_up( pairings_t &pairs, CharType ct )
{
	ATLASSERT( ! pairs.empty() ) ;

	MatchType MatchState = pairs.begin()->m_MatchType ;

	std::vector<wstring> markup ;
	wstring buffer ;

	FOREACH ( pairing_entity entity, pairs )
	{
		const wchar_t c = entity.get_char(ct) ;

		if( c != 0 )
		{
			if (entity.m_MatchType != MatchState )
			{
				markup.push_back(add_buffer_to_markup( MatchState, buffer )) ;
				buffer = L"" ;
				MatchState = entity.m_MatchType ;
			}

			if ( c == L'<' )	// we need to convert back to &lt; for display purposes
			{
				buffer += L"&lt;" ;
			}
			else if ( c == L'>' )	// we need to convert back to &gt; for display purposes
			{
				buffer += L"&gt;" ;
			}
			else if ( c == L'&' )	// we need to convert back to &amp; for display purposes
			{
				buffer += L"&amp;" ;
			}
			else
			{
				buffer += c ;
			}
		}
	}

	markup.push_back(add_buffer_to_markup( MatchState, buffer )) ;

	return boost::join(markup, L"") ;
}


wstring add_buffer_to_markup(MatchType MatchState, const wstring buffer) 
{
	const static wstring NoMatchFmt( L"<span class=\"nomatch\">%s</span>" ) ;
	const static wstring PlacementFmt( L"<span class=\"placement\">%s</span>" ) ;

	using boost::wformat ;

	if( buffer.empty() )
	{
		return buffer ;
	}

	if ( MatchState == NOMATCH )
	{
		return ( wformat( NoMatchFmt ) % buffer ).str() ;
	}
	else if ( MatchState == PLACEMENT )
	{
		return ( wformat( PlacementFmt ) % buffer ).str() ;
	}

	return buffer ;
}


double calc_score(pairings_t &pairs)
{
	double SourceLen(0.0f), QueryLen(0.0f), Distance( 0.0f ) ;

	for ( auto pos = pairs.begin() ; pos != pairs.end() ; ++pos )
	{
		if ( pos->source() != 0 )
		{
			SourceLen += 1.0f ;
		}
		if ( pos->query() != 0 )
		{
			QueryLen += 1.0f ;
		}

		if ( pos->m_MatchType == NOMATCH )
		{
			Distance += 1.0f ;
		}
		else if ( pos->m_MatchType == PLACEMENT )
		{
			if ( ! is_num_or_null( pos->source() ) || ! is_num_or_null( pos->query() ) )
			{
				pos->m_MatchType = NOMATCH ;
				Distance += 1.0f ;
			}
		}
	}

	double HighLen = max( SourceLen, QueryLen ) ;
	return ( HighLen - Distance ) / HighLen ;
}

double calc_score_gloss(pairings_t &pairs)
{
	double SourceLen(0.0f), QueryLen(0.0f), Distance( 0.0f ) ;

	for ( auto pos = pairs.begin() ; pos != pairs.end() ; ++pos )
	{
		if ( pos->source() != 0 )
		{
			SourceLen += 1.0f ;
		}
		if ( pos->query() != 0 )
		{
			QueryLen += 1.0f ;
		}

		if ( pos->m_MatchType == NOMATCH )
		{
			Distance += 1.0f ;
		}
	}

	double HighLen = max( SourceLen, QueryLen ) ;
	return ( HighLen - Distance ) / HighLen ;
}

/************************************************************************/
/* match_string_pairing                                                 */
/************************************************************************/

/** Base contructor.
 */
match_string_pairing::match_string_pairing(void)
{
}

/** Match source and query characters.
 */
void match_string_pairing::match( wchar_t s, wchar_t q )
{
	m_pairs.push_front( pairing_entity( s, MATCH, q ) ) ;
}

/** Pair query and source characters, but they don't match.
 */
void match_string_pairing::no_match( wchar_t s, wchar_t q )
{
	m_pairs.push_front( pairing_entity( s, NOMATCH, q ) ) ;
}
/** Match source character against epsilon.
 */
void match_string_pairing::source_to_epsilon( wchar_t s )
{
	this->no_match(s, 0) ;
}

/** Match query character against epsilon.
 */
void match_string_pairing::query_to_epsilon( wchar_t q )
{
	this->no_match(0, q) ;
}

/** Returns the marked up source string.
 */
std::wstring match_string_pairing::mark_up_source()
{
	return mark_up( get(), SOURCE ) ;
}

/** Returns the marked up query string.
 */
std::wstring match_string_pairing::mark_up_query()
{
	return mark_up( get(), QUERY ) ;
}

void match_string_pairing::clear()
{
	m_placement_positions.clear() ;
	m_pairs.clear() ;
}

pairings_t & match_string_pairing::get()
{
	if (m_pairvec.empty())
	{
		m_pairvec.assign(m_pairs.begin(), m_pairs.end()) ;
	}
	return m_pairvec ;
}
}
}