#include "stdafx.h"
#include "pagination.h"
#include "text_templates.h"
#include "numberfmt.h"
#include "cpptempl.h"

using namespace cpptempl ;

const static size_t PAGE_WINDOW_SIZE = 10 ;

Paginator::Paginator(size_t records_per_page) :
m_current_page(0),
m_num_records(0),
m_num_pages(0),
m_records_per_page(records_per_page)
{
}

size_t Paginator::get_current_page()
{
	return m_current_page ;
}

void Paginator::next_page()
{
	++m_current_page ;
}

void Paginator::prev_page()
{
	--m_current_page ;
}

bool Paginator::has_next()
{
	if (! m_num_pages)
	{
		return false ;
	}
	return m_current_page < (m_num_pages-1) ;
}

bool Paginator::has_prev()
{
	return m_current_page > 0 ;
}

size_t Paginator::get_num_pages()
{
	return m_num_pages ;
}

size_t Paginator::get_start()
{
	return m_current_page * m_records_per_page ;
}

size_t Paginator::get_end()
{
	return min(m_num_records, get_start() + m_records_per_page) ;
}

void Paginator::set_num_records( size_t num_records, bool reset_current_page )
{
	m_num_records = num_records ;
	m_num_pages = num_records / m_records_per_page ;
	if ((m_num_pages*m_records_per_page) < num_records)
	{
		++m_num_pages ;
	}
	if (reset_current_page)
	{
		m_current_page = 0 ;
	}
}

size_t Paginator::get_num_records()
{
	return m_num_records ;
}

void Paginator::goto_page( size_t page )
{
	m_current_page = page ;
}

void Paginator::set_current_page( size_t page )
{
	m_current_page = page ;
}
/*
	Create the window of page numbers to show.
	This is so if there are lots of pages, only the surrounding ones are shown, 
	with links to the start and end.

	params
	current: current page
	num: number of pages
	window_size: size of window (half on each side)

	Something like this:

	[<<] [<] [10] [11] [12] 13 [14] [15] [16] [>] [>>]

	Where 13 is the current item.
*/
window_range make_window(size_t current, size_t num, size_t window_size)
{
	if (num < window_size)
	{
		return std::make_pair(0u, num) ;
	}
	if (! current)
	{
		return std::make_pair(0u, window_size) ;
	}
	size_t half = window_size / 2 ;
	if (current < half)
	{
		return std::make_pair(0u, window_size) ;
	}
	if (current > num - half)
	{
		return std::make_pair(num - window_size, num) ;
	}

	return std::make_pair(current - half, current + half) ;
}

wstring get_pagination_text(Paginator &paginator)
{
	data_map data ;

	// previous
	if (paginator.has_prev())
	{
		data[L"has_prev"] = make_data(L"true") ;
		data[L"prev_page"] = make_data(ulong2wstring(paginator.get_current_page())) ;
	}
	else
	{
		data[L"has_prev"] = make_data(L"") ;
	}

	// next
	if (paginator.has_next())
	{
		data[L"has_next"] = make_data(L"true") ;
		data[L"next_page"] = make_data(ulong2wstring(paginator.get_current_page()+2)) ;
	}
	else
	{
		data[L"has_next"] = make_data(L"") ;
	}

	// current page
	data[L"current_page"] = make_data(ulong2wstring(paginator.get_current_page()+1)) ;
	data[L"last_page"] = make_data(ulong2wstring(paginator.get_num_pages())) ;

	// num matches
	data[L"num_matches"] = make_data(fmt_num(paginator.get_num_records())) ;

	// start/end
	data[L"matches_start"] = make_data(fmt_num(paginator.get_start()+1)) ;
	data[L"matches_end"] = make_data(fmt_num(paginator.get_end())) ;

	// pages
	cpptempl::data_list pages ;

	window_range pagerange = make_window(paginator.get_current_page(), paginator.get_num_pages(), PAGE_WINDOW_SIZE) ;
	for (size_t i = pagerange.first ; i < pagerange.second ; ++i )
	{
		pages.push_back(cpptempl::make_data(ulong2wstring(i+1))) ;
	}
	if (pagerange.second < paginator.get_num_pages())
	{
		pages.push_back(cpptempl::make_data(ulong2wstring(pagerange.second+1))) ;
	}

	data[L"pages"] = cpptempl::make_data(pages) ;

	wstring text = get_template_text(_T("pagination.txt")) ;
	return cpptempl::parse(text, data) ;
}