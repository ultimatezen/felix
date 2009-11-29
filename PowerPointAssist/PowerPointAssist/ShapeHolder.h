#pragma once

#include "PowerPointImports.h"
#include <set>
#include "stringex.h"

class CShapeHolder
{
public:
	CShapeHolder(const ShapePtr &shape);
	CShapeHolder(const CShapeHolder &holder);

	~CShapeHolder(void);

	double GetLeft() const
	{
		return m_left;
	}

	double GetTop() const
	{
		return m_top;
	}

	PowerPoint::ShapePtr& GetShape()
	{
		return m_shape;
	}
	wstring GetShapeName() const 
	{
		if ( m_shape == NULL )
		{
			return wstring() ;
		}
		return BSTR2wstring( m_shape->Name ) ;
	}
private:
	double m_left;
	double m_top;
	ShapePtr m_shape ;
private:
	void setPos(void);
};

// compare left to right, top to bottom
// going top to bottom, left to right is too disorienting.
inline bool operator < ( const CShapeHolder &lhs, const CShapeHolder &rhs )
{
	// compare left
	if ( lhs.GetLeft() < rhs.GetLeft() )
	{
		return true ;
	}
	if ( lhs.GetLeft() > rhs.GetLeft() )
	{
		return false ;
	}

	// compare top
	if ( lhs.GetTop() < rhs.GetTop() )
	{
		return true ;
	}
	if ( lhs.GetTop() > rhs.GetTop() )
	{
		return false ;
	}

	// compare IDs

	return ( lhs.GetShapeName() < rhs.GetShapeName() ) ;
}

typedef std::set< CShapeHolder > holderSet ;
