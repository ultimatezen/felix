#pragma once

#include "TranslationMemory.h"
#include "MatchStringPairing.h"

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
	typedef match_string_pairing::pair_list pairings_t ;
	typedef match_string_pairing::pairing_entity pairing_t ;

	// hole defs.

	/** A 'hole' in a string (non-matching portion.
	*/
	struct hole_t
	{
		size_t start ;
		size_t len ;
		hole_t() : start(0), len(0) {}
		hole_t(size_t s, size_t l) : start(s), len(l) {}
		/// Is it an empty hole?
		bool empty() const ;
		//! Given the start and end, define the hole start/len
		void define(size_t s, size_t end);
		//! Translate a hole into a substring.
		wstring get_str(const wstring text) const;
		wstring get_str(const pairings_t &pairings, const CharType &index) const;
		wstring get_str_source(const pairings_t &pairings) const;
		wstring get_str_query(const pairings_t &pairings) const;
	};

	/** A pair of holes
	*/
	struct hole_pair_t
	{
		hole_t lhs ;
		hole_t rhs ;
		hole_pair_t() {}
		hole_pair_t(hole_t l, hole_t r) : lhs(l), rhs(r) {}
		/// Are one of the holes empty?
		bool empty() const;
	};

	/**
	Finds 'holes' in two strings.
	*/
	class hole_finder
	{
	public:

		/** 
		Find a 'hole' between `lhs` and `rhs`.
		
		A hole is a mismatched segment between the two strings.
		Example: 
				AAA BBB XXX CCC
				AAA BBB YYY CCC
		Here, the hole is XXX vs YYY.
		*/
		bool find_hole(const wstring &lhs, const wstring &rhs, hole_pair_t &holes) const;
		bool find_hole(const pairings_t &pairings, hole_pair_t &holes) const;

		/// Advance over the part at the beginning that matches.
		size_t find_start( const wstring &lhs, const wstring &rhs ) const;


	};

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
	class gloss
	{
		memory_list &m_memories ;
	public:
		gloss(memory_list &memories) : m_memories(memories)
		{
		}
		// matches with source containing `text`
		void get_matches(search_match_container &matches, const wstring text);

		// Number of times needle is found in haystack
		size_t num_hits(const wstring needle, const wstring haystack);

		// Get the source matches that have gloss hits in translation
		void get_trans_subset(search_match_container &matches, const wstring trans);

		// Get placement candidates
		bool place(pairings_t &pairings, const wstring trans)
		{
			hole_pair_t holes ;
			hole_finder finder ;
			if (! finder.find_hole(pairings, holes))
			{
				return false ;
			}
			wstring source = holes.lhs.get_str_source(pairings); 
			wstring query = holes.rhs.get_str_query(pairings) ;

			search_match_container s_matches ;
			search_match_container q_matches ;

			this->get_matches(q_matches, query) ;
			this->get_matches(s_matches, source) ;
			this->get_trans_subset(s_matches, trans) ;

			return ! s_matches.empty() && ! q_matches.empty() ;
		}
	};

}
}