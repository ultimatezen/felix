#pragma once

#import "C:\dev\cpp\Common DLLs\Word 2000 OLBs\MSO.DLL"	\
	rename_namespace("MSOffice") rename("RGB", "OfficeRGB") rename( "SearchPath", "OfficeSearchPath" ) rename("DocumentProperties", "OfficeDocumentProperties") named_guids    

#import "C:\Program Files (x86)\Common Files\microsoft shared\VBA\VBA6\\VBE6EXT.olb" \
	rename_namespace("VBE6") 

#import "C:\dev\cpp\Common DLLs\Word 2000 OLBs\EXCEL9.olb" \
	exclude( "IFont" ) exclude( "IPicture" )\
	rename("DialogBox", "DialogBoxForExcel") rename( "RGB", "RGBForExcel") \
	rename( "CopyFile", "CopyFileForExcel" ) rename( "ReplaceText", "ReplaceTextForExcel" ) \

#include "TranslationMemory.h"


wstring process_cell_text(const wstring text) ;

class CExcelExporter
{
	Excel::_ApplicationPtr	m_app ;
	Excel::_WorkbookPtr		m_book ;
	Excel::_WorksheetPtr	m_sheet ;

	CProgressListener		*m_listener ;
public:

	enum
	{
		SOURCE_COL = 1,
		TRANS_COL,
		CONTEXT_COL,
		RELIABILITY_COL,
		CREATED_COL,
		MODIFIED_COL,
		VERIFIED_COL
	};

	bool m_needs_quitting;
	bool m_ignore_errors ;

	CExcelExporter(CProgressListener* listener);
	~CExcelExporter(void);
	void export_excel(mem_engine::memory_pointer mem, const CString& mem_name);
	void write_header(void);

	void set_header_cell( Excel::RangePtr pRange, wstring text );
	void set_cell_text( Excel::RangePtr pRange, wstring text );
	void write_record(mem_engine::record_pointer rec, int count);
};
