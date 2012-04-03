
#pragma once

#include <iterator>

/** A dynamic array class */
template < class TYPE >
class Array
{
	typedef Array< TYPE >	self ;
//	friend Array ;

	size_t mArraySize ;
	size_t mMaxSize ;

	TYPE *mTheArray ;

public:
	typedef TYPE			value_type ;
	typedef TYPE*			iterator ;
	typedef const TYPE*		const_iterator ;
	typedef TYPE&			reference ;
	typedef const TYPE&		const_reference ;
	typedef size_t			size_type ;

	// iterator support 

	/** beginning of the array sequence. 
	@return iterator the first element in the array. 
	*/
	iterator begin() ;

	/** constant reference to beginning of array sequence. 
	@return const_iterator the first element in the array. 
	*/
	const_iterator begin() const  ;

	/** end of the array sequence. 
	@return iterator the last element in the array. 
	*/
	iterator end()  ;

	/** constant reference to end of array sequence. 
	@return const_iterator he last element in the array. 
	*/
	const_iterator end() const ;

	// matrix info

	bool empty() const ;
	/** the size of the array. 
	@return size_t the size. 
	*/
	size_t size( ) const ;

	/** the capacity of the array. 
	@return size_t the capacity. 
	*/
	size_t max_size( ) const ;

	/** array operator[].
	Read only.
	@return const_reference reference to array member i. 
	@param i the position in the array. 
	*/
	const_reference operator[] ( const size_t i ) const  ;

	/** array operator.
	Read-write.
	@return reference reference to array member i. 
	@param i the position in the array. 
	*/
	reference operator() ( const size_t i ) ;

	/** init all members of array to val. 
	@param &val the value to initialize to. 
	*/
	void init( const TYPE &val ) ;

	/** Resize the array.
	Capacity is grow-only. Contents of array are not preserved on grow.
	@return bool was the resize successful? 
	@param requested_size The new size. Setting to 0 does not affect the array capacity.
	*/
	bool resize ( const size_t requested_size ) ;

	/** clear the array. 
	*/
	void clear()  ;

	/** assignment operator. 
	@return self_reference  self. 
	@param &rhs array to copy. 
	*/
	self& operator = ( const self &rhs ) ;

	/** creates a copy of self into rhs. 
	@return bool success 
	@param &rhs another array. 
	*/
	bool clone( self &rhs ) const ;


	/** checks whether values of arrays up to size are identical.
	Useful for testing
	@return bool Are the arrays value identical? 
	@param &rhs the other array. 
	*/
	bool val_identity( const self &rhs ) const ;

	/** swap contents of another array with this one. 
	@return bool was the operation successful? 
	@param &rhs another array. 
	*/
	bool swap( self &rhs ) ;

	//
	// Construction/destruction
	//

	/** default constructor. 
	*/
	explicit Array();

	/** constructor taking size argument. 
	@param requested_size initial size of the array. 
	*/
	explicit Array( const size_t requested_size) ;

	/** copy constructor. 
	@param &rhs another array to copy. 
	*/
	Array( const self &rhs ) ;

	/** free resources on destruction. 
	*/
	~Array() ;

private:

	/** copies another array into this one. 
	The copy will be exact: capacity, size, and contents will all match.
	Content beyond size() not affected. 
	@return bool success. 
	@param &rhs array to receive the copy. 
	*/
	bool internal_copy( const self &rhs ) ;
	/** dispose of the array. 
	private helper function
	*/
	void  dispose( ) ;
} ;


template < class TYPE >
TYPE* Array<TYPE>::begin()
{ 
	return mTheArray ; 
}


template < class TYPE >
const TYPE* Array<TYPE>::begin() const 
{ 
	return mTheArray ; 
}

template < class TYPE >
TYPE* Array<TYPE>::end() 
{ 
	return mTheArray + mArraySize ; 
}

template < class TYPE >
const TYPE* Array<TYPE>::end() const 
{ 
	return mTheArray + mArraySize ; 
}

// matrix info
template < class TYPE >
bool Array<TYPE>::empty() const
{ 
	return ( begin() == end() ) ; 
}

template < class TYPE >
size_t Array<TYPE>::size( ) const
{ 
	_ASSERTE( mArraySize <= mMaxSize && "size must be <= capacity" ) ;

	return mArraySize ; 
}

template < class TYPE >
size_t Array<TYPE>::max_size( ) const
{ 
	_ASSERTE( mArraySize <= mMaxSize && "size must be <= capacity" ) ;
	_ASSERTE( (mMaxSize > 0 || mTheArray == NULL) && "if size = 0 then the Array is null" ) ;
	_ASSERTE( (mMaxSize == 0 || mTheArray != NULL) && "if the Array is null then size = 0" ) ;

	return mMaxSize ; 
}

template < class TYPE >
const TYPE& Array<TYPE>::operator[] ( const size_t i ) const 
{ 
	if ( i >= mArraySize )
	{
		throw std::out_of_range( "Index out of bounds" ) ;
	}

	return mTheArray[i] ; 
}

