#ifndef NONMODIFYING_H
#define NONMODIFYING_H

// Nonmodifying.h -- versions of non-modifying STL algorithms that accept entire 
//                   containers rather than iterators.

// Author: Brandon Corfman, 7/23/02

#include <algorithm>

namespace easystl
{
	// 
	// count
	//
	template <typename C, typename T> ptrdiff_t count(C& cont, const T& val)
	{
		return std::count(cont.begin(), cont.end(), val);
	}

	// 
	// count_if
	//
	template <typename C, typename Op> ptrdiff_t count_if(C& cont, Op op)
	{
		return std::count_if(cont.begin(), cont.end(), op);
	}

	//
	// min_element
	//
	template <typename C> typename C::value_type min_element(C& c)
	{
		if (c.empty())
			return C::value_type();
		else
			return *std::min_element(c.begin(), c.end());
	}

	template <typename C, typename Fn> 
		typename C::value_type min_element(C& c, Fn fn)
	{
		if (c.empty())
			return C::value_type();
		else
			return *std::min_element(c.begin(), c.end(), fn);
	}

	//
	// max_element
	//
	template <typename C> typename C::value_type max_element(C& c)
	{
		if (c.empty())
			return C::value_type();
		else
			return *std::max_element(c.begin(), c.end());
	}

	template <typename C, typename Fn> typename C::value_type max_element(C& c, Fn fn)
	{
		if (c.empty())
			return C::value_type();
		else
			return *std::max_element(c.begin(), c.end(), fn);
	}

	//
	// find
	//
	template <typename C, typename T> typename C::value_type find(C& c, const T& val)
	{
		return *std::find(c.begin(), c.end(), val);
	}

	//
	// find_if
	//
	template <typename C, typename Op> typename C::value_type find_if(C& c, Op op)
	{
		return *std::find_if(c.begin(), c.end(), op);
	}

	//
	// search_n
	//
	template <typename C, typename T> 
		typename C::iterator search_n(C& c, ptrdiff_t size, const T& val)
	{
		return std::search_n(c.begin(), c.end(), size, val);
	}

    template <typename C, typename T, typename Op> 
		typename C::iterator search_n(C& c, ptrdiff_t size, const T& val, Op op)
	{
		return std::search_n(c.begin(), c.end(), size, val, op);
	}

	//
	// search
	//
	template <typename C1, typename C2> 
		typename C1::iterator search(C1& c1, C2& c2)
	{
		return std::search(c1.begin(), c1.end(), c2.begin(), c2.end());
	}

	template <typename C1, typename C2, typename Op> 
		typename C1::iterator search(C1& c1, C2& c2, Op op)
	{
		return std::search(c1.begin(), c1.end(), c2.begin(), c2.end(), op);
	}

	//
	// find_end
	//
	template <typename C1, typename C2> 
		typename C1::iterator find_end(C1& c1, C2& c2)
	{
		return std::find_end(c1.begin(), c1.end(), c2.begin(), c2.end());
	}

	template <typename C1, typename C2, typename Op> 
		typename C1::iterator find_end(C1& c1, C2& c2, Op op)
	{
		return std::find_end(c1.begin(), c1.end(), c2.begin(), c2.end(), op);
	}

	//
	// find_first_of
	//
	template <typename C1, typename C2> 
		typename C1::iterator find_first_of(C1& c1, C2& c2)
	{
		return std::find_first_of(c1.begin(), c1.end(), c2.begin(), c2.end());
	}

	template <typename C1, typename C2, typename Op> 
		typename C1::iterator find_first_of(C1& c1, C2& c2, Op op)
	{
		return std::find_first_of(c1.begin(), c1.end(), c2.begin(), c2.end(), op);
	}

	//
	// adjacent_find
	//
	template <typename C> 
		typename C::iterator adjacent_find(C& c)
	{
		return std::adjacent_find(c.begin(), c.end());
	}

	template <typename C, typename P> 
		typename C::iterator adjacent_find(C& c, const P& pred)
	{
		return std::adjacent_find(c.begin(), c.end(), pred);
	}

	//
	// equal
	//
	template <typename T1, typename T2>
		bool equal(T1& c1, T2& c2)
	{
		return std::equal(c1.begin(), c1.end(), c2.begin());
	}

	template <typename T1, typename T2, typename Pr>
		bool equal(T1& c1, T2& c2, Pr pr)
	{
		return std::equal(c1.begin(), c1.end(), c2.begin(), pr);
	}

	//
	// mismatch
	//
	template <typename C1, typename C2> 
		std::pair<typename C1::iterator, typename C2::iterator> mismatch(C1& c1, C2& c2)
	{
		return std::mismatch(c1.begin(), c1.end(), c2.begin());
	}

	template <typename C1, typename C2, typename Pr> 
		std::pair<typename C1::iterator, typename C2::iterator> 
			mismatch(C1& c1, C2& c2, Pr pr)
	{
		return std::mismatch(c1.begin(), c1.end(), c2.begin(), pr);
	}

	//
	// lexicographical_compare
	//
	template <typename C1, typename C2>
		bool lexicographical_compare(C1& c1, C2& c2)
	{
		return std::lexicographical_compare(c1.begin(), c1.end(), c2.begin(), c2.end());
	}

	template <typename C1, typename C2, typename Op>
		bool lexicographical_compare(C1& c1, C2& c2, Op op)
	{
		return std::lexicographical_compare(c1.begin(), c1.end(), c2.begin(), c2.end(), op);
	}
}

#endif
