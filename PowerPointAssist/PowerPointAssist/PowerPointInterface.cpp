#include "resource.h"
#include "stdafx.h"
#include "PowerPointInterface.h"

#include "ui.h"
#include "InputDlg.h"
#include "AddGlossDialog.h"

#include "DispatchWrapper.h"

#include "PPTWriter.h"		// CPPTWriter
#include "PPTFormatter.h"	// CPPTFormatter

#include "KeyboardDriver.h"
#include "tag_stripper.h"
#include "Hooker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

#define _HR_FALSE( x ) \
{ ATLTRACE( "Test: [" #x "]\n" ) ;\
	HRESULT _hr = (x) ; \
	if ( _hr == S_FALSE ) return _hr ;\
	if ( FAILED( _hr ) ) \
{ \
	throw CAtlException( _hr ) ;\
} }
#define _HR( x ) \
{ ATLTRACE( "Test: [" #x "]\n" ) ;\
	HRESULT _hr = (x) ; \
	if ( FAILED( _hr ) ) \
{ \
	throw CAtlException( _hr ) ;\
} }
#define _HRB( x ) \
{ ATLTRACE( "Test: [" #x "]\n" ) ;\
	HRESULT _hr = (x) ; \
	if ( FAILED( _hr ) ) \
{ \
	return false ;\
} }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPowerPointInterface::CPowerPointInterface() : 
	m_queryStart( 0 )
{

}

CPowerPointInterface::~CPowerPointInterface()
{

}

PowerPoint::TextRangePtr CPowerPointInterface::getSelectionTextRange()
{
	return getSelection()->GetTextRange() ;
}

PowerPoint::SelectionPtr CPowerPointInterface::getSelection()
{
	return m_app->GetActiveWindow()->GetSelection() ;
}



// Function name	: CAddin::OnGet
// Description	    : 
// Return type		: HRESULT 
// Argument         :  IDispatch *Ctrl
// Argument         : VARIANT_BOOL * CancelDefault

HRESULT CPowerPointInterface::OnGetAction(bool plaintext) 
{
	BANNER( "CAddin::OnGet" ) ;

	try
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		PowerPoint::TextRangePtr selectionTextRange = getSelectionTextRange() ;
		ATLASSERT( selectionTextRange ) ;

		if ( ! selectionTextRange )
		{
			return E_FAIL ;
		}

		wstring trans = (LPCWSTR)( getAssistant()->Trans ) ;

		if ( plaintext )
		{
			writePlainText(selectionTextRange, trans);
		}
		else
		{
			CPPTWriter writer( selectionTextRange ) ;
			writer.write_html( trans ) ;
		}

		//		PowerPoint::TextRangePtr collapsed = collapse_selection( selectionTextRange ) ;
		//		collapsed->Select() ;
	}
	ADDIN_CATCH( _T("PowerPoint Interface") )
		
	return S_OK ;

}

void CPowerPointInterface::writePlainText(PowerPoint::TextRangePtr& selectionTextRange, std::wstring& trans)
{
	selectionTextRange->Text = strip_tags( trans ).c_str() ;
	int len = selectionTextRange->Length + 1 ;
	PowerPoint::TextRangePtr rng = selectionTextRange->Characters( len, 0 ) ;
	rng->Select() ;

}

// Function name	: CAddin::OnSet
// Description	    : 
// Return type		: HRESULT 
// Argument         :  IDispatch *Ctrl
// Argument         : VARIANT_BOOL * CancelDefault
HRESULT CPowerPointInterface::OnSetAction( bool ) 
{
	BANNER( "CAddin::OnSet" ) ;

	try 
	{

		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		if ( setSelTextRange() ) 
		{
			return S_OK ;
		}

		if ( setSelShapeRange() ) 
		{
			return S_OK ;
		}
		
		PowerPoint::PpViewType viewType = m_app->ActiveWindow->ActivePane->ViewType ;
		if ( viewType == PowerPoint::ppViewNotesPage )
		{
			return setNotesPage( getSelectionTextRange() ) ;
		}

		return E_FAIL ;

	}
	ADDIN_CATCH( _T("PowerPoint Interface") )

	return S_OK ;
}


// Function name	: CPowerPointInterface::OnGetAndNextAction
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnGetAndNextAction( bool plaintext )
{
	BANNER( "CPowerPointInterface::OnGetAndNextAction" ) ;


	try 
	{
		OnGetAction( plaintext ) ;
		OnSetAndNextAction( plaintext ) ;
	}

	ADDIN_CATCH( _T("PowerPoint Interface") )

	return S_OK ;
}


// Function name	: CPowerPointInterface::OnSetAndNextAction
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnSetAndNextAction( bool plaintext )
{
	BANNER( "CPowerPointInterface::OnSetAndNextAction" ) ;


	try 
	{
		OnSetAction( plaintext ) ;
		OnLookupNextAction( plaintext ) ;
	}

	ADDIN_CATCH( _T("PowerPoint Interface") )

	return S_OK ;
}

// Function name	: CPowerPointInterface::OnLookupNextAction
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnLookupNextAction( bool plaintext )
{
	BANNER( "CPowerPointInterface::OnLookupNextAction" ) ;

	try 
	{
		if ( FAILED( lookupNext() ) )
		{
			return E_FAIL ;
		}  

		OnLookupAction( plaintext ) ;	

	}
#ifdef _DEBUG
	ADDIN_CATCH( _T("PowerPoint Interface") )
#else
	catch ( ... ) 
	{
		MessageBeep(MB_ICONASTERISK) ;
		return S_OK ;
	}
#endif

	return S_OK ;
}



// Function name	: CPowerPointInterface::OnLookupAction
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnLookupAction( bool plaintext )
{
	BANNER( "CPowerPointInterface::OnLookupAction" ) ;

	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}


		PowerPoint::TextRangePtr selectionTextRange = getSelectionTextRange() ;
		m_queryStart = selectionTextRange->Start ;

		_bstr_t formatted_text = m_textRangeParser.getSelectedText( plaintext ) ;
		// don't send empty queries!
		if ( formatted_text.length() > 0 ) 
		{
			Felix::IAppPtr app = getAssistant() ;
			app->Query = formatted_text ;
		}

	}

	ADDIN_CATCH( _T("Looking up query from PowerPoint") )

		return S_OK ;
}


