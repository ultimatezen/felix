#pragma once

#include "PowerPointImports.h"
#include "AddinExceptionHandlerDef.h"
#import "C:\dev\cpp\Assistant Suite\Felix\Unicode_Release\Felix.tlb" \
	rename("TranslateAccelerator", "FelixTranslateAccelerator")

#include "shapeparser.h"
#include "TextRangeParser.h"
#include "EasyUnitTester.h"
#include "app_state.h"


class CPowerPointInterface
{
	PowerPoint::_ApplicationPtr m_app ; 
	long						m_queryStart ;
	CShapeParser				m_shapeParser ;
	CTextRangeParser			m_textRangeParser ;
	app_state					*m_properties ;
	std::vector<wstring>	m_ok_endings ;
public:
	bool m_is_auto ;

	CPowerPointInterface();
	virtual ~CPowerPointInterface();

	void setProperties(app_state *properties)
	{
		m_properties = properties ;
		m_textRangeParser.setProperties(properties) ;
	}
	void setApp( PowerPoint::_ApplicationPtr app )
	{
		m_app = app ;
		m_shapeParser.setApp( app ) ;
	}
	HRESULT select_next_cell();
	HRESULT select_next_shape();
	HRESULT select_next_slide();
	HRESULT correct_notes_page( PowerPoint::TextRangePtr &sel_text_range );
	//bool select_next_sentence();
	HRESULT set_gloss_entry( int index, bool plaintext = false );
	PowerPoint::SelectionPtr getSelection();
	PowerPoint::TextRangePtr getSelectionTextRange();

	HRESULT  OnAutoTransAction ( ) ;
	HRESULT  OnAutoTransFuzzyAction ( ) ;
	HRESULT OnConcordanceAction ( bool plaintext = false ) ;
	HRESULT OnExtendLookupAction ( bool plaintext = false ) ;
	HRESULT OnRegisterGlossAction ( bool plaintext = false )	;
	HRESULT OnDeleteAction ( ) ;
	HRESULT OnNextAction ( ) ;
	HRESULT OnPrevAction ( ) ;
	HRESULT OnEntryAction ( int n, bool plaintext = false ) ;
	HRESULT OnLookupTransAction ( bool plaintext = false ) ;
	HRESULT OnLookupNextTransAction ( bool plaintext = false ) ;
	HRESULT OnTransConcordanceAction ( bool plaintext = false ) ;
	HRESULT OnExtendTransLookupAction ( bool plaintext = false )	;
	HRESULT OnCorrectTransAction ( bool plaintext = false ) ;
	HRESULT OnCorrectAndNextTransAction ( bool plaintext = false ) ;
	HRESULT OnRestoreTransAction ( bool plaintext = false ) ;
	HRESULT OnRestoreAndNextTransAction ( bool plaintext = false ) ;

	void OnUnitTestAction()
	{
#ifdef _DEBUG

		try
		{
			run_unit_tests() ;
		}
		catch( _com_error &e )
		{
			MessageBox( NULL, e.ErrorMessage(), _T("COM ERROR"), MB_OK | MB_SYSTEMMODAL ) ;
		}
		catch( ... )
		{
			ATLASSERT( "OOPS!" && false ) ;
		}

#endif // #ifdef _DEBUG
	}


	PowerPoint::TextRangePtr collapse_selection( PowerPoint::TextRangePtr &selection );
	PowerPoint::TextRangePtr eat_whitespace( PowerPoint::TextRangePtr &range );
	PowerPoint::TextRangePtr setTableCell( PowerPoint::TextRangePtr &sel_text_range );
	HRESULT select_next_sentence_from_textrange( PowerPoint::TextRangePtr &range );
	PowerPoint::TextRangePtr select_next_table_cell( PowerPoint::TextRangePtr &sel_text_range );
	PowerPoint::TextRangePtr select_next_notes_page(PowerPoint::TextRangePtr &sel_text_range) ;
	PowerPoint::TextRangePtr select_next_text_frame( PowerPoint::ShapeRangePtr &shape_range );
	HRESULT setNotesPage(PowerPoint::TextRangePtr sel_text_range) ;
	PowerPoint::TextRangePtr get_notes_textrange( ) ;

	HRESULT OnGetAction( bool plaintext = false ) ;
	HRESULT OnSetAction( bool plaintext = false ) ;
	HRESULT OnGetAndNextAction( bool plaintext = false ) ;
	HRESULT OnSetAndNextAction( bool plaintext = false ) ;
	HRESULT OnLookupAction( bool plaintext = false ) ;
	HRESULT OnLookupNextAction( bool plaintext = false ) ;
	HRESULT OnGlossNAction( ) ;

	bool has_grouped_shapes( PowerPoint::_SlidePtr slide ) ;
	void OnSaveAction()
	{
		getAssistant()->Save() ;
	}

private:
	Felix::IAppPtr getAssistant(void) ;
	void selectAutoShape(PowerPoint::ShapePtr shape) ;
	void selectPlaceHolder(PowerPoint::ShapePtr shape) ;
	void selectTextBox(PowerPoint::ShapePtr shape) ;
	void selectShape(PowerPoint::ShapePtr shape) ;
	PowerPoint::TextRangePtr getTextRange(PowerPoint::ShapePtr shape) ;
	PowerPoint::TextRangePtr getTextRange(PowerPoint::ShapeRangePtr shape) ;
	PowerPoint::TextRangePtr getTextRangeExtent( PowerPoint::TextRangePtr, long selLen );
	void selectNextObject(void);
	bool setSelTextRange(void);
	bool setSelShapeRange(void);
	bool setTransText(PowerPoint::TextRangePtr range);
	bool setCorrectionText(PowerPoint::TextRangePtr range) ;


	void writePlainText(PowerPoint::TextRangePtr& selectionTextRange, std::wstring trans);

	PowerPoint::TextRangePtr getSetSelShapeRange();

	HRESULT lookupNext();

	bool is_ignorable( const wstring& text, bool skip_numbers, int skip_j );
	bool has_j( const wstring &text );
};