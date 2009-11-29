#pragma once

#include "PowerPointImports.h"

class CShapeParsingStrategy
{
protected:
	PowerPoint::ShapePtr m_shape ;
public:

	virtual ~CShapeParsingStrategy(void)
	{
	}

	virtual void SetShape( PowerPoint::ShapePtr shape )
	{
		m_shape = shape ;
	}

	virtual PowerPoint::TextRangePtr nextTextRange( ) = 0;
};


typedef CShapeParsingStrategy* parsingStrategy ;