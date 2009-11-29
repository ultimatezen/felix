#include "StdAfx.h"
#include ".\excelexporter.h"

#include "file.h"
#include "logging.h"

wstring process_cell_text(const wstring text)
{
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

	for ( pos = mem->begin() ; pos != mem->end() ; ++pos )
	{
		record_pointer rec = *pos ;
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

	_bstr_t save_loc( mem_name ) ;

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
	Excel::RangePtr pRange = m_sheet->Cells->Item[1][1] ;

	pRange->FormulaR1C1 = L"Source" ;
	pRange->Font->Bold = VARIANT_TRUE ;

	pRange = m_sheet->Cells->Item[1][2] ;

	pRange->FormulaR1C1 = L"Trans" ;
	pRange->Font->Bold = VARIANT_TRUE ;

	pRange = m_sheet->Cells->Item[1][3] ;
	
	pRange->FormulaR1C1 = L"Context" ;
	pRange->Font->Bold = VARIANT_TRUE ;

	pRange = m_sheet->Cells->Item[1][4] ;
	
	pRange->FormulaR1C1 = L"Reliability" ;
	pRange->Font->Bold = VARIANT_TRUE ;

	pRange = m_sheet->Cells->Item[1][5] ;
	
	pRange->FormulaR1C1 = L"Created" ;
	pRange->Font->Bold = VARIANT_TRUE ;

	pRange = m_sheet->Cells->Item[1][6] ;
	
	pRange->FormulaR1C1 = L"Modified" ;
	pRange->Font->Bold = VARIANT_TRUE ;

	pRange = m_sheet->Cells->Item[1][7] ;
	
	pRange->FormulaR1C1 = L"Verified" ;
	pRange->Font->Bold = VARIANT_TRUE ;
}

void CExcelExporter::write_record(record_pointer rec, int count)
{
	Excel::RangePtr pRange = m_sheet->Cells->Item[count][1] ;
	pRange->FormulaR1C1 = process_cell_text(rec->get_source_plain()).c_str() ; 

	pRange = m_sheet->Cells->Item[count][2] ;
	pRange->FormulaR1C1 = process_cell_text(rec->get_trans_plain()).c_str() ;

	pRange = m_sheet->Cells->Item[count][3] ;
	pRange->FormulaR1C1 = process_cell_text(rec->get_context_plain()).c_str() ;

	pRange = m_sheet->Cells->Item[count][4] ;
	pRange->FormulaR1C1 = int2wstring( rec->get_reliability() ).c_str() ;

	pRange = m_sheet->Cells->Item[count][5] ;
	pRange->FormulaR1C1 = string2wstring( rec->get_created().get_default_date_time_string() ).c_str() ;

	pRange = m_sheet->Cells->Item[count][6] ;
	pRange->FormulaR1C1 = string2wstring( rec->get_modified().get_default_date_time_string() ).c_str() ;

	pRange = m_sheet->Cells->Item[count][7] ;
	pRange->FormulaR1C1 = bool2wstring( rec->is_validated() ).c_str() ;
}
