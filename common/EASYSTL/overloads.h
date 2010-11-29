#ifndef OVERLOADS_H
#define OVERLOADS_H

// Overloads.h -- operator overloads to conveniently print out the contents of 
//                STL containers using cout (or any other streams). Each element
//                is written to the stream space-separated.

// Author: Brandon Corfman, 7/23/02

#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>

namespace easystl
{
	template <typename T> std::ostream& operator << (std::ostream& os, 
		const std::list<T>& lst)
	{
		std::copy(lst.begin(), lst.end(), std::ostream_iterator<T>(os, " "));
		return os;
	}

	template <typename T> std::ostream& operator << (std::ostream& os, 
		const std::vector<T>& v)
	{
		std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
		return os;
	}

	template <typename T> std::ostream& operator << (std::ostream& os, 
		const std::deque<T>& d)
	{
		std::copy(d.begin(), d.end(), std::ostream_iterator<T>(os, " "));
		return os;
	}

	template <typename T> std::ostream& operator << (std::ostream& os, 
		const std::set<T>& s)
	{
		std::copy(s.begin(), s.end(), std::ostream_iterator<T>(os, " "));
		return os;
	}

	template <typename T> std::ostream& operator << (std::ostream& os,
		const std::vector<std::vector<T> >& vv)
	{
		typedef std::vector<std::vector<T> >::size_type VVSize;
		
		for (VVSize i=0; i<vv.size()-1; i++)
		{
			std::copy(vv[i].begin(), vv[i].end(), std::ostream_iterator<T>(os, " "));				
			cout << "\n";
		}
		std::copy(vv[i].begin(), vv[i].end(), std::ostream_iterator<T>(os, " "));				
		return os;
	}

	template <typename T> std::ostream& operator << (std::ostream& os,
		const std::deque<std::deque<T> >& dd)
	{
		typedef std::deque<std::deque<T> >::size_type DDSize;
		
		for (DDSize i=0; i<dd.size()-1; i++)
		{
			std::copy(dd[i].begin(), dd[i].end(), std::ostream_iterator<T>(os, " "));			
			cout << "\n";
		}
		std::copy(dd[i].begin(), dd[i].end(), std::ostream_iterator<T>(os, " "));				
		return os;
	}

	template <typename T> std::ostream& operator << (std::ostream& os,
		const std::list<std::list<T> >& ll)
	{
		typedef std::list<std::list<T> > LL;
		typedef LL::const_iterator LLIter;
	
		LLIter end = ll.end();
		LLIter test;
		for (LLIter i=ll.begin(); i!=end; ++i)
		{
			std::copy(i->begin(), i->end(), std::ostream_iterator<T>(os, " "));	
			test = i;
			std::advance(test, 1);
			if (test != end)
				cout << "\n";
		}
		return os;
	}
}

#endif
