#pragma once
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
	lhs - source
	rhs - query
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

	void create_new_pairings( pairings_t &pairings, const hole_t &hole ) ;
	}

}