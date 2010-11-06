/********************************************************************
	created:	2004/11/09
	created:	9:11:2004   15:48
	filename: 	C:\Programming\Programming Projects\Felix Interface for Word\WordController.cpp
	file path:	C:\Programming\Programming Projects\Felix Interface for Word
	file base:	WordController
	file ext:	cpp
	author:		Ryan Ginstrom
	
	purpose:	implementation of the WordController class

*********************************************************************/

// Code walkthrough [2004-11-9]

#include "stdafx.h"
#include "CStringInterface.h"
#include "WordController.h"

#include "WordParser.h"
#include "word_writer.h"

#include "Path.h"

#include "InputNumDlg.h"
#include "AddGlossDialog.h"

#include "WordScreenUpdateLock.h"

#include ".\wordcontroller.h"
#include "input_device_file.h"

#include "HtmlHelp.h"
#pragma message("   automatic link to HtmlHelp.lib")
#pragma comment(lib, "HtmlHelp.lib")

using namespace except ;

#define CATCH_C_EXCEPTIONS(msg) catch ( std::exception &e ) { return handle_exception( e, msg ) ; }
#define CATCH_COM_EXCEPTIONS(msg) catch ( CComException &e ) { return handle_exception( e, msg ) ; }
#define CATCH_WIN_EXCEPTIONS(msg) catch ( CWinException &e ) { return handle_exception( e, msg ) ; }
#define CATCH_SW_EXCEPTIONS(msg) catch ( CSWException &e ) { return handle_exception( e, msg ) ; }
#define CATCH_EXCEPTIONS(msg) catch ( CException &e ) { return handle_exception( e, msg ) ; }
#define CATCH_COM_ERRORS(msg) catch (_com_error &e ) { return handle_exception( e, msg ) ; }

#define CATCH_ALL(msg) CATCH_C_EXCEPTIONS(_T(msg)) CATCH_COM_EXCEPTIONS(_T(msg)) CATCH_WIN_EXCEPTIONS(_T(msg)) CATCH_SW_EXCEPTIONS(_T(msg)) CATCH_EXCEPTIONS(_T(msg)) CATCH_COM_ERRORS(_T(msg))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


WordController::WordController(LPCWSTR history_server) :
	m_is_trans_mode(true),
	m_isAndSetAction(false),
	m_history_name(history_server),
	m_trans_history(NULL),
	m_app(NULL),
	m_is_auto(false)
{
	logging::log_debug("Initializing Word controller") ;
	m_properties.read_from_registry() ;
	load_abbreviations();
	HRESULT hr = Word2HtmlObject::CreateInstance( &m_word2html ) ;
	if ( SUCCEEDED( hr ) )
	{
		m_word2html->AddRef() ;
		m_word2html->set_properties(&m_properties) ;
	}

	if (m_properties.m_use_trans_hist)
	{
		ensure_trans_history();
	}
}

WordController::~WordController()
{
	dispose_trans_history() ;
}

/*!
 * \brief
 * Write brief comment for set_app here.
 */
void WordController::set_app(LPDISPATCH Application)
{
	try
	{
		m_word_object.set_app( Application ) ;
		m_app = Application ;
	}
	catch (CException& e)
	{
		logging::log_debug("Failed to set application") ;
		logging::log_exception(e) ;
	}
}

// Can the segment be ingored according to our criteria?
bool WordController::is_ignorable( const wstring& text, bool skip_numbers, int skip_j )
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

// Does the segment have any Asian characters?
bool WordController::has_j( const wstring &text )
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

/*!
 * \brief
 * Auto translate all the 100% matches in the current selection.
 */
bool WordController::OnAutoTransAction ( bool as_plaintext )
{
	try
	{
		m_is_auto = true ;
		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnMem2TransAction( as_plaintext ) ;
		}

		Felix::IAppPtr app = getAssistant( ) ;

		// set_status_bar_text( IDS_MENU_AUTO_TRANS_SEL_TT ) ;
		//	Dim StartPos As Long
		//	StartPos = Selection.start

		MSWord::SelectionPtr selection = m_word_object.get_selection() ;

		if( selection->Start == selection->End )
		{
			return true ;
		}

		//	Selection.Collapse direction:=wdCollapseEnd

		MSWord::RangePtr endRange = selection->Range ;

		//	ActiveDocument.Bookmarks.Add Name:="EndAutoRange", Range:=Selection.Range

		//	Selection.start = StartPos
		//	Selection.Collapse direction:=wdCollapseStart

		ATLASSERT ( get_query_start() >= 0 ) ; 
		
		MSWord::RangePtr range = selection->Range ;
		set_query_start(range->Start) ;
		range->End = range->Start ;
		range->Select() ;

		//	While Selection.End <= ActiveDocument.Bookmarks("EndAutoRange").Range.start

		selection = m_word_object.get_selection() ;
		while( selection->End < endRange->End && selection->Start < endRange->End )
		{
			TRACE( endRange->End ) ;
			//	EatWhiteSpace
			//	oParser.SelectNextSentence

			bool success = false ;
			success = select_next_sentence() ;

			//	' some minimal error checking
			//	If IsAtEndOfStory Then
			//	ActiveDocument.Bookmarks("EndAutoRange").Delete
			//	Exit Sub
			//	End If

			if ( ! success )
			{
				return true ;
			}

			TRACE( selection->End ) ;

			ATLASSERT ( get_query_start() >= 0 ) ; 
			set_query_start(selection->Start) ;


			app->Query = get_selection_text( as_plaintext ) ;

			selection = m_word_object.get_selection() ;
			if ( selection->End >= endRange->End )
			{
				endRange->Start = endRange->End ;
				endRange->Select() ;
				return true ;
			}
			//	If oAssistant.score = 1 Then
			//	oParser.WriteTranslation oAssistant.Translation
			//	End If

			double score = app->Score ;

			if ( FLOAT_EQ( score, 1.0 ) )
			{
				OnGetAction( as_plaintext ) ;
			}

			//	Selection.Collapse direction:=wdCollapseEnd
			selection = m_word_object.get_selection() ;
			_variant_t direction( (int)MSWord::wdCollapseEnd ) ;
			selection->Collapse( &direction ) ;

			if (escape_is_pressed())
			{
				m_is_auto = false ;
				return true ;
			}
		}
		m_is_auto = false ;

	}
	CATCH_ALL("Action: Auto translating current selection")


	return true ;
}

