/*!
	@file match_maker.h
	@brief Memory matching engine -- the core!
	@date 2005/06/23
	Time: 0:14:17
	@author Ryan Ginstrom
	@dir C:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "Matrix.h"					// for match space
#include "query.h"					// search_match_pointer
#include "MatchStringPairing.h"
#include "distance.h"
#include "segment.h"
#include "stringex.h"

namespace mem_engine
{

	inline int detect_match_algo(const wstring s)
	{
		if (has_asian(s))
		{
			return IDC_ALGO_CHAR ;
		}
		if (has_spaces(s))
		{
			return IDC_ALGO_WORD ;
		}
		// If it's a single word (no spaces), then use the
		// char algo even if there are no Asian chars.
		return IDC_ALGO_CHAR ;
	}

	/** The Felix matching engine */
class match_maker
{
	Segment		m_row ;
	Segment		m_col ;

	wstring		m_row_string ;
	wstring		m_col_string ;

	wstring		m_wide_normalized_row_string ;
	wstring		m_wide_normalized_col_string ;

	wstring		m_col_show_string ;
	wstring		m_row_show_string ;

	size_t					m_num_rows ;
	size_t					m_num_cols ;
	double					m_minimum_score ;
	Matrix< int >			m_matrix ;
	search_match_ptr		m_match ;

	std::multiset< wstring >		m_col_tags ;
	std::multiset< wstring >		m_row_tags ;

	// get the row tokens
	std::vector< wstring >			m_word_tokens ;

	Distance		m_distance ;

public:
	bool m_assess_format_penalty ;
	bool fuzzy_gloss_score(const Segment needle,
		const Segment haystack,
		search_match_ptr &match);

	size_t calc_word_distance( const wstring row_word, const wstring col_word, Matrix< size_t > &matrix );
	bool tokenize_words( const wstring words, std::vector< wstring > &tokens );
	bool get_trans_score_word( search_match_ptr &match);
	bool get_score_character_trans( search_match_ptr &match );

	void source_trans_switcheroo();
	bool get_score_word( search_match_ptr &match);
	bool get_score_character( search_match_ptr &match );

	bool get_score_character_common( search_match_ptr & match );
	void get_tags( const wstring raw_string, std::multiset<wstring> &tags );
	double get_format_penalty();
	void set_minimum_score( const double &score );
	double get_minimum_score() { return m_minimum_score ; }

	bool pass_minimum_tests();
	match_maker( double minimum_score = 0.5 )
		: m_minimum_score( minimum_score ), 
		m_num_rows(0), m_num_cols(0), 
		m_score(0.0),
		m_assess_format_penalty(false),
		m_match(new search_match)
	{ }
	bool get_score(const Segment row,
		const Segment col,
		int match_algo,
		search_match_ptr &match ) ;
	bool get_trans_score(const Segment row,
		const Segment col,
		int match_algo,
		search_match_ptr &match);

	void set_row_col_strings(const Segment &row,
		const Segment &col);
	void set_assess_format_penalty( bool assess )
	{
		m_assess_format_penalty = assess ;
	}
	bool get_assess_format_penalty() { return m_assess_format_penalty ; }
private:
	double m_score;

	double calculate_score( );
	boost::tuple<size_t, size_t> match_cells(size_t row_num, size_t col_num) const ;
	bool get_path ( ) ;
	bool populate_matrix_edges( ) ;
	bool populate_matrix_costs( ) ;

	void create_path_stacks( );

	double get_best_score(search_match_ptr& match);

	void set_perfect_match();

	bool below_min_distance(int min_dist);

	double calc_gloss_score(int min_dist);

	int get_min_distance( );

	void set_gloss_match_info(double gloss_score);

	void set_match_score();

	void put_together_markup(std::list< std::wstring >& col_list, std::list< std::wstring >& row_list);

	void compose_query_string(std::list< std::wstring >& row_list);

	void compose_source_string(std::list< std::wstring >& col_list);
public:
	wstring compose_markup_string( std::list< std::wstring >& element_list );
	wstring get_gloss_markup_start( double gloss_score );
	wstring get_gloss_markup_end(void);
} ;

}
