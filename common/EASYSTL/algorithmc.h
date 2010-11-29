#ifndef ALGORITHMC_H
#define ALGORITHMC_H

// algorithmc.h -- miscellaneous custom algorithms not in the original STL or 
//                 beefed-up versions of existing algorithms that take 
//                 multiple parameters.

// Author: Brandon Corfman, 7/23/02

#include <string>
#include <algorithm>

namespace easystl
{
	// 
	// insert_back & insert_front -- inserts up to 10 elements into an existing container
	//
	template <typename C, typename E> C insert_back(C c, const E& e1)
	{
		c.push_back(e1);
		return c;
	}

	template <typename C, typename E> C insert_back(C c, const E& e1, const E& e2)
	{
		c.push_back(e1);
		c.push_back(e2);
		return c;
	}

	template <typename C, typename E> C insert_back(C c, const E& e1, const E& e2,
													const E& e3)
	{
		c.push_back(e1);
		c.push_back(e2);
		c.push_back(e3);
		return c;
	}

	template <typename C, typename E> C insert_back(C c, const E& e1, const E& e2,
													const E& e3, const E& e4)
	{
		c.push_back(e1);
		c.push_back(e2);
		c.push_back(e3);
		c.push_back(e4);
		return c;
	}

	template <typename C, typename E> C insert_back(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5)
	{
		c.push_back(e1);
		c.push_back(e2);
		c.push_back(e3);
		c.push_back(e4);
		c.push_back(e5);
		return c;
	}

	template <typename C, typename E> C insert_back(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6)
	{
		c.push_back(e1);
		c.push_back(e2);
		c.push_back(e3);
		c.push_back(e4);
		c.push_back(e5);
		c.push_back(e6);
		return c;
	}

	template <typename C, typename E> C insert_back(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7)
	{
		c.push_back(e1);
		c.push_back(e2);
		c.push_back(e3);
		c.push_back(e4);
		c.push_back(e5);
		c.push_back(e6);
		c.push_back(e7);
		return c;
	}

	template <typename C, typename E> C insert_back(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7, const E& e8)
	{
		c.push_back(e1);
		c.push_back(e2);
		c.push_back(e3);
		c.push_back(e4);
		c.push_back(e5);
		c.push_back(e6);
		c.push_back(e7);
		c.push_back(e8);
		return c;
	}

	template <typename C, typename E> C insert_back(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7, const E& e8,
													const E& e9)
	{
		c.push_back(e1);
		c.push_back(e2);
		c.push_back(e3);
		c.push_back(e4);
		c.push_back(e5);
		c.push_back(e6);
		c.push_back(e7);
		c.push_back(e8);
		c.push_back(e9);
		return c;
	}

	template <typename C, typename E> C insert_back(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7, const E& e8,
													const E& e9, const E& e10)
	{
		c.push_back(e1);
		c.push_back(e2);
		c.push_back(e3);
		c.push_back(e4);
		c.push_back(e5);
		c.push_back(e6);
		c.push_back(e7);
		c.push_back(e8);
		c.push_back(e9);
		c.push_back(e10);
		return c;
	}

	template <typename C> C insert_back(C c1, C& c2)
	{
		if (c2.empty())
			return c1;
		
		C::iterator end = c2.end();
		for (C::iterator i=c2.begin(); i != end; ++i)
			c1.push_back(*i);
		return c1;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1)
	{
		c.push_front(e1);
		return c;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1, const E& e2)
	{
		c.push_front(e1);
		c.push_front(e2);
		return c;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1, const E& e2,
													const E& e3)
	{
		c.push_front(e1);
		c.push_front(e2);
		c.push_front(e3);
		return c;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1, const E& e2,
													const E& e3, const E& e4)
	{
		c.push_front(e1);
		c.push_front(e2);
		c.push_front(e3);
		c.push_front(e4);
		return c;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5)
	{
		c.push_front(e1);
		c.push_front(e2);
		c.push_front(e3);
		c.push_front(e4);
		c.push_front(e5);
		return c;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6)
	{
		c.push_front(e1);
		c.push_front(e2);
		c.push_front(e3);
		c.push_front(e4);
		c.push_front(e5);
		c.push_front(e6);
		return c;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7)
	{
		c.push_front(e1);
		c.push_front(e2);
		c.push_front(e3);
		c.push_front(e4);
		c.push_front(e5);
		c.push_front(e6);
		c.push_front(e7);
		return c;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7, const E& e8)
	{
		c.push_front(e1);
		c.push_front(e2);
		c.push_front(e3);
		c.push_front(e4);
		c.push_front(e5);
		c.push_front(e6);
		c.push_front(e7);
		c.push_front(e8);
		return c;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7, const E& e8,
													const E& e9)
	{
		c.push_front(e1);
		c.push_front(e2);
		c.push_front(e3);
		c.push_front(e4);
		c.push_front(e5);
		c.push_front(e6);
		c.push_front(e7);
		c.push_front(e8);
		c.push_front(e9);
		return c;
	}

