#pragma once

#include "excel_application.h"

#ifdef _DEBUG

namespace excel
{
	class FakeRange : public range
	{
	public:
		int row ;
		int col ;
		_variant_t text ;

		int Row()
		{
			return row ;
		}
		int Column()
		{
			return col ;
		}
		_variant_t GetText()
		{
			return this->text ;
		}
		void SetText( _variant_t text_to_set )
		{
			this->text = text_to_set ;
		}

	};
	class FakeWorksheet : public worksheet
	{
	public:
		_bstr_t m_name ;
		int m_rows ;
		int m_cols ;

		std::vector< string > m_calls ;

		FakeWorksheet():
			m_rows(0),
			m_cols(0)
		{

		}
		_bstr_t GetName()
		{
			return m_name ;
		}

		void SetName(_bstr_t name)
		{
			m_name  = name ;
		}

		void Activate()
		{
			m_calls.push_back("Activate") ;
		}

		range_ptr Cell( int row, int col )
		{
			range_ptr cell = range_ptr(new range) ;
			return cell ;
		}

		virtual int UsedRows()
		{
			return m_rows ;
		}
		virtual int UsedColumns()
		{
			return m_cols ;
		}
	};

	//! Fake Excel characters class
	class FakeCharacters : public characters
	{
	public:
		_bstr_t text ;

		_bstr_t GetText()
		{
			return this->text ;
		}
		void SetText(_bstr_t text_to_set ) 
		{
			this->text = text_to_set ;
		}
	};

	//! Fake Excel application class.
	class FakeApplication : public application
	{
	public:
		sheet_ptr active_sheet ;
		chars_ptr chars ;
		range_ptr active_cell ;

		FakeApplication() :
			active_sheet(new FakeWorksheet)
			{

			}

		sheet_ptr GetActiveSheet()
		{
			return active_sheet ;
		}
		chars_ptr GetSelectedChars()
		{
			if (! this->chars )
			{
				throw _com_error(E_FAIL) ;
			}
			return this->chars ;
		}

		range_ptr GetSelectedCell()
		{
			if ( ! this->active_cell )
			{
				throw _com_error(E_FAIL) ;
			}
			return this->active_cell ;
		}
		range_ptr GetActiveCell()
		{
			if ( ! this->active_cell )
			{
				throw _com_error(E_FAIL) ;
			}
			return this->active_cell ;
		}
	};
}
#endif