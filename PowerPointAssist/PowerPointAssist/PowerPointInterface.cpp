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
#include "logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

using namespace except ;

#define _HR( x ) \
{ ATLTRACE( "Test: [" #x "]\n" ) ;\
	HRESULT _hr = (x) ; \
	if ( FAILED( _hr ) ) \
{ \
	throw CAtlException( _hr ) ;\
} }
#define _HRB( x ) \
if ( FAILED( x ) ) \
{ \
	return false ;\
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPowerPointInterface::CPowerPointInterface() : 
	m_queryStart( 0 ),
	m_is_auto(false)
{
	m_ok_endings += L"Mr.", L"Mrs.", L"Ms.", L"Dr.", L"e.g.", L"i.e." ;

}

CPowerPointInterface::~CPowerPointInterface()
{

}

// getSelectionTextRange
PowerPoint::TextRangePtr CPowerPointInterface::getSelectionTextRange()
{
	return getSelection()->GetTextRange() ;
}

// getSelection
PowerPoint::SelectionPtr CPowerPointInterface::getSelection()
{
	return m_app->GetActiveWindow()->GetSelection() ;
}



// OnGetAction
HRESULT CPowerPointInterface::OnGetAction(bool plaintext) 
{
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

		const wstring trans = BSTR2wstring( getAssistant()->Trans ) ;

		if ( plaintext )
		{
			writePlainText(selectionTextRange, trans);
		}
		else
		{
			CPPTWriter writer( selectionTextRange ) ;
			writer.write_html( trans ) ;
		}
	}
	ADDIN_CATCH( _T("Error processing OnGetAction command in PowerPoint Interface") )
		
	return S_OK ;

}

// writePlainText
void CPowerPointInterface::writePlainText(PowerPoint::TextRangePtr& selectionTextRange, const std::wstring trans)
{
	selectionTextRange->Text = string2BSTR(strip_tags(trans)) ;
	const int len = selectionTextRange->Length + 1 ;
	PowerPoint::TextRangePtr rng = selectionTextRange->Characters( len, 0 ) ;
	rng->Select() ;
}

// OnSetAction
HRESULT CPowerPointInterface::OnSetAction( bool ) 
{
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
	ADDIN_CATCH( _T("Error processing OnSetAction command in PowerPoint Interface") )

	return S_OK ;
}


// OnGetAndNextAction
HRESULT CPowerPointInterface::OnGetAndNextAction( bool plaintext )
{
	BANNER( "CPowerPointInterface::OnGetAndNextAction" ) ;


	try 
	{
		OnGetAction( plaintext ) ;
		OnSetAndNextAction( plaintext ) ;
	}

	ADDIN_CATCH( _T("Error processing OnGetAndNextAction command in PowerPoint Interface") )

	return S_OK ;
}


// OnSetAndNextAction 
HRESULT CPowerPointInterface::OnSetAndNextAction( bool plaintext )
{
	try 
	{
		OnSetAction( plaintext ) ;
		OnLookupNextAction( plaintext ) ;
	}

	ADDIN_CATCH( _T("Error processing OnSetAndNextAction command in PowerPoint Interface") )

	return S_OK ;
}

// OnLookupNextAction
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
	ADDIN_CATCH( _T("Error processing lookup next commmand in PowerPoint Interface") )
#else
	catch ( ... ) 
	{
		MessageBeep(MB_ICONASTERISK) ;
		return S_OK ;
	}
#endif

	return S_OK ;
}



// OnLookupAction 
HRESULT CPowerPointInterface::OnLookupAction( bool plaintext )
{
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
			if (m_is_auto)
			{
				app->Query = formatted_text ;
			}
			else
			{
				app->LookupDeferred(formatted_text) ;
			}
		}

	}

	ADDIN_CATCH( _T("Error looking up query from PowerPoint") )

	return S_OK ;
}


// OnGlossNAction 
HRESULT CPowerPointInterface::OnGlossNAction(  )
{
	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		CInputDlg dlg ;

		if ( dlg.DoModal( ) == IDCANCEL )
		{
			return S_FALSE ;
		}

		set_gloss_entry( dlg.get_num() ) ;
	}
	ADDIN_CATCH( _T("PowerPoint Interface :: Retrieving a Glossary Entry") )

	return S_OK ;
}