void WordController::load_abbreviations()
{
	input_device_ptr input(new InputDeviceFile) ;
	output_device_ptr output(new OutputDeviceFile) ;
	m_abbreviations.load(get_config_text(_T("abbreviations.txt"), output, input)) ;
}
bool WordController::escape_is_pressed()
{
	SHORT val = ::GetAsyncKeyState(VK_ESCAPE) & 0x8000 ;
	return !! (val) ;
}
void WordController::dispose_trans_history()
{
	if (m_trans_history)
	{
		try
		{
			delete m_trans_history ;
			m_trans_history = NULL ;
		}
		catch (CException& e)
		{
			logging::log_debug("Error while disposing of translation history component") ;
			logging::log_exception(e) ;
		}
		catch (_com_error& e)
		{
			logging::log_debug("Error while disposing of translation history component") ;
			logging::log_exception(e) ;
		}
	}
}
void WordController::close_doc(IDispatch* doc)
{
	if (m_properties.m_use_trans_hist)
	{
		try
		{
			ensure_trans_history() ;
			VARIANT var ;
			var.vt = VT_DISPATCH ;
			var.pdispVal = doc ;
			m_trans_history->method(L"ShutDown", var) ;
		}
		catch (CException& e)
		{
			logging::log_debug("Error while handling close-document action in translation history") ;
			logging::log_exception(e) ;
		}
		catch (_com_error& e)
		{
			logging::log_debug("Error while handling close-document action in translation history") ;
			logging::log_exception(e) ;
		}
	}
}

wstring WordController::get_active_doc_name()
{
	try
	{
		return BSTR2wstring(m_word_object.get_active_document()->FullName) ;
	}
	catch (CException& e)
	{
		logging::log_error("Failed to get name of active document") ;
		logging::log_exception(e) ;
		return wstring(L"WordDocument") ;
	}
	catch (_com_error& e)
	{
		logging::log_error("Failed to get name of active document") ;
		logging::log_exception(e) ;
		return wstring(L"WordDocument") ;
	}
}

void WordController::ensure_trans_history()
{
	if(!m_trans_history)
	{
		if (m_app || m_word_object.is_instantiated())
		{
			m_trans_history = new CDispatchWrapper(m_history_name.c_str()) ;
			VARIANT var ;
			var.vt = VT_DISPATCH ;
			if (m_app)
			{
				var.pdispVal = m_app ;
			}
			else
			{
				var.pdispVal = static_cast< LPDISPATCH >( (word_app_ptr)m_word_object ) ;
			}
			m_trans_history->prop_put(L"App", var) ;
		}
	}
}

void WordController::set_parser( LPDISPATCH parser )
{
	try
	{
		ensure_trans_history() ;
		VARIANT var ;
		var.vt = VT_DISPATCH ;
		var.pdispVal = parser ;
		m_trans_history->prop_put(L"Parser", var) ;
	}
	catch (CException& e)
	{
		logging::log_error("Failed to set parser in trans history") ;
		logging::log_exception(e) ;
	}
	catch (_com_error& e)
	{
		logging::log_error("Failed to set parser in trans history") ;
		logging::log_exception(e) ;
	}
}
CString WordController::get_help_file_name()
{
	file::CPath faq_path ;
	faq_path.GetModulePath( _AtlModule.GetResourceInstance() ) ;
	faq_path.RemoveBackslash() ;
	faq_path.MakeLong() ;
	faq_path.Append( _T("Docs\\Felix Help.chm") ) ;
	return faq_path.Path() ;
}
void WordController::set_query_start( long query_start )
{
	wstring key = get_active_doc_name() ;
	m_query_starts[key] = query_start ;
	m_query_start = query_start ;
}
long WordController::get_query_start()
{
	wstring key = get_active_doc_name() ;
	map_type::iterator pos = m_query_starts.find(key) ;
	if (pos != m_query_starts.end())
	{
		return pos->second ;
	}
	m_query_starts[key] = m_query_start ;
	return m_query_start ;
}
/*!
 * \brief
 * Auto translate until a fuzzy match is reached.
 */
bool WordController::OnAutoTransFuzzyAction(  bool as_plaintext  )
{
	try
	{
		m_is_auto = true ;
		
		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnTrans2MemAction( as_plaintext ) ;
		}
		
		while ( true ) 
		{
			bool success = false ;
			success = OnLookupNextAction( as_plaintext ) ;

			if ( ! success )
			{
				return true ;
			}
			
			if ( m_word_object.selection_is_empty() )
			{
				return true ;
			}

			Felix::IAppPtr app = getAssistant( ) ;
			double score = app->Score ;

			if ( FLOAT_EQ( score, 1.0 ) )
			{
				OnGetAction( as_plaintext ) ;
			}
			else
			{
				return true ;
			}
			
			if (escape_is_pressed())
			{
				m_is_auto = false ;
				return true ;
			}
		} 
	}	
	CATCH_ALL("Action: Auto translating until next fuzzy segment")
		
	return true ;
}

/*!
 * \brief
 * Restore the translation in the Felix memory.
 */
bool WordController::OnRestoreAction( bool as_plaintext )
{
	try
	{
		
		Felix::IAppPtr app = getAssistant( ) ;

		long startPos = m_word_object.get_selection()->Start ;

		wstring trans = BSTR2wstring( app->Trans ) ;
		write_translation( trans, as_plaintext ) ;

		if ( ! FLOAT_EQ( 1.f, app->Score ) && ! m_isAndSetAction)
		{
			MSWord::SelectionPtr sel = m_word_object.get_selection() ;
			sel->Start = startPos ;
			sel->End = startPos ;
			sel->Select() ;
		}
		else if (m_isAndSetAction)
		{
			if (m_properties.m_use_trans_hist)
			{
				m_word2html->set_plaintext(as_plaintext) ;
				CComPtr<IDispatch> parser ;
				m_word2html->QueryInterface(&parser); 
				this->set_parser(parser) ;
				ensure_trans_history();
				m_trans_history->method(L"RecordTrans") ;
			}
		}

	}
	CATCH_ALL("Action: Restoring the Felix translation to the Word file")
		
	return true ;
}

/*!
 * \brief
 * Write brief comment for OnRestoreAndNextAction here.
 */
bool WordController::OnRestoreAndNextAction( bool as_plaintext )
{
	try
	{
		m_isAndSetAction = true ;
		bool success = OnRestoreAction(as_plaintext) ;
		m_isAndSetAction = false ;

		if(! success)
		{
			return false ;
		}

		if ( ! OnLookupNextTransAction(as_plaintext) ) 
		{
			return false ;
		}
	}
	CATCH_ALL("Action: Restoring translation and selecting next segment")
	
	return true ;
}


/*!
 * \brief
 * get and set translations.
 */
