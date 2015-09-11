#pragma once

#include "TranslationMemory.h"
#include "MatchStringPairing.h"
#include "placement_holes.h"


namespace mem_engine
{
	namespace placement
	{
		typedef std::set< size_t > pos_holder ;

		/**
		* Substitutes number placements.
		* The simplest of placement rules. For example, given this query:
		*	 - AAA 111 BBB
		* And given this source and translation
		*   - AAA 222 BBB
		*	 - XXX 222 YYY
		* Then the rule will suggest the following translation:
		*	 - XXX 111 YYY
		*/
		class number_placer
		{
		public:
			//! Get placement candidates
			bool place(pairings_t &pairings, trans_pair &trans);

			std::wstring narrow_num_str( const std::wstring &SourceNum );
			//! Re-aligns the pairs.
			void re_align_pairs(pairings_t& pair_vec, const pos_holder &positions);
			//! Returns a number for placement.
			std::wstring get_num(pairings_t& pair_vec, size_t& CharPos, CharType ct, std::set< size_t > &positions );

		};
	}

}