/*!
 * Implemenation file for CFelixExcelIF
 * 
 * Copyright (c) 2005 by Ryan Ginstrom
 */
#include "StdAfx.h"
#include "FelixExcelIF.h"
#include "KeyboardDriver.h"

// classes for converting between various string types
#include "ExcelString.h"
#include "FelixString.h"

#include "Exceptions.h"
#include "DispatchWrapper.h"
#include "EasyUnitTester.h"

#include "easyunit/testharness.h"
#include <cmath>    // for fabs( )



const static int MAX_CELL_EXTEND = 30 ;
const static int ROW_SELECTION_INTERVAL = 10 ;
const static int MAX_ROWS_NO_ENTRY = 5 ;

bool double_equals(double left, double right, double epsilon=0.001)
{
	return (fabs(left - right) < epsilon);
}


#ifdef _DEBUG
	/*!
	 * Runs our unit tests.
	 */
	void CFelixExcelIF::OnUnitTest()
	{
		run_unit_tests() ;
	}
#endif

/*!
 * Default constructor.
 * 
 * Initializes the various class members here.
 */
CFelixExcelIF::CFelixExcelIF(LPCWSTR trans_hist_servername) :
	 m_shouldColor(false),
	 numTransSheet(0),
	 numNotTransSheet(0),
	 numTransBook(0),
	 numNotTransBook(0),
	 m_is_trans_mode(true),
	 m_trans_history(new CDispatchWrapper(trans_hist_servername))
{
	this->getAssistant = boost::bind( &CFelixExcelIF::realGetAssistant, this ) ;
	this->restore_cell_color = boost::bind( &CFelixExcelIF::real_restore_cell_color, this ) ;
}
 CFelixExcelIF::~CFelixExcelIF()
 {
	 if (m_trans_history)
	 {
		 delete m_trans_history ;
		 m_trans_history = NULL ;
	 }
 }
/*!
 * Automatically add a glossary to Felix.
 * 
 * Will skip up to three blank lines (hard coded!)
 */
