#ifndef FOREACH_H
#define FOREACH_H

// ForEach.h -- for_each that accepts a container rather than iterators.
// Author: Brandon Corfman, 7/23/02

namespace easystl
{
	// 
	// for_each
	// 
	template <typename C, typename Op> Op for_each(C& cont, Op op)
	{
		return std::for_each(cont.begin(), cont.end(), op);
	}
}

#endif
