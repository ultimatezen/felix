#ifndef MAPSETX_H
#define MAPSETX_H

// MapSetX.h -- high-level map, multimap, set & multiset functions
// Author: Brandon Corfman, 7/23/02

#include <map>
#include <set>
#include <hash_map>
#include <hash_set>
#include <fstream>
#include <string>

namespace easystl
{
	//
	// read_map series -- directly populates a map from a stream or file
	// 
	template <typename K, typename V> 
		bool read_map_from_file(std::map<K,V>& m, const std::string filename)
	{
		std::ifstream file(filename.c_str());
		if (file.fail())
			return false;
		K key; V val;
		while (file >> key >> val)
			m.insert(make_pair(key, val));
		file.close();
		return true;
	}
	
	template <typename K, typename V> 
		bool read_map_from_stream(std::map<K,V>& m, std::istream& is)
	{
		K key; V val;
		while (is >> key >> val)
			m.insert(make_pair(key, val));
		if (is.fail())
			return false;
		return true;
	}

	template <typename K, typename V>
		bool read_map_from_stream(std::map<K,V>& m, std::istream& is, long lines)
	{
		std::map<K,V> tmp(m);
		K key; V val;
		for (long i=0; i<lines; i++)
		{
			if (!(is >> key >> val))
				return false; // don't commit changes to original map
			tmp.insert(make_pair(key, val));
		}
		m = tmp;
		return true;
	}
	
	//
	// read_multimap series -- directly populates a multimap from a stream or file
	// 
	template <typename K, typename V> 
		bool read_multimap_from_file(std::multimap<K,V>& m, const std::string filename)
	{
		ifstream file(filename.c_str());
		if (file.fail())
			return false;
		K key; V val;
		while (file >> key >> val)
			m.insert(make_pair(key, val));
		file.close();
		return true;
	}

	template <typename K, typename V> 
		bool read_multimap_from_stream(std::multimap<K,V>& m, std::istream& is)
	{
		K key; V val;
		while (is >> key >> val)
			m.insert(make_pair(key, val));
		if (is.fail())
			return false;
		return true;
	}

	template <typename K, typename V>
		bool read_multimap_from_stream(std::multimap<K,V>& m, std::istream& is, long lines)
	{
		// make copy so we don't change original if there are errors
		std::multimap<K,V> tmp(m);
		K key; V val;
		for (long i=0; i<lines; i++)
		{
			if (!(is >> key >> val))
				return false; // don't commit changes to original multimap
			tmp.insert(make_pair(key, val));
		}
		m = tmp;
		return true;
	}

	// 
	// lookup -- looks up a value or values from an associative container
	//           based on a key
	// 
	template <typename K, typename V> V lookup(std::map<K,V>& m, const K& key,
		                                       V defvalue = NULL)
	{
		std::map<K,V>::const_iterator i = m.find(key);
		// if no value found for that key, return NULL
		if (i == m.end())
			return V(defvalue);
		else
			return i->second;
	}

	// specialization for std::strings to avoid an "ambigious template parameter K"
	// error in the template above
	template <typename V> V lookup(std::map<std::string,V>& m, std::string& key)
	{
		std::map<std::string,V>::const_iterator i = m.find(key);
		// if no value found for that key, return empty vector
		if (i == m.end())
			return V();
		else
			return i->second;
	}

	template <typename V> V lookup(std::set<V>& s, const V& val, V defvalue = NULL)
	{
		std::set<V>::const_iterator i = s.find(val);
		// if no value found for that key, return default value
		if (i == s.end())
			return defvalue;
		else
			return *i;
	}

	template <typename K, typename V> 
		std::vector<V> lookup(std::multimap<K,V>& m, const K& key)
	{
		std::multimap<K,V>::const_iterator beg = m.equal_range(key).first;
		std::multimap<K,V>::const_iterator end = m.equal_range(key).second;
		
		// if no value found for that key, return an empty vector
		if (beg == m.end())
			return std::vector<V>();
		else
		{
			// populate a vector with the range of elements found
			std::vector<V> v;
			std::vector<V>::size_type sz = std::distance(beg,end);
			v.resize(sz);
			std::vector<V>::size_type index = 0;
			for (std::multimap<K,V>::const_iterator i=beg; i!=end; ++i)
				v[index++] = i->second;
			return v;
		}
	}

