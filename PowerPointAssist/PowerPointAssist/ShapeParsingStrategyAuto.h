#pragma once
#include "shapeparsingstrategy.h"

class CShapeParsingStrategyAuto :
	public CShapeParsingStrategy
{
public:
	CShapeParsingStrategyAuto(void);
	~CShapeParsingStrategyAuto(void);

	PowerPoint::TextRangePtr nextTextRange( ) ;
};
