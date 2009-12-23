// KeyboardListener.h: interface for the CKeyboardListener class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDLISTENER_H__65F63A6C_820B_47D5_BB35_067D48D329FE__INCLUDED_)
#define AFX_KEYBOARDLISTENER_H__65F63A6C_820B_47D5_BB35_067D48D329FE__INCLUDED_

#pragma once

#include "WordAutomationObject.h" // for word_app_ptr
#include "app_state.h"

class CKeyboardListener  
{
public:
	virtual void set_auto_off() = 0 ;
	//////////////////////////////////////////////////////////////////////////
	// these are only needed by the Word interface, not the keyboard listener
	//////////////////////////////////////////////////////////////////////////
	virtual bool is_translation_mode() = 0 ;
	virtual void OnSwitchModeAction() = 0 ;
	virtual bool OnHelpAction() = 0 ;
	virtual void OnWindowDeactivate() = 0 ;
	virtual void OnWindowActivate() = 0 ;
	virtual void OnDocumentOpenAction(IDispatch *doc) = 0 ;
	virtual void set_app(LPDISPATCH Application) = 0 ;
	virtual bool set_properties( const app_state params ) = 0 ;

	virtual void shut_down() = 0 ;
	virtual void close_doc(IDispatch* doc) = 0 ;
	//////////////////////////////////////////////////////////////////////////
	// these are actually needed by the keyboard listener
	//////////////////////////////////////////////////////////////////////////

	virtual bool IsActive() = 0 ;

	virtual bool OnAutoTransAction ( bool as_plaintext ) = 0 ;
	virtual bool OnAutoTransFuzzyAction ( bool as_plaintext ) = 0 ;

	// get and set translations
	virtual bool OnGetAction( bool as_plaintext ) = 0 ;
	virtual bool OnSetAction( bool as_plaintext ) = 0 ;
	virtual bool OnGetAndNextAction( bool as_plaintext ) = 0 ;
	virtual bool OnSetAndNextAction( bool as_plaintext ) = 0 ;

	// look up source segment
	virtual bool OnLookupAction( bool as_plaintext ) = 0 ;
	virtual bool OnLookupNextAction( bool as_plaintext ) = 0 ;
	virtual bool OnConcordanceAction ( bool as_plaintext ) = 0 ;
	virtual bool OnExtendLookupAction ( bool as_plaintext ) = 0 ;
	virtual bool OnToMaruAction ( bool as_plaintext ) = 0 ;

	// register a glossary entry
	virtual bool OnRegisterGlossAction ( bool as_plaintext ) = 0 ;
	// delete entry
	virtual bool OnDeleteAction ( ) = 0 ;
	// navigate through matches
	virtual bool OnNextAction ( ) = 0 ;
	virtual bool OnPrevAction ( ) = 0 ;
	// get glossary entries
	virtual bool OnEntry0Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry1Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry2Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry3Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry4Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry5Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry6Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry7Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry8Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry9Action ( bool as_plaintext ) = 0 ;
	virtual bool OnGlossNAction( bool as_plaintext ) = 0 ;

	// look up translation
	virtual bool OnLookupTransAction ( bool as_plaintext ) = 0 ;
	virtual bool OnLookupNextTransAction ( bool as_plaintext ) = 0 ;
	virtual bool OnTransConcordanceAction ( bool as_plaintext ) = 0 ;
	virtual bool OnExtendTransLookupAction ( bool as_plaintext ) = 0 ;

	virtual bool OnCorrectAction ( bool as_plaintext ) = 0 ;
	virtual bool OnCorrectAndNextAction ( bool as_plaintext ) = 0 ;

	virtual bool OnRestoreAction ( bool as_plaintext ) = 0 ;
	virtual bool OnRestoreAndNextAction ( bool as_plaintext ) = 0 ;

	virtual bool OnSaveMemoryAction( ) = 0 ;
	virtual bool OnShiftStateAction( ) = 0 ;

	virtual bool OnAnalyze() = 0 ;
	//	IDS_MENU_AUTO_TRANS_SEL					"&Auto Translate Selection (ALT + M, A)"
	//	IDS_MENU_REGISTER_GLOSS					"Regis&ter As Glossary Entry (ALT + M, G)"
	//	IDS_MENU_GLOSS_N						"Specify &Number... (ALT + M, H)"
	//	IDS_MENU_LOOKUP_TRANS					"&Find Current Translation (CTL + ALT + M, L)"
	//	IDS_MENU_CONCORDANCE_TRANS				"Translat&ion Concordance (CTL + ALT + M, C)"
	//	IDS_MENU_CONCORDANCE					"Find C&oncordance (ALT + M, C)"
	//	IDS_MENU_SAVE							"Save &Memory (ALT + M, S)"
//	virtual bool OnShowContextMenu() = 0 ;
//	virtual bool OnShowContextMenuCtl() = 0 ;
};

#endif // !defined(AFX_KEYBOARDLISTENER_H__65F63A6C_820B_47D5_BB35_067D48D329FE__INCLUDED_)
