#ifndef NUMERIC_H
#define NUMERIC_H

// Numeric.h -- versions of numeric STL algorithms that accept entire 
//              containers rather than iterators.

#include <numeric>

namespace easystl
{
	// 
	// accumulate
	//
	template <typename C, typename T>
		T accumulate(C& c, T val)
	{
		return std::accumulate(c.begin(), c.end(), val);
	}

	template <typename C, typename T, typename Op>
		T accumulate(C& c, T val, Op op)
	{
		return std::accumulate(c.begin(), c.end(), val, op);
	}

	//
	// inner_product
	//
	template <typename C, typename T>
		T inner_product(C& c1, C& c2, T val)
	{
		return std::inner_product(c1.begin(), c1.end(), c2.begin(), val);
	}

	template <typename C, typename T, typename Op1, typename Op2>
		T inner_product(C& c1, C& c2, T val, Op1 op1, Op2 op2)
	{
		return std::inner_product(c1.begin(), c1.end(), c2.begin(), val, op1, op2);
	}

	//
	// partial_sum
	//
	template <typename C>
		C partial_sum(C& c)
	{
		C c2(c.size());
		std::partial_sum(c.begin(), c.end(), c2.begin());
		return c2;
	}

	template <typename C, typename Op>
		C partial_sum(C& c, Op op)
	{
		C c2(c.size());
		std::partial_sum(c.begin(), c.end(), c2.begin(), op);
		return c2;
	}

	//
	// adjacent_difference
	//
	template <typename C>
		C adjacent_difference(C& c)
	{
		C c2(c.size());
		std::adjacent_difference(c.begin(), c.end(), c2.begin());
		return c2;
	}

	template <typename C, typename Op>
		C adjacent_difference(C& c, Op op)
	{
		C c2(c.size());
		std::adjacent_difference(c.begin(), c.end(), c2.begin(), op);
		return c2;
	}
}
#endif
