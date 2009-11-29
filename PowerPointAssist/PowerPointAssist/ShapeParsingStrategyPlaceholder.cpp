#include "StdAfx.h"
#include "ShapeParsingStrategyPlaceholder.h"

CShapeParsingStrategyPlaceholder::CShapeParsingStrategyPlaceholder(void)
{
}

CShapeParsingStrategyPlaceholder::~CShapeParsingStrategyPlaceholder(void)
{
}

PowerPoint::TextRangePtr CShapeParsingStrategyPlaceholder::nextTextRange( )
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
