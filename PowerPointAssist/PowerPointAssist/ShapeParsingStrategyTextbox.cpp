#include "StdAfx.h"
#include "ShapeParsingStrategyTextbox.h"

CShapeParsingStrategyTextbox::CShapeParsingStrategyTextbox(void)
{
}

CShapeParsingStrategyTextbox::~CShapeParsingStrategyTextbox(void)
{
}

PowerPoint::TextRangePtr CShapeParsingStrategyTextbox::nextTextRange( )
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
