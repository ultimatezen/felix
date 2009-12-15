#include "StdAfx.h"
#include "logging.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

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
namespace easyunit
{
	// cp_from_lang_str
	TEST( test_logging, log_warn )
	{
		file_logger logger ;
		logger.log_warn("from felix") ;
		ASSERT_TRUE(true) ;
	}

	TEST( test_logger, log_warn)
	{
		init_vectors() ;
		fake_logger *faker = new fake_logger ;
		logging::set_logger(logger_ptr(faker)) ;
		logging::log_warn("spam") ;

		ASSERT_EQUALS_V(0, (int)m_debug_calls.size()) ;
		ASSERT_EQUALS_V(1, (int)m_warn_calls.size()) ;
		ASSERT_EQUALS_V(0, (int)m_error_calls.size()) ;
		ASSERT_EQUALS_V(0, (int)m_exception_calls.size()) ;
		string actual = m_warn_calls[0] ;
		ASSERT_EQUALS_V("spam", SimpleString(actual.c_str()))

	}
	TEST( test_logger, log_error)
	{
		init_vectors() ;
		fake_logger *faker = new fake_logger ;
		logging::set_logger(logger_ptr(faker)) ;
		logging::log_error("spam") ;

		ASSERT_EQUALS_V(0, (int)m_debug_calls.size()) ;
		ASSERT_EQUALS_V(0, (int)m_warn_calls.size()) ;
		ASSERT_EQUALS_V(1, (int)m_error_calls.size()) ;
		ASSERT_EQUALS_V(0, (int)m_exception_calls.size()) ;
		string actual = m_error_calls[0] ;
		ASSERT_EQUALS_V("spam", SimpleString(actual.c_str()))
	}

}


#endif // #ifdef _DEBUG