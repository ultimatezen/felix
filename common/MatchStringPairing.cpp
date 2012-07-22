/**
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include "StdAfx.h"
#include "MatchStringPairing.h"
#include "EASYSTL/algorithmx.h"
#include "stringex.h"

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


/** Base contructor.
 */
match_string_pairing::match_string_pairing(void)
{
}

/** Destructor.
 */
match_string_pairing::~match_string_pairing(void)
{
}

/** Match source character against epsilon.
 */
void match_string_pairing::SourceToEpsilon( wchar_t s )
{
	m_Pairs.push_front( pairing_entity( s, NOMATCH, 0 ) ) ;
}

/** Match query character against epsilon.
 */
void match_string_pairing::QueryToEpsilon( wchar_t q )
{
	m_Pairs.push_front( pairing_entity( 0, NOMATCH, q ) ) ;
}

/** Match source and query characters.
 */
void match_string_pairing::Match( wchar_t s, wchar_t q )
{
	m_Pairs.push_front( pairing_entity( s, MATCH, q ) ) ;
}

/** Pair query and source characters, but they don't match.
 */
void match_string_pairing::NoMatch( wchar_t s, wchar_t q )
{
	m_Pairs.push_front( pairing_entity( s, NOMATCH, q ) ) ;
}

/** Returns the marked up source string.
 */
std::wstring match_string_pairing::MarkupSource()
{
	return MarkupString( SOURCE ) ;
}

/** Returns the marked up query string.
 */
std::wstring match_string_pairing::MarkupQuery()
{
	return MarkupString( QUERY ) ;
}

/** Marks up a string.
 */
std::wstring match_string_pairing::MarkupString( CharType ct )
{
	MatchType MatchState = m_Pairs.begin()->m_MatchType ;

	MarkedUpString.erase() ;
	TextBuffer.erase() ;

	for ( auto pos = m_Pairs.begin() ; pos != m_Pairs.end() ; ++pos )
	{
		wchar_t c = pos->m_Chars[ct] ;

		if( c != 0 )
		{
			if ( pos->m_MatchType != MatchState )
			{
				AddBufferToMarkup( MatchState ) ;
				MatchState = pos->m_MatchType ;
			}

			if ( c == L'<' )	// we need to convert back to &lt; for display purposes
			{
				TextBuffer += L"&lt;" ;
			}
			else if ( c == L'>' )	// we need to convert back to &gt; for display purposes
			{
				TextBuffer += L"&gt;" ;
			}
			else if ( c == L'&' )	// we need to convert back to &amp; for display purposes
			{
				TextBuffer += L"&amp;" ;
			}
			else
			{
				TextBuffer += c ;
			}
		}
	}

	AddBufferToMarkup( MatchState ) ;

	return MarkedUpString ;
}

/** Adds the buffer we have stored to the marked up string.
 */
void match_string_pairing::AddBufferToMarkup(MatchType MatchState)
{
	const static wstring NoMatchFmt( L"<span class=\"nomatch\">%s</span>" ) ;
	const static wstring PlacementFmt( L"<span class=\"placement\">%s</span>" ) ;

	using boost::wformat ;

	if( TextBuffer.empty() )
	{
		return ;
	}

	if ( MatchState == NOMATCH )
	{
		MarkedUpString += ( wformat( NoMatchFmt ) % TextBuffer ).str() ;
	}
	else if ( MatchState == PLACEMENT )
	{
		MarkedUpString += ( wformat( PlacementFmt ) % TextBuffer ).str() ;
	}
	else
	{
		MarkedUpString += TextBuffer ;
	}
	TextBuffer.erase() ;
}

/** Places numbers found in source and trans.
 */
bool match_string_pairing::PlaceNumbers( std::pair< wstring, wstring >& trans )
{
	const static wstring PlacementFmt( L"<span class=\"placement\">%s</span>" ) ;

	m_PlacementPositions.clear() ;
	m_PlacementPositionsTmp.clear() ;

	std::vector< pairing_entity > PairVec ;
	PairVec.assign(m_Pairs.begin(), m_Pairs.end()) ;
	ATLASSERT( m_Pairs.size() == PairVec.size() ) ;

	bool PairedNums = false ;

	for( size_t i = 0 ; i < PairVec.size() ;)
	{
		pairing_entity pe = PairVec[i] ;
		if( IsNumPair(pe)
			&& pe.m_MatchType == NOMATCH )
		{
			size_t SourcePos = i ;
			std::wstring SourceNum = GetNum(PairVec, SourcePos, SOURCE);

			size_t QueryPos = i ;
			std::wstring QueryNum = GetNum(PairVec, QueryPos, QUERY);

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

			if( IsSubstitution(trans, SourceNum, TransPos, QueryNum) ) 
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
				boost::replace_first( trans.second, SourceNum, ( boost::wformat( PlacementFmt ) % QueryNum ).str() ) ;

				for ( std::set< size_t >::iterator pos = m_PlacementPositionsTmp.begin() ;
						pos != m_PlacementPositionsTmp.end() ;
						++pos )
				{
					m_PlacementPositions.insert( *pos ) ;
				}
			}

			m_PlacementPositionsTmp.clear() ;

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
		ReAlignPairs(PairVec);
		boost::replace_all(trans.first, L"&", L"&amp;") ;
		boost::replace_all(trans.first, L"<", L"&lt;") ;
		boost::replace_all(trans.first, L">", L"&gt;") ;

		boost::replace_all(trans.second, L"&", L"&amp;") ;
		boost::replace_all(trans.second, L"<", L"&lt;") ;
		boost::replace_all(trans.second, L">", L"&gt;") ;

		boost::replace_all(trans.second, L"&lt;span class=\"nomatch\"&gt;", L"<span class=\"nomatch\">") ;
		boost::replace_all(trans.second, L"&lt;span class=\"placement\"&gt;", L"<span class=\"placement\">") ;
		boost::replace_all(trans.second, L"&lt;/span&gt;", L"</span>") ;
	}

	return PairedNums ;
}

