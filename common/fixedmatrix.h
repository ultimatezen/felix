/*!
	@file FixedMatrix.h
	@brief Fixed matrix header file
	@date 2004/12/17
	Time: 12:54:38
	@author Ryan Ginstrom
 */

#pragma once

#include "FixedArray.h"

/** FixedMatrix */
template < class TYPE, size_t ROW_SIZE, size_t COL_SIZE >
class FixedMatrix
{
	typedef FixedMatrix< TYPE, ROW_SIZE, COL_SIZE > self ;
	typedef FixedArray< TYPE, ROW_SIZE * COL_SIZE > ArrayType ;
	ArrayType mTheMatrix ;

public:
	typedef TYPE			value_type ;
	typedef TYPE*			iterator ;
	typedef const TYPE*		const_iterator ;
	typedef TYPE&			reference ;
	typedef const TYPE&		const_reference ;
	typedef size_t			size_type ;


	/** initialize the matrix to this value. 
	@param value parameter. 
	*/
	void init( const TYPE &value = TYPE(0) ) 
	{ 
		mTheMatrix.init( value ) ; 
	}

	// iterator support 

	/** access operator. 
	Read-write access
	@sa reference GetVal( const size_t row, const size_t col ) 
	*/
	reference operator()( size_t row, size_t col ) 
	{ 
		ATLASSERT( row < num_rows() && "the row Index must be lower than our max rows") ;
		ATLASSERT( col < num_cols() && "the column Index must be lower than our max cols") ;

		return GetVal( row, col ) ; 
	}

	/** read-only reference. 
	@sa reference GetVal( const size_t row, const size_t col ) 
	*/
	const_reference operator()( size_t row, size_t col ) const 
	{ 
		ATLASSERT( row < num_rows() && "the row Index must be lower than our max rows") ;
		ATLASSERT( col < num_cols() && "the column Index must be lower than our max cols") ;

		return GetVal( row, col ) ; 
	}
	
	/** The number of rows in the matrix. 
	@return size_type number of rows . 
	*/
	size_type num_rows() const 
	{ 
		return ROW_SIZE ; 
	}

	/** The number of columns in the matrix. 
	@return size_type number of columns. 
	*/
	size_type num_cols() const 
	{ 
		return COL_SIZE ; 
	}


private:
	/** Private access function. 
	@return reference the matrix item. 
	@param row the row. 
	@param col the column. 
	*/
	reference GetVal( const size_t row, const size_t col )
	{
		ATLASSERT( row < num_rows() && "the row Index must be lower than our max rows") ;
		ATLASSERT( col < num_cols() && "the column Index must be lower than our max cols") ;

		size_t pos = row + ( col * ROW_SIZE ) ;
		_ASSERTE ( pos < mTheMatrix.size() ) ;
		return mTheMatrix[ pos ] ;
	}

} ;

