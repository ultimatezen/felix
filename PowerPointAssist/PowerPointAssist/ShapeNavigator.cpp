#include "StdAfx.h"
#include "ShapeNavigator.h"
#include "globalApp.h"
#include "nav_query_dlg.h"

CShapeNavigator::CShapeNavigator( )
: m_lastWasNotesPage(false)
{
	m_properties.read_from_registry() ;
	if (m_properties.m_data.m_navigation_type == NAV_TYPE_SLIDE_TOP)
	{
		m_last_nav_choice = IDC_SLIDE_TOP ;
	}
	else
	{
		m_last_nav_choice =  IDC_NEXT_SLIDE ;
	}
	
}

CShapeNavigator::~CShapeNavigator(void)
{
}

parsingStrategy CShapeNavigator::nextStrategy( )
{
	m_app = globalApp::get() ;

	PowerPoint::SelectionPtr selection = getSelection() ;
	TRACE(selection->Type) ;
	switch( selection->Type )
	{
	case ppSelectionNone:
	case ppSelectionSlides:
		return firstShape() ;
	case ppSelectionShapes:
		{
			ShapeRangePtr srange = selection->ShapeRange ;
			ShapePtr shape = srange->Item( 1 ) ;
			return nextShape( shape ) ;
		}
	case ppSelectionText:
		{
			ShapePtr tableShape = getTableFromSelection( selection ) ;

			if ( tableShape )
			{
				parsingStrategy strat = strategyFactory::create( tableShape ) ;
				if (strat)
				{
					return strat ;
				}
			}
			return nextShape( getSelectedShape() ) ;
		}
	default:
		ATLASSERT( FALSE && "Unknown selection type!" ) ;
		return NULL ;
	}
}

parsingStrategy CShapeNavigator::getStrategyFromNextPane()
{
	m_properties.read_from_registry() ;
	int nav_pref = m_properties.get_navigation_type() ;

	if (nav_pref == NAV_TYPE_ASK)
	{
		if (m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE)
		{
			CNavQueryDlgJ dlg ;
			dlg.m_nav_type = m_last_nav_choice ;
			if (IDCANCEL != dlg.DoModal())
			{
				if (dlg.m_nav_type == IDC_NEXT_SLIDE)
				{
					nav_pref = NAV_TYPE_NEXT_SLIDE ;
				}
				else
				{
					nav_pref = NAV_TYPE_SLIDE_TOP ;
				}
				if (dlg.m_dont_ask)
				{
					m_properties.set_navigation_type(nav_pref) ;
					m_properties.write_to_registry() ;
				}
				m_last_nav_choice = dlg.m_nav_type ;
			}
			else
			{
				return NULL ;
			}
		} 
		else
		{
			CNavQueryDlgE dlg ;
			dlg.m_nav_type = m_last_nav_choice ;
			if (IDCANCEL != dlg.DoModal())
			{
				if (dlg.m_nav_type == IDC_NEXT_SLIDE)
				{
					nav_pref = NAV_TYPE_NEXT_SLIDE ;
				}
				else
				{
					nav_pref = NAV_TYPE_SLIDE_TOP ;
				}
				if (dlg.m_dont_ask)
				{
					m_properties.set_navigation_type(nav_pref) ;
					m_properties.write_to_registry() ;
				}
				m_last_nav_choice = dlg.m_nav_type ;
			}
			else
			{
				return NULL ;
			}
		}
	}
	if (nav_pref == NAV_TYPE_NEXT_SLIDE)
	{
		if ( ! selectNextSlide() )
		{
			return NULL ;
		}
	}
	return firstShape() ;

}

PowerPoint::ShapePtr CShapeNavigator::getSelectedShape()
{
	_variant_t item( 1 ) ;
	item.vt = VT_I4 ;
	return getSelection()->ShapeRange->Item( item ) ;
}

PowerPoint::SelectionPtr CShapeNavigator::getSelection()
{
	return m_app->ActiveWindow->Selection;
}

parsingStrategy CShapeNavigator::firstShape(void)
{
	fillHolders() ;

	if ( m_holders.empty() == false )
	{
		return getStrategy( m_holders.begin()->GetShape() ) ;
	}

	return NULL ;
}

