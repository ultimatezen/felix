/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

using std::wstring ;

namespace mem_engine
{

namespace placement
{
	typedef std::pair< std::wstring, std::wstring > trans_pair ;
	wchar_t narrow_num( wchar_t c ) ;

	void fix_match_spans( wstring &segment );
	void fix_html_entities( wstring &segment );
	bool is_num_rep(std::wstring& PotentialNum);
	bool is_substitution(trans_pair& trans, std::wstring& SourceNum, size_t TransPos, std::wstring& QueryNum);
	int is_num_or_null( wchar_t c );


enum MatchType { MATCH, NOMATCH, PLACEMENT } ;
enum CharType { SOURCE, QUERY } ;

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

	   MatchType match_type()
	   {
		   return m_MatchType ;
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
	typedef std::vector< pairing_entity > pairings_t ;
	typedef pairing_entity pairing_t ;

/** Adds the buffer we have stored to the marked up string.
*/	
wstring add_buffer_to_markup(MatchType MatchState, const wstring buffer);

/** Marks up a string.
* 
* Keep accumulating characters in a buffer until the match type
* changes, and then dump it into our markup string.
*/
wstring mark_up( pairings_t &pairs, CharType ct );

/** Are we looking at a pair of number characters?
*/
int is_num_pair(pairing_entity& pe);
	
/** Calculates the score based on our pairings.
*/
double calc_score(pairings_t &pairs);
double calc_score_gloss(pairings_t &pairs);

/*!
 * Represents a pairing of match strings.
 */
class match_string_pairing
{
public:

	pair_list m_pairs ;
	pairings_t m_pairvec ;

	std::set< size_t > m_placement_positions ;

	match_string_pairing(void);

	pairings_t &get();
	void clear();

	void source_to_epsilon( wchar_t s );
	void query_to_epsilon( wchar_t q );
	void match( wchar_t s, wchar_t q );
	void no_match( wchar_t s, wchar_t q );
	wstring mark_up_source();
	wstring mark_up_query();

};

}
}