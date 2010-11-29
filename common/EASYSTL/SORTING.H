#ifndef SORTING_H
#define SORTING_H

// sorting.h -- versions of sorting STL algorithms that accept entire 
//			    containers rather than iterators.

// Author: Brandon Corfman, 7/23/02

#include <algorithm>

namespace easystl
{
	//
	// sort
	// 
	template <typename C> C sort(C c)
	{
		std::sort(c.begin(), c.end());
		return c;
	}

	template <typename C, typename Op> C sort(C c, Op op)
	{
		std::sort(c.begin(), c.end(), op);
		return c;
	}

	//
	// stable_sort
	//
	template <typename C> C stable_sort(C c)
	{
		std::stable_sort(c.begin(), c.end());
		return c;
	}

	template <typename C, typename Op> C stable_sort(C c, Op op)
	{
		std::stable_sort(c.begin(), c.end(), op);
		return c;
	}

}

#endif
