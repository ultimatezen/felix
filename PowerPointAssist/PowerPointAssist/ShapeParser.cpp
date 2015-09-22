#include "StdAfx.h"
#include "ShapeParser.h"
#include "ShapeNavigator.h"

CShapeParser::CShapeParser(void)
: m_strategy(NULL), m_canceled_nav(false)
{
}

CShapeParser::~CShapeParser(void)
{
	strategyFactory::dispose( m_strategy ) ;
}

PowerPoint::TextRangePtr CShapeParser::nextTextRange( )
{
	if ( ! nextShape() )
	{
		return NULL ;
	} 

	return getNextShapeTextRange( );
}

PowerPoint::TextRangePtr CShapeParser::serveUpTextRange(PowerPoint::TextRangePtr& tr)
{
	BANNER( "CShapeParser::serveUpTextRange" ) ;

	if ( ! tr )
	{
		return tr ;
	}
	// we need this because trying to select the Note Page fails!
	try
	{
		// give a nice, fresh, unselected textrange!
		tr->Characters(1,0)->Select() ;
	}
	catch ( _com_error &e ) // select will throw if it's the notes page
	{
		ATLTRACE( "COM ERROR Selecting Text! -- %s\n", e.ErrorMessage() ) ;
		logging::log_error("Failed to initialize selection in text range (serveUpTextRange)") ;
		logging::log_exception(e) ;
	}

	return tr ;
}

PowerPoint::TextRangePtr CShapeParser::getNextShapeTextRange()
{
	BANNER( "CShapeParser::getNextShapeTextRange" ) ;

	PowerPoint::TextRangePtr tr = m_strategy->nextTextRange() ;

	return serveUpTextRange(tr);	
}

bool CShapeParser::noStrategy()
{
	return m_strategy == NULL ;
}

/*
 *	Get and select the next shape having a textframe. false if no more textframes found.
 */
bool CShapeParser::nextShape(void)
{
	strategyFactory::dispose( m_strategy ) ;

	// navigate to the next shape
	// get a new strategy based on the shape
	m_strategy = m_navigator.nextStrategy() ;
	m_canceled_nav = m_navigator.m_canceled_nav ;

	return ! noStrategy() ;
}
