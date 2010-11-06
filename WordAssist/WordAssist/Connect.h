/*!
* The AddIn COM server that MS Word calls
* 	created:	2007/01/29
* 	author:		Ryan Ginstrom
* 	
* 	purpose:	Header for CConnect class
* 
*/

#pragma once
#include "resource.h"       // main symbols

#include "WordController.h"
#include "app_state.h"
#include "Properties.h"
#include "hooker.h"
#include "Broadcaster.h"
#include "Broker.h"
#include "Localizer.h"
#include "logging.h"

#include "keyboard_shortcuts.h"
#include "keyboard_shortcuts_word.h"
#include "key_mapper_word.h"

// static const GUID DIID__CommandBarButtonEvents = {0x000C0351,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};
//static const GUID DIID_ApplicationEvents2 =
//    {0x000209fe,0x0000,0x0000,{0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};


extern _ATL_FUNC_INFO OnClickButtonInfo ;

extern _ATL_FUNC_INFO WindowActivateInfo ;
extern _ATL_FUNC_INFO WindowDeactivateInfo ;

// 0x00000003
extern _ATL_FUNC_INFO DocumentChangeInfo ;
// 0x00000004
extern _ATL_FUNC_INFO DocumentOpenInfo ;
// 0x00000009
extern _ATL_FUNC_INFO NewDocumentInfo ;
// 0x0000000C
extern _ATL_FUNC_INFO WindowSelectionChangeInfo ;
// 0x00000006
extern _ATL_FUNC_INFO OnBeforeCloseInfo ;

#define SINK_BUTTON( id, fnc, event_impl ) SINK_ENTRY_INFO( id, __uuidof(button_events), /*dispid*/ 0x00000001, fnc, event_impl )

enum 
{ 
	BUTTON_ID_GET = 1, 
	BUTTON_ID_SET, 
	BUTTON_ID_GET_AND_NEXT, 
	BUTTON_ID_SET_AND_NEXT, 
	BUTTON_ID_LOOKUP, 
	BUTTON_ID_LOOKUP_NEXT, 
	BUTTON_ID_GLOSS_N,
	BUTTON_ID_HELP,

	BUTTON_ID_RESTORE, 
	BUTTON_ID_CORRECT, 
	BUTTON_ID_CORRECT_AND_NEXT, 
	BUTTON_ID_RESTORE_AND_NEXT, 
	BUTTON_ID_LOOKUP_TRANS, 
	BUTTON_ID_LOOKUP_NEXT_TRANS, 

	// -------------------------
	MENU_ID_AUTO_TRANS_SEL,
	MENU_ID_AUTO_TRANS_FUZZY,

	MENU_ID_LOOKUP, 
	MENU_ID_LOOKUP_NEXT, 
	MENU_ID_CONCORDANCE,

	MENU_ID_GET, 
	MENU_ID_GET_AND_NEXT, 

	MENU_ID_SET, 
	MENU_ID_SET_AND_NEXT, 

	MENU_ID_EXTEND_LOOKUP,
	MENU_ID_REGISTER_GLOSS,

	MENU_ID_DELETE,
	MENU_ID_NEXT,
	MENU_ID_PREV,

	MENU_ID_ENTRY_0,
	MENU_ID_ENTRY_1,
	MENU_ID_ENTRY_2,
	MENU_ID_ENTRY_3,
	MENU_ID_ENTRY_4,
	MENU_ID_ENTRY_5,
	MENU_ID_ENTRY_6,
	MENU_ID_ENTRY_7,
	MENU_ID_ENTRY_8,
	MENU_ID_ENTRY_9,
	MENU_ID_GLOSS_N,

	MENU_ID_LOOKUP_TRANS,
	MENU_ID_LOOKUP_NEXT_TRANS,
	MENU_ID_TRANS_CONCORDANCE,
	MENU_ID_CORRECT_TRANS,
	MENU_ID_EXTEND_LOOKUP_TRANS,

	MENU_ID_SAVE,
	MENU_ID_HELP,

	MENU_ID_MENU_PREFERENCES,

	MENU_ID_SWITCH,

	MENU_ID_ANALYZE,

	// Word events

