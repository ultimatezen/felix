#pragma once

#include "ExcelImport.h"
#include "excel_range.h"
#include "excel_worksheet.h"
#include "excel_characters.h"

#include "DispatchWrapper.h"

namespace excel
{

class application
{
public:
	Excel::_ApplicationPtr m_pApplication ;

	// allow subclassing
	virtual ~application()
	{

	}

	virtual void set_app( Excel::_ApplicationPtr app )
	{
		m_pApplication = app ;
	}

	/*!
	* \brief
	* Gets the active sheet.
	*/
	virtual range_ptr GetActiveCell()
	{
		range_ptr active_cell = range_ptr(new range) ;
		active_cell->set_range(m_pApplication->GetActiveCell()) ;
		return active_cell ;
	}

	/*!
	* \brief
	* Gets the active sheet.
	*/
	virtual sheet_ptr GetActiveSheet()
	{
		sheet_ptr active_sheet = sheet_ptr(new worksheet) ;
		active_sheet->set_sheet(m_pApplication->ActiveSheet) ;
		return active_sheet ;
	}
	virtual sheets_ptr GetSheets()
	{
		sheets_ptr active_sheets = sheets_ptr(new worksheets) ;
		active_sheets->set_sheets(m_pApplication->Sheets) ;
		return active_sheets ;
	}

	/*!
	\brief
	Gets the current selection

	TODO:
	Change to interface
	*/
	virtual IDispatchPtr GetSelection()
	{
		return m_pApplication->Selection ;
	}

	virtual chars_ptr GetSelectedChars()
	{
		CDispatchWrapper selDisp = m_pApplication->Selection ;
		Excel::CharactersPtr chars = selDisp.prop_get( L"Characters" ).pdispVal ;
CComPtr<IDispatch> foo ;
		chars_ptr excel_chars(new characters) ;
		excel_chars->set_chars(chars) ;

		return excel_chars ;
	}

	virtual range_ptr GetSelectedCell()
	{
		Excel::RangePtr activeCell = m_pApplication->Selection ;
		range_ptr selection(new range) ;
		selection->set_range(activeCell) ;
		return selection ;
	}
	/*!
	 * \brief
	 * Gets the app workbooks.
	 * TODO:
	 change to interface
	 */
	virtual Excel::WorkbooksPtr GetWorkbooks()
	{
		return m_pApplication->Workbooks ;
	}

};

typedef boost::shared_ptr< application > app_ptr ;
}