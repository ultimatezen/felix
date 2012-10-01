#pragma once

#include "TranslationMemory.h"
#include "MatchStringPairing.h"
#include "placement_holes.h"


namespace mem_engine
{
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

	namespace placement
	{

		class number_placer
		{
			std::set< size_t > m_placement_positions ;
		public:
			// Get placement candidates
			bool place(pairings_t &pairings, trans_pair &trans, hole_pair_t &holes);
			// Re-aligns the pairs.
			void re_align_pairs(pairings_t& pair_vec);
			// Returns a number for placement.
			std::wstring get_num(pairings_t& pair_vec, size_t& CharPos, CharType ct, std::set< size_t > &positions );
			void clear();
		};
	}

}