bool WordController::OnGetAction( bool as_plaintext )
{
	try
	{
		
		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnRestoreAction(as_plaintext) ;
		}

		// Line 1: dim app as object 
		Felix::IAppPtr app = getAssistant( ) ;

		long startPos = m_word_object.get_selection()->Start ;

		wstring trans = BSTR2wstring( app->Trans ) ;
		write_translation( trans, as_plaintext ) ;

		if ( ! FLOAT_EQ( 1.f, app->Score ) && ! m_isAndSetAction)
		{
			MSWord::SelectionPtr sel = m_word_object.get_selection() ;
			sel->Start = startPos ;
			sel->End = startPos ;
			sel->Select() ;
		}
		else if (m_isAndSetAction)
		{
			if (m_properties.m_use_trans_hist)
			{
				m_word2html->set_plaintext(as_plaintext) ;
				CComPtr<IDispatch> parser ;
				m_word2html->QueryInterface(&parser); 
				this->set_parser(parser) ;
				ensure_trans_history();
				m_trans_history->method(L"RecordTrans") ;
			}
		}
	
	}
	CATCH_ALL("Action: Retrieving translation from Felix")
		
	return true ;
}

/*!
 * \brief
 * Write brief comment for OnSetAction here.
 */
bool WordController::OnSetAction( bool as_plaintext ) 
{
	try
	{
		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnCorrectAction(as_plaintext) ;
		}
		// set_status_bar_text( IDS_MENU_REGISTER_TT ) ;
		WordSelection selection = m_word_object.get_selection() ;

		if ( selection.get_start() == selection.get_end() )
		{
			ATLASSERT ( get_query_start() >= 0 ) ; 
			ATLASSERT ( get_query_start() <= selection.get_end() ) ; 
			
			if ( abs( selection.get_end() - get_query_start() ) > 1000 ) 
			{
				if ( IDNO == MessageBox( NULL, _("You are about to set a long translation in Felix. Are you sure?"), _("Felix Warning"), MB_YESNO | MB_SETFOREGROUND | MB_ICONWARNING ) )
				{
					return false ;
				}
			}
			ATLASSERT ( get_query_start() >= 0 ) ; 
			selection.put_start( get_query_start() ) ;
		}
		else
		{
			set_query_start(selection.get_start()) ;
			ATLASSERT ( get_query_start() >= 0 ) ; 
		}

		Felix::IAppPtr app = getAssistant( ) ;
		app->Trans = get_selection_text( as_plaintext ) ;

		if (m_properties.m_use_trans_hist)
		{
			ensure_trans_history();
			m_word2html->set_plaintext(as_plaintext) ;
			CComPtr<IDispatch> parser ;
			m_word2html->QueryInterface(&parser); 
			this->set_parser(parser) ;
			m_trans_history->method(L"RecordTrans") ;
		}
	}
	CATCH_ALL("Action: Adding translation to Felix")
		
	return true ;
}

/*!
 * Gets the translation and looks up the next sentence.
 */
bool WordController::OnGetAndNextAction( bool as_plaintext ) 
{
	try
	{
		
		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnRestoreAndNextAction(as_plaintext) ;
		}
	
		m_isAndSetAction = true ;
		OnGetAction( as_plaintext ) ;
		m_isAndSetAction = false ;

		Felix::IAppPtr app = getAssistant( ) ;
		if (! FLOAT_EQ(app->Score, 1.0))
		{
			if (! OnSetAction( as_plaintext ))
			{
				return false ;
			}
		}
		OnLookupNextAction( as_plaintext ) ;
	}
	CATCH_ALL("Action: Retrieving translation and selecting next segment")
		
	return true ;
}


/*!
 * Sets the translation and looks up the next sentence.
 */
bool WordController::OnSetAndNextAction( bool as_plaintext ) 
{
	try
	{
		
		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnCorrectAndNextAction(as_plaintext) ;
		}
		// set_status_bar_text( IDS_MENU_SET_AND_NEXT_TT ) ;
	
		if (OnSetAction( as_plaintext ))
		{
			OnLookupNextAction( as_plaintext ) ;
		}

	}
	CATCH_ALL("Action: Adding translation to Felix and looking up next segment")
		
	return true ;
}


/*!
 * \brief
 * Looks up the current selection.
 */
bool WordController::OnLookupAction( bool as_plaintext ) 
{
	try
	{
		
		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnLookupTransAction(as_plaintext) ;
		}

		try
		{
			WordSelection selection = m_word_object.get_selection() ;
			ATLASSERT ( get_query_start() >= 0 ) ; 
			set_query_start( selection.get_start() ) ;
			ATLASSERT ( get_query_start() >= 0 ) ; 
		}
		catch (CSWException& e)
		{
			logging::log_error("Failed to obtain Word selection") ;
			logging::log_exception(e) ;
			throw CSWException(e) ;
		}
		catch (CException& e)
		{
			logging::log_error("Failed to obtain Word selection") ;
			logging::log_exception(e) ;
			throw CException(e) ;
		}
		catch (_com_error& e)
		{
			logging::log_error("Failed to obtain Word selection (_com_error)") ;
			CComException ce(e) ;
			logging::log_exception(ce) ;
			throw CComException(e) ;
		}

		Felix::IAppPtr app = getAssistant( ) ;

		try
		{
			if (m_is_auto)
			{
				app->Query = get_selection_text(as_plaintext) ;
			}
			else
			{
				try
				{
					app->LookupDeferred(get_selection_text(as_plaintext)) ;
				}
				catch (CSWException& e)
				{
					logging::log_error("Failed to do deferred lookup") ;
					logging::log_exception(e) ;
					Felix::IAppPtr app = getAssistant( ) ;
					app->Query = get_selection_text(as_plaintext) ;
				}
			}
		}
		catch (CSWException& e)
		{
			logging::log_error("Failed to set Felix query (structured windows exception)") ;
			throw CSWException(e) ;
		}
		catch (CException& e)
		{
			logging::log_error("Failed to set Felix query (program exception)") ;
			throw CException(e) ;
		}
		catch (_com_error& e)
		{
			logging::log_error("Failed to set Felix query (_com_error)") ;
			throw CComException(e) ;
		}


	}
	CATCH_ALL("Action: Looking up current selection")
		
	return true ;
}

/*!
 * \brief
 * Selects and looks up the next segment.
 */
bool WordController::OnLookupNextAction( bool as_plaintext ) 
{
	try
	{
		
		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnLookupNextTransAction(as_plaintext) ;
		}

		select_next_sentence() ;	

		OnLookupAction(as_plaintext) ;
		
	}
	CATCH_ALL("Action: Looking up next segment")

	return true ;
}


