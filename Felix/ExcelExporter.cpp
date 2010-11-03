#include "StdAfx.h"
#include ".\excelexporter.h"

#include "file.h"
#include "logging.h"
#include "ExcelInterfaceReal.h"
#include "input_device_file.h"

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

CExcelExporter::CExcelExporter( CProgressListener* listener, ExcelInterfacePtr excel ) : 
	m_needs_quitting(false),
	m_listener(listener),
	m_ignore_errors(false),
	m_excel(excel)
{
}

CExcelExporter::~CExcelExporter(void)
{
}

void CExcelExporter::export_excel(memory_pointer mem, const CString mem_name, input_device_ptr input)
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
	if ( input->exists(mem_name) )
	{
		input->delete_file(mem_name) ;
	}

	m_excel->SaveAs(mem_name) ;

	m_listener->OnProgressDoneWrite( mem->size() ) ;
	logging::log_debug("... done exporting as Excel workbook") ;
}

void CExcelExporter::write_header(void)
{
	set_header_cell(1, SOURCE_COL, L"Source");
	set_header_cell(1, TRANS_COL, L"Trans");
	set_header_cell(1, CONTEXT_COL, L"Context") ;
	set_header_cell(1, RELIABILITY_COL, L"Reliability");
	set_header_cell(1, CREATED_COL, L"Created");
	set_header_cell(1, MODIFIED_COL, L"Modified");
	set_header_cell(1, VERIFIED_COL, L"Verified");
}

void CExcelExporter::write_record(record_pointer rec, const int count)
{
	set_cell_text(count, SOURCE_COL, rec->get_source_plain());
	set_cell_text(count, TRANS_COL, rec->get_trans_plain());
	set_cell_text(count, CONTEXT_COL, rec->get_context_plain()) ;
	set_cell_text(count, RELIABILITY_COL, int2wstring( rec->get_reliability() ));
	misc_wrappers::date created = rec->get_created() ;
	set_cell_text(count, CREATED_COL, created.get_date_string() + L" " + created.get_time_string());
	misc_wrappers::date modified = rec->get_modified() ;
	set_cell_text(count, MODIFIED_COL, modified.get_date_string() + L" " + modified.get_time_string());
	set_cell_text(count, VERIFIED_COL,  bool2wstring( rec->is_validated() ));
}

void CExcelExporter::set_cell_text(int row, int col, const wstring text )
{
	m_excel->SetCellText(row, col, process_cell_text(text)) ;
}

void CExcelExporter::set_header_cell(int row, int col, const wstring text )
{
	set_cell_text(row, col, text);
	m_excel->MakeCellBold(row, col) ;
}