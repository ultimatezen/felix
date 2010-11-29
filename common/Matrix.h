/*!
	@file Matrix.h
	@brief Matrix class header
	@date 2004/12/16
	Time: 16:57:23
	@author Ryan Ginstrom
	@dir C:/Programming/MyLibrary
 */

#pragma once

#include "Array.h"

#ifdef _DEBUG
#include <exception>
#endif

/** dynamic two-dimensional matrix.
We purposefully do not include iterator support, because
we want the class to be accessed as a matrix only.
*/
template < class TYPE >
class Matrix
{
	typedef Matrix< TYPE >	self ;

	friend Matrix ;

	size_t mNumRows ;
	size_t mNumCols ;

	Array< TYPE > mTheMatrix ;

public:
	typedef TYPE&			reference ;
	typedef const TYPE&		const_reference ;

	// matrix info

	/** the number of rows in the matrix. 
	@return size_t the number of rows. 
	*/
	size_t num_rows( ) const
	{ 
		return mNumRows ; 
	}

	/** the number of columns in the matrix. 
	@return size_t the number of columns. 
	*/
	size_t num_cols( ) const
	{ 
		return mNumCols ; 
	}

	// direct access

	/** read-only access. 
	@return const_reference the selected matrix item. 
	@sa reference operator() ( const size_t, const size_t ) 
	*/
	const_reference operator() ( const size_t row, const size_t col ) const 
	{
#ifdef _DEBUG
		if ( row >= mNumRows || col >= mNumCols )
		{
			throw std::out_of_range( "Index out of range" ) ;
		}
#endif		

		size_t pos = row + ( col * mNumRows ) ;
		return static_cast< const_reference >( mTheMatrix[pos] )  ; 
	}

	/** read-write access. 
	@par sample usage:
@code matrix(0, 1) = 3 ;
int i = matrix(0, 1) ; @endcode
	@return reference the selected matrix item. 
	@param row the row number. 
	@param col the column number. 
	*/
	reference operator() ( const size_t row, const size_t col ) 
	{
#ifdef _DEBUG
		if ( row >= mNumRows || col >= mNumCols )
		{
			throw std::out_of_range( "Index out of range" ) ;
		}
#endif		

		size_t pos = row + ( col * mNumRows ) ;
		return static_cast< reference >( mTheMatrix(pos) )  ; 
	}

	/** init all members of matrix to val. 
	@param &val the value to initialize to. 
	*/
	void init( const TYPE &val )
	{
		mTheMatrix.init( val ) ;
	}

	/** Deletes current array, and creates new one with specified dimensions.
	Contents are not guaranteed after resize!
	@return bool success. 
	@param rows the number of rows. 
	@param cols the number of columns. 
	*/
	bool resize( const size_t rows, const size_t cols )
	{ 
#ifdef _DEBUG
		if ( rows == 0 || cols == 0 )
		{
			throw std::invalid_argument( "Invalid matrix dimensions" ) ;
		}
#endif		
		
		// get rid of the present array
		mNumRows = rows ;
		mNumCols = cols ;
		mTheMatrix.resize( rows * cols ) ;
		
		return true ;
	}

	/** assignment operator. 
	@return self& reference to self. 
	@param &rhs the other matrix. 
	*/
	self& operator =( const self &rhs )
	{
		internal_copy( rhs ) ;
		return *this ;
	}

	/** creates a copy. 
	@return bool success. 
	@param &rhs parameter. 
	*/
	bool clone ( self &rhs )
	{
		rhs.mNumRows = mNumRows ;
		rhs.mNumCols = mNumCols ;
		rhs.mTheMatrix = mTheMatrix ;
		return true ;
	}
	//
	// Construction/destruction
	//

	/** default constructor. 
	*/
	Matrix( ) : 
		mNumRows(0), 
		mNumCols(0)
	{
		// we need this statement because of our little assembly-language trick...
		return ;
	}

	/** default constructor. 
	@param rows number of initial rows. 
	@param cols number of initial cols. 
	*/
	Matrix( const size_t rows, const size_t cols ) : 
		mTheMatrix( rows*cols ), 
		mNumRows(rows), 
		mNumCols(cols)
	{
#ifdef _DEBUG
		if ( mNumRows == 0 || mNumCols == 0 )
		{
			throw std::invalid_argument( "Invalid matrix dimensions" ) ;
		}
#endif		

		// we need this statement because of our little assembly-language trick...
		return ;
	}

	/** copy constructor. 
	@param &self another matrix. 
	*/
	Matrix( const self &rhs ) :
		mNumRows(0), 
		mNumCols(0)
	{
		internal_copy( rhs ) ;
	}
private:
	/** copies another matrix into this one. 
	The copy will be exact: number of rows and columns, and contents will all match.
	@return bool success. 
	@param &rhs array to receive the copy. 
	*/
	bool internal_copy( const self &rhs )
	{
		mTheMatrix = rhs.mTheMatrix ;
		mNumRows = rhs.mNumRows ;
		mNumCols = rhs.mNumCols ;
		return true ;
	}

} ;

