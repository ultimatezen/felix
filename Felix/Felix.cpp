// ***************************************************************
//  @brief Entry point for Felix app.
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************

/** @mainpage

  @section intro Introduction
  
	This is the documentation for the Felix program. Felix is the main program in the
	Assistant Suite; most of the other programs in the suite use this one as an automation server.

  @section details Details

	Felix is a translation memory program. 

	Frame (observer) classes:
	- \ref CMainFrame
	- \ref CGlossaryDialog

	Felix is an automation server, and interacts with several application 
	interfaces and stand-alone applications.
 */

#include "StdAfx.h"

#include "resource.h"
#include "ErrorTranslator.h"
#include "logging.h"

// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f Felixps.mk in the project directory.
#include "initguid.h"
#include "Felix.h"
#include "Felix_i.c"

#include "Library.h"			// CLibrary

#include "App.h"

#include <float.h> // _controlfp

#include "FelixApp.h"
#include "DispatchWrapper.h"
#include "atlscintilla.h"
#define BOOST_TEST_MODULE FelixUnitTests
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

using namespace except ;

/** memory debugging */
#ifdef _DEBUG // start memory leak checker

#include < crtdbg.h >

/*!
	@class CLeakChecker
	@brief Checks for memory leaks.
	Making it a module-level variable ensures that it sets the leak-check flags 
	before other program code runs.
 */
class CLeakChecker

{
public:
	CLeakChecker()
	{	
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
	}
} ;

#ifndef UNIT_TEST
CLeakChecker	leak_checker ;
#endif

#endif // #if _DEBUG // start memory leak checker


void felix_invalid_parameter_handler(
									 const wchar_t * expression,
									 const wchar_t * function_name, 
									 const wchar_t * file_name, 
									 int line,
									 uintptr_t /*pReserved*/
									 )
{
	wstring msg = (wformat(L"Invalid parameter in function %1%.\rExpression: %2%\rFile: %3%\rLine: %4%" ) 
		% function_name 
		% expression
		% file_name
		% line).str() ;

	CString err_msg(msg.c_str()) ;
	throw CProgramException(err_msg) ;
}

// Our customized version of the server app module, to enable
// the resource DLL to be loaded dynamically (according to language)
CLocalizedServerAppModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

/*!
 * Set up the app message loop.
 */
int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame &view = app::get_app() ;

	ATLASSERT( ! view.m_message_map.empty() ) ;
	
	if(view.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	ATLASSERT( view.IsWindow() ) ;

	_Module.Lock();
	view.put_visible(nCmdShow) ;

	const int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();

	return nRet;
}

/*!
 * \brief
 * Put it in a subroutine to allow exceptions to be caught.
 * 
 * \returns
 * int - success
 */
