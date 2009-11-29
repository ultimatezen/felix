#pragma once

const static size_t records_per_page = 20 ;

class Paginator
{
	size_t m_current_page ;
	size_t m_num_pages ;
	size_t m_num_records ;
public:
	Paginator();

	void set_num_records(size_t num_records);
	size_t get_current_page();
	void next_page();
	void prev_page();
	bool has_next();
	bool has_prev();
	size_t get_num_pages();
	size_t get_start();
	size_t get_end();
};

wstring get_pagination_text(Paginator &paginator) ;