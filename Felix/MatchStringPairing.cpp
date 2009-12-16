/**
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include "StdAfx.h"
#include "MatchStringPairing.h"
#include "EASYSTL/algorithmx.h"

#include "easyunit/testharness.h"
#include "match_maker.h"

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

/** Replaces the first occurrence of from (if any) with to.
*/
bool replace_first( wstring &source, const wstring &from, const wstring &to )
{
	size_t pos = source.find( from ) ;
	if( pos == wstring::npos )
	{
		return false ;
	}

	source.replace( pos, from.size(), to ) ;
	return true ;
}


/** Base contructor.
 */
CMatchStringPairing::CMatchStringPairing(void)
{
}

/** Destructor.
 */
CMatchStringPairing::~CMatchStringPairing(void)
{
}

/** Match source character against epsilon.
 */
void CMatchStringPairing::SourceToEpsilon( wchar_t s )
{
	m_Pairs.push_front( PairingEntity( s, NOMATCH, 0 ) ) ;
}

/** Match query character against epsilon.
 */
void CMatchStringPairing::QueryToEpsilon( wchar_t q )
{
	m_Pairs.push_front( PairingEntity( 0, NOMATCH, q ) ) ;
}

/** Match source and query characters.
 */
void CMatchStringPairing::Match( wchar_t s, wchar_t q )
{
	m_Pairs.push_front( PairingEntity( s, MATCH, q ) ) ;
}

/** Pair query and source characters, but they don't match.
 */
void CMatchStringPairing::NoMatch( wchar_t s, wchar_t q )
{
	m_Pairs.push_front( PairingEntity( s, NOMATCH, q ) ) ;
}

/** Returns the marked up source string.
 */
std::wstring CMatchStringPairing::MarkupSource()
{
	return MarkupString( SOURCE ) ;
}

/** Returns the marked up query string.
 */
std::wstring CMatchStringPairing::MarkupQuery()
{
	return MarkupString( QUERY ) ;
}

/** Marks up a string.
 */