// Function name	: CPowerPointInterface::OnGlossNAction
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnGlossNAction(  )
{
	BANNER( "CPowerPointInterface::OnGlossNAction" ) ;

	try 
	{

		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		CInputDlg dlg ;

		INT_PTR ret_val = dlg.DoModal( ) ;

		ATLASSERT ( ! dlg.IsWindow() )  ;

		if ( ret_val == IDCANCEL )
			return S_FALSE ;

		int num = dlg.get_num() ;

		set_gloss_entry( num ) ;
	}

	ADDIN_CATCH( _T("PowerPoint Interface :: Retrieving a Glossary Entry") )

	return S_OK ;
}


// Function name	:  CPowerPointInterface::OnAutoTransAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT  CPowerPointInterface::OnAutoTransAction ( )
{
	BANNER( " CPowerPointInterface::OnAutoTransAction " ) ;

	try 
	{

		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		MessageBox( NULL, _T("OnAutoTrans"), _T("To be implemented!"), MB_OK ) ;

	}

	ADDIN_CATCH( _T("PowerPoint Interface") )
		return S_OK ;
}

// Function name	:  CPowerPointInterface::OnAutoTransFuzzyAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT  CPowerPointInterface::OnAutoTransFuzzyAction ( )
{
	BANNER( " CPowerPointInterface::OnAutoTransFuzzyAction " ) ;

	bool plaintext = shift_key_is_pressed() ;
	try 
	{

		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		while ( true ) 
		{
			if ( ! SUCCEEDED(OnLookupNextAction(plaintext)) )
			{
				return S_OK ;
			}

			_bstr_t formatted_text = m_textRangeParser.getSelectedText( plaintext ) ;
			// don't send empty queries!
			if ( formatted_text.length() == 0 ) 
			{
				return S_OK ;
			}

			Felix::IAppPtr app = getAssistant( ) ;
			double score = app->Score ;

			if ( FLOAT_EQ( score, 1.0 ) )
			{
				OnGetAction(plaintext) ;
			}
			else
			{
				return S_OK ;
			}

		} 

	} 
	ADDIN_CATCH( _T("PowerPoint Interface") ) ;
		
	return S_OK ;
}

// Function name	: CPowerPointInterface::OnConcordanceAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnConcordanceAction ( bool )
{
	BANNER( "CPowerPointInterface::OnConcordanceAction " ) ;

	try 
	{

		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		PowerPoint::TextRangePtr textRange = getSelectionTextRange() ;

		if ( textRange->Text.length() > 0 )
		{
			Felix::IAppPtr app = getAssistant() ;
			app->Concordance( textRange->Text ) ;
		}

	} 
	ADDIN_CATCH( _T("PowerPoint Interface") )
		return S_OK ;
}

// Function name	: CPowerPointInterface::OnExtendLookupAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnExtendLookupAction ( bool plaintext )
{
	BANNER( "CPowerPointInterface::OnExtendLookupAction " ) ;

	try 
	{
	
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		// If there is no text selected, we should not call extend selection.
		_bstr_t text = m_textRangeParser.getSelectedText(plaintext) ;
		if ( text.length() == 0 )
		{
			return E_FAIL ;
		}

		if ( ! m_textRangeParser.extendSelection() )
		{
			MessageBeep( MB_ICONASTERISK ) ;
			return E_FAIL ;
		}

		text = m_textRangeParser.getSelectedText(plaintext) ;

		if ( text.length() == 0 )
		{
			return E_FAIL ;
		}

		getAssistant()->Query = text ;
	} 
	ADDIN_CATCH( _T("PowerPoint Interface") )
	
	return S_OK ;
}

// Function name	: CPowerPointInterface::OnRegisterGlossAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnRegisterGlossAction ( bool /*as_plaintext*/)
{
	BANNER( "CPowerPointInterface::OnRegisterGlossAction " ) ;
	try 
	{

		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		_bstr_t source = m_textRangeParser.getSelectedText(true) ;
		_bstr_t trans ;

		if ( m_properties->get_preferred_gui_lang() == PREF_LANG_ENGLISH )
		{
			CAddGlossDlgE dlg ;
			dlg.m_source = (LPCWSTR)source ;
			if ( IDCANCEL == dlg.DoModal( ) ) 
			{
				return false ;
			}
			trans = (LPCWSTR)dlg.m_trans ;
			source = (LPCWSTR)dlg.m_source ;
		}
		else
		{
			CAddGlossDlgJ dlg ;
			dlg.m_source = (LPCWSTR)source ;
			if ( IDCANCEL == dlg.DoModal( ) ) 
			{
				return false ;
			}
			trans = (LPCWSTR)dlg.m_trans ;
			source = (LPCWSTR)dlg.m_source ;
		}


		Felix::IAppPtr app = getAssistant() ;
		_bstr_t context = L"" ;
		app->AddGlossaryEntry( source, trans, context ) ;

		return true ;
	}

	ADDIN_CATCH( _T("PowerPoint Interface :: Adding a Glossary Entry") )

	return S_OK ;
}

