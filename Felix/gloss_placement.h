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


	/** 
	Make glossary placements in queries.

	Given a query Q and a fuzzy match S/T, such that
	Q = `AAA BBB CCC DDD`
	S = `XXX YYY CCC ZZZ`

	T = `111 222 333 444`

	Given the 'hole' be `BBB` for Q, and `YYY` for S:

	We then get all entries in our glossary with source `BBB` (GQ),
	and with source `YYY` (GS) and trans that is a subset of T (GT).

	We then posit a substitution of the translation of GQ with the
	translation of GS in S, as our placement candidate.

	There can be more than one placement candidate (one for each valid hole filling).

	*/
	class gloss_placer
	{
		memory_list &m_memories ;
	public:
		gloss_placer(memory_list &memories) : m_memories(memories)
		{
		}
		// Get placement candidates
		bool place(pairings_t &pairings, std::pair< wstring, wstring > &trans);

		bool is_valid_placement( hole_pair_t &holes, pairings_t & pairings, wstring & trans,
			search_match_container &q_matches, search_match_container &s_matches);
		// matches with source containing `text`
		size_t get_matches(search_match_container &matches, const wstring text);

		// Number of times needle is found in haystack
		size_t num_hits(const wstring needle, const wstring haystack) const;

		// Get the source matches that have gloss hits in translation
		size_t get_trans_subset(search_match_container &matches, const wstring trans) const;

		void replace_trans_term( const wstring qword, const wstring trans_plain, std::pair< wstring, wstring > & trans ) const;
		void create_new_pairings( pairings_t &pairings, const hole_pair_t &holes ) const;
	};

}
}