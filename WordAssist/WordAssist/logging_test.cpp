#include "StdAfx.h"
#include "logging.h"


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_logging )

class fake_logger : public logger
{
public:
	std::vector<string> m_debug_calls ;
	std::vector<string> m_warn_calls ;
	std::vector<string> m_error_calls ;
	std::vector<string> m_exception_calls ;

	void log_debug(const string msg)
	{
		m_debug_calls.push_back(msg) ;
	}
	void log_warn(const string msg)
	{
		m_warn_calls.push_back(msg) ;
	}
	void log_error(const string msg)
	{
		m_error_calls.push_back(msg) ;
	}
	void log_exception(CException &e)
	{
		m_exception_calls.push_back((LPCSTR)CStringA(e.format_message_for_message_box())) ;
	}
};


	// cp_from_lang_str
	BOOST_AUTO_TEST_SUITE( test_log_warn )
	{
		logging::log_warn("from felix") ;
		ASSERT_TRUE(true) ;
	}
BOOST_AUTO_TEST_SUITE_END()