// Function name	: CPowerPointInterface::OnDeleteAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnDeleteAction ( )
{
	BANNER( "CPowerPointInterface::OnDeleteAction " ) ;

	try {

		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		Felix::IAppPtr app = getAssistant() ;
		app->DeleteMemEntry() ;

	} ADDIN_CATCH( _T("PowerPoint Interface") )
		return S_OK ;
}

// Function name	: CPowerPointInterface::OnNextAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnNextAction ( )
{
	BANNER( "CPowerPointInterface::OnNextAction " ) ;

	try 
	{

		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		Felix::IAppPtr app = getAssistant() ;
		app->NextTrans() ;

	} 
	ADDIN_CATCH( _T("PowerPoint Interface") )
	
	return S_OK ;
}

// Function name	: CPowerPointInterface::OnPrevAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnPrevAction ( )
{
	BANNER( "CPowerPointInterface::OnPrevAction " ) ;

	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		Felix::IAppPtr app = getAssistant() ;
		app->PrevTrans() ;

	} 
	ADDIN_CATCH( _T("PowerPoint Interface") )
		return S_OK ;
}

// Function name	: CPowerPointInterface::OnEntry0Action 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnEntryAction ( int n, bool plaintext )
{
	BANNER( "CPowerPointInterface::OnEntry0Action " ) ;

	try 
	{

		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		set_gloss_entry( n, plaintext ) ;

	} ADDIN_CATCH( _T("PowerPoint Interface") )
		return S_OK ;
}


// Function name	: CPowerPointInterface::OnLookupTransAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnLookupTransAction (bool plaintext)
{
	BANNER( "CPowerPointInterface::OnLookupTransAction " ) ;
	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}


		PowerPoint::TextRangePtr selectionTextRange = getSelectionTextRange() ;
		ATLASSERT( selectionTextRange ) ;
		if ( ! selectionTextRange ) return E_FAIL ;

		m_queryStart = selectionTextRange->Start ;

		_bstr_t formatted_text = m_textRangeParser.getSelectedText( plaintext ) ;
		// don't send empty queries!
		if ( formatted_text.length() > 0 ) 
		{
			Felix::IAppPtr app = getAssistant() ;
			app->LookupTrans( formatted_text ) ;
		}

	}
	ADDIN_CATCH( _T("PowerPoint Interface") )
	
	return S_OK ;
}


// Function name	: CPowerPointInterface::OnLookupNextTransAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnLookupNextTransAction ( bool plaintext )
{
	BANNER( "CPowerPointInterface::OnLookupNextTransAction " ) ;

	try 
	{
		if ( FAILED( lookupNext() ) )
		{
			return E_FAIL ;
		}  
		OnLookupTransAction( plaintext ) ;
	} 
#ifdef _DEBUG
	ADDIN_CATCH( _T("PowerPoint Interface") )
#else
	catch ( ... ) 
	{
		MessageBeep(MB_ICONASTERISK) ;
		return S_OK ;
	}
#endif
	
	return S_OK ;
}

HRESULT CPowerPointInterface::lookupNext()
{
	if (m_app->Active != Office::msoTrue )
	{
		return E_FAIL ;
	}

	if ( m_textRangeParser.selectNext() )
	{
		wstring text = BSTR2wstring(m_textRangeParser.getSelectedText(true)) ;
		if (this->is_ignorable(text, !! m_properties->m_data.m_skipNumbers, m_properties->m_data.m_skipJ))
		{
			return lookupNext() ;
		}
		return S_OK ;
	}

	PowerPoint::TextRangePtr next_range = m_shapeParser.nextTextRange() ;
	if (! next_range)
	{
		m_shapeParser.nextShape() ;
		next_range = m_shapeParser.nextTextRange() ;
	}
	m_textRangeParser.setRange( next_range ) ;
	if ( ! m_textRangeParser.selectNext() )
	{
		return E_FAIL ;
	}

	wstring text = BSTR2wstring(m_textRangeParser.getSelectedText(true)) ;
	if (this->is_ignorable(text, !! m_properties->m_data.m_skipNumbers, m_properties->m_data.m_skipJ))
	{
		return lookupNext() ;
	}
	return S_OK ;
}


// Function name	: CPowerPointInterface::OnTransConcordanceAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnTransConcordanceAction ( bool)
{
	BANNER( "CPowerPointInterface::OnTransConcordanceAction " ) ;

	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		PowerPoint::TextRangePtr textRange = getSelectionTextRange() ;

		CComBSTR text ;
		_HR( textRange->get_Text( &text ) ) ;

		if ( text.Length() > 0 )
		{
			Felix::IAppPtr app = getAssistant() ;
			app->TransConcordance( textRange->Text ) ;
		}
	} 
	ADDIN_CATCH( _T("PowerPoint Interface") )
		return S_OK ;
}


// Function name	: CPowerPointInterface::OnExtendTransLookupAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnExtendTransLookupAction ( bool plaintext )
{
	BANNER( "CPowerPointInterface::OnExtendTransLookupAction " ) ;

	try 
	{
		if ( ! m_textRangeParser.extendSelection() )
		{
			MessageBeep( MB_ICONASTERISK ) ;
			return S_OK ;
		}

		_bstr_t text = m_textRangeParser.getSelectedText(plaintext) ;

		// don't send empty queries!
		if ( text.length() == 0 )
		{
			return E_FAIL ;
		}

		Felix::IAppPtr app = getAssistant() ;
		app->LookupTrans( text ) ;

	} 
	ADDIN_CATCH( _T("PowerPoint Interface") )
		return S_OK ;
}