// OnAutoTransAction
HRESULT  CPowerPointInterface::OnAutoTransAction ( )
{
	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}
		m_is_auto = true ;

		MessageBox( NULL, _T("OnAutoTrans"), _T("To be implemented!"), MB_OK ) ;
	}

	ADDIN_CATCH( _T("Error processing OnAutoTransFuzzyAction command in PowerPoint Interface") )
	return S_OK ;
}

// OnAutoTransFuzzyAction 
HRESULT CPowerPointInterface::OnAutoTransFuzzyAction( )
{
	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}
		m_is_auto = true ;
		const bool plaintext = shift_key_is_pressed() ;

		while ( true ) 
		{
			if ( ! SUCCEEDED(OnLookupNextAction(plaintext)) )
			{
				return S_OK ;
			}

			const _bstr_t formatted_text = m_textRangeParser.getSelectedText( plaintext ) ;
			// don't send empty queries!
			if ( formatted_text.length() == 0 ) 
			{
				return S_OK ;
			}

			Felix::IAppPtr app = getAssistant( ) ;

			// If it's a perfect match, retrieve the translation and continue.
			// Otherwise, quit with the fuzzy match selected.
			if ( FLOAT_EQ( app->Score, 1.0 ) )
			{
				OnGetAction(plaintext) ;
			}
			else
			{
				return S_OK ;
			}

		} 
	} 
	ADDIN_CATCH( _T("Error processing OnAutoTransFuzzyAction command in PowerPoint Interface") ) ;
		
	return S_OK ;
}

// OnConcordanceAction
HRESULT CPowerPointInterface::OnConcordanceAction( bool )
{
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
	ADDIN_CATCH( _T("Error processing OnConcordanceAction command in PowerPoint Interface") )
	return S_OK ;
}

// OnExtendLookupAction
HRESULT CPowerPointInterface::OnExtendLookupAction ( bool plaintext )
{
	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		// If there is no text selected, we should not call extend selection.
		const _bstr_t selected_text = m_textRangeParser.getSelectedText(plaintext) ;
		if ( selected_text.length() == 0 )
		{
			return E_FAIL ;
		}

		if ( ! m_textRangeParser.extendSelection() )
		{
			MessageBeep( MB_ICONASTERISK ) ;
			return E_FAIL ;
		}

		const _bstr_t text = m_textRangeParser.getSelectedText(plaintext) ;

		if ( text.length() == 0 )
		{
			return E_FAIL ;
		}
		if (m_is_auto)
		{
			getAssistant()->Query = text ;
		}
		else
		{
			getAssistant()->LookupDeferred(text) ;
		}

	} 
	ADDIN_CATCH( _T("Error processing OnExtendLookupAction command in PowerPoint Interface") )
	
	return S_OK ;
}

// OnRegisterGlossAction
// Show a dialog, with the selection as the source text, and prompting
// for a translation. This is entered into Felix as a glossary entry.
HRESULT CPowerPointInterface::OnRegisterGlossAction ( bool /*as_plaintext*/)
{
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

// OnDeleteAction
HRESULT CPowerPointInterface::OnDeleteAction ( )
{
	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		Felix::IAppPtr app = getAssistant() ;
		app->DeleteMemEntry() ;

	} 
	ADDIN_CATCH( _T("Error processing OnDeleteAction command in PowerPoint Interface") )
	return S_OK ;
}

// OnNextAction 
// Show the next translation in Felix
HRESULT CPowerPointInterface::OnNextAction ( )
{
	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		Felix::IAppPtr app = getAssistant() ;
		app->NextTrans() ;

	} 
	ADDIN_CATCH( _T("Error processing OnNextAction command in PowerPoint Interface") )
	
	return S_OK ;
}

// OnPrevAction
// Show the previous translation in Felix
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
	ADDIN_CATCH( _T("Error processing OnPrevAction command in PowerPoint Interface") )
		return S_OK ;
}

// OnEntryAction 
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
	} 
	ADDIN_CATCH( _T("Error processing OnEntryAction command in PowerPoint Interface") )
	return S_OK ;
}


// OnLookupTransAction
HRESULT CPowerPointInterface::OnLookupTransAction (bool plaintext)
{
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
			if (m_is_auto)
			{
				app->LookupTrans( formatted_text ) ;
			}
			else
			{
				app->LookupTransDeferred( formatted_text ) ;
			}
		}

	}
	ADDIN_CATCH( _T("Error processing OnLookupTransAction command in PowerPoint Interface") )
	
	return S_OK ;
}


