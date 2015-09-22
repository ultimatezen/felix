/*!
 * definitions for event sink
 * 
 * Copyright (c) 2005 by Ryan Ginstrom
 */
#pragma once

#include "ExcelImport.h"

// Event information for Excel events
struct EventInfo 
{
	DWORD dwCookie;
	IConnectionPoint *pCP;
};


extern _ATL_FUNC_INFO OnClickButtonInfo;

class CConnect ;

typedef MSOffice::_CommandBarButtonEvents button_events ;

#define SINK_BUTTON( id, fnc ) SINK_ENTRY_INFO( id, __uuidof(button_events), /*dispid*/ 0x00000001, fnc, &OnClickButtonInfo )


/*!
 * enums for our various sunk events.
 */
enum 
{
	// toolbar buttoms
	BUTTON_ID_GET = 1, 
	BUTTON_ID_SET, 
	BUTTON_ID_GET_AND_NEXT, 
	BUTTON_ID_SET_AND_NEXT, 
	BUTTON_ID_LOOKUP, 
	BUTTON_ID_LOOKUP_NEXT, 
	BUTTON_ID_SWITCH,
	BUTTON_ID_HELP,
	// Menu items
	EVENT_ADD_GLOSS,
	EVENT_ADD_MEM,
	EVENT_LOOKUP,
	EVENT_LOOKUP_NEXT,
	EVENT_SET,
	EVENT_SET_AND_NEXT,
	EVENT_GET,
	EVENT_GET_AND_NEXT,
	EVENT_SWITCH_LANG,
	EVENT_AUTO_TRANS_SEL,
	EVENT_AUTO_TRANS_SHEET,
	EVENT_AUTO_TRANS_WORKBOOK,

	EVENT_GLOSS_1,
	EVENT_GLOSS_2,
	EVENT_GLOSS_3,
	EVENT_GLOSS_4,
	EVENT_GLOSS_5,
	EVENT_GLOSS_6,
	EVENT_GLOSS_7,
	EVENT_GLOSS_8,
	EVENT_GLOSS_9,
	EVENT_GLOSS_10,

	EVENT_NEXT_TRANS,
	EVENT_PREV_TRANS,
	EVENT_PREFERENCES,

	// excel events
	EXCEL_ACTIVATE_SHEET,
	EXCEL_DEACTIVATE_SHEET,
	EXCEL_WORKBOOK_BEFORE_CLOSE,
};

typedef IDispEventSimpleImpl<BUTTON_ID_GET,			CConnect, &__uuidof(button_events) >  ButtonGetEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_SET,			CConnect, &__uuidof(button_events) >  ButtonSetEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_GET_AND_NEXT,CConnect, &__uuidof(button_events) >  ButtonGetAndNextEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_SET_AND_NEXT,CConnect, &__uuidof(button_events) >  ButtonSetAndNextEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_LOOKUP,		CConnect, &__uuidof(button_events) >  ButtonLookupEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_LOOKUP_NEXT,	CConnect, &__uuidof(button_events) >  ButtonLookupNextEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_SWITCH,		CConnect, &__uuidof(button_events) >  ButtonSwitchEventImpl ;
typedef IDispEventSimpleImpl<BUTTON_ID_HELP,		CConnect, &__uuidof(button_events) >  ButtonHelpEventImpl ;



typedef IDispEventSimpleImpl<EVENT_ADD_GLOSS,		CConnect, &__uuidof(button_events) >  MenuAutoAddGlossEventImpl ;
typedef IDispEventSimpleImpl<EVENT_ADD_MEM,			CConnect, &__uuidof(button_events) >  MenuAutoAddMemEventImpl ;
typedef IDispEventSimpleImpl<EVENT_LOOKUP,			CConnect, &__uuidof(button_events) >  MenuLookupEventImpl ;
typedef IDispEventSimpleImpl<EVENT_LOOKUP_NEXT,		CConnect, &__uuidof(button_events) >  MenuLookupNextEventImpl ;
typedef IDispEventSimpleImpl<EVENT_SET,				CConnect, &__uuidof(button_events) >  MenuSetEventImpl ;
typedef IDispEventSimpleImpl<EVENT_SET_AND_NEXT,	CConnect, &__uuidof(button_events) >  MenuSetAndNextEventImpl ;
typedef IDispEventSimpleImpl<EVENT_GET,				CConnect, &__uuidof(button_events) >  MenuGetEventImpl ;
typedef IDispEventSimpleImpl<EVENT_GET_AND_NEXT,	CConnect, &__uuidof(button_events) >  MenuGetAndNextEventImpl ;
typedef IDispEventSimpleImpl<EVENT_SWITCH_LANG,		CConnect, &__uuidof(button_events) >  MenuSwitchLangEventImpl ;
typedef IDispEventSimpleImpl<EVENT_AUTO_TRANS_SEL,	CConnect, &__uuidof(button_events) >  MenuAutoTransSelEventImpl ;
typedef IDispEventSimpleImpl<EVENT_PREFERENCES,		CConnect, &__uuidof(button_events) >  MenuPreferencesEventImpl ;

typedef IDispEventSimpleImpl<EVENT_AUTO_TRANS_SHEET,	CConnect, &__uuidof(button_events) >  MenuAutoTransSheetEventImpl ;
typedef IDispEventSimpleImpl<EVENT_AUTO_TRANS_WORKBOOK,	CConnect, &__uuidof(button_events) >  MenuAutoTransWorkbookEventImpl ;

typedef IDispEventSimpleImpl<EVENT_GLOSS_1,	CConnect, &__uuidof(button_events) >  MenuGloss1 ;
typedef IDispEventSimpleImpl<EVENT_GLOSS_2,	CConnect, &__uuidof(button_events) >  MenuGloss2 ;
typedef IDispEventSimpleImpl<EVENT_GLOSS_3,	CConnect, &__uuidof(button_events) >  MenuGloss3 ;
typedef IDispEventSimpleImpl<EVENT_GLOSS_4,	CConnect, &__uuidof(button_events) >  MenuGloss4 ;
typedef IDispEventSimpleImpl<EVENT_GLOSS_5,	CConnect, &__uuidof(button_events) >  MenuGloss5 ;
typedef IDispEventSimpleImpl<EVENT_GLOSS_6,	CConnect, &__uuidof(button_events) >  MenuGloss6 ;
typedef IDispEventSimpleImpl<EVENT_GLOSS_7,	CConnect, &__uuidof(button_events) >  MenuGloss7 ;
typedef IDispEventSimpleImpl<EVENT_GLOSS_8,	CConnect, &__uuidof(button_events) >  MenuGloss8 ;
typedef IDispEventSimpleImpl<EVENT_GLOSS_9,	CConnect, &__uuidof(button_events) >  MenuGloss9 ;
typedef IDispEventSimpleImpl<EVENT_GLOSS_10, CConnect, &__uuidof(button_events) >  MenuGloss10 ;

typedef IDispEventSimpleImpl<EVENT_NEXT_TRANS,	CConnect, &__uuidof(button_events) >  MenuNextTrans ;
typedef IDispEventSimpleImpl<EVENT_PREV_TRANS,	CConnect, &__uuidof(button_events) >  MenuPrevTrans ;
