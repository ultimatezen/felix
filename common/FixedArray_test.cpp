/*!
	@file FixedArray_test.cpp
	@brief Test FixedArray class
	@date 2005/06/01
	Time: 12:12:33
	@author Ryan Ginstrom
	@dir C:\Programming\MyLibrary
 */


#include "StdAfx.h"
#include "FixedArray.h"
#include "dbc.h"
#include "memory_debugging.h"
#include <exception>
#include <iterator> // advance

#include "easyunit/testharness.h"


#ifdef _DEBUG

namespace easyunit
{
	/************************************************************************/
	/* Unit Tests for FixedArray                                            */
	/************************************************************************/

	TEST( FixedArray, max_size )
	{
		FixedArray< char, 100 > c_array ;
		ASSERT_TRUE ( c_array.max_size() == 100 ) ; 

		FixedArray< long, 999 > l_array ;
		ASSERT_TRUE( l_array.max_size() == 999 ) ;
	}

	TEST( FixedArray, size )
	{
		FixedArray< char, 100 > c_array ;
		ASSERT_TRUE( c_array.size() == 100 ) ;

		FixedArray< long, 1000 > l_array ;
		ASSERT_TRUE( l_array.size() == 1000 ) ;
	} 

	TEST( FixedArray, SquareBracketsOperator )
	{
		// set up the array
		typedef FixedArray< wchar_t, 50 > array_type ;

		array_type array ;
		for ( size_t i = 0 ; i < array.size() ; ++i )
		{
			array[i] = wchar_t( L'a' + i ) ;
		}
		const wchar_t c = array[10] ;
		ASSERT_TRUE( array[10] == c ) ;

		for ( size_t i = 0 ; i < array.size() ; ++i )
		{
			ASSERT_TRUE( array[i] == wchar_t( L'a' + i ) ) ;
		}

	} 

	TEST( FixedArray, end )
	{
		// set up the array
		typedef FixedArray< short, 10 > array_type ;

		array_type array ;
		for ( size_t i = 0 ; i < array.size() ; ++i )
		{
			array[i] = static_cast< short >( i ) ;
		}

		array_type::const_iterator cpos ;
		cpos = array.end() ;
		--cpos ;
		ASSERT_TRUE( *cpos == array[9] ) ;
		array_type::const_iterator cpos_start ;
		cpos_start = array.begin() ;
		std::advance( cpos_start, 9 ) ;
		ASSERT_TRUE( cpos_start == cpos ) ;

		array_type::iterator pos ;
		pos = array.end() ;
		--pos ;
		ASSERT_TRUE( *pos == array[9] ) ;
		FixedArray< short, 10 >::iterator pos_start ;
		pos_start = array.begin() ;
		std::advance( pos_start, 9 ) ;
		ASSERT_TRUE( pos_start == pos ) ;

		*pos = 25 ;
		ASSERT_TRUE( *pos == 25 ) ;
		ASSERT_TRUE( 25 == array[9] ) ;
	}

	TEST( FixedArray, begin )
	{
		// set up the array
		typedef FixedArray< float, 10 > array_type ;

		array_type array ;
		for ( size_t i = 0 ; i < array.size() ; ++i )
		{
			array[i] = static_cast< float >( i ) ;
		}

		// test const_iterator
		array_type::const_iterator cpos ;
		cpos = array.begin() ;

		ASSERT_TRUE( *cpos == array[0] ) ;
		++cpos ;
		ASSERT_TRUE( *cpos == array[1] ) ;
		std::advance( cpos, 1 ) ;
		ASSERT_TRUE( *cpos == array[2] ) ;
		std::advance( cpos, 1 ) ;
		ASSERT_TRUE( *cpos == array[3] ) ;
		std::advance( cpos, 1 ) ;
		ASSERT_TRUE( *cpos == array[4] ) ;
		std::advance( cpos, 1 ) ;
		ASSERT_TRUE( *cpos == array[5] ) ;
		std::advance( cpos, 1 ) ;
		ASSERT_TRUE( *cpos == array[6] ) ;
		std::advance( cpos, 1 ) ;
		ASSERT_TRUE( *cpos == array[7] ) ;
		++cpos ;
		ASSERT_TRUE( *cpos == array[8] ) ;
		++cpos ;
		ASSERT_TRUE( *cpos == array[9] ) ;
		++cpos ;
		ASSERT_TRUE( cpos == array.end() ) ;
		--cpos ;
		ASSERT_TRUE( *cpos == array[9] ) ;

		// test iterator
		array_type::iterator pos ;
		pos = array.begin() ;
		ASSERT_TRUE( *pos == array[0] ) ;
		++pos ;
		ASSERT_TRUE( *pos == array[1] ) ;
		std::advance( pos, 1 ) ;
		ASSERT_TRUE( *pos == array[2] ) ;
		std::advance( pos, 1 ) ;
		ASSERT_TRUE( *pos == array[3] ) ;
		std::advance( pos, 1 ) ;
		ASSERT_TRUE( *pos == array[4] ) ;
		std::advance( pos, 1 ) ;
		ASSERT_TRUE( *pos == array[5] ) ;
		std::advance( pos, 1 ) ;
		ASSERT_TRUE( *pos == array[6] ) ;
		std::advance( pos, 1 ) ;
		ASSERT_TRUE( *pos == array[7] ) ;
		++pos ;
		ASSERT_TRUE( *pos == array[8] ) ;
		++pos ;
		ASSERT_TRUE( *pos == array[9] ) ;
		++pos ;
		ASSERT_TRUE( pos == array.end() ) ;
		--pos ;
		ASSERT_TRUE( *pos == array[9] ) ;

		pos = array.begin() ;
		*pos = 25.f ;
		ASSERT_TRUE( FLOAT_EQ( *pos, 25.f ) ) ;
		ASSERT_TRUE( FLOAT_EQ( 25.f, array[0] ) ) ;
	} 

	TEST( FixedArray, init )
	{
		typedef FixedArray< int, 49 > array_type ;
		array_type array ;
		array.init( 12 ) ;

		for ( size_t i = 0 ; i < array.size() ; ++i )
		{
			ASSERT_TRUE( array[i] == 12 ) ;
		}
	} 



	TEST( FixedArray, copy_algo )
	{
		typedef FixedArray< size_t, 10 > array_type ;

		array_type array ;
		for ( size_t i = 0 ; i < array.size() ; ++i )
		{
			array[i] = static_cast< size_t >( i ) ;
		}
		array_type cpy_array ;
		std::copy( array.begin(), array.end(), cpy_array.begin() ) ;

		array_type::iterator pos1 = array.begin() ;
		array_type::iterator pos2 = cpy_array.begin() ;

		while ( pos1 != array.end( ) ) 
		{
			ASSERT_TRUE( pos2 != cpy_array.end() ) ;

			ASSERT_TRUE( *pos1 == *pos2 ) ;

			++pos1 ;
			++pos2 ;
		}
	}

}


#endif // #ifdef _DEBUG