	template <typename C, typename E> C insert_front(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7, const E& e8,
													const E& e9, const E& e10)
	{
		c.push_front(e1);
		c.push_front(e2);
		c.push_front(e3);
		c.push_front(e4);
		c.push_front(e5);
		c.push_front(e6);
		c.push_front(e7);
		c.push_front(e8);
		c.push_front(e9);
		c.push_front(e10);
		return c;
	}

	template <typename C> C insert_front(C c1, C& c2)
	{
		if (c2.empty())
			return c1;
		
		C::iterator end = c2.end();
		for (C::iterator i=c2.begin(); i != end; ++i)
			c1.push_front(*i);
		return c1;
	}

	// 
	// insert_elem -- inserts up to 10 elements into a set/multiset
	//
	template <typename C, typename E> C insert_elem(C c, const E& e1)
	{
		c.insert(e1);
		return c;
	}

	template <typename C, typename E> C insert_elem(C c, const E& e1, const E& e2)
	{
		c.insert(e1);
		c.insert(e2);
		return c;
	}

	template <typename C, typename E> C insert_elem(C c, const E& e1, const E& e2,
													const E& e3)
	{
		c.insert(e1);
		c.insert(e2);
		c.insert(e3);
		return c;
	}

	template <typename C, typename E> C insert_elem(C c, const E& e1, const E& e2,
													const E& e3, const E& e4)
	{
		c.insert(e1);
		c.insert(e2);
		c.insert(e3);
		c.insert(e4);
		return c;
	}

	template <typename C, typename E> C insert_elem(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5)
	{
		c.insert(e1);
		c.insert(e2);
		c.insert(e3);
		c.insert(e4);
		c.insert(e5);
		return c;
	}

	template <typename C, typename E> C insert_elem(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6)
	{
		c.insert(e1);
		c.insert(e2);
		c.insert(e3);
		c.insert(e4);
		c.insert(e5);
		c.insert(e6);
		return c;
	}

	template <typename C, typename E> C insert_elem(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7)
	{
		c.insert(e1);
		c.insert(e2);
		c.insert(e3);
		c.insert(e4);
		c.insert(e5);
		c.insert(e6);
		c.insert(e7);
		return c;
	}

	template <typename C, typename E> C insert_elem(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7, const E& e8)
	{
		c.insert(e1);
		c.insert(e2);
		c.insert(e3);
		c.insert(e4);
		c.insert(e5);
		c.insert(e6);
		c.insert(e7);
		c.insert(e8);
		return c;
	}

	template <typename C, typename E> C insert_elem(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7, const E& e8,
													const E& e9)
	{
		c.insert(e1);
		c.insert(e2);
		c.insert(e3);
		c.insert(e4);
		c.insert(e5);
		c.insert(e6);
		c.insert(e7);
		c.insert(e8);
		c.insert(e9);
		return c;
	}

	template <typename C, typename E> C insert_elem(C c, const E& e1, const E& e2,
													const E& e3, const E& e4,
													const E& e5, const E& e6,
													const E& e7, const E& e8,
													const E& e9, const E& e10)
	{
		c.insert(e1);
		c.insert(e2);
		c.insert(e3);
		c.insert(e4);
		c.insert(e5);
		c.insert(e6);
		c.insert(e7);
		c.insert(e8);
		c.insert(e9);
		c.insert(e10);
		return c;
	}

	template <typename C> C insert_elem(C c1, C& c2)
	{
		if (c2.empty())
			return c1;
		
		C::iterator end = c2.end();
		for (C::iterator i=c2.begin(); i != end; ++i)
			c1.insert(*i);
		return c1;
	}

	// 
	// min -- same as STL version, except it can take from 3-10 parameters
	//
	template <typename T> T min(const T& a, const T& b, const T& c)
	{
		return std::min(a, 
			   std::min(b,c));
	}

	template <typename T> T min(const T& a, const T& b, const T& c, const T& d)
	{
		return std::min(a, 
			   std::min(b,
			   std::min(c,d)));
	}

	template <typename T> T min(const T& a, const T& b, const T& c, const T& d,
								const T& e)
	{
		return std::min(a, 
			   std::min(b,
			   std::min(c,
			   std::min(d,e))));
	}

	template <typename T> T min(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f)
	{
		return std::min(a, 
			   std::min(b,
			   std::min(c,
			   std::min(d,
			   std::min(e,f)))));
	}

	template <typename T> T min(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f, const T& g)
	{
		return std::min(a, 
			   std::min(b,
			   std::min(c,
			   std::min(d,
			   std::min(e,
			   std::min(f,g))))));
	}