/*!
 * Get concordance for the current selection.
 */
bool WordController::OnConcordanceAction( bool as_plaintext ) 
{
	try
	{
		
		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnTransConcordanceAction(as_plaintext) ;
		}
	
		_bstr_t text = get_selection_text( as_plaintext ) ;

		if ( text.length() == 0 )
		{
			return false ;
		}

		Felix::IAppPtr app = getAssistant( ) ;
		app->Concordance( text ) ;
	
	}
	CATCH_ALL("Action: Getting concordance for selection")
		
	return true ;
}


/*!
 * Extend the lookup segment.
 */
bool WordController::OnExtendLookupAction( bool as_plaintext ) 
{
	try
	{

		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			return OnExtendTransLookupAction(as_plaintext) ;
		}

		if ( m_word_object.selection_is_empty() )
		{
			return false ;
		}
		
		if ( ! extend_lookup( ) )
		{
			return false ;
		}

		return OnLookupAction( as_plaintext ) ;

		// set_status_bar_text( IDS_MENU_EXTEND_TT ) ;
		
	}
	CATCH_ALL("Action: Extending the lookup segment")
}



/*!
 * \brief
 * register a glossary entry.
 */
bool WordController::OnRegisterGlossAction ( bool /*as_plaintext*/ )
{
	try
	{
		_bstr_t trans ;
		_bstr_t source = get_selection_text( true ) ; // as plain text

		if ( m_properties.get_preferred_gui_lang() == PREF_LANG_ENGLISH )
		{
			CAddGlossDlgE dlg ;
			if (m_is_trans_mode)
			{
				dlg.m_source = (LPCWSTR)source ;
			}
			else
			{
				dlg.m_trans = (LPCWSTR)source ;
			}
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
			if (m_is_trans_mode)
			{
				dlg.m_source = (LPCWSTR)source ;
			}
			else
			{
				dlg.m_trans = (LPCWSTR)source ;
			}
			if ( IDCANCEL == dlg.DoModal( ) ) 
			{
				return false ;
			}
			trans = (LPCWSTR)dlg.m_trans ;
			source = (LPCWSTR)dlg.m_source ;
		}

	
		Felix::IAppPtr app = getAssistant( ) ;
		_bstr_t context = L"" ;
		app->AddGlossaryEntry( source, trans, context ) ;

		return true ;
	
	}
	CATCH_ALL("Action: Registering a glossary entry")
		
}


/*!
 * Delete the currently displayed translation.
 */
bool WordController::OnDeleteAction ( ) 
{
	try
	{
		
		Felix::IAppPtr app = getAssistant( ) ;
		app->DeleteMemEntry() ;
	}
	CATCH_ALL("Action: Deleting the current translation in Felix")
		
	return true ;
}


/*!
 * \brief
 * navigate through matches.
 */
bool WordController::OnNextAction ( ) 
{
	try
	{
		
		Felix::IAppPtr app = getAssistant( ) ;
		app->NextTrans() ;
	
	}
	CATCH_ALL("Action: Navigating to the next translation in Felix")
		
	return true ;
}


/*!
 * \brief
 * navigate through matches.
 */
bool WordController::OnPrevAction ( ) 
{
	try
	{
		
		Felix::IAppPtr app = getAssistant( ) ;
		app->PrevTrans() ;
		
	}
	CATCH_ALL("Action: Navigate to previous match in Felix")
		
	return true ;
}

/*!
* \brief
* get glossary entry 0.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* Retrieves entry 0 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \remarks
* Will retrieve entry 0 or entry 10, depending on whether Felix is set to 1-based 
* or 0-based indexing.
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry0Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_0_TT ) ;
	
		write_gloss_entry( 0, as_plaintext ) ;

	}
	CATCH_ALL("Action: Retrieving glossary entry 0")
		
	return true ;
}

/*!
* \brief
* get glossary entry 1.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Retrieves entry 1 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry1Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_1_TT ) ;
	
		write_gloss_entry( 1, as_plaintext  ) ;
	}
	CATCH_ALL("Action: Retrieving glossary entry")
		
	return true ;
}

/*!
* \brief
* get glossary entry 2.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Retrieves entry 2 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry2Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_2_TT ) ;
	
		write_gloss_entry( 2, as_plaintext  ) ;
		
	}
	CATCH_ALL("Action: Retrieving glossary entry")
		
	return true ;
}

/*!
* \brief
* get glossary entry 3.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Retrieves entry 3 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry3Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_3_TT ) ;
	
		write_gloss_entry( 3, as_plaintext  ) ;
		
	}
	CATCH_ALL("Action: Retrieving glossary entry")
		
	return true ;
}

/*!
* \brief
* get glossary entry 4.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Retrieves entry 4 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry4Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_4_TT ) ;
	
		write_gloss_entry( 4, as_plaintext  ) ;
		
	}
	CATCH_ALL("Action: Retrieving glossary entry")
	
	return true ;
}

/*!
* \brief
* get glossary entry 5.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Retrieves entry 5 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry5Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_5_TT ) ;
	
		write_gloss_entry( 5, as_plaintext  ) ;
		
	}
	CATCH_ALL("Action: Retrieving glossary entry")
	
	return true ;
}

/*!
* \brief
* get glossary entry 6.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Retrieves entry 6 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry6Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_6_TT ) ;
	
		write_gloss_entry( 6, as_plaintext  ) ;
		
	}
	CATCH_ALL("Action: Retrieving glossary entry")
	
	return true ;
}

/*!
* \brief
* get glossary entry 7.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Retrieves entry 7 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry7Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_7_TT ) ;
	
		write_gloss_entry( 7, as_plaintext  ) ;
		
	}
	CATCH_ALL("Action: Retrieving glossary entry")
	
	return true ;
}

/*!
* \brief
* get glossary entry 8.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Retrieves entry 8 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry8Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_8_TT ) ;
	
		write_gloss_entry( 8, as_plaintext  ) ;
		
	}
	CATCH_ALL("Action: Retrieving glossary entry")
	
	return true ;
}

/*!
* \brief
* get glossary entry 9.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Retrieves entry 9 and inserts it at the current cursor location. If as_plaintext
* is true, it will insert the entry as plain text. Otherwise, it will include all
* formatting set in the preferences (app_state).
* 
* \see
* app_state | CParserBase
*/
bool WordController::OnEntry9Action ( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_ENTRY_9_TT ) ;
	
		write_gloss_entry( 9, as_plaintext  ) ;
		
	}
	CATCH_ALL("Action: Retrieving glossary entry")
	
	return true ;
}

