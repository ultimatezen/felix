#pragma once

#include "ShapeParsingStrategy.h"
#include "PowerPointImports.h"

class strategyFactory
{
public:
	static CShapeParsingStrategy *create( PowerPoint::ShapePtr shape ) ;
	
	static void dispose(CShapeParsingStrategy *) ;
};
