#pragma once

#include "PowerPointImports.h"
#include "StrategyFactory.h"
#include "ShapeHolder.h"
#include "app_state.h"

class CShapeNavigator
{
	typedef PowerPoint::_ApplicationPtr appPtr ;
	typedef PowerPoint::ShapePtr  shapePtr ;

	appPtr	m_app ;

	holderSet m_holders ;
	bool m_lastWasNotesPage;

	app_state m_properties ;
	int m_last_nav_choice ;

public:
	CShapeNavigator( );
	~CShapeNavigator(void);

	parsingStrategy nextStrategy( ) ;

private:
	parsingStrategy firstShape( );
	void fillHolders(void);

	PowerPoint::ShapesPtr getActiveSlideShapes();
	PowerPoint::ShapePtr getSelectedShape();
	bool selectNextSlide(void);
	long getSlideIndex(PowerPoint::_SlidePtr& activeSlide);

	bool notesPaneSelected(void);

	parsingStrategy getStrategy(ShapePtr shape);
	parsingStrategy getNotesPageStrategy(void);
	parsingStrategy getStrategyFromNextPane();

	PowerPoint::SelectionPtr getSelection();
	ShapePtr getTableFromSelection(SelectionPtr selection);
	parsingStrategy nextShape(ShapePtr shape);
};
