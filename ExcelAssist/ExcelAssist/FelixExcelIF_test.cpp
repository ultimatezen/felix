#include "StdAfx.h"
#include "FelixExcelIF.h"

#include "fake_excel.h"
#include "fake_ta.h"


#ifdef UNIT_TEST
#include <boost/test/unit_test.hpp>

struct IFHolder
{
	excel::sheet_ptr sheet ;
	ta_ptr app ;

	CFelixExcelIF excelif ;

	std::vector< string > m_calls ;

	IFHolder()
	{
		sheet = excel::sheet_ptr( new excel::FakeWorksheet ) ;
		app = ta_ptr( new FakeFelix ) ;

		excelif.m_excel_app = excel::app_ptr(new excel::FakeApplication ) ;

		excelif.getAssistant =  boost::bind(&IFHolder::fakeGetAssistant, this ) ;
		excelif.restore_cell_color =  boost::bind(&IFHolder::fakeRestoreColor, this ) ;
	}
	ta_ptr fakeGetAssistant()
	{
		return app ;
	}
	void fakeRestoreColor()
	{
		m_calls.push_back("fakeRestoreColor") ;
	}
	excel::FakeApplication* get_excel()
	{
		return dynamic_cast< excel::FakeApplication* >(this->excelif.m_excel_app.get()) ;
	}
	FakeFelix* get_app()
	{
		return dynamic_cast< FakeFelix* >(this->app.get()) ;
	}
};

namespace easyunit
{
	// OnLookup
	TEST( CFelixExcelIFTest, OnLookupChars)
	{
		IFHolder h ;
		excel::FakeApplication *app = h.get_excel() ;
		ASSERT_TRUE(app) ;

		excel::FakeCharacters *fchars = new excel::FakeCharacters ;
		fchars->text = L"eggs" ;
		app->chars = excel::chars_ptr( fchars ) ;

		FakeFelix *fta = h.get_app() ;
		ASSERT_TRUE(fta) ;

		h.excelif.OnLookup() ;
		ASSERT_EQUALS( fta->m_query, _bstr_t(L"eggs") ) ;

	}
	TEST( CFelixExcelIFTest, OnLookup)
	{
		IFHolder h ;

		excel::FakeRange *frange = new excel::FakeRange;
		frange->text = L"spam" ;

		excel::FakeApplication *app = h.get_excel() ;
		ASSERT_TRUE(app) ;
		app->active_cell = excel::range_ptr(frange) ;

		h.excelif.OnLookup() ;
		FakeFelix *fta = dynamic_cast< FakeFelix* >(h.app.get()) ;
		ASSERT_TRUE(fta) ;
		ASSERT_EQUALS( fta->m_query, _bstr_t(L"spam") ) ;

	}

	// OnSet

	TEST( CFelixExcelIFTest, OnSetChars)
	{
		IFHolder h ;
		excel::FakeApplication *app = h.get_excel() ;
		ASSERT_TRUE(app) ;

		excel::FakeCharacters *fchars = new excel::FakeCharacters ;
		fchars->text = L"eggs" ;

		app->chars = excel::chars_ptr( fchars ) ;
		FakeFelix *fta = dynamic_cast< FakeFelix* >(h.app.get()) ;
		ASSERT_TRUE(fta) ;

		h.excelif.OnSet() ;
		ASSERT_EQUALS( fta->m_trans, _bstr_t(L"eggs") ) ;

	}
	TEST( CFelixExcelIFTest, OnSet)
	{
		IFHolder h ;

		excel::FakeRange *frange = new excel::FakeRange;
		frange->text = L"spam" ;

		excel::FakeApplication *app = h.get_excel() ;
		ASSERT_TRUE(app) ;
		app->active_cell = excel::range_ptr(frange) ;

		h.excelif.OnSet() ;
		FakeFelix *fta = dynamic_cast< FakeFelix* >(h.app.get()) ;
		ASSERT_TRUE(fta) ;
		ASSERT_EQUALS( fta->m_trans, _bstr_t(L"spam") ) ;

	}

	// get_selection_text
	TEST( CFelixExcelIFTest, get_selection_text)
	{
		excel::FakeApplication *fapp = new excel::FakeApplication ;

		excel::FakeRange *frange = new excel::FakeRange;
		frange->text = L"spam" ;

		fapp->active_cell = excel::range_ptr(frange) ;

		excel::app_ptr app(fapp) ;

		CFelixExcelIF excelif ;
		_bstr_t text = excelif.get_selection_text( app ) ;
		ASSERT_EQUALS( text, _bstr_t(L"spam") ) ;
	}
	TEST( CFelixExcelIFTest, get_selection_textChars)
	{
		excel::FakeApplication *fapp = new excel::FakeApplication ;

		// we should get the text from the characters,
		// even though we are setting an active cell.
		excel::FakeRange *frange = new excel::FakeRange;
		frange->text = L"spam" ;
		fapp->active_cell = excel::range_ptr(frange) ;

		excel::FakeCharacters *fchars = new excel::FakeCharacters ;
		fchars->text  = L"eggs" ;
		fapp->chars = excel::chars_ptr(fchars) ;

		excel::app_ptr app(fapp) ;

		CFelixExcelIF excelif ;
		_bstr_t text = excelif.get_selection_text( app ) ;
		ASSERT_EQUALS( text, _bstr_t(L"eggs") ) ;
	}


