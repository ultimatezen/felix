#include "StdAfx.h"
#include "distance.h"
#include <map>

using namespace std; 

size_t Distance::edist(const wstring &a, const wstring &b)
{
	const wchar_t* a_str = a.c_str() ;
	size_t a_len = a.size() ;
	const wchar_t *b_str = b.c_str() ;
	size_t b_len = b.size() ;

	// swap if b is smaller
	if (b_len < a_len)
	{
		const wchar_t *temp = a_str;
		a_str = b_str;
		b_str = temp;

		a_len^=b_len;
		b_len^=a_len;
		a_len^=b_len;
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
		return (size_t)a_len + b_len;
	}

	// check a_len == 1
	if (a_len == 1)
	{
		wchar_t c = *a_str;
		for (size_t i = 0 ; i < b_len ; ++i)
		{
			if (b_str[i] == c)
			{
				return (size_t)b_len - 1;
			}
		}
		return (size_t)b_len;
	}

	a_len++;
	b_len++;

	// Halfway point used for trimming triangle below
	size_t half = a_len >> 1;

	// allocate memory for row1 if necessary
	ensure_size(b_len) ;

	size_t *end = row1 + b_len - 1;

	// initialize the first row1
	size_t col_b = 0;
	for (col_b = 0; col_b < b_len - half; col_b++)
	{
		row1[col_b] = col_b;
	}

	// set up loop variables
	row1[0] = a_len - half - 1;
	size_t diag, x;
	size_t *p;
	const wchar_t *above;
	size_t col = 0;

	for (col = 1; col < a_len; col++)
	{
		const wchar_t c = a_str[col - 1];
		// skip the upper triangle
		if (col >= a_len - half)
		{
			size_t offset = col - (a_len - half);
			size_t cell;

			above = b_str + offset;
			p = row1 + offset;
			cell = *(p++) + (c != *(above++));
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
			size_t cell = --diag + (c != *(above++));
			x++;
			if (x > cell)
			{
				x = cell;
			}
			diag = *p + 1;
			if (x > diag)
			{
				x = diag;
			}
			*(p++) = x;
		}

		// lower triangle sentinel
		if (col <= half)
		{
			size_t cell = --diag + (c != *above);
			x++;
			if (x > cell)
			{
				x = cell;
			}
			*p = x;
		}
	}

	return *end;
}

size_t Distance::subdist(const wstring &needle, const wstring &haystack)
{
	const wchar_t* needle_str = needle.c_str() ; 
	size_t needle_len = needle.size() ; 
	const wchar_t *haystack_str = haystack.c_str() ;
	size_t haystack_len = haystack.size() ;

	// ensure our static rows are large enough
	ensure_size(haystack_len+1) ;

	// init first row
	std::fill(row1, row1+haystack_len+1, 0) ;

	size_t cost = 0;

	// Fill the matrix costs
	for (size_t i = 0; i < needle_len; ++i)
	{
		row2[0] = i+1;

		for (size_t j = 0; j < haystack_len; ++j)
		{
			cost = 1;
			if (needle_str[i] == haystack_str[j])
			{
				cost = 0;
			}

			row2[j+1] = min3(row1[j+1]+1, //  deletion
							 row2[j]+1, // insertion
							 row1[j]+cost) // substitution
							;
		}
		// row1 = row2
		size_t *tmp = row1 ;
		row1 = row2 ;
		row2 = tmp ;
	}

	// return min(row1)
	return *std::min_element(row1, row1+haystack_len+1) ;
}

void Distance::ensure_size(size_t min_row_size)
{
	if (row_size < min_row_size)
	{
		if (row1)
		{
			free(row1) ;
		}
		if (row2)
		{
			free(row2) ;
		}
		row_size = min_row_size;
		row1 = (size_t*)calloc(row_size, sizeof(size_t));
		row2 = (size_t*)calloc(row_size, sizeof(size_t));
		if (!row1 || !row2) // Allocation failed
		{
			throw std::bad_alloc("Failed to allocated memory for Distance test") ;
		}
	}
}

size_t Distance::min2( size_t a, size_t b ) const 
{
	if (a < b)
	{
		return a;
	}
	return b;
}

size_t Distance::min3( size_t a, size_t b, size_t c ) const 
{
	return min2(a, min2(b, c) ) ;
}

double Distance::edist_score( const wstring &a, const wstring &b )
{
	// set maxlen and minlen
	const size_t a_len = a.size() ;
	const size_t b_len = b.size() ;

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
	const size_t lendiff = maxlen - minlen ;
	const size_t maxdiff = maxlen - (size_t)((float)maxlen * minscore) ;
	if ( lendiff > maxdiff)
	{
		return 0.0 ;
	}

	// screen using multiset intersection for best possible matchup
	std::map<wchar_t, size_t> bmap ;
	foreach(wchar_t element, b)
	{
		if (bmap.find(element) == bmap.end())
		{
			bmap[element] = 1 ;
		}
		else
		{
			bmap[element]++ ;
		}
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

	const size_t mindiff = maxlen - maxalike ;
	if ( mindiff > maxdiff)
	{
		return 0.0 ;
	}

	const size_t distance = this->edist(a, b) ;

	// calculate the score
	// score = (maxlen - distance) / maxlen

	return ((double)maxlen - (double)distance) / (double)maxlen ;
}