// OnLookupNextTransAction
HRESULT CPowerPointInterface::OnLookupNextTransAction ( bool plaintext )
{
	try 
	{
		if ( FAILED( lookupNext() ) )
		{
			return E_FAIL ;
		}  
		OnLookupTransAction( plaintext ) ;
	} 
#ifdef _DEBUG
	ADDIN_CATCH( _T("Error processing OnLookupNextTransAction command in PowerPoint Interface") )
#else
	catch ( ... ) 
	{
		MessageBeep(MB_ICONASTERISK) ;
		return S_OK ;
	}
#endif
	
	return S_OK ;
}

// lookupNext
HRESULT CPowerPointInterface::lookupNext()
{
	if (m_app->Active != Office::msoTrue )
	{
		return E_FAIL ;
	}

	if ( m_textRangeParser.selectNext() )
	{
		const wstring text = BSTR2wstring(m_textRangeParser.getSelectedText(true)) ;
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

	const wstring text = BSTR2wstring(m_textRangeParser.getSelectedText(true)) ;
	if (this->is_ignorable(text, !! m_properties->m_data.m_skipNumbers, m_properties->m_data.m_skipJ))
	{
		return lookupNext() ;
	}
	return S_OK ;
}


// OnTransConcordanceAction 
HRESULT CPowerPointInterface::OnTransConcordanceAction ( bool)
{
	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}

		PowerPoint::TextRangePtr textRange = getSelectionTextRange() ;

		const _bstr_t text = textRange->Text ;

		if ( text.length() > 0 )
		{
			Felix::IAppPtr app = getAssistant() ;
			app->TransConcordance( text ) ;
		}
	} 
	ADDIN_CATCH( _T("Error processing OnTransConcordanceAction command in PowerPoint Interface") )
	return S_OK ;
}


// OnExtendTransLookupAction 
HRESULT CPowerPointInterface::OnExtendTransLookupAction ( bool plaintext )
{
	try 
	{
		if ( ! m_textRangeParser.extendSelection() )
		{
			MessageBeep( MB_ICONASTERISK ) ;
			return S_OK ;
		}

		const _bstr_t text = m_textRangeParser.getSelectedText(plaintext) ;

		// don't send empty queries!
		if ( text.length() == 0 )
		{
			return E_FAIL ;
		}

		Felix::IAppPtr app = getAssistant() ;
		app->LookupTrans( text ) ;

	} 
	ADDIN_CATCH( _T("Error processing OnExtendTransLookupAction command in PowerPoint Interface") )
	return S_OK ;
}


// OnCorrectTransAction
HRESULT CPowerPointInterface::OnCorrectTransAction ( bool)
{
	BANNER( "CPowerPointInterface::OnCorrectTransAction " ) ;

	try 
	{
		if (m_app->Active != Office::msoTrue )
		{
			return E_FAIL ;
		}
		// See if there's already some selected text
		PowerPoint::TextRangePtr selectionTextRange = m_textRangeParser.getSetRange() ;
		if ( selectionTextRange && selectionTextRange->Length)
		{
			if ( ! setCorrectionText(selectionTextRange) )
			{
				return E_FAIL ;
			} 
			return S_OK ;
		}


		// Or see if there's a selected shape
		if ( ! setCorrectionText(getSetSelShapeRange()) )
		{
			return E_FAIL ;
		} 

	} 
	ADDIN_CATCH( _T("Error processing OnCorrectTransAction command in PowerPoint Interface") )
		
	return S_OK ;
}

// OnCorrectAndNextTransAction
HRESULT CPowerPointInterface::OnCorrectAndNextTransAction ( bool) 
{
	logging::log_warn("OnCorrectAndNextTransAction not supported") ;
	return S_OK ;
}
// OnRestoreTransAction
HRESULT CPowerPointInterface::OnRestoreTransAction ( bool) 
{
	logging::log_warn("OnRestoreTransAction not supported") ;
	return S_OK ;
}
// OnRestoreAndNextTransAction
HRESULT CPowerPointInterface::OnRestoreAndNextTransAction ( bool) 
{
	logging::log_warn("OnRestoreAndNextTransAction not supported") ;
	return S_OK ;
}