void CShapeNavigator::fillHolders(void)
{
	m_holders.clear() ;

	PowerPoint::ShapesPtr shapes = getActiveSlideShapes();
	TRACE( shapes->Count ) ;

	long nextShapeIndex = 1 ;
	for ( nextShapeIndex = 1 ; nextShapeIndex <= shapes->Count ; ++nextShapeIndex ) 
	{
		PowerPoint::ShapePtr shape = shapes->Item( nextShapeIndex ) ;
		TRACE( shape->Name ) ;
		parsingStrategy strat = strategyFactory::create( shape ) ;
		// add only shapes with valid strategies
		if ( strat != NULL )
		{
			CShapeHolder shapeHolder( shape ) ;
			m_holders.insert( shapeHolder ) ;
			strategyFactory::dispose( strat ) ;
		}
	}

	TRACE( m_holders.size() ) ;

}

PowerPoint::ShapesPtr CShapeNavigator::getActiveSlideShapes()
{
	PowerPoint::_SlidePtr slide = m_app->ActiveWindow->View->Slide ;
	return slide->Shapes ;
}

bool CShapeNavigator::selectNextSlide(void)
{
	PowerPoint::_SlidePtr activeSlide = m_app->ActiveWindow->View->Slide ; // Selection->SlideRange->Item(1) ;

	long index = getSlideIndex( activeSlide ) ;

	// if we are not on the last slide, select the next one
	// and return true
	if ( index < m_app->ActivePresentation->Slides->Count )
	{
		m_app->ActivePresentation->Slides->Item( index + 1 )->Select() ;

		fillHolders() ;
		return true ;
	}

	// otherwise, there are no more slides left to select.
	// return false
	return false ;
}

long CShapeNavigator::getSlideIndex(PowerPoint::_SlidePtr& activeSlide)
{
	PowerPoint::SlidesPtr slides = m_app->GetActivePresentation()->GetSlides() ;
	
	if ( 0 == slides->Count )
	{
		return 0 ;
	}

	for ( long index = 1 ; index <= slides->Count ; ++index )
	{
		if ( activeSlide == slides->Item( index ) )
		{
			return index ;
		}
	}
	return 1 ;
}

parsingStrategy CShapeNavigator::getStrategy(ShapePtr shape)
{
	m_lastWasNotesPage = false ;
	shape->Select( Office::msoTrue ) ;
	return strategyFactory::create( shape ) ;
}

bool CShapeNavigator::notesPaneSelected(void)
{
	return ( ppViewNotesPage == m_app->ActiveWindow->ActivePane->ViewType ) ;
}

parsingStrategy CShapeNavigator::getNotesPageStrategy(void)
{
	PowerPoint::_SlidePtr activeSlide = m_app->ActiveWindow->View->Slide ;

	PowerPoint::SlideRangePtr notesPage = activeSlide->NotesPage ;

	PowerPoint::ShapesPtr shapes = notesPage->Shapes ;

	for ( int i = 1 ; i <= shapes->Count ; ++i )
	{
		PowerPoint::ShapePtr shape = shapes->Item(i) ;

		if ( shape->PlaceholderFormat->Type == ppPlaceholderBody && shape->HasTextFrame == Office::msoTrue )
		{
			if ( shape->TextFrame->TextRange->Text == _bstr_t(L"") )
			{
				return strategyFactory::create( NULL ) ;
			}
			m_lastWasNotesPage = true ;
			return strategyFactory::create( shape ) ;
		}
	}

	return strategyFactory::create( NULL ) ;
}

ShapePtr CShapeNavigator::getTableFromSelection(SelectionPtr selection)
{
	if ( PowerPoint::ppSelectionText != selection->Type )
	{
		return NULL ;
	}

	ShapeRangePtr shapeRange = selection->ShapeRange ;

	if ( Office::msoTable != shapeRange->Type )
	{
		return NULL ;
	}

	return shapeRange->Table->Parent ;
}

parsingStrategy CShapeNavigator::nextShape(ShapePtr shape)
{
	fillHolders() ;

	CShapeHolder holder( shape ) ;
	// make sure it's there!
	// WE NEED THIS -- because the currently selected shape may not have
	// a text range
	m_holders.insert( holder ) ;
	holderSet::iterator pos = m_holders.find( holder ) ;
	ATLASSERT( pos != m_holders.end() ) ;
	++pos ;
	if ( pos == m_holders.end() )
	{
		return getStrategyFromNextPane();
	}

	return getStrategy( pos->GetShape() ) ;
}
