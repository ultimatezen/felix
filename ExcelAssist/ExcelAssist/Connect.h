/*!
* Connect.h : Declaration of the CConnect
* 
* Copyright (c) 2005 by <your name/ organization here>
*/


#pragma once
#include "resource.h"       // main symbols
#include "ExcelImport.h"
#include "ConnectEvents.h"
#include "FelixExcelIF.h"
#include "Path.h"
#include "Properties.h"
#include "logging.h"

_bstr_t glossCap( _bstr_t txt, int num ) ;
_bstr_t glossDesc( _bstr_t txt, int num ) ;

extern _ATL_FUNC_INFO SheetActivationInfo ;
extern _ATL_FUNC_INFO BeforeCloseInfo ;


typedef IDispEventSimpleImpl<EXCEL_ACTIVATE_SHEET, CConnect, &__uuidof(Excel::AppEvents) >  ExcelSheetActivateImpl ;
typedef IDispEventSimpleImpl<EXCEL_DEACTIVATE_SHEET, CConnect, &__uuidof(Excel::AppEvents) >  ExcelSheetDeactivateImpl ;
typedef IDispEventSimpleImpl<EXCEL_WORKBOOK_BEFORE_CLOSE, CConnect, &__uuidof(Excel::AppEvents) >  ExcelWbBeforeCloseImpl ;


