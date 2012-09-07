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
wchar_t narrow_num( wchar_t c ) ;

void fix_match_spans( wstring &segment );
void fix_html_entities( wstring &segment );

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

/*!
 * Represents a pairing of match strings.
 */
class match_string_pairing
{
public:

	pair_list m_pairs ;

	wstring m_marked_up_string ;
	wstring m_text_buffer ;

	std::set< size_t > m_placement_positions ;

	match_string_pairing(void);
	~match_string_pairing(void);
	void clear();

	void source_to_epsilon( wchar_t s );
	void query_to_epsilon( wchar_t q );
	void match( wchar_t s, wchar_t q );
	void no_match( wchar_t s, wchar_t q );
	wstring mark_up_source();
	wstring mark_up_query();
	wstring mark_up( CharType ct );
	bool place_numbers( std::pair< wstring, wstring >& trans ) ;

	double calc_score();
	bool is_num_rep(std::wstring& PotentialNum);
	bool is_substitution(std::pair< std::wstring, std::wstring >& trans, std::wstring& SourceNum, size_t TransPos, std::wstring& QueryNum);

	void add_buffer_to_markup(MatchType MatchState) ;
	int is_num_pair(pairing_entity& pe);
	std::wstring get_num(std::vector< pairing_entity >& PairVec, size_t& CharPos, CharType ct, std::set< size_t > &positions );
	int is_num_or_null( wchar_t c );
	void re_align_pairs(std::vector< pairing_entity >& PairVec);

};

}
}