void CFelixExcelIF::OnAutoAddGloss ( ) 
{
	try
	{
		ta_ptr assistant = getAssistant() ;

		excel::range_ptr activeCell = m_excel_app->GetActiveCell() ;

		excel::sheet_ptr activeSheet = GetActiveSheet() ;

		long iRow = activeCell->Row() ;
		long iCol = activeCell->Column() ;

		long numRowsWithoutEntry = 0 ;

		while ( numRowsWithoutEntry < MAX_ROWS_NO_ENTRY ) 
		{
			CExcelString sourceText, transText, contextText ;

			try
			{
				sourceText = activeSheet->Cell(iRow, iCol )->GetText() ;
			}
			catch (_com_error& e)
			{
				logging::log_error("COM exception getting cell text") ;
				logging::log_exception(e) ;
				sourceText = activeSheet->GetCellFormula(iRow, iCol ) ;
			}
			try
			{
				transText = activeSheet->Cell(iRow, iCol+1 )->GetText() ;
			}
			catch (_com_error& e)
			{
				logging::log_error("COM exception getting cell text") ;
				logging::log_exception(e) ;
				transText = activeSheet->GetCellFormula(iRow, iCol+1 ) ;
			}
			try
			{
				contextText = activeSheet->Cell(iRow, iCol+2 )->GetText() ;
			}
			catch (_com_error& e)
			{
				logging::log_error("COM exception getting cell text") ;
				logging::log_exception(e) ;
				contextText = activeSheet->GetCellFormula(iRow, iCol+2 ) ;
			}


			if ( sourceText.as_bstr().length() > 0 && transText.as_bstr().length() > 0 ) 
			{
				assistant->AddGlossaryEntry( sourceText.as_bstr(), 
												transText.as_bstr(), 
												contextText.as_bstr() ) ;

				numRowsWithoutEntry = 0 ;
			}
			else
			{
				numRowsWithoutEntry++ ;
			}

			iRow++ ;
		}

		excel::range_ptr final_cell = activeSheet->Cell(iRow, iCol) ;
		final_cell->Select() ;
	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("Glossary Registration Failure"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Initialization Error") ) ;
	}
}

/*!
* Automatically add a memory to Felix.
* 
* Will skip up to three blank lines (hard coded!)
*/
void CFelixExcelIF::OnAutoAddMem ( ) 
{
	try
	{
		ta_ptr assistant = getAssistant() ;

		excel::range_ptr activeCell = m_excel_app->GetActiveCell() ;

		excel::sheet_ptr activeSheet = GetActiveSheet() ;

		long iRow = activeCell->Row() ;
		long iCol = activeCell->Column() ;

		long numRowsWithoutEntry = 0 ;

		while ( numRowsWithoutEntry < MAX_ROWS_NO_ENTRY ) 
		{
			CExcelString sourceText, transText, contextText ;

			try
			{
				sourceText = activeSheet->Cell(iRow, iCol )->GetText() ;
			}
			catch (_com_error& e)
			{
				logging::log_error("COM exception getting cell text") ;
				logging::log_exception(e) ;
				sourceText = activeSheet->GetCellFormula(iRow, iCol ) ;
			}
			try
			{
				transText = activeSheet->Cell(iRow, iCol+1 )->GetText() ;
			}
			catch (_com_error& e)
			{
				logging::log_error("COM exception getting cell text") ;
				logging::log_exception(e) ;
				transText = activeSheet->GetCellFormula(iRow, iCol+1 ) ;
			}
			try
			{
				contextText = activeSheet->Cell(iRow, iCol+2 )->GetText() ;
			}
			catch (_com_error& e)
			{
				logging::log_error("COM exception getting cell text") ;
				logging::log_exception(e) ;
				contextText = activeSheet->GetCellFormula(iRow, iCol+2 ) ;
			}

			if ( sourceText.as_bstr().length() > 0 && transText.as_bstr().length() > 0 ) 
			{
				assistant->AddMemoryEntry( sourceText.as_bstr(), 
																			 transText.as_bstr(), 
																			 contextText.as_bstr() ) ;
				numRowsWithoutEntry = 0 ;
			}
			else
			{
				numRowsWithoutEntry++ ;
			}

			iRow++ ;
		}

		excel::range_ptr final_cell = activeSheet->Cell( iRow, iCol ) ;
		final_cell->Select() ;
	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("Memory Registration Failure"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Initialization Error") ) ;
	}
}
/*!
 * Responds to lookup command.
 * 
 * Gets the current cell text, and sends it to Felix as a query.
 */
void CFelixExcelIF::OnLookup( ) 
{
	try
	{
		ta_ptr assistant = getAssistant() ;

		remember_active_sheet(GetActiveSheet()) ;

		if (this->is_trans_mode())
		{
			assistant->SetQuery(get_selection_text(m_excel_app));
		}
		else
		{
			if (m_properties->m_use_trans_hist)
			{
				m_trans_history->method(L"LookupTrans") ;
			}
			else
			{
				assistant->LookupTrans(get_selection_text(m_excel_app)) ;
			}
		}
	}
	catch( _com_error &e ) 
	{
		CString message = _T("Failed to look up cell text") ;
		CComException com_exception(e) ;
		com_exception.notify_user(message, MB_OK, _T("COM Error"));
	}
}

/*!
 * Navigates to the next sheet, if any.
 *
 * If there is a next sheet, performs \ref OnLookupNext()
 */
void CFelixExcelIF::checkNextSheet()
{
	excel::sheet_ptr activeSheet = GetActiveSheet() ;

	excel::sheets_ptr sheets = m_excel_app->GetSheets() ;
	int NumSheets = sheets->Count() ;

	for ( int CurrentSheet = 1 ; CurrentSheet <= NumSheets ; ++CurrentSheet )
	{
		excel::sheet_ptr sheet = sheets->Item(CurrentSheet) ;

		if ( sheet == activeSheet && CurrentSheet + 1 <= NumSheets )
		{
			// Why?
			if ( m_shouldColor )
			{
				m_ColoredCells.clear() ;
			}
			sheet = sheets->Item(CurrentSheet+1) ;
			sheet->Activate() ;
			m_CurrentSheetName = BSTR2wstring( sheet->GetName() ) ;
			TRACE( m_CurrentSheetName ) ;
			excel::range_ptr activeCell = sheet->Cell( 1 , 1 ) ;
			activeCell->Select() ;
			OnLookupNext() ;
			return ;
		}
	}
}


/*!
 * Finds the next non-empty cell, and looks it up in TA.
 */
void CFelixExcelIF::OnLookupNext( ) 
{
	try
	{
		excel::range_ptr activeCell = m_excel_app->GetActiveCell() ;

		excel::sheet_ptr activeSheet = GetActiveSheet() ;

		int UsedRows = max( activeSheet->UsedRows(), activeCell->Row() + MAX_CELL_EXTEND ) ;
		ATLASSERT( UsedRows > activeCell->Row() ) ;

		int UsedColumns = max( activeSheet->UsedColumns(), MAX_CELL_EXTEND ) ;
		ATLASSERT( UsedColumns > activeCell->Column() ) ;

		long startCol = activeCell->Column() + 1;

		for ( int iRow = activeCell->Row() ; iRow <= UsedRows ; ++iRow )
		{
			for ( int iCol = startCol ; iCol <= UsedColumns ; ++iCol )
			{
				excel::range_ptr activeCell = activeSheet->Cell( iRow, iCol ) ;
				CExcelString lookupText = activeCell->GetFormula() ;

				if ( ! lookupText.empty() && ! IsIgnoreCell(BSTR2wstring(lookupText.as_bstr()) ) )
				{
					activeCell->Select() ;
					OnLookup() ;
					return ;
				}
			}
			startCol = 1 ;
		}

		checkNextSheet() ;

	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("COM Error"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Initialization Error") ) ;
	}
}

/*!
 * Sets the current selection/cell contents as the translation.
 */
void CFelixExcelIF::OnSet( ) 
{
	try
	{
		ta_ptr assistant = getAssistant() ;

		remember_active_sheet(GetActiveSheet()) ;

		if (this->is_trans_mode())
		{
			assistant->SetTrans(this->get_selection_text(m_excel_app)) ;
			if (m_properties->m_use_trans_hist)
			{
				m_trans_history->method(L"RecordTrans") ;
			}
		}
		else
		{
			if (m_properties->m_use_trans_hist)
			{
				m_trans_history->method(L"CorrectTrans") ;
			}
			else
			{
				assistant->CorrectTrans(this->get_selection_text(m_excel_app)) ;
			}
		}


		restore_cell_color();
	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("COM Error"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Initialization Error") ) ;
	}
}

/*!
 * Shortcut for OnSet(); OnLookupNext()
 */
void CFelixExcelIF::OnSetAndNext( ) 
{
		OnSet() ;
		OnLookupNext() ;
}

/*!
 * Retrieves the current translation from TA, and sets it in the active selection/cell.
 */
void CFelixExcelIF::OnGet( ) 
{
	try
	{
		ta_ptr assistant = getAssistant() ;

		CFelixString trans = assistant->GetTrans() ;

		set_cell_text(assistant->GetTrans()) ;

		restore_cell_color();

	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("COM Error"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Error retrieving translation from Felix") ) ;
	}
}

/*!
 * Equivalent to OnGet() ; OnLookupNext()
 */
void CFelixExcelIF::OnGetAndNext( ) 
{
	OnGet() ;
	OnLookupNext() ;
}

/*!
 * Auto-translated the current selection range.
 */
void CFelixExcelIF::OnAutoTransSel ( ) 
{
	BANNER( "CFelixExcelIF::OnAutoTransSel" ) ;
	numTransSheet = 0 ;
	numNotTransSheet = 0 ;

	m_shouldColor = !!( ::GetKeyState(VK_SHIFT) & 0x8000 ) ;
	if (m_shouldColor)
	{
		m_ColoredCells.clear() ;
	}

	try
	{
		ta_ptr assistant = getAssistant() ;

		excel::sheet_ptr activeSheet = GetActiveSheet() ;

		CDispatchWrapper selectionDispatch(m_excel_app->GetSelection()) ;
		long iCount = selectionDispatch.prop_get( L"Count" ).lVal ;

		for ( long i=1 ; i<= iCount ; ++i ) 
		{
			Excel::RangePtr activeCell = selectionDispatch.prop_get( L"Item", CComVariant(i) ).pdispVal ;
			excel::range_ptr range = excel::range_ptr(new excel::range) ;
			range->set_range(activeCell) ;
			AutoTransCell( range, assistant );
		}

	}
	catch( _com_error &e ) 
	{
		CComException com_e(e) ;
		com_e.notify_user(_T("Auto Translation Error\r\nError calling Excel object via COM dispatch.")) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Auto Translation Error") ) ;
	}
}

/*!
 * Retrieves a smart pointer for the Felix server.
 */
ta_ptr CFelixExcelIF::realGetAssistant(void)
{
	// If we have previously created the server, but the server
	// went away (like if the user quit Felix), then
	// we'll get an error when we try to call any of its methods.
	if ( m_Assistant )
	{
		try
		{
			m_Assistant->Visible = VARIANT_TRUE ;
		}
		// So here, we try to access it, and if we get any kind of exception,
		// we created the COM server again, and return that.
		catch (...)
		{
			Felix::IAppPtr assistant ;
			assistant.CreateInstance(L"Felix.App" ) ;
			m_Assistant = assistant ;
			m_Assistant->Visible = VARIANT_TRUE ;
		}
	}
	else
	{
		m_Assistant.CreateInstance( L"Felix.App" ) ;
		m_Assistant->Visible = VARIANT_TRUE ;
	}
	
	ta_ptr ta = ta_ptr( new FelixApp ) ;
	ta->set_app(m_Assistant) ;
	return ta ;
}

/*!
 * Auto translate the current worksheet.
 *
 * This does the should_color check, then delegates to the actual worker method.
 */
void CFelixExcelIF::OnAutoTransSheet()
{
	BANNER( " CFelixExcelIF::OnAutoTransSheet" ) ;
	m_shouldColor = !!( ::GetKeyState(VK_SHIFT) & 0x8000 ) ;
	if (m_shouldColor)
	{
		m_ColoredCells.clear() ;
	}

	PerformSheetAutoTrans();
}


/*!
 * Auto translate all open workbooks.
 */
void CFelixExcelIF::OnAutoTransWorkbook()
{
	BANNER( " CFelixExcelIF::OnAutoTransWorkbook" ) ;
	numTransBook = 0 ;
	numNotTransBook = 0 ;
	m_shouldColor = !!( ::GetKeyState(VK_SHIFT) & 0x8000 ) ;
	if (m_shouldColor)
	{
		m_ColoredCells.clear() ;
	}
	try
	{
		ta_ptr assistant = getAssistant() ;

		Excel::WorkbooksPtr books = m_excel_app->GetWorkbooks() ;

		int numBooks = books->Count ;

		for ( int currentBook = 1 ; currentBook <= numBooks ; ++currentBook )
		{
			Excel::_WorkbookPtr book = books->Item[currentBook] ;

			book->Activate() ;

			// will be replaced by workbook ptr...
			Excel::SheetsPtr esheets = book->Sheets ;
			excel::sheets_ptr sheets = excel::sheets_ptr( new excel::worksheets ) ;
			sheets->set_sheets(esheets) ;
			int NumSheets = sheets->Count() ;

			for ( int CurrentSheet = 1 ; CurrentSheet <= NumSheets ; ++CurrentSheet )
			{
				excel::sheet_ptr sheet = sheets->Item(CurrentSheet) ;
				sheet->Activate() ;

				PerformSheetAutoTrans() ;
				numTransBook += numTransSheet ;
				numNotTransBook += numNotTransSheet ;
			}

			excel::sheet_ptr sheet = sheets->Item(1) ;
			sheet->Activate() ;
			excel::range_ptr activeCell = sheet->Cell( 1 , 1 ) ;
			activeCell->Select() ;

		}

	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("COM Error"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Auto Translation Error") ) ;
	}

	CString msg ;
	msg.Format( _T("Auto Trans Workbook Done.\r\n\r\nSegments translated: %d\r\nSegs not translated: %d"), numTransBook, numNotTransBook ) ;
	MessageBox( NULL, msg, _T("ExcelAssist"), MB_OK ) ;
}

/*!
 * Sends "next match" message to Felix.
 */
void CFelixExcelIF::OnNext()
{
	try
	{
		ta_ptr assistant = getAssistant() ;
		assistant->NextTrans() ;
	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("COM Error"), MB_OK ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Error navigating to next translation in Felix") ) ;
	}
}

/*!
 * Sends "previous match" request to Felix.
 */
void CFelixExcelIF::OnPrev()
{
	try
	{
		ta_ptr assistant = getAssistant() ;
		assistant->PrevTrans() ;
	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("COM Error"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Error navigating to previous translation in Felix") ) ;
	}
}

/*!
 * Retrieve the glossary entry number and write it to the current cell.
 */
void CFelixExcelIF::OnEntry( int num )
{
	try
	{
		ta_ptr assistant = getAssistant() ;

		_bstr_t entry = assistant->GlossMatch( num ) ;
		TRACE( entry ) ;

		set_cell_text(entry) ;
	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("COM Error"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Error setting glossary entry") ) ;
	}
}

/*!
 * Should we ingore this cell when looking for stuff to translate?
 * 
 * We ignore empty cells, formulas, and numbers
 */
bool CFelixExcelIF::IsIgnoreCell( const wstring &text )
{
	// empty or formula
	if ( text.empty() || text[0] == L'=' )
	{
		ATLTRACE( "%ls is a formula; skipping\n", text.c_str() ) ;
		return true ;
	}

	if (m_properties->m_skipNumbers)
	{
		// number
		wstring tmp( text ) ;
		str::normalize_width( tmp ) ;
		if ( str::is_number( tmp ) ) 
		{
			ATLTRACE( "%ls is a number; skipping\n", text.c_str() ) ;
			return true ;
		}
	}

	// double-byte characters
	if (m_properties->get_segmentation_type() == SKIP_IF_J)
	{
		foreach( wchar_t ch, text )
		{
			if ( str::is_double_byte( ch ) )
			{
				return true ;
			}
		}
	}
	else if (m_properties->get_segmentation_type() == SKIP_UNLESS_J)
	{
		bool hasJ = false ;
		foreach( wchar_t ch, text )
		{
			if ( str::is_double_byte( ch ) )
			{
				hasJ = true ;
				break ;
			}
		}
		if (! hasJ)
		{
			return true ;
		}
	}

	return false ;
}
/*!
 * Auto translate activeCell.
 */
void CFelixExcelIF::AutoTransCell( excel::range_ptr activeCell, ta_ptr assistant )
{
	try
	{
		if ( activeCell->FormulaType() != VT_BSTR )
		{
			return ;
		}
		CExcelString lookupText = activeCell->GetFormula() ;

		if ( lookupText.empty() ) 
		{
			return ;
		}

		if ( IsIgnoreCell(BSTR2wstring(lookupText.as_bstr()) ) )
		{
			return ;
		}

		assistant->SetQuery( lookupText.as_bstr() ) ;

		if ( ! double_equals(assistant->GetScore(), 1.0) ) // || ! isAcceptableCell )
		{
			numNotTransSheet++ ;

			if ( m_shouldColor )
			{
				ATLTRACE( "No match -- coloring cell...\n" ) ;
				int color = activeCell->FillColor() ;

				m_ColoredCells[get_cell_loc(m_CurrentSheetName, activeCell)] = color ;

                // If the cell is already colored "33" (turquoise-ish),
                // then we color it 32 (dark blue) so there will still be a 
                // contrast
				if ( color == 33 )
				{
					activeCell->SetFillColor( 32 );
				}
				else
				{
					activeCell->SetFillColor( 33 );
				}

			}
			return ;
		}

		CFelixString trans = assistant->GetTrans() ; // string2BSTR( text.c_str() ) ;
		activeCell->SetFormula( trans.as_variant() );

		restoreColor(activeCell);
		numTransSheet++ ;
	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("COM Error"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Error processing worksheet cell") ) ;
	}
	catch ( CAtlException &atl )
	{
		atl ;
		TRACE_HRESULT( atl.m_hr ) ;
		ATLTRACE( "We don't need to tell the user about this...\n" ) ;
		return ;
	}
}

/*!
 * Restore the cell's background color.
 */
void CFelixExcelIF::restoreColor( excel::range_ptr activeCell )
{
	BANNER( "CFelixExcelIF::restoreColor" ) ;

	cell_loc id =  get_cell_loc(m_CurrentSheetName, activeCell) ;

	if ( m_ColoredCells.find( id ) != m_ColoredCells.end() ) // we have colored this cell before...
	{
		ATLTRACE( "We have a match! New color index = %d\n", m_ColoredCells[id] ) ;
		activeCell->SetFillColor( m_ColoredCells[id] );
	}
}


/*!
 * Auto translate the entire sheet.
 */
void CFelixExcelIF::PerformSheetAutoTrans()
{
	BANNER( " CFelixExcelIF::PerformSheetAutoTrans" ) ;
	numTransSheet = 0 ;
	numNotTransSheet = 0 ;

	try
	{
		if (this->is_trans_mode())
		{
			ta_ptr assistant = getAssistant() ;

			excel::sheet_ptr activeSheet = GetActiveSheet() ;

			TRACE( activeSheet->GetName() ) ;
			autoTransSheetName(activeSheet, assistant);

			// Now do the sheet cells
			int UsedRows = max( activeSheet->UsedRows(),  m_excel_app->GetActiveCell()->Row() + MAX_CELL_EXTEND ) ;
			int UsedColumns = max( activeSheet->UsedColumns(), MAX_CELL_EXTEND ) ;

			for ( int iRow = 1 ; iRow <= UsedRows ; ++iRow )
			{
				for ( int iCol = 1 ; iCol <= UsedColumns ; ++iCol )
				{
					excel::range_ptr activeCell = activeSheet->Cell( iRow, iCol ) ;
					AutoTransCell( activeCell, assistant );
				}

				if ( iRow % ROW_SELECTION_INTERVAL == 0 )
				{
					excel::range_ptr activeCell = activeSheet->Cell( iRow , 1 ) ;
					activeCell->Select() ;
					// maybe we found another used range...
					UsedRows = max( activeSheet->UsedRows(), UsedRows ) ;
				}

				// sanity check...
				ATLASSERT ( UsedRows < 10000 ) ;
			}

			excel::range_ptr topLeftCell = activeSheet->Cell( 1 , 1 ) ;
			topLeftCell->Select() ;
		}
		else if (this->m_properties->m_use_trans_hist)
		{
			m_trans_history->method(L"ReflectChanges") ;
		}
	}
	catch( _com_error &e ) 
	{
		MessageBox( NULL, e.ErrorMessage(), _T("COM Error"), MB_OK | MB_ICONSTOP ) ;
	}
	catch( CException &myException )
	{
		myException.notify_user( _T("Auto Translation Error") ) ;
	}
}

/*!
 * \brief
 * Get the cell location.
  * 
 * The cell location is a combination of the sheet name and a cellCoords object,
 * which represents the row and column coordinates.
 * Together, these give us a unique location within the workbook, which we
 * can use to restore the cell background color after we translate it.
 */
CFelixExcelIF::cell_loc CFelixExcelIF::get_cell_loc( const wstring &name, excel::range_ptr activeCell )
{
	BANNER( "CFelixExcelIF::getCellLoc" ) ;
	int row = activeCell->Row() ;
	int col = activeCell->Column() ;

	return cell_loc( name, cellCoords( row, col ) ) ;
}

/*!
 * Retrieves the active sheet.
 */
excel::sheet_ptr CFelixExcelIF::GetActiveSheet()
{
	BANNER( "CFelixExcelIF::GetActiveSheet" ) ;
	excel::sheet_ptr activeSheet = m_excel_app->GetActiveSheet() ;

	ATLASSERT( activeSheet ) ;

	remember_active_sheet(activeSheet) ;

	return activeSheet ;
}

/*!
 * The connection object sets the Excel app here.
 */
void CFelixExcelIF::setApp( excel::app_ptr excelApp )
{
	m_excel_app = excelApp ;
}

//! Auto-translates the sheet name.
void CFelixExcelIF::autoTransSheetName( excel::sheet_ptr activeSheet, ta_ptr assistant )
{
	wstring sheetName = BSTR2wstring( activeSheet->GetName() ) ;
	if ( ! IsIgnoreCell( sheetName ) )
	{
		assistant->SetQuery( activeSheet->GetName() );
		if ( double_equals(assistant->GetScore(), 1.0) )
		{
			activeSheet->SetName( assistant->GetTrans() );
			this->m_CurrentSheetName = BSTR2wstring( activeSheet->GetName() ) ;
		}
	}
}


/*!
 * Keep a record of the active sheet, to fill in our color map.
 */
void CFelixExcelIF::remember_active_sheet(excel::sheet_ptr activeSheet)
{
	// Remember the current sheet name for the color map
	m_CurrentSheetName = BSTR2wstring( activeSheet->GetName() ) ;

	TRACE( m_CurrentSheetName ) ;
}


//! Gets the text from the active shape/cell
_bstr_t CFelixExcelIF::get_selection_text( excel::app_ptr application )
{
	CExcelString lookupText ;

	try
	{
		lookupText = application->GetSelectedChars()->GetText() ;
	}
	catch ( _com_error &e ) 
	{
		ATLTRACE("_com_error -- Failed to get text from Selection->Characters\n") ;
		CComException cce(e) ;
		ATLTRACE(cce.format_message_for_message_box()) ;

		excel::range_ptr activeCell = application->GetActiveCell() ;
		lookupText = activeCell->GetText() ;
	}

	return lookupText.as_bstr() ;
}

/*!
 * Sets the text in the active shape/cell.
 */
void CFelixExcelIF::set_cell_text( _bstr_t text )
{
	CFelixString trans = text ;

	// First we try writing to the Selection.
	try
	{
		m_excel_app->GetSelectedChars()->SetText(trans.as_bstring()) ;
	}
	// If that fails, we try writing to the active cell
	catch ( ... ) 
	{
		ATLTRACE( "Failed to set the Characters.Text -- setting the formula instead..." ) ;
		excel::range_ptr activeCell = m_excel_app->GetActiveCell() ;
		activeCell->SetFormula( trans.as_variant() ) ;
	}
}

//! Restores the original cell color.
//! Called after we get or set a translation.
void CFelixExcelIF::real_restore_cell_color()
{
	try
	{
		// We should only do this if we have a cell selected.
		restoreColor( m_excel_app->GetSelectedCell() );
	}
	catch ( CComException &e ) 
	{
		logging::log_exception(e) ;
		ATLTRACE( "Failed to restore the cell color from the selection. That must mean that the selection isn't a cell!\n" ) ;
	}
	catch( _com_error &e )
	{
		logging::log_exception(e) ;
		ATLTRACE( "_com_error -- Failed to restore the cell color from the selection. That must mean that the selection isn't a cell!\n" ) ;
	}
	//catch (...) 
	//{
	//	ATLASSERT( "Unknown error restoring cell text!" && FALSE ) ;
	//	ATLTRACE( "Unknown error...\n"
	//							"Failed to restore the cell color from the selection.\n"
	//							"That must mean that the selection isn't a cell!" ) ;
	//}
}

void CFelixExcelIF::setProperties( app_state *properties )
{
	m_properties = properties ;
}

void CFelixExcelIF::set_trans_mode()
{
	m_is_trans_mode = true ;
}

void CFelixExcelIF::set_review_mode()
{
	m_is_trans_mode = false ;
}

bool CFelixExcelIF::is_trans_mode()
{
	return m_is_trans_mode ;
}

void CFelixExcelIF::set_app_ptr( LPDISPATCH app )
{
	VARIANT var ;
	var.vt = VT_DISPATCH ;
	var.pdispVal = app ;
	m_trans_history->prop_put(L"App", var) ;
}

void CFelixExcelIF::shut_down()
{
	if (m_trans_history)
	{
		delete m_trans_history ;
		m_trans_history = NULL ;
	}
}

void CFelixExcelIF::close_workbook( IDispatch* workbook )
{
	VARIANT var ;
	var.vt = VT_DISPATCH ;
	var.pdispVal = workbook ;
	m_trans_history->method(L"ShutDown", var) ;
}


/************************************************************************/
/* Unit tests
/************************************************************************/

#ifdef _DEBUG

namespace easyunit
{
	TEST( double_equalsTest, basic)
	{
		ASSERT_TRUE(double_equals( 1.0, 0.2*5.0 ) ) ;
		ASSERT_TRUE(double_equals( 0.5, 0.1 * 5.0 ) ) ;
		ASSERT_TRUE( ! double_equals( 1.0, 0.99 ) ) ;
	}
}

#endif