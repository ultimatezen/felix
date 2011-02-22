// NewDev.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "NewDev.h"

#define BOOST_TEST_MODULE NewDevTests
#include <boost/test/unit_test.hpp>
#include <boost/timer.hpp>

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

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	::OutputDebugStringA( "Unit testing app...\n" ) ;

	char *args[] = {"", "--log_format=detailed", "--result_code=yes", "--log_level=message"};
	int ut_result = ::boost::unit_test::unit_test_main(&init_unit_test_suite, sizeof(args) / sizeof(char*), args);

	if (ut_result)
	{
		::MessageBeep(MB_ICONSTOP) ;
		::OutputDebugStringA("ERRORS IN UNIT TESTS!\n") ;
	}
	else
	{
		::MessageBeep(MB_ICONINFORMATION) ;
		::OutputDebugStringA("Boost unit tests: 0 errors\n\nok.\n") ;
	}

	return EXIT_SUCCESS ;


	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
}
