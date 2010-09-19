#pragma once
#include "WordController.h"
#include "KeyboardListener.h"
#include "wordscreenupdatelock.h"

class CWordControllerWord :
	public CKeyboardListener,
	public WordController
{
public:
	CWordControllerWord();
	~CWordControllerWord(void);
	void set_auto_off()
	{
		m_is_auto = false ;
	}

	bool is_translation_mode() { return WordController::is_translation_mode() ; }

	void OnSwitchModeAction() { WordController::OnSwitchModeAction() ; }
	bool OnHelpAction() { return WordController::OnHelpAction() ; }

	void OnWindowDeactivate() { WordController::OnWindowDeactivate() ; }
	void OnWindowActivate() { WordController::OnWindowActivate() ; }
	void OnDocumentOpenAction( IDispatch *doc )
	{
		WordController::OnDocumentOpenAction(doc) ;
	}	
	void set_app(LPDISPATCH Application)
	{
		WordController::set_app(Application) ;
	}
	bool set_properties( const app_state params )
	{
		return WordController::set_properties(params) ;
	}
	void shut_down()
	{
		dispose_trans_history();
	}
	void close_doc( IDispatch* doc )
	{
		WordController::close_doc(doc) ;
	}
	//////////////////////////////////////////////////////////////////////////
	// these are actually needed by the keyboard listener
	//////////////////////////////////////////////////////////////////////////

	bool IsActive() { return WordController::IsActive() ; }

	bool OnAutoTransAction ( bool as_plaintext ) { return WordController::OnAutoTransAction(as_plaintext) ; }
	bool OnAutoTransFuzzyAction ( bool as_plaintext ) { return WordController::OnAutoTransFuzzyAction(as_plaintext) ; }

	// get and set translations
	bool OnGetAction( bool as_plaintext ) { return WordController::OnGetAction(as_plaintext) ; }
	bool OnSetAction( bool as_plaintext ) { return WordController::OnSetAction(as_plaintext) ; }
	bool OnGetAndNextAction( bool as_plaintext ) 
	{
		set_auto_off() ;
		return WordController::OnGetAndNextAction(as_plaintext) ; 
	}
	bool OnSetAndNextAction( bool as_plaintext ) 
	{ 
		set_auto_off() ;
		return WordController::OnSetAndNextAction(as_plaintext) ; 
	}

	// look up source segment
	bool OnLookupAction( bool as_plaintext ) 
	{ 
		set_auto_off() ;
		return WordController::OnLookupAction(as_plaintext) ; 
	}
	bool OnLookupNextAction( bool as_plaintext ) { return WordController::OnLookupNextAction(as_plaintext) ; }
	bool OnConcordanceAction ( bool as_plaintext ) { return WordController::OnConcordanceAction(as_plaintext) ; }
	bool OnExtendLookupAction ( bool as_plaintext ) { return WordController::OnExtendLookupAction(as_plaintext) ; }
	bool OnToMaruAction ( bool as_plaintext ) { return WordController::OnToMaruAction(as_plaintext) ; }

	// register a glossary entry
	bool OnRegisterGlossAction ( bool as_plaintext ) { return WordController::OnRegisterGlossAction(as_plaintext) ; }
	// delete entry
	bool OnDeleteAction ( ) { return WordController::OnDeleteAction() ; }
	// navigate through matches
	bool OnNextAction ( ) { return WordController::OnNextAction() ; }
	bool OnPrevAction ( ) { return WordController::OnPrevAction() ; }
	// get glossary entries
	bool OnEntry0Action ( bool as_plaintext ) { return WordController::OnEntry0Action(as_plaintext) ; }
	bool OnEntry1Action ( bool as_plaintext ) { return WordController::OnEntry1Action(as_plaintext) ; }
	bool OnEntry2Action ( bool as_plaintext ) { return WordController::OnEntry2Action(as_plaintext) ; }
	bool OnEntry3Action ( bool as_plaintext ) { return WordController::OnEntry3Action(as_plaintext) ; }
	bool OnEntry4Action ( bool as_plaintext ) { return WordController::OnEntry4Action(as_plaintext) ; }
	bool OnEntry5Action ( bool as_plaintext ) { return WordController::OnEntry5Action(as_plaintext) ; }
	bool OnEntry6Action ( bool as_plaintext ) { return WordController::OnEntry6Action(as_plaintext) ; }
	bool OnEntry7Action ( bool as_plaintext ) { return WordController::OnEntry7Action(as_plaintext) ; }
	bool OnEntry8Action ( bool as_plaintext ) { return WordController::OnEntry8Action(as_plaintext) ; }
	bool OnEntry9Action ( bool as_plaintext ) { return WordController::OnEntry9Action(as_plaintext) ; }
	bool OnGlossNAction( bool as_plaintext ) { return WordController::OnGlossNAction(as_plaintext) ; }

	// look up translation
	bool OnLookupTransAction ( bool as_plaintext ) {return  WordController::OnLookupTransAction(as_plaintext) ; }
	bool OnLookupNextTransAction ( bool as_plaintext ) { return WordController::OnLookupNextTransAction(as_plaintext) ; }
	bool OnTransConcordanceAction ( bool as_plaintext ) { return WordController::OnTransConcordanceAction(as_plaintext) ; }
	bool OnExtendTransLookupAction ( bool as_plaintext ) { return WordController::OnExtendTransLookupAction(as_plaintext) ; }

	bool OnCorrectAction ( bool as_plaintext ) { return WordController::OnCorrectAction(as_plaintext) ; }
	bool OnCorrectAndNextAction ( bool as_plaintext ) { return WordController::OnCorrectAndNextAction(as_plaintext) ; }

	bool OnRestoreAction ( bool as_plaintext ) { return WordController::OnRestoreAction(as_plaintext) ; }
	bool OnRestoreAndNextAction ( bool as_plaintext ) { return WordController::OnRestoreAndNextAction(as_plaintext) ; }

	bool OnSaveMemoryAction( ) { return WordController::OnSaveMemoryAction( ) ; }
	bool OnShiftStateAction( ) { return WordController::OnShiftStateAction( ) ; }

	bool OnAnalyze();
};