/*!
* \brief
* Get user-specified glossary entry.
* 
* \param bool as_plaintext
* Get the entry as plain text?
* 
* \returns
* bool - success
* 
* Pops up a dialog asking for a glossary entry number. Then retrieves that entry from
* Felix, and inserts it at the current cursor position. See OnEntry0Action
* for details.
* 
* \remarks
* Useful when the desired glossary entry is out of the 1-10/0-9 range. 
* 
* \see
* app_state | CParserBase | OnEntry0Action
*/
bool WordController::OnGlossNAction( bool as_plaintext ) 
{
	try
	{
		
		// set_status_bar_text( IDS_MENU_GLOSS_N_TT ) ;
	
		CInputNumDlg dlg ;

		if ( dlg.DoModal() == IDCANCEL )
		{
			return true ;
		}

		int selection = dlg.get_selection() ;
		ATLASSERT( selection >= 0 ) ;
		
		if ( selection < 0 )
		{
			return true ;
		}

		write_gloss_entry( selection, as_plaintext  ) ;
		
	}
	CATCH_ALL("Action: Retrieving glossary entry")
	
	return true ;
}


/*!
 * \brief
 * look up translation.
 */
bool WordController::OnLookupTransAction ( bool as_plaintext ) 
{
	try
	{
		WordSelection selection = m_word_object.get_selection() ;
		set_query_start( selection.get_start() );
		ATLASSERT(get_query_start() >= 0) ; 

		if (m_properties.m_use_trans_hist)
		{
			m_word2html->set_plaintext(as_plaintext) ;
			CComPtr<IDispatch> parser ;
			m_word2html->QueryInterface(&parser); 
			this->set_parser(parser) ;
			ensure_trans_history();
			m_trans_history->method(L"LookupTrans") ;
		}
		else
		{
			Felix::IAppPtr app = getAssistant() ;
			if (m_is_auto)
			{
				app->LookupTrans(get_selection_text(as_plaintext)) ;
			}
			else
			{
				app->LookupTransDeferred(get_selection_text(as_plaintext)) ;
			}
		}
	}
	CATCH_ALL("Action: Looking up the current translation")
	
	return true ;
}

/*!
 * \brief
 * Write brief comment for OnLookupNextTransAction here.
 */
bool WordController::OnLookupNextTransAction ( bool as_plaintext ) 
{
	try
	{
		
		if ( ! select_next_sentence() )
		{
			return false ;
		}
		
		OnLookupTransAction(as_plaintext) ;

	}
	CATCH_ALL("Action: Selecting and looking up the next translation")
	
	return true ;
}

/*!
 * \brief
 * Write brief comment for OnTransConcordanceAction here.
 */
bool WordController::OnTransConcordanceAction ( bool as_plaintext ) 
{
	try
	{
		
		Felix::IAppPtr app = getAssistant( ) ;
		app->TransConcordance( get_selection_text( as_plaintext ) ) ;

	}
	CATCH_ALL("Action: Getting translation concordance")
	
	return true ;
}

/*!
 * \brief
 * Write brief comment for OnExtendTransLookupAction here.
 */
bool WordController::OnExtendTransLookupAction ( bool as_plaintext ) 
{
	try
	{
		
		if ( m_word_object.selection_is_empty() ) 
		{
			return false ;
		}

		if ( ! extend_lookup( ) )
		{
			return false ;
		}

		return OnLookupTransAction( as_plaintext ) ;

	}
	CATCH_ALL("Action: Extending the lookup of the translation segment")
}

/*!
 * \brief
 * Write brief comment for OnCorrectAndNextAction here.
 */
bool WordController::OnCorrectAndNextAction ( bool as_plaintext )
{
	try
	{

		bool success = OnCorrectAction( as_plaintext ) ;
		if( ! success )
		{
			return false ;
		}

		if( ! OnLookupNextTransAction(as_plaintext) )
		{
			return false ;
		}
	}
	CATCH_ALL("Action: Correcting current translation and selecting next segment")

	return true ;
}

/*!
 * \brief
 * Write brief comment for OnCorrectAction here.
 */
bool WordController::OnCorrectAction ( bool as_plaintext ) 
{
	try
	{
		WordSelection selection = m_word_object.get_selection() ;

		wstring method_name ;
		if ( selection.get_start() == selection.get_end() )
		{
			ATLASSERT ( get_query_start() >= 0 ) ; 
			if ( abs( selection.get_end() - get_query_start() ) > 1000 ) 
			{
				if ( IDNO == MessageBox( NULL, _("You are about to look up a long segment in Felix. Are you sure?"), _("Felix Warning"), MB_YESNO | MB_SETFOREGROUND | MB_ICONWARNING ) )
				{
					return false ;

				}
			}
			ATLASSERT ( get_query_start() >= 0 ) ; 
			selection.put_start( get_query_start() ) ;
			method_name = L"CorrectTrans" ;
		}
		else
		{
			set_query_start( selection.get_start() );
			ATLASSERT ( get_query_start() >= 0 ) ; 
			method_name = L"CorrectTransFixed" ;
		}

		if (m_properties.m_use_trans_hist)
		{
			m_word2html->set_plaintext(as_plaintext) ;
			CComPtr<IDispatch> parser ;
			m_word2html->QueryInterface(&parser); 
			this->set_parser(parser) ;
			ensure_trans_history();
			m_trans_history->method(method_name.c_str()) ;
		}
		else
		{
			Felix::IAppPtr app = getAssistant( ) ;
			app->CorrectTrans(get_selection_text( as_plaintext )) ;
		}

	}
	CATCH_ALL("Action: Correcting translation")
	
	return true ;
}


/*!
 * \brief
 * Write brief comment for OnShowContextMenu here.
 */
bool WordController::OnShowContextMenu() 
{

	
	return true ;
}

/*!
 * \brief
 * Write brief comment for OnShowContextMenuCtl here.
 */
bool WordController::OnShowContextMenuCtl() 
{

	return true ;
}


/*!
 * \brief
 * Save the active memory in Felix.
 */
bool WordController::OnSaveMemoryAction()
{
	try
	{
		
		Felix::IAppPtr app = getAssistant( ) ;

		app->Save() ;
	}
	CATCH_ALL("Action: Saving Felix memory/glossary")
	
	return true ;
}

/*!
 * \brief
 * Show help file for WordAssist.
  */
