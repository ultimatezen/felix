#include "StdAfx.h"
#include ".\excelexporter.h"

#include "file.h"
#include "logging.h"

using namespace except ;
using namespace mem_engine ;

// So our text doesn't show up as (broken) formulas.
wstring process_cell_text(const wstring text)
{
	if (text.empty())
	{
		return text ;
	}
	if (text[0] == L'=')
	{
		return L"\'" + text ;
	}
	return text ;
}

CExcelExporter::CExcelExporter( CProgressListener* listener ) : 
	m_needs_quitting(false),
	m_listener(listener),
	m_ignore_errors(false)
{
	m_app.CreateInstance( L"Excel.Application" ) ;

	Excel::WorkbooksPtr wBooks = m_app->Workbooks ;

	long wbCount = wBooks->GetCount() ;

	if ( wbCount == 0 ) 
	{
		m_needs_quitting = true ;
	}

	m_book = wBooks->Add( ) ;
	m_sheet = m_book->GetActiveSheet() ;

}

CExcelExporter::~CExcelExporter(void)
{
}

void CExcelExporter::export_excel(memory_pointer mem, const CString& mem_name)
{
	m_listener->OnProgressInit( mem_name, 1, mem->size() ) ;

	CTranslationMemory::iterator_type pos ;
	
	write_header() ;

	int row_count = 1 ;

	foreach(record_pointer rec, mem->get_records()) 
	{
		try
		{
			write_record( rec, row_count + 1 ) ;
		}
		catch (_com_error& e)
		{
			if (! m_ignore_errors)
			{
				CString message(_T("Failed to write Felix record to Excel file\r")) ;
				message += _T("Source: ") ;
				message += rec->get_source_plain().c_str() ;
				message += _T("\r") ;
				message += _T("Trans:  ") ;
				message += rec->get_trans_plain().c_str() ;
				message += _T("\r") ;

				CComException com_exp(e) ;

				com_exp.set_bottom_message(_T("\rContinue Saving [OK], or Cancel.")) ;

				CString title(_T("Excel Write Error")) ;
				if (IDCANCEL == com_exp.notify_user(message, MB_OKCANCEL, title))
				{
					throw CComException(e) ;
				}

				m_ignore_errors = true ;
			}
		}

		m_listener->OnProgressWriteUpdate( row_count ) ;
		row_count++ ;
	}

	// delete the file if it exists
	if ( file::file::exists( mem_name ) )
	{
		file::file::delete_file( mem_name ) ;
	}

	const _bstr_t save_loc( mem_name ) ;

	_variant_t file_format(Excel::xlWorkbookNormal) ;
	m_book->SaveAs( save_loc, 
		file_format,
		vtMissing,
		vtMissing,
		vtMissing,
		vtMissing,
		Excel::xlNoChange) ;

	m_listener->OnProgressDoneWrite( mem->size() ) ;
	logging::log_debug("... done exporting as Excel workbook") ;

	if ( m_needs_quitting ) 
	{
		m_app->Quit() ;
	}
}

void CExcelExporter::write_header(void)
{
	set_header_cell(m_sheet->Cells->Item[1][SOURCE_COL], L"Source");
	set_header_cell(m_sheet->Cells->Item[1][TRANS_COL], L"Trans");
	set_header_cell(m_sheet->Cells->Item[1][CONTEXT_COL], L"Context") ;
	set_header_cell(m_sheet->Cells->Item[1][RELIABILITY_COL], L"Reliability");
	set_header_cell(m_sheet->Cells->Item[1][CREATED_COL], L"Created");
	set_header_cell(m_sheet->Cells->Item[1][MODIFIED_COL], L"Modified");
	set_header_cell(m_sheet->Cells->Item[1][VERIFIED_COL], L"Verified");
}

void CExcelExporter::write_record(record_pointer rec, const int count)
{
	set_cell_text(m_sheet->Cells->Item[count][SOURCE_COL], rec->get_source_plain());
	set_cell_text(m_sheet->Cells->Item[count][TRANS_COL], rec->get_trans_plain());
	set_cell_text(m_sheet->Cells->Item[count][CONTEXT_COL], rec->get_context_plain()) ;
	set_cell_text(m_sheet->Cells->Item[count][RELIABILITY_COL], int2wstring( rec->get_reliability() ));
	set_cell_text(m_sheet->Cells->Item[count][CREATED_COL], string2wstring( rec->get_created().get_default_date_time_string() ));
	set_cell_text(m_sheet->Cells->Item[count][MODIFIED_COL], string2wstring( rec->get_modified().get_default_date_time_string() ));
	set_cell_text(m_sheet->Cells->Item[count][VERIFIED_COL],  bool2wstring( rec->is_validated() ));
}

void CExcelExporter::set_cell_text( Excel::RangePtr cell, const wstring text )
{
	try
	{
		cell->GetCharacters()->Text = string2BSTR(process_cell_text(text)) ;
	}
	catch (_com_error& e)
	{
		logging::log_error("_com_error setting text in cell - using formula property") ;
		logging::log_exception(e) ;
		cell->FormulaR1C1 = _variant_t(string2BSTR(process_cell_text(text))) ;
	}
}

void CExcelExporter::set_header_cell( Excel::RangePtr pRange, const wstring text )
{
	set_cell_text(pRange, text);
	pRange->Font->Bold = VARIANT_TRUE ;
}