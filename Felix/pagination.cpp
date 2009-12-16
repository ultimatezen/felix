#include "stdafx.h"
#include "pagination.h"
#include "TextTemplate.h"
#include "text_templates.h"

using namespace text_template ;

Paginator::Paginator() :
m_current_page(0),
m_num_records(0),
m_num_pages(0)
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
	return m_current_page * records_per_page ;
}

size_t Paginator::get_end()
{
	return min(m_num_records, get_start() + records_per_page) ;
}

void Paginator::set_num_records( size_t num_records )
{
	m_num_records = num_records ;
	m_num_pages = num_records / records_per_page ;
	if ((m_num_pages*records_per_page) < num_records)
	{
		++m_num_pages ;
	}
	m_current_page = 0 ;
}

wstring get_pagination_text(Paginator &paginator)
{
	CTextTemplate text_template ;

	if (paginator.has_prev())
	{
		text_template.Assign(L"has_prev", L"true") ;
	}
	else
	{
		text_template.Assign(L"has_prev", L"") ;
	}

	if (paginator.has_next())
	{
		text_template.Assign(L"has_next", L"true") ;
	}
	else
	{
		text_template.Assign(L"has_next", L"") ;
	}

	wstring text = get_template_text(_T("pagination.txt")) ;
	return text_template.Fetch(text) ;
}