bool WordController::OnHelpAction()
{
	try
	{
	
		file::CPath faq_path(get_help_file_name());

		if ( faq_path.FileExists() )
		{
			CString current_directory = faq_path.Path() ;
			current_directory << _T("::/5.html") ;
			HtmlHelp(NULL, current_directory, HH_DISPLAY_TOPIC, NULL ); 
			return 0L ;
		}
		app_state properties ;
		properties.read_from_registry() ;

		CString html_path(_T("http://felix-cat.com/media/manuals/felix/5.html")) ;
		
		CLEAR_WINERRORS ;
		
		int res = (int)::ShellExecute(
			GetDesktopWindow(),		//	HWND hwnd, 
			TEXT("open"),			// LPCTSTR lpOperation,
			html_path,				// LPCTSTR lpFile, 
			NULL,					// LPCTSTR lpParameters, 
			NULL,					// LPCTSTR lpDirectory,
			SW_SHOW					// INT nShowCmd
			);	
	
		if ( res < 32 ) // error
		{
			TRACE_WINERR( res ) ;
			ATLASSERT( FALSE && "ShellExecute failed" ) ;
		}
		CLEAR_WINERRORS ;
	}
	CATCH_ALL("Action: Showing help file")
	
	return true ;
}


/*!
 * \brief
 * Select the next sentence in word.
 */
bool WordController::select_next_sentence()
{
	typedef boost::shared_ptr<WordParser> parser_ptr ;

	CWordScreenUpdateLock screen_lock( m_word_object ) ;
		
	WordSelection selection = m_word_object.get_selection() ;
		
	parser_ptr parser = parser_ptr(new WordParser( selection, &m_properties, &m_abbreviations )) ;
	while ( parser->select_next_sentence( ) )
	{
		WordSelection selection = m_word_object.get_selection() ;
		wstring text = BSTR2wstring( selection.get_text() ) ;

		if ( ! this->is_ignorable(text, 
								parser->m_skipNumbers, 
								parser->m_skipJ))
		{
			set_query_start( selection.get_start() ) ;
			ATLASSERT ( get_query_start() >= 0 ) ; 
			try
			{
				parser = parser_ptr() ;
			}
			catch (CException& e)
			{
				logging::log_error("WordController::select_next_sentence - Error destroying WordParser object") ;
				logging::log_exception(e) ;
			}
			return true ;
		}
	}
	try
	{
		parser = parser_ptr() ;
	}
	catch (CException& e)
	{
		logging::log_error("WordController::select_next_sentence - Error destroying WordParser object") ;
		logging::log_exception(e) ;
	}

	return false ;
}

/*!
 * \brief
 * Write brief comment for get_selection_text here.
  */
_bstr_t WordController::get_selection_text(bool as_plaintext )
{
	typedef boost::shared_ptr<WordParser> parser_ptr ;
	CWordScreenUpdateLock screen_lock( m_word_object ) ;
		
	WordSelection selection = m_word_object.get_selection() ;
	
	parser_ptr parser = parser_ptr(new WordParser( selection, &m_properties, &m_abbreviations )) ;

	_bstr_t text ;
	if ( as_plaintext || ! parser->formatting_enabled())
	{
		text = parser->segment_as_plaintext( ) ;
	}
	else
	{
		text = parser->segment_as_html( ) ;
	}

	try
	{
		parser = parser_ptr() ;
	}
	catch (CException& e)
	{
		logging::log_error("WordController::get_selection_text - Error destroying WordParser object") ;
		logging::log_exception(e) ;
	}
	return text ;
}

/*!
 * \brief
 * Write brief comment for configure_parser_font_settings here.
 */
void WordController::configure_parser_font_settings(boost::shared_ptr<WordParser> parser)
{
	m_word2html->set_properties(&m_properties) ;

	parser->setPrefs(&m_properties) ;
}

/*!
 * \brief
 * Write brief comment for write_gloss_entry here.
 */
void WordController::write_gloss_entry( int index, bool as_plaintext )
{
	ATLASSERT( index >= 0 ) ;

	Felix::IAppPtr app = getAssistant( ) ;
	wstring entry = BSTR2wstring( app->GlossMatch[static_cast<short>(index)] );
	
	if ( as_plaintext )
	{
		boost::to_lower( entry ) ;
	}

	write_translation( entry, as_plaintext ) ;
}

/*!
 * \brief
 * Write brief comment for OnWindowDeactivate here.
 */
void WordController::OnWindowDeactivate()
{
	m_word_object.deactivate() ;
}

/*!
 * \brief
 * Write brief comment for OnWindowActivate here.
 */
void WordController::OnWindowActivate()
{
	m_word_object.activate() ;
}

/*!
 * \brief
 * Write brief comment for extend_lookup here.
 */
bool WordController::extend_lookup( )
{
	ATLASSERT ( get_query_start() >= 0 ) ; 
	long old_start = get_query_start() ;
	
	if ( ! select_next_sentence() )
	{
		WordSelection selection = m_word_object.get_selection() ;
		selection.put_start( old_start ) ;

		set_query_start(old_start) ;
		ATLASSERT ( get_query_start() >= 0 ) ; 

		return false ;
	}
	
	WordSelection selection = m_word_object.get_selection() ;
	selection.set_start( old_start ) ;

	set_query_start( old_start ) ;
	ATLASSERT ( get_query_start() >= 0 ) ; 

	return true ;
}

/*!
 * \brief
 * Write brief comment for new_window_has_focus here.
 */
bool WordController::new_window_has_focus()
{
	return ( ! m_word_object.active_window_has_focus() ) ;
}

/*!
 * \brief
 * Write brief comment for set_status_bar_text here.
  */
void WordController::set_status_bar_text( const int string_id )
{
	int local_string_id = string_id ;

	if ( m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE ) 
	{
		int string_offset = IDS_MENU_JAPANESE_FIRST - IDS_MENU_ENGLISH_FIRST ;
		local_string_id += string_offset ;
	}

	CStringW sb_text ;
	sb_text.LoadString( local_string_id ) ;

	m_word_object.set_status_bar( (LPCWSTR)sb_text ) ;
}

/*!
 * \brief
 * Write brief comment for OnSwitchModeAction here.
  */
void WordController::OnSwitchModeAction()
{
	m_is_trans_mode = ! m_is_trans_mode ;
}

bool WordController::is_translation_mode()
{
	return m_is_trans_mode ;
}

/*!
 * \brief
 * Select to the next period/maru or other segment-ending character (!, ?, ...).
 */
