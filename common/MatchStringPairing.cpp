/**
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include "StdAfx.h"
#include "MatchStringPairing.h"
#include "stringex.h"

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
	foreach(wchar_t c, PotentialNum)
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
	return ( iswdigit( narrow_num( c ) ) || c == 0 ||  c == L'.' || c == L'-' || c == L'D' || c == L',') ;
}

int is_num_pair(pairing_entity& pe)
{
	return iswdigit( narrow_num( pe.source() ) ) 
		&& iswdigit( narrow_num( pe.query() ) );
}


std::wstring mark_up( pair_list &pairs, CharType ct )
{
	ATLASSERT( ! pairs.empty() ) ;

	MatchType MatchState = pairs.begin()->m_MatchType ;

	std::vector<wstring> markup ;
	wstring buffer ;

	foreach ( pairing_entity entity, pairs )
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


double calc_score(pair_list &pairs)
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

double calc_score_gloss(pair_list &pairs)
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
	return mark_up( m_pairs, SOURCE ) ;
}

/** Returns the marked up query string.
 */
std::wstring match_string_pairing::mark_up_query()
{
	return mark_up( m_pairs, QUERY ) ;
}

/** Places numbers found in source and trans.
 */
bool match_string_pairing::place_numbers( trans_pair& trans )
{
	const static wstring placement_fmt( L"<span class=\"placement\">%s</span>" ) ;

	m_placement_positions.clear() ;
	std::set< size_t > positions_tmp ;

	std::vector< pairing_entity > PairVec ;
	PairVec.assign(m_pairs.begin(), m_pairs.end()) ;
	ATLASSERT( m_pairs.size() == PairVec.size() ) ;

	bool PairedNums = false ;

	for( size_t i = 0 ; i < PairVec.size() ;)
	{
		pairing_entity pe = PairVec[i] ;
		if( is_num_pair(pe)
			&& pe.m_MatchType == NOMATCH )
		{
			size_t SourcePos = i ;
			std::wstring SourceNum = get_num(PairVec, SourcePos, SOURCE, positions_tmp);

			size_t QueryPos = i ;
			std::wstring QueryNum = get_num(PairVec, QueryPos, QUERY, positions_tmp);

			// is the num in the translation? (but only once...)
			size_t TransPos = trans.first.find( SourceNum ) ;
			if (TransPos == wstring::npos && has_asian(SourceNum))
			{
				wstring newsource ;
				foreach(wchar_t c, SourceNum)
				{
					newsource += narrow_num(c) ;
				}
				SourceNum = newsource ;
				TransPos = trans.first.find( SourceNum ) ;
			}

			if( is_substitution(trans, SourceNum, TransPos, QueryNum) ) 
			{
				PairedNums = true ;
				if (! has_asian(SourceNum) && has_asian(QueryNum))
				{
					wstring newsource ;
					foreach(wchar_t c, QueryNum)
					{
						newsource += narrow_num(c) ;
					}
					QueryNum = newsource ;
				}
				
				boost::replace_first( trans.first, SourceNum, QueryNum ) ;
				boost::replace_first( trans.second, SourceNum, ( boost::wformat( placement_fmt ) % QueryNum ).str() ) ;

				std::copy(
					positions_tmp.begin(), positions_tmp.end(),
					std::inserter( m_placement_positions, m_placement_positions.begin() ) );
			}


			// how much to skip
			i = max( SourcePos, QueryPos ) + 1 ;
		}
		else // we didn't find a number, so examine the next pairing
		{
			++i ;
		}
	}

	if( PairedNums )
	{
		re_align_pairs(PairVec);
		fix_html_entities(trans.first);
		fix_html_entities(trans.second);
		fix_match_spans(trans.second);
	}

	return PairedNums ;
}



/** Re-aligns the pairs.
 */
void match_string_pairing::re_align_pairs(std::vector< pairing_entity >& PairVec)
{
	m_pairs.clear() ;

	for( size_t i = 0 ; i < PairVec.size() ; ++i )
	{
		pairing_entity pe = PairVec[i] ;

		if ( m_placement_positions.find( i ) == m_placement_positions.end() )
		{
			m_pairs.push_back(pe) ;
		}
		else
		{
			if( pe.query() != 0 )
			{
				pe.source() = pe.query() ;
				pe.m_MatchType = PLACEMENT ;
				m_pairs.push_back(pe) ;
			}
		}

	}
}

/** Returns a number for placement.
 */
std::wstring match_string_pairing::get_num(std::vector< pairing_entity >& PairVec, size_t& CharPos, CharType ct, std::set< size_t > &positions )
{
	while( is_num_or_null(PairVec[CharPos].get_char(ct)) && CharPos > 0 )
	{
		CharPos-- ;
	}

	if ( ! is_num_or_null(PairVec[CharPos].get_char(ct)) )
	{
		CharPos++ ;
	}
	ATLASSERT( is_num_or_null(PairVec[CharPos].get_char(ct)) ) ;

	wstring Num ;
	while( CharPos < PairVec.size() && is_num_or_null(PairVec[CharPos].get_char(ct)) )
	{
		if( PairVec[CharPos].get_char(ct) != 0 )
		{
			positions.insert( CharPos ) ;
			Num += PairVec[CharPos].get_char(ct) ;
		}
		CharPos++ ;
	}

	return Num;
}



void match_string_pairing::clear()
{
	m_placement_positions.clear() ;
	m_pairs.clear() ;
}

}
}