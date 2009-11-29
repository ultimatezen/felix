#pragma once

#include "tag_stripper.h"
#include <boost/algorithm/string.hpp>
#include "StringEx.h"

class CmpMaker
{
public:
	bool m_ignore_width ;
	bool m_ignore_hira_kata ;
	bool m_ignore_case ;
	CmpMaker(bool ignore_width=false,
		bool ignore_hira_kara=false,
		bool ignore_case=false) : 
			m_ignore_width(ignore_width), 
			m_ignore_hira_kata(ignore_hira_kara), 
			m_ignore_case(ignore_case)
	{}
	CmpMaker(const CmpMaker &maker) :
			m_ignore_width(maker.m_ignore_width), 
			m_ignore_hira_kata(maker.m_ignore_hira_kata), 
			m_ignore_case(maker.m_ignore_case)
	{}
	wstring make_cmp(const wstring &rich)
	{
		wstring cmp = strip_tags(rich) ;
		if (m_ignore_width)
		{
			str::normalize_width(cmp) ;
		}
		if (m_ignore_case)
		{
			boost::to_lower(cmp) ;
		}
		if (m_ignore_hira_kata)
		{
			str::normalize_hiragana_to_katakana(cmp) ;
		}
		return cmp ;
	}
};

class Segment
{
	wstring m_rich ;
	wstring m_cmp ;

	CmpMaker m_cmp_maker ;

public:
	Segment(const CmpMaker maker, const wstring &rhs) : m_rich(rhs), m_cmp_maker(maker) 
	{
		create_cmp(rhs) ;
	}
	Segment(const CmpMaker maker) : m_cmp_maker(maker) 
	{
	}
	Segment(const Segment &segment) :
		m_rich(segment.rich()),
		m_cmp(segment.cmp()),
		m_cmp_maker(segment.m_cmp_maker)
	{}
	Segment() {}
	Segment& operator=(const Segment &segment)
	{
		m_rich = segment.rich() ;
		m_cmp = segment.cmp() ;
		m_cmp_maker = segment.m_cmp_maker ;

		return *this ;
	}
	void set_value(const wstring &rhs)
	{
		m_rich = rhs ;
		create_cmp(rhs) ;
	}
	void set_cmp_maker(const CmpMaker maker)
	{
		m_cmp_maker = maker ;
		create_cmp(m_rich) ;
	}
	const wstring &rich() const
	{
		return m_rich ;
	}
	const wstring plain() const
	{
		return strip_tags(m_rich) ;
	}
	const wstring &cmp() const
	{
		return m_cmp ;
	}
	void create_cmp(const wstring &rich)
	{
		m_cmp = m_cmp_maker.make_cmp(rich) ;
	}
};