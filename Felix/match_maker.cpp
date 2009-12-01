#include "StdAfx.h"
#include "TranslationMemory.h"
#include "StringEx.h"
#include ".\match_maker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

namespace memory_engine
{
	const static wstring nomatch_tag_open1(L"<span class=\"nomatch\">") ;
	const static wstring nomatch_tag_open2(L"<span class=\"partial_match1\">") ;
	const static wstring nomatch_tag_open3(L"<span class=\"partial_match2\">") ;
	const static wstring nomatch_tag_close(L"</span>") ;


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
		cell( size_t d, size_t l, size_t a )
			:	m_diag( d), m_left( l ), m_above( a )
		{}
		
		size_t min_val()
		{
			size_t mval = min( m_diag, m_above ) ;
			return min( mval, m_left ) ;
		}
	};

	// ************************************
	// *
	// * match_maker member functions
	// *
	// *************************************

	// get_score

	// Function name	: 
	// Description	    : 
	// Return type		: bool match_maker::get_score 
	// Argument         : search_match_ptr &match
	bool match_maker::get_score (const Segment row,
		const Segment col,
		int match_algo,
		search_match_ptr &match )
	{
		set_row_col_strings(row, col);
		
		switch( match_algo )
		{
		case IDC_ALGO_CHAR:
			return get_score_character( match ) ;
		case IDC_ALGO_WORD:
			return get_score_word( match ) ;
		default:
			ATLASSERT( "Unknown match algorithm!" && FALSE ) ;
			return false ;
		}

	}
	bool match_maker::get_trans_score(const Segment row,
		const Segment col,
		int match_algo,
		search_match_ptr &match )
	{
		set_row_col_strings(row, col);

		switch( match_algo )
		{
		case IDC_ALGO_CHAR:
			return get_trans_score_character( match ) ;
		case IDC_ALGO_WORD:
			return get_trans_score_word( match ) ;
		default:
			ATLASSERT( "Unknown match algorithm!" && FALSE ) ;
			return false ;
		}
	}

	// get_path

	// Function name	: 
	// Description	    : 
	// Return type		: bool match_maker::get_path 
	bool match_maker::get_path ( )
	{
		create_path_stacks( ) ;
		// the query
		m_match->get_markup()->SetQuery( m_match->MatchPairing().MarkupQuery() ) ;
		// the source
		m_match->get_markup()->SetSource( m_match->MatchPairing().MarkupSource() ) ;
		// the translation -- unchanged for now, but we may make additions
		// if we add automatic vocab substitution
		m_match->get_markup()->SetTrans( m_match->get_record()->get_trans_rich() ) ;
		return true ;
	}

	// bool populate_matrix_edges

	// Function name	: match_maker::populate_matrix_edges
	// Description	    : 
	// Return type		: bool 
	bool match_maker::populate_matrix_edges( )
	{
		size_t num_rows = m_num_rows+1 ;
		size_t num_cols = m_num_cols+1;

		// populate row edges
		for ( size_t row_num = 0u ; row_num < num_rows ; row_num++ )
			m_matrix( row_num, 0u ) = static_cast< int >( row_num )  ;
		// populate column edges
		for ( size_t col_num = 0u ; col_num < num_cols ; col_num++ )
			m_matrix( 0u, col_num ) = static_cast< int >( col_num )  ;

		return true ;
	}

	// bool populate_matrix_costs

	// Function name	: match_maker::populate_matrix_costs
	// Description	    : 
	// Return type		: bool 
	bool match_maker::populate_matrix_costs( )
	{
		// variables for surrounding cells
		int above, left, diag ;

		int min_cost_cell ;

		wchar_t row_char, col_char ;

		size_t MaxLen = max( m_num_rows, m_num_cols ) ;
		size_t MaxDist = MaxLen - static_cast< size_t >( ( MaxLen * m_minimum_score ) ) ;

		int RowMin = 0 ;
		// variable for calculating cost
		int cost ;

		// step through rows
		LPCWSTR row_cstr = m_row_string.c_str() ;
		LPCWSTR col_cstr = m_col_string.c_str() ;
		for ( size_t row_num = 1 ; row_num <= m_num_rows ; row_num++ )
		{
			RowMin = MaxDist + 1 ;
			// step through coloumns
			for ( size_t col_num = 1 ; col_num <= m_num_cols ; col_num++ )
			{
				// get values of cells above, to left, and diagonal
				above = m_matrix( row_num-1, col_num ) ;
				left = m_matrix( row_num, col_num-1 ) ;
				diag = m_matrix( row_num-1, col_num-1 ) ;

				// Compute cost
				row_char = row_cstr[row_num-1] ;
				col_char = col_cstr[col_num-1] ;
				if ( row_char == col_char )
				{
					cost = 0 ;
				}
				else
				{
					cost = 1 ;
				}

				// get minimum cost
				// formula: cell = min( diag + cost, left + 1, above + 1 )
				min_cost_cell = ( diag + cost ) ;
				if ( min_cost_cell > ++left  )
					min_cost_cell = left  ;
				if ( min_cost_cell > ++above )
					min_cost_cell = above ;
				
				m_matrix( row_num, col_num ) = min_cost_cell ;
				RowMin = min( RowMin, min_cost_cell ) ;
			}
			if( RowMin > static_cast< int >( MaxDist ) )
			{
				return false ;
			}
		}

		return true ;
	}


	double match_maker::calculate_score()
	{
		// Formula for similarity score:
		// length of longest sentence = L
		// lower right corner of matrix = distance
		// score = ( L - distance ) / L 
		size_t high_len = __max( m_num_rows, m_num_cols ) ;
		int lower_right_corner = m_matrix( m_num_rows, m_num_cols ) ;
		return static_cast< double > ( high_len - lower_right_corner ) / static_cast< double >( high_len )  ;
	}


	void match_maker::match_cells(unsigned int &row_num, unsigned int &col_num)
	{
		int above = m_matrix( row_num-1, col_num );
		int left = m_matrix( row_num, col_num-1 );
		int diag = m_matrix( row_num-1, col_num-1 );
		if (diag<=above && diag<=left) // m_match symbols
		{
			int matrix_cell = m_matrix( row_num, col_num ) ;					
			if ( matrix_cell == diag )
			{
				m_match->MatchPairing().Match( m_col_show_string[--col_num], m_row_show_string[--row_num] ) ;
			}
			else
			{
				m_match->MatchPairing().NoMatch( m_col_show_string[--col_num], m_row_show_string[--row_num] ) ;
			}
		}	
		else if (above<left)
		{	
			// m_match row with epsilon
			m_match->MatchPairing().QueryToEpsilon( m_row_show_string[--row_num] ) ;
		}
		else
		{
			// m_match column with epsilon
			m_match->MatchPairing().SourceToEpsilon( m_col_show_string[--col_num] ) ;
		}
	}


	// Function name	: match_maker::set_minimum_score
	// Description	    : 
	// Return type		: void 
	// Argument         : const double &score
	void match_maker::set_minimum_score(const double &score)
	{
		m_minimum_score = score ;
	}



	double match_maker::get_format_penalty()
	{
		m_row_tags.clear() ;
		m_col_tags.clear() ;
		
		get_tags( m_row.rich(), m_row_tags ) ;
		get_tags( m_col.rich(), m_col_tags ) ;
		
		std::multiset<wstring>::iterator pos ;
		
		for ( pos=m_row_tags.begin() ; pos != m_row_tags.end() ; )
		{
			std::multiset< wstring >::iterator found_pos = m_col_tags.find(*pos) ;
			if ( found_pos != m_col_tags.end() ) 
			{
				m_col_tags.erase( found_pos ) ;
				m_row_tags.erase( pos ) ;
				pos = m_row_tags.begin() ;
			}
			else
			{
				++pos ;
			}
		}
		
		if ( m_row_tags.empty() && m_col_tags.empty() ) 
		{
			return 0.0 ;
		}
		
		return (double)( m_row_tags.size() + m_col_tags.size() ) / 100.0 ;
	}

	void match_maker::get_tags(const wstring &raw_string, std::multiset<wstring> &tags)
	{
		textstream_reader< wchar_t > reader ;
		
		wstring tag ;
		reader.set_buffer( raw_string.c_str() ) ;
		reader.find( L"<", true ) ;
		while ( reader.empty() == false ) 
		{
			reader.getline( tag, L'>', true ) ;
			str::make_lower( tag ) ;
			if ( tag.empty() == false && tag[0] != L'/' ) 
			{
				tags.insert( tag ) ;
			}
			reader.find( L"<", true ) ;
		}
		
	}

	bool match_maker::get_score_character(search_match_ptr &match)
	{
		m_match = match ;

		if ( m_num_cols == 0 )
		{
			return false ;
		}

		if ( ! pass_minimum_tests() )
		{
			return false ;
		}

		// resize matrix
		m_matrix.resize( m_num_rows+1, m_num_cols+1 ) ;
		
		// Populate matrix edges
		populate_matrix_edges( ) ;
		
		// Populate matrix cells
		if ( ! populate_matrix_costs( ) )
		{
			return false ;
		}
		// calculate score here
		// make sure we have at least the minimum score
		m_score = calculate_score() ;
		if ( m_score < m_minimum_score ) return false ;
		
		set_match_score() ;

		if ( FLOAT_EQ( m_match->get_score(), 1.0 ) )
		{
			set_perfect_match() ;
		}
		else
		{
			// Get the path
			get_path() ;
		}
		
		return true ;
	}
	bool match_maker::get_trans_score_character(search_match_ptr &match)
	{
		m_match = match ;
		// get matrix dimensions
		if ( m_num_cols == 0 )
			return false ;
		
		// the minimum possible score is difference in length, divided by max of the two lengths
		if ( ! pass_minimum_tests() )
			return false ;
		
		// resize matrix
		m_matrix.resize( m_num_rows+1, m_num_cols+1 ) ;
		
		// Populate matrix edges
		populate_matrix_edges( ) ;
		
		// Populate matrix cells
		if ( ! populate_matrix_costs( ) )
		{
			return false ;
		}
		
		// calculate score here
		m_score = calculate_score() ;
		
		// make sure we have at least the minimum score
		if ( m_score < m_minimum_score ) return false ;
		
		m_match->set_base_score( m_score ) ;

		if ( get_assess_format_penalty() ) 
		{
			double format_penalty = get_format_penalty() ;
			m_match->set_formatting_penalty( format_penalty ) ;
		}
		
		if ( FLOAT_EQ( m_match->get_score(), 1.0 ) )
		{
			set_perfect_match() ;
		}
		else
		{
			// Get the path
			get_path( ) ;
			// we need a little switcheroo here (because our matcher stupidly thinks the source is the query)
			wstring sw1, sw2 ;
			sw1 = m_match->get_markup()->GetSource() ;
			sw2 = m_match->get_record()->get_source_rich() ;
			m_match->get_markup()->SetSource( sw2 ) ;
			m_match->get_markup()->SetTrans( sw1 ) ;
		}
		
		return true ;
	}

	/*!
	 * \brief
	 * Write brief comment for get_score_word here.
	 * 
	 * \param search_match_ptr &match
	 * \param bool check_char
	 * Description of parameter search_match_ptr &match.
	 * Description of parameter bool check_char.
	 * 
	 * \returns
	 * bool
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for get_score_word here.
	 * 
	 * \remarks
	 * Write remarks for get_score_word here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	bool match_maker::get_score_word(search_match_ptr &match)
	{
		this->get_score_character(match) ;
		match->set_base_score( 0.0 ) ;

		m_match = match ;

		std::vector< wstring > col_tokens ;
		tokenize_words( m_col_string, col_tokens ) ;

		// get the col tokens
		wstring row_string = m_row_string ;
		std::vector< wstring > row_tokens ;
		tokenize_words( m_row_string, row_tokens ) ;

		// get our sizes
		size_t num_rows = row_tokens.size() ;
		size_t num_cols = col_tokens.size() ;

		// our matrix
		Matrix< cell > matrix ;
		matrix.resize( num_rows+1, num_cols+1 ) ;

		// ======================
		// Populate matrix edges
		// ======================

		matrix(0,0) = cell( 0, 0, 0 ) ;
		size_t running_cell_cost = 0 ;
		// populate row edges
		for ( size_t row_num = 1 ; row_num <= num_rows ; row_num++ )
		{
			running_cell_cost += row_tokens[row_num-1].size() ;
			matrix( row_num, 0 ) = cell( running_cell_cost, running_cell_cost, running_cell_cost) ;
		}
		// populate column edges				
		running_cell_cost = 0 ;
		for ( size_t col_num = 1 ; col_num <= num_cols ; col_num++ )
		{
			running_cell_cost += col_tokens[col_num-1].size() ;
			matrix( 0, col_num ) = cell( running_cell_cost, running_cell_cost, running_cell_cost)  ;
		}

		// ======================
		// Populate matrix cells
		// ======================
		
		// Populate matrix cells
		// variables for surrounding cells
		cell above, diag, left ;
		
		// variable for calculating cost
		cell c;

		wstring row_token_string, col_token_string ;
		size_t num_token_rows ;
		size_t num_token_cols ;
		Matrix< size_t > token_matrix ;

		// step through rows
		for ( size_t row_num = 1 ; row_num <= num_rows ; row_num++ )
		{
			// step through columns
			for ( size_t col_num = 1 ; col_num <= num_cols ; col_num++ )
			{
				// Compute cost
				const wstring row_token = row_tokens[row_num-1] ;
				row_token_string = row_token ;
				const wstring col_token = col_tokens[col_num-1] ;
				col_token_string = col_token ;

				num_token_rows = row_token_string.size() ;
				num_token_cols = col_token_string.size();

				// diag
				diag = matrix( row_num-1, col_num-1 ) ;
				c.m_diag = diag.min_val() + calc_word_distance( row_token_string, col_token_string, token_matrix ) ;

				// above
				above = matrix( row_num-1, col_num ) ;
				c.m_above = above.min_val() + num_token_rows ;

				// left
				left = matrix( row_num, col_num-1 ) ;
				c.m_left = left.min_val() + num_token_cols ;

				matrix( row_num, col_num ) = c ;
			}
		}
		  
		// Get the path
		size_t row_num = num_rows ;
		size_t col_num = num_cols ;
		
		// get the row tokens
		row_tokens.clear() ;
		tokenize_words( m_row_show_string, row_tokens ) ;
		col_tokens.clear() ;
		tokenize_words( m_col_show_string, col_tokens ) ;

		std::list< wstring > row_list ;
		std::list< wstring > col_list ;

		size_t total_cost = 0 ;

		// backtrack to find the path we took	
		while ( row_num || col_num )
		{
			if(!row_num) // no more source wstring to m_match
			{
				// match col wstring with epsilon
				col_list.push_front( nomatch_tag_open1 + col_tokens[--col_num] + nomatch_tag_close ) ;
				total_cost += col_tokens[col_num].size() ;
			}
			else if (!col_num) // no more translation wstring to m_match
			{
				// match row wstring with epsilon
				row_list.push_front( nomatch_tag_open1 + row_tokens[--row_num] + nomatch_tag_close ) ;
				total_cost += row_tokens[row_num].size() ;
			}
			else
			{
				c = 	matrix( row_num, col_num ) ;

				above = matrix( row_num-1, col_num ) ;
				left = matrix( row_num, col_num-1 ) ;
				diag = matrix( row_num-1, col_num-1 ) ;

				size_t above_cost = c.m_above - above.min_val() ;
				size_t left_cost = c.m_left - left.min_val() ;
				size_t diag_cost = c.m_diag - diag.min_val() ;

				if (c.m_diag<=c.m_above && c.m_diag<=c.m_left) // match symbols
				{
					--col_num ; --row_num ;

					if ( c.m_diag == diag.min_val() )
					{
						// m_match
						col_list.push_front( col_tokens[col_num] ) ;
						row_list.push_front( row_tokens[row_num] ) ;
					}
					else
					{
						size_t token_high_len = col_tokens[col_num].size() ;
						if ( token_high_len < row_tokens[row_num].size() )
						{
							token_high_len  = row_tokens[row_num].size() ;
						}

 						total_cost += diag_cost ;

						double token_score = (double)( token_high_len - diag_cost ) / (double) token_high_len ;

						// color code for size of mismatch!!!
						if ( token_score >= 0.8 )
						{
							col_list.push_front( nomatch_tag_open3 + col_tokens[col_num] + nomatch_tag_close ) ;
							row_list.push_front( nomatch_tag_open3 + row_tokens[row_num] + nomatch_tag_close ) ;
						}
						else if ( token_score >= 0.6 )
						{
							col_list.push_front( nomatch_tag_open2 + col_tokens[col_num] + nomatch_tag_close ) ;
							row_list.push_front( nomatch_tag_open2 + row_tokens[row_num] + nomatch_tag_close ) ;
						}
						else
						{
							col_list.push_front( nomatch_tag_open1 + col_tokens[col_num] + nomatch_tag_close ) ;
							row_list.push_front( nomatch_tag_open1 + row_tokens[row_num] + nomatch_tag_close ) ;
						}
					}
				}	
				else if (c.m_above < c.m_left )
				{	
					// m_match row with epsilon
					row_list.push_front( nomatch_tag_open1 + row_tokens[--row_num] + nomatch_tag_close ) ;
					total_cost += above_cost ;
				}
				else
				{
					// m_match column with epsilon
					col_list.push_front( nomatch_tag_open1 + col_tokens[--col_num] + nomatch_tag_close ) ;
					total_cost += left_cost ;
				}
			}
		}
		
		size_t high_len = m_match->get_record()->get_source_plain().size() ;
		if ( high_len < m_row_string.size() )
		{
			high_len = m_row_string.size() ;
		}

		if ( high_len <= total_cost )
		{
			m_score = 0.0 ;
		}
		else
		{
			m_score = (double)(high_len - total_cost) / (double) high_len ;
		}

		// make sure we have at least the minimum score
		if( m_score < m_minimum_score ) return false ;

		set_match_score() ;

		if ( FLOAT_EQ( m_match->get_score(), 1.0 ) )
		{
			set_perfect_match() ;
			return true ;
		}
		

		wstring marked_up_string ;

		std::list<wstring>::iterator mark_pos ;

		// the query
		for	( mark_pos = row_list.begin() ; mark_pos != row_list.end() ; ++mark_pos )
			marked_up_string += *mark_pos ;

		m_match->get_markup()->SetQuery( marked_up_string ) ;

		marked_up_string.erase() ;
		
		// the source
		for	( mark_pos = col_list.begin() ; mark_pos != col_list.end() ; ++mark_pos )
			marked_up_string += *mark_pos ;
		
		m_match->get_markup()->SetSource( marked_up_string );

		// the translation -- unchanged for now, but we may make additions
		// if we add automatic vocab substitution
		m_match->get_markup()->SetTrans( m_match->get_record()->get_trans_rich() ) ;


		return true ;
	}


	// Function name	: match_maker::get_trans_score_word
	// Description	    : 
	// Return type		: bool 
	// Argument         : search_match_ptr &match
	bool match_maker::get_trans_score_word(search_match_ptr &match)
	{
		this->get_trans_score_character(match) ;
		match->set_base_score( 0.0 ) ;

		m_match = match ;

		std::vector< wstring > col_tokens ;
		tokenize_words( m_match->get_record()->get_trans_cmp(), col_tokens ) ;

		// get the col tokens
		wstring row_string = m_row_string ;
		std::vector< wstring > row_tokens ;
		tokenize_words( m_row_string, row_tokens ) ;

		// get our sizes
		size_t num_rows = row_tokens.size() ;
		size_t num_cols = col_tokens.size() ;

		// our matrix
		Matrix< cell > matrix ;
		matrix.resize( num_rows+1, num_cols+1 ) ;

		// ======================
		// Populate matrix edges
		// ======================

		matrix(0,0) = cell( 0, 0, 0 ) ;
		size_t running_cell_cost = 0 ;
		// populate row edges
		for ( size_t row_num = 1 ; row_num <= num_rows ; row_num++ )
		{
			running_cell_cost += row_tokens[row_num-1].size() ;
			matrix( row_num, 0 ) = cell( running_cell_cost, running_cell_cost, running_cell_cost) ;
		}
		// populate column edges				
		running_cell_cost = 0 ;
		for ( size_t col_num = 1 ; col_num <= num_cols ; col_num++ )
		{
			running_cell_cost += col_tokens[col_num-1].size() ;
			matrix( 0, col_num ) = cell( running_cell_cost, running_cell_cost, running_cell_cost)  ;
		}

		// ======================
		// Populate matrix cells
		// ======================

		// Populate matrix cells
		// variables for surrounding cells
		cell above, diag, left ;

		// variable for calculating cost
		cell c;

		wstring row_token_string, col_token_string ;
		size_t num_token_rows ;
		size_t num_token_cols ;
		Matrix< size_t > token_matrix ;

		// step through rows
		for ( size_t row_num = 1 ; row_num <= num_rows ; row_num++ )
		{
			// step through columns
			for ( size_t col_num = 1 ; col_num <= num_cols ; col_num++ )
			{
				// Compute cost
				const wstring row_token = row_tokens[row_num-1] ;
				row_token_string = row_token ;
				const wstring col_token = col_tokens[col_num-1] ;
				col_token_string = col_token ;

				num_token_rows = row_token_string.size() ;
				num_token_cols = col_token_string.size();

				// diag
				diag = matrix( row_num-1, col_num-1 ) ;
				c.m_diag = diag.min_val() + calc_word_distance( row_token_string, col_token_string, token_matrix ) ;

				// above
				above = matrix( row_num-1, col_num ) ;
				c.m_above = above.min_val() + num_token_rows ;

				// left
				left = matrix( row_num, col_num-1 ) ;
				c.m_left = left.min_val() + num_token_cols ;

				matrix( row_num, col_num ) = c ;
			}
		}

		// Get the path
		size_t row_num = num_rows ;
		size_t col_num = num_cols ;

		// get the row tokens
		row_tokens.clear() ;
		tokenize_words( m_row_show_string, row_tokens ) ;
		col_tokens.clear() ;
		tokenize_words( m_match->get_record()->get_trans_plain(), col_tokens ) ;

		std::list< wstring > row_list ;
		std::list< wstring > col_list ;

		size_t total_cost = 0 ;

		// backtrack to find the path we took	
		while ( row_num || col_num )
		{
			if(!row_num) // no more source wstring to m_match
			{
				// match col wstring with epsilon
				col_list.push_front( nomatch_tag_open1 + col_tokens[--col_num] + nomatch_tag_close ) ;
				total_cost += col_tokens[col_num].size() ;
			}
			else if (!col_num) // no more translation wstring to m_match
			{
				// match row wstring with epsilon
				row_list.push_front( nomatch_tag_open1 + row_tokens[--row_num] + nomatch_tag_close ) ;
				total_cost += row_tokens[row_num].size() ;
			}
			else
			{
				c = 	matrix( row_num, col_num ) ;

				above = matrix( row_num-1, col_num ) ;
				left = matrix( row_num, col_num-1 ) ;
				diag = matrix( row_num-1, col_num-1 ) ;

				size_t above_cost = c.m_above - above.min_val() ;
				size_t left_cost = c.m_left - left.min_val() ;
				size_t diag_cost = c.m_diag - diag.min_val() ;

				if (c.m_diag<=c.m_above && c.m_diag<=c.m_left) // match symbols
				{
					--col_num ; --row_num ;

					if ( c.m_diag == diag.min_val() )
					{
						// m_match
						col_list.push_front( col_tokens[col_num] ) ;
						row_list.push_front( row_tokens[row_num] ) ;
					}
					else
					{
						size_t token_high_len = col_tokens[col_num].size() ;
						if ( token_high_len < row_tokens[row_num].size() )
						{
							token_high_len  = row_tokens[row_num].size() ;
						}

						total_cost += diag_cost ;

						double token_score = (double)( token_high_len - diag_cost ) / (double) token_high_len ;

						// color code for size of mismatch!!!
						if ( token_score >= 0.8 )
						{
							col_list.push_front( nomatch_tag_open3 + col_tokens[col_num] + nomatch_tag_close ) ;
							row_list.push_front( nomatch_tag_open3 + row_tokens[row_num] + nomatch_tag_close ) ;
						}
						else if ( token_score >= 0.6 )
						{
							col_list.push_front( nomatch_tag_open2 + col_tokens[col_num] + nomatch_tag_close ) ;
							row_list.push_front( nomatch_tag_open2 + row_tokens[row_num] + nomatch_tag_close ) ;
						}
						else
						{
							col_list.push_front( nomatch_tag_open1 + col_tokens[col_num] + nomatch_tag_close ) ;
							row_list.push_front( nomatch_tag_open1 + row_tokens[row_num] + nomatch_tag_close ) ;
						}
					}
				}	
				else if (c.m_above < c.m_left )
				{	
					// m_match row with epsilon
					row_list.push_front( nomatch_tag_open1 + row_tokens[--row_num] + nomatch_tag_close ) ;
					total_cost += above_cost ;
				}
				else
				{
					// m_match column with epsilon
					col_list.push_front( nomatch_tag_open1 + col_tokens[--col_num] + nomatch_tag_close ) ;
					total_cost += left_cost ;
				}
			}
		}

		size_t high_len = m_match->get_record()->get_trans_plain().size() ;
		if ( high_len < m_row_string.size() )
		{
			high_len = m_row_string.size() ;
		}

		if ( high_len <= total_cost )
		{
			m_score = 0.0 ;
		}
		else
		{
			m_score = (double)(high_len - total_cost) / (double) high_len ;
		}

		// make sure we have at least the minimum score
		if( m_score < m_minimum_score ) return false ;

		set_match_score() ;

		if ( FLOAT_EQ( m_match->get_score(), 1.0 ) )
		{
			set_perfect_match() ;
			return true ;
		}

		wstring marked_up_string ;

		std::list<wstring>::iterator mark_pos ;

		// the query
		for	( mark_pos = row_list.begin() ; mark_pos != row_list.end() ; ++mark_pos )
			marked_up_string += *mark_pos ;

		m_match->get_markup()->SetQuery( marked_up_string ) ;

		marked_up_string.erase() ;

		// the source
		for	( mark_pos = col_list.begin() ; mark_pos != col_list.end() ; ++mark_pos )
			marked_up_string += *mark_pos ;

		m_match->get_markup()->SetTrans( marked_up_string );

		// the translation -- unchanged for now, but we may make additions
		// if we add automatic vocab substitution
		m_match->get_markup()->SetSource( m_match->get_record()->get_source_rich() ) ;


		return true ;
	}

	// Function name	: match_maker::tokenize_words
	// Description	    : 
	// Return type		: bool 
	// Argument         : const wstring &words
	// Argument         : std::vector< wstring > &tokens
	bool match_maker::tokenize_words(const wstring &words, std::vector< wstring > &tokens)
	{
		textstream_reader< wchar_t > reader ;
		reader.set_buffer( words.c_str() ) ;

		wstring token ;
		while ( reader.empty() == false )
		{
			if ( reader.is_space() )
			{
				while ( reader.is_space() )
				{
					reader.get( token ) ;
				}
				tokens += token ;
				token.erase() ;
			}


			if ( reader.is_punct() )
			{
				reader.get( token ) ;

				tokens += token ;
				token.erase() ;
			}

			if ( reader.is_space() == false && reader.is_punct() == false && reader.empty() == false )
			{
				while ( reader.is_space() == false && ( reader.is_punct()  == false || reader.current_is( L'-' ) || reader.current_is( L'_') ) && reader.empty() == false )
				{
					reader.get( token ) ;
				}
				tokens += token ;
				token.erase() ;
			}
		}

		return true ;
	}

	size_t match_maker::calc_word_distance(const wstring &row_word, const wstring &col_word, Matrix< size_t > &token_matrix)
	{
		size_t num_token_rows = row_word.size() ;
		size_t num_token_cols = col_word.size() ;

		// resize matrix
		token_matrix.resize( num_token_rows+1, num_token_cols+1 ) ;
		
		token_matrix(0,0) = 0 ;
		// populate row edges
		for ( size_t token_row_num = 1 ; token_row_num <= num_token_rows ; token_row_num++ )
			token_matrix( token_row_num, 0 ) = token_row_num ;
		// populate column edges
		for ( size_t token_col_num = 1 ; token_col_num <= num_token_cols ; token_col_num++ )
			token_matrix( 0, token_col_num ) = token_col_num ;
		
		// Populate matrix cells
		// variables for surrounding cells
		size_t token_above, token_diag, token_left ;
		
		// variable for calculating cost
		size_t token_cost ;
		
		// step through rows
		for ( size_t token_row_num = 1 ; token_row_num <= num_token_rows ; token_row_num++ )
		{
			// step through columns
			for (  size_t token_col_num = 1 ; token_col_num <= num_token_cols ; token_col_num++ )
			{
				// get values of cells above and diagonal
				token_above = token_matrix( token_row_num-1, token_col_num ) + 1 ;
				token_diag = token_matrix( token_row_num-1, token_col_num-1 ) ;
				token_left = token_matrix( token_row_num, token_col_num-1 ) + 1 ;
				
				// Compute cost
				if ( row_word[token_row_num-1] != col_word[token_col_num-1] )
					token_diag++ ;
				
				token_cost = min( token_diag, token_left );
				
				// get minimum cost
				token_matrix( token_row_num, token_col_num ) = min( token_cost, token_above ) ;
			}
		}

		return token_matrix( num_token_rows, num_token_cols ) ;
	}

	bool match_maker::fuzzy_gloss_score(const Segment needle,
		const Segment haystack,
		search_match_ptr &match )
	{
		m_row = haystack ;
		m_col = needle ;
		m_match = match ;

		double score = m_distance.subdist_score(needle.cmp(), haystack.cmp()) ;

		if ( score < m_minimum_score )
		{
			return false ;
		}

		if(FLOAT_EQ(score, 1.0))
		{
			set_perfect_match() ;
			return true ;
		}
		
		set_gloss_match_info(score);
		return true ;
	}

	void match_maker::create_path_stacks( )
	{
		// current row/col
		// count backwards, load into stack
		size_t row_num = m_num_rows ;
		size_t col_num = m_num_cols ;

		// make sure that we have a clean slate.
		m_match->MatchPairing().clear() ;

		// backtrack to find the path we took	
		while ( row_num || col_num )
		{
			if( 0 == row_num) // no more source wstring to match
			{
				// m_match col wstring with epsilon
				m_match->MatchPairing().SourceToEpsilon( m_col_show_string[--col_num] ) ;
			}
			else if ( 0 == col_num) // no more translation wstring to match
			{
				// m_match row wstring with epsilon
				m_match->MatchPairing().QueryToEpsilon( m_row_show_string[--row_num] ) ;
			}
			else
			{
				match_cells( row_num, col_num ) ;
			}
		}
	}
	double match_maker::get_best_score(search_match_ptr& match)
	{
		double best_score = 0.0f ;

		Segment query(m_row) ;
		foreach(wstring token, m_word_tokens)
		{
			query.set_value(token) ;
			match->get_markup()->SetQuery( token ) ;
			double score = m_distance.edist_score(query.cmp(), m_col.cmp()) ;

			if ( FLOAT_EQ( score, 1.0 ) )
			{
				return score ;
			}
			if (score > best_score)
			{
				best_score = score ;
			}
		}

		return best_score;
	}
	void match_maker::set_perfect_match()
	{
		m_match->set_base_score( 1.0f )  ;
		m_match->set_formatting_penalty( 0.0f ) ;
		m_match->get_markup()->SetQuery( m_row.rich() ) ;
		m_match->get_markup()->SetTrans( m_match->get_record()->get_trans_rich() ) ;
		m_match->get_markup()->SetSource( m_match->get_record()->get_source_rich() ) ;

	}
	bool match_maker::below_min_distance(int min_dist)
	{
		return m_num_cols < static_cast< size_t >( min_dist );
	}
	double match_maker::calc_gloss_score(int min_dist)
	{
		return static_cast< double > ( m_num_cols - min_dist ) / static_cast< double >( m_num_cols );
	}
	int match_maker::get_min_distance( )
	{
		int min_dist = m_matrix( m_num_rows, m_num_cols ) ;
		int row_dist(0);
		for ( size_t min_row_num=m_num_rows-1 ; min_row_num ; --min_row_num )
		{
			row_dist = m_matrix(min_row_num, m_num_cols) ;
			if ( row_dist < min_dist )
			{
				min_dist = row_dist ;
			}
		}

		return min_dist;
	}

	void match_maker::set_gloss_match_info(double gloss_score)
	{
		m_match->set_base_score( gloss_score )  ;

		m_match->get_markup()->SetTrans( m_match->get_record()->get_trans_rich() ) ;

		wstring markup_string = 
			get_gloss_markup_start( gloss_score ) + 
			m_match->get_record()->get_source_rich() +
			get_gloss_markup_end() ;

		m_match->get_markup()->SetSource( markup_string ) ;
	}
	void match_maker::set_match_score()
	{
		m_match->set_base_score( m_score ) ;

		if ( get_assess_format_penalty() ) 
		{
			double format_penalty = get_format_penalty() ;
			m_match->set_formatting_penalty( format_penalty ) ;
		}

	}
	void match_maker::put_together_markup(std::list< std::wstring >& col_list, std::list< std::wstring >& row_list)
	{
		compose_query_string(row_list);

		compose_source_string(col_list);

		// the translation -- unchanged for now, but we may make additions
		// if we add automatic vocab substitution
		m_match->get_markup()->SetTrans( m_match->get_record()->get_trans_rich() ) ;

	}

	void match_maker::compose_source_string(std::list< std::wstring >& col_list)
	{
		m_match->get_markup()->SetSource( compose_markup_string( col_list ) );
	}

	void match_maker::compose_query_string(std::list< std::wstring >& row_list)
	{
		m_match->get_markup()->SetQuery( compose_markup_string(row_list ) ) ;
	}

	wstring match_maker::compose_markup_string( std::list< std::wstring >& element_list )
	{
		wstring marked_up_string ;
		std::list<wstring>::iterator mark_pos ;
		for	( mark_pos = element_list.begin() ; mark_pos != element_list.end() ; ++mark_pos )
		{
			marked_up_string += *mark_pos ;
		}
		return marked_up_string ;
	}

	wstring match_maker::get_gloss_markup_start( double gloss_score )
	{
		if ( gloss_score >= 0.9 )
		{
			return nomatch_tag_open1 ;
		}
		else if ( gloss_score >= 0.75 )
		{
			return nomatch_tag_open2 ;
		}
		else
		{
			return nomatch_tag_open3 ;
		}
	}

	wstring match_maker::get_gloss_markup_end(void)
	{
		return nomatch_tag_close ;
	}

	bool match_maker::pass_minimum_tests()
	{
		double high_len = (double)__max(m_num_cols,m_num_rows) ;
		double diff = abs(static_cast< double >( m_num_cols ) - static_cast< double >( m_num_rows ) ) ;
		return ( (high_len - diff) / high_len >= m_minimum_score ) ;
	}

	void match_maker::set_row_col_strings(const Segment &row,
		const Segment &col)
	{
		m_row = row ;
		m_col = col ;

		m_col_string = col.cmp() ;
		m_col_show_string = col.plain() ;
		m_num_cols = m_col_string.size() ;

		m_row_string = row.cmp() ;
		m_row_show_string = row.plain() ;
		m_num_rows = m_row_string.size() ;
		m_score = 0.0 ;
	}
}