bool WordController::OnToMaruAction(bool as_plaintext)
{
	try
	{
		
		WordSelection selection = m_word_object.get_selection() ;
		
		boost::shared_ptr<WordParser> parser = 
			boost::shared_ptr<WordParser>(new WordParser( selection, &m_properties, &m_abbreviations )) ;

		if( ! parser->select_to_maru( ) )
		{
			try
			{
				parser = boost::shared_ptr<WordParser>() ;
			}
			catch (CException& e)
			{
				logging::log_error("Error destroying WordParser object") ;
				logging::log_exception(e) ;
			}
			return false ;
		}
		
		set_query_start( selection.get_start() ) ;
		ATLASSERT ( get_query_start() >= 0 ) ; 

		if ( ! ( m_properties.get_classic_if() || m_is_trans_mode ) ) 
		{
			try
			{
				parser = boost::shared_ptr<WordParser>() ;
			}
			catch (CException& e)
			{
				logging::log_error("Error destroying WordParser object") ;
				logging::log_exception(e) ;
			}
			return OnLookupTransAction(as_plaintext) ;
		}

		OnLookupAction(as_plaintext) ;
		try
		{
			parser = boost::shared_ptr<WordParser>() ;
		}
		catch (CException& e)
		{
			logging::log_error("Error destroying WordParser object") ;
			logging::log_exception(e) ;
		}
	}
	CATCH_ALL("Action: Extending lookup to next end-of-sentence marker")

	return true ;
}

bool WordController::OnMem2TransAction(bool as_plaintext)
{
	m_is_auto = true ;
	WordSelection selection = m_word_object.get_selection() ;

	boost::shared_ptr<WordParser> parser 
		= boost::shared_ptr<WordParser>(new WordParser( selection, &m_properties, &m_abbreviations )) ;

	word_writer writer( selection ) ;
	
	configure_writer_font_settings(writer);	

	Felix::IAppPtr app = getAssistant( ) ;

	while ( parser->select_next_sentence( ) )
	{
		_bstr_t sel_text = get_selection_text( as_plaintext ) ;

		if ( sel_text.length() == 0 )
		{
			try
			{
				parser = boost::shared_ptr<WordParser>() ;
			}
			catch (CException& e)
			{
				logging::log_error("Error destroying WordParser object") ;
				logging::log_exception(e) ;
			}
			return false ;
		}

		app->LookupTrans( sel_text ) ;

		double trans_score = app->Score ;

		if ( trans_score >= 0.80 ) 
		{
			wstring text = BSTR2wstring( app->Trans ) ;
			
			if ( as_plaintext )
			{
				writer.write_plaintext( wstring( text ) ) ;
			}
			else
			{
				writer.write_html( text ) ;
			}
		}
		else
		{
			long old_start = selection.get_start() ;
			long old_end = selection.get_end() ;
			parser->select_next_sentence() ;
			long end_pos = selection.get_end() ;
			WordRange range = selection.get_range() ;
			if ( end_pos == old_end ) 
			{
				range.select_range(end_pos, end_pos) ;
				try
				{
					parser = boost::shared_ptr<WordParser>() ;
				}
				catch (CException& e)
				{
					logging::log_error("Error destroying WordParser object") ;
					logging::log_exception(e) ;
				}
				return true ;
			}
			range.select_range(old_start, end_pos) ;
			app->LookupTrans( get_selection_text( as_plaintext ) ) ;

			double trans_score_2 = app->Score ;
			if ( trans_score_2 >= 0.80 ) 
			{
				wstring text = BSTR2wstring( app->Trans ) ;
				if ( as_plaintext )
				{
					writer.write_plaintext( wstring( text ) ) ;
				}
				else
				{
					writer.write_html( text ) ;
				}
			}
			else if ( trans_score < trans_score_2 ) // try again
			{
				parser->select_next_sentence() ;
				end_pos = selection.get_end() ;
				range = selection.get_range() ;
				range.select_range(old_start, end_pos) ;
				
				app->LookupTrans( get_selection_text( as_plaintext ) ) ; 

				double trans_score_3 = app->Score ;
				if ( trans_score_3 >= 0.80 ) 
				{
					wstring text = BSTR2wstring( app->Trans ) ;
					if ( as_plaintext )
					{
						writer.write_plaintext( wstring( text ) ) ;
					}
					else
					{
						writer.write_html( text ) ;
					}
				}
				else
				{
					range.select_range( old_end, old_end ) ;
				}
				
			}
			else
			{
				range.select_range( old_end, old_end ) ;
			}
		}
	}
	try
	{
		parser = boost::shared_ptr<WordParser>() ;
	}
	catch (CException& e)
	{
		logging::log_error("Error destroying WordParser object") ;
		logging::log_exception(e) ;
	}

	return true ;
}

void WordController::configure_writer_font_settings(word_writer& writer)
{
	writer.set_write_font_face( m_properties.get_font_face() ) ;
	writer.set_write_font_color( m_properties.get_font_color() ) ;
	writer.set_write_font_bold( m_properties.get_font_bold() ) ;
	writer.set_write_font_italic( m_properties.get_font_italic() ) ;
	writer.set_write_font_underline( m_properties.get_font_underline() ) ;
	writer.set_write_font_super_sub( m_properties.get_font_super_sub() ) ;
}

bool WordController::OnTrans2MemAction(bool as_plaintext)
{
	m_is_auto = true ;
	if (m_properties.m_use_trans_hist)
	{
		m_word2html->set_plaintext(as_plaintext) ;
		IDispatch *parser ;
		m_word2html->QueryInterface(&parser); 
		this->set_parser(parser) ;
		ensure_trans_history();
		m_trans_history->method(L"ReflectChanges") ;
		parser->Release() ;
	}
	else
	{
		WordSelection selection = m_word_object.get_selection() ;

		boost::shared_ptr<WordParser> parser 
			= boost::shared_ptr<WordParser>(new WordParser( selection, &m_properties, &m_abbreviations )) ;

		Felix::IAppPtr app = getAssistant( ) ;

		while ( parser->select_next_sentence( ) )
		{
			_bstr_t sel_text = get_selection_text( as_plaintext ) ;

			if ( sel_text.length() == 0 )
			{
				try
				{
					parser = boost::shared_ptr<WordParser>() ;
				}
				catch (CException& e)
				{
					logging::log_error("Error destroying WordParser object") ;
					logging::log_exception(e) ;
				}
				return false ;
			}

			app->LookupTrans( sel_text ) ;

			double trans_score = app->Score ;

			if ( trans_score >= 0.80 ) 
			{
				app->CorrectTrans( sel_text ) ;
			}
			else
			{
				long old_start = selection.get_start() ;
				long old_end = selection.get_end() ;
				parser->select_next_sentence() ;
				long end_pos = selection.get_end() ;
				WordRange range = selection.get_range() ;
				if ( end_pos == old_end ) 
				{
					range.select_range(end_pos, end_pos) ;
					try
					{
						parser = boost::shared_ptr<WordParser>() ;
					}
					catch (CException& e)
					{
						logging::log_error("Error destroying WordParser object") ;
						logging::log_exception(e) ;
					}
					return true ;
				}
				range.select_range(old_start, end_pos) ;
				app->LookupTrans( get_selection_text( as_plaintext ) ) ;
				double trans_score_2 = app->Score ;
				if ( trans_score_2 >= 0.80 ) 
				{
					app->CorrectTrans( get_selection_text( as_plaintext ) ) ;
				}
				else if ( trans_score < trans_score_2 ) // try again
				{
					parser->select_next_sentence() ;
					end_pos = selection.get_end() ;
					range = selection.get_range() ;
					range.select_range(old_start, end_pos) ;
					app->LookupTrans( get_selection_text( as_plaintext ) ) ;
					double trans_score_3 = app->Score ;
					if ( trans_score_3 >= 0.80 ) 
					{
						app->CorrectTrans( get_selection_text( as_plaintext ) ) ;
					}
					else
					{
						range.select_range( old_end, old_end ) ;
					}

				}
				else
				{
					range.select_range( old_end, old_end ) ;
				}
			}
		}
		try
		{
			parser = boost::shared_ptr<WordParser>() ;
		}
		catch (CException& e)
		{
			logging::log_error("Error destroying WordParser object") ;
			logging::log_exception(e) ;
		}
	}
	
	return true ;
}