	template <typename T> T min(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f, const T& g, const T& h)
	{
		return std::min(a, 
			   std::min(b,
			   std::min(c,
			   std::min(d,
			   std::min(e,
			   std::min(f,
			   std::min(g,h)))))));
	}

	template <typename T> T min(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f, const T& g, const T& h,
								const T& i)
	{
		return std::min(a, 
			   std::min(b,
			   std::min(c,
			   std::min(d,
			   std::min(e,
			   std::min(f,
			   std::min(g,
			   std::min(h,i))))))));
	}

	template <typename T> T min(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f, const T& g, const T& h,
								const T& i, const T& j)
	{
		return std::min(a, 
			   std::min(b,
			   std::min(c,
			   std::min(d,
			   std::min(e,
			   std::min(f,
			   std::min(g,
			   std::min(h,
			   std::min(i,j)))))))));
	}

	// 
	// max -- same as STL version, except it can take from 3-10 parameters
	//
	template <typename T> T max(const T& a, const T& b, const T& c)
	{
		return std::max(a, 
			   std::max(b,c));
	}

	template <typename T> T max(const T& a, const T& b, const T& c, const T& d)
	{
		return std::max(a, 
			   std::max(b,
			   std::max(c,d)));
	}

	template <typename T> T max(const T& a, const T& b, const T& c, const T& d,
								const T& e)
	{
		return std::max(a, 
			   std::max(b,
			   std::max(c,
			   std::max(d,e))));
	}

	template <typename T> T max(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f)
	{
		return std::max(a, 
			   std::max(b,
			   std::max(c,
			   std::max(d,
			   std::max(e,f)))));
	}

	template <typename T> T max(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f, const T& g)
	{
		return std::max(a, 
			   std::max(b,
			   std::max(c,
			   std::max(d,
			   std::max(e,
			   std::max(f,g))))));
	}

	template <typename T> T max(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f, const T& g, const T& h)
	{
		return std::max(a, 
			   std::max(b,
			   std::max(c,
			   std::max(d,
			   std::max(e,
			   std::max(f,
			   std::max(g,h)))))));
	}

	template <typename T> T max(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f, const T& g, const T& h,
								const T& i)
	{
		return std::max(a, 
			   std::max(b,
			   std::max(c,
			   std::max(d,
			   std::max(e,
			   std::max(f,
			   std::max(g,
			   std::max(h,i))))))));
	}

	template <typename T> T max(const T& a, const T& b, const T& c, const T& d,
								const T& e, const T& f, const T& g, const T& h,
								const T& i, const T& j)
	{
		return std::max(a, 
			   std::max(b,
			   std::max(c,
			   std::max(d,
			   std::max(e,
			   std::max(f,
			   std::max(g,
			   std::max(h,
			   std::max(i,j)))))))));
	}

	// 
	// remove_dup -- sorts and removes duplicates in a container
	//		         
	template <typename C> C remove_dup(C c)
	{
		std::sort(c.begin(), c.end());
		typename C::iterator p = std::unique(c.begin(), c.end());
		c.erase(p, c.end());
		return c;
	}

	//
	// remove_elem -- encapsulates the erase/remove idiom 
	//
	template <typename C, typename E> C remove_elem(C c, const E& e)
	{
		c.erase(std::remove(c.begin(), c.end(), e), c.end());
		return c;
	}

	//
	// remove_elem_if -- removes elements from a container if they 
	//                   satisfy a supplied predicate. 
	//
	template <typename C, typename Op> C remove_elem_if(C c, Op op)
	{	
		c.erase(std::remove_if(c.begin(), c.end(), op), c.end());
		return c;
	}

	//
	// remove_elem_if_not -- removes elements from a container if they do not
	//                       satisfy a supplied predicate. Same as Python's 
	//                       "filter" keyword.
	//
	template <typename C, typename Op> C remove_elem_if_not(C c, Op op)
	{	
		C rem(c);
		typename C::iterator end = std::remove_if(rem.begin(), rem.end(), op);
		for (typename C::iterator i = rem.begin(); i != end; ++i)
			c = remove_elem(c, *i);
		return c;
	}

	// 
	// n_highest & n_lowest -- returns the partitions created by nth_element
	//
	template <typename C> C nth_highest(C c, typename const C::size_type n)
	{
		std::nth_element(c.begin(), c.end()-n, c.end());
		C cpy(c.end()-n, c.end());
		return cpy;
	}

	template <typename C> C nth_lowest(C c, typename const C::size_type n)
	{
		std::nth_element(c.begin(), c.begin()+n, c.end());
		C cpy(c.begin(), c.begin()+n);
		return cpy;
	}

}

#endif
