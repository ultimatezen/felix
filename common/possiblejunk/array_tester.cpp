/*!
	@file array_tester.cpp
	@brief tester for Array class
	@date 2004/12/29
	Time: 23:22:06
	@author Ryan Ginstrom
 */
#include "array_tester.h"

#ifdef _DEBUG

#include "memory_debugging.h"

#include "dbc.h"
#include <exception>

#ifndef _DbC_
#error _DbC_ must be defined
#endif
/** Class to conduct black-box unit testing of Array. */ 
class Array_tester
{ 
public:

	/* Run the suite of black-box tests for Array. */
	void run_test_suite() ; 

private:
	/** white-box tests */
	void run_white_box_tests() ;

	void test_resize() ;

	/** black-box test for begin(). */
	void test_begin( ) ; 
	/** black-box test for end(). */
	void test_end( ) ; 
	/** black-box test for size(). */
	void test_size( ) ; 
	/** black-box test for max_size(). */
	void test_max_size( ) ; 
	/** black-box test for operator(). */
	void test_operator( ) ; 
	/** black-box test for init(). */
	void test_init( ) ; 
	/** black-box test for clear(). */
	void test_clear( ) ; 
	/** black-box test for operator = (). */
	void test_operator_equal ( ) ; 
	/** black-box test for clone(). */
	void test_clone( ) ; 
	/** black-box test for val_identity(). */
	void test_val_identity( ) ; 
	/** black-box test for swap(). */
	void test_swap( ) ; 
	/** black-box test for Array(). */
	void test_Array( ) ; 
	/** black-box test for Array(). */
} ; 

/** Implementation of black-box test suite routine. */ 
void Array_tester::run_test_suite( )
{
	run_white_box_tests() ;
	test_resize() ;
	test_begin( ) ; 
	test_end( ) ; 
	test_size( ) ; 
	test_max_size( ) ; 
	test_operator( ) ; 
	test_init( ) ; 
	test_clear( ) ; 
	test_operator_equal ( ) ; 
	test_clone( ) ; 
	test_val_identity( ) ; 
	test_swap( ) ; 
	test_Array( ) ; 

}

void Array_tester::test_resize()
{
	Array< char > array1 ;

	_ASSERTE( array1.size() == 0 ) ;
	_ASSERTE( array1.max_size() == 0 ) ;

	_ASSERTE( array1.resize( 10 ) == true ) ;

	_ASSERTE( array1.size() == 10 ) ;
	_ASSERTE( array1.max_size() == 10 ) ;

	_ASSERTE( array1.resize( 100 ) == true ) ;

	_ASSERTE( array1.size() == 100 ) ;
	_ASSERTE( array1.max_size() == 100 ) ;
}

/** implementation of white-box tests for Array. 
*/
void Array_tester::run_white_box_tests()
{
	Array< char > array1 ;
	array1.run_test_suite() ;

	Array< wchar_t > array2 ;
	array2.run_test_suite() ;
}


/** implementation of black-box test for begin. */
void Array_tester::test_begin( ) 
{
	typedef Array< short > array_type ;
	array_type array ;

	_ASSERTE( array.begin() == array.end() ) ;

	array.resize(10) ;
	array(0) = 5 ;
	array(1) = 4 ;

	array_type::iterator pos = array.begin() ;
	_ASSERTE( *pos == array[0] ) ;
	++pos ;
	_ASSERTE( *pos == array[1] ) ;
	*pos = 25 ;
	_ASSERTE( 25 == array[1] ) ;

	array_type::const_iterator cpos = array.begin() ;

	std::advance( cpos, 1 ) ;
	_ASSERTE( cpos == pos ) ;
	_ASSERTE( *cpos == *pos ) ;
	_ASSERTE( *cpos == array[1] ) ;
	_ASSERTE( *cpos == 25 ) ;

	cpos = array.begin() ;
	std::advance( cpos, array.size() ) ;
	_ASSERTE( cpos == array.end() ) ;
}

/** implementation of black-box test for end. */
void Array_tester::test_end( ) 
{
	typedef Array< double > array_type ;
	array_type array ;

	_ASSERTE( array.begin() == array.end() ) ;

	array.resize(10) ;
	array(8) = 5.f ;
	array(9) = 4.f ;

	array_type::iterator pos = array.end() ;
	--pos ;
	_ASSERTE( FLOAT_EQ( *pos, array[9] ) ) ;
	--pos ;
	_ASSERTE( FLOAT_EQ( *pos, array[8] )) ;
	*pos = 25.f ;
	_ASSERTE( FLOAT_EQ( 25.f, array[8] ) ) ;

	array_type::const_iterator cpos = array.end() ;

	--cpos ;
	_ASSERTE( FLOAT_EQ( *cpos, array[9] ) ) ;
	--cpos ;
	_ASSERTE( FLOAT_EQ( *cpos, array[8] )) ;
	*pos = 25.f ;
	cpos = pos ;
	_ASSERTE( FLOAT_EQ( 25.f, *cpos ) ) ;
}

