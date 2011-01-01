#pragma once

#include <string>

class Distance
{
	size_t *row1 ;
	size_t *row2 ;
	size_t row_size ;
	double minscore ;
public:
	Distance() :
	  row1(NULL),
	  row2(NULL),
	  row_size(0),
	  minscore(0.5f)
	{

	}
	~Distance()
	{
		if (row1) free(row1) ;
		if (row2) free(row2) ;
	}
	void set_minscore(double score)
	{
		minscore = score ;
	}
	size_t edist(const std::wstring &a, const std::wstring &b);
	size_t subdist(const std::wstring &needle, const std::wstring &haystack);

	double edist_score(const std::wstring &a, const std::wstring &b);
	double subdist_score(const std::wstring &needle, const std::wstring &haystack)
	{
		size_t needle_len = needle.size() ; 

		if (needle_len == 0) // avoid divide by zero errors
		{
			return 0.0 ;
		}

		size_t distance = this->subdist(needle, haystack) ;

		return ((double)needle_len - (double)distance) / (double)needle_len ;
	}

	void ensure_size(size_t min_row_size) ;
	// minimum of two values
	size_t min2(size_t a, size_t b) const ;
	// minimum of three values
	size_t min3(size_t a, size_t b, size_t c) const ;

};