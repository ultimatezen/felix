/*! Entry point for Felix app.

  Felix could get called as a main application, or as a COM server.
  - Main application
	  Launches the mainframe window, starts message loop
  - COM server
    Creates Felix::App and starts message loop, without
	creating main window.
  
****************************************************************/

/*! @mainpage

  @section intro Introduction
  
	This is the documentation for the Felix program. Felix is the main program in the
	Assistant Suite; most of the other programs in the suite use this one as an automation server.

  @section details Details

	Felix is a translation memory program. 

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

#ifdef UNIT_TEST

#ifndef BOOST_TEST_MODULE
	#define BOOST_TEST_MODULE FelixUnitTests
#endif

	#define BOOST_TEST_MAIN
	#include "unit_test_config.h"
	#include "unit_testing.h"
	#include <boost/test/unit_test_suite.hpp>

#endif

#include "LuaState.h"

using namespace except ;

/** memory debugging */
#ifdef _DEBUG // start memory leak checker
#ifndef UNIT_TEST
CLeakChecker	leak_checker ;
#endif

#endif // #if _DEBUG // end memory leak checker

/*! Handler for invalid parameters.

Function to be called when the CRT detects an invalid argument.
*/
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

//! Our customized version of the server app module, to enable
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

	// singleton ensures we have only one main window.
	MemoryWindowFrame &view = app::get_app() ;

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
 * Put main code into a subroutine to allow exceptions to be caught more easily.
 * 
 * \returns
 * int - success: EXIT_SUCCESS on success, other value otherwise.
 */
int MainSub(HINSTANCE hInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{
	int nRet = EXIT_SUCCESS ;

	ATLTRACE( " _MSC_VER = %d\n",  _MSC_VER ) ;

	// add flags to support other controls
	LOG_DEBUG("Loading Windows libraries") ;

	if(! AtlInitCommonControls( ICC_COOL_CLASSES | 
				ICC_BAR_CLASSES | 
				ICC_INTERNET_CLASSES | 
				ICC_USEREX_CLASSES | 
				ICC_WIN95_CLASSES | 
				ICC_NATIVEFNTCTL_CLASS |
				ICC_UPDOWN_CLASS))
	{
		logging::log_error("Failed to init common controls.") ;
	}


	LOG_DEBUG("Initializing COM server") ;
	// Initialize Felix module and ActiveX windowing
	// We need ActiveX because we embed the IE web browser
	COM_ENFORCE( _Module.Init(ObjectMap, hInstance, &LIBID_ATLLib), _T("Failed to initialize the module.") );
	TRUE_ENFORCE( FALSE != AtlAxWinInit(), _T("Call to AtlAxWinInit() failed!") ) ;

	bool bRun = true;
	bool bAutomation = false;
	bool bRegister = false ;
#ifdef _DEBUG
	bRegister = true ;
#endif

	// Check command line
	TCHAR szTokens[] = _T("-/");
	LPCTSTR lpszToken = _Module.FindOneOf(::GetCommandLine(), szTokens);
	while(lpszToken != NULL)
	{
		// unregister COM component
		if( ! _tcsicmp( lpszToken, _T("UnregServer") ) )
		{
			nRet = _Module.UnregisterServer();
			bRun = false;
			break;
		}
		// register COM component
		else if( ! _tcsicmp( lpszToken, _T("RegServer") )  )
		{
			bRegister = true ;
			bRun = false;
			break;
		}
		// automation (run as COM server)
		else if(! _tcsicmp( lpszToken, _T("Automation") ) ||
			! _tcsicmp( lpszToken, _T("Embedding") ) )
		{
			bAutomation = true;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}
	
	// During debugging, we go ahead and register the COM server every time.
#ifdef _DEBUG
	bRegister = true ;
#endif

	// register
	if (bRegister)
	{
		try
		{
			logging::log_debug("Registering Felix COM server") ;
			COM_ENFORCE( _Module.RegisterServer(TRUE), _T("Failed to register server") ) ;
			::MessageBeep( MB_ICONINFORMATION ) ;
		}
		catch (CComException &e)
		{
			logging::log_error("Failed to register COM server.") ;
			logging::log_exception(e) ;
			e.notify_user(L"Failed to register COM server.") ;
		}
	}
	// run as main window
	if(bRun)
	{
		try
		{
			// Copy over the HTML files if they aren't present. These are the templates
			// we use in our HTML views.
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
		// Create the main window and run the message loop.
		else
		{
			nRet = Run(lpstrCmdLine, nCmdShow);
		}
		
	}
	::Sleep(_Module.m_dwPause);
	_Module.RevokeClassObjects();
	_Module.Term();
	return nRet ;
}

/*! Ensures that OLE is initialized and unitialized.
*/
class COleManager
{
public:
	COleManager()
	{
		ATLVERIFY(SUCCEEDED(::OleInitialize(NULL)));
	}
	~COleManager()
	{
		::OleUninitialize() ;
	}
};

#ifdef UNIT_TEST


int do_unit_testing(HINSTANCE hInstance)
{
	try
	{
		ATLTRACE( "Unit testing app...\n" ) ;
		COM_ENFORCE( _Module.Init(ObjectMap, hInstance, &LIBID_ATLLib), _T("Failed to initialize the module.") );
		if(! _Module.set_library( _T("lang\\EngResource.dll") ))
		{
			logging::log_error("Failed to load English-language resource") ;
		}

		char *args[] = {"", "--report_level=detailed", "--result_code=yes", "--log_level=error"};
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
	}
	catch (CException& e)
	{
		e ;
	}
	catch(...)
	{
		ATLTRACE("whaaa?\n");
	}

	_Module.Term() ;
	return EXIT_SUCCESS ;

}
#endif
/*!
 * Our program's entry point.
 * 
 * Main entry point of the program, called by the real main (which lives in crt land).
 */
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	COleManager ole_manager ;

	// sets us to convert SEH into C++ exceptions,
	// and sets up float exceptions as well
	set_up_exception_handling() ;
	_set_invalid_parameter_handler((_invalid_parameter_handler)felix_invalid_parameter_handler);
	
	try
	{
		logging::log_debug("Launching Felix") ;
	}
	catch (_com_error& e)
	{
		// logging has failed somehow
		e ;
		ATLASSERT(FALSE && "Exception trying to do logging") ;
	}
	catch(...)
	{
		ATLASSERT(FALSE && "Exception trying to do logging") ;
	}


	logging::log_debug("Loading Scintilla library") ;
	WTL::ScintillaModule &scintilla_module = WTL::ScintillaModule::instance() ;
	ATLVERIFY(scintilla_module.IsLoaded()) ;

// For unit testing
#ifdef UNIT_TEST
	lpstrCmdLine ;
	nCmdShow ;
	do_unit_testing(hInstance) ;


#else
	// normal program execution (not unit testing)

	// going to a subroutine makes it cleaner to separate app code from error handling
	try
	{
		luawrapper::LuaState lua_state ;
		if (! lua_state.is_valid())
		{
			logging::log_error("Failed to initialize Lua VM.") ;
			logging::log_error(lua_state.get_string(-1)) ;
		}
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

	return EXIT_FAILURE ;

#endif
}