	WORD_ACTIVATE_DOCUMENT,
	WORD_DEACTIVATE_DOCUMENT,
	WORD_DOCUMENT_CHANGE,
	WORD_DOCUMENT_OPEN,
	WORD_NEW_DOCUMENT,
	WORD_WINDOW_SEL_CHANGE,
	WORD_BEFORE_DOC_CLOSE
};

typedef MSOffice::_CommandBarButtonEvents button_events ;
typedef MSWord::ApplicationEvents2 app_events2 ;

// forward declaration
class CConnect ;


typedef IDispEventSimpleImpl<WORD_ACTIVATE_DOCUMENT,	CConnect, &__uuidof(app_events2) >  ActivateDocumentEventImpl ;
typedef IDispEventSimpleImpl<WORD_DEACTIVATE_DOCUMENT,	CConnect, &__uuidof(app_events2) >  DeActivateDocumentEventImpl ;

typedef IDispEventSimpleImpl<WORD_DOCUMENT_CHANGE,		CConnect, &__uuidof(app_events2) >  DocumentChangeEventImpl ;
typedef IDispEventSimpleImpl<WORD_DOCUMENT_OPEN,		CConnect, &__uuidof(app_events2) >  DocumentOpenEventImpl ;
typedef IDispEventSimpleImpl<WORD_NEW_DOCUMENT,			CConnect, &__uuidof(app_events2) >  NewDocumentEventImpl ;
typedef IDispEventSimpleImpl<WORD_WINDOW_SEL_CHANGE,	CConnect, &__uuidof(app_events2) >  WindowSelectionChangeEventImpl ;
typedef IDispEventSimpleImpl<WORD_BEFORE_DOC_CLOSE,		CConnect, &__uuidof(app_events2) >  BeforeDocumentCloseImpl ;

// buttons 

typedef IDispEventSimpleImpl<BUTTON_ID_GET,			CConnect, &__uuidof(button_events) >  ButtonGetEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_SET,			CConnect, &__uuidof(button_events) >  ButtonSetEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_GET_AND_NEXT,CConnect, &__uuidof(button_events) >  ButtonGetAndNextEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_SET_AND_NEXT,CConnect, &__uuidof(button_events) >  ButtonSetAndNextEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_LOOKUP,		CConnect, &__uuidof(button_events) >  ButtonLookupEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_LOOKUP_NEXT,	CConnect, &__uuidof(button_events) >  ButtonLookupNextEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_GLOSS_N,		CConnect, &__uuidof(button_events) >  ButtonGlossNEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_HELP,		CConnect, &__uuidof(button_events) >  ButtonHelpEventImpl ;

// menu items

typedef IDispEventSimpleImpl<MENU_ID_LOOKUP,		CConnect, &__uuidof(button_events) >  MenuLookupEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_LOOKUP_NEXT,	CConnect, &__uuidof(button_events) >  MenuLookupNextEventImpl ;

typedef IDispEventSimpleImpl<MENU_ID_GET,			CConnect, &__uuidof(button_events) >  MenuGetEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_SET,			CConnect, &__uuidof(button_events) >  MenuSetEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_GET_AND_NEXT,	CConnect, &__uuidof(button_events) >  MenuGetAndNextEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_SET_AND_NEXT,	CConnect, &__uuidof(button_events) >  MenuSetAndNextEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_GLOSS_N,		CConnect, &__uuidof(button_events) >  MenuGlossNEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_SAVE,			CConnect, &__uuidof(button_events) >  MenuSaveEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_HELP,			CConnect, &__uuidof(button_events) >  MenuHelpEventImpl ;

typedef IDispEventSimpleImpl<MENU_ID_AUTO_TRANS_SEL,	CConnect, &__uuidof(button_events) >  MenuAutoTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_AUTO_TRANS_FUZZY,	CConnect, &__uuidof(button_events) >  MenuAutoTransFuzzyEventImpl ;

