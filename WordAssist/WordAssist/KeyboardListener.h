#pragma once

#include "WordAutomationObject.h" // for word_app_ptr
#include "app_state.h"
#include "key_target_word.h"

class CKeyboardListener : public KeyTargetWord
{
public:
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

	virtual void load_abbreviations() = 0 ;
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
