#ifndef SORTED_RANGE_H
#define SORTED_RANGE_H

// Sorted_range.h -- versions of sorted range STL algorithms that accept entire 
//					 containers rather than iterators.

// Author: Brandon Corfman, 7/23/02

#include <algorithm>

namespace easystl
{
	//
	// binary_search
	//
	template <typename C, typename T>
		bool binary_search(C& c, const T& val)
	{
		return std::binary_search(c.begin(), c.end(), val);
	}

	template <typename C, typename T, typename Op>
		bool binary_search(C& c, const T& val, Op op)
	{
		return std::binary_search(c.begin(), c.end(), val, op);
	}

	// 
	// includes
	//
	template <typename C1, typename C2>
		bool includes(C1& c1, C2& c2)
	{
		return std::includes(c1.begin(), c1.end(), c2.begin(), c2.end());
	}

	template <typename C1, typename C2, typename Op>
		bool includes(C1& c1, C2& c2, Op op)
	{
		return std::includes(c1.begin(), c1.end(), c2.begin(), c2.end(), op);
	}

	//
	// merge
	//
	template <typename C>
		C merge(const C& c1, const C& c2)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::merge(c1.begin(), c1.end(), c2.begin(), c2.end(), c3.begin());
		c3.erase(i, c3.end());
		return c3;
	}

	template <typename C, typename Op>
		C merge(const C& c1, const C& c2, Op op)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::merge(c1.begin(), c1.end(), c2.begin(), c2.end(), c3.begin(), op);
		c3.erase(i, c3.end());
		return c3;
	}

	// 
	// set_union
	//
	template <typename C>
		C set_union(const C& c1, const C& c2)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::set_union(c1.begin(), c1.end(), c2.begin(), c2.end(), c3.begin());
		c3.erase(i, c3.end());
		return c3;
	}

	template <typename C, typename Op>
		C set_union(const C& c1, const C& c2, Op op)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::set_union(c1.begin(), c1.end(), c2.begin(), c2.end(), c3.begin(), op);
		c3.erase(i, c3.end());
		return c3;
	}

	// 
	// set_intersection
	//
	template <typename C>
		C set_intersection(const C& c1, const C& c2)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::set_intersection(c1.begin(), c1.end(), c2.begin(), 
			c2.end(), c3.begin());
		c3.erase(i, c3.end());
		return c3;
	}

	template <typename C, typename Op>
		C set_intersection(const C& c1, const C& c2, Op op)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::set_intersection(c1.begin(), c1.end(), c2.begin(), 
			c2.end(), c3.begin(), op);
		c3.erase(i, c3.end());
		return c3;
	}

	// 
	// set_difference
	//
	template <typename C>
		C set_difference(const C& c1, const C& c2)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::set_difference(c1.begin(), c1.end(), c2.begin(), c2.end(), c3.begin());
		c3.erase(i, c3.end());
		return c3;
	}

	template <typename C, typename Op>
		C set_difference(const C& c1, const C& c2, Op op)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::set_difference(c1.begin(), c1.end(), c2.begin(), c2.end(), c3.begin(), op);
		c3.erase(i, c3.end());
		return c3;
	}

	// 
	// set_symmetric_difference
	//
	template <typename C>
		C set_symmetric_difference(const C& c1, const C& c2)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::set_symmetric_difference(c1.begin(), c1.end(), c2.begin(), c2.end(), c3.begin());
		c3.erase(i, c3.end());
		return c3;
	}

	template <typename C, typename Op>
		C set_symmetric_difference(const C& c1, const C& c2, Op op)
	{ 
		C c3(c1.size() + c2.size());
		C::iterator i = std::set_symmetric_difference(c1.begin(), c1.end(), c2.begin(), c2.end(), c3.begin(), op);
		c3.erase(i, c3.end());
		return c3;
	}
}

#endif