// Function name	: CPowerPointInterface::OnCorrectTransAction 
// Description	    : 
// Return type		: HRESULT 
HRESULT CPowerPointInterface::OnCorrectTransAction ( bool)
{
	BANNER( "CPowerPointInterface::OnCorrectTransAction " ) ;

	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}
		PowerPoint::TextRangePtr selectionTextRange = m_textRangeParser.getSetRange() ;
		if ( selectionTextRange && selectionTextRange->Length)
		{
			if ( ! setCorrectionText(selectionTextRange) )
			{
				return E_FAIL ;
			} 
			return S_OK ;
		}


		// We see if we already have a selection...
		if ( ! setCorrectionText(getSetSelShapeRange()) )
		{
			return E_FAIL ;
		} 

	} 
	ADDIN_CATCH( _T("PowerPoint Interface") )
		
	return S_OK ;
}

HRESULT CPowerPointInterface::OnCorrectAndNextTransAction ( bool) 
{

	return S_OK ;
}
HRESULT CPowerPointInterface::OnRestoreTransAction ( bool) 
{

	return S_OK ;
}
HRESULT CPowerPointInterface::OnRestoreAndNextTransAction ( bool) 
{
	return S_OK ;
}

// Function name	: CPowerPointInterface::set_gloss_entry
// Description	    : 
// Return type		: HRESULT 
// Argument         : int index
HRESULT CPowerPointInterface::set_gloss_entry(int index, bool plaintext)
{
	BANNER( "CPowerPointInterface::set_gloss_entry" ) ;

	PowerPoint::TextRangePtr selectionTextRange = getSelectionTextRange() ;
	if( ! selectionTextRange ) 
		return E_FAIL ;

	if ( ! selectionTextRange ) return E_FAIL ;

	Felix::IAppPtr app = getAssistant() ;
	_bstr_t entry = app->GlossMatch[ static_cast<short>(index) ] ;

	if ( plaintext )
	{
		wstring plain = str::make_lower_out(strip_tags(BSTR2wstring(entry))) ;
		selectionTextRange->Text = string2BSTR(plain) ;
	}
	else
	{
		CPPTWriter writer( selectionTextRange ) ;

		writer.write_html( (LPCWSTR)entry ) ;
	}

	// ok, move on to the shaperange...
	PowerPoint::SelectionPtr selection = getSelection() ;
	PowerPoint::ShapeRangePtr shapeRange ;
	selection->get_ShapeRange( &shapeRange ) ;

	if( ! shapeRange ) 
	{

		PowerPoint::DocumentWindowPtr activeWindow ;
		m_app->get_ActiveWindow( &activeWindow ) ;
		ATLASSERT( activeWindow )  ;

		PowerPoint::PanePtr activePane ;
		activeWindow->get_ActivePane( &activePane ) ;
		ATLASSERT( activePane )  ;

		PowerPoint::PpViewType viewType ;
		activePane->get_ViewType( &viewType )	;

		CKeyboardDriver driver ;
		if ( viewType == PowerPoint::ppViewNotesPage )
		{
			long len ;
			selectionTextRange->get_Length( &len ) ;

			if ( len > 0 )
			{
				driver.clearAltKey() ;
				driver.clearControlKey() ;

				m_app->Activate() ;

				driver.doRight() ;
				selectionTextRange =   getSelectionTextRange() ;
			}
		}	
		return E_FAIL ;
	}

	PowerPoint::TextRangePtr collapsed = collapse_selection( selectionTextRange ) ;
	collapsed->Select() ;

	return S_OK ;
}


// Function name	: CAddin::eat_whitespace
// Description	    : 
// Return type		: PowerPoint::TextRangePtr 
// Argument         : PowerPoint::TextRangePtr &range
PowerPoint::TextRangePtr CPowerPointInterface::eat_whitespace(PowerPoint::TextRangePtr &range)
{
	BANNER( "CAddin::eat_whitespace" ) ;

	long len ;
	range->get_Length( &len ) ;

	PowerPoint::TextRangePtr character ;

	long i ;
	for ( i=1 ; i<=len ; ++i )
	{
		character = range->Characters( i, 1 ) ;
		CComBSTR c_text ;
		character->get_Text( &c_text ) ;
		TRACE( c_text ) ;

		ATLASSERT( (LPCWSTR)c_text != NULL ) ;

		if ( ! iswspace( ((LPCWSTR)c_text)[0] ) ) 
			break ;

		character.Release() ;
	}

	PowerPoint::TextRangePtr segment_range = range->Characters( i, (len-i)+1 ) ;
	ATLASSERT( segment_range ) ;
	return segment_range ;
}


// Function name	: CAddin::collapse_selection
// Description	    : 
// Return type		: void 
// Argument         : PowerPoint::TextRangePtr &selection
PowerPoint::TextRangePtr CPowerPointInterface::collapse_selection(PowerPoint::TextRangePtr &selection)
{
	BANNER( "CAddin::collapse_selection" ) ;

	long start  = selection->Start ;
	long len = selection->Length ;

	return selection->Characters( start+len, 0 ) ;
}


// Function name	: CAddin::select_next_text_frame
// Description	    : 
// Return type		: void 
// Argument         : PowerPoint::ShapeRangePtr &shapeRange
PowerPoint::TextRangePtr CPowerPointInterface::select_next_text_frame(PowerPoint::ShapeRangePtr &shapeRange)
{
	BANNER( "CAddin::select_next_text_frame" ) ;

	PowerPoint::TextFramePtr text_frame ;
	shapeRange->get_TextFrame( &text_frame ) ;
	ATLASSERT( text_frame ) ;

	// now get the range for this text frame
	PowerPoint::TextRangePtr range ;
	text_frame->get_TextRange( &range ) ;
	ATLASSERT( range ) ;

	if ( ! range ) 
		return PowerPoint::TextRangePtr() ;

	select_next_sentence_from_textrange( range ) ;
	range->Select() ;

	PowerPoint::TextRangePtr text_frame_range ;
	text_frame->get_TextRange( &text_frame_range ) ;
	return text_frame_range ;
}

