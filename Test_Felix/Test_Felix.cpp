// Test_Felix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ErrorTranslator.h"
#include "Exceptions.h"

#define BOOST_TEST_MODULE FelixTests
#include <boost/test/unit_test.hpp>

// Our customized version of the server app module, to enable
// the resource DLL to be loaded dynamically (according to language)
CLocalizedServerAppModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

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
	throw except::CProgramException(err_msg) ;
}

struct FelixTestConfig {
	FelixTestConfig()   
	{ 
		::OleInitialize(NULL);

		// sets us to convert SEH into C++ exceptions,
		// and sets up float exceptions as well
		set_up_exception_handling() ;
		_invalid_parameter_handler oldHandler;
		oldHandler = _set_invalid_parameter_handler((_invalid_parameter_handler)felix_invalid_parameter_handler);
		_Module.Init(ObjectMap, GetModuleHandle(NULL), &LIBID_ATLLib);
		_Module.set_library( _T("lang\\EngResource.dll") ) ;
	}
	~FelixTestConfig()  
	{
		_Module.Term() ;
		::OleUninitialize() ;
	}
};

//____________________________________________________________________________//

BOOST_GLOBAL_FIXTURE( FelixTestConfig );

