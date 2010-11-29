/*!
	@file FixedMatrix_test.cpp
	@brief Unit tests for FixedMatrix class
	@date 2005/06/02
	Time: 10:16:23
	@author Ryan Ginstrom
	@dir C:\Programming\MyLibrary
 */

#include "FixedMatrix.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG
    using namespace easyunit;
#endif // #ifdef _DEBUG


#ifdef _DEBUG


/** implementation of black-box test for init. */
TEST( FixedMatrix, init ) 
{
	typedef FixedMatrix< short, 10, 10 > matrix_type ;

	matrix_type matrix ;
	matrix.init( 5 ) ;

	for ( size_t i = 0 ; i < matrix.num_rows() ; ++ i )
	{
		for ( size_t j = 0 ; j < matrix.num_rows() ; ++ j )
		{
			ASSERT_TRUE ( 5 == matrix(i, j) ) ;
		}
	}
}

/** implementation of black-box test for operator. */
TEST( FixedMatrix, AssignmentOperator ) 
{
	ASSERT_TRUE ( test_GetVal_outofrange_rows() ) ;
	ASSERT_TRUE ( test_GetVal_outofrange_cols() ) ;
	ASSERT_TRUE ( test_GetVal_outofrange_both() ) ;

	typedef FixedMatrix< float, 10, 10 > matrix_type ;
	matrix_type matrix ;

	matrix( 3, 4 ) = 5.f ;
	const float x = matrix( 3, 4 ) ;
	ASSERT_TRUE ( FLOAT_EQ( 5.f, x ) ) ;

	matrix( 8, 2 ) = 100056.f ;
	float y = matrix( 8, 2 ) ;
	ASSERT_TRUE ( FLOAT_EQ( 100056.f, y ) ) ;

	matrix( 0, 0 ) = x ;
	y = matrix( 0, 0 ) ;
	ASSERT_TRUE ( FLOAT_EQ( x, y ) ) ;

}
/** sub-test for test_GetVal. 
@return bool success. 
*/
TEST( FixedMatrix, GetVal_outofrange_rows )
{
	typedef FixedMatrix< float, 10, 10 > matrix_type ;
	matrix_type matrix ;
	try
	{
		const float x = matrix( 10, 0 ) ;
		x ;
	}
	catch ( ... ) 
	{
		return ;
	}
}
/** sub-test for test_GetVal. 
@return bool success. 
*/
TEST( FixedMatrix, GetVal_outofrange_cols )
{
	typedef FixedMatrix< float, 10, 10 > matrix_type ;
	matrix_type matrix ;
	try
	{
		const float x = matrix( 0, 10 ) ;
		x ;
	}
	catch ( ... ) 
	{
		return ;
	}
	FAIL_M( "Expected exception not thrown" ) ;
}
/** sub-test for test_GetVal. 
@return bool success. 
*/
TEST( FixedMatrix, GetVal_outofrange_both )
{
	typedef FixedMatrix< float, 10, 10 > matrix_type ;
	matrix_type matrix ;
	try
	{
		const float x = matrix( 11, 11 ) ;
		x ;
	}
	catch ( ... ) 
	{
		return ;
	}
	FAIL_M( "Expected exception not thrown" ) ;
}

/** implementation of black-box test for num_rows( ). */
TEST( FixedMatrix, num_rows ) 
{
	FixedMatrix< std::exception, 3, 3 > matrix1 ;

	ASSERT_TRUE ( matrix1.num_rows() == 3 ) ;

	FixedMatrix< char, 106, 3 > matrix2 ;
	
	ASSERT_TRUE ( matrix2.num_rows() == 106 ) ;
	
	FixedMatrix< long, 37, 104 > matrix3 ;
	
	ASSERT_TRUE ( matrix3.num_rows() == 37 ) ;

	return true ; 
}

/** implementation of black-box test for num_cols( ). */
TEST( FixedMatrix, num_cols ) 
{
	FixedMatrix< std::exception, 3, 3 > matrix1 ;
	
	ASSERT_TRUE ( matrix1.num_cols() == 3 ) ;
	
	FixedMatrix< char, 106, 3 > matrix2 ;
	
	ASSERT_TRUE ( matrix2.num_cols() == 3 ) ;
	
	FixedMatrix< long, 37, 104 > matrix3 ;
	
	ASSERT_TRUE ( matrix3.num_cols() == 104 ) ;

	return true ; 
}


#endif // #ifdef _DEBUG
