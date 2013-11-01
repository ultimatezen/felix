#include "StdAfx.h"
#include "logging.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_logging )

using namespace except;

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


	BOOST_AUTO_TEST_CASE( log_warn)
	{
		fake_logger *faker = new fake_logger ;
		logging::set_logger(logger_ptr(faker)) ;
		logging::log_warn("spam") ;

		BOOST_CHECK_EQUAL(3u, faker->m_debug_calls.size()) ;
		BOOST_CHECK_EQUAL("write_entry", faker->m_debug_calls[0]) ;
		BOOST_CHECK_EQUAL("WARN", faker->m_debug_calls[1]) ;
		BOOST_CHECK_EQUAL("spam", faker->m_debug_calls[2]) ;
	}
	BOOST_AUTO_TEST_CASE( log_error)
	{
		fake_logger *faker = new fake_logger ;
		logging::set_logger(logger_ptr(faker)) ;
		logging::log_error("spam") ;

		BOOST_CHECK_EQUAL(3u, faker->m_debug_calls.size()) ;
		BOOST_CHECK_EQUAL("write_entry", faker->m_debug_calls[0]) ;
		BOOST_CHECK_EQUAL("ERROR", faker->m_debug_calls[1]) ;
		BOOST_CHECK_EQUAL("spam", faker->m_debug_calls[2]) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef _DEBUG