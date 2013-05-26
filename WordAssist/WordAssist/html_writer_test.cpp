#include "stdafx.h"
#include "html_processing.h"
#include "html_parser.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

	class fake_html_writer : public writer_interface
	{
	public:
		std::vector<wstring> m_sensing_variable ;
		html_parser m_parser ;

		fake_html_writer() : m_parser(this){}

		void write_text( const wstring &text)
		{
			m_sensing_variable.push_back(text) ;
		}
		void apply_linebreak()
		{
			m_sensing_variable.push_back(L"apply_linebreak") ;
		};
		void apply_paragraph()
		{
			m_sensing_variable.push_back(L"apply_paragraph") ;
		};
		void write_html(const wstring html_text)
		{
			m_parser.write_html(html_text) ;
		}
		
	};

BOOST_AUTO_TEST_SUITE( html_writer_test )

	BOOST_AUTO_TEST_CASE(test_plain_text)
	{
		fake_html_writer writer ;
		writer.write_html(L"text") ;
		BOOST_CHECK_EQUAL(writer.m_sensing_variable.size(), 1u) ;
		BOOST_CHECK_EQUAL(writer.m_sensing_variable[0], L"text") ;
	}

	BOOST_AUTO_TEST_CASE(test_bold_text)
	{
		fake_html_writer writer ;
		writer.write_html(L"<b>text</b>") ;
		BOOST_CHECK_EQUAL(writer.m_sensing_variable.size(), 1u) ;
		BOOST_CHECK_EQUAL(writer.m_sensing_variable[0], L"text") ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