	// specialization for std::strings to avoid an "ambigious template parameter K"
	// error in the template above
	template <typename V> 
		std::vector<V> lookup(std::multimap<std::string, V>& m, std::string& key)
	{
		std::multimap<std::string,V>::const_iterator beg = m.equal_range(key).first;
		std::multimap<std::string,V>::const_iterator end = m.equal_range(key).second;
		
		// if no value found for that key, return an empty vector
		if (beg == m.end())
			return std::vector<V>();
		else
		{
			// populate a vector with the range of elements found
			std::vector<V> v;
			std::vector<V>::size_type sz = std::distance(beg,end);
			v.resize(sz);
			std::vector<V>::size_type index = 0;
			for (std::multimap<std::string,V>::const_iterator i=beg; i!=end; ++i)
				v[index++] = i->second;
			return v;
		}
	}

	template <typename V> V lookup(std::multiset<V>& s, const V& val, 
								   V defvalue = NULL)
	{
		std::multiset<V>::const_iterator i = s.find(val);
		// if no value found for that key, return NULL
		if (i == s.end())
			return defvalue;
		else
			return *i;
	}

	template <typename K, typename V> V lookup(std::hash_map<K,V>& m, const K& key,
		                                       V defvalue = NULL)
	{
		std::hash_map<K,V>::const_iterator i = m.find(key);
		// if no value found for that key, return NULL
		if (i == m.end())
			return V(defvalue);
		else
			return i->second;
	}

	// specialization for std::strings to avoid an "ambigious template parameter K"
	// error in the template above
	template <typename V> V lookup(std::hash_map<std::string,V>& m, std::string& key)
	{
		std::hash_map<std::string,V>::const_iterator i = m.find(key);
		// if no value found for that key, return empty vector
		if (i == m.end())
			return V();
		else
			return i->second;
	}

	template <typename V> V lookup(std::hash_set<V>& s, const V& val, V defvalue = NULL)
	{
		std::hash_set<V>::const_iterator i = s.find(val);
		// if no value found for that key, return default value
		if (i == s.end())
			return defvalue;
		else
			return *i;
	}

	template <typename K, typename V> 
		std::vector<V> lookup(std::hash_multimap<K,V>& m, const K& key)
	{
		std::hash_multimap<K,V>::const_iterator beg = m.equal_range(key).first;
		std::hash_multimap<K,V>::const_iterator end = m.equal_range(key).second;
		
		// if no value found for that key, return an empty vector
		if (beg == m.end())
			return std::vector<V>();
		else
		{
			// populate a vector with the range of elements found
			std::vector<V> v;
			std::vector<V>::size_type sz = std::distance(beg,end);
			v.resize(sz);
			std::vector<V>::size_type index = 0;
			for (std::hash_multimap<K,V>::const_iterator i=beg; i!=end; ++i)
				v[index++] = i->second;
			return v;
		}
	}

	// specialization for std::strings to avoid an "ambigious template parameter K"
	// error in the template above
	template <typename V> 
		std::vector<V> lookup(std::hash_multimap<std::string, V>& m, std::string& key)
	{
		std::hash_multimap<std::string,V>::const_iterator beg = m.equal_range(key).first;
		std::hash_multimap<std::string,V>::const_iterator end = m.equal_range(key).second;
		
		// if no value found for that key, return an empty vector
		if (beg == m.end())
			return std::vector<V>();
		else
		{
			// populate a vector with the range of elements found
			std::vector<V> v;
			std::vector<V>::size_type sz = std::distance(beg,end);
			v.resize(sz);
			std::vector<V>::size_type index = 0;
			for (std::hash_multimap<std::string,V>::const_iterator i=beg; i!=end; ++i)
				v[index++] = i->second;
			return v;
		}
	}

	template <typename V> V lookup(std::hash_multiset<V>& s, const V& val, 
								   V defvalue = NULL)
	{
		std::hash_multiset<V>::const_iterator i = s.find(val);
		// if no value found for that key, return NULL
		if (i == s.end())
			return defvalue;
		else
			return *i;
	}

	//
	// make_set -- creates a set with up to 10 elements
	//
	template <typename V> std::set<V> make_set(const V& v1)
	{
		std::set<V> s;
		s.insert(v1);
		return s;
	}

	template <typename V> std::set<V> make_set(const V& v1, const V& v2)
	{
		std::set<V> s;
		s.insert(v1);
		s.insert(v2);
		return s;
	}

	template <typename V> std::set<V> make_set(const V& v1, const V& v2, const V& v3)
	{
		std::set<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		return s;
	}

