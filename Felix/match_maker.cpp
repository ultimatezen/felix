#include "StdAfx.h"
#include "TranslationMemory.h"
#include "StringEx.h"
#include ".\match_maker.h"
#include "distance.h"

#ifdef max
#undef max
#endif

namespace mem_engine
{
	const static wstring fuzzy_tag_low(L"<span class=\"nomatch\">") ;
	const static wstring fuzzy_tag_mid(L"<span class=\"partial_match1\">") ;
	const static wstring fuzzy_tag_high(L"<span class=\"partial_match2\">") ;
	const static wstring fuzzy_tag_close(L"</span>") ;




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
			return get_score_character_trans( match ) ;
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
		const size_t num_rows = m_num_rows+1 ;
		const size_t num_cols = m_num_cols+1;

		// populate row edges
		for ( size_t row_num = 0u ; row_num < num_rows ; row_num++ )
			m_matrix( row_num, 0u ) = row_num ;
		// populate column edges
		for ( size_t col_num = 0u ; col_num < num_cols ; col_num++ )
			m_matrix( 0u, col_num ) = col_num  ;

		return true ;
	}

	// bool is_match_candidate
	// Could this match be within the minimum score?
	// Check each row, and fail as soon as possible.
	bool match_maker::is_match_candidate( )
	{
		const size_t MaxLen = std::max( m_num_rows, m_num_cols ) ;
		const size_t MaxDist = MaxLen - static_cast< size_t >( ( MaxLen * m_minimum_score ) ) ;

		size_t RowMin = 0 ;

		LPCWSTR row_cstr = m_row_string.c_str() ;
		LPCWSTR col_cstr = m_col_string.c_str() ;
		// step through rows
		for ( size_t row_num = 1 ; row_num <= m_num_rows ; row_num++ )
		{
			RowMin = MaxDist + 1 ;
			// step through coloumns
			for ( size_t col_num = 1 ; col_num <= m_num_cols ; col_num++ )
			{
				// get values of cells above, to left, and diagonal
				const size_t above = m_matrix( row_num-1, col_num ) ;
				const size_t left = m_matrix( row_num, col_num-1 ) ;
				const size_t diag = m_matrix( row_num-1, col_num-1 ) ;

				// Compute cost
				const size_t cost = compute_cost(row_cstr[row_num-1], col_cstr[col_num-1]);

				// get minimum cost
				// formula: cell = min( diag + cost, left + 1, above + 1 )
				const size_t min_cost_cell = min3(diag+cost, left+1, above+1) ;
				
				m_matrix( row_num, col_num ) = min_cost_cell ;
				RowMin = min( RowMin, min_cost_cell ) ;
			}
			if( RowMin >MaxDist )
			{
				return false ;
			}
		}

		return true ;
	}

	size_t match_maker::compute_cost( const wchar_t row_char, const wchar_t col_char ) const
	{
		if ( row_char == col_char )
		{
			return 0u ;
		}
		else
		{
			return 1u ;
		}
	}

	double match_maker::calculate_score(size_t num_rows, size_t num_cols, int lower_right_corner) const
	{
		// Formula for similarity score:
		// length of longest sentence = L
		// lower right corner of matrix = distance
		// score = ( L - distance ) / L 
		const size_t high_len = std::max( num_rows, num_cols ) ;
		return compute_score(high_len, lower_right_corner) ;
	}


	boost::tuple<size_t, size_t> match_maker::match_cells(const size_t row_num, const size_t col_num) const
	{
		const size_t above = m_matrix( row_num-1, col_num );
		const size_t left = m_matrix( row_num, col_num-1 );
		const size_t diag = m_matrix( row_num-1, col_num-1 );
		if (diag<=above && diag<=left) // m_match symbols
		{
			const size_t matrix_cell = m_matrix( row_num, col_num ) ;					
			if ( matrix_cell == diag )
			{
				m_match->MatchPairing().Match( m_col_show_string[col_num-1], m_row_show_string[row_num-1] ) ;
			}
			else
			{
				m_match->MatchPairing().NoMatch( m_col_show_string[col_num-1], m_row_show_string[row_num-1] ) ;
			}
			return boost::make_tuple(row_num-1, col_num-1) ;
		}	
		else if (above<left)
		{	
			// m_match row with epsilon
			m_match->MatchPairing().QueryToEpsilon( m_row_show_string[row_num-1] ) ;
			return boost::make_tuple(row_num-1, col_num) ;
		}
		else
		{
			// m_match column with epsilon
			m_match->MatchPairing().SourceToEpsilon( m_col_show_string[col_num-1] ) ;
			return boost::make_tuple(row_num, col_num-1) ;
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

	void match_maker::get_tags(const wstring raw_string, std::multiset<wstring> &tags)
	{
		textstream_reader< wchar_t > reader ;
		
		reader.set_buffer( raw_string.c_str() ) ;
		reader.find( L"<", true ) ;
		while ( reader.empty() == false ) 
		{
			const wstring tag = reader.getline(L'>', true ) ;
			if ( tag.empty() == false && tag[0] != L'/' ) 
			{
				tags.insert( boost::to_lower_copy(tag) ) ;
			}
			reader.find( L"<", true ) ;
		}
		
	}

	bool match_maker::get_score_character(search_match_ptr &match)
	{
		if (! get_score_character_common(match))
		{
			return false ;
		}


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
	bool match_maker::get_score_character_trans(search_match_ptr &match)
	{
		if (! get_score_character_common(match))
		{
			return false ;
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
			source_trans_switcheroo();
		}
		
		return true ;
	}
	bool match_maker::get_score_character_common( search_match_ptr & match )
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
		if ( ! is_match_candidate( ) )
		{
			return false ;
		}
		// calculate score here
		// make sure we have at least the minimum score
		m_score = calculate_score(m_num_rows, m_num_cols, m_matrix(m_num_rows, m_num_cols)) ;
		if ( m_score < m_minimum_score ) 
		{
			return false ;
		}

		set_match_score() ;
		return true ;
	}

	void match_maker::source_trans_switcheroo()
	{
		markup_ptr markup = m_match->get_markup() ;
		markup->SetTrans( markup->GetSource() ) ;
		markup->SetSource( m_match->get_record()->get_source_rich() ) ;
	}

	/*!
	 * get_score_word here.
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

		// declare and size our matrix
		Matrix< cell > matrix ;
		matrix.resize( row_tokens.size()+1, col_tokens.size()+1 ) ;

		// ======================
		// Populate matrix edges
		// ======================

		popuplate_matrix_edges_words(matrix, row_tokens, col_tokens);

		// ======================
		// Populate matrix cells
		// ======================
		
		// Populate matrix cells
		// variables for surrounding cells
		cell above, diag, left ;
		
		// variable for calculating cost
		cell c;

		wstring row_token_string, col_token_string ;
		Matrix< size_t > token_matrix ;

		// get our sizes
		const size_t num_rows = row_tokens.size() ;
		const size_t num_cols = col_tokens.size() ;

		// step through rows
		for ( size_t row_num = 1 ; row_num <= num_rows ; row_num++ )
		{
			// step through columns
			for ( size_t col_num = 1 ; col_num <= num_cols ; col_num++ )
			{
				// Compute cost
				const wstring row_token = row_tokens[row_num-1] ;
				const wstring col_token = col_tokens[col_num-1] ;

				const size_t num_token_rows = row_token.size() ;
				const size_t num_token_cols = col_token.size();

				// diag
				diag = matrix( row_num-1, col_num-1 ) ;
				c.m_diag = diag.min_val() + calc_word_distance( row_token, col_token, token_matrix ) ;

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
		wstring row_show = m_row_show_string ;
		for (size_t i = 0 ; i < row_tokens.size() ; ++i)
		{
			wstring token = row_tokens[i] ;
			size_t len = token.size() ;
			row_tokens[i] = row_show.substr(0, len) ;
			row_show = row_show.substr(len) ;
		}
		wstring col_show = m_col_show_string ;
		for (size_t i = 0 ; i < col_tokens.size() ; ++i)
		{
			wstring token = col_tokens[i] ;
			size_t len = token.size() ;
			col_tokens[i] = col_show.substr(0, len) ;
			col_show = col_show.substr(len) ;
		}

		std::list< wstring > row_list ;
		std::list< wstring > col_list ;

		size_t total_cost = 0 ;

		// backtrack to find the path we took	
		while ( row_num || col_num )
		{
			if(!row_num) // no more source wstring to m_match
			{
				// match col wstring with epsilon
				col_list.push_front( fuzzy_tag_low + col_tokens[--col_num] + fuzzy_tag_close ) ;
				total_cost += col_tokens[col_num].size() ;
			}
			else if (!col_num) // no more translation wstring to m_match
			{
				// match row wstring with epsilon
				row_list.push_front( fuzzy_tag_low + row_tokens[--row_num] + fuzzy_tag_close ) ;
				total_cost += row_tokens[row_num].size() ;
			}
			else
			{
				const cell &c = matrix( row_num, col_num ) ;

				above = matrix( row_num-1, col_num ) ;
				left = matrix( row_num, col_num-1 ) ;
				diag = matrix( row_num-1, col_num-1 ) ;


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

						const size_t diag_cost = c.m_diag - diag.min_val() ;
 						total_cost += diag_cost ;

						double token_score = (double)( token_high_len - diag_cost ) / (double) token_high_len ;

						const double FUZZY_HIGH = 0.8 ;
						const double FUZZY_MID = 0.6 ;
						// color code for size of mismatch!!!
						if ( token_score >= FUZZY_HIGH )
						{
							col_list.push_front( fuzzy_tag_high + col_tokens[col_num] + fuzzy_tag_close ) ;
							row_list.push_front( fuzzy_tag_high + row_tokens[row_num] + fuzzy_tag_close ) ;
						}
						else if ( token_score >= FUZZY_MID )
						{
							col_list.push_front( fuzzy_tag_mid + col_tokens[col_num] + fuzzy_tag_close ) ;
							row_list.push_front( fuzzy_tag_mid + row_tokens[row_num] + fuzzy_tag_close ) ;
						}
						else
						{
							col_list.push_front( fuzzy_tag_low + col_tokens[col_num] + fuzzy_tag_close ) ;
							row_list.push_front( fuzzy_tag_low + row_tokens[row_num] + fuzzy_tag_close ) ;
						}
					}
				}	
				else if (c.m_above < c.m_left )
				{	
					// match row with epsilon
					row_list.push_front( fuzzy_tag_low + row_tokens[--row_num] + fuzzy_tag_close ) ;
					const size_t above_cost = c.m_above - above.min_val() ;
					total_cost += above_cost ;
				}
				else
				{
					// match column with epsilon
					col_list.push_front( fuzzy_tag_low + col_tokens[--col_num] + fuzzy_tag_close ) ;
					const size_t left_cost = c.m_left - left.min_val() ;
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
			m_score = this->compute_score(high_len, total_cost) ;
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
		this->get_score_character_trans(match) ;
		match->set_base_score( 0.0 ) ;

		m_match = match ;

		std::vector< wstring > col_tokens ;
		tokenize_words( m_match->get_record()->get_trans_cmp(), col_tokens ) ;

		// get the col tokens
		wstring row_string = m_row_string ;
		std::vector< wstring > row_tokens ;
		tokenize_words( m_row_string, row_tokens ) ;

		// get our sizes
		const size_t num_rows = row_tokens.size() ;
		const size_t num_cols = col_tokens.size() ;

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

		Matrix< size_t > token_matrix ;

		// step through rows
		for ( size_t row_num = 1 ; row_num <= num_rows ; row_num++ )
		{
			// step through columns
			for ( size_t col_num = 1 ; col_num <= num_cols ; col_num++ )
			{
				// Compute cost
				const wstring row_token = row_tokens[row_num-1] ;
				const wstring col_token = col_tokens[col_num-1] ;

				const size_t num_token_rows = row_token.size() ;
				const size_t num_token_cols = col_token.size();

				// diag
				const cell diag = matrix( row_num-1, col_num-1 ) ;
				const size_t diag_val = diag.min_val() + calc_word_distance( row_token, col_token, token_matrix ) ;

				// left
				const cell left = matrix( row_num, col_num-1 ) ;
				const size_t left_val = left.min_val() + num_token_cols ;

				// above
				const cell above = matrix( row_num-1, col_num ) ;
				const size_t above_val = above.min_val() + num_token_rows ;

				matrix( row_num, col_num ) = cell(diag_val, left_val, above_val) ;
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
				col_list.push_front( fuzzy_tag_low + col_tokens[--col_num] + fuzzy_tag_close ) ;
				total_cost += col_tokens[col_num].size() ;
			}
			else if (!col_num) // no more translation wstring to m_match
			{
				// match row wstring with epsilon
				row_list.push_front( fuzzy_tag_low + row_tokens[--row_num] + fuzzy_tag_close ) ;
				total_cost += row_tokens[row_num].size() ;
			}
			else
			{
				const cell &c = 	matrix( row_num, col_num ) ;

				const cell &above = matrix( row_num-1, col_num ) ;
				const cell &left = matrix( row_num, col_num-1 ) ;
				const cell &diag = matrix( row_num-1, col_num-1 ) ;


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
						const size_t token_high_len = std::max(col_tokens[col_num].size(), row_tokens[row_num].size() );

						const size_t diag_cost = c.m_diag - diag.min_val() ;
						total_cost += diag_cost ;

						const double token_score = (double)( token_high_len - diag_cost ) / (double) token_high_len ;

						// color code for size of mismatch!!!
						if ( token_score >= 0.8 )
						{
							col_list.push_front( fuzzy_tag_high + col_tokens[col_num] + fuzzy_tag_close ) ;
							row_list.push_front( fuzzy_tag_high + row_tokens[row_num] + fuzzy_tag_close ) ;
						}
						else if ( token_score >= 0.6 )
						{
							col_list.push_front( fuzzy_tag_mid + col_tokens[col_num] + fuzzy_tag_close ) ;
							row_list.push_front( fuzzy_tag_mid + row_tokens[row_num] + fuzzy_tag_close ) ;
						}
						else
						{
							col_list.push_front( fuzzy_tag_low + col_tokens[col_num] + fuzzy_tag_close ) ;
							row_list.push_front( fuzzy_tag_low + row_tokens[row_num] + fuzzy_tag_close ) ;
						}
					}
				}	
				else if (c.m_above < c.m_left )
				{	
					// m_match row with epsilon
					row_list.push_front( fuzzy_tag_low + row_tokens[--row_num] + fuzzy_tag_close ) ;
					const size_t above_cost = c.m_above - above.min_val() ;
					total_cost += above_cost ;
				}
				else
				{
					// m_match column with epsilon
					col_list.push_front( fuzzy_tag_low + col_tokens[--col_num] + fuzzy_tag_close ) ;
					const size_t left_cost = c.m_left - left.min_val() ;
					total_cost += left_cost ;
				}
			}
		}

		const size_t high_len = std::max(m_match->get_record()->get_trans_plain().size(), m_row_string.size()) ;

		if ( high_len <= total_cost )
		{
			m_score = 0.0 ;
		}
		else
		{
			m_score = compute_score(high_len, total_cost) ;
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
	// Argument         : const wstring words
	// Argument         : std::vector< wstring > &tokens
	bool match_maker::tokenize_words(const wstring words, std::vector< wstring > &tokens)
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

	size_t match_maker::calc_word_distance(const wstring row_word, const wstring col_word, Matrix< size_t > &token_matrix)
	{
		const size_t num_token_rows = row_word.size() ;
		const size_t num_token_cols = col_word.size() ;

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
				boost::tie(row_num, col_num) = match_cells( row_num, col_num ) ;
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
			return fuzzy_tag_low ;
		}
		else if ( gloss_score >= 0.75 )
		{
			return fuzzy_tag_mid ;
		}
		else
		{
			return fuzzy_tag_high ;
		}
	}

	wstring match_maker::get_gloss_markup_end(void)
	{
		return fuzzy_tag_close ;
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

	void match_maker::popuplate_matrix_edges_words( Matrix< cell > &matrix, std::vector< wstring > &row_tokens, std::vector< wstring > &col_tokens ) const
	{
		matrix(0,0) = cell( 0, 0, 0 ) ;
		const size_t num_rows = row_tokens.size() ;
		const size_t num_cols = col_tokens.size() ;
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
	}

	double match_maker::compute_score( const size_t high_len, size_t total_cost ) const
	{
		// protect from divide by 0 error
		if (! high_len)
		{
			logging::log_warn("Divide by zero error: Attempted to compute_score with high len of 0") ;
			return 0.0 ;
		}
		ATLASSERT(high_len >= total_cost) ;
		const size_t matching_elements = std::max(high_len, total_cost) - total_cost ;
		return static_cast<double>(matching_elements) / static_cast<double>(high_len) ;
	}
}