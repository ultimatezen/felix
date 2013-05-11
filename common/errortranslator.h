/*!
	@file ErrorTranslator.h
	@brief interface for the error translation functions
	@author Ryan Ginstrom
 */

#pragma once

#include "Exceptions.h"
#include "File.h"
#include <new.h>
#include <float.h>	// _clearfp()

//////////////////////////////////////////
// Override _com_raise_error to get our own error types.
// FullName:  _com_raise_error
// Access:    public 
// Qualifier: 
// \return    void __stdcall
// \param     HRESULT hr
// \param     IErrorInfo* perrinfo
//////////////////////////////////////////
inline void __stdcall _com_raise_error(HRESULT hr, IErrorInfo* perrinfo ) 
{
	BANNER( "Raising COM error: _com_raise_error" ) ;

	_com_error error( hr, perrinfo ) ;

	throw except::CComException( TEXT("Error raised by COM"), error ) ;
}

//////////////////////////////////////////
//!  Convert exceptions raised by new into our exceptions.
// FullName:  NewHandler
// Access:    public 
// Qualifier: 
// \return    int _cdecl
// \param     size_t size
//////////////////////////////////////////
inline int _cdecl NewHandler( size_t size )
{
	throw except::CAllocException(size) ;
}

//////////////////////////////////////////
//!  TranslateSEHtoCE.
// FullName:  TranslateSEHtoCE
// Access:    public 
// Qualifier: 
// \return    void _cdecl
// \param     UINT code
// \param     PEXCEPTION_POINTERS pep
//////////////////////////////////////////
inline void _cdecl TranslateSEHtoCE( UINT code, PEXCEPTION_POINTERS pep )
{

#define CHECK_RECOVERABLE_CODE(x)			\
	case x:									\
		throw except::CSWException( TEXT("System-generated exception: " ) _T(#x) , code, pep ) ;

#define CHECK_FLOAT_EXCEPTION(x)		\
	case x:								\
	_clearfp() ;						\
	throw except::CFloatException( TEXT("System-generated exception: " ) _T(#x) , code, pep ) ;

#define CHECK_MELTDOWN_CODE(x) \
	case x: \
		MessageBox( \
			NULL, \
			TEXT("System-generated exception: " ) \
				_T( #x ) \
				_T("\rAn unrecoverable exception has occurred.")\
				_T("\rThe program must terminate now.\rPrepare for meltdown!"), \
			_T("Fatal Error"), \
			MB_OK | MB_ICONSTOP ) ; \
	throw TEXT("Unrecoverable exception!") ;

	switch ( code )
	{
	// =============================
	// recoverable errors
	// =============================

	CHECK_RECOVERABLE_CODE( WAIT_IO_COMPLETION )                  
	CHECK_RECOVERABLE_CODE( STILL_ACTIVE )                      
	// 	EXCEPTION_ACCESS_VIOLATION 
	// The thread tried to read from or write to a virtual address for which it does not have 
	// the appropriate access. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_ACCESS_VIOLATION )        
	//	EXCEPTION_DATATYPE_MISALIGNMENT 
	//	The thread tried to read or write data that is misaligned on hardware that does 
	//  not provide alignment. 
	//	For example, 16-bit values must be aligned on 2-byte boundaries; 
	//	32-bit values on 4-byte boundaries, and so on. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_DATATYPE_MISALIGNMENT )   
	// EXCEPTION_BREAKPOINT 
	// A breakpoint was encountered. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_BREAKPOINT )             
	//	EXCEPTION_SINGLE_STEP 
	//	A trace trap or other single-instruction mechanism signaled that one instruction has been executed. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_SINGLE_STEP )             
	// EXCEPTION_ARRAY_BOUNDS_EXCEEDED 
	// The thread tried to access an array element that is out of bounds and the underlying hardware 
	// supports bounds checking. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_ARRAY_BOUNDS_EXCEEDED )   
	
	//	EXCEPTION_FLT_DENORMAL_OPERAND 
	//	One of the operands in a floating-point operation is denormal. A denormal value is
	//	one that is too small to represent as a standard floating-point value. 
	CHECK_FLOAT_EXCEPTION( EXCEPTION_FLT_DENORMAL_OPERAND )   
	//	EXCEPTION_FLT_DIVIDE_BY_ZERO 
	//	The thread tried to divide a floating-point value by a floating-point divisor of zero. 
	CHECK_FLOAT_EXCEPTION( EXCEPTION_FLT_DIVIDE_BY_ZERO )     
	//	EXCEPTION_FLT_INEXACT_RESULT 
	//	The result of a floating-point operation cannot be represented exactly as a decimal fraction. 
	CHECK_FLOAT_EXCEPTION( EXCEPTION_FLT_INEXACT_RESULT )     
	//	EXCEPTION_FLT_INVALID_OPERATION 
	//	This exception represents any floating-point exception not included in this list. 
	CHECK_FLOAT_EXCEPTION( EXCEPTION_FLT_INVALID_OPERATION )  
	//	EXCEPTION_FLT_OVERFLOW 
	//	The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type. 
	CHECK_FLOAT_EXCEPTION( EXCEPTION_FLT_OVERFLOW )          
	//	EXCEPTION_FLT_STACK_CHECK 
	//	The stack overflowed or underflowed as the result of a floating-point operation. 
	CHECK_FLOAT_EXCEPTION( EXCEPTION_FLT_STACK_CHECK )       
	//	EXCEPTION_FLT_UNDERFLOW 
	//	The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type. 
	CHECK_FLOAT_EXCEPTION( EXCEPTION_FLT_UNDERFLOW )
	
	//	EXCEPTION_INT_DIVIDE_BY_ZERO 
	//	The thread tried to divide an integer value by an integer divisor of zero. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_INT_DIVIDE_BY_ZERO )      
	//	EXCEPTION_INT_OVERFLOW 
	//	The result of an integer operation caused a carry out of the most significant bit of the result. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_INT_OVERFLOW )         
	//	EXCEPTION_IN_PAGE_ERROR 
	//	The thread tried to access a page that was not present, 
	//	and the system was unable to load the page. 
	//	For example, this exception might occur if a network connection is lost while running a 
	//	program over the network. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_IN_PAGE_ERROR )         
	//	EXCEPTION_INVALID_DISPOSITION 
	//	An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_INVALID_DISPOSITION )     
	CHECK_RECOVERABLE_CODE( EXCEPTION_GUARD_PAGE )             
	CHECK_RECOVERABLE_CODE( EXCEPTION_INVALID_HANDLE )         
	CHECK_RECOVERABLE_CODE( CONTROL_C_EXIT )    

	//	EXCEPTION_PRIV_INSTRUCTION 
	//	The thread tried to execute an instruction whose operation is not allowed in the current machine mode. 
	CHECK_RECOVERABLE_CODE( EXCEPTION_PRIV_INSTRUCTION )      

	// =============================
	// non-recoverable errors
	// =============================

	//	EXCEPTION_ILLEGAL_INSTRUCTION 
	//	The thread tried to execute an invalid instruction. 
	CHECK_MELTDOWN_CODE( EXCEPTION_ILLEGAL_INSTRUCTION )     
	//	EXCEPTION_NONCONTINUABLE_EXCEPTION 
	//	The thread tried to continue execution after a noncontinuable exception occurred. 
	CHECK_MELTDOWN_CODE( EXCEPTION_NONCONTINUABLE_EXCEPTION )
	//	EXCEPTION_STACK_OVERFLOW 
	//	The thread used up its stack. 
	case EXCEPTION_STACK_OVERFLOW:
			throw TEXT("Stack Overflow!") ;


	default: // other exceptions are not recoverable
		MessageBox( NULL, _T("An unrecoverable exception has occurred.\rThe program must terminate now.\rPrepare for meltdown!"), 
				_T("Fatal Error"), MB_OK | MB_ICONSTOP ) ;

		throw TEXT("Unrecoverable exception!") ;

	} // switch ( code )
}

//////////////////////////////////////////
//!  Set up exception handling so it throws the types we expect.
// - Translate structured exceptions into C++ exceptions
// - Convert exceptions raised by new into our exception type
// - Throw exceptions for floating-point overflow, underflow, zero divide, denormal, and invalid
// FullName:  set_up_exception_handling
// Access:    public 
// Qualifier: 
// \return    void
//////////////////////////////////////////
inline void set_up_exception_handling()
{
	_set_se_translator( TranslateSEHtoCE ) ;

	_set_new_handler( NewHandler ) ;
	_set_new_mode(1) ;
}

