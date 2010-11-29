#ifndef LISTX_H
#define LISTX_H

// ListX.h -- high-level list functions
// Author: Brandon Corfman, 7/23/02

#include <limits>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <math.h>

namespace easystl
{
	//
	// make_list -- creates a list containing the passed parameters
	//
	template <typename T> std::list<T> make_list(const T& e1)
	{
		std::list<T> lst;
		lst.push_back(e1);
		return lst;
	}

	template <typename T> std::list<T> make_list(const T& e1, const T& e2)
	{
		std::list<T> lst;
		lst.push_back(e1);
		lst.push_back(e2);
		return lst;
	}

	template <typename T> std::list<T> make_list(const T& e1, const T& e2, const T& e3)
	{
		std::list<T> lst;
		lst.push_back(e1);
		lst.push_back(e2);
		lst.push_back(e3);
		return lst;
	}

	template <typename T> std::list<T> make_list(const T& e1, const T& e2, const T& e3, const T& e4)
	{
		std::list<T> lst;
		lst.push_back(e1);
		lst.push_back(e2);
		lst.push_back(e3);
		lst.push_back(e4);
		return lst;
	}

	template <typename T> std::list<T> make_list(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5)
	{
		std::list<T> lst;
		lst.push_back(e1);
		lst.push_back(e2);
		lst.push_back(e3);
		lst.push_back(e4);
		lst.push_back(e5);
		return lst;
	}

	template <typename T> std::list<T> make_list(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
											const T& e6)
	{
		std::list<T> lst;
		lst.push_back(e1);
		lst.push_back(e2);
		lst.push_back(e3);
		lst.push_back(e4);
		lst.push_back(e5);
		lst.push_back(e6);

		return lst;
	}

	template <typename T> std::list<T> make_list(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
											const T& e6, const T& e7)
	{
		std::list<T> lst;
		lst.push_back(e1);
		lst.push_back(e2);
		lst.push_back(e3);
		lst.push_back(e4);
		lst.push_back(e5);
		lst.push_back(e6);
		lst.push_back(e7);
		return lst;
	}

	template <typename T> std::list<T> make_list(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
											const T& e6, const T& e7, const T& e8)
	{
		std::list<T> lst;
		lst.push_back(e1);
		lst.push_back(e2);
		lst.push_back(e3);
		lst.push_back(e4);
		lst.push_back(e5);
		lst.push_back(e6);
		lst.push_back(e7);
		lst.push_back(e8);
		return lst;
	}

	template <typename T> std::list<T> make_list(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
											const T& e6, const T& e7, const T& e8, const T& e9)
	{
		std::list<T> lst;
		lst.push_back(e1);
		lst.push_back(e2);
		lst.push_back(e3);
		lst.push_back(e4);
		lst.push_back(e5);
		lst.push_back(e6);
		lst.push_back(e7);
		lst.push_back(e8);
		lst.push_back(e9);
		return lst;
	}

	template <typename T> std::list<T> make_list(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
											const T& e6, const T& e7, const T& e8, const T& e9, const T& e10)
	{
		std::list<T> lst;
		lst.push_back(e1);
		lst.push_back(e2);
		lst.push_back(e3);
		lst.push_back(e4);
		lst.push_back(e5);
		lst.push_back(e6);
		lst.push_back(e7);
		lst.push_back(e8);
		lst.push_back(e9);
		lst.push_back(e10);
		return lst;
	}

	//
	// car
	//
	template <typename T> T car(const std::list<T>& lst)
	{
		if (lst.size() > 0)
		{
			T val = lst.front();
			return val;
		}
		else
			return T();
	}

	//
	// cdr
	//
	template <typename T> std::list<T> cdr(std::list<T>& lst)
	{
		if (lst.size() > 1)
		{
			std::list<T>::iterator beg = lst.begin();
			std::advance(beg, 1);
			std::list<T> cpy(beg, lst.end());
			return cpy;
		}
		else
			return std::list<T>();
	}

	//
	// cons
	//
	template <typename T> std::list<T> cons(std::list<T> lst1, const std::list<T>& lst2)
	{
		lst1.insert(lst1.end(), lst2.begin(), lst2.end());
		return lst1;
	}

	template <typename T> std::list<T> cons(const T& atom, std::list<T> lst)
	{
		lst.insert(lst.begin(), atom);
		return lst;
	}

	template <typename T> std::list<T> cons(std::list<T> lst, const T& atom)
	{
		lst.insert(lst.end(), atom);
		return lst;
	}
}

#endif
