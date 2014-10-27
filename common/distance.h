/*
	Calculates edit distance, and similarity score (0 to 1) based on 
	edit distance.
*/

#pragma once

// minimum of three values
size_t min3(size_t a, size_t b, size_t c) ;
// Gets the score, given the highest length and diff (total cost)
// side effect: Logs warning on divide by 0 error.
double compute_score( const size_t high_len, size_t total_cost ) ;

// Class to calculate edit distance and similarity score
class Distance
{
	size_t *row1 ;
	size_t *row2 ;
	size_t m_row_size ;
	double minscore ;
public:
	Distance();
	~Distance();
	void set_minscore(double score);
	size_t edist(const std::wstring &a, const std::wstring &b);

	// get the edit distance when one of the strings only has one character
	size_t len_1_edist( const wchar_t c, const size_t b_len, const wchar_t * b_str ) const;
	size_t subdist(const std::wstring &needle, const std::wstring &haystack);

	double edist_score(const std::wstring &a, const std::wstring &b);
	double subdist_score(const std::wstring &needle, const std::wstring &haystack);

	void ensure_size(size_t min_row_size) ;
};