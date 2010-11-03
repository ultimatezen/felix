#pragma once

#include "TranslationMemory.h"
#include "ExcelInterface.h"

wstring process_cell_text(const wstring text) ;

class CExcelExporter
{
	CProgressListener		*m_listener ;
	ExcelInterfacePtr		m_excel ;
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

	CExcelExporter(CProgressListener* listener, ExcelInterfacePtr excel);
	~CExcelExporter(void);
	void export_excel(mem_engine::memory_pointer mem, const CString mem_name, input_device_ptr input);
	void write_header(void);

	void set_header_cell(int row, int col, wstring text );
	void set_cell_text(int row, int col, wstring text );
	void write_record(mem_engine::record_pointer rec, int count);
};
