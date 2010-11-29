// Loop.h: interface for the Loop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOOP_H__09A4C018_E6A4_4F4D_9D3C_E76F73589A8C__INCLUDED_)
#define AFX_LOOP_H__09A4C018_E6A4_4F4D_9D3C_E76F73589A8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <crtdbg.h>
#include <vector>		// for the internal vector
#include <set>			// for assignment
#include <algorithm>	// for std::copy
#include <iterator>		// for std::advance

#define iterator_end m_container.size()

template< class TYPE >
struct loop_iterator  
{
	typedef TYPE						value_type ;
	typedef	std::vector < value_type >	vector_type ;
	typedef	std::set < value_type >		set_type ;
	typedef	std::multiset< value_type >	multiset_type ;
	typedef vector_type::iterator		iterator_type ;
	typedef iterator_type				iterator ;
	typedef value_type&					reference ;
	typedef loop_iterator< TYPE >		self ;

	size_t			m_pos ;
	vector_type		m_container ;
	
	// constructors
	loop_iterator( ) : m_pos( iterator_end ) { }
	loop_iterator( self &copy ) : m_container(copy.m_container), m_pos( copy.m_pos )  {}
	// operations on loop
	void clear()
	{
		m_pos = 0 ;
		m_container.clear() ;
	}
	void erase_current()
	{
		erase( m_pos ) ;
	}
	void erase( size_t index )
	{
		iterator_type pos = begin() ;
		std::advance( pos, index ) ;

		m_container.erase( pos ) ;
		m_pos = index ;
		if ( m_pos == iterator_end )	
			m_pos = 0 ;

	}
	void push_back( TYPE &type ) { m_container.push_back( type ) ; }
	// navigation
	iterator begin() { return m_container.begin() ; }
	iterator end() { return m_container.end() ; }
	self & operator++ ( )
	{
		increment_state() ;
		return *this ;
	}
	self & operator-- ( )
	{
		decrement_state() ;
		return *this ;
	}
	bool move_to( size_t pos )
	{
		if ( pos >= size() ) return false ;
		m_pos = pos ;
	}
	// assignment
	template< class CONTAINER_TYPE >
	self & operator= ( CONTAINER_TYPE &rhs )
	{
		m_container.clear() ;
		m_container.resize( rhs.size() ) ;
		std::copy( rhs.begin(), rhs.end(), m_container.begin() ) ;
		return *this ;
	}
	// reference
	const value_type &operator*() const
	{
		_ASSERTE( m_pos != iterator_end ) ;
		return m_container[m_pos] ;
	}
	value_type &operator*()
	{
		_ASSERTE( m_pos != iterator_end ) ;
		return m_container[m_pos] ;
	}
	value_type &operator[](size_t pos)
	{
		_ASSERTE( pos >= 0 && pos < iterator_end ) ;
		return m_container[pos] ;
	}
	void resize( size_t sz )
	{
		m_container.resize( sz ) ;
	}
	// is the container empty? 
	bool empty( ) {	
		_ASSERTE ( ! ( ( iterator_end == 0 ) && ( m_pos != iterator_end ) ) ) ;
		return ( m_pos == iterator_end ) ; 
	}

	// how many members are in the container?
	size_t size() { return m_container.size() ; }
	size_t pos() { return m_pos ; }

private:
	void increment_state( )
	{
		// can't loop on an empty container
		if ( m_pos == iterator_end )
			return  ;

		++m_pos ;
		if ( m_pos == iterator_end )	
			m_pos = 0 ;
	}
	void decrement_state( )
	{
		// can't loop on an empty container
		if ( m_pos == iterator_end )
			return ;

		if ( m_pos == 0 )
			m_pos = iterator_end ;
		--m_pos ;
	}
} ;

#endif // !defined(AFX_LOOP_H__09A4C018_E6A4_4F4D_9D3C_E76F73589A8C__INCLUDED_)
