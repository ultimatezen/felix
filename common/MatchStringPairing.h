/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

using std::wstring ;

wchar_t narrow_num( wchar_t c ) ;

enum MatchType { MATCH, NOMATCH, PLACEMENT } ;
enum CharType { SOURCE, QUERY } ;

/*!
 * Represents a pairing of match strings.
 */
class match_string_pairing
{
public:

	/*!
		Internal structure of character pairs.

		Conceptually, it takes the following form:
			SOURCE_CHAR	MATCH_TYPE	QUERY_CHAR

			Where:
			SOURCE_CHAR = source string character (0 for epsilon)
			MATCH_TYPE = indicator of how chars are matched (match, no match, placement)
			QUERY_CHAR = query string character (0 for epsilon)
	 */
	struct pairing_entity
	{
		wchar_t m_Chars[2] ;
		MatchType m_MatchType ;

		pairing_entity(wchar_t s, MatchType mt, wchar_t q ):
			 m_MatchType(mt)
		{
			m_Chars[SOURCE] = s ;
			m_Chars[QUERY] = q ;
		}

	   wchar_t& source()
	   {
		   return get_char(SOURCE) ; 
	   }
	   wchar_t& query()
	   {
		   return get_char(QUERY) ; 
	   }
	   wchar_t& get_char(const CharType &index)
	   {
		   return m_Chars[index] ;
	   }
	   // keep const-correctness
	   const wchar_t get_char(const CharType &index) const
	   {
		   return m_Chars[index] ;
	   }
	} ;

	typedef std::list< pairing_entity > pair_list ;

	pair_list m_Pairs ;

	wstring MarkedUpString ;
	wstring TextBuffer ;

	std::set< size_t > m_PlacementPositions ;
	std::set< size_t > m_PlacementPositionsTmp ;

	match_string_pairing(void);
	~match_string_pairing(void);
	void clear();

	void SourceToEpsilon( wchar_t s );
	void QueryToEpsilon( wchar_t q );
	void Match( wchar_t s, wchar_t q );
	void NoMatch( wchar_t s, wchar_t q );

	wstring MarkupSource();
	wstring MarkupQuery();

	bool PlaceNumbers( std::pair< wstring, wstring >& trans ) ;
	double CalcScore();

	bool IsNumRep(std::wstring& PotentialNum);

	bool IsSubstitution(std::pair< std::wstring, std::wstring >& trans, std::wstring& SourceNum, size_t TransPos, std::wstring& QueryNum);
private:
	wstring MarkupString( CharType ct );
	void AddBufferToMarkup(MatchType MatchState) ;
	int IsNumPair(match_string_pairing::pairing_entity& pe);
	std::wstring GetNum(std::vector< match_string_pairing::pairing_entity >& PairVec, size_t& CharPos, CharType ct);
	int IsNumOrNull( wchar_t c );
	void ReAlignPairs(std::vector< match_string_pairing::pairing_entity >& PairVec);

};
