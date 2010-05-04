#include "stdafx.h"
#include "html_writer.h"


#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( html_writer_test )

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

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		BOOST_CHECK_NO_THROW(fake_html_writer()) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
