#include "StdAfx.h"
#include "logging.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

class fake_logger : public logger
{
public:
	std::vector<string> m_debug_calls ;

	void init(std::ostream)
	{
		m_debug_calls.push_back("init") ;
	}
	void write_entry(const string level, const string msg)
	{
		m_debug_calls.push_back("write_entry") ;
		m_debug_calls.push_back(level) ;
		m_debug_calls.push_back(msg) ;
	}
	void send_report(const CString language, const CString filename)
	{
		m_debug_calls.push_back("send_report") ;
	}
};


BOOST_AUTO_TEST_SUITE( test_logging )

	// cp_from_lang_str
	BOOST_AUTO_TEST_CASE( test_log_warn )
	{
		fake_logger *faker = new fake_logger ;
		logging::set_logger(logger_ptr(faker)) ;
		BOOST_CHECK_NO_THROW(logging::log_warn("from felix")) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif