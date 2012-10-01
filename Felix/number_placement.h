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
		public:
			// Get placement candidates
			bool place(pairings_t &pairings, trans_pair &trans, hole_pair_t &holes);

		};
	}

}