// CConnect
/*!
* \brief
* The COM object that Excel instantiates.
* 
- Registers keyboard hooks
- Creates menus and advises events
- Listens for events from Excel, and delegates them to \ref CFelixExcelIF
* 
* \see
* CFelixExcelIF | CMenuAdder
*/
class ATL_NO_VTABLE CConnect : 
	public CComCoClass<CConnect, &CLSID_Connect>,
	public IDispatchImpl<AddInDesignerObjects::_IDTExtensibility2, &AddInDesignerObjects::IID__IDTExtensibility2, &AddInDesignerObjects::LIBID_AddInDesignerObjects, 1, 0>,
	public CComObjectRootEx<CComSingleThreadModel>,

	public ExcelSheetActivateImpl,
	public ExcelSheetDeactivateImpl,
	public ExcelWbBeforeCloseImpl,

	// button event impl's
	public ButtonGetEventImpl ,       
	public ButtonSetEventImpl ,       
	public ButtonGetAndNextEventImpl ,
	public ButtonSetAndNextEventImpl ,
	public ButtonLookupEventImpl ,
	public ButtonLookupNextEventImpl,
	public ButtonSwitchEventImpl,
	public ButtonHelpEventImpl,
	// menu event impl's

	public MenuGloss1, 
	public MenuGloss2, 
	public MenuGloss3, 
	public MenuGloss4, 
	public MenuGloss5, 
	public MenuGloss6, 
	public MenuGloss7, 
	public MenuGloss8, 
	public MenuGloss9, 
	public MenuGloss10, 

	public MenuNextTrans, 
	public MenuPrevTrans, 

	public MenuAutoAddMemEventImpl,
	public MenuLookupEventImpl,
	public MenuLookupNextEventImpl,
	public MenuSetEventImpl,
	public MenuSetAndNextEventImpl,
	public MenuGetEventImpl,
	public MenuGetAndNextEventImpl,
	public MenuSwitchLangEventImpl,
	public MenuAutoTransSelEventImpl,
	public MenuAutoAddGlossEventImpl,
	public MenuAutoTransSheetEventImpl,
	public MenuAutoTransWorkbookEventImpl,

	public MenuPreferencesEventImpl
{

	MSOffice::CommandBarPtr m_toolbar ;
	MSOffice::CommandBarControlPtr m_menu ;

	// toolbar buttons
	MSOffice::_CommandBarButtonPtr	m_button_get ;
	MSOffice::_CommandBarButtonPtr	m_button_set ;
	MSOffice::_CommandBarButtonPtr	m_button_get_and_next ;
	MSOffice::_CommandBarButtonPtr	m_button_set_and_next ;
	MSOffice::_CommandBarButtonPtr	m_button_lookup ;
	MSOffice::_CommandBarButtonPtr	m_button_lookup_next ;
	MSOffice::_CommandBarButtonPtr	m_button_switch ;
	MSOffice::_CommandBarButtonPtr	m_button_help ;

	// menu items
	MSOffice::_CommandBarButtonPtr m_menuAutoAddGloss ;
	MSOffice::_CommandBarButtonPtr m_menuAutoAddMem ;
	MSOffice::_CommandBarButtonPtr m_menuLookup ;
	MSOffice::_CommandBarButtonPtr m_menuLookupNext ;
	MSOffice::_CommandBarButtonPtr m_menuSet;
	MSOffice::_CommandBarButtonPtr m_menuSetAndNext ;
	MSOffice::_CommandBarButtonPtr m_menuGet ;
	MSOffice::_CommandBarButtonPtr m_menuGetAndNext ;

	MSOffice::CommandBarControlPtr m_menuGloss ;

	MSOffice::_CommandBarButtonPtr m_menuGloss1 ;
	MSOffice::_CommandBarButtonPtr m_menuGloss2 ;
	MSOffice::_CommandBarButtonPtr m_menuGloss3 ;
	MSOffice::_CommandBarButtonPtr m_menuGloss4 ;
	MSOffice::_CommandBarButtonPtr m_menuGloss5 ;
	MSOffice::_CommandBarButtonPtr m_menuGloss6 ;
	MSOffice::_CommandBarButtonPtr m_menuGloss7 ;
	MSOffice::_CommandBarButtonPtr m_menuGloss8 ;
	MSOffice::_CommandBarButtonPtr m_menuGloss9 ;
	MSOffice::_CommandBarButtonPtr m_menuGloss10 ;

	MSOffice::_CommandBarButtonPtr m_menuNextTrans ;
	MSOffice::_CommandBarButtonPtr m_menuPrevTrans ;

	MSOffice::_CommandBarButtonPtr m_menuLookupTrans ;
	MSOffice::_CommandBarButtonPtr m_menuSetTrans ;
	MSOffice::_CommandBarButtonPtr m_menuGetTrans ;

	MSOffice::_CommandBarButtonPtr m_menuSwitchLang ;
	MSOffice::_CommandBarButtonPtr m_menuPreferences ;

	MSOffice::CommandBarControlPtr m_menuAuto ;

	MSOffice::_CommandBarButtonPtr m_menuAutoTransSel ;
	MSOffice::_CommandBarButtonPtr m_menuAutoTransSheet ;
	MSOffice::_CommandBarButtonPtr m_menuAutoTransWorkbook ;

	CFelixExcelIF	m_excelIF ;


	Excel::_ApplicationPtr m_pApplication ;
	CComPtr<IDispatch> m_pAddInInstance;

	app_state m_properties ;

public:

	bool is_2007_version()
	{
		return CStringW(static_cast< LPCWSTR >( this->m_pApplication->Version )) == L"12.0";
	}
	HRESULT add_toolbar( MSOffice::_CommandBarsPtr &spCmdBars );
	void add_toolbar_items( MSOffice::CommandBarControlsPtr spBarControls );	
	MSOffice::_CommandBarButtonPtr add_toolbar_item(MSOffice::CommandBarControlsPtr &controls, int button_id );

	void set_tb_img_and_text( MSOffice::_CommandBarButtonPtr toolbarItem, int button_id );
	MSOffice::_CommandBarButtonPtr addControl(MSOffice::CommandBarControlsPtr& controls);	
	void setToolbarItemText(int button_id, MSOffice::_CommandBarButtonPtr& toolbarItem);
	void pastePicture(int button_id, MSOffice::_CommandBarButtonPtr& toolbarItem);	
	bool load_picture(MSOffice::_CommandBarButtonPtr &button, int button_id);
	void purgeClipboard();

	void putImageInClipboard(HBITMAP hBmp);	
	void on_toggle_shortcuts(BOOL shortcuts_enabled) ;
	wstring get_menu_caption();
	//! Default constructor
	CConnect();
	//! DTOR
	~CConnect()
	{
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

public:
	//IDTExtensibility2 implementation:
	STDMETHOD(OnConnection)(IDispatch * Application, AddInDesignerObjects::ext_ConnectMode ConnectMode, IDispatch *AddInInst, SAFEARRAY **custom);

	STDMETHOD(OnDisconnection)(AddInDesignerObjects::ext_DisconnectMode RemoveMode, SAFEARRAY **custom );
	STDMETHOD(OnAddInsUpdate)(SAFEARRAY **custom );
	STDMETHOD(OnStartupComplete)(SAFEARRAY **custom );
	STDMETHOD(OnBeginShutdown)(SAFEARRAY **custom );

	void unadvise_menu_methods();
	void unadvise_toolbar_methods();
	void unadvise_app_methods();
	void advise_button_events();
	void __stdcall OnAutoAddGloss ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnAutoAddMem ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnLookup ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnLookupNext ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnSet ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnSetAndNext ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGet ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGetAndNext ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnSwitch ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnAutoTransSel ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnAutoTransSheet ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnAutoTransWorkbook ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnNextTrans ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnPrevTrans ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnGloss1 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGloss2 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGloss3 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGloss4 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGloss5 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGloss6 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGloss7 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGloss8 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGloss9 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnGloss10 ( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnPreferences( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnSwitchModes( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );
	void __stdcall OnHelp		( IDispatch *Ctrl, VARIANT_BOOL * CancelDefault );

	void __stdcall OnSheetActivate(IDispatch *sheet_dispatch)
	{
		ATLTRACE("OnSheetActivate\n") ;
		Excel::_WorksheetPtr sheet(sheet_dispatch) ;
		TRACE(sheet->Name) ;
	}
	void __stdcall OnSheetDeactivate(IDispatch *sheet_dispatch)
	{
		ATLTRACE("OnSheetDeactivate\n") ;
		Excel::_WorksheetPtr sheet(sheet_dispatch) ;
		TRACE(sheet->Name) ;
	}
	void __stdcall OnWorkbookBeforeClose(IDispatch * workbook, VARIANT_BOOL *);

	BEGIN_SINK_MAP( CConnect )
		SINK_ENTRY_INFO(EXCEL_ACTIVATE_SHEET,     __uuidof(Excel::AppEvents),/*dispid*/ 0x00000619, OnSheetActivate,&SheetActivationInfo)
		SINK_ENTRY_INFO(EXCEL_DEACTIVATE_SHEET,   __uuidof(Excel::AppEvents),/*dispid*/ 0x0000061a, OnSheetDeactivate,&SheetActivationInfo)
		SINK_ENTRY_INFO(EXCEL_WORKBOOK_BEFORE_CLOSE,   __uuidof(Excel::AppEvents),/*dispid*/ 0x00000622, OnWorkbookBeforeClose,&BeforeCloseInfo)
		// button events
		SINK_ENTRY_INFO(BUTTON_ID_GET,			__uuidof(button_events), /*dispid*/ 0x01, OnGet,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_SET,			__uuidof(button_events), /*dispid*/ 0x01, OnSet,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_GET_AND_NEXT, __uuidof(button_events), /*dispid*/ 0x01, OnGetAndNext, &OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_SET_AND_NEXT, __uuidof(button_events), /*dispid*/ 0x01, OnSetAndNext, &OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_LOOKUP,		__uuidof(button_events), /*dispid*/ 0x01, OnLookup,		&OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_LOOKUP_NEXT,	__uuidof(button_events), /*dispid*/	0x01, OnLookupNext,	&OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_SWITCH,		__uuidof(button_events), /*dispid*/	0x01, OnSwitchModes, &OnClickButtonInfo )
		SINK_ENTRY_INFO(BUTTON_ID_HELP,		    __uuidof(button_events), /*dispid*/ 0x01, OnHelp,		&OnClickButtonInfo )
		// menu events
		SINK_BUTTON(EVENT_ADD_GLOSS,	 	OnAutoAddGloss )
		SINK_BUTTON(EVENT_ADD_MEM,		 	OnAutoAddMem )
		SINK_BUTTON(EVENT_LOOKUP,		 	OnLookup )
		SINK_BUTTON(EVENT_LOOKUP_NEXT,	 	OnLookupNext )
		SINK_BUTTON(EVENT_SET,			 	OnSet )
		SINK_BUTTON(EVENT_SET_AND_NEXT,  	OnSetAndNext )
		SINK_BUTTON(EVENT_GET,			 	OnGet )
		SINK_BUTTON(EVENT_GET_AND_NEXT,  	OnGetAndNext )
		SINK_BUTTON(EVENT_SWITCH_LANG,	 	OnSwitch )
		// auto translate
		SINK_BUTTON(EVENT_AUTO_TRANS_SEL,   OnAutoTransSel )
		SINK_BUTTON(EVENT_AUTO_TRANS_SHEET,   OnAutoTransSheet )
		SINK_BUTTON(EVENT_AUTO_TRANS_WORKBOOK,   OnAutoTransWorkbook )
		SINK_BUTTON(EVENT_PREFERENCES,   OnPreferences )

		// glossary entries
		SINK_BUTTON(EVENT_GLOSS_1,   OnGloss1 )
		SINK_BUTTON(EVENT_GLOSS_2,   OnGloss2 )
		SINK_BUTTON(EVENT_GLOSS_3,   OnGloss3 )
		SINK_BUTTON(EVENT_GLOSS_4,   OnGloss4 )
		SINK_BUTTON(EVENT_GLOSS_5,   OnGloss5 )
		SINK_BUTTON(EVENT_GLOSS_6,   OnGloss6 )
		SINK_BUTTON(EVENT_GLOSS_7,   OnGloss7 )
		SINK_BUTTON(EVENT_GLOSS_8,   OnGloss8 )
		SINK_BUTTON(EVENT_GLOSS_9,   OnGloss9 )
		SINK_BUTTON(EVENT_GLOSS_10,   OnGloss10 )

		// next/prev
		SINK_BUTTON(EVENT_NEXT_TRANS,   OnNextTrans )
		SINK_BUTTON(EVENT_PREV_TRANS,   OnPrevTrans )

	END_SINK_MAP()

public:
	void checkHR(HRESULT hr);
	void switchGuiLang(void);
	void killOldMenu(void);
	void setMenuStrings(int lang_id);
	void createMenu() ;

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
	void handle_exception(CComException &e, tstring _msg)
	{
		logging::log_error(string("COM Exception: ") + string2string(_msg)) ;
		logging::log_exception(e) ;
		CString msg ; 
		msg << _T("Error in Felix Interface\rCOM Exception\r\r") 
			<< CString(_msg.c_str()) ; 
		e.notify_user( msg ) ;
	}
	void handle_exception(CWinException &e, tstring _msg)
	{
		logging::log_error(string("Windows Error: ") + string2string(_msg)) ;
		logging::log_exception(e) ;
		CString msg ; 
		msg << _T("Error in Felix Interface\rWindows Error\r\r")
			<< CString(_msg.c_str()) ; 
		e.notify_user( msg ) ;
	}
	void handle_exception(CSWException &e, tstring _msg)
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
	void handle_base_exception(CException &e, tstring _msg)
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
		CComException e(ce) ; 
		logging::log_exception(e) ;
		e.notify_user( msg )  ;
	}
	void handle_exception(CAtlException &ae, tstring _msg)
	{
		logging::log_error(string("ATL COM error: ") + string2string(_msg)) ;
		CString msg ;
		msg << _T("Error in Felix Interface\rCOM Library Exception\r\r") 
			<< CString(_msg.c_str()) ; 
		CComException e( _T("Felix Internal Error"), ae ) ;
		logging::log_exception(e) ;
		e.notify_user( msg ) ;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(Connect), CConnect)

