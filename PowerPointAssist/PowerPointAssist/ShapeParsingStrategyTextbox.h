#pragma once
#include "shapeparsingstrategy.h"

class CShapeParsingStrategyTextbox :
	public CShapeParsingStrategy
{
public:
	CShapeParsingStrategyTextbox(void);
	~CShapeParsingStrategyTextbox(void);

	PowerPoint::TextRangePtr nextTextRange( ) ;
};