/** implementation of black-box test for size. */
void Array_tester::test_size( ) 
{
	typedef Array< char > array_type ;
	array_type array ;

	_ASSERTE( array.size() == 0 ) ;
	array.resize( 10 ) ;
	_ASSERTE( array.size() == 10 ) ;
	array.resize( 100 ) ;
	_ASSERTE( array.size() == 100 ) ;
	array.resize( 10 ) ;
	_ASSERTE( array.size() == 10 ) ;
	array.resize( 1 ) ;
	_ASSERTE( array.size() == 1 ) ;
}

/** implementation of black-box test for max_size. */
void Array_tester::test_max_size( ) 
{
	typedef Array< wchar_t > array_type ;
	array_type array ;

	_ASSERTE( array.max_size() == 0 ) ;
	array.resize( 10 ) ;
	_ASSERTE( array.max_size() == 10 ) ;
	array.resize( 100 ) ;
	_ASSERTE( array.max_size() == 100 ) ;
	array.resize( 10 ) ;
	_ASSERTE( array.max_size() == 100 ) ;
	array.resize( 1 ) ;
	_ASSERTE( array.max_size() == 100 ) ;
	array.resize( 1000 ) ;
	_ASSERTE( array.max_size() == 1000 ) ;
	array.resize( 10 ) ;
	_ASSERTE( array.max_size() == 1000 ) ;
}

/** implementation of black-box test for operator. */
void Array_tester::test_operator( ) 
{
	typedef Array< short > array_type ;
	array_type array ;

	_ASSERTE( array.max_size() == 0 ) ;
	array.resize( 10 ) ;

	array(0) = 3 ;
	_ASSERTE( array(0) == 3 ) ;
	_ASSERTE( array(0) == array[0] ) ;

	array(1) = 10 ;
	_ASSERTE( array(1) == 10 ) ;
	_ASSERTE( array(1) == array[1] ) ;

	try
	{
		array(11) = 4 ;
	}
	catch( std::exception &e )
	{
		e ;
		try
		{
			short x = 0 ;
			x = array[10] ;
		}
		catch ( std::exception &e2 ) 
		{
			e2 ;
			return ;
		}
		_ASSERTE( false && "Expected error not thrown!" ) ;
	}
	_ASSERTE( false && "Expected error not thrown!" ) ;
}

/** implementation of black-box test for init. */
void Array_tester::test_init( ) 
{
	typedef Array< short > array_type ;
	array_type array ;

	array.resize( 15 ) ;
	array.init(35) ;

	for ( size_t i = 0u ; i < array.size() ; ++i )
	{
		_ASSERTE( array[i] == 35 ) ;
	}

	array.resize( 10 ) ;
	array.init(43) ;

	for ( i = 0 ; i < array.size() ; ++i )
	{
		_ASSERTE( array[i] == 43 ) ;
	}

	array.resize( 100 ) ;
	array.init(4334) ;

	for ( i = 0 ; i < array.size() ; ++i )
	{
		_ASSERTE( array[i] == 4334 ) ;
	}

}

/** implementation of black-box test for clear. */
void Array_tester::test_clear( ) 
{
	typedef Array< short > array_type ;
	array_type array ;

	array.resize( 15 ) ;
	_ASSERTE( array.begin() != array.end() ) ;
	_ASSERTE( array.size() == 15 ) ;
	_ASSERTE( array.max_size() == 15 ) ;

	array.init(35) ;

	array.clear() ;
	_ASSERTE( array.begin() == array.end() ) ;
	_ASSERTE( array.size() == 0 ) ;
	_ASSERTE( array.max_size() == 0 ) ;
}

/** implementation of black-box test for operator = . */
void Array_tester::test_operator_equal ( ) 
{
	typedef Array< short > array_type ;
	array_type array1, array2 ;

	array1.resize( 15 ) ;
	array1.init(35) ;

	array2 = array1 ;

	_ASSERTE( array1.size() == array2.size() ) ;
	_ASSERTE( array1.max_size() == array2.max_size() ) ;

	for ( size_t i = 0u ; i < array1.size() ; ++i )
	{
		_ASSERTE( array2[i] == array1[i] ) ;
	}
	_ASSERTE( array1.val_identity(array2) ) ;

	array_type array3 ;
	array3.resize( 10 ) ;
	array3.init(13) ;

	array2 = array3 ;

	_ASSERTE( array3.size() == array2.size() ) ;
	_ASSERTE( array3.max_size() == array2.max_size() ) ;

	for ( i = 0 ; i < array3.size() ; ++i )
	{
		_ASSERTE( array2[i] == array3[i] ) ;
	}
	_ASSERTE( array3.val_identity(array2) ) ;

	_ASSERTE( array1.size() != array2.size() ) ;
	_ASSERTE( array1.max_size() != array2.max_size() ) ;

	for ( i = 0 ; i < array1.size() && i < array2.size() ; ++i )
	{
		_ASSERTE( array2[i] != array1[i] ) ;
	}

}