// set_gloss_entry
HRESULT CPowerPointInterface::set_gloss_entry(int index, bool plaintext)
{
	BANNER( "CPowerPointInterface::set_gloss_entry" ) ;

	PowerPoint::TextRangePtr selectionTextRange = getSelectionTextRange() ;
	if( ! selectionTextRange ) 
	{
		return E_FAIL ;
	}

	Felix::IAppPtr app = getAssistant() ;
	const _bstr_t entry = app->GlossMatch[ static_cast<short>(index) ] ;

	if ( plaintext )
	{
		wstring plain = boost::to_lower_copy(strip_tags(BSTR2wstring(entry))) ;
		selectionTextRange->Text = string2BSTR(plain) ;
	}
	else
	{
		CPPTWriter writer( selectionTextRange ) ;

		writer.write_html( BSTR2wstring(entry) ) ;
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

		if ( viewType == PowerPoint::ppViewNotesPage )
		{
			if ( selectionTextRange->Length > 0 )
			{
				CKeyboardDriver driver ;
				driver.clearAltKey() ;
				driver.clearControlKey() ;

				m_app->Activate() ;

				driver.doRight() ;
				selectionTextRange = getSelectionTextRange() ;
			}
		}	
		return E_FAIL ;
	}

	PowerPoint::TextRangePtr collapsed = collapse_selection( selectionTextRange ) ;
	collapsed->Select() ;

	return S_OK ;
}

// eat_whitespace
// Keep advancing the start pos while the current char is whitespace
PowerPoint::TextRangePtr CPowerPointInterface::eat_whitespace(PowerPoint::TextRangePtr &range)
{
	const long len = range->Length ;

	PowerPoint::TextRangePtr character ;

	long i ;
	for ( i=1 ; i<=len ; ++i )
	{
		character = range->Characters( i, 1 ) ;
		_bstr_t c_text = character->Text ;

		ATLASSERT( c_text.length() > 0 && c_text.GetBSTR()[0] != 0 ) ;

		if( ! iswspace( c_text.GetBSTR()[0] ) )
		{
			break ;
		}
		character.Release() ;
	}

	PowerPoint::TextRangePtr segment_range = range->Characters( i, (len-i)+1 ) ;
	ATLASSERT( segment_range ) ;
	return segment_range ;
}


// collapse_selection
PowerPoint::TextRangePtr CPowerPointInterface::collapse_selection(PowerPoint::TextRangePtr &selection)
{
	BANNER( "CAddin::collapse_selection" ) ;

	const long start  = selection->Start ;
	const long len = selection->Length ;

	return selection->Characters( start+len, 0 ) ;
}


// select_next_text_frame
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
	{
		return PowerPoint::TextRangePtr() ;
	}

	select_next_sentence_from_textrange( range ) ;
	range->Select() ;

	PowerPoint::TextRangePtr text_frame_range ;
	text_frame->get_TextRange( &text_frame_range ) ;
	return text_frame_range ;
}

// get_notes_textrange
PowerPoint::TextRangePtr CPowerPointInterface::get_notes_textrange( ) 
{
	PowerPoint::SelectionPtr selection = getSelection() ;

	PowerPoint::SlideRangePtr slides = selection->SlideRange ;
	ATLASSERT( slides ) ;
	ATLASSERT( slides->Count == 1 ) ;

	PowerPoint::_SlidePtr slide = slides->Item(1) ;
	ATLASSERT( slide ) ;

	PowerPoint::SlideRangePtr notes_page = slide->NotesPage ;
	ATLASSERT(notes_page) ;

	PowerPoint::ShapesPtr shapes ;
	notes_page->get_Shapes( &shapes ) ;
	ATLASSERT( shapes ) ;

	const int shape_count = shapes->Count ;
	ATLASSERT(shape_count >= 0) ;

	for ( int i = 1 ; i <= shape_count ; ++i )
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
			if (frame)
			{
				return frame->TextRange ;
			}
			else
			{
				return PowerPoint::TextRangePtr() ;
			}
		}

	}

	return PowerPoint::TextRangePtr() ;
}

// setNotesPage
HRESULT CPowerPointInterface::setNotesPage(PowerPoint::TextRangePtr selectionTextRange)
{
	PowerPoint::TextRangePtr range = get_notes_textrange() ;

	const long current_start = selectionTextRange->Start ;

	// Why is this needed?
	selectionTextRange.Release() ;

	const int len = current_start - m_queryStart ;
	selectionTextRange = range->Characters( m_queryStart, len ) ;

	CPPTFormatter formatter( selectionTextRange ) ;
	const wstring rangeText( formatter.formatRangeText() ) ;
	if ( rangeText.empty() )
	{
		MessageBeep( MB_ICONERROR ) ;
		return S_FALSE ;
	}
	Felix::IAppPtr app = getAssistant() ;
	app->Trans = string2BSTR(rangeText) ;

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

	const long len = selectionTextRange->Length ;

	m_queryStart = selectionTextRange->Start + len ;

	const long old_start = m_queryStart ;

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


	const int rangelen = range->Length ;

	for ( long i = 0 ; i < rangelen ; ++i )
	{
		driver.doShiftRight() ;
	}

	return range ;
}


