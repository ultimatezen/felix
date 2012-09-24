#include "stdafx.h"
#include "gloss_placement.h"

namespace mem_engine
{
	namespace placement
	{
		//////////////////////////////////////////////////////////////////////////
		// hole_t
		//////////////////////////////////////////////////////////////////////////

		bool hole_t::empty() const
		{
			return len == 0 ;
		}

		void hole_t::define( size_t s, size_t end )
		{
			start = s ;
			len = std::max(s, end) - s ;
		}

		wstring hole_t::get_str( const wstring text ) const
		{
			return text.substr(this->start, this->len) ;
		}

		wstring hole_t::get_str( const pairings_t &pairings, const CharType &index ) const
		{
			wstring out ;

			std::vector<pairing_t> pairvec ;
			auto start = pairings.begin() ;
			auto end = pairings.begin() ;
			std::advance(start, this->start) ;
			std::advance(end, this->start + this->len) ;
			while(start != end)
			{
				const wchar_t &c = start->get_char(index) ;
				if (c)
				{
					out += c ;
				}
				++start ;
			}
			return out ;
		}

		wstring hole_t::get_str_source(const pairings_t &pairings) const
		{
			return this->get_str(pairings, SOURCE) ;
		}
		wstring hole_t::get_str_query(const pairings_t &pairings) const
		{
			return this->get_str(pairings, QUERY) ;
		}

		//////////////////////////////////////////////////////////////////////////
		// hole_pair_t
		//////////////////////////////////////////////////////////////////////////

		bool hole_pair_t::empty() const
		{
			return lhs.empty() || rhs.empty() ;
		}

		//////////////////////////////////////////////////////////////////////////
		// hole_finder
		//////////////////////////////////////////////////////////////////////////

		bool hole_finder::find_hole( const wstring &lhs, const wstring &rhs, hole_pair_t &holes ) const
		{
			// If either string is empty, there is no hole.
			if (lhs.empty() || rhs.empty())
			{
				return false ;
			}

			// Get the first place where they don't match.
			const size_t start = find_start(lhs, rhs);

			// Advance over the part at the end that matches.
			size_t end1 = lhs.size() ;
			size_t end2 = rhs.size() ;
			while(end1 && end2 && lhs[end1-1] == rhs[end2-1])
			{
				--end1 ;
				--end2 ;
			}

			// Define the holes.
			holes.lhs.define(start, end1) ;
			holes.rhs.define(start, end2) ;

			return ! holes.empty() ;
		}

		bool hole_finder::find_hole( const pairings_t &pairings, hole_pair_t &holes ) const
		{
			if (pairings.empty())
			{
				return false ;
			}
			std::vector<pairing_t> pairvec ;
			pairvec.assign(pairings.begin(), pairings.end()) ;

			size_t start = 0 ;
			while(start < pairvec.size() && pairvec[start].match_type() != NOMATCH)
			{
				++start ;
			}

			size_t end = pairvec.size() ;
			while(end && pairvec[end-1].match_type() != NOMATCH)
			{
				--end ;
			}

			holes.lhs.define(start, end) ;
			holes.rhs.define(start, end) ;


			return ! holes.empty() ;
		}
		size_t hole_finder::find_start( const wstring &lhs, const wstring &rhs ) const
		{
			size_t start = 0 ;
			while(start < std::min(lhs.size(), rhs.size()) && lhs[start] == rhs[start])
			{
				++start ;
			}
			return start ;
		}

		/************************************************************************/
		/* gloss                                                                */
		/************************************************************************/

		bool gloss_placer::place( pairings_t &pairings, std::pair< wstring, wstring > &trans )
		{

			hole_pair_t holes ;
			hole_finder finder ;
			if (! finder.find_hole(pairings, holes))
			{
				return false ;
			}

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
			create_new_pairings(pairings, holes);

			// Now, do the replacement in the translation segment.
			auto qmatch = *q_matches.begin() ;
			auto qrec = qmatch->get_record() ;
			auto smatch = *s_matches.begin() ;
			auto srec = smatch->get_record() ;

			// replace the translation
			replace_trans_term(qrec->get_trans_plain(), srec->get_trans_plain(), trans);

			return true ;
		}

		size_t gloss_placer::get_matches( search_match_container &matches, const wstring text )
		{
			foreach(memory_pointer mem, m_memories)
			{
				mem->get_perfect_matches(matches, text) ;
			}
			return matches.size() ;
		}

		size_t gloss_placer::num_hits( const wstring needle, const wstring haystack ) const
		{
			size_t count = 0 ;
			for(size_t pos = haystack.find(needle) ; pos != wstring::npos; pos = haystack.find(needle, pos+1))
			{
				++count ;
			}
			return count ;
		}


		size_t gloss_placer::get_trans_subset( search_match_container &matches, const wstring trans ) const
		{
			search_match_container tmp ;
			foreach(search_match_ptr match, matches)
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


		void gloss_placer::create_new_pairings( pairings_t &pairings, const hole_pair_t &holes ) const
		{
			const wstring query = holes.lhs.get_str_query(pairings) ;
			std::vector<pairing_t> pairvec ;
			pairvec.assign(pairings.begin(), pairings.end()) ;

			pairings.clear() ;

			// start
			for(size_t i=0 ; i < holes.lhs.start ; ++i)
			{
				pairings.push_back(pairvec[i]) ;
			}

			// middle
			foreach(wchar_t c, query)
			{
				pairings.push_back(pairing_entity(c, PLACEMENT, c)) ;
			}

			// end
			for(size_t i=holes.lhs.start + holes.lhs.len ; i < pairvec.size() ; ++i)
			{
				pairings.push_back(pairvec[i]) ;
			}

		}

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

		bool gloss_placer::is_valid_placement( hole_pair_t &holes, pairings_t & pairings, wstring & trans, search_match_container &q_matches, search_match_container &s_matches )
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
