#pragma once

const static size_t RECORDS_PER_PAGE = 20 ;

class Paginator
{
public:
	size_t m_current_page ;
	size_t m_num_pages ;
	size_t m_num_records ;
	size_t m_records_per_page ;

	Paginator(size_t records_per_page = RECORDS_PER_PAGE);

	void set_num_records(size_t num_records, bool reset_current_page=true);
	size_t get_current_page();
	size_t get_num_records();

	void next_page();
	void prev_page();

	void goto_page(size_t page);

	bool has_next();
	bool has_prev();
	size_t get_num_pages();
	size_t get_start();
	size_t get_end();
};
typedef std::pair<size_t, size_t> window_range ;

window_range make_window(size_t current, size_t num, size_t window_size) ;

wstring get_pagination_text(Paginator &paginator, CString tmpl_name = _T("pagination.txt")) ;