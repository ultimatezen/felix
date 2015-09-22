#include "stdafx.h"
#include "StrategyFactory.h"

#include "ShapeParsingStrategyAuto.h"
#include "ShapeParsingStrategyPlaceholder.h"
#include "ShapeParsingStrategyTable.h"
#include "ShapeParsingStrategyTextbox.h"


CShapeParsingStrategyTable stratTable ;
CShapeParsingStrategyTextbox stratTextbox ;
CShapeParsingStrategyPlaceholder stratPlaceholder ;
CShapeParsingStrategyAuto stratAuto ;

CShapeParsingStrategy *strategyFactory::create( PowerPoint::ShapePtr shape )
{
	if ( NULL == shape )
	{
		return NULL ;
	}

	Office::MsoShapeType type = shape->Type ;
	switch( type ) 
	{
	case Office::msoAutoShape :
		if ( ! shape->HasTextFrame )
		{
			return NULL ;
		}
		if ( 0 == shape->TextFrame->TextRange->Length )
		{
			return NULL ;
		}
		stratAuto.SetShape( shape ) ;
		return static_cast< CShapeParsingStrategy* >( &stratAuto ) ;

	case Office::msoPlaceholder :
		if ( ! shape->HasTextFrame )
		{
			return NULL ;
		}
		if ( 0 == shape->TextFrame->TextRange->Length )
		{
			return NULL ;
		}
		stratPlaceholder.SetShape( shape ) ;
		return static_cast< CShapeParsingStrategy* >( &stratPlaceholder ) ;

	case Office::msoTextBox :
		if ( ! shape->HasTextFrame )
		{
			return NULL ;
		}
		if ( 0 == shape->TextFrame->TextRange->Length )
		{
			return NULL ;
		}
		stratTextbox.SetShape( shape ) ;
		return static_cast< CShapeParsingStrategy* >( &stratTextbox ) ;

	case Office::msoTable :
		stratTable.SetShape( shape ) ;
		if (stratTable.is_exhausted())
		{
			return NULL ;
		}
		return static_cast< CShapeParsingStrategy* >( &stratTable ) ;

	default:
		/* It's something else, but if it has a text frame, then we can get
		   text from it! (duh)
	   */
		{
			if ( ! shape->HasTextFrame )
			{
				return NULL ;
			}
			if ( 0 == shape->TextFrame->TextRange->Length )
			{
				return NULL ;
			}
			stratAuto.SetShape( shape ) ;
			return static_cast< CShapeParsingStrategy* >( &stratAuto ) ;
		}
	}
}

void strategyFactory::dispose(CShapeParsingStrategy *)
{
}