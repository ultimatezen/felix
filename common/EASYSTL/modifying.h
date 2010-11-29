#ifndef MODIFYING_H
#define MODIFYING_H

// Modifying.h -- versions of modifying STL algorithms that accept entire 
//                containers rather than iterators.

// Author: Brandon Corfman, 7/23/02

#include <algorithm>

namespace easystl
{
	// 
	// copy
	//
	template <typename C, typename Iter> 
		Iter copy(C& c, Iter i)
	{
		return std::copy(c.begin(), c.end(), i);
	}

	// 
	// copy_backward
	//
	template <typename C, typename Iter> 
		Iter copy_backward(C& c, Iter i)
	{
		return std::copy_backward(c.begin(), c.end(), i);
	}

	//
	// transform
	//
	template <typename C, typename Func> C transform(C c, Func func)
	{
		std::transform(c.begin(), c.end(), c.begin(), func);
		return c;
	}

	template <typename C, typename Func> C transform(C& c1, C& c2, Func func)
	{
		// this is ridiculous ... std::transform expects the 2nd range to be
		// at least as large or larger than the 1st range, or it will bomb out.
		// So here I make a safety check so that it works no matter which order
		// you use. It will call func(c1Elem, c2Elem) until it reaches the end
		// of the smallest container.
		C out(std::min(c1.size(), c2.size()));
		if (c2.size() > c1.size())
			std::transform(c1.begin(), c1.end(), c2.begin(), out.begin(), func);
		else
			std::transform(c2.begin(), c2.end(), c1.begin(), out.begin(), func);
		return out;
	}

	//
	// swap_ranges
	//
	template <typename C1, typename C2>
		typename C2::iterator swap_ranges(C1& c1, C2& c2)
	{
		return std::swap_ranges(c1.begin(), c1.end(), c2.begin());
	}

	// 
	// fill
	//
	template <typename C, typename T> 
		void fill(C& c, const T& newval)
	{
		std::fill(c.begin(), c.end(), newval);
	}

	// 
	// generate
	// 
	template <typename C, typename Func>
		C generate(C c, Func op)
	{
		std::generate(c.begin(), c.end(), op);
		return c;
	}

	// 
	// replace
	//
	template <typename T, typename E> 
		T replace(T c, const E& e1, const E& e2)
	{
		std::replace(c.begin(), c.end(), e1, e2);
		return c;
	}

	//
	// replace_if
	//
	template <typename C, typename Pred, typename T> 
		C replace_if(C c, Pred& op, const T& newval)
	{
		std::replace_if(c.begin(), c.end(), op, newval);
		return c;
	}

	// 
	// replace_copy
	//
	template <typename C, typename Iter, typename E> 
		Iter replace_copy(C& c, Iter iter, const E& e1, const E& e2)
	{
		return std::replace_copy(c.begin(), c.end(), iter, e1, e2);
	}

	// 
	// replace_copy_if
	//
	template <typename C, typename Iter, typename Op, typename E> 
		Iter replace_copy_if(C& c, Iter iter, Op op, const E& e)
	{
		return std::replace_copy(c.begin(), c.end(), iter, op, e);
	}

}

#endif
