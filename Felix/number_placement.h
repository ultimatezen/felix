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
		typedef pair_list pairings_t ;
		typedef pairing_entity pairing_t ;

		class number_placer
		{

		};
	}

}