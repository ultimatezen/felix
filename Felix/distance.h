#pragma once

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
	size_t edist(const wstring &a, const wstring &b);
	size_t subdist(const wstring &needle, const wstring &haystack);

	double edist_score(const wstring &a, const wstring &b)
	{
		size_t a_len = a.size() ;
		size_t b_len = b.size() ;

		size_t minlen, maxlen ;
		if ( a_len > b_len )
		{
			minlen = b_len ;
			maxlen = a_len ;
		}
		else
		{
			minlen = a_len ;
			maxlen = b_len ;
		}

		// Avoid divide by zero errors
		if (maxlen == 0) 
		{
			return 0.0 ;
		}

		// Make sure difference in lengths is not too great
		size_t diff = maxlen - minlen ;
		size_t maxdiff = maxlen - (size_t)((float)maxlen * minscore) ;
		if ( diff > maxdiff)
		{
			return 0.0 ;
		}

		size_t distance = this->edist(a, b) ;

		// calculate the score
		// score = (maxlen - distance) / maxlen

		return ((double)maxlen - (double)distance) / (double)maxlen ;
	}
	double subdist_score(const wstring &needle, const wstring &haystack)
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
	size_t min2(size_t a, size_t b);
	// minimum of three values
	size_t min3(size_t a, size_t b, size_t c);

};