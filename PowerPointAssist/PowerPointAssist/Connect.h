// Connect.h : Declaration of the CConnect
// PowerPointAssist

#pragma once
#include "resource.h"       // main symbols
#include "PowerPointImports.h"
#include "PowerPointEventDefs.h"
#include "PowerPointInterface.h"

#include "app_state.h"

#include "AddIn.h"
#include "logging.h"
#include "keyboard_shortcuts.h"
#include "keyboard_shortcuts_ppt.h"
#include "key_mapper_ppt.h"

// CConnect
class ATL_NO_VTABLE CConnect : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CConnect, &CLSID_Connect>,
	public IDispatchImpl<AddInDesignerObjects::_IDTExtensibility2, &AddInDesignerObjects::IID__IDTExtensibility2, &AddInDesignerObjects::LIBID_AddInDesignerObjects, 1, 0>,
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
	//public MenuAlignEventImpl,
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
	public MenuCorrectAndNextTransEventImpl,
	public MenuRestoreTransEventImpl,
	public MenuRestoreAndNextTransEventImpl,
	public MenuExtendTransLookupEventImpl,
	public MenuGuiEventImpl,
	public MenuPreferencesEventImpl,
	public MenuSwitchModeEventImpl
{

	Office::CommandBarControlPtr m_gloss_menu_popup ;
	CPowerPointInterface m_interface ;

	Office::CommandBarPtr m_toolbar ;

	// toolbar buttons
	Office::_CommandBarButtonPtr	m_button_get ;
	Office::_CommandBarButtonPtr	m_button_set ;
	Office::_CommandBarButtonPtr	m_button_get_and_next ;
	Office::_CommandBarButtonPtr	m_button_set_and_next ;
	Office::_CommandBarButtonPtr	m_button_lookup ;
	Office::_CommandBarButtonPtr	m_button_lookup_next ;
	Office::_CommandBarButtonPtr	m_button_gloss_n ;
	Office::_CommandBarButtonPtr	m_button_help ;
	// menu items
	Office::_CommandBarButtonPtr	m_menu_get ;
	Office::_CommandBarButtonPtr	m_menu_set ;
	Office::_CommandBarButtonPtr	m_menu_get_and_next ;
	Office::_CommandBarButtonPtr	m_menu_set_and_next ;
	Office::_CommandBarButtonPtr	m_menu_lookup ;
	Office::_CommandBarButtonPtr	m_menu_lookup_next ;
	Office::_CommandBarButtonPtr	m_menu_gloss_n ;
	Office::_CommandBarButtonPtr	m_menu_save ;
	//Office::_CommandBarButtonPtr	m_menu_align ;
	Office::_CommandBarButtonPtr	m_menu_help ;
	Office::_CommandBarButtonPtr	m_menu_preferences ;
	Office::_CommandBarButtonPtr	m_menu_switch ;

	Office::_CommandBarButtonPtr	m_menu_auto_trans ;
	Office::_CommandBarButtonPtr	m_menu_auto_trans_fuzzy ;
	Office::_CommandBarButtonPtr	m_menu_concordance ;
	Office::_CommandBarButtonPtr	m_menu_extend_lookup ;
	Office::_CommandBarButtonPtr	m_menu_register_gloss ;
	Office::_CommandBarButtonPtr	m_menu_delete ;
	Office::_CommandBarButtonPtr	m_menu_next ;
	Office::_CommandBarButtonPtr	m_menu_prev ;
	Office::_CommandBarButtonPtr	m_menu_entry_0 ;
	Office::_CommandBarButtonPtr	m_menu_entry_1 ;
	Office::_CommandBarButtonPtr	m_menu_entry_2 ;
	Office::_CommandBarButtonPtr	m_menu_entry_3 ;
	Office::_CommandBarButtonPtr	m_menu_entry_4 ;
	Office::_CommandBarButtonPtr	m_menu_entry_5 ;
	Office::_CommandBarButtonPtr	m_menu_entry_6 ;
	Office::_CommandBarButtonPtr	m_menu_entry_7 ;
	Office::_CommandBarButtonPtr	m_menu_entry_8 ;
	Office::_CommandBarButtonPtr	m_menu_entry_9 ;
	Office::_CommandBarButtonPtr	m_menu_lookup_trans ;
	Office::_CommandBarButtonPtr	m_menu_lookup_next_trans ;
	Office::_CommandBarButtonPtr	m_menu_trans_concordance ;
	Office::_CommandBarButtonPtr	m_menu_correct_trans ;
	Office::_CommandBarButtonPtr	m_menu_correct_and_next_trans ;
	Office::_CommandBarButtonPtr	m_menu_restore_trans ;
	Office::_CommandBarButtonPtr	m_menu_restore_and_next_trans ;
	Office::_CommandBarButtonPtr	m_menu_extend_trans_lookup ;
	Office::_CommandBarButtonPtr	m_menu_gui ;

	Office::CommandBarControlPtr m_glossMenuPopup ;

	PowerPoint::_ApplicationPtr m_pApplication;
	CComPtr<IDispatch>			m_pAddInInstance;
	Office::CommandBarControlPtr m_felix_menu ;
	app_state m_properties ;

	KeyMapperPpt m_mapper ;
	shortcuts::KeyboardShortcuts m_keyboard_shortcuts ;

public:
	HMODULE scidll_;

	CConnect() : 
		scidll_(NULL),
		m_keyboard_shortcuts(&m_mapper)
	{
		scidll_ = ::LoadLibrary( _T("SciLexer.dll") ) ;
	}
	~CConnect()
	{
		if ( scidll_ )
		{
			::FreeLibrary( scidll_ ) ;
		}
	}

	bool is_2007_version()
	{
		return CStringW(static_cast< LPCWSTR >( this->m_pApplication->Version )) == L"12.0";
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ADDIN)
DECLARE_NOT_AGGREGATABLE(CConnect)

BEGIN_COM_MAP(CConnect)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(AddInDesignerObjects::IDTExtensibility2)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

	// =======================
	// _IDTExtensibility2
	// =======================

	void init_toolbar( Office::_CommandBarsPtr spCmdBars );

	void init_properties();

	STDMETHOD(OnConnection)(IDispatch * Application, AddInDesignerObjects::ext_ConnectMode ConnectMode, IDispatch *AddInInst, SAFEARRAY **custom);
	STDMETHOD(OnDisconnection)(AddInDesignerObjects::ext_DisconnectMode RemoveMode, SAFEARRAY **custom );
	STDMETHOD(OnAddInsUpdate)(SAFEARRAY **custom );
	STDMETHOD(OnStartupComplete)(SAFEARRAY **custom );
	STDMETHOD(OnBeginShutdown)(SAFEARRAY **custom );

	void on_toggle_shortcuts(BOOL shortcuts_enabled) ;
	wstring get_menu_caption();

	void gui_to_english();
	void gui_to_japanese();

	void __stdcall  OnAutoTrans( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall  OnAutoTransFuzzy( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnConcordance( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnExtendLookup( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnRegisterGloss( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnDelete( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnNext( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnPrev( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry0( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry1( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry2 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry3( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry4 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry5 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry6 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry7( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry8 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnEntry9 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnLookupTrans ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnLookupNextTrans ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnTransConcordance ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnExtendTransLookup ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnCorrectTrans ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnCorrectAndNextTrans ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault ); 
	void __stdcall OnRestoreTrans ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault ); 
	void __stdcall OnRestoreAndNextTrans ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault ); 




	void __stdcall OnMenuGui ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnGet		( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnSet		( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnGetAndNext	( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnSetAndNext	( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnLookup		( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnLookupNext	( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnGlossN		( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnButtonGlossN( IDispatch *Ctrl, VARIANT_BOOL *CancelDefault );

	void __stdcall OnSave		( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnHelp		( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnMenuSwitchMode( IDispatch *, VARIANT_BOOL * );
	void __stdcall OnMenuPreferences ( IDispatch *, VARIANT_BOOL *  );
	BEGIN_SINK_MAP(CConnect)
		// button events
		SINK_ENTRY_INFO(BUTTON_ID_GET,			__uuidof(button_events), /*dispid*/ 0x01, OnGet,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_SET,			__uuidof(button_events), /*dispid*/ 0x01, OnSet,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_GET_AND_NEXT, __uuidof(button_events), /*dispid*/ 0x01, OnGetAndNext, &OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_SET_AND_NEXT, __uuidof(button_events), /*dispid*/ 0x01, OnSetAndNext, &OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_LOOKUP,		__uuidof(button_events), /*dispid*/ 0x01, OnLookup,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_LOOKUP_NEXT,	__uuidof(button_events), /*dispid*/	0x01, OnLookupNext,	&OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_GLOSS_N,		__uuidof(button_events), /*dispid*/ 0x01, OnButtonGlossN,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_HELP,		    __uuidof(button_events), /*dispid*/ 0x01, OnHelp,		&OnClickButtonInfo )
		// menu events
		SINK_ENTRY_INFO(MENU_ID_GET,			__uuidof(button_events), /*dispid*/ 0x01, OnGet,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_SET,			__uuidof(button_events), /*dispid*/ 0x01, OnSet,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_GET_AND_NEXT,	__uuidof(button_events), /*dispid*/ 0x01, OnGetAndNext, &OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_SET_AND_NEXT,	__uuidof(button_events), /*dispid*/ 0x01, OnSetAndNext, &OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_LOOKUP,			__uuidof(button_events), /*dispid*/ 0x01, OnLookup,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_LOOKUP_NEXT,	__uuidof(button_events), /*dispid*/	0x01, OnLookupNext,	&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_GLOSS_N,		__uuidof(button_events), /*dispid*/ 0x01, OnGlossN,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_SAVE,			__uuidof(button_events), /*dispid*/ 0x01, OnSave,		&OnClickButtonInfo )
		//SINK_ENTRY_INFO(MENU_ID_ALIGN,			__uuidof(button_events), /*dispid*/ 0x01, OnAlign,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_HELP,			__uuidof(button_events), /*dispid*/ 0x01, OnHelp,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_AUTO_TRANS_SEL,		__uuidof(button_events), /*dispid*/ 0x01, OnAutoTrans ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_AUTO_TRANS_FUZZY,	__uuidof(button_events), /*dispid*/ 0x01, OnAutoTransFuzzy ,	&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_CONCORDANCE,		__uuidof(button_events), /*dispid*/ 0x01, OnConcordance ,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_EXTEND_LOOKUP,		__uuidof(button_events), /*dispid*/ 0x01, OnExtendLookup ,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_REGISTER_GLOSS,		__uuidof(button_events), /*dispid*/ 0x01, OnRegisterGloss ,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_DELETE,				__uuidof(button_events), /*dispid*/ 0x01, OnDelete ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_NEXT,				__uuidof(button_events), /*dispid*/ 0x01, OnNext ,				&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_PREV,				__uuidof(button_events), /*dispid*/ 0x01, OnPrev ,				&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_0,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry0 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_1,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry1 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_2,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry2 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_3,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry3 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_4,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry4 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_5,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry5 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_6,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry6 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_7,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry7 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_8,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry8 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_ENTRY_9,			__uuidof(button_events), /*dispid*/ 0x01, OnEntry9 ,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_LOOKUP_TRANS,			__uuidof(button_events), /*dispid*/ 0x01, OnLookupTrans ,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_LOOKUP_NEXT_TRANS,		__uuidof(button_events), /*dispid*/ 0x01, OnLookupNextTrans ,	&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_TRANS_CONCORDANCE,		__uuidof(button_events), /*dispid*/ 0x01, OnTransConcordance, 	&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_CORRECT_TRANS,			__uuidof(button_events), /*dispid*/ 0x01, OnCorrectTrans ,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_CORRECT_AND_NEXT_TRANS,			__uuidof(button_events), /*dispid*/ 0x01, OnCorrectAndNextTrans ,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_RESTORE_TRANS,			__uuidof(button_events),         /*dispid*/ 0x01, OnRestoreTrans ,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_RESTORE_AND_NEXT_TRANS,			__uuidof(button_events), /*dispid*/ 0x01, OnRestoreAndNextTrans ,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_EXTEND_LOOKUP_TRANS,	__uuidof(button_events), /*dispid*/ 0x01, OnExtendTransLookup ,	&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_MENU_GUI,			__uuidof(button_events),     /*dispid*/ 0x01, OnMenuGui,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_PREFERENCES,			__uuidof(button_events),     /*dispid*/ 0x01, OnMenuPreferences,			&OnClickButtonInfo )
		SINK_ENTRY_INFO(MENU_ID_SWITCH,			__uuidof(button_events),     /*dispid*/ 0x01, OnMenuSwitchMode,			&OnClickButtonInfo )

	END_SINK_MAP()


	// =======================
	// IAddin
	// =======================


	HRESULT add_menu(    Office::_CommandBarsPtr &spCmdBars );
	Office::_CommandBarButtonPtr add_menu_item(    Office::CommandBarControlsPtr &controls, int button_id, const CComBSTR caption, const CComBSTR tooltip ) ;
	HRESULT add_toolbar( Office::_CommandBarsPtr &spCmdBars );
	Office::_CommandBarButtonPtr add_toolbar_item( Office::CommandBarControlsPtr &controls, int button_id );

	void killOldMenu(void) ;
	// =======================
	// error handling
	// =======================

	void switch_to_translation_toolbar();
	void switch_to_review_toolbar( );

	void switch_to_translation_menu();
	void switch_to_review_menu( );


	void set_button_image(Office::_CommandBarButtonPtr& button, const int image_id) ;

private:
	void putImageInClipboard(HBITMAP hBmp);

	void purgeClipboard();

	Office::_CommandBarButtonPtr addControl(Office::CommandBarControlsPtr& controls);

	void pastePicture(int button_id, Office::_CommandBarButtonPtr& toolbar_item);

	void set_toolbar_item_text(int button_id, Office::_CommandBarButtonPtr& toolbarItem);

	void add_toolbar_items( Office::CommandBarControlsPtr spBarControls );
	void load_keyboard_shortcuts();
	void handle_exception(std::exception &e, tstring _msg)
	{
		logging::log_error(string("std::exception: ") + string2string(_msg)) ;
		logging::log_error(e.what()) ;
		CString msg ; 
		msg << _T("Error in Felix Interface\rRuntime Exception\r\r") 
			<< CString(_msg.c_str()) << _T("\r") 
			<< CString( e.what() ) ; 
		MessageBox( NULL, msg, _T("Felix Runtime Error"), MB_OK ) ;
	}
	void handle_exception(except::CComException &e, tstring _msg)
	{
		logging::log_error(string("COM Exception: ") + string2string(_msg)) ;
		logging::log_exception(e) ;
		CString msg ; 
		msg << _T("Error in Felix Interface\rCOM Exception\r\r") 
			<< CString(_msg.c_str()) ; 
		e.notify_user( msg ) ;
	}
	void handle_exception(except::CWinException &e, tstring _msg)
	{
		logging::log_error(string("Windows Error: ") + string2string(_msg)) ;
		logging::log_exception(e) ;
		CString msg ; 
		msg << _T("Error in Felix Interface\rWindows Error\r\r")
			<< CString(_msg.c_str()) ; 
		e.notify_user( msg ) ;
	}
	void handle_exception(except::CSWException &e, tstring _msg)
	{
		logging::log_error(string("Structured Windows Exception: ") + string2string(_msg)) ;
		logging::log_exception(e) ;
		CString language = L"English" ;
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
		CString msg ; 
		msg << _T("Error in Felix Interface\rProgram Error\r\r") 
			<< CString(_msg.c_str()) ; 
		e.notify_user( msg )  ; 
	}
	void handle_exception(_com_error &ce, tstring _msg)
	{
		logging::log_error(string("COM error: ") + string2string(_msg)) ;
		CString msg ; 
		msg << _T("Error in Felix Interface\rAutomation Exception\r\r") 
			<< CString(_msg.c_str()) ; 
		except::CComException e(ce) ; 
		logging::log_exception(e) ;
		e.notify_user( msg )  ;
	}
	void handle_exception(CAtlException &ae, tstring _msg)
	{
		logging::log_error(string("ATL COM error: ") + string2string(_msg)) ;
		CString msg ;
		msg << _T("Error in Felix Interface\rCOM Library Exception\r\r") 
			<< CString(_msg.c_str()) ; 
		except::CComException e( _T("Felix Internal Error"), ae ) ;
		logging::log_exception(e) ;
		e.notify_user( msg ) ;
	}

};

OBJECT_ENTRY_AUTO(__uuidof(Connect), CConnect)