int MainSub(HINSTANCE hInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{
	int nRet = EXIT_SUCCESS ;

	ATLTRACE( " _MSC_VER = %d\n",  _MSC_VER ) ;

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L) ;
	ATLASSERT( ::GetLastError() == 1400 ) ;
	::SetLastError( 0 ) ; // clear the error caused by calling defwindowproc with NULL
	
	// add flags to support other controls
	logging::log_debug("Loading Windows libraries") ;
	ATLVERIFY(AtlInitCommonControls( ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_INTERNET_CLASSES | ICC_USEREX_CLASSES | ICC_WIN95_CLASSES | ICC_NATIVEFNTCTL_CLASS)) ;
	CLibrary riched_lib( CWideRichEdit::GetLibraryName() ) ;

	ATLASSERT( riched_lib.is_loaded() ) ;

	if ( ! riched_lib.is_loaded() )
	{
		logging::log_debug("Loading RTF library") ;
		riched_lib.load( CRichEditCtrl::GetLibraryName() ) ;
	}

	logging::log_debug("Opening main Felix window") ;
	COM_ENFORCE( _Module.Init(ObjectMap, hInstance, &LIBID_ATLLib), _T("Failed to initialize the module.") );
	TRUE_ENFORCE( FALSE != AtlAxWinInit(), _T("Call to AtlAxWinInit() failed!") ) ;

	bool bRun = true;
	bool bAutomation = false;
	bool bRegister = false ;
#ifdef _DEBUG
	bRegister = true ;
#endif

	TCHAR szTokens[] = _T("-/");
	LPCTSTR lpszToken = _Module.FindOneOf(::GetCommandLine(), szTokens);
	while(lpszToken != NULL)
	{
		if( ! _tcsicmp( lpszToken, _T("UnregServer") ) )
		{
			nRet = _Module.UnregisterServer();
			bRun = false;
			break;
		}
		else if( ! _tcsicmp( lpszToken, _T("RegServer") )  )
		{
			bRegister = true ;
			bRun = false;
			break;
		}
		else if(! _tcsicmp( lpszToken, _T("Automation") ) ||
			! _tcsicmp( lpszToken, _T("Embedding") ) )
		{
			bAutomation = true;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}
	
	if (bRegister)
	{
		logging::log_debug("Registering Felix COM server") ;
		COM_ENFORCE( _Module.RegisterServer(TRUE), _T("Failed to register server") ) ;
		::MessageBeep( MB_ICONINFORMATION ) ;
	}
	if(bRun)
	{
#ifdef _DEBUG
		_Module.RegisterServer(TRUE) ;
#endif

		try
		{
			logging::log_debug("Ensuring HTML files") ;
			CDispatchWrapper utils(L"Felix.Utilities") ;
			utils.method(L"EnsureHtml") ;
		}
		catch (_com_error& e)
		{
			logging::log_error("Failed to ensure HTML files") ;
			logging::log_exception(e) ;
			ATLASSERT(FALSE && "Failed to ensure HTML files") ;
		}


		_Module.StartMonitor();
		HRESULT hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
		ATLASSERT( SUCCEEDED( hRes ) ) ;

		hRes = ::CoResumeClassObjects();
		ATLASSERT(SUCCEEDED(hRes));
		
		// This starts the message loop without creating a main window.
		// The user can then create the main window from the App object (App.Visible = TRUE).
		if(bAutomation)
		{
			CMessageLoop theLoop;
			_Module.AddMessageLoop(&theLoop);
			
			_Module.Lock();
			
			nRet = theLoop.Run();
			
			_Module.RemoveMessageLoop();
		}
		else
		{
			nRet = Run(lpstrCmdLine, nCmdShow);
		}
		
	}
	::Sleep(_Module.m_dwPause);
	_Module.RevokeClassObjects();
	_Module.Term();
	::OleUninitialize() ;
	return nRet ;
}

/*!
 * Our program's entry point.
 * 
 * Main entry point of the program, called by the real main (which lives in crt land).
 */
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	ATLVERIFY(SUCCEEDED(::OleInitialize(NULL)));

	// sets us to convert SEH into C++ exceptions,
	// and sets up float exceptions as well
	set_up_exception_handling() ;
	_invalid_parameter_handler oldHandler;
	oldHandler = _set_invalid_parameter_handler((_invalid_parameter_handler)felix_invalid_parameter_handler);
	
	try
	{
		logging::log_debug("Launching Felix") ;
	}
	catch (_com_error& e)
	{
		e ;
	}
	catch(...)
	{
		ATLASSERT(FALSE && "Exception trying to do logging") ;
	}


	logging::log_debug("Loading Scintilla library") ;
	WTL::ScintillaModule &scintilla_module = WTL::ScintillaModule::instance() ;
	scintilla_module ;
	ATLASSERT(scintilla_module.IsLoaded()) ;

#ifdef UNIT_TEST
	lpstrCmdLine ;
	nCmdShow ;

    ATLTRACE( "Unit testing app...\n" ) ;
	COM_ENFORCE( _Module.Init(ObjectMap, hInstance, &LIBID_ATLLib), _T("Failed to initialize the module.") );
	ATLVERIFY(_Module.set_library( _T("lang\\EngResource.dll") )) ;

	char *args[] = {"", "--report_level=detailed", "--result_code=yes", "--log_level=message"};
	int ut_result = ::boost::unit_test::unit_test_main(&init_unit_test_suite, sizeof(args) / sizeof(char*), args);

	if (ut_result)
	{
		::MessageBeep(MB_ICONSTOP) ;
		ATLTRACE("ERRORS IN UNIT TESTS!\n") ;
	}
	else
	{
		::MessageBeep(MB_ICONINFORMATION) ;
		ATLTRACE("Boost unit tests: 0 errors\n\nok.\n") ;
	}

	_Module.Term() ;
	::OleUninitialize() ;
    return EXIT_SUCCESS ;

#else

	// going to a subroutine makes it cleaner to separate app code from error handling
	try
	{
		 return MainSub( hInstance, lpstrCmdLine, nCmdShow ) ;
	}
	catch ( CException &e )
	{
		logging::log_error("Fatal program exception. The program will shut down now.") ;
		logging::log_exception(e) ;
		e.notify_user( CString( "Fatal program exception. The program will shut down now." ) ) ;
	}
	catch ( std::exception &e ) 
	{ 
		logging::log_error("Fatal CRT exception. Program must terminate now.") ;
		logging::log_error(e.what()) ;
		::MessageBoxA( NULL, ( e.what() + string("\rFatal CRT exception. Program must terminate now.") ).c_str(), "C Runtime Exception", MB_OK | MB_ICONSTOP | MB_SETFOREGROUND   ) ;
	}
	catch ( _com_error &e ) 
	{ 
		logging::log_error("Fatal COM exception. The program will shut down now.") ;
		logging::log_exception(e) ;
		CComException com_ex(e) ;
		com_ex.notify_user( CString( "Fatal COM exception. The program will shut down now." ) ) ;
	}
	catch ( ... )
	{
		logging::log_error("Fatal exception. The program will terminate now.") ;
		::MessageBoxA( NULL, "Fatal exception. The program will terminate now.", "FATAL ERROR", MB_OK | MB_SETFOREGROUND | MB_ICONSTOP ) ;
	}

	::OleUninitialize();
	return EXIT_FAILURE ;

#endif
}