	template <typename V> std::set<V> make_set(const V& v1, const V& v2, const V& v3,
											   const V& v4)
	{
		std::set<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		return s;
	}

	template <typename V> std::set<V> make_set(const V& v1, const V& v2, const V& v3,
											   const V& v4, const V& v5)
	{
		std::set<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		return s;
	}

	template <typename V> std::set<V> make_set(const V& v1, const V& v2, const V& v3,
											   const V& v4, const V& v5, const V& v6)
	{
		std::set<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		return s;
	}

	template <typename V> std::set<V> make_set(const V& v1, const V& v2, const V& v3,
											   const V& v4, const V& v5, const V& v6,
											   const V& v7)
	{
		std::set<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		s.insert(v7);
		return s;
	}

	template <typename V> std::set<V> make_set(const V& v1, const V& v2, const V& v3,
											   const V& v4, const V& v5, const V& v6,
											   const V& v7, const V& v8)
	{
		std::set<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		s.insert(v7);
		s.insert(v8);
		return s;
	}

	template <typename V> std::set<V> make_set(const V& v1, const V& v2, const V& v3,
											   const V& v4, const V& v5, const V& v6,
											   const V& v7, const V& v8, const V& v9)
	{
		std::set<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		s.insert(v7);
		s.insert(v8);
		s.insert(v9);
		return s;
	}

	template <typename V> std::set<V> make_set(const V& v1, const V& v2, const V& v3,
											   const V& v4, const V& v5, const V& v6,
											   const V& v7, const V& v8, const V& v9,
											   const V& v10)
	{
		std::set<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		s.insert(v7);
		s.insert(v8);
		s.insert(v9);
		s.insert(v10);
		return s;
	}

	template <typename V> std::set<V> make_set(std::vector<V> v)
	{
		std::set<V> s;
		for (std::vector<V>::size_type i=0; i<v.size(); i++)
			s.insert(v[i]);
		return s;
	}

	//
	// make_multiset -- creates a multiset with up to 10 elements
	//
	template <typename V> 
		std::multiset<V> make_multiset(const V& v1)
	{
		std::multiset<V> s;
		s.insert(v1);
		return s;
	}

	template <typename V> 
		std::multiset<V> make_multiset(const V& v1, const V& v2)
	{
		std::multiset<V> s;
		s.insert(v1);
		s.insert(v2);
		return s;
	}

	template <typename V> 
		std::multiset<V> make_multiset(const V& v1, const V& v2, const V& v3)
	{
		std::multiset<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		return s;
	}

	template <typename V> 
		std::multiset<V> make_multiset(const V& v1, const V& v2, const V& v3,
									   const V& v4)
	{
		std::multiset<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		return s;
	}

	template <typename V> 
		std::multiset<V> make_multiset(const V& v1, const V& v2, const V& v3,
									   const V& v4, const V& v5)
	{
		std::multiset<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		return s;
	}

	template <typename V> 
		std::multiset<V> make_multiset(const V& v1, const V& v2, const V& v3,
									   const V& v4, const V& v5, const V& v6)
	{
		std::multiset<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		return s;
	}

	template <typename V> 
		std::multiset<V> make_multiset(const V& v1, const V& v2, const V& v3,
									   const V& v4, const V& v5, const V& v6,
									   const V& v7)
	{
		std::multiset<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		s.insert(v7);
		return s;
	}

	template <typename V> 
		std::multiset<V> make_multiset(const V& v1, const V& v2, const V& v3,
									   const V& v4, const V& v5, const V& v6,
									   const V& v7, const V& v8)
	{
		std::multiset<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		s.insert(v7);
		s.insert(v8);
		return s;
	}

	template <typename V> 
		std::multiset<V> make_multiset(const V& v1, const V& v2, const V& v3,
									   const V& v4, const V& v5, const V& v6,
									   const V& v7, const V& v8, const V& v9)
	{
		std::multiset<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		s.insert(v7);
		s.insert(v8);
		s.insert(v9);
		return s;
	}

	template <typename V> 
		std::multiset<V> make_multiset(const V& v1, const V& v2, const V& v3,
									   const V& v4, const V& v5, const V& v6,
									   const V& v7, const V& v8, const V& v9,
									   const V& v10)
	{
		std::multiset<V> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v3);
		s.insert(v4);
		s.insert(v5);
		s.insert(v6);
		s.insert(v7);
		s.insert(v8);
		s.insert(v9);
		s.insert(v10);
		return s;
	}
}

#endif
