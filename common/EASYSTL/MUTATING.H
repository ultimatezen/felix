#ifndef MUTATING_H
#define MUTATING_H

// Mutating.h -- versions of mutating STL algorithms that accept entire 
//               containers rather than iterators.

// Author: Brandon Corfman, 7/23/02

#include <list>
#include <deque>
#include <vector>

namespace easystl
{
	//
	// reverse
	//
	template <typename T> std::list<T> reverse(std::list<T> c)
	{
		c.reverse();
		return c;
	}

	template <typename T> std::deque<T> reverse(std::deque<T> c)
	{
		std::reverse(c.begin(), c.end());
		return c;
	}

	template <typename T> std::vector<T> reverse(std::vector<T> c)
	{
		std::reverse(c.begin(), c.end());
		return c;
	}

	//
	// rotate
	//
	template <typename C> C rotate(C c, typename C::size_type idx)
	{
		C::iterator beg = c.begin();
		C::iterator pos = beg + idx;
		std::rotate(c.begin(), pos, c.end());
		return c;
	}

	//
	// next_permutation
	//
	template <typename C> 
		C next_permutation(C c)
	{
		std::next_permutation(c.begin(), c.end());
		return c;
	}

	//
	// prev_permutation
	//
	template <typename C> 
		C prev_permutation(C c)
	{
		std::prev_permutation(c.begin(), c.end());
		return c;
	}

	// 
	// random_shuffle
	//
	template <typename C> 
		C random_shuffle(C c)
	{
		std::random_shuffle(c.begin(), c.end());
		return c;
	}

	template <typename C, typename Op> 
		void random_shuffle(C c, Op op)
	{
		std::random_shuffle(c.begin(), c.end(), op);
		return c;
	}

	// 
	// partition
	//
	template <typename C, typename Op>
		typename C::iterator partition(C& c, Op op)
	{
		return std::partition(c.begin(), c.end(), op);
	}

	// 
	// stable_partition
	//
	template <typename C, typename Op>
		typename C::iterator stable_partition(C& c, Op op)
	{
		return std::stable_partition(c.begin(), c.end(), op);
	}
}

#endif
