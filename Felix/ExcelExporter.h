#pragma once

//#pragma warning(disable:4146) // unary minus operator applied to unsigned type

#import "C:\dev\cpp\Common DLLs\Word 2000 OLBs\MSO.DLL"	\
	rename_namespace("MSOffice") rename("RGB", "OfficeRGB") rename( "SearchPath", "OfficeSearchPath" ) rename("DocumentProperties", "OfficeDocumentProperties") named_guids    

#import "C:\Program Files (x86)\Common Files\microsoft shared\VBA\VBA6\\VBE6EXT.olb" \
	rename_namespace("VBE6") 

#import "C:\dev\cpp\Common DLLs\Word 2000 OLBs\EXCEL9.olb" \
	exclude( "IFont" ) exclude( "IPicture" )\
	rename("DialogBox", "DialogBoxForExcel") rename( "RGB", "RGBForExcel") \
	rename( "CopyFile", "CopyFileForExcel" ) rename( "ReplaceText", "ReplaceTextForExcel" ) \

#include "TranslationMemory.h"

using namespace memory_engine ;

wstring process_cell_text(const wstring text) ;

class CExcelExporter
{
	Excel::_ApplicationPtr	m_app ;
	Excel::_WorkbookPtr		m_book ;
	Excel::_WorksheetPtr	m_sheet ;

	CProgressListener		*m_listener ;
public:
	bool m_needs_quitting;
	bool m_ignore_errors ;

	CExcelExporter( CProgressListener* listener );
	~CExcelExporter(void);
	void export_excel(memory_pointer mem, const CString& mem_name);
	void write_header(void);
	void write_record(record_pointer rec, int count);
};