typedef IDispEventSimpleImpl<MENU_ID_CONCORDANCE,			CConnect, &__uuidof(button_events) >  MenuConcordanceEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_EXTEND_LOOKUP,			CConnect, &__uuidof(button_events) >  MenuExtendLookupEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_REGISTER_GLOSS,		CConnect, &__uuidof(button_events) >  MenuRegisterGlossEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_DELETE,				CConnect, &__uuidof(button_events) >  MenuDeleteEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_NEXT,					CConnect, &__uuidof(button_events) >  MenuNextEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_PREV,					CConnect, &__uuidof(button_events) >  MenuPrevEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_0,				CConnect, &__uuidof(button_events) >  MenuEntry0EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_1,				CConnect, &__uuidof(button_events) >  MenuEntry1EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_2,				CConnect, &__uuidof(button_events) >  MenuEntry2EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_3,				CConnect, &__uuidof(button_events) >  MenuEntry3EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_4,				CConnect, &__uuidof(button_events) >  MenuEntry4EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_5,				CConnect, &__uuidof(button_events) >  MenuEntry5EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_6,				CConnect, &__uuidof(button_events) >  MenuEntry6EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_7,				CConnect, &__uuidof(button_events) >  MenuEntry7EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_8,				CConnect, &__uuidof(button_events) >  MenuEntry8EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_9,				CConnect, &__uuidof(button_events) >  MenuEntry9EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_LOOKUP_TRANS,			CConnect, &__uuidof(button_events) >  MenuLookupTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_LOOKUP_NEXT_TRANS,		CConnect, &__uuidof(button_events) >  MenuLookupNextTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_TRANS_CONCORDANCE,		CConnect, &__uuidof(button_events) >  MenuTransConcordanceEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_CORRECT_TRANS,			CConnect, &__uuidof(button_events) >  MenuCorrectTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_EXTEND_LOOKUP_TRANS,	CConnect, &__uuidof(button_events) >  MenuExtendTransLookupEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_MENU_PREFERENCES,		CConnect, &__uuidof(button_events) >  MenuPreferencesEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_SWITCH,		        CConnect, &__uuidof(button_events) >  MenuSwitchEventImpl ;

typedef IDispEventSimpleImpl<MENU_ID_ANALYZE,		        CConnect, &__uuidof(button_events) >  MenuAnalyzeEventImpl ;

