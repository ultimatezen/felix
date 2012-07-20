#pragma once

#include "TranslationMemory.h"

namespace mem_engine
{
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

	/**
	Glossary placement.

	Given a query Q and a fuzzy match S/T, such that
		Q = `AAA BBB CCC DDD`
		S = `XXX YYY CCC ZZZ`

		T = `111 222 333 444`

	The 'hole' would be `BBB` for Q, and `YYY` for S.

	We then get all entries in our glossary with source `BBB` (GQ),
	and with source `YYY` and trans that is a subset of T (GS).

	We then posit a substitution of the translation of GQ with the
	translation of GS in S, as our placement candidate.

	There can be more than one placement candidate (one for each valid hole filling).
	*/
	class gloss_placement
	{
	public:

		// hole defs.
		struct hole_t
		{
			size_t start ;
			size_t len ;
			hole_t() : start(0), len(0) {}
			hole_t(size_t s, size_t l) : start(s), len(l) {}
			bool empty() { return len == 0 ;}
			void define(size_t s, size_t end)
			{
				start = s ;
				len = std::max(s, end) - s ;
			}
		};
		struct hole_pair_t
		{
			hole_t lhs ;
			hole_t rhs ;
			hole_pair_t() {}
			hole_pair_t(hole_t l, hole_t r) : lhs(l), rhs(r) {}
			bool empty() {return lhs.empty() || rhs.empty() ;}
		};

		// Find a 'hole' between `lhs` and `rhs`.
		// A hole is a mismatched segment between the two strings.
		// Example: 
		//		AAA BBB XXX CCC
		//		AAA BBB YYY CCC
		// Here, the hole is XXX vs YYY.
		bool find_hole(const wstring &lhs, const wstring &rhs, hole_pair_t &holes) const
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

		// Advance over the part at the beginning that matches.
		size_t find_start( const wstring &lhs, const wstring &rhs ) const
		{
			size_t start = 0 ;
			while(start < std::min(lhs.size(), rhs.size()) && lhs[start] == rhs[start])
			{
				++start ;
			}
			return start ;
		}
		// Translate a hole into a substring.
		wstring str_hole(const wstring text, const hole_t h) const
		{
			return text.substr(h.start, h.len) ;
		}

	};
}