std::wstring CMatchStringPairing::MarkupString( CharType ct )
{
	MatchType MatchState = m_Pairs.begin()->m_MatchType ;

	MarkedUpString.erase() ;
	TextBuffer.erase() ;

	for ( PairListIter pos = m_Pairs.begin() ; pos != m_Pairs.end() ; ++pos )
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
			else if ( c == L'>' )	// we need to convert back to &lt; for display purposes
			{
				TextBuffer += L"&gt;" ;
			}
			else if ( c == L'&' )	// we need to convert back to &lt; for display purposes
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
void CMatchStringPairing::AddBufferToMarkup(MatchType MatchState)
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
bool CMatchStringPairing::PlaceNumbers( std::pair< wstring, wstring >& trans )
{
	const static wstring PlacementFmt( L"<span class=\"placement\">%s</span>" ) ;

	m_PlacementPositions.clear() ;
	m_PlacementPositionsTmp.clear() ;

	std::vector< PairingEntity > PairVec ;
	std::copy( m_Pairs.begin(), m_Pairs.end(), std::back_inserter( PairVec ) ) ;
	ATLASSERT( m_Pairs.size() == PairVec.size() ) ;

	bool PairedNums = false ;

	for( size_t i = 0 ; i < PairVec.size() ;)
	{
		PairingEntity pe = PairVec[i] ;
		if( IsNumPair(pe)
			&& pe.m_MatchType == NOMATCH )
		{
			size_t SourcePos = i ;
			std::wstring SourceNum = GetNum(PairVec, SourcePos, SOURCE);

			size_t QueryPos = i ;
			std::wstring QueryNum = GetNum(PairVec, QueryPos, QUERY);

			// is the num in the translation? (but only once...)
			size_t TransPos = trans.first.find( SourceNum ) ;
			if (TransPos == wstring::npos && mem_engine::has_asian(SourceNum))
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
				if (! mem_engine::has_asian(SourceNum) && mem_engine::has_asian(QueryNum))
				{
					wstring newsource ;
					foreach(wchar_t c, QueryNum)
					{
						newsource += narrow_num(c) ;
					}
					QueryNum = newsource ;
				}
				replace_first( trans.first, SourceNum, QueryNum ) ;
				replace_first( trans.second, SourceNum, ( boost::wformat( PlacementFmt ) % QueryNum ).str() ) ;

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
		str::replace_all(trans.first, L"&", L"&amp;") ;
		str::replace_all(trans.first, L"<", L"&lt;") ;
		str::replace_all(trans.first, L">", L"&gt;") ;

		str::replace_all(trans.second, L"&", L"&amp;") ;
		str::replace_all(trans.second, L"<", L"&lt;") ;
		str::replace_all(trans.second, L">", L"&gt;") ;

		str::replace_all(trans.second, L"&lt;span class=\"nomatch\"&gt;", L"<span class=\"nomatch\">") ;
		str::replace_all(trans.second, L"&lt;span class=\"placement\"&gt;", L"<span class=\"placement\">") ;
		str::replace_all(trans.second, L"&lt;/span&gt;", L"</span>") ;
	}

	return PairedNums ;
}

/** Finds whether we are looking at a substitution text range.
 */
bool CMatchStringPairing::IsSubstitution(std::pair< std::wstring, 
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
bool CMatchStringPairing::IsNumRep(std::wstring& PotentialNum)
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
void CMatchStringPairing::ReAlignPairs(std::vector< CMatchStringPairing::PairingEntity >& PairVec)
{
	m_Pairs.clear() ;

	for( size_t i = 0 ; i < PairVec.size() ; ++i )
	{
		PairingEntity pe = PairVec[i] ;

		if ( m_PlacementPositions.find( i ) == m_PlacementPositions.end() )
		{
			m_Pairs.push_back(pe) ;
		}
		else
		{
			if( pe.m_Chars[QUERY] != 0 )
			{
				pe.m_Chars[SOURCE] = pe.m_Chars[QUERY] ;
				pe.m_MatchType = PLACEMENT ;
				m_Pairs.push_back(pe) ;
			}
		}

	}
}

/** Returns a number for placement.
 */
std::wstring CMatchStringPairing::GetNum(std::vector< CMatchStringPairing::PairingEntity >& PairVec, size_t& CharPos, CharType ct )
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
int CMatchStringPairing::IsNumOrNull( wchar_t c )
{
	return ( iswdigit( narrow_num( c ) ) || c == 0 ||  c == L'.' || c == L'-' || c == L'D') ;
}

/** Are we looking at a pair of number characters?
 */
int CMatchStringPairing::IsNumPair(CMatchStringPairing::PairingEntity& pe)
{
	return iswdigit( narrow_num( pe.m_Chars[SOURCE] ) ) 
		&& iswdigit( narrow_num( pe.m_Chars[QUERY] ) );
}

/** Calculates the score based on our pairings.
 */
double CMatchStringPairing::CalcScore()
{
	double SourceLen(0.0f), QueryLen(0.0f), Distance( 0.0f ) ;

	for ( PairListIter pos = m_Pairs.begin() ; pos != m_Pairs.end() ; ++pos )
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

void CMatchStringPairing::clear()
{
	m_PlacementPositionsTmp.clear() ;
	m_PlacementPositions.clear() ;
	TextBuffer.clear() ;
	MarkedUpString.clear() ;
	m_Pairs.clear() ;
}

#ifdef _DEBUG

namespace easyunit
{
	TEST( narrow_num, NoMatches )
	{
		ASSERT_EQUALS( L'a', narrow_num( L'a' ) ) ;
		ASSERT_EQUALS( L'a', narrow_num( L'a' ) ) ;
		ASSERT_EQUALS( 0, narrow_num( L'\0' ) ) ;
		ASSERT_EQUALS( L'b', narrow_num( L'b' ) ) ;
		ASSERT_EQUALS( L'z', narrow_num( L'z' ) ) ;
		ASSERT_EQUALS( L'!', narrow_num( L'!' ) ) ;
		ASSERT_EQUALS( L'@', narrow_num( L'@' ) ) ;
		ASSERT_EQUALS( L'\\', narrow_num( L'\\' ) ) ;
	}
	TEST( narrow_num, MatchesNarrow )
	{
		ASSERT_EQUALS( L'0', narrow_num( L'0' ) ) ;
		ASSERT_EQUALS( L'2', narrow_num( L'2' ) ) ;
		ASSERT_EQUALS( L'4', narrow_num( L'4' ) ) ;
		ASSERT_EQUALS( L'6', narrow_num( L'6' ) ) ;
		ASSERT_EQUALS( L'8', narrow_num( L'8' ) ) ;
		ASSERT_EQUALS( L'9', narrow_num( L'9' ) ) ;
	}
	TEST( narrow_num, MatchesWide )
	{
		ASSERT_EQUALS( L'0', narrow_num( L'‚O' ) ) ;
		ASSERT_EQUALS( L'1', narrow_num( L'‚P' ) ) ;
		ASSERT_EQUALS( L'2', narrow_num( L'‚Q' ) ) ;
		ASSERT_EQUALS( L'3', narrow_num( L'‚R' ) ) ;
		ASSERT_EQUALS( L'4', narrow_num( L'‚S' ) ) ;
		ASSERT_EQUALS( L'5', narrow_num( L'‚T' ) ) ;
		ASSERT_EQUALS( L'6', narrow_num( L'‚U' ) ) ;
		ASSERT_EQUALS( L'7', narrow_num( L'‚V' ) ) ;
		ASSERT_EQUALS( L'8', narrow_num( L'‚W' ) ) ;
		ASSERT_EQUALS( L'9', narrow_num( L'‚X' ) ) ;
	}

	TEST( replace_first, foo_foo )
	{
		wstring src, to, from ;
		CStringA expected, actual ;

		// foo -> foo
		src = L"foo" ;
		from = L"baz" ;
		to = L"bar" ;
		replace_first( src, from, to ) ;
		expected = "foo" ;
		actual = src.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

	}

	TEST( replace_first, foo_bar )
	{
		wstring src, to, from ;
		CStringA expected, actual ;
		// foo -> bar
		src = L"foo" ;
		from = L"foo" ;
		to = L"bar" ;
		replace_first( src, from, to ) ;
		expected = "bar" ;
		actual = src.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}

	TEST( replace_first, foo_foo_2_bar_foo )
	{
		wstring src, to, from ;
		// foo foo -> bar foo
		src = L"foo foo" ;
		from = L"foo" ;
		to = L"bar" ;
		replace_first( src, from, to ) ;
		CStringA expected = "bar foo" ;
		CStringA actual = src.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}

	TEST( replace_first, modern_major_general )
	{
		wstring src, to, from ;
		CStringA expected, actual ;

		// "I am the very model of a modern major general" -> "I ate the very model of a modern major general"
		src = L"I am the very model of a modern major general" ;
		from = L"m" ;
		to = L"te" ;
		replace_first( src, from, to ) ;
		expected = "I ate the very model of a modern major general" ;
		actual = src.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

	}

}

#endif