// CConnect
class ATL_NO_VTABLE CConnect : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CConnect, &CLSID_Connect>,
	public IDispatchImpl<AddInDesignerObjects::_IDTExtensibility2, &AddInDesignerObjects::IID__IDTExtensibility2, &AddInDesignerObjects::LIBID_AddInDesignerObjects, 1, 0>,
	// Word event impl's
	public ActivateDocumentEventImpl,
	public DeActivateDocumentEventImpl,
	public DocumentChangeEventImpl ,
	public DocumentOpenEventImpl ,	
	public NewDocumentEventImpl ,	
	public WindowSelectionChangeEventImpl ,	
	public BeforeDocumentCloseImpl,

	// button event impl's
	public ButtonGetEventImpl ,       
	public ButtonSetEventImpl ,       
	public ButtonGetAndNextEventImpl ,
	public ButtonSetAndNextEventImpl ,
	public ButtonLookupEventImpl ,
	public ButtonLookupNextEventImpl,
	public ButtonGlossNEventImpl,
	public ButtonHelpEventImpl,

	// menu event impl's
	public MenuGetEventImpl ,         
	public MenuSetEventImpl ,         
	public MenuGetAndNextEventImpl ,  
	public MenuSetAndNextEventImpl ,  
	public MenuLookupEventImpl,
	public MenuLookupNextEventImpl,
	public MenuGlossNEventImpl,
	public MenuSaveEventImpl,
	public MenuHelpEventImpl ,

	public MenuAutoTransEventImpl,
	public MenuAutoTransFuzzyEventImpl,

	public MenuConcordanceEventImpl,
	public MenuExtendLookupEventImpl,
	public MenuRegisterGlossEventImpl,
	public MenuDeleteEventImpl,
	public MenuNextEventImpl,
	public MenuPrevEventImpl,
	public MenuEntry0EventImpl,
	public MenuEntry1EventImpl,
	public MenuEntry2EventImpl,
	public MenuEntry3EventImpl,
	public MenuEntry4EventImpl,
	public MenuEntry5EventImpl,
	public MenuEntry6EventImpl,
	public MenuEntry7EventImpl,
	public MenuEntry8EventImpl,
	public MenuEntry9EventImpl,
	public MenuLookupTransEventImpl,
	public MenuLookupNextTransEventImpl,
	public MenuTransConcordanceEventImpl,
	public MenuCorrectTransEventImpl,
	public MenuExtendTransLookupEventImpl,
	public MenuPreferencesEventImpl,
	public MenuSwitchEventImpl,

	public MenuAnalyzeEventImpl
{

	IDispatchPtr	m_pAddInInstance;
	word_app_ptr	m_app ;

	// toolbar buttons
	command_button_ptr	m_button_get ;
	command_button_ptr	m_button_set ;
	command_button_ptr	m_button_get_and_next ;
	command_button_ptr	m_button_set_and_next ;
	command_button_ptr	m_button_lookup ;
	command_button_ptr	m_button_lookup_next ;
	command_button_ptr	m_button_gloss_n ;
	command_button_ptr	m_button_help ;

	// menu items
	command_button_ptr	m_menu_get ;
	command_button_ptr	m_menu_set ;
	command_button_ptr	m_menu_get_and_next ;
	command_button_ptr	m_menu_set_and_next ;
	command_button_ptr	m_menu_lookup ;
	command_button_ptr	m_menu_lookup_next ;
	command_button_ptr	m_menu_gloss_n ;
	command_button_ptr	m_menu_save ;
	command_button_ptr	m_menu_help ;

	command_button_ptr	m_menu_auto_trans ;
	command_button_ptr	m_menu_auto_trans_fuzzy ;
	command_button_ptr	m_menu_concordance ;
	command_button_ptr	m_menu_extend_lookup ;
	command_button_ptr	m_menu_register_gloss ;
	command_button_ptr	m_menu_delete ;
	command_button_ptr	m_menu_next ;
	command_button_ptr	m_menu_prev ;
	command_button_ptr	m_menu_entry_0 ;
	command_button_ptr	m_menu_entry_1 ;
	command_button_ptr	m_menu_entry_2 ;
	command_button_ptr	m_menu_entry_3 ;
	command_button_ptr	m_menu_entry_4 ;
	command_button_ptr	m_menu_entry_5 ;
	command_button_ptr	m_menu_entry_6 ;
	command_button_ptr	m_menu_entry_7 ;
	command_button_ptr	m_menu_entry_8 ;
	command_button_ptr	m_menu_entry_9 ;
	command_button_ptr	m_menu_lookup_trans ;
	command_button_ptr	m_menu_lookup_next_trans ;
	command_button_ptr	m_menu_trans_concordance ;
	command_button_ptr	m_menu_correct_trans ;
	command_button_ptr	m_menu_extend_trans_lookup ;
	command_button_ptr	m_menu_preferences ;
	command_button_ptr	m_menu_switch ;

	command_button_ptr	m_MenuAnalyze ;

	office_cmd_bar_ctl m_gloss_menu_popup ;
	office_cmd_bar_ctl m_trans_assist_menu_popup ;
	// the toolbar
	CComPtr <MSOffice::CommandBar> m_toolbar ;
	CComPtr <MSOffice::CommandBar> m_revision_toolbar ;


	CKeyboardListener	*m_controller ;

	app_state			m_properties ;

	CBroadcaster::SignalPtrType m_PropChangeSig ;

	KeyMapperWord m_mapper ;
	shortcuts::KeyboardShortcuts m_keyboard_shortcuts ;


public:
	CConnect();

	DECLARE_REGISTRY_RESOURCEID(IDR_ADDIN)
	DECLARE_NOT_AGGREGATABLE(CConnect)

	BEGIN_COM_MAP(CConnect)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(AddInDesignerObjects::IDTExtensibility2)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();

	void FinalRelease();

public:
	//IDTExtensibility2 implementation:
	STDMETHOD(OnConnection)(IDispatch * Application, AddInDesignerObjects::ext_ConnectMode ConnectMode, IDispatch *AddInInst, SAFEARRAY **custom);
	STDMETHOD(OnDisconnection)(AddInDesignerObjects::ext_DisconnectMode RemoveMode, SAFEARRAY **custom );
	STDMETHOD(OnAddInsUpdate)(SAFEARRAY **custom );
	STDMETHOD(OnStartupComplete)(SAFEARRAY **custom );
	STDMETHOD(OnBeginShutdown)(SAFEARRAY **custom );

	void __stdcall  OnAutoTrans( IDispatch *, VARIANT_BOOL *  );
	void __stdcall  OnAutoTransFuzzy( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnConcordance( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnExtendLookup( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnRegisterGloss( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnDelete( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnNext( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnPrev( IDispatch *, VARIANT_BOOL * );

	void __stdcall OnEntry0( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnEntry1( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnEntry2 ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnEntry3( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnEntry4 ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnEntry5 ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnEntry6 ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnEntry7( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnEntry8 ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnEntry9 ( IDispatch *, VARIANT_BOOL * );

	void __stdcall OnLookupTrans ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnLookupNextTrans ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnTransConcordance ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnExtendTransLookup ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnCorrect ( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnCorrectAndNext( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnRestore( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnRestoreAndNext( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnMenuPreferences ( IDispatch *, VARIANT_BOOL *  ) ;
	void __stdcall OnGet( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnSet( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnGetAndNext( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnSetAndNext( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnLookup( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnLookupNext( IDispatch *, VARIANT_BOOL * );

	void __stdcall OnGlossN( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnButtonGlossN( IDispatch *Ctrl, VARIANT_BOOL *CancelDefault );

	void __stdcall OnSave( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnHelp( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnMenuSwitchMode( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnMenuAnalyze( IDispatch *, VARIANT_BOOL * );

	// ===================
	// Word events
	// ===================

	void __stdcall OnWindowActivate( IDispatch *, IDispatch * );

	void __stdcall OnWindowDeactivate( IDispatch *, IDispatch * );
	void __stdcall OnDocumentChange();
	void __stdcall OnDocumentOpen( IDispatch * );
	void __stdcall OnNewDocument( IDispatch * );
	void __stdcall OnWindowSelectionChange( IDispatch * );

	void __stdcall OnBeforeDocumentClose(IDispatch* doc, VARIANT_BOOL *);

	BEGIN_SINK_MAP(CConnect)
		// Word events
		SINK_ENTRY_INFO(WORD_DOCUMENT_CHANGE,     __uuidof(MSWord::ApplicationEvents2),/*dispid*/ 0x00000003, OnDocumentChange, &DocumentChangeInfo)
		SINK_ENTRY_INFO(WORD_DOCUMENT_OPEN,       __uuidof(MSWord::ApplicationEvents2),/*dispid*/ 0x00000004, OnDocumentOpen, &DocumentOpenInfo)
		SINK_ENTRY_INFO(WORD_NEW_DOCUMENT,        __uuidof(MSWord::ApplicationEvents2),/*dispid*/ 0x00000009, OnNewDocument, &NewDocumentInfo)
		SINK_ENTRY_INFO(WORD_ACTIVATE_DOCUMENT,   __uuidof(MSWord::ApplicationEvents2),/*dispid*/ 0x0000000A, OnWindowActivate, &WindowActivateInfo)
		SINK_ENTRY_INFO(WORD_DEACTIVATE_DOCUMENT, __uuidof(MSWord::ApplicationEvents2),/*dispid*/ 0x0000000B, OnWindowDeactivate, &WindowDeactivateInfo)
		SINK_ENTRY_INFO(WORD_WINDOW_SEL_CHANGE,   __uuidof(MSWord::ApplicationEvents2),/*dispid*/ 0x0000000C, OnWindowSelectionChange, &WindowSelectionChangeInfo)
		SINK_ENTRY_INFO(WORD_BEFORE_DOC_CLOSE,   __uuidof(MSWord::ApplicationEvents2),/*dispid*/ 0x00000006, OnBeforeDocumentClose, &OnBeforeCloseInfo)

		// button events
		SINK_BUTTON( BUTTON_ID_GET,				OnGet,			&OnClickButtonInfo )
		SINK_BUTTON( BUTTON_ID_SET,				OnSet,			&OnClickButtonInfo )
		SINK_BUTTON( BUTTON_ID_GET_AND_NEXT,	OnGetAndNext,	&OnClickButtonInfo )
		SINK_BUTTON( BUTTON_ID_SET_AND_NEXT,	OnSetAndNext,	&OnClickButtonInfo )
		SINK_BUTTON( BUTTON_ID_LOOKUP,			OnLookup,		&OnClickButtonInfo )
		SINK_BUTTON( BUTTON_ID_LOOKUP_NEXT,		OnLookupNext,	&OnClickButtonInfo )
		SINK_BUTTON( BUTTON_ID_GLOSS_N,			OnButtonGlossN,	&OnClickButtonInfo )
		SINK_BUTTON( BUTTON_ID_HELP,		    OnHelp,			&OnClickButtonInfo )

		// menu events
		SINK_BUTTON( MENU_ID_GET,				OnGet,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_SET,				OnSet,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_GET_AND_NEXT,		OnGetAndNext,	&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_SET_AND_NEXT,		OnSetAndNext,	&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_LOOKUP,			OnLookup,		&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_LOOKUP_NEXT,		OnLookupNext,	&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_GLOSS_N,			OnGlossN,		&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_SAVE,				OnSave,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_HELP,				OnHelp,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_AUTO_TRANS_SEL,	OnAutoTrans ,	&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_AUTO_TRANS_FUZZY,	OnAutoTransFuzzy ,	&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_CONCORDANCE,		OnConcordance ,		&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_EXTEND_LOOKUP,		OnExtendLookup ,	&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_REGISTER_GLOSS,	OnRegisterGloss ,	&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_DELETE,			OnDelete ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_NEXT,				OnNext ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_PREV,				OnPrev ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_0,			OnEntry0 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_1,			OnEntry1 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_2,			OnEntry2 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_3,			OnEntry3 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_4,			OnEntry4 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_5,			OnEntry5 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_6,			OnEntry6 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_7,			OnEntry7 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_8,			OnEntry8 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_ENTRY_9,			OnEntry9 ,			&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_LOOKUP_TRANS,		OnLookupTrans ,		&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_LOOKUP_NEXT_TRANS,	OnLookupNextTrans ,	&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_TRANS_CONCORDANCE,	OnTransConcordance,		&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_CORRECT_TRANS,		OnCorrect,				&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_EXTEND_LOOKUP_TRANS,	OnExtendTransLookup ,	&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_MENU_PREFERENCES,		OnMenuPreferences,		&OnClickButtonInfo )
		SINK_BUTTON( MENU_ID_SWITCH,				OnMenuSwitchMode,		&OnClickButtonInfo )

		SINK_BUTTON( MENU_ID_ANALYZE,				OnMenuAnalyze,		&OnClickButtonInfo )

	END_SINK_MAP()


	HRESULT add_toolbar( office_cmd_bars &spCmdBars );

	HRESULT add_menu( office_cmd_bars &spCmdBars );
	HRESULT add_menu_review(office_cmd_bars &spCmdBars);
	HRESULT add_classic_menu( office_cmd_bars &spCmdBars ) ;

	command_button_ptr add_toolbar_item( office_cmd_bar_ctls &controls, int button_id, int string_id );
	command_button_ptr add_menu_item( office_cmd_bar_ctls &controls, int button_id, int string_id) ;

	void unadvise_button_items();
	void unadvise_document_events();
	void advise_document_events();
	void switch_to_review_menu();
	void switch_to_translation_menu();
	void switch_to_new_menu();
	void switch_to_classic_menu();
	void switch_to_classic_toolbar();
	bool set_button_image( command_button_ptr &button, const int image_id );

	void switch_to_review_toolbar( );
	void switch_to_translation_toolbar( );

	void set_tag_text( command_button_ptr &button, const _bstr_t &tag_text );
	bool load_picture( command_button_ptr &button, int button_id );
	void gui_to_language( int string_offset );
	void set_command_bar_text( command_button_ptr &button, int string_id );
	void set_menu_text(  command_button_ptr &button, int string_id  );
	void set_button_text( command_button_ptr &button, int string_id );

	HRESULT unadvise_menu_items();
	HRESULT advise_menu_items();
	void persist_app_state();
	void kill_old_menu( office_cmd_bar_ctls controls );
	void gui_to_japanese();
	void gui_to_english();

	void setup_stuff() ;

	// Get a keyboard listener of the desired type
	CKeyboardListener* get_listener(int listener_type);

	bool is_2007_version()
	{
		return CStringW(static_cast< LPCWSTR >( this->m_app->Version )) == L"12.0";
	}
	void on_toggle_shortcuts(BOOL shortcuts_enabled) ;
private:

	void setUpController();

	void setUpCommandBars();

	void load_button_controls();
	void load_toolbar( office_cmd_bars spCmdBars );
	void addCorrectMenu(office_cmd_bars& spCmdBars);

	/*! 
	* Reflect the classic/new menu type selected in preferences.
	*/
	void reflectClassicPref(BOOL old_classic_if);

	/*! 
	* Reflect the GUI language (English or Japanese) selected in preferences.
	*/
	void reflectPreferredLang(int old_preferred_gui);
	/*! 
	* Reflect the segmentation type (Word or HTML document) selected in preferences.
	*/
	void reflectSegType(int old_segmentation_type);

	void handle_exception(std::exception &e, tstring _msg)
	{
		logging::log_error(string("std::exception: ") + string2string(_msg)) ;
		logging::log_error(e.what()) ;
		ATL::CString msg ; 
		msg << _T("Error in Felix Interface\rRuntime Exception\r\r") 
			<< ATL::CString(_msg.c_str()) << _T("\r") 
			<< ATL::CString( e.what() ) ; 
		MessageBox( NULL, msg, _T("Felix Runtime Error"), MB_OK ) ;
	}
	void handle_exception(except::CComException &e, tstring _msg)
	{
		logging::log_error(string("COM Exception: ") + string2string(_msg)) ;
		logging::log_exception(e) ;
		ATL::CString msg ; 
		msg << _T("Error in Felix Interface\rCOM Exception\r\r") 
			<< ATL::CString(_msg.c_str()) ; 
		e.notify_user( msg ) ;
	}
	void handle_exception(except::CWinException &e, tstring _msg)
	{
		logging::log_error(string("Windows Error: ") + string2string(_msg)) ;
		logging::log_exception(e) ;
		ATL::CString msg ; 
		msg << _T("Error in Felix Interface\rWindows Error\r\r")
			<< ATL::CString(_msg.c_str()) ; 
		e.notify_user( msg ) ;
	}
	void handle_exception(except::CSWException &e, tstring _msg)
	{
		logging::log_error(string("Structured Windows Exception: ") + string2string(_msg)) ;
		logging::log_exception(e) ;
		ATL::CString language = L"English" ;
		if(m_properties.get_preferred_gui_lang() == PREF_LANG_JAPANESE)
		{
			language = L"Japanese" ;
		}
		logging::send_report(language, e.get_filename()) ;
	}
	void handle_base_exception(except::CException &e, tstring _msg)
	{
		logging::log_error(string("Error: ") + string2string(_msg)) ;
		logging::log_exception(e) ;
		ATL::CString msg ; 
		msg << _T("Error in Felix Interface\rProgram Error\r\r") 
			<< ATL::CString(_msg.c_str()) ; 
		e.notify_user( msg )  ; 
	}
	void handle_exception(_com_error &ce, tstring _msg)
	{
		logging::log_error(string("COM error: ") + string2string(_msg)) ;
		ATL::CString msg ; 
		msg << _T("Error in Felix Interface\rAutomation Exception\r\r") 
			<< ATL::CString(_msg.c_str()) ; 
		except::CComException e(ce) ; 
		logging::log_exception(e) ;
		e.notify_user( msg )  ;
	}
	void handle_exception(ATL::CAtlException &ae, tstring _msg)
	{
		logging::log_error(string("ATL COM error: ") + string2string(_msg)) ;
		ATL::CString msg ;
		msg << _T("Error in Felix Interface\rCOM Library Exception\r\r") 
			<< ATL::CString(_msg.c_str()) ; 
		except::CComException e( _T("Felix Internal Error"), ae ) ;
		logging::log_exception(e) ;
		e.notify_user( msg ) ;
	}

	void load_keyboard_shortcuts();
};

OBJECT_ENTRY_AUTO(__uuidof(Connect), CConnect)
