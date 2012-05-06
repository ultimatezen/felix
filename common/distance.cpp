#include "StdAfx.h"
#include "distance.h"
#include "boost/tuple/tuple.hpp"

#ifdef min
#undef min
#endif

size_t min3( size_t a, size_t b, size_t c ) 
{
	return std::min(std::min(a, b), c) ;
}
double compute_score( const size_t high_len, size_t total_cost ) 
{
	// protect from divide by 0 error
	if (! high_len)
	{
		return 0.0 ;
	}
	const size_t matching_elements = max(high_len, total_cost) - total_cost ;
	return static_cast<double>(matching_elements) / static_cast<double>(high_len) ;
}

size_t Distance::edist(const std::wstring &a, const std::wstring &b)
{
	const wchar_t* a_str = a.c_str() ;
	size_t a_len = a.size() ;
	const wchar_t *b_str = b.c_str() ;
	size_t b_len = b.size() ;

	// swap if b is smaller
	if (b_len < a_len)
	{
		std::swap(a_str, b_str) ;
		std::swap(a_len, b_len) ;
	}

	// skip equal start sequence, if any
	while (*a_str==*b_str)
	{
		if (!*a_str)
		{
			break;
		}
		a_str++;
		b_str++;
		a_len--;
		b_len--;
	}

	// skip equal end sequence, if any
	while (a_len && b_len && a_str[a_len-1] == b_str[b_len-1])
	{
		a_len--;
		b_len--;
	}

	// base cases
	if (!a_len || !b_len)
	{
		// Since one of them is 0, just add them to get the nonzero value
		return a_len + b_len;
	}

	// check a_len == 1
	if (a_len == 1)
	{
		return len_1_edist(*a_str, b_len, b_str);
	}

	a_len++;
	b_len++;

	// Halfway point used for trimming triangle below
	const size_t half = a_len >> 1;

	// allocate memory for row1 if necessary
	ensure_size(b_len) ;

	size_t *end = row1 + b_len - 1;

	// initialize the first row1
	for (size_t col_b = 0; col_b < b_len - half; col_b++)
	{
		row1[col_b] = col_b;
	}

	// set up loop variables
	row1[0] = a_len - half - 1;
	size_t diag, x;
	size_t *p;
	const wchar_t *above;

	for (size_t col = 1; col < a_len; col++)
	{
		const wchar_t c = a_str[col - 1];
		// skip the upper triangle
		if (col >= a_len - half)
		{
			const size_t offset = col - (a_len - half);

			above = b_str + offset;
			p = row1 + offset;
			const size_t cell = *(p++) + (c != *(above++));
			diag = x = *p + 1;

			if (x > cell)
			{
				x = cell;
			}
			*(p++) = x;
		}
		else
		{
			p = row1 + 1;
			above = b_str;
			diag = x = col;
		}

		// skip the lower triangle
		if (col <= half + 1)
		{
			end = row1 + b_len + col - half - 2;
		}

		// Do the main matching
		while (p <= end)
		{
			const size_t cell = --diag + (c != *(above++));
			diag = *p + 1;
			x++;

			x = min3(x, cell, diag) ;

			*(p++) = x;
		}

		// lower triangle sentinel
		if (col <= half)
		{
			const size_t cell = --diag + (c != *above);
			x++;

			x = std::min(x, cell) ;

			*p = x;
		}
	}

	return *end;
}

size_t Distance::len_1_edist( const wchar_t c, const size_t b_len, const wchar_t * b_str ) const
{
	const wchar_t *end = b_str + b_len ;
	return std::find(b_str, end, c) == end ? b_len : b_len -1 ;
}

size_t Distance::subdist(const std::wstring &needle, const std::wstring &haystack)
{
	const wchar_t* needle_str = needle.c_str() ; 
	const size_t needle_len = needle.size() ; 
	const wchar_t *haystack_str = haystack.c_str() ;
	const size_t haystack_len = haystack.size() ;

	// ensure our static rows are large enough
	ensure_size(haystack_len+1) ;

	// init first row
	std::fill(row1, row1+haystack_len+1, 0) ;

	// Fill the matrix costs
	for (size_t i = 0; i < needle_len; ++i)
	{
		row2[0] = i+1;

		for (size_t j = 0; j < haystack_len; ++j)
		{
			const size_t cost = needle_str[i] == haystack_str[j] ? 0 : 1 ;

			row2[j+1] = min3(row1[j+1]+1, //  deletion
							 row2[j]+1, // insertion
							 row1[j]+cost) // substitution
							;
		}
		// row1 = row2
		std::swap(row1, row2) ;
	}

	// return min(row1)
	return *std::min_element(row1, row1+haystack_len+1) ;
}

void Distance::ensure_size(size_t min_row_size)
{
	if (m_row_size < min_row_size)
	{
		if (row1)
		{
			free(row1) ;
		}
		if (row2)
		{
			free(row2) ;
		}
		m_row_size = min_row_size;
		row1 = (size_t*)calloc(m_row_size, sizeof(size_t));
		row2 = (size_t*)calloc(m_row_size, sizeof(size_t));
		if (!row1 || !row2) // Allocation failed
		{
			throw std::bad_alloc("Failed to allocated memory for Distance test") ;
		}
	}
}



double Distance::edist_score( const std::wstring &a, const std::wstring &b )
{
	// set maxlen and minlen

	size_t a_len = a.size() ;
	size_t b_len = b.size() ;

	if (a_len > b_len)
	{
		std::swap(a_len, b_len) ;
	}


	// Avoid divide by zero errors
	if (b_len == 0) 
	{
		return 0.0 ;
	}

	// Make sure difference in lengths is not too great
	const size_t lendiff = b_len - a_len ;
	const size_t maxdiff = b_len - (size_t)((float)b_len * minscore) ;
	if ( lendiff > maxdiff)
	{
		return 0.0 ;
	}

	// screen using multiset intersection for best possible matchup
	std::map<wchar_t, size_t> bmap ;
	foreach(wchar_t element, b)
	{
		bmap[element]++ ;
	}
	size_t maxalike = 0 ;
	foreach(wchar_t element, a)
	{
		if (bmap.find(element) != bmap.end())
		{
			if (bmap[element])
			{
				maxalike++ ;
				bmap[element]-- ;
			}
		}
	}

	const size_t mindiff = b_len - maxalike ;
	if ( mindiff > maxdiff)
	{
		return 0.0 ;
	}

	// calculate the score
	// score = (maxlen - distance) / maxlen
	return compute_score(b_len, this->edist(a, b)) ;
}

void Distance::set_minscore( double score )
{
	minscore = score ;
}

double Distance::subdist_score( const std::wstring &needle, const std::wstring &haystack )
{
	const size_t needle_len = needle.size() ; 

	if (needle_len == 0) // avoid divide by zero errors
	{
		return 0.0 ;
	}

	size_t distance = this->subdist(needle, haystack) ;

	return compute_score(needle_len, distance) ;
}

Distance::Distance() :
row1(NULL),
	row2(NULL),
	m_row_size(0),
	minscore(0.5f)
{

}

Distance::~Distance()
{
	if (row1) free(row1) ;
	if (row2) free(row2) ;
}

