#ifndef STRINGX_H
#define STRINGX_H

// StringX.h -- high-level string handling functions
// Author: Brandon Corfman, 8/23/02

#include <sstream>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <string>
#include <vector>

namespace easystl
{
	// split -- breaks a std::string on any of the characters in the supplied
	//          separators and returns a std::vector of the pieces. The
	//          default value of the separators is whitespace. 
	namespace aux
	{
		inline void split2(std::string s, std::vector<std::string>& v, 
			const std::string delim)
		{
			if (s.empty())
				return;
			
			std::string::size_type pos = s.find_first_of(delim);
			if (pos == std::string::npos)
			{
				v.push_back(s);
				return;
			}
			else
			{
				v.push_back(s.substr(0, pos));		
				split2(s.substr(pos+1, s.size()-1), v, delim);
			}
		}
	}

	inline std::vector<std::string> split(std::string s, std::string delim = " ")
	{
		std::vector<std::string> v;
		aux::split2(s, v, delim);
		return v;
	}

	//
	// splitlines -- Splits a string on line breaks (carriage return and/or
	//               newline) and inserts the substrings into a std::vector. 
	//				 If you set keepends equal to true, splitlines retains the
	//               terminating character.
	namespace aux
	{
		inline void splitlines2(std::string s, std::vector<std::string>& v, bool keepends)
		{
			if (s.empty() || s == "\n" || s == "\r\n")
				return;
			
			const std::string::size_type maxpos = 
				std::numeric_limits<std::string::size_type>::max();
			
			std::string::size_type cr = s.find("\r");
			if (cr == std::string::npos)
				cr = maxpos;
			std::string::size_type nl = s.find("\n");
			if (nl == std::string::npos)
				nl = maxpos;

			std::string::size_type pos = std::min(nl, cr);
			if (pos == maxpos)
			{
				v.push_back(s);
				return;
			}
			else
			{
				if (cr < nl && nl-cr == 1)
				{
					pos++;
					if (keepends)
						v.push_back(s.substr(0, nl+1));
					else
						v.push_back(s.substr(0, cr));
				}
				else
				{
					if (keepends)
						v.push_back(s.substr(0, pos+1));
					else
						v.push_back(s.substr(0, pos));
				}
			}
			splitlines2(s.substr(pos+1, s.size()-1), v, keepends);
		}
	}

	inline std::vector<std::string> splitlines(std::string s, bool keepends = false)
	{
		std::vector<std::string> v;
		aux::splitlines2(s, v, keepends);
		return v;
	}

	//
	// join -- returns a string consisting of all the strings in the supplied
	//         std::vector, using the std::string delim as a delimiter.
	//
	inline std::string join(std::vector<std::string> v, std::string delim = " ")
	{
		std::vector<std::string>::iterator i = v.begin();
		std::string s = *i;
		while (++i != v.end())
			s += delim + *i;
		return s;
	}

	//
	// ltrim -- returns a string with leading whitespace removed
	// 
	inline std::string ltrim(std::string s)
	{
		std::string::size_type pos = s.find_first_not_of(" "); 
		if (pos != std::string::npos)
			s.erase(0, pos);
		return s;
	}

	//
	// rtrim -- returns a string with trailing whitespace removed
	// 
	inline std::string rtrim(std::string s)
	{
		std::string::size_type pos = s.find_last_not_of(" "); 
		if (pos != std::string::npos)
			s.erase(pos+1, s.size()-pos);
		return s;
	}

	//
	// trim -- returns a string with leading and trailing whitespace removed
	// 
	inline std::string trim(std::string s)
	{
		return ltrim(rtrim(s));
	}

	// to_string -- takes an stream-compatible type and converts it to a string.
	template <typename T> std::string to_string(T t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	// to_string usage for floats
	inline std::string to_string(float elem, int prec)
	{
		std::stringstream ss;
		std::streamsize p = ss.precision();
		std::ios::fmtflags f = ss.flags();
		ss << std::fixed << std::setprecision(prec) << elem;
		ss.precision(p);
		ss.flags(f);
		return ss.str();
	}

	// to_string usage for doubles
	inline std::string to_string(double elem, int prec)
	{
		std::stringstream ss;
		std::streamsize p = ss.precision();
		std::ios::fmtflags f = ss.flags();
		ss << std::fixed << std::setprecision(prec) << elem << std::setprecision(p);
		ss.precision(p);
		ss.flags(f);
		return ss.str();
	}

	// to_long -- converts a std::string to a long.
	inline long to_long(std::string s)
	{
		return atol(s.c_str());
	}

	// to_int -- converts a std::string to an int.
	inline int to_int(std::string s)
	{
		return atoi(s.c_str());
	}

	// toDouble -- converts a std::string to a double.
	inline double to_double(std::string s)
	{
		return atof(s.c_str());
	}

	// to_upper -- converts a std::string to uppercase.
	inline std::string to_upper(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	// to_lower -- converts a std::string to lowercase.
	inline std::string to_lower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}

	// to_title -- converts a std::string to titlecase, e.g.
	//            The First Letter Of Every Word Is Capitalized.
	inline std::string to_title(std::string s)
	{
		bool capNextLetter = true;
		for (std::string::iterator i = s.begin(); i!= s.end(); i++)
		{
			if (isspace(*i))
				capNextLetter = true;
			else if (isalpha(*i) && capNextLetter)
			{
				*i = toupper(*i);
				capNextLetter = false;
			}
			else if (isalpha(*i) && !capNextLetter)
				*i = tolower(*i);
		}
		return s;
	}			

	// 
	// is_number -- returns true if a string consists of digits only; false otherwise.
	//
	inline bool is_number(std::string s)
	{
		return !(s == "" ||
			     std::find_if(s.begin(), s.end(), iscntrl) != s.end() ||
			     std::find_if(s.begin(), s.end(), ispunct) != s.end() ||
			     std::find_if(s.begin(), s.end(), isspace) != s.end() ||
				 std::find_if(s.begin(), s.end(), isalpha) != s.end());
	}

	// 
	// is_alnum -- returns true if a string consists of alphanumeric characters only;
	//            false otherwise.
	// 
	inline bool is_alnum(std::string s)
	{
		return !(s == "" ||
			     std::find_if(s.begin(), s.end(), iscntrl) != s.end() ||
			     std::find_if(s.begin(), s.end(), ispunct) != s.end() ||
			     std::find_if(s.begin(), s.end(), isspace) != s.end());
	}

	// 
	// is_alpha -- returns true if a string consists of alphabetic characters only;
	//             false otherwise.
	// 
	inline bool is_alpha(std::string s)
	{
		return !(s == "" ||
			     std::find_if(s.begin(), s.end(), iscntrl) != s.end() ||
			     std::find_if(s.begin(), s.end(), ispunct) != s.end() ||
			     std::find_if(s.begin(), s.end(), isdigit) != s.end() ||
				 std::find_if(s.begin(), s.end(), isspace) != s.end());
	}

	//
	// is_lower -- returns true if a string consists of lowercase characters only;
	//             false otherwise.
	//
	inline bool is_lower(std::string s)
	{
		return !(s == "" ||
			     std::find_if(s.begin(), s.end(), isupper) != s.end());
	}

	//
	// is_upper -- returns true if a string consists of uppercase characters only;
	//             false otherwise.
	//
	inline bool is_upper(std::string s)
	{
		return !(s == "" ||
			     std::find_if(s.begin(), s.end(), islower) != s.end());
	}
	
}

#endif
