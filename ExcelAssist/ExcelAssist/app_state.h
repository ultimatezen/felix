// Stores Excel preferences in the registry
#pragma once

#include "RegistryStuff.h"
#include "resource.h"


enum
{
	SKIP_IF_J = IDC_SKIP_HASJ,
	SKIP_UNLESS_J = IDC_SKIP_NOJ,
	NO_SKIP = IDC_NOSKIP,
};
enum
{
	PREF_LANG_ENGLISH,
	PREF_LANG_JAPANESE,
};

/*!
* Holds app preferences (stored in registry)
* 
* \todo Split this class into subclasses based on properties pages:
*	- General
*	- Formatting
*	- Segmentation
*/ 
#define SEG_CHAR_SIZE 265
#define MANUAL_URL_SIZE 512

class app_state : public props::CRegMap
{
public:

	BOOL	m_toolbar_visible ;

	int		m_preferred_gui_lang ;

	BOOL	m_skipNumbers ;
	int		m_skipJ ;

	BOOL	m_use_trans_hist ;

	BOOL	m_freshInstall ; // is this useful?
	BOOL	m_shortcuts_active ;
	BOOL	m_raise_felix ;

	BOOL get_toolbar_visible( ) { return m_toolbar_visible  ; }
	int get_preferred_gui_lang( ) { return m_preferred_gui_lang ; }
	int get_segmentation_type() { return m_skipJ ; }
	BOOL	get_shortcuts_active() { return m_shortcuts_active ; }

	void set_toolbar_visible(BOOL setting ) { m_toolbar_visible  = setting ; }
	void set_preferred_gui_lang(int setting ) { m_preferred_gui_lang = setting ; }
	void set_shortcuts_active(BOOL setting ) { m_shortcuts_active = setting ; }

	void setDefaults();

	void set_default_lang();
	app_state() ;
	app_state( const app_state &rhs ) ;
	app_state &operator=( const app_state &rhs ) ;

	void ensure_valid()
	{
		if (m_skipJ != SKIP_IF_J &&
			m_skipJ != SKIP_UNLESS_J &&
			m_skipJ != NO_SKIP)
		{
			m_skipJ = NO_SKIP ;
		}
	}
	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, _T("Software\\AssistantSuite\\FelixExcelInterface"), _T("PROPERTIES") )

		REG_ENTRY_BOOL( _T("TOOLBAR_VISIBLE"), m_toolbar_visible )

		REG_ENTRY_INT( _T("PREFERRED_GUI_LANG"), m_preferred_gui_lang )

		REG_ENTRY_INT( _T("SEG_SKIP_DBYTE"), m_skipJ )
		REG_ENTRY_BOOL( _T("SEG_SKIP_NUMBERS"), m_skipNumbers )

		REG_ENTRY_BOOL(_T("USE_TRANS_HISTORY"), m_use_trans_hist)

		REG_ENTRY_BOOL( _T("FRESH_INSTALL"),	m_freshInstall )
		REG_ENTRY_BOOL(_T("SHORTCUTS_ACTIVE"), m_shortcuts_active)
		REG_ENTRY_BOOL( _T("RAISE_FELIX"), m_raise_felix )
		ensure_valid() ;

	END_REGISTRY_MAP

} ;

