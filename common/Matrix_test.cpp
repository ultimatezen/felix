// ***************************************************************
//  \brief Unit tests for Matrix class
//  -------------------------------------------------------------
//  Matrix_test   version:  1.0   |  date: 07/09/2006
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include "stdafx.h"
#include "Matrix.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{

	/** @test implementation of black-box test for num_rows. */
	TEST( Matrix, num_rows ) 
	{
		Matrix< int > matrix ;
		ASSERT_TRUE ( matrix.num_rows() == 0 ) ;

		matrix.resize(10, 15) ;
		ASSERT_TRUE ( matrix.num_rows() == 10 ) ;

		matrix.resize(1,100) ;
		ASSERT_TRUE  ( matrix.num_rows() == 1 ) ; 

		Matrix< short > matrix2( 10, 10 ) ;
		ASSERT_TRUE ( matrix2.num_rows() == 10 ) ;

	}

	/** @test implementation of black-box test for num_cols. */
	TEST( Matrix, num_cols ) 
	{
		Matrix< short > matrix ;
		ASSERT_TRUE ( matrix.num_cols() == 0 ) ;

		matrix.resize(10, 15) ;
		ASSERT_TRUE ( matrix.num_cols() == 15 ) ;

		matrix.resize(1,100) ;
		ASSERT_TRUE  ( matrix.num_cols() == 100 ) ; 

		Matrix< short > matrix2( 10, 10 ) ;
		ASSERT_TRUE ( matrix2.num_cols() == 10 ) ;

	}

	/** @test implementation of black-box test for operator. */
	TEST( Matrix, OperatorEquals ) 
	{
		typedef Matrix< char > matrix_type ;
		matrix_type matrix1( 1, 1 ) ;

		matrix1(0, 0) = 'a' ;
		ASSERT_TRUE  ( matrix1(0,0) == 'a' ) ; 

		matrix1.resize(10,10) ;
		matrix1(9, 9) = 'a' ;
		ASSERT_TRUE  ( matrix1(9,9) == 'a' ) ; 

		for ( size_t i = 0 ; i < 10 ; ++i )
		{
			for ( size_t j = 0 ; j < 10 ; ++j )
			{
				matrix1(i,j) = static_cast< char >( static_cast< int >( 'a' ) + static_cast< int >( i ) ) ;
			}
		}

		for ( size_t i = 0 ; i < 10 ; ++i )
		{
			for ( size_t j = 0 ; j < 10 ; ++j )
			{
				ASSERT_TRUE  ( matrix1(i,j) == static_cast< char >( static_cast< int >( 'a' ) + static_cast< int >( i ) ) ) ;
				const char c = matrix1(i,j) ;
				c ;
			}
		}

		try
		{
			matrix1(0,11) = 'z' ;
		}
		catch ( ... ) 
		{

			try
			{
				matrix1( 11, 1 ) = 'd' ;
			}
			catch ( ... ) 
			{

				try
				{
					matrix1( 12, 13 ) = 'Z' ;
				}
				catch ( ... ) 
				{

					try
					{
						const char c = matrix1( 1, 11 ) ;
						c ;
					}
					catch ( ... ) 
					{

						try
						{
							Matrix< double > double_matrix ; // 0 rows, 0 cols ;
							double_matrix(0,0) = 5.f ;
						}
						catch ( ... ) 
						{
							return ;
						}
						FAIL_M("Expected exception not thrown!" ) ;

					}
					FAIL_M("Expected exception not thrown!" ) ;

				}
				FAIL_M("Expected exception not thrown!" ) ;

			}
			FAIL_M("Expected exception not thrown!" ) ;

		}
		FAIL_M("Expected exception not thrown!" ) ;


	}

	/** @test implementation of black-box test for init. */
	TEST( Matrix, init ) 
	{
		typedef Matrix< wchar_t > matrix_type ;
		matrix_type matrix1( 15, 15 ) ;

		matrix1.init( L'A' ) ;

		for ( size_t i = 0 ; i < matrix1.num_rows() ; ++i )
		{
			for ( size_t j = 0 ; j < matrix1.num_cols() ; ++j )
			{
				ASSERT_TRUE  ( matrix1(i,j) == L'A' ) ;
			}
		}

	}

	/** @test implementation of black-box test for resize. */
	TEST( Matrix, resize ) 
	{
		typedef Matrix< float > matrix_type ;
		matrix_type matrix1 ;
		ASSERT_TRUE  ( matrix1.num_cols() == 0 ) ; 
		ASSERT_TRUE  ( matrix1.num_rows() == 0 ) ; 

		matrix1.resize( 10, 10 ) ;
		ASSERT_TRUE  ( matrix1.num_cols() == 10 ) ; 
		ASSERT_TRUE  ( matrix1.num_rows() == 10 ) ; 

		matrix1.resize( 15, 15 ) ;
		ASSERT_TRUE  ( matrix1.num_rows() == 15 ) ; 
		ASSERT_TRUE  ( matrix1.num_cols() == 15 ) ; 
		matrix1.init( 3.f ) ;

		matrix1.resize( 5, 5 ) ;
		ASSERT_TRUE  ( matrix1.num_rows() == 5 ) ; 
		ASSERT_TRUE  ( matrix1.num_cols() == 5 ) ; 

		try
		{
			matrix1.resize( 0, 0 ) ;
		}
		catch ( ... ) 
		{

			try
			{
				matrix1.resize(0,1) ;
			}
			catch ( ... ) 
			{

				try
				{
					matrix1.resize(1,0) ;
				}
				catch ( ... ) 
				{

					return ;
				}
				FAIL_M("Expected exception not thrown!" ) ;

			}
			FAIL_M( "Expected exception not thrown!" ) ;

		}
		FAIL_M( "Expected exception not thrown!" ) ;
	}

	/** @test implementation of black-box test for _equal_op. */
	TEST( Matrix, AssignmentOperator ) 
	{
		typedef Matrix< size_t > matrix_type ;
		matrix_type matrix1( 10, 10), matrix2 ;

		ASSERT_TRUE  ( matrix1.num_cols() == 10 ) ; 
		ASSERT_TRUE  ( matrix1.num_rows() == 10 ) ; 

		ASSERT_TRUE  ( matrix2.num_cols() == 0 ) ; 
		ASSERT_TRUE  ( matrix2.num_rows() == 0 ) ; 

		matrix1.init( 23 ) ;
		matrix2 = matrix1 ;

		ASSERT_TRUE  ( matrix2.num_cols() == 10 ) ; 
		ASSERT_TRUE  ( matrix2.num_rows() == 10 ) ; 
		ASSERT_TRUE  ( matrix1.num_cols() == matrix2.num_cols() ) ; 
		ASSERT_TRUE  ( matrix1.num_rows() == matrix2.num_rows() ) ; 

		for ( size_t i = 0 ; i < matrix1.num_rows() ; ++i )
		{
			for ( size_t j = 0 ; j < matrix1.num_cols() ; ++j )
			{
				ASSERT_TRUE  ( matrix1(i,j) == matrix2(i,j) ) ;
			}
		}

		matrix2.resize( 5, 5 ) ;
		for ( size_t i = 0 ; i < matrix2.num_rows() ; ++i )
		{
			for ( size_t j = 0 ; j < matrix2.num_cols() ; ++j )
			{
				matrix2(i,j) = i*j ;
			}
		}

		matrix1 = matrix2 ;
		ASSERT_TRUE  ( matrix1.num_cols() == 5 ) ; 
		ASSERT_TRUE  ( matrix1.num_rows() == 5 ) ; 
		ASSERT_TRUE  ( matrix1.num_cols() == matrix2.num_cols() ) ; 
		ASSERT_TRUE  ( matrix1.num_rows() == matrix2.num_rows() ) ;

		for ( size_t i = 0 ; i < matrix1.num_rows() ; ++i )
		{
			for ( size_t j = 0 ; j < matrix1.num_cols() ; ++j )
			{
				ASSERT_TRUE  ( matrix1(i,j) == matrix2(i,j) ) ;
			}
		}

	}

	/** @test implementation of black-box test for Matrix. */
	TEST( Matrix, Constructor ) 
	{
		typedef Matrix< long > matrix_type ;

		matrix_type matrix1( 10, 10 ) ;

		ASSERT_TRUE  ( matrix1.num_cols() == 10 ) ; 
		ASSERT_TRUE  ( matrix1.num_rows() == 10 ) ; 

		matrix_type matrix1a ;

		ASSERT_TRUE  ( matrix1a.num_cols() == 0 ) ; 
		ASSERT_TRUE  ( matrix1a.num_rows() == 0 ) ; 

		matrix_type matrix1c( matrix1a ) ;
		ASSERT_TRUE  ( matrix1c.num_cols() == 0 ) ; 
		ASSERT_TRUE  ( matrix1c.num_rows() == 0 ) ; 

		matrix1a.resize( 10, 10 ) ;

		ASSERT_TRUE  ( matrix1a.num_cols() == 10 ) ; 
		ASSERT_TRUE  ( matrix1a.num_rows() == 10 ) ; 

		matrix_type matrix1b( matrix1a ) ;
		ASSERT_TRUE  ( matrix1b.num_cols() == 10 ) ; 
		ASSERT_TRUE  ( matrix1b.num_rows() == 10 ) ; 

		try
		{
			matrix_type matrix2( 0, 1 ) ;
		}
		catch ( ... ) 
		{

			try
			{
				matrix_type matrix3( 1, 0 ) ;
			}
			catch ( ... ) 
			{

				try
				{
					matrix_type matrix4( 0, 0 ) ;
				}
				catch ( ... ) 
				{

					return ;
				}
				FAIL_M( "Expected exception not thrown!" ) ;

			}
			FAIL_M( "Expected exception not thrown!" ) ;

		}
		FAIL_M( "Expected exception not thrown!" ) ;

	}

}


#endif // #ifdef _DEBUG