template < class TYPE >
TYPE& Array<TYPE>::operator() ( const size_t i ) 
{
	if ( i >= mArraySize )
	{
		throw std::out_of_range( "Index out of bounds" ) ;
	}
	return mTheArray[i] ; 
}

template < class TYPE >
void Array<TYPE>::init( const TYPE &val )
{
	ATLASSERT( size() > 0 ) ;

	std::fill( begin(), end(), val ) ;
}

template < class TYPE >
bool Array<TYPE>::resize( const size_t requested_size )
{ 
	// only grow it if we have to
	if ( requested_size <= mMaxSize )
	{
		mArraySize = requested_size ;
		return true ;
	}

	// create the new array
	TYPE *new_array = new TYPE[ requested_size ] ;
	iterator end_pos = begin() ;
	std::advance( end_pos, ( requested_size < size() ? requested_size : size() ) ) ;
#pragma warning(disable:4996) // Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
	std::copy( begin(), end_pos, new_array ) ;
#pragma warning(default:4996)

	// get rid of the present array
	dispose() ;

	// initialize the new one
	mTheArray = new_array ;

	mMaxSize = requested_size ;
	mArraySize = requested_size ;

	return true ;
}

template < class TYPE >
void Array<TYPE>::clear() 
{ 
	dispose() ; 
}

template < class TYPE >
Array<TYPE>& Array<TYPE>::operator = ( const self &rhs )
{
	if ( &rhs == this ) 
	{
		return *this ;
	}

	TYPE* tmp = new TYPE[rhs.mMaxSize] ;
#pragma warning(disable:4996) // Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
	std::copy( rhs.begin(), rhs.end(), tmp ) ;
#pragma warning(default:4996)

	dispose() ;

	mArraySize = rhs.mArraySize ;
	mMaxSize = rhs.mMaxSize ;
	mTheArray = tmp ;

	return *this ;
}

template < class TYPE >
bool Array<TYPE>::clone( self &rhs ) const
{
	/** So we only secure enough memory to store the actual
	size of the array, not maximum size.<br>
	We can use this to remove excess capacity via the swap trick...<br>
	<code> my_array_type( foo_array ).swap( foo_array ) ; </code>
	*/
	rhs.resize( size() ) ;
#pragma warning(disable:4996) // Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
	std::copy( begin(), end(), rhs.begin() ) ;
#pragma warning(default:4996)
	return true ;
}


template < class TYPE >
bool Array<TYPE>::val_identity( const self &rhs ) const
{
	size_t lhs_len = size() ;

	if ( lhs_len != rhs.size() ) 
	{
		return false ;
	}

	for ( size_t i = 0u ; i < lhs_len ; ++i )
	{
		if ( mTheArray[i] != rhs[i] ) 
		{
			return false ;
		}
	}
	return true ;
}

template < class TYPE >
bool Array<TYPE>::swap( self &rhs )
{
	// temp variables to remember our own data
	size_type tmp_size = mArraySize ;
	size_type tmp_max_size = mMaxSize ;
	iterator tmp_array = mTheArray ;

	// set this to rhs
	mArraySize = rhs.mArraySize ;
	mMaxSize = rhs.mMaxSize ;
	mTheArray = rhs.mTheArray ;

	// set rhs to the temp values we saved earlier (weren't we smart to do that?)
	rhs.mArraySize = tmp_size ;
	rhs.mMaxSize = tmp_max_size ;
	rhs.mTheArray = tmp_array ;

	return true ;
}

//
// Construction/destruction
//

template < class TYPE >
Array<TYPE>::Array() : 
mArraySize(0),
mMaxSize(0),
mTheArray( NULL ) 
{
}

template < class TYPE >
Array<TYPE>::Array( const size_t requested_size) : 
mArraySize(0),
mMaxSize(0),
mTheArray( NULL ) 
{
	ATLASSERT( mTheArray == NULL ) ;
	ATLASSERT( mMaxSize == 0 ) ;
	ATLASSERT( mArraySize == 0 ) ;

	if ( requested_size > 0 ) 
	{
		resize( requested_size ) ;
	}
}

template < class TYPE >
Array<TYPE>::Array( const self &rhs ) :
mArraySize(0),
mMaxSize(0),
mTheArray( NULL ) 
{
	ATLASSERT( mTheArray == NULL ) ;
	ATLASSERT( mMaxSize == 0 ) ;
	ATLASSERT( mArraySize == 0 ) ;

	internal_copy( rhs ) ;
}

template < class TYPE >
Array<TYPE>::~Array() 
{ 
	dispose() ; 
}

template < class TYPE >
bool Array<TYPE>::internal_copy( const self &rhs )
{
	dispose() ;
	resize( rhs.max_size() ) ;
	resize( rhs.size() ) ;
#pragma warning(disable:4996) // Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
	std::copy( rhs.begin(), rhs.end(), begin() ) ;
#pragma warning(default:4996)

	return true ;
}

template < class TYPE >
void Array<TYPE>::dispose( )
{
	if ( mTheArray != NULL ) 
	{
		delete[] mTheArray ;
		mArraySize = 0 ;
		mMaxSize = 0 ;
		mTheArray = 0 ;
	}

}
