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

		wstring hole_t::str_hole( const wstring text ) const
		{
			return text.substr(this->start, this->len) ;
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

		size_t hole_finder::find_start( const wstring &lhs, const wstring &rhs ) const
		{
			size_t start = 0 ;
			while(start < std::min(lhs.size(), rhs.size()) && lhs[start] == rhs[start])
			{
				++start ;
			}
			return start ;
		}
	}
}
