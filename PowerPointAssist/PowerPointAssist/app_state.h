// Stores Word preferences in the registry
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
	PREF_LANG_ENGLISH = IDC_LANG_ENG,
	PREF_LANG_JAPANESE = IDC_LANG_JAPANESE,
};
enum
{
	NAV_TYPE_NEXT_SLIDE = IDC_NAV_SLIDE,
	NAV_TYPE_SLIDE_TOP = IDC_NAV_TOP,
	NAV_TYPE_ASK = IDC_NAV_ASK,
};

/*!
 * Holds app preferences (stored in registry)
 * 
 * \todo Split this class into subclasses based on properties pages:
 *	- General
 *	- Formatting
 *	- Segmentation
*/ 
const int SEG_CHAR_SIZE = 265 ;

class app_state : public props::CRegMap
{
public:

	struct app_data
	{
		BOOL	m_toolbar_visible ;

		int		m_preferred_gui_lang ;

		BOOL	m_font_color ;
		BOOL	m_font_bold ;
		BOOL	m_font_italic ;
		BOOL	m_font_underline ;

		int		m_navigation_type ;
		TCHAR	m_segChars[SEG_CHAR_SIZE] ;
		BOOL	m_skipNumbers ;
		int		m_skipJ ;
		BOOL	m_select_spaces ;

		BOOL	m_freshInstall ; // is this useful?
		BOOL	m_shortcuts_active ;
		BOOL	m_raise_felix ;

		app_data();
		void set_segchars_to_defaults();
	};

	app_data m_data ;

	BOOL	get_toolbar_visible( ) { return m_data.m_toolbar_visible  ; }
	int		get_preferred_gui_lang( ) { return m_data.m_preferred_gui_lang ; }
	BOOL	get_font_color( ) { return m_data.m_font_color ; }
	BOOL	get_font_bold( ) { return m_data.m_font_bold  ; }
	BOOL	get_font_italic( ) { return m_data.m_font_italic  ; }
	BOOL	get_font_underline( ) { return m_data.m_font_underline  ; }
	BOOL	get_select_spaces( ) { return m_data.m_select_spaces  ; }
	int		get_navigation_type() { return m_data.m_navigation_type ; }
	BOOL	get_shortcuts_active() { return m_data.m_shortcuts_active ; }

	void set_toolbar_visible(BOOL setting) { m_data.m_toolbar_visible  = setting ; }
	void set_preferred_gui_lang(int setting) { m_data.m_preferred_gui_lang = setting ; }
	void set_font_color(BOOL setting) { m_data.m_font_color = setting ; }
	void set_font_bold(BOOL setting) { m_data.m_font_bold  = setting ; }
	void set_font_italic(BOOL setting) { m_data.m_font_italic  = setting ; }
	void set_font_underline(BOOL setting) { m_data.m_font_underline  = setting ; }
	void set_select_spaces(BOOL setting) { m_data.m_select_spaces = setting ; }
	void set_navigation_type(int setting) { m_data.m_navigation_type = setting ; }
	void set_shortcuts_active(BOOL setting ) { m_data.m_shortcuts_active = setting ; }

	void setDefaults();
	app_state() ;
	app_state( const app_state &rhs ) ;
	app_state &operator=( const app_state &rhs ) ;

	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, _T("Software\\AssistantSuite\\FelixWordInterface"), _T("PROPERTIES") )
		
		REG_ENTRY_BOOL( _T("TOOLBAR_VISIBLE"), m_data.m_toolbar_visible )
		
		REG_ENTRY_INT( _T("PREFERRED_GUI_LANG"), m_data.m_preferred_gui_lang )

		REG_ENTRY_BOOL( _T("FORMAT_COLOR"), m_data.m_font_color )
		REG_ENTRY_BOOL( _T("FORMAT_BOLD"), m_data.m_font_bold )
		REG_ENTRY_BOOL( _T("FORMAT_ITALIC"), m_data.m_font_italic )
		REG_ENTRY_BOOL( _T("FORMAT_UNDERLINE"), m_data.m_font_underline )

		REG_ENTRY_INT( _T("NAVIGATION_RULE"), m_data.m_navigation_type )

		REG_ENTRY_INT( _T("SEG_SKIP_DBYTE"), m_data.m_skipJ )
		REG_ENTRY_BOOL( _T("SEG_SKIP_NUMBERS"), m_data.m_skipNumbers )
		REG_ENTRY_STRING( _T("SEGMENT_END_CHARACTERS"),	m_data.m_segChars, SEG_CHAR_SIZE )

		REG_ENTRY_BOOL(_T("SELECT_SPACES"), m_data.m_select_spaces)

		REG_ENTRY_BOOL( _T("FRESH_INSTALL"), m_data.m_freshInstall )
		REG_ENTRY_BOOL(_T("SHORTCUTS_ACTIVE"), m_data.m_shortcuts_active)
		REG_ENTRY_BOOL( _T("RAISE_FELIX"),  m_data.m_raise_felix )

	END_REGISTRY_MAP

} ;

