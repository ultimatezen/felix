/*!
	@brief Tests the Array class
 */

#include "StdAfx.h"
#include "Array.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( ArrayTests )

	/** @test begin() method of class Array.
	 */
	BOOST_AUTO_TEST_CASE( begin )
	{
		typedef Array< short > array_type ;
		array_type a ;
		BOOST_CHECK_EQUAL ( a.begin() , a.end() ) ; 
		a.resize( 10 ) ;
		std::fill(a.begin(), a.end(), 0) ;
		a(0) = 5 ;
		a(1) = 25 ;
		BOOST_CHECK ( a.begin() != a.end() ) ;

		array_type::iterator pos = a.begin() ;
		BOOST_CHECK_EQUAL( 5 , *pos ) ; 

		array_type::const_iterator cpos = a.begin() ;

		std::advance( cpos, 1 ) ;
		BOOST_CHECK_EQUAL( cpos , ++pos ) ;
		BOOST_CHECK_EQUAL( *cpos , *pos ) ;
		BOOST_CHECK_EQUAL( 25, *cpos) ;

		cpos = a.begin() ;
		std::advance( cpos, a.size() ) ;
		BOOST_CHECK_EQUAL( cpos , a.end() ) ;

	}

	/** @test end() method of class Array.
	*/
	BOOST_AUTO_TEST_CASE( end )
	{
		typedef Array< double > array_type ;

		array_type a ;
		BOOST_CHECK_EQUAL ( a.begin() , a.end() ) ; 
		a.resize( 10 ) ;
		a.init(0.0) ;
		BOOST_CHECK_EQUAL ( a.begin() + 10 , a.end() ) ;

		a(8) = 5.f ;
		a(9) = 4.f ;

		array_type::iterator pos = a.end() ;
		--pos ;
		BOOST_CHECK( FLOAT_EQ( *pos, a[9] ) ) ;
		--pos ;
		BOOST_CHECK( FLOAT_EQ( *pos, a[8] )) ;
		*pos = 25.f ;
		BOOST_CHECK( FLOAT_EQ( 25.f, a[8] ) ) ;

		array_type::const_iterator cpos = a.end() ;

		--cpos ;
		BOOST_CHECK( FLOAT_EQ( *cpos, a[9] ) ) ;
		--cpos ;
		BOOST_CHECK( FLOAT_EQ( *cpos, a[8] )) ;
		*pos = 25.f ;
		cpos = pos ;
		BOOST_CHECK( FLOAT_EQ( 25.f, *cpos ) ) ;
	 }


	/** @test size() method of class Array.
	*/
	BOOST_AUTO_TEST_CASE( size )
	{
		Array< double > a ;
		BOOST_CHECK_EQUAL ( a.size() , 0u ) ; 
		a.resize( 5 ) ;
		BOOST_CHECK_EQUAL ( a.size() , 5u ) ;
	}

	/** @test max_size() method of class Array.
	*/
	BOOST_AUTO_TEST_CASE( max_size )
	{
		Array< double > a ;
		BOOST_CHECK_EQUAL ( a.max_size() , 0u ) ; 
		a.resize( 10 ) ;
		BOOST_CHECK_EQUAL ( a.max_size() , 10u ) ; 
		a.resize( 5 ) ;
		BOOST_CHECK_EQUAL ( a.max_size() , 10u ) ; 
	}

	/** @test clear() method of class Array.
	*/
	BOOST_AUTO_TEST_CASE( clear )
	{
		Array< double > a ;

		a.resize( 10 ) ;
		BOOST_CHECK_EQUAL ( a.size() , 10u ) ; 
		BOOST_CHECK_EQUAL ( a.max_size() , 10u ) ; 

		a.clear( ) ;
		BOOST_CHECK_EQUAL ( a.size() , 0u ) ; 
		BOOST_CHECK_EQUAL ( a.max_size() , 0u ) ; 
		BOOST_CHECK_EQUAL ( a.empty() , true ) ;
		BOOST_CHECK_EQUAL ( a.begin() , a.end() ) ; 
	}

	/** @test resize() method of class Array.
	*/
	BOOST_AUTO_TEST_CASE( resize )
	{
		Array< long > a(10) ;

		std::fill(a.begin(), a.end(), 5) ;
		a.resize( 5 ) ;
		BOOST_CHECK_EQUAL( 5, (int)a.size()) ; 
		BOOST_CHECK_EQUAL( 10, (int)a.max_size()) ; 
		BOOST_CHECK_EQUAL( a[3] , 5 ) ; 
	
		a.resize( 20 ) ;
		BOOST_CHECK_EQUAL( 20, (int)a.size()) ; 
		BOOST_CHECK_EQUAL( 20, (int)a.max_size()) ; 
		BOOST_CHECK_EQUAL( a[0] , 5 ) ; 

	}

	/** @test ParensOperator of class Array.
	*/
	BOOST_AUTO_TEST_CASE( ParensOperator )
	{
		typedef Array< short > array_type ;
		array_type a ;

		BOOST_CHECK_EQUAL( 0u, a.max_size()) ;
		a.resize( 10 ) ;

		a(0) = 3 ;
		BOOST_CHECK_EQUAL( a(0) , 3 ) ;

		a(1) = 10 ;
		BOOST_CHECK_EQUAL( a(1) , 10 ) ;
	}
	BOOST_AUTO_TEST_CASE( ParensOperator_throws_on_oob )
	{
		typedef Array< short > array_type ;
		array_type a ;

		BOOST_CHECK_EQUAL( 0u, a.max_size()) ;

		try
		{
			a(2) = 4 ;
			BOOST_FAIL( "Should have thrown when referencing out of bounds index" ) ;
		}
		catch( std::out_of_range& )
		{
			BOOST_CHECK_EQUAL( 0u, a.max_size()) ;
		}
	}
	BOOST_AUTO_TEST_CASE( BracketOperator_throws_on_oob )
	{
		typedef Array< short > array_type ;
		array_type a ;

		BOOST_CHECK_EQUAL( 0u, a.max_size()) ;
		a.resize( 2 ) ;

		try
		{
			const short i = a[2] ;
			i ;
			BOOST_FAIL( "Should have thrown when referencing out of bounds index" ) ;
		}
		catch( std::out_of_range& )
		{
			BOOST_CHECK_EQUAL( 2u, a.max_size()) ;
		}
	}

	/** @test init() method of class Array.
	*/
	BOOST_AUTO_TEST_CASE( init )
	{
		typedef Array< short > array_type ;
		array_type a ;

		a.resize( 15 ) ;
		a.init(35) ;

		FOREACH(short i, a)
		{
			BOOST_CHECK_EQUAL(35, i) ;
		}
	}

	/** @test OperatorEqual method of class Array.
	*/
	BOOST_AUTO_TEST_CASE( OperatorEqual )
	{
		typedef Array< short > array_type ;
		array_type array1, array2 ;

		array1.resize( 15 ) ;
		array1.init(35) ;

		array2 = array1 ;

		BOOST_CHECK_EQUAL( array1.size() , array2.size() ) ;
		BOOST_CHECK_EQUAL( array1.max_size() , array2.max_size() ) ;

		for ( size_t i = 0u ; i < array1.size() ; ++i )
		{
			BOOST_CHECK_EQUAL( array2[i] , array1[i] ) ;
		}
		BOOST_CHECK( array1.val_identity(array2) ) ;
	}

	/** @test implementation of black-box test for clone. */
	BOOST_AUTO_TEST_CASE( clone )
	{
		typedef Array< short > array_type ;
		array_type a, b ;

		a.resize( 15 ) ;
		a.init(35) ;

		a.clone( b ) ;

		BOOST_CHECK(a.val_identity(b)) ;
	}


	/** @test implementation of black-box test for val_identity. */
	BOOST_AUTO_TEST_CASE( val_identity )
	{
		typedef Array< short > array_type ;
		array_type array1, array2 ;

		array1.resize( 15 ) ;
		array1.init(35) ;

		array2 = array1 ;

		BOOST_CHECK_EQUAL( (int)array1.size() , (int)array2.size() ) ;
		BOOST_CHECK_EQUAL( (int)array1.max_size() , (int)array2.max_size() ) ;

		BOOST_CHECK( array1.val_identity(array2) ) ;
		BOOST_CHECK( array2.val_identity(array1) ) ;
	}
	BOOST_AUTO_TEST_CASE( val_identity_self )
	{
		typedef Array< short > array_type ;
		array_type a(5) ;

		a.init(3) ;

		BOOST_CHECK(a.val_identity(a)) ;
	}
	BOOST_AUTO_TEST_CASE( val_identity_false_val )
	{
		typedef Array< short > array_type ;
		array_type a(5), b(5) ;

		a.init(3) ;
		b.init(4) ;

		BOOST_CHECK(! a.val_identity(b)) ;
		BOOST_CHECK(! b.val_identity(a)) ;
	}
	BOOST_AUTO_TEST_CASE( val_identity_false_size )
	{
		typedef Array< short > array_type ;
		array_type a(5), b(7) ;

		a.init(0) ;
		b.init(0) ;

		BOOST_CHECK(! a.val_identity(b)) ;
		BOOST_CHECK(! b.val_identity(a)) ;
	}


	/** @test swap() method of class Array.
	*/
	BOOST_AUTO_TEST_CASE( swap )
	{
		typedef Array< short > array_type ;
		array_type a, b ;

		a.resize(5) ;
		a.init(10) ;

		b.swap(a) ;

		BOOST_CHECK_EQUAL(0u, a.size()) ;
		BOOST_CHECK_EQUAL(5, (int)b.size()) ;

		BOOST_CHECK_EQUAL(0u, a.max_size()) ;
		BOOST_CHECK_EQUAL(5, (int)b.max_size()) ;

		FOREACH(short i, b)
		{
			BOOST_CHECK_EQUAL(10, i) ;
		}
	}

	/** @test implementation of black-box test for Array.
	no requirement for number of elements to be greater than 0.	
	*/
	BOOST_AUTO_TEST_CASE( Constructor )
	{
		typedef Array< short > array_type ;
		array_type array1(10) ;
		BOOST_CHECK_EQUAL( array1.size() , 10u ) ;
		BOOST_CHECK_EQUAL( array1.max_size() , 10u ) ;

		array1.init(10) ;

		array_type array2( array1 ) ;
		BOOST_CHECK( array1.val_identity(array2) ) ;
		BOOST_CHECK( array2.val_identity(array1) ) ;
		BOOST_CHECK_EQUAL( array1.size() , array2.size() ) ;
		BOOST_CHECK_EQUAL( array1.max_size() , array2.max_size() ) ;

		array1.resize(5) ;

		array_type array3( array1 ) ;
		BOOST_CHECK( array1.val_identity(array3) ) ;
		BOOST_CHECK( array3.val_identity(array1) ) ;
		BOOST_CHECK_EQUAL( array1.size() , array3.size() ) ;
		BOOST_CHECK_EQUAL( array1.max_size() , array3.max_size() ) ;
	}
	BOOST_AUTO_TEST_CASE( assignment_to_self )
	{
		typedef Array< int > array_type ;
		array_type a(5) ;
		a.init(10) ;

		a = a ;
		FOREACH(int i, a)
		{
			BOOST_CHECK_EQUAL(10, i) ;
		}
	}
	BOOST_AUTO_TEST_CASE( assignment_to_other)
	{
		typedef Array< int > array_type ;
		array_type a ;
		array_type b(10) ;
		b.init(10) ;

		a = b ;
		BOOST_CHECK(b.val_identity(a)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
