#include "stdafx.h"
#include "number_placement.h"

namespace mem_engine
{
	namespace placement
	{
		// Try to find a number placement.
		bool number_placer::place( pairings_t &pairings, trans_pair &trans )
		{
			const static wstring placement_fmt( L"<span class=\"placement\">%s</span>" ) ;

			std::set< size_t > positions ;

			bool PairedNums = false ;

			for( size_t i = 0 ; i < pairings.size() ;)
			{
				pairing_entity pe = pairings[i] ;
				if( is_num_pair(pe)
					&& pe.m_MatchType == NOMATCH )
				{
					size_t SourcePos = i ;
					std::wstring SourceNum = get_num(pairings, SourcePos, SOURCE, positions);

					size_t QueryPos = i ;
					std::wstring QueryNum = get_num(pairings, QueryPos, QUERY, positions);

					// is the num in the translation? (but only once...)
					size_t TransPos = trans.first.find( SourceNum ) ;
					if (TransPos == wstring::npos && has_asian(SourceNum))
					{
						SourceNum = narrow_num_str(SourceNum);
						TransPos = trans.first.find( SourceNum ) ;
					}

					if( is_substitution(trans, SourceNum, TransPos, QueryNum) ) 
					{
						PairedNums = true ;
						if (! has_asian(SourceNum) && has_asian(QueryNum))
						{
							QueryNum = narrow_num_str(QueryNum);
						}

						boost::replace_first( trans.first, SourceNum, QueryNum ) ;
						boost::replace_first( trans.second, SourceNum, ( boost::wformat( placement_fmt ) % QueryNum ).str() ) ;
					}


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
				re_align_pairs(pairings, positions);
				fix_html_entities(trans.first);
				fix_html_entities(trans.second);
				fix_match_spans(trans.second);
			}

			return PairedNums ;
		}

		// If we have aligned any positions, re-mark them as `PLACEMENT` in the pair vec,
		// and place the chars from query into source.
		void number_placer::re_align_pairs( pairings_t& pair_vec, const pos_holder &positions )
		{
			pairings_t temp ;

			for( size_t i = 0 ; i < pair_vec.size() ; ++i )
			{
				pairing_entity pe = pair_vec[i] ;

				if ( positions.find( i ) == positions.end() )
				{
					temp.push_back(pe) ;
				}
				else
				{
					if( pe.query() != 0 )
					{
						pe.source() = pe.query() ;
						pe.m_MatchType = PLACEMENT ;
						temp.push_back(pe) ;
					}
				}
			}
			pair_vec.clear() ;
			pair_vec.assign(temp.begin(), temp.end()) ;
		}

		// Get an aligned number from the source or query.
		std::wstring number_placer::get_num( pairings_t& pair_vec, size_t& CharPos, CharType ct, pos_holder &positions )
		{
			while( is_num_or_null(pair_vec[CharPos].get_char(ct)) && CharPos > 0 )
			{
				CharPos-- ;
			}

			if ( ! is_num_or_null(pair_vec[CharPos].get_char(ct)) )
			{
				CharPos++ ;
			}
			ATLASSERT( is_num_or_null(pair_vec[CharPos].get_char(ct)) ) ;

			wstring Num ;
			while( CharPos < pair_vec.size() && is_num_or_null(pair_vec[CharPos].get_char(ct)) )
			{
				if( pair_vec[CharPos].get_char(ct) != 0 )
				{
					positions.insert( CharPos ) ;
					Num += pair_vec[CharPos].get_char(ct) ;
				}
				CharPos++ ;
			}

			return Num;
		}

		// Normaize number to single-byte chars.
		std::wstring number_placer::narrow_num_str( const std::wstring &SourceNum )
		{
			wstring newsource ;
			FOREACH(wchar_t c, SourceNum)
			{
				newsource += narrow_num(c) ;
			}
			return newsource;
		}
	}
}

