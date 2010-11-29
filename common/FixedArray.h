/*!
	@file FixedArray.h
	@brief Fixed array class header
	@date 2004/12/16
	Time: 23:00:44
	@author Ryan Ginstrom
	@dir C:\PROGRAMMING\MYLIBRARY
 */

#pragma once

#include <exception>
#include <iterator> // advance

#include "easyunit/testharness.h"

using namespace easyunit;


#ifndef FLOAT_EQ
#define EPSILON 0.00001
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x <( v + EPSILON)))
#endif 

/** FixedArray.
FixedArray is a high-performance array that acts as an stl container.
Adapted from:
Bjarne Stroustrup, "The C++ Programming Language," 3rd edition
 */
template < class TYPE, size_t SIZE >
class FixedArray
{
	TYPE mArray[SIZE] ;
public:
	typedef TYPE			value_type ;
	typedef TYPE*			iterator ;
	typedef const TYPE*		const_iterator ;
	typedef TYPE&			reference ;
	typedef const TYPE&		const_reference ;
	typedef size_t			size_type ;


	/** initialize the array with the specified value. 
	@param value value to initialize array with. 
	*/
	void init( const TYPE value ) 
	{ 
		std::fill( begin(), end(), value ) ; 
	}
	/** The beginning of the array. 
	@return iterator description. 
	*/
	iterator begin() 
	{ 
		return mArray ; 
	}

	/** @sa iterator begin() 
	*/
	const_iterator begin() const 
	{ 
		return mArray ; 
	}

	/** The end of the array. 
	@return iterator description. 
	*/
	iterator end() 
	{ 
		return mArray + SIZE ; 
	}

	/** @sa iterator end() 
	*/
	const_iterator end() const 
	{ 
		return mArray + SIZE ; 
	}

	/** brief. 
	@return reference description. 
	@param i parameter. 
	*/
	reference operator[]( size_t i ) 
	{ 
		ATLASSERT( i < SIZE ) ;

		return mArray[i] ; 
	}

	/** @sa reference operator[]( size_t i ) 
	*/
	const_reference operator[]( size_t i ) const 
	{ 
		ATLASSERT( i < SIZE )

		return mArray[i] ; 
	}
	
	// size is constant

	/** the size of the array. 
	@return size_type description. 
	*/
	size_type size() const 
	{ 
		return SIZE ; 
	}

	/** the maximum size of the array. 
	@return size_type description. 
	*/
	size_type max_size() const 
	{ 
		return SIZE ; 
	}
} ;


