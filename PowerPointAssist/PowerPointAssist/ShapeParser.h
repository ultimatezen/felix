#pragma once

#include "ShapeParsingStrategy.h"
#include "PowerPointImports.h"
#include "StrategyFactory.h"
#include "ShapeNavigator.h"

class CShapeParser
{

public:
	bool m_canceled_nav ;
	CShapeParser(void);
	~CShapeParser(void);

	PowerPoint::TextRangePtr nextTextRange( );
	void setApp( PowerPoint::_ApplicationPtr app )
	{
		m_app = app ;
	}

	bool nextShape(void);
	bool noStrategy();

	PowerPoint::_ApplicationPtr m_app ;
	parsingStrategy m_strategy ;
	CShapeNavigator m_navigator ;


	PowerPoint::TextRangePtr getNextShapeTextRange( );

	PowerPoint::TextRangePtr serveUpTextRange(PowerPoint::TextRangePtr& tr);
};