/** Finds whether we are looking at a substitution text range.
 */
bool match_string_pairing::IsSubstitution(std::pair< std::wstring, 
										 std::wstring >& trans, 
										 std::wstring& SourceNum, 
										 size_t TransPos, 
										 std::wstring& QueryNum)
{
	if ( ! IsNumRep(SourceNum) )
	{
		return false ;
	}
	if ( ! IsNumRep(QueryNum) )
	{
		return false ;
	}
	
	return ( TransPos != wstring::npos && trans.first.find( SourceNum, TransPos + SourceNum.size() ) == wstring::npos ) ;
}

/** Returns whether we are looking at a number representation.
 */
bool match_string_pairing::IsNumRep(std::wstring& PotentialNum)
{
	foreach(wchar_t c, PotentialNum)
	{
		if (! IsNumOrNull(c))
		{
			return false ;
		}
	}
	return true ;
}

/** Re-aligns the pairs.
 */
void match_string_pairing::ReAlignPairs(std::vector< match_string_pairing::pairing_entity >& PairVec)
{
	m_Pairs.clear() ;

	for( size_t i = 0 ; i < PairVec.size() ; ++i )
	{
		pairing_entity pe = PairVec[i] ;

		if ( m_PlacementPositions.find( i ) == m_PlacementPositions.end() )
		{
			m_Pairs.push_back(pe) ;
		}
		else
		{
			if( pe.query() != 0 )
			{
				pe.source() = pe.query() ;
				pe.m_MatchType = PLACEMENT ;
				m_Pairs.push_back(pe) ;
			}
		}

	}
}

/** Returns a number for placement.
 */
std::wstring match_string_pairing::GetNum(std::vector< match_string_pairing::pairing_entity >& PairVec, size_t& CharPos, CharType ct )
{
	while( IsNumOrNull(PairVec[CharPos].m_Chars[ct]) && CharPos > 0 )
	{
		CharPos-- ;
	}

	if ( ! IsNumOrNull(PairVec[CharPos].m_Chars[ct]) )
	{
		CharPos++ ;
	}
	ATLASSERT( IsNumOrNull(PairVec[CharPos].m_Chars[ct]) ) ;

	wstring Num ;
	while( CharPos < PairVec.size() && IsNumOrNull(PairVec[CharPos].m_Chars[ct]) )
	{
		if( PairVec[CharPos].m_Chars[ct] != 0 )
		{
			m_PlacementPositionsTmp.insert( CharPos ) ;
			Num += PairVec[CharPos].m_Chars[ct] ;
		}
		CharPos++ ;
	}

	return Num;
}

/** Returns whether the character is a number character or epsilon.
 */
int match_string_pairing::IsNumOrNull( wchar_t c )
{
	return ( iswdigit( narrow_num( c ) ) || c == 0 ||  c == L'.' || c == L'-' || c == L'D' || c == L',') ;
}

/** Are we looking at a pair of number characters?
 */
int match_string_pairing::IsNumPair(match_string_pairing::pairing_entity& pe)
{
	return iswdigit( narrow_num( pe.source() ) ) 
		&& iswdigit( narrow_num( pe.query() ) );
}

/** Calculates the score based on our pairings.
 */
double match_string_pairing::CalcScore()
{
	double SourceLen(0.0f), QueryLen(0.0f), Distance( 0.0f ) ;

	for ( auto pos = m_Pairs.begin() ; pos != m_Pairs.end() ; ++pos )
	{
		if ( pos->m_Chars[SOURCE] != 0 )
		{
			SourceLen += 1.0f ;
		}
		if ( pos->m_Chars[QUERY] != 0 )
		{
			QueryLen += 1.0f ;
		}

		if ( pos->m_MatchType == NOMATCH )
		{
			Distance += 1.0f ;
		}
		else if ( pos->m_MatchType == PLACEMENT )
		{
			if ( ! this->IsNumOrNull( pos->m_Chars[SOURCE] ) || ! this->IsNumOrNull( pos->m_Chars[QUERY] ) )
			{
				pos->m_MatchType = NOMATCH ;
				Distance += 1.0f ;
			}
		}
	}

	double HighLen = max( SourceLen, QueryLen ) ;
	return ( HighLen - Distance ) / HighLen ;
}

void match_string_pairing::clear()
{
	m_PlacementPositionsTmp.clear() ;
	m_PlacementPositions.clear() ;
	TextBuffer.clear() ;
	MarkedUpString.clear() ;
	m_Pairs.clear() ;
}