// Tells our controller that a new document has opened
void WordController::OnDocumentOpenAction(LPDISPATCH doc)
{
	if (m_properties.m_use_trans_hist)
	{
		try
		{
			ensure_trans_history() ;
			VARIANT var ;
			var.vt = VT_DISPATCH ;
			var.pdispVal = doc ;
			m_trans_history->method(L"OpenDoc", var) ;
		}
		catch (CException& e)
		{
			logging::log_debug("Error while handling open-document action in translation history") ;
			logging::log_exception(e) ;
		}
		catch (_com_error& e)
		{
			logging::log_debug("Error while handling open-document action in translation history") ;
			logging::log_exception(e) ;
		}
	}
	set_query_start(0) ;
}





void WordController::write_translation(const wstring& text, bool as_plaintext )
{
	// set_status_bar_text( IDS_MENU_GET_TT ) ;
	WordSelection selection = m_word_object.get_selection() ;

	word_writer writer( selection ) ;

	if ( as_plaintext )
	{
		writer.write_plaintext( wstring( text ) ) ;
	}
	else
	{
		writer.set_write_font_face( m_properties.get_font_face() ) ;
		writer.set_write_font_color( m_properties.get_font_color() ) ;
		writer.set_write_font_bold( m_properties.get_font_bold() ) ;
		writer.set_write_font_italic( m_properties.get_font_italic() ) ;
		writer.set_write_font_underline( m_properties.get_font_underline() ) ;
		writer.set_write_font_super_sub( m_properties.get_font_super_sub() ) ;

		writer.write_html( text ) ;
	}
}

Felix::IAppPtr WordController::getAssistant(void)
{
	try
	{
		Felix::IAppPtr assistant ;
		COM_ENFORCE(assistant.CreateInstance( L"Felix.App" ), _T("Failed to connect to Felix window") ) ;

		assistant->Visible = VARIANT_TRUE ;
		return assistant ;
	}
	catch (_com_error& e)
	{
		CComException com_exept(e) ;

		logging::log_error("Failed to connect to Felix window") ;
		logging::log_exception(com_exept) ;

		CString message(_T("Failed to connect to Felix window")) ;
		com_exept.add_to_message(message) ;
		throw CComException(com_exept) ;
	}

}

void WordController::SetQueryStart( long start )
{
	set_query_start( start ) ;
}
bool WordController::refresh_properties( boost::any &Param )
{
	BANNER( "WordParser::OnPreferencesChanged" ) ;
	return set_properties(boost::any_cast<app_state>(Param));
}

bool WordController::IsActive()
{
	return m_word_object.is_active() ;
}

bool WordController::OnShiftStateAction()
{
	return true ;
}

bool WordController::handle_exception( std::exception &e, CString msg )
{
	CString message( "Runtime Error\r\nCall to Felix.WordInterface on automation server failed.\r\n" ) ;	
	message << _("Error: " ) << CA2T(e.what()) ;
	message << _T("\r\n") << msg ;
	MessageBox( NULL, message, _("C Runtime Error"), MB_OK | MB_ICONHAND | MB_SETFOREGROUND | MB_SYSTEMMODAL ) ;

	logging::log_error("std::exception") ;
	logging::log_error(e.what()) ;
	return false ;
}

bool WordController::handle_exception( CComException &com_exception, CString msg )
{
	CString message( _("COM Error\r\nCall to Felix.WordInterface on automation server failed.\r\n") ) ;	
	message << _T("\r\n") << msg ;
	com_exception.notify_user( message ) ;								

	logging::log_error("COM Exception") ;
	logging::log_exception(com_exception) ;
	return false ;
}

bool WordController::handle_exception( CWinException &e, CString msg )
{
	CString message( _("Windows Error\r\nCall to Felix.WordInterface on automation server failed.\r\n") ) ;	
	message << _T("\r\n") << msg ;
	e.notify_user( message ) ;								

	logging::log_error("Windows Error") ;
	logging::log_exception(e) ;
	return false ;
}

bool WordController::handle_exception( CSWException &e, CString msg )
{
	logging::log_error(string("Structured Windows Exception: ") + (LPCSTR)CT2A(msg)) ;
	logging::log_exception(e) ;
	CString language = L"English" ;
	if(m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE)
	{
		language = L"Japanese" ;
	}
	logging::send_report(language, e.get_filename()) ;
	return false ;
}

bool WordController::handle_exception( _com_error &e, CString msg )
{
	CComException com_exception(_("COM Error\r\nCall to Felix.WordInterface on automation server failed.\r\n"), e) ;					
	CString message( _("COM Error") ) ;	
	message << _T("\r\n") << msg ;
	com_exception.notify_user( message ) ;								

	logging::log_error("COM Error") ;
	logging::log_exception(com_exception) ;
	return false ;
}

bool WordController::handle_exception( CException &e, CString msg )
{
	CString message( _("Call to Felix.WordInterface on automation server failed.\r\n") ) ;	
	message << _T("\r\n") << msg ;
	e.notify_user( message ) ;								

	logging::log_error("Call to Felix.WordInterface on automation server failed.") ;
	logging::log_exception(e) ;
	return false ;
}