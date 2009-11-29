#pragma once

#include "ExcelImport.h"
#include "excel_range.h"

namespace excel
{

	class worksheet
	{
	public:
		Excel::_WorksheetPtr m_sheet ;

		void set_sheet(Excel::_WorksheetPtr sheet)
		{
			m_sheet = sheet ;
		}

		virtual _bstr_t GetName()
		{
			return m_sheet->Name ;
		}
		virtual void SetName(_bstr_t name)
		{
			m_sheet->Name  = name ;
		}

		virtual void Activate()
		{
			m_sheet->Activate() ;
		}

		_variant_t GetCellFormula(int row, int col)
		{
			return this->Cell(row, col)->GetFormula() ;
		}
		virtual range_ptr Cell( int row, int col )
		{
			range_ptr cell = range_ptr(new range) ;
			cell->set_range(m_sheet->Cells->Item[ row][ col ]) ;
			return cell ;
		}

		virtual int UsedRows()
		{
			return m_sheet->UsedRange->Rows->Count ;
		}
		virtual int UsedColumns()
		{
			return m_sheet->UsedRange->Columns->Count ;
		}
	};

	typedef boost::shared_ptr< worksheet > sheet_ptr ;

	class worksheets
	{
	public:
		Excel::SheetsPtr m_sheets ;

		void set_sheets(Excel::SheetsPtr sheets)
		{
			m_sheets = sheets ;
		}
		virtual int Count()
		{
			return m_sheets->Count ;
		}
		virtual sheet_ptr Item(int item)
		{
			sheet_ptr active_sheet = sheet_ptr(new worksheet) ;
			active_sheet->set_sheet(m_sheets->Item[item]) ;
			return active_sheet ;
		}
		
	};

	typedef boost::shared_ptr< worksheets > sheets_ptr ;

}