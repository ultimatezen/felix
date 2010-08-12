/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "ExcelImport.h"


#include "ta_application.h"
#include "debugutilities.h"
#include "excel_application.h"
#include "excel_range.h"
#include "Properties.h"
#include "logging.h"

#pragma warning( disable : 4180 ) 

/*!
 * \brief
 * Interface to the Excel application
 * 
 * Wraps the various actions we want to perform in Excel, like looking up and retrieving cells.
 */

_bstr_t message_cell_text(_bstr_t text) ;
_bstr_t get_cell_text(excel::range_ptr cell); // prototype
void set_cell_text(excel::range_ptr cell, _bstr_t text); // prototype

class CFelixExcelIF
{
#ifdef _DEBUG
public:
#endif
	app_state *m_properties ;

	typedef std::pair< int, int > cellCoords ;
	typedef std::pair< wstring, cellCoords > cell_loc ;

	// The Excel application
	excel::app_ptr m_excel_app ;

	// Should non-matching cells be colored?
	bool m_shouldColor ;

	// Number of cells translated on the sheet
	int numTransSheet ;
	// Number of cells not translated on the sheet
	int numNotTransSheet ;
	// Number of cells translated in the book
	int numTransBook ;
	// Number of cells not translated in the book
	int numNotTransBook ;

	// Felix COM server
	Felix::IAppPtr m_Assistant ;

	// The name of the current sheet
	wstring m_CurrentSheetName ;

	bool m_is_trans_mode ;

	CDispatchWrapper *m_trans_history ;
	/*!
	 * \brief
	 * A map of the cells we have colored.
	 * 
	 * We use this to return them to their original color after
	 * we translate them.
	 */
	std::map< cell_loc, int > m_ColoredCells ;

	boost::function< ta_ptr() > getAssistant ;
	boost::function<void()> restore_cell_color ;

public:
	CFelixExcelIF(LPCWSTR trans_hist_servername = L"FelixTransHistory.Excel");
	~CFelixExcelIF();
	void set_app_ptr(LPDISPATCH app);
	void shut_down();
	void close_workbook(IDispatch* workbook);	
	
	void setApp( excel::app_ptr excelApp );
	void setProperties(app_state *properties);
	void set_trans_mode();
	void set_review_mode();
	bool is_trans_mode();

	void OnAutoAddGloss( ) ;

	void select_final_cell( excel::sheet_ptr activeSheet, long current_row, const long start_col );
	void OnAutoAddMem( )  ;

	excel::sheet_ptr GetActiveSheet();
	void OnLookup ( )  ;

	void remember_active_sheet(excel::sheet_ptr activeSheet);
	_bstr_t get_selection_text( excel::app_ptr application );
	void set_active_cell_text(_bstr_t text );
	void OnLookupNext( ) ;
	void OnSet( )  ;

	void real_restore_cell_color();
	void OnSetAndNext( )  ;
	void OnGet( )  ;
	void OnGetAndNext( ) ;
	void OnAutoTransSel( ) ;

	void OnNext();
	void OnPrev();
	void OnEntry( int num );

	void OnAutoTransSheet();

	void PerformSheetAutoTrans();

	void autoTransSheetName( excel::sheet_ptr activeSheet, ta_ptr assistant );
	void AutoTransCell( excel::range_ptr activeCell, ta_ptr assistant );

	bool IsIgnoreCell( const wstring &text ) ;
	void restoreColor( excel::range_ptr activeCell );

	cell_loc get_cell_loc( const wstring &name, excel::range_ptr activeCell ) ;
	void OnAutoTransWorkbook( );

#ifdef _DEBUG
	void OnUnitTest() ;
#endif

	ta_ptr realGetAssistant(void);
	void checkNextSheet();
};