/** implementation of black-box test for clone. */
void Array_tester::test_clone( ) 
{
	typedef Array< short > array_type ;
	array_type array1, array2 ;

	array1.resize( 15 ) ;
	array1.init(35) ;

	array1.clone( array2 ) ;

	_ASSERTE( array1.size() == array2.size() ) ;
	_ASSERTE( array1.size() == array1.max_size() ) ;
	_ASSERTE( array1.max_size() == array2.max_size() ) ;

	for ( size_t i = 0u ; i < array1.size() ; ++i )
	{
		_ASSERTE( array2[i] == array1[i] ) ;
	}

	array_type array3 ;
	array3.resize( 10 ) ;
	array3.init(13) ;

	array3.clone( array2 ) ;

	_ASSERTE( array3.size() == array2.size() ) ;
	_ASSERTE( array3.max_size() != array2.max_size() ) ;
	_ASSERTE( array1.max_size() == array2.max_size() ) ;

	for ( i = 0 ; i < array3.size() ; ++i )
	{
		_ASSERTE( array2[i] == array3[i] ) ;
	}

	_ASSERTE( array1.size() != array2.size() ) ;
	_ASSERTE( array1.max_size() == array2.max_size() ) ;

	for ( i = 0 ; i < array1.size() && i < array2.size() ; ++i )
	{
		_ASSERTE( array2[i] != array1[i] ) ;
	}
}

/** implementation of black-box test for val_identity. */
void Array_tester::test_val_identity( ) 
{
	typedef Array< short > array_type ;
	array_type array1, array2 ;

	array1.resize( 15 ) ;
	array1.init(35) ;

	array2 = array1 ;

	_ASSERTE( array1.size() == array2.size() ) ;
	_ASSERTE( array1.max_size() == array2.max_size() ) ;

	for ( size_t i = 0u ; i < array1.size() ; ++i )
	{
		_ASSERTE( array2[i] == array1[i] ) ;
	}
	_ASSERTE( array1.val_identity(array2) ) ;
	_ASSERTE( array2.val_identity(array1) ) ;

	array2(14) = 1003 ;

	_ASSERTE( ! array1.val_identity(array2) ) ;
	_ASSERTE( ! array2.val_identity(array1) ) ;

	array2.clear() ;
	array2.resize(5) ;
	array2.init(15) ;

	array2.clone(array1) ;
	_ASSERTE( array1.val_identity(array2) ) ;
	_ASSERTE( array2.val_identity(array1) ) ;
	_ASSERTE( array1.size() == array2.size() ) ;
	_ASSERTE( array1.max_size() != array2.max_size() ) ;
}

/** implementation of black-box test for swap. */
void Array_tester::test_swap( ) 
{
	typedef Array< short > array_type ;
	array_type array1, array2 ;
	array_type array3, array4 ;

	array1.resize(5) ;
	array1.init(10) ;

	array2 = array1 ;
	_ASSERTE( array1.val_identity(array2) ) ;
	_ASSERTE( array2.val_identity(array1) ) ;

	array3.resize(15) ;
	array3.init(10) ;

	array4 = array3 ;

	_ASSERTE( array3.val_identity(array4) ) ;
	_ASSERTE( array4.val_identity(array3) ) ;

	array1.swap(array3) ;

	_ASSERTE( array1.val_identity(array4) ) ;
	_ASSERTE( array4.val_identity(array1) ) ;
	_ASSERTE( ! array1.val_identity(array2) ) ;
	_ASSERTE( ! array2.val_identity(array1) ) ;

	_ASSERTE( array1.size() == array4.size() ) ;
	_ASSERTE( array1.max_size() == array4.max_size() ) ;

	_ASSERTE( array3.val_identity(array2) ) ;
	_ASSERTE( array2.val_identity(array3) ) ;
	_ASSERTE( ! array3.val_identity(array4) ) ;
	_ASSERTE( ! array4.val_identity(array3) ) ;

	_ASSERTE( array3.size() == array2.size() ) ;
	_ASSERTE( array3.max_size() == array2.max_size() ) ;

}

/** implementation of black-box test for Array.
no requirement for number of elements to be greater than 0.	
*/
void Array_tester::test_Array( ) 
{
	typedef Array< short > array_type ;
	array_type array1(10) ;
	_ASSERTE( array1.size() == 10 ) ;
	_ASSERTE( array1.max_size() == 10 ) ;

	array1.init(10) ;

	array_type array2( array1 ) ;
	_ASSERTE( array1.val_identity(array2) ) ;
	_ASSERTE( array2.val_identity(array1) ) ;
	_ASSERTE( array1.size() == array2.size() ) ;
	_ASSERTE( array1.max_size() == array2.max_size() ) ;

	array1.resize(5) ;

	array_type array3( array1 ) ;
	_ASSERTE( array1.val_identity(array3) ) ;
	_ASSERTE( array3.val_identity(array1) ) ;
	_ASSERTE( array1.size() == array3.size() ) ;
	_ASSERTE( array1.max_size() == array3.max_size() ) ;

}

void test_Array()
{
	Array_tester tester_object ;
	tester_object.run_test_suite() ;

}

#endif // #ifdef _DEBUG
