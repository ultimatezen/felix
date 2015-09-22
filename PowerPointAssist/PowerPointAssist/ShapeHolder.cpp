#include "StdAfx.h"
#include "ShapeHolder.h"

CShapeHolder::CShapeHolder(const ShapePtr &shape) :
	m_shape( shape )
		, m_left(0)
		, m_top(0)
{
	setPos() ;
}

CShapeHolder::CShapeHolder(const CShapeHolder &holder) :
	m_shape( holder.m_shape )
{
	setPos() ;
}


CShapeHolder::~CShapeHolder(void)
{
}

void CShapeHolder::setPos(void)
{
	m_left = m_shape->Left ;
	m_top = m_shape->Top ;
}