PowerPoint::TextRangePtr CPowerPointInterface::get_notes_textrange( ) 
{
	PowerPoint::SelectionPtr selection = getSelection() ;

	PowerPoint::SlideRangePtr slides ;
	selection->get_SlideRange( &slides ) ;

	ATLASSERT( slides ) ;

	long cnt ;
	slides->get_Count( &cnt ) ;

	ATLASSERT( cnt == 1 ) ;

	PowerPoint::_SlidePtr slide = slides->Item(1) ;

	ATLASSERT( slide ) ;

	PowerPoint::SlideRangePtr notes_page ;
	slide->get_NotesPage( &notes_page ) ;

	PowerPoint::ShapesPtr shapes ;
	notes_page->get_Shapes( &shapes ) ;
	ATLASSERT( shapes ) ;

	int shapeCount ;
	shapes->get_Count( &shapeCount ) ;

	PowerPoint::TextRangePtr range ;

	for ( int i = 1 ; i <= shapeCount ; ++i )
	{
		PowerPoint::ShapePtr shape = shapes->Item(i) ;

		PowerPoint::PlaceholderFormatPtr  ph_format ;
		shape->get_PlaceholderFormat( &ph_format ) ;

		PowerPoint::PpPlaceholderType type ;

		ph_format->get_Type( &type ) ;

		if ( type == PowerPoint::ppPlaceholderBody )
		{
			PowerPoint::TextFramePtr frame = shape->TextFrame;
			ATLASSERT( frame ) ;

			return frame->TextRange ;
		}

	}

	return range ;
}


HRESULT CPowerPointInterface::setNotesPage(PowerPoint::TextRangePtr selectionTextRange)
{
	PowerPoint::TextRangePtr range = get_notes_textrange() ;

	long current_start ;
	selectionTextRange->get_Start( &current_start ) ;

	selectionTextRange.Release() ;
	int len = current_start - m_queryStart ;
	selectionTextRange = range->Characters( m_queryStart, len ) ;

	CPPTFormatter formatter( selectionTextRange ) ;
	wstring rangeText( formatter.formatRangeText() ) ;
	if ( rangeText.empty() )
	{
		MessageBeep( MB_ICONERROR ) ;
		return S_FALSE ;
	}
	Felix::IAppPtr app = getAssistant() ;
	app->Trans = rangeText.c_str() ;

	m_app->Activate() ;

	CKeyboardDriver driver ;

	driver.clearAltKey();
	driver.clearControlKey();

	for ( int i = 0 ; i < len ; ++i )
	{
		driver.doShiftLeft();
	}

	return S_OK ;
}

// Function name	: CAddin::select_next_table_cell
// Description	    : 
// Return type		: void 
// Argument         : PowerPoint::TextRangePtr &selectionTextRange
PowerPoint::TextRangePtr CPowerPointInterface::select_next_notes_page(PowerPoint::TextRangePtr &selectionTextRange)
{
	BANNER( "CAddin::select_next_notes_page" ) ;

	long len ;
	selectionTextRange->get_Length( &len ) ;

	selectionTextRange->get_Start( &m_queryStart ) ;

	m_queryStart += len ;

	long old_start = m_queryStart ;

	PowerPoint::TextRangePtr range = get_notes_textrange() ;
	select_next_sentence_from_textrange( range ) ;

	long jump_count = m_queryStart - old_start ;

	if ( len > 0 )
	{
		jump_count++ ;
	}

	m_app->Activate() ;

	CKeyboardDriver driver ;

	driver.clearAltKey() ;
	driver.clearControlKey() ;

	for( long i = 0 ; i < jump_count ; ++i )
	{
		driver.doRight() ;
	}


	range->get_Length( &len ) ;

	for ( long i = 0 ; i < len ; ++i )
	{
		driver.doShiftRight() ;
	}

	return range ;
}


// Function name	: CAddin::select_next_table_cell
// Description	    : 
// Return type		: void 
// Argument         : PowerPoint::TextRangePtr &selectionTextRange
PowerPoint::TextRangePtr CPowerPointInterface::select_next_table_cell(PowerPoint::TextRangePtr &selectionTextRange)
{
	BANNER( "CAddin::select_next_table_cell" ) ;

	CDispatchWrapper parent = selectionTextRange->GetParent() ;

	PowerPoint::ShapePtr grandParentShape = parent.prop_get( L"Parent" ).pdispVal ;

	PowerPoint::TextRangePtr textRange = getTextRange( grandParentShape ) ;

	if ( !textRange ) 
		return textRange ;

	select_next_sentence_from_textrange( textRange ) ;

	textRange->Select() ;

	return textRange ;
}