	// getActiveSheet
	TEST( CFelixExcelIFTest, getActiveSheet)
	{
		IFHolder h ;

		excel::FakeApplication *as_fake = static_cast< excel::FakeApplication* >(h.excelif.m_excel_app.get());
		as_fake->active_sheet = h.sheet ;

		excel::sheet_ptr sheet2 = h.excelif.GetActiveSheet() ;
		ASSERT_EQUALS(h.sheet,  sheet2) ;
		h.sheet->SetName(L"foo") ;
		ASSERT_EQUALS( sheet2->GetName(), _bstr_t(L"foo")) ;
	}
	TEST( CFelixExcelIFTest, getAssistant)
	{
		IFHolder h ;

		ta_ptr app2 = h.excelif.getAssistant() ;
		ASSERT_EQUALS(h.app, app2) ;
		h.app->SetTrans(L"foo") ;
		ASSERT_EQUALS( app2->GetTrans(), _bstr_t(L"foo")) ;
	}

	TEST( CFelixExcelIFTest, Instantiate )
	{
		try
		{
			CFelixExcelIF excel_if ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate CFelixExcelIF in test harness" ) ;
		}
	}
	TEST( CFelixExcelIFTest, IsIgnoreCell)
	{
	
		CFelixExcelIF excel_if ;
		app_state props ;
		excel_if.setProperties(&props) ;

		wstring text = L"foo" ;
		ASSERT_TRUE( ! excel_if.IsIgnoreCell(text) ) ;

		text = L"34" ;
		ASSERT_TRUE(  excel_if.IsIgnoreCell(text) ) ;

		text = L"" ;
		ASSERT_TRUE(  excel_if.IsIgnoreCell(text) ) ;

		text = L"=SUM(C34, B14)" ;
		ASSERT_TRUE(  excel_if.IsIgnoreCell(text) ) ;

	}
	TEST( CFelixExcelIFTest, remember_active_sheet)
	{
		excel::sheet_ptr sheet( new excel::FakeWorksheet ) ;
		sheet->SetName( L"foo" ) ;

		CFelixExcelIF excel_if ;

		excel_if.remember_active_sheet(sheet) ;
		ASSERT_EQUALS( excel_if.m_CurrentSheetName, L"foo" ) ;
	}
	TEST( CFelixExcelIFTest, get_cell_loc)
	{
		excel::range_ptr range( new excel::FakeRange ) ;
		excel::FakeRange *as_fake = static_cast< excel::FakeRange* >(range.get());
		as_fake->row = 1 ;
		as_fake->col = 2 ;
		wstring name = L"spam" ;

		CFelixExcelIF excel_if ;

		CFelixExcelIF::cell_loc location = excel_if.get_cell_loc(name, range) ;
		ASSERT_EQUALS( location.first, name ) ;
		ASSERT_EQUALS( location.second.first, 1) ;
		ASSERT_EQUALS( location.second.second, 2 ) ;
	}
	TEST( CFelixExcelIFTest, autoTransSheetName_no)
	{
		FakeFelix *fta = new FakeFelix ;
		fta->m_score = 0.1 ;
		fta->m_trans = L"eggs" ;
		ta_ptr ta(fta) ;

		excel::FakeWorksheet *fsheet = new excel::FakeWorksheet ;
		fsheet->m_name = L"spam" ;
		excel::sheet_ptr sheet(fsheet) ;

		CFelixExcelIF excelif ;
		app_state props ;
		excelif.setProperties(&props) ;
		
		excelif.autoTransSheetName(sheet, ta) ;

		ASSERT_EQUALS(sheet->GetName(), _bstr_t(L"spam") ) ;
	}
	TEST( CFelixExcelIFTest, autoTransSheetName_yes)
	{
		FakeFelix *fta = new FakeFelix ;
		fta->m_score = 1.0 ;
		fta->m_trans = L"eggs" ;
		ta_ptr ta(fta) ;

		excel::FakeWorksheet *fsheet = new excel::FakeWorksheet ;
		fsheet->m_name = L"spam" ;
		excel::sheet_ptr sheet(fsheet) ;

		CFelixExcelIF excelif ;
		app_state props ;
		excelif.setProperties(&props) ;

		excelif.autoTransSheetName(sheet, ta) ;

		ASSERT_EQUALS(sheet->GetName(), _bstr_t(L"eggs") ) ;
	}

	TEST( CFelixExcelIFTest, setApp)
	{
		excel::app_ptr app( new excel::FakeApplication) ;

		CFelixExcelIF excel_if ;

		excel_if.setApp(app) ;
		ASSERT_EQUALS(excel_if.m_excel_app, app) ;
	}

	TEST( CFelixExcelIFTest, set_review_mode)
	{
		CFelixExcelIF excel_if ;
		ASSERT_TRUE(excel_if.m_is_trans_mode) ;
		ASSERT_TRUE(excel_if.is_trans_mode()) ;

		excel_if.set_review_mode() ;
		ASSERT_TRUE(! excel_if.m_is_trans_mode) ;
		ASSERT_TRUE(! excel_if.is_trans_mode()) ;
	}
	TEST( CFelixExcelIFTest, set_trans_mode)
	{
		CFelixExcelIF excel_if ;

		excel_if.set_review_mode() ;
		excel_if.set_trans_mode() ;
		ASSERT_TRUE(excel_if.m_is_trans_mode) ;
		ASSERT_TRUE(excel_if.is_trans_mode()) ;
	}

}

#endif