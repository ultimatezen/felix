// SafeArray.h: interface for the CSafeArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAFEARRAY_H__BB99D7BF_D27A_409F_8C7E_5CD89D1678AA__INCLUDED_)
#define AFX_SAFEARRAY_H__BB99D7BF_D27A_409F_8C7E_5CD89D1678AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Exceptions.h"
#include "StringEx.h"

// ======================
// class CSafeArrayWrapper
// ======================
template < class TYPE, VARTYPE VT >
class CSafeArrayWrapper
{
	typedef CSafeArrayWrapper< TYPE, VT >	array_type ;
	typedef TYPE							elem_type ;

protected:
	SAFEARRAY	*m_psa ;
public:
	// constructors
	CSafeArrayWrapper( SAFEARRAY *sfa = NULL ) : m_psa( sfa ) {}

	// access the array
	SAFEARRAY *array() { return m_psa ; }
	// copy
	// wraps SafeArrayCopy
	void copy( SAFEARRAY *cpy )
	{
		ATLASSERT( cpy != NULL ) ;
		ATLASSERT( 1 == SafeArrayGetDim( m_psa ) ) ;

		destroy() ;
		// copy the array
		COM_ENFORCE( SafeArrayCopy( cpy, &m_psa ), TEXT("Failed to clone SAFEARRAY!") ) ;

		set_bounds() ;
	}		
	// destroy
	// wraps SafeArrayDestroy
	void destroy()
	{
		if ( m_psa != NULL )
			SafeArrayDestroy( m_psa ) ;
	}
	// create_vector
	// wraps SafeArrayCreateVector
	bool create_vector( unsigned int  cElements  )
	{
		m_psa = SafeArrayCreateVector( VT, 0, cElements ) ;
		return m_psa != NULL ; // let the caller decide whether to throw an exception if it is NULL
	}  
	// get_dim
	// wraps SafeArrayGetDim
	UINT get_dim()
	{
		return SafeArrayGetDim( m_psa );
 	}
	// access_data
	// wraps SafeArrayAccessData
	elem_type** access_data()
	{
		elem_type** out_value = NULL ;
		COM_ENFORCE( SafeArrayAccessData( m_psa, (void **)out_value ), TEXT("Failed to access SafeArray data!") ) ;
		return out_value ;
	}
	// at
	elem_type& at( long index )
	{
		index -= lbound() ;
		if ( index >= ubound() )
			throw CException( TEXT("SAFEARRAY index ") + int2tstring( index ) + TEXT(" is out of bounds") ) ;
		return static_cast< TYPE *>( m_psa->pvData )[index] ;
	}
	// []
	elem_type& operator[]( long index )
	{
		index -= lbound() ;

		ATLASSERT( index < ubound() ) ;

		return static_cast< TYPE *>( m_psa->pvData )[index] ;
	}
	// lbound
	long lbound()
	{
		if ( m_psa == NULL )
			return 0 ;

		return bounds().lLbound ;
	}
	// ubound
	long ubound()
	{
		if ( m_psa == NULL )
			return 0 ;

		return bounds().cElements - bounds().lLbound ;
	}
	// size
	long size()
	{
		if ( m_psa == NULL )
			return 0 ;
		return bounds().cElements ;
	}
private:
	SAFEARRAYBOUND &bounds()
	{
		return array()->rgsabound[0];
	}

} ;

// ======================
// class CSafeArray
// ======================
template < class TYPE, VARTYPE VT >
class CSafeArray : public CSafeArrayWrapper< TYPE, VT >
{
	typedef CSafeArrayWrapper< TYPE, VT >	array_type ;
	typedef TYPE							elem_type ;

public:
	// constructors
	CSafeArray(unsigned int  size  ) 
	{
		if ( false == create_vector( size ) )
			throw CWinException( TEXT("Failed to create SAFEARRAY!") ) ;
	}
	CSafeArray( SAFEARRAY *cpy )
	{
		copy( cpy )  ;
	}
	CSafeArray( const CSafeArray &cpy )
	{
		copy( cpy.m_psa )  ;
	}
	// destructor
	~CSafeArray()
	{
		destroy() ;
	}
};

#endif // !defined(AFX_SAFEARRAY_H__BB99D7BF_D27A_409F_8C7E_5CD89D1678AA__INCLUDED_)