// Function name	: CAddin::select_next_sentence_from_textrange
// Description	    : 
// Return type		: void 
// Argument         : PowerPoint::TextRangePtr &range
HRESULT CPowerPointInterface::select_next_sentence_from_textrange(PowerPoint::TextRangePtr &range)
{
	BANNER( "CAddin::select_next_sentence_from_textrange" ) ;

	long endPoint = (range->Length - m_queryStart ) + 1 ;

	// make up for the 1-based index by adding 1 to the length
	// subtracting 1 from start will select first character twice if start=1
	PowerPoint::TextRangePtr characters_pre = range->Characters( m_queryStart, endPoint ) ;
	ATLASSERT( characters_pre ) ;

	PowerPoint::TextRangePtr characters = eat_whitespace( characters_pre ) ;

	// the start of our query after trimming the whitespace
	characters->get_Start( &m_queryStart ) ;

	PowerPoint::TextRangePtr character ;
	wstring delimiters = resource2wstring( IDS_DELIMITERS, _AtlModule.GetResourceInstance() ) ;
	ATLTRACE( "Delimiters = " ) ;
	ATLTRACE( delimiters.c_str() ) ;
	ATLTRACE( "\n" ) ;

	long len = characters->Length ;
	bool last_char_was_period = false ;

	wchar_t this_char ;

	long i ;
	for ( i=1 ; i<=len ; ++i )
	{
		character = characters->Characters( i, 1 ) ;
		CComBSTR c_text ;
		character->get_Text( &c_text ) ;
		TRACE( c_text ) ;
		character.Release() ;

		this_char = ((LPCWSTR)c_text)[0] ;

		if ( delimiters.find( this_char ) != wstring::npos ) 
			break ;

		if ( this_char == L' ')
		{
			if ( last_char_was_period ) 
			{
				--i ;
				break ;
			}
		}
		else
		{
			// effectively we are testing for whitespace other than spaces.
			if ( this_char >= 0x09 && this_char <= 0x0D )
			{
				--i ; // don't select the whitespace char itself
				break ;
			}
		}

		if ( this_char == L'.' ) 
		{
			last_char_was_period = true ;
		}
		else
			last_char_was_period = false ;

	}

	range = characters->Characters( 1, i ) ;
	ATLASSERT( range ) ;


	return S_OK ;
}


// Function name	: CAddin::setTableCell
// Description	    : 
// Return type		: void 
// Argument         : PowerPoint::TextRangePtr &selectionTextRange
PowerPoint::TextRangePtr CPowerPointInterface::setTableCell(PowerPoint::TextRangePtr &selectionTextRange)
{
	BANNER( "CAddin::setTableCell" ) ;

	long selStart, selLen ;
	selectionTextRange->get_Start( &selStart ) ;
	selectionTextRange->get_Length( &selLen ) ;

	selLen += selStart ;
	--selLen ; // make up for 1-based index

	CTextRangeParser parser ;
	parser.setRangeFromSelection() ;
	PowerPoint::TextRangePtr textRange = parser.m_range ;

	if ( !textRange ) 
		return NULL ;

	// get the translation
	long textExtent = (selLen-m_queryStart)+1 ;
	return textRange->Characters( m_queryStart, textExtent ) ;
}

//DEL PowerPoint::TextRangePtr CPowerPointInterface::select_sentence_for_type(Office::MsoShapeType type)
//DEL {
//DEL 
//DEL }

HRESULT CPowerPointInterface::correct_notes_page(PowerPoint::TextRangePtr &selectionTextRange)
{
	PowerPoint::TextRangePtr range = get_notes_textrange() ;

	long current_start ;
	selectionTextRange->get_Start( &current_start ) ;

	selectionTextRange = range->Characters( m_queryStart, ( current_start - m_queryStart ) ) ;

	CComBSTR text ;

	selectionTextRange->get_Text( &text ) ;

	if ( text.Length() == 0 )
	{
		MessageBeep( MB_ICONERROR ) ;
		return S_FALSE ;
	}

	Felix::IAppPtr app = getAssistant() ;
	app->CorrectTrans( selectionTextRange->Text ) ;

	return S_OK ;
}

HRESULT CPowerPointInterface::select_next_slide()
{
	PowerPoint::DocumentWindowPtr activeWindow ;
	m_app->get_ActiveWindow( &activeWindow ) ;
	ATLASSERT( activeWindow )  ;

	PowerPoint::DocumentWindowsPtr windows ;
	m_app->get_Windows( &windows )	;
	ATLASSERT( windows )  ;

	long cnt ;
	windows->get_Count( &cnt ) ;

	for ( long i=1 ; i<cnt ; ++i )
	{
		PowerPoint::DocumentWindowPtr window = windows->Item( i ) ;

		ATLASSERT( window )  ;

		if ( window == activeWindow )
		{
			window = windows->Item( i+1 ) ;
			ATLASSERT( window )  ;
			window->Activate() ;

			PowerPoint::SelectionPtr selection = getSelection() ;

			if ( ! selection ) return E_FAIL ;

			// ok, move on to the shaperange...
			PowerPoint::ShapeRangePtr shapeRange ;
			selection->get_ShapeRange( &shapeRange ) ;
			ATLASSERT( shapeRange )  ;

			shapeRange->get_Count( &cnt ) ;

			if ( ! cnt )
			{
				return select_next_slide() ;
			}

			PowerPoint::ShapePtr shape = shapeRange->Item( 1 ) ;
			shape->Select(Office::msoTrue) ;

			Office::MsoShapeType type ;
			shape->get_Type( &type ) ;

			switch( type ) 
			{
				CASE( Office::msoAutoShape )
					return S_OK ;
				CASE( Office::msoPlaceholder )
					return S_OK ;
				CASE( Office::msoTextBox )
					return S_OK ;
				CASE( Office::msoTable )
					return S_OK ;
				CASE( Office::msoGroup )
					return S_OK ;
			}


			if ( S_FALSE == select_next_shape() )
			{
				return S_FALSE ;
			}

			return S_OK ;
		}
	}

	return S_FALSE ;
}

