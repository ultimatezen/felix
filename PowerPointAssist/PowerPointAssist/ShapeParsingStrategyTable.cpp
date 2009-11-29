#include "StdAfx.h"
#include "ShapeParsingStrategyTable.h"
#include "DispatchWrapper.h"

CShapeParsingStrategyTable::CShapeParsingStrategyTable(void)
{
}

CShapeParsingStrategyTable::~CShapeParsingStrategyTable(void)
{
}

PowerPoint::TextRangePtr CShapeParsingStrategyTable::nextTextRange( )
{
	if (is_exhausted())
	{
		return NULL ;
	}

	// If we're on the first cell
	if (m_currentCol == 0)
	{
		if (! advance_cell_pos())
		{
			return NULL ;
		}
		PowerPoint::TextRangePtr cell_range = m_table->Cell( m_currentRow, m_currentCol )->Shape->TextFrame->TextRange ;
		cell_range->Select() ;
		if ( cell_range->Length == 0 )
		{
			return nextTextRange() ;
		}
		else
		{
			return cell_range ;
		}
	}

	// If we have selected text inside the cell
	PowerPoint::SelectionPtr sel = m_table->Application->ActiveWindow->Selection ;
	if (sel->Type == PowerPoint::ppSelectionText)
	{
		// Is there text left in the cell to the right of the cursor?
		if (! current_cell_exhausted())
		{
			TextRangePtr sel_text_range = sel->TextRange ;
			PowerPoint::TextRangePtr cell_range = m_table->Cell( m_currentRow, m_currentCol )->Shape->TextFrame->TextRange ;

			int start_point = sel_text_range->Start + sel_text_range->Length ;

			TRACE(start_point) ;
			TRACE(cell_range->Length) ;

			if( cell_range->Length <= start_point )
			{
				return nextTextRange() ;
			}
			else
			{
				int end_point = cell_range->Length - start_point ;
				return cell_range->Characters(start_point, end_point) ;
			}
		}
		// Current cell is exhausted, so try moving to the next cell
		if (! advance_cell_pos())
		{
			return NULL ;
		}
		return nextTextRange() ;
	}

	// There isn't a text selection, so we'll grab the whole cell instead.
	PowerPoint::TextRangePtr cell_range = m_table->Cell( m_currentRow, m_currentCol )->Shape->TextFrame->TextRange ;
	TRACE(cell_range->Text) ;
	// If the cell is empty, move on to the next.
	if ( cell_range->Length == 0 )
	{
		if (! advance_cell_pos())
		{
			return NULL ;
		}
		return nextTextRange() ;
	}
	cell_range->Characters(1,0)->Select() ;
	return cell_range ;
}

/*
 We get passed in the shape when it's time to parse it.
 */
void CShapeParsingStrategyTable::SetShape( PowerPoint::ShapePtr shape )
{
	logging::log_debug("Setting current shape as table") ;
	m_shape = shape ;
	ATLASSERT( m_shape->HasTable ) ;
	m_table = m_shape->Table ;

	get_selected_table_pos();
}

/*
 Is there any text left in the table?
 */
bool CShapeParsingStrategyTable::is_exhausted()
{
	this->get_selected_table_pos() ;

	long num_rows = m_table->Rows->Count ;
	long num_cols = m_table->Columns->Count ;

	if (m_currentRow < num_rows || m_currentCol < num_cols)
	{
		return false ;
	}

	return current_cell_exhausted();

}

/*
 Find the selected cell in the table, or set to a default value
 if there are no selected cells.
 */
void CShapeParsingStrategyTable::get_selected_table_pos()
{
	PowerPoint::_ApplicationPtr app = m_table->Application ;
	PowerPoint::SelectionPtr selection = app->ActiveWindow->Selection ;

	// If we haven't selected the table yet (doesn't happen?),
	// then initialize to a special value (m_currentCol = 0) and
	// bail
	if (selection->Type == PowerPoint::ppSelectionSlides)
	{
		m_currentCol = 0 ;
		m_currentRow = 1 ;
		return ;
	}
	// Same as above, except watch out, because the selection might
	// not have a ShapeRange property, at which point we'll get a Dispatch
	// error.
	try
	{
		if ( Office::msoTable != selection->ShapeRange->Type )
		{
			m_currentCol = 0 ;
			m_currentRow = 1 ;
			return ;
		}
	}
	catch (_com_error &e)
	{
		logging::log_warn("Failed to retrieve ShapeRange object from selection") ;
		logging::log_exception(e) ;
	}

	// Find the selected cell in the table, if any.
	long num_rows = m_table->Rows->Count ;
	long num_cols = m_table->Columns->Count ;
	for ( m_currentRow = 1 ; m_currentRow <= num_rows ; ++m_currentRow )
	{
		for ( m_currentCol = 1 ; m_currentCol <= num_cols ; ++m_currentCol )
		{
			if (m_table->Cell( m_currentRow, m_currentCol )->Selected)
			{
				return ;
			}
		}
	}

	// This is the old method from PPT 2003. It doesn't
	// work with PPT 2007, hence the check for `frame`.
	if (selection->Type == PowerPoint::ppSelectionText)
	{
		TextFramePtr frame = selection->TextRange->Parent ;
		if (frame)
		{
			PowerPoint::ShapePtr shape = frame->Parent ;

			for ( m_currentRow = 1 ; m_currentRow <= num_rows ; ++m_currentRow )
			{
				for ( m_currentCol = 1 ; m_currentCol <= num_cols ; ++m_currentCol )
				{
					if ( shape == m_table->Cell( m_currentRow, m_currentCol )->Shape )
					{
						return ;
					}
				}
			}
		}
	}

	// reset to defaults
	m_currentRow = 1 ;
	m_currentCol = 0 ;
}

/* 
 Is there any text left in the text range, to the right of the cursor?

 Calculate from the end of the selection, so if the text is selected we'll advance
 to the next cell.
 */
bool CShapeParsingStrategyTable::current_cell_exhausted()
{
	TextRangePtr cell_range = m_table->Cell( m_currentRow, m_currentCol )->Shape->TextFrame->TextRange ;

	PowerPoint::_ApplicationPtr app = m_table->Application ;
	PowerPoint::SelectionPtr sel = app->ActiveWindow->Selection ;

	int start_point = sel->TextRange->Start + sel->TextRange->Length ;

	TRACE(start_point) ;
	TRACE(cell_range->Length) ;

	bool exhausted = ( cell_range->Length <= start_point ) ;
	TRACE(exhausted) ;
	if (exhausted)
	{
		logging::log_debug("End of current cell text reached") ;
	}
	return exhausted ;
}

/*
 Select the next cell in the table.
 Returns success (not at end of table)
 */
bool CShapeParsingStrategyTable::advance_cell_pos()
{
	++m_currentCol ;

	if ( m_currentCol > m_table->Columns->Count )
	{
		m_currentCol = 1 ;
		++m_currentRow ;
		if ( m_currentRow > m_table->Rows->Count )
		{
			return false ;
		}
	}
	PowerPoint::TextRangePtr cell_range = m_table->Cell( m_currentRow, m_currentCol )->Shape->TextFrame->TextRange ;
	// Select the start of the text range.
	cell_range->Characters(1,0)->Select() ;

	return true ;
}