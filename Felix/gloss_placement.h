#pragma once

#include "TranslationMemory.h"

namespace mem_engine
{
#ifdef min
#undef min
#endif

	// Place glossary matches.
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
		};
		struct hole_pair_t
		{
			hole_t lhs ;
			hole_t rhs ;
			hole_pair_t() {}
			hole_pair_t(hole_t l, hole_t r) : lhs(l), rhs(r) {}
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

			// Advance over the part at the beginning that matches.
			size_t start = 0 ;
			while(start < std::min(lhs.size(), rhs.size()) && lhs[start] == rhs[start])
			{
				++start ;
			}

			// Advance over the part at the end that matches.
			size_t end1 = lhs.size() ;
			size_t end2 = rhs.size() ;
			while(end1 && end2 && lhs[end1-1] == rhs[end2-1])
			{
				--end1 ;
				--end2 ;
			}

			// Define the hole.
			holes.lhs.start = start ;
			holes.rhs.start = start ;

			holes.lhs.len = end1 - start ;
			holes.rhs.len = end2 - start ;

			return true ;
	
		}

		// Translate a hole into a substring.
		wstring str_hole(const wstring text, const hole_t h) const
		{
			return text.substr(h.start, h.len) ;
		}

	};
}