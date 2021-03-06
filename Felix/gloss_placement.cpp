#include "stdafx.h"
#include "gloss_placement.h"

namespace mem_engine
{
	namespace placement
	{


		/************************************************************************/
		/* gloss                                                                */
		/************************************************************************/

		// Try to place the glossary match in our source/trans from the query.
		bool gloss_placer::place( pairings_t &pairings, std::pair< wstring, wstring > &trans, hole_pair_t &holes )
		{

			/* First, see if it's a valid placement.
			 * Got to:
			 *	1. Have a gloss match for the query hole.
			 *	2. Have a gloss match for the source hole.
			 *	3. Have a match between the gloss trans and the source translation.
			 */

			search_match_container q_matches ;
			search_match_container s_matches ;
			if (! is_valid_placement(holes, pairings, trans.first, q_matches, s_matches))
			{
				return false ;
			}

			// It's a valid placement, so create the new pairings
			create_new_pairings(pairings, holes.lhs);

			// Now, do the replacement in the translation segment.
			auto qmatch = *q_matches.begin() ;
			auto qrec = qmatch->get_record() ;
			auto smatch = *s_matches.begin() ;
			auto srec = smatch->get_record() ;

			// replace the translation
			replace_trans_term(qrec->get_trans_plain(), srec->get_trans_plain(), trans);

			return true ;
		}

		// Get the matches for our glossary entry in TM.
		size_t gloss_placer::get_matches( search_match_container &matches, const wstring text )
		{
			FOREACH(memory_pointer mem, m_memories)
			{
				mem->get_perfect_matches(matches, text) ;
			}
			return matches.size() ;
		}

		// Narrow down gloss matches to those whose translations are in trans.
		size_t gloss_placer::get_trans_subset( search_match_container &matches, const wstring trans ) const
		{
			search_match_container tmp ;
			FOREACH(search_match_ptr match, matches)
			{
				wstring gloss_hit = match->get_record()->get_trans_plain() ;
				// IFF it occurs once in trans
				if (this->num_hits(gloss_hit, trans) == 1)
				{
					tmp.insert(match) ;
				}
			}
			std::swap(matches, tmp) ;
			return matches.size() ;
		}

		// How many matches are in the translation?
		// We want to make sure it is in the translation once, and only once.
		size_t gloss_placer::num_hits( const wstring needle, const wstring haystack ) const
		{
			size_t count = 0 ;
			for(size_t pos = haystack.find(needle) ; pos != wstring::npos; pos = haystack.find(needle, pos+1))
			{
				++count ;
			}
			return count ;
		}

		// Replace the translated term in the translation
		void gloss_placer::replace_trans_term( const wstring qword, const wstring trans_plain, std::pair< wstring, wstring > & trans ) const
		{
			const static wstring placement_fmt( L"<span class=\"placement\">%s</span>" ) ;
			wstring replacement = ( boost::wformat( placement_fmt ) % qword ).str() ;
			boost::replace_first(trans.first, trans_plain, qword) ;
			boost::replace_first(trans.second, trans_plain, replacement) ;
			fix_html_entities(trans.first) ;
			fix_html_entities(trans.second) ;
			fix_match_spans(trans.second) ;
		}

		// Gathers up the matches for the query and source, and determines whether this is a valid placement.
		bool gloss_placer::is_valid_placement( const hole_pair_t &holes, const pairings_t & pairings, const wstring & trans, search_match_container &q_matches, search_match_container &s_matches )
		{
			const wstring query = holes.rhs.get_str_query(pairings) ;
			if(this->get_matches(q_matches, query) == 0)
			{
				return false ;
			}

			const wstring source = holes.lhs.get_str_source(pairings); 
			if(this->get_matches(s_matches, source) == 0)
			{
				return false ;
			}
			if(this->get_trans_subset(s_matches, trans) == 0)
			{
				return false ;
			}
			return true ;
		}
	}
}
