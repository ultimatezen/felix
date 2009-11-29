#pragma once

#include "PowerPointImports.h"


static const GUID DIID__CommandBarButtonEvents = {0x000C0351,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};
//static const GUID DIID__CommandBarComboBoxEvents = {0x000C0354,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};

struct EventInfo 
{
	DWORD dwCookie;
	IConnectionPoint *pCP;
};


extern _ATL_FUNC_INFO OnClickButtonInfo;

class CConnect ;

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
	MENU_ID_CORRECT_AND_NEXT_TRANS,
	MENU_ID_RESTORE_TRANS,
	MENU_ID_RESTORE_AND_NEXT_TRANS,
	MENU_ID_EXTEND_LOOKUP_TRANS,

	MENU_ID_SAVE,
	MENU_ID_HELP,

	MENU_ID_PREFERENCES,

	MENU_ID_MENU_GUI,
};

typedef Office::_CommandBarButtonEvents button_events ;

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

typedef IDispEventSimpleImpl<MENU_ID_CONCORDANCE,			 CConnect, &__uuidof(button_events) >  MenuConcordanceEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_EXTEND_LOOKUP,			 CConnect, &__uuidof(button_events) >  MenuExtendLookupEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_REGISTER_GLOSS,		 CConnect, &__uuidof(button_events) >  MenuRegisterGlossEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_DELETE,				 CConnect, &__uuidof(button_events) >  MenuDeleteEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_NEXT,					 CConnect, &__uuidof(button_events) >  MenuNextEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_PREV,					 CConnect, &__uuidof(button_events) >  MenuPrevEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_0,				 CConnect, &__uuidof(button_events) >  MenuEntry0EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_1,				 CConnect, &__uuidof(button_events) >  MenuEntry1EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_2,				 CConnect, &__uuidof(button_events) >  MenuEntry2EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_3,				 CConnect, &__uuidof(button_events) >  MenuEntry3EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_4,				 CConnect, &__uuidof(button_events) >  MenuEntry4EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_5,				 CConnect, &__uuidof(button_events) >  MenuEntry5EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_6,				 CConnect, &__uuidof(button_events) >  MenuEntry6EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_7,				 CConnect, &__uuidof(button_events) >  MenuEntry7EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_8,				 CConnect, &__uuidof(button_events) >  MenuEntry8EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_ENTRY_9,				 CConnect, &__uuidof(button_events) >  MenuEntry9EventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_LOOKUP_TRANS,			 CConnect, &__uuidof(button_events) >  MenuLookupTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_LOOKUP_NEXT_TRANS,		 CConnect, &__uuidof(button_events) >  MenuLookupNextTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_TRANS_CONCORDANCE,		 CConnect, &__uuidof(button_events) >  MenuTransConcordanceEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_CORRECT_TRANS,			 CConnect, &__uuidof(button_events) >  MenuCorrectTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_CORRECT_AND_NEXT_TRANS, CConnect, &__uuidof(button_events) >  MenuCorrectAndNextTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_RESTORE_TRANS,			 CConnect, &__uuidof(button_events) >  MenuRestoreTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_RESTORE_AND_NEXT_TRANS, CConnect, &__uuidof(button_events) >  MenuRestoreAndNextTransEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_EXTEND_LOOKUP_TRANS,	 CConnect, &__uuidof(button_events) >  MenuExtendTransLookupEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_MENU_GUI,				 CConnect, &__uuidof(button_events) >  MenuGuiEventImpl ;
typedef IDispEventSimpleImpl<MENU_ID_PREFERENCES,			 CConnect, &__uuidof(button_events) >  MenuPreferencesEventImpl ;
