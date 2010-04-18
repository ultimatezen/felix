#include "stdafx.h"
#include "html_writer.h"


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( TestMemoryFunctions )


	class fake_html_writer : public html_writer
	{
	public:
		string m_sensing_variable ;

		void write_text( const wstring&)
		{
			m_sensing_variable += "[write_text]" ;
		}
		void apply_linebreak()
		{
			m_sensing_variable += "[apply_linebreak]" ;
		};
		void apply_paragraph()
		{
			m_sensing_variable += "[apply_paragraph]" ;
		};
		
	};

	TEST( html_writer_test, Instantiate )
	{
		try
		{
			fake_html_writer writer ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate html_writer class in test harness" ) ;
		}
	}
BOOST_AUTO_TEST_SUITE_END()