// select_next_table_cell
PowerPoint::TextRangePtr CPowerPointInterface::select_next_table_cell(PowerPoint::TextRangePtr &selectionTextRange)
{
	CDispatchWrapper parent = selectionTextRange->GetParent() ;

	PowerPoint::ShapePtr grandParentShape = parent.prop_get( L"Parent" ).pdispVal ;

	PowerPoint::TextRangePtr textRange = getTextRange( grandParentShape ) ;

	if ( !textRange ) 
	{
		return textRange ;
	}

	select_next_sentence_from_textrange( textRange ) ;

	textRange->Select() ;

	return textRange ;
}


// select_next_sentence_from_textrange
HRESULT CPowerPointInterface::select_next_sentence_from_textrange(PowerPoint::TextRangePtr &range)
{
	BANNER( "CAddin::select_next_sentence_from_textrange" ) ;

	const long endPoint = (range->Length - m_queryStart ) + 1 ;

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

	const long len = characters->Length ;
	bool last_char_was_period = false ;

	long i ;
	for ( i=1 ; i<=len ; ++i )
	{
		character = characters->Characters( i, 1 ) ;
		_bstr_t c_text = character->Text ;
		TRACE( c_text ) ;
		character.Release() ;

		const wchar_t this_char = ((LPCWSTR)c_text)[0] ;

		if ( delimiters.find( this_char ) != wstring::npos ) 
		{
			break ;
		}

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
				ATLASSERT(iswspace(this_char)) ;
				--i ; // don't select the whitespace char itself
				break ;
			}
		}

		last_char_was_period = ( this_char == L'.' ) ;
		if (last_char_was_period)
		{
			PowerPoint::TextRangePtr chars_so_far = characters->Characters( 1, i ) ;
			const wstring text_so_far = BSTR2wstring(chars_so_far->Text) ;
			foreach(wstring word, m_ok_endings)
			{
				if (boost::ends_with(text_so_far, word))
				{
					last_char_was_period = false ;
					break ;
				}
			}
		}
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

	const long selStart = selectionTextRange->Start ;
	const long selLen = selectionTextRange->Length + selStart - 1;

	CTextRangeParser parser ;
	parser.setRangeFromSelection() ;
	PowerPoint::TextRangePtr textRange = parser.m_range ;

	if ( !textRange ) 
	{
		return NULL ;
	}

	// get the translation
	const long text_extent = (selLen-m_queryStart)+1 ;
	return textRange->Characters( m_queryStart, text_extent ) ;
}

//correct_notes_page
HRESULT CPowerPointInterface::correct_notes_page(PowerPoint::TextRangePtr &selectionTextRange)
{
	PowerPoint::TextRangePtr range = get_notes_textrange() ;

	long current_start ;
	selectionTextRange->get_Start( &current_start ) ;

	selectionTextRange = range->Characters( m_queryStart, ( current_start - m_queryStart ) ) ;

	const _bstr_t text = selectionTextRange->Text ;
	if ( text.length() == 0 )
	{
		MessageBeep( MB_ICONERROR ) ;
		return S_FALSE ;
	}

	Felix::IAppPtr app = getAssistant() ;
	app->CorrectTrans( text ) ;

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

	long cnt = windows->Count ;

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

			cnt = shapeRange->Count ;

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

// select_next_shape
HRESULT CPowerPointInterface::select_next_shape()
{
	PowerPoint::SelectionPtr selection = getSelection() ;

	// Nothing selected
	if ( ! selection ) 
	{
		return E_FAIL ;
	}

	// ok, move on to the shape range...
	PowerPoint::ShapeRangePtr shapeRange = selection->ShapeRange ;

	// No shapes in selection
	if ( shapeRange->Count == 0 )
	{
		return S_FALSE ;
	}

	PowerPoint::ShapePtr activeShape  = shapeRange->Item( 1 ) ;
	PowerPoint::DocumentWindowPtr activeWindow = m_app->ActiveWindow ;

	activeWindow->Activate() ;

	PowerPoint::PanePtr activePane = activeWindow->ActivePane ;

	// No slide selected
	if ( activePane->ViewType != PowerPoint::ppViewSlide )
	{
		return S_FALSE ;
	}

	PowerPoint::ViewPtr activeView = activeWindow->View ;
	PowerPoint::_SlidePtr active_slide = activeView->Slide  ;

	PowerPoint::ShapesPtr shapes = active_slide->Shapes ;

	const int shape_count = shapes->Count ;

	long nextShapeIndex = 1 ;
	for ( nextShapeIndex = 1 ; nextShapeIndex < shape_count ; ++nextShapeIndex ) 
	{
		PowerPoint::ShapePtr shape = shapes->Item( nextShapeIndex ) ;
		if ( shape == activeShape ) 
		{
			break ;
		}
	}

	for ( nextShapeIndex++ ; nextShapeIndex<= shape_count ; ++nextShapeIndex )
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
				// If whatever else we've selected has a text frame, then
				// we'll still grab the text even though we don't know what
				// it is!
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

// select_next_cell
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

	PowerPoint::TablePtr table = shape->Table ;

	PowerPoint::ColumnsPtr columns = table->Columns ;
	PowerPoint::RowsPtr rows = table->Rows ;

	const long num_cols = columns->Count ;
	const long num_rows = rows->Count ;

	for ( long row=1 ; row <= num_rows ; ++row )
	{
		for ( long col=1 ; col <= num_cols ; ++col)
		{
			PowerPoint::CellPtr cell = table->Cell( row, col ) ;
			shape = cell->GetShape( ) ;
			// If this cell shape is the selected shape
			if ( shape == activeShape )
			{
				// Wind past the selected shape (might be multiple "cells" (cell coords) in shape)
				while ( shape == activeShape )
				{
					// Not last col: advance
					if ( col < num_cols )
					{
						++col ;
					}
					// So next row, unless we're out
					else if ( row == num_rows ) 
					{
						return S_FALSE ;
					}
					// Back to first col, advance rows
					else
					{
						col = 1 ;
						++row ;
					}
					cell = table->Cell( row, col ) ;
					shape = cell->Shape ;
				}
				PowerPoint::TextFramePtr text_frame = shape->TextFrame;
				PowerPoint::TextRangePtr textRange = text_frame->TextRange ;
				PowerPoint::TextRangePtr start_char_sel = textRange->Characters( 1, 0 ) ;
				start_char_sel->Select() ;
				return S_OK ;
			}
		}
	}
	return S_FALSE ;
}

// getAssistant
Felix::IAppPtr CPowerPointInterface::getAssistant(void)
{
	Felix::IAppPtr assistant ;
	assistant.CreateInstance( L"Felix.App" ) ;

	assistant->Visible = VARIANT_TRUE ;

	return assistant ;
}

// has_grouped_shapes
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

// selectAutoShape
void CPowerPointInterface::selectAutoShape(PowerPoint::ShapePtr shape)
{
	selectShape( shape ) ;
}

// selectPlaceHolder
void CPowerPointInterface::selectPlaceHolder(PowerPoint::ShapePtr shape)
{
	selectShape( shape ) ;
}

// selectTextBox
void CPowerPointInterface::selectTextBox(PowerPoint::ShapePtr shape)
{
	selectShape( shape ) ;
}

// selectShape
void CPowerPointInterface::selectShape(PowerPoint::ShapePtr shape)
{
	PowerPoint::TextFramePtr text_frame = shape->TextFrame ;

	// now get the range for this text frame
	PowerPoint::TextRangePtr range = text_frame->TextRange ;

	// get the translation
	PowerPoint::TextRangePtr char_range = range->Characters( 1, 0 ) ;

	char_range->Select() ;
}

// getTextRange
PowerPoint::TextRangePtr CPowerPointInterface::getTextRange(PowerPoint::ShapePtr shape)
{
	PowerPoint::TextFramePtr textFrame = shape->GetTextFrame() ;
	return textFrame->GetTextRange() ;
}
// getTextRange
PowerPoint::TextRangePtr CPowerPointInterface::getTextRange(PowerPoint::ShapeRangePtr shape)
{
	PowerPoint::TextFramePtr textFrame = shape->GetTextFrame() ;
	return textFrame->GetTextRange() ;
}
// getTextRangeExtent
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

// selectNextObject
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