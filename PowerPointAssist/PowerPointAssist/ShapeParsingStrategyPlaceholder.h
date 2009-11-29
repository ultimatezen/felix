#pragma once
#include "shapeparsingstrategy.h"

class CShapeParsingStrategyPlaceholder :
	public CShapeParsingStrategy
{
public:
	CShapeParsingStrategyPlaceholder(void);
	~CShapeParsingStrategyPlaceholder(void);

	PowerPoint::TextRangePtr nextTextRange( ) ;
};
