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

	/**
		@struct cell
		@brief Represents a cell in the distance matrix.
	 */


	struct cell
	{
		size_t m_diag ;
		size_t m_left ;
		size_t m_above ;
		
		cell(  )
			:	m_diag( 0 ), m_left( 0 ), m_above( 0 )
		{}
		
		cell( const cell &cpy )
			:	m_diag( cpy.m_diag ), m_left( cpy.m_left ), m_above( cpy.m_above )
		{}
		cell( size_t diag, size_t left, size_t above )
			:	m_diag(diag), m_left(left), m_above(above)
		{}
		
		size_t min_val() const
		{
			return min3(m_diag, m_above, m_left) ;
		}
	};

	/** The Felix matching engine */
class match_maker
{
	Segment		m_row ;
	Segment		m_col ;

	wstring		m_row_string ;
	wstring		m_col_string ;

	wstring		m_col_show_string ;
	wstring		m_row_show_string ;

	size_t					m_num_rows ;
	size_t					m_num_cols ;
	double					m_minimum_score ;
	Matrix< size_t >		m_matrix ;
	search_match_ptr		m_match ;

	Distance		m_distance ;
	double			m_score;

public:
	match_maker( double minimum_score = 0.5 )
		: m_minimum_score( minimum_score ), 
		m_num_rows(0), m_num_cols(0), 
		m_score(0.0),
		m_assess_format_penalty(false),
		m_match(new search_match)
	{ }


	bool m_assess_format_penalty ;
	bool fuzzy_gloss_score(const Segment needle,
		const Segment haystack,
		search_match_ptr &match);

	size_t calc_word_distance( const wstring row_word, const wstring col_word, Matrix< size_t > &matrix );
	bool tokenize_words( const wstring words, std::vector< wstring > &tokens ) const;
	bool get_score_word_trans( search_match_ptr &match);

	bool get_score_character_trans( search_match_ptr &match );

	void source_trans_switcheroo();
	bool get_score_word( search_match_ptr &match);

	wstring get_fuzzy_tag_by_score(double score) const;

	void color_tokens_by_score( double token_score, 
							    std::list< wstring > &cols, const wstring &col, 
								std::list< wstring > &rows, const wstring &row ) const;
	void popuplate_matrix_edges_words( Matrix< cell > &matrix, std::vector< wstring > &row_tokens, std::vector< wstring > &col_tokens ) const;
	bool get_score_character( search_match_ptr &match );

	bool get_score_character_common( search_match_ptr & match );
	void get_tags( const wstring raw_string, std::multiset<wstring> &tags ) const;
	double get_format_penalty(wstring row, wstring col) const ;
	void set_minimum_score( const double &score );
	double get_minimum_score() const { return m_minimum_score ; }

	bool pass_minimum_tests() const ;

	bool get_score(const Segment row,
		const Segment col,
		int match_algo,
		search_match_ptr &match ) ;
	bool get_score_trans(const Segment row,
		const Segment col,
		int match_algo,
		search_match_ptr &match);

	void set_row_col_strings(const Segment &row,
		const Segment &col);
	void set_assess_format_penalty( bool assess )
	{
		m_assess_format_penalty = assess ;
	}
	bool get_assess_format_penalty() const { return m_assess_format_penalty ; }
	double calculate_score(size_t num_rows, size_t num_cols, int lower_right_corner) const;
	size_t compute_cost( const wchar_t row_char, const wchar_t col_char ) const ;

	boost::tuple<size_t, size_t> match_cells(size_t row_num, size_t col_num) const ;
	bool get_path ( ) ;
	bool populate_matrix_edges( ) ;
	// Is it possible that this match is within the minimum score?
	bool is_match_candidate( ) ;

	bool match_candidate_bag( const size_t MaxLen, const wstring &row, const wstring &col ) const;

	size_t bag_difference( const wstring &row, const wstring &col ) const;
	void create_path_stacks( );

	void set_perfect_match();

	void set_gloss_match_info(double gloss_score);

	void set_match_score();

	void put_together_markup(const std::list< std::wstring >& col_list, const std::list< std::wstring >& row_list);

	void compose_query_string(const std::list< std::wstring >& row_list);
	void compose_source_string(const std::list< std::wstring >& col_list);

	wstring compose_markup_string( const std::list< std::wstring >& element_list ) const;
	wstring get_gloss_markup_start( double gloss_score ) const ;
	wstring get_gloss_markup_end(void) const;
} ;

}
