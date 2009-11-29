#include "StdAfx.h"
#include "ShapeParsingStrategyAuto.h"

CShapeParsingStrategyAuto::CShapeParsingStrategyAuto(void)
{
}

CShapeParsingStrategyAuto::~CShapeParsingStrategyAuto(void)
{
}

PowerPoint::TextRangePtr CShapeParsingStrategyAuto::nextTextRange( )
{
	if ( m_shape == NULL )
	{
		return NULL ;
	}
	TextRangePtr range = m_shape->TextFrame->TextRange ;

	// we have "exhausted" this shape
	m_shape = NULL ;

	return range ;
}

