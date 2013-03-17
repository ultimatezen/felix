#pragma once


#import "D:\dev\cpp\Common DLLs\Word 2000 OLBs\MSO.DLL"	\
	rename_namespace("MSOffice") rename("RGB", "OfficeRGB") rename( "SearchPath", "OfficeSearchPath" ) rename("DocumentProperties", "OfficeDocumentProperties") named_guids    

#import "D:\Program Files (x86)\Common Files\microsoft shared\VBA\VBA6\\VBE6EXT.olb" \
	rename_namespace("VBE6") 

#import "D:\dev\cpp\Common DLLs\Word 2000 OLBs\EXCEL9.olb" \
	exclude( "IFont" ) exclude( "IPicture" )\
	rename("DialogBox", "DialogBoxForExcel") rename( "RGB", "RGBForExcel") \
	rename( "CopyFile", "CopyFileForExcel" ) rename( "ReplaceText", "ReplaceTextForExcel" ) \

#include "ExcelInterface.h"
#include "logging.h"

class ExcelInterfaceReal : public ExcelInterface
{
	Excel::_ApplicationPtr	m_app ;
	Excel::_WorkbookPtr		m_book ;
	Excel::_WorksheetPtr	m_sheet ;

	bool m_needs_quitting;

public:
	ExcelInterfaceReal() :
	  m_needs_quitting(false)
	  {
		  m_app.CreateInstance( L"Excel.Application" ) ;

		  Excel::WorkbooksPtr wBooks = m_app->Workbooks ;

		  if ( wBooks->GetCount() == 0 ) 
		  {
			  m_needs_quitting = true ;
		  }

		  m_book = wBooks->Add( ) ;
		  m_sheet = m_book->GetActiveSheet() ;
	  }
	  ~ExcelInterfaceReal()
	  {
		  if (m_needs_quitting)
		  {
			  m_app->Quit() ;
		  }
	  }

	  void SaveAs(const CString location)
	  {
		  const _bstr_t save_loc(location) ;
		  _variant_t file_format(Excel::xlWorkbookNormal) ;
		  m_book->SaveAs( save_loc, 
			  file_format,
			  vtMissing,
			  vtMissing,
			  vtMissing,
			  vtMissing,
			  Excel::xlNoChange) ;
	  }

	  void SetCellText(int row, int col, const wstring text)
	  {
		  Excel::RangePtr cell = m_sheet->Cells->Item[row][col] ;

		  try
		  {
			  cell->GetCharacters()->Text = string2BSTR(text) ;
		  }
		  catch (_com_error& e)
		  {
			  logging::log_error("_com_error setting text in cell - using formula property") ;
			  logging::log_exception(e) ;
			  cell->FormulaR1C1 = _variant_t(string2BSTR(text)) ;
		  }

	  }
	  void MakeCellBold(int row, int col)
	  {
		  Excel::RangePtr cell = m_sheet->Cells->Item[row][col] ;
		  cell->Font->Bold = VARIANT_TRUE ;
	  }

};