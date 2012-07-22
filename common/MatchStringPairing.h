/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

using std::wstring ;

wchar_t narrow_num( wchar_t c ) ;

/*!
 * Represents a pairing of match strings.
 */
class match_string_pairing
{
public:
	enum MatchType { MATCH, NOMATCH, PLACEMENT } ;
	enum CharType { SOURCE, QUERY } ;

	/*!
	 * Internal structure of character pairs
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
		   return m_Chars[SOURCE] ; 
	   }
	   wchar_t& query()
	   {
		   return m_Chars[QUERY] ; 
	   }

	} ;

	typedef std::list< pairing_entity > pair_list ;

	pair_list m_Pairs ;

	wstring MarkedUpString ;
	wstring TextBuffer ;

	/*!
	 * \brief
	 * Write brief comment for m_PlacementPositions here.
	 */
	std::set< size_t > m_PlacementPositions ;
	/*!
	 * \brief
	 * Write brief comment for m_PlacementPositionsTmp here.
	 */
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
