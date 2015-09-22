#pragma once

#define BOOST_TEST_MODULE FelixUnitTests
#include <boost/test/unit_test.hpp>
#include <boost/timer.hpp>

// Setup stuff for unit testing using Boost.Test
template <class CharT, class TraitsT = std::char_traits<CharT> >
class basic_debugbuf : 
	public std::basic_stringbuf<CharT, TraitsT>
{
public:

	virtual ~basic_debugbuf()
	{
		sync();
	}

protected:

	int sync()
	{
		output_debug_string(str().c_str());
		str(std::basic_string<CharT>());    // Clear the string buffer

		return 0;
	}

	void output_debug_string(const CharT *text) {}
};
template<>
void basic_debugbuf<char>::output_debug_string(const char *text)
{
	::OutputDebugStringA(text);
}

template<>
void basic_debugbuf<wchar_t>::output_debug_string(const wchar_t *text)
{
	::OutputDebugStringW(text);
}
template<class CharT, class TraitsT = std::char_traits<CharT> >
class basic_dostream : 
	public std::basic_ostream<CharT, TraitsT>
{
public:

	basic_dostream() : std::basic_ostream<CharT, TraitsT>
		(new basic_debugbuf<CharT, TraitsT>()) {}
	~basic_dostream() 
	{
		delete rdbuf(); 
	}
};

typedef basic_dostream<char>    dostream;
typedef basic_dostream<wchar_t> wdostream;

struct UnitTestConfig {
	boost::timer t ;

	UnitTestConfig()
	{ 
		boost::unit_test::unit_test_log.set_stream( out ); 
	}
	~UnitTestConfig()  
	{
		BOOST_TEST_MESSAGE((format("Finished in %.2f seconds\n\n") % t.elapsed()).str()) ;
		boost::unit_test::unit_test_log.set_stream( std::cout ); 
	}

	dostream                    out ;
};

BOOST_GLOBAL_FIXTURE( UnitTestConfig );