HRESULT CPowerPointInterface::select_next_shape()
{
	PowerPoint::SelectionPtr selection = getSelection() ;

	if ( ! selection ) return E_FAIL ;

	// ok, move on to the shaperange...
	PowerPoint::ShapeRangePtr shapeRange = selection->ShapeRange ;

	long cnt = shapeRange->Count ;

	if ( cnt == 0 )
	{
		return S_FALSE ;
	}

	PowerPoint::ShapePtr activeShape  = shapeRange->Item( 1 ) ;
	PowerPoint::DocumentWindowPtr activeWindow = m_app->ActiveWindow ;

	activeWindow->Activate() ;

	PowerPoint::PanePtr activePane = activeWindow->ActivePane ;

	if ( activePane->ViewType != PowerPoint::ppViewSlide )
		return S_FALSE ;

	PowerPoint::ViewPtr activeView = activeWindow->View ;
	PowerPoint::_SlidePtr active_slide = activeView->Slide  ;

	PowerPoint::ShapesPtr shapes = active_slide->Shapes ;

	int shapeCount = shapes->Count ;

	long nextShapeIndex = 1 ;
	for ( nextShapeIndex = 1 ; nextShapeIndex < shapeCount ; ++nextShapeIndex ) 
	{
		PowerPoint::ShapePtr shape = shapes->Item( nextShapeIndex ) ;
		if ( shape == activeShape ) 
		{
			break ;
		}
	}

	for ( nextShapeIndex++ ; nextShapeIndex<= shapeCount ; ++nextShapeIndex )
	{
		PowerPoint::ShapePtr shape = shapes->Item( nextShapeIndex ) ;

		Office::MsoShapeType type = shape->Type ;
		shape->Select(Office::msoTrue) ;
		switch( type ) 
		{
			CASE( Office::msoAutoShape )
			{
				selectAutoShape( shape ) ;
				return S_OK ;
			}
			CASE( Office::msoPlaceholder )
			{
				selectPlaceHolder( shape ) ;
				return S_OK ;
			}
			CASE( Office::msoTextBox )
			{
				selectTextBox( shape ) ;
				return S_OK ;
			}
			CASE( Office::msoTable )
				return E_FAIL ;
			CASE( Office::msoGroup )
				return E_FAIL ;
		default:
			{
				if(shape->HasTextFrame)
				{
					selectTextBox( shape ) ;
					return S_OK ;
				}
			}

		}
	}

	return S_FALSE ;
}

HRESULT CPowerPointInterface::select_next_cell()
{
	PowerPoint::TextRangePtr selectionTextRange = getSelectionTextRange() ;

	// we need to do type Object automation style!
	CDispatchWrapper wrapper = selectionTextRange->GetParent() ;
	PowerPoint::ShapePtr activeShape = wrapper.prop_get( L"Parent" ).pdispVal ;
	ATLASSERT( activeShape ) ;

	PowerPoint::SelectionPtr selection = getSelection() ;

	// ok, move on to the shaperange...
	PowerPoint::ShapeRangePtr shapeRange = selection->GetShapeRange() ;

	PowerPoint::ShapePtr shape = shapeRange->Item( 1 ) ;

	PowerPoint::TablePtr table ;
	shape->get_Table( &table ) ;

	PowerPoint::ColumnsPtr columns ;
	PowerPoint::RowsPtr rows ;

	long num_cols, num_rows ;
	table->get_Columns( &columns ) ;
	table->get_Rows( &rows ) ;
	columns->get_Count( &num_cols ) ;
	rows->get_Count( &num_rows ) ;

	for ( long iRow=1 ; iRow <= num_rows ; ++iRow )
	{
		for ( long iCol=1 ; iCol <= num_cols ; ++iCol)
		{
			PowerPoint::CellPtr cell = table->Cell( iRow, iCol ) ;
			shape = cell->GetShape( ) ;
			if ( shape == activeShape )
			{
				while ( shape == activeShape )
				{
					if ( iCol < num_cols )
					{
						++iCol ;
					}
					else if ( iRow == num_rows ) 
					{
						return S_FALSE ;
					}
					else
					{
						iCol = 1 ;
						++iRow ;
					}
					cell = table->Cell( iRow, iCol ) ;
					shape = cell->Shape ;
				}
				PowerPoint::TextFramePtr text_frame ;
				shape->get_TextFrame( &text_frame ) ;
				PowerPoint::TextRangePtr textRange ;
				text_frame->get_TextRange( &textRange ) ;
				PowerPoint::TextRangePtr start_char_sel = textRange->Characters( 1, 0 ) ;
				start_char_sel->Select() ;
				return S_OK ;
			}
		}
	}
	return S_FALSE ;
}

Felix::IAppPtr CPowerPointInterface::getAssistant(void)
{
	Felix::IAppPtr assistant ;
	assistant.CreateInstance( L"Felix.App" ) ;

	assistant->Visible = VARIANT_TRUE ;

	return assistant ;
}

// Function name	: has_grouped_shapes
// Description	    : 
// Return type		: bool 
// Argument         : PowerPoint::_Slide > slide
bool CPowerPointInterface::has_grouped_shapes( PowerPoint::_SlidePtr slide )
{
	ATLTRACE( "\n\n*** FUNCTION: has_grouped_shapes\n\n" ) ;

	PowerPoint::ShapesPtr shapes ;
	_HRB( slide->get_Shapes( &shapes ) ) ;

	int count ;
	_HRB( shapes->get_Count( &count ) ) ;

	bool result( false ) ;
	for( int i=1 ; i<=count ; ++i )
	{
		PowerPoint::ShapePtr shape = shapes->Item( i ) ;

		if( shape->Type == Office::msoGroup )
		{
			result = true ;
			shape->Ungroup() ;
		}
	}

	return result ;
}


void CPowerPointInterface::selectAutoShape(PowerPoint::ShapePtr shape)
{
	selectShape( shape ) ;
}

void CPowerPointInterface::selectPlaceHolder(PowerPoint::ShapePtr shape)
{
	selectShape( shape ) ;
}

void CPowerPointInterface::selectTextBox(PowerPoint::ShapePtr shape)
{
	selectShape( shape ) ;
}

