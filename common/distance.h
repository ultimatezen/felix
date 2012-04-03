#pragma once

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
	size_t subdist(const std::wstring &needle, const std::wstring &haystack);

	double edist_score(const std::wstring &a, const std::wstring &b);
	double subdist_score(const std::wstring &needle, const std::wstring &haystack);

	void ensure_size(size_t min_row_size) ;
	// minimum of three values
	size_t min3(size_t a, size_t b, size_t c) const ;

};