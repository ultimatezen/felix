#include "StdAfx.h"
#include "logging.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_logging )

static std::vector<string> m_debug_calls ;
static std::vector<string> m_warn_calls ;
static std::vector<string> m_error_calls ;
static std::vector<string> m_exception_calls ;

using namespace except;

class fake_logger : public logger
{
public:

	void init()
	{
		m_debug_calls.push_back("init") ;
	}
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
	void log_exception(_com_error &e)
	{
		m_exception_calls.push_back((LPCSTR)CStringA(e.ErrorMessage())) ;
	}
	void send_report(const CString language, const CString filename)
	{
		m_exception_calls.push_back("send_report") ;
	}
};

void init_vectors()
{
	m_debug_calls.clear() ;
	m_warn_calls.clear() ;
	m_error_calls.clear() ;
	m_exception_calls.clear() ;
}

	// cp_from_lang_str
	BOOST_AUTO_TEST_CASE( test_logging_log_warn )
	{
		file_logger logger ;
		logger.log_warn("from felix") ;
		BOOST_CHECK(true) ;
	}

	BOOST_AUTO_TEST_CASE( log_warn)
	{
		init_vectors() ;
		fake_logger *faker = new fake_logger ;
		logging::set_logger(logger_ptr(faker)) ;
		logging::log_warn("spam") ;

		BOOST_CHECK_EQUAL(0, (int)m_debug_calls.size()) ;
		BOOST_CHECK_EQUAL(1, (int)m_warn_calls.size()) ;
		BOOST_CHECK_EQUAL(0, (int)m_error_calls.size()) ;
		BOOST_CHECK_EQUAL(0, (int)m_exception_calls.size()) ;
		string actual = m_warn_calls[0] ;
		BOOST_CHECK_EQUAL("spam", string(actual.c_str())) ;

	}
	BOOST_AUTO_TEST_CASE( log_error)
	{
		init_vectors() ;
		fake_logger *faker = new fake_logger ;
		logging::set_logger(logger_ptr(faker)) ;
		logging::log_error("spam") ;

		BOOST_CHECK_EQUAL(0, (int)m_debug_calls.size()) ;
		BOOST_CHECK_EQUAL(0, (int)m_warn_calls.size()) ;
		BOOST_CHECK_EQUAL(1, (int)m_error_calls.size()) ;
		BOOST_CHECK_EQUAL(0, (int)m_exception_calls.size()) ;
		string actual = m_error_calls[0] ;
		BOOST_CHECK_EQUAL("spam", string(actual.c_str())) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef _DEBUG