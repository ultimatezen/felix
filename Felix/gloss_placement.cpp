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
			while(start < pairvec.size() && pairvec[start].m_MatchType != NOMATCH)
			{
				++start ;
			}

			size_t end = pairvec.size() ;
			while(end && pairvec[end-1].m_MatchType != NOMATCH)
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

		size_t gloss::num_hits( const wstring needle, const wstring haystack )
		{
			size_t count = 0 ;
			for(size_t pos = haystack.find(needle) ; pos != wstring::npos; pos = haystack.find(needle, pos+1))
			{
				++count ;
			}
			return count ;
		}

		void gloss::get_matches( search_match_container &matches, const wstring text )
		{
			foreach(memory_pointer mem, m_memories)
			{
				mem->get_perfect_matches(matches, text) ;
			}
		}

		void gloss::get_trans_subset( search_match_container &matches, const wstring trans )
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
		}
	}
}