void CPowerPointInterface::selectShape(PowerPoint::ShapePtr shape)
{
	PowerPoint::TextFramePtr text_frame = shape->TextFrame ;

	// now get the range for this text frame
	PowerPoint::TextRangePtr range = text_frame->TextRange ;

	// get the translation
	PowerPoint::TextRangePtr char_range = range->Characters( 1, 0 ) ;

	char_range->Select() ;
}

PowerPoint::TextRangePtr CPowerPointInterface::getTextRange(PowerPoint::ShapePtr shape)
{
	PowerPoint::TextFramePtr textFrame = shape->GetTextFrame() ;
	return textFrame->GetTextRange() ;
}
PowerPoint::TextRangePtr CPowerPointInterface::getTextRange(PowerPoint::ShapeRangePtr shape)
{
	PowerPoint::TextFramePtr textFrame = shape->GetTextFrame() ;
	return textFrame->GetTextRange() ;
}

PowerPoint::TextRangePtr CPowerPointInterface::getTextRangeExtent(PowerPoint::TextRangePtr range, long selLen )
{
	long textExtent = (selLen-m_queryStart)+1 ;
	try
	{
		range->Select() ;
		PowerPoint::TextRangePtr text_range = range->Characters( m_queryStart, textExtent ) ;
		text_range->Select() ;
		return text_range ;
	}
	catch (_com_error& e)
	{
		logging::log_error("Failed to set text range from selected shape. Assuming shape is grouped") ;
		logging::log_exception(e) ;
	}
	// Now we try on the assumption that we have a grouped text range
	PowerPoint::SelectionPtr selection = range->Application->ActiveWindow->Selection ;
	PowerPoint::TextRangePtr sel_range = selection->TextRange ;

	PowerPoint::TextFramePtr frame = sel_range->Parent ;
	if (!frame)
	{
		ATLTRACE("Failed to retrieve text frame from selection\n") ;
	}
	PowerPoint::TextRangePtr shape_range = frame->TextRange ;

	PowerPoint::TextRangePtr text_range = shape_range->Characters( m_queryStart, textExtent ) ;
	text_range->Select() ;
	return text_range ;
}

void CPowerPointInterface::selectNextObject(void)
{
	PowerPoint::PpViewType viewType = m_app->ActiveWindow->ActivePane->ViewType ;

	if ( viewType == PowerPoint::ppViewNotesPage )
	{
		select_next_slide() ;
		OnLookupNextAction() ;
		return ;
	}	

	// ok, move on to the shaperange...
	PowerPoint::ShapeRangePtr shapeRange = getSelection()->ShapeRange ;

	Office::MsoShapeType type = shapeRange->Type ;

	if( type == Office::msoTable )
	{
		select_next_cell( ) ;	
	}
	else
	{
		select_next_shape() ;
	}

}

bool CPowerPointInterface::setSelTextRange(void)
{
	PowerPoint::TextRangePtr selectionTextRange = m_textRangeParser.getSetRange() ;

	if ( ! selectionTextRange || ! selectionTextRange->Length) return false ;

	// We see if we already have a selection...
	return setTransText( selectionTextRange ) ;
}

bool CPowerPointInterface::setSelShapeRange(void)
{
	return setTransText(getSetSelShapeRange()) ;
}

PowerPoint::TextRangePtr CPowerPointInterface::getSetSelShapeRange()
{
	PowerPoint::ShapeRangePtr shapeRange = getSelection()->ShapeRange ;

	if( ! shapeRange ) 
	{
		return NULL ;
	}

	TRACE(shapeRange->Type);
	PowerPoint::TextRangePtr selectionTextRange = getSelectionTextRange() ;
	if( ! selectionTextRange ) 
	{
		return NULL ;
	}

	if( Office::msoTable == shapeRange->Type ) 
	{
		return setTableCell(selectionTextRange) ;
	}

	// now get the range for this text frame
	long selStart, selLen ;
	selectionTextRange->get_Start( &selStart ) ;
	selectionTextRange->get_Length( &selLen ) ;

	selLen += selStart ;
	--selLen ; // make up for 1-based index

	CTextRangeParser parser ;
	parser.setRangeFromSelection() ;
	PowerPoint::TextRangePtr textRange = parser.m_range ;

	if ( !textRange ) 
		return NULL ;

	// get the translation
	long textExtent = (selLen-m_queryStart)+1 ;
	return textRange->Characters( m_queryStart, textExtent ) ;
}

bool CPowerPointInterface::setTransText(PowerPoint::TextRangePtr range)
{
	TRACE(range->Text) ;
	range->Select() ;
	TRACE(range->Text) ;
	CPPTFormatter formatter( range ) ;
	_bstr_t transText( formatter.formatRangeText().c_str() ) ;

	if ( transText.length() > 0 ) 
	{
		getAssistant()->Trans = transText ;
		return true ;
	}

	return false ;
}

bool CPowerPointInterface::setCorrectionText(PowerPoint::TextRangePtr range)
{
	range->Select() ;
	CPPTFormatter formatter( range ) ;
	_bstr_t transText( formatter.formatRangeText().c_str() ) ;

	if ( transText.length() > 0 ) 
	{
		getAssistant()->CorrectTrans( transText ) ;
		return true ;
	}

	return false ;
}

bool CPowerPointInterface::is_ignorable( const wstring& text, bool skip_numbers, int skip_j )
{
	if ( skip_numbers && str::is_number(text))
	{
		return true ;
	}

	switch( skip_j )
	{
	case SKIP_IF_J:
		return this->has_j(text) ;

	case SKIP_UNLESS_J:
		return ! this->has_j(text) ;

	case NO_SKIP:
		return false ;
	}

	return false ;
}

bool CPowerPointInterface::has_j( const wstring &text )
{
	foreach( wchar_t ch, text )
	{
		if ( str::is_double_byte( ch ) )
		{
			return true ;
		}
	}

	return false ;
}