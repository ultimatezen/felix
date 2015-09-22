// Stores Word preferences in the registry
#pragma once

#include "RegistryStuff.h"
#include "resource.h"
#include "abbreviations.h"
enum
{
	SEG_TYPE_WORD_DOC = IDC_RADIO_WORD_DOC,
	SEG_TYPE_HTML_DOC = IDC_RADIO_HTML_DOC,
};
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

	struct app_state_data
	{
		app_state_data();

		BOOL	m_toolbar_visible ;

		int		m_preferred_gui_lang ;
		BOOL	m_classic_if ;

		BOOL	m_font_face ;
		BOOL	m_font_color ;
		BOOL	m_font_bold ;
		BOOL	m_font_italic ;
		BOOL	m_font_underline ;
		BOOL	m_font_super_sub ;

		int		m_segmentation_type ;
		TCHAR	m_segChars[SEG_CHAR_SIZE] ;
		BOOL	m_skipNumbers ;
		int		m_skipJ ;
		BOOL	m_select_spaces ;

		BOOL	m_freshInstall ; // is this useful?
		TCHAR	m_manual_url[MANUAL_URL_SIZE] ;
		BOOL	m_use_trans_hist ;

		BOOL	m_shortcuts_active ;

		BOOL	m_really_wants_no_delims ;

		BOOL	m_raise_felix ;
	};

	app_state_data m_data ;

	BOOL	get_toolbar_visible( ) { return m_data.m_toolbar_visible  ; }
	int		get_preferred_gui_lang( ) { return m_data.m_preferred_gui_lang ; }
	BOOL	get_classic_if( ) { return m_data.m_classic_if  ; }
	BOOL	get_font_face( ) { return m_data.m_font_face ; }
	BOOL	get_font_color( ) { return m_data.m_font_color ; }
	BOOL	get_font_bold( ) { return m_data.m_font_bold  ; }
	BOOL	get_font_italic( ) { return m_data.m_font_italic  ; }
	BOOL	get_font_underline( ) { return m_data.m_font_underline  ; }
	BOOL	get_font_super_sub( ) { return m_data.m_font_super_sub  ; }
	int		get_segmentation_type() { return m_data.m_segmentation_type ; }
	BOOL	get_select_spaces( ) { return m_data.m_select_spaces  ; }
	BOOL	get_shortcuts_active() { return m_data.m_shortcuts_active ; }
	BOOL	get_use_trans_hist() { return m_data.m_use_trans_hist ; }
	BOOL	get_raise_felix() { return m_data.m_raise_felix ; }

	void set_toolbar_visible(BOOL setting ) { m_data.m_toolbar_visible  = setting ; }
	void set_preferred_gui_lang(int setting ) { m_data.m_preferred_gui_lang = setting ; }
	void set_classic_if(BOOL setting ) { m_data.m_classic_if  = setting ; }
	void set_font_face(BOOL setting ) { m_data.m_font_face = setting ; }
	void set_font_color(BOOL setting ) { m_data.m_font_color = setting ; }
	void set_font_bold(BOOL setting ) { m_data.m_font_bold  = setting ; }
	void set_font_italic(BOOL setting ) { m_data.m_font_italic  = setting ; }
	void set_font_underline(BOOL setting ) { m_data.m_font_underline  = setting ; }
	void set_font_super_sub(BOOL setting ) { m_data.m_font_super_sub  = setting ; }
	void set_segmentation_type(int setting ) { m_data.m_segmentation_type = setting ; }
	void set_select_spaces(BOOL setting ) { m_data.m_select_spaces = setting ; }
	void set_shortcuts_active(BOOL setting ) { m_data.m_shortcuts_active = setting ; }

	void set_is_fresh_install(){m_data.m_freshInstall = TRUE ; }
	void set_is_not_fresh_install(){m_data.m_freshInstall = FALSE ; }

	void setDefaults();
	void set_seg_defaults() ;
	app_state() ;
	app_state( const app_state &rhs ) ;
	app_state &operator=( const app_state &rhs ) ;
	void _internal_copy ( const app_state &rhs ) ;

	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, _T("Software\\AssistantSuite\\FelixWordInterface"), _T("PROPERTIES") )
		
		REG_ENTRY_BOOL( _T("TOOLBAR_VISIBLE"), m_data.m_toolbar_visible )
		
		REG_ENTRY_INT( _T("PREFERRED_GUI_LANG"), m_data.m_preferred_gui_lang )

		REG_ENTRY_BOOL( _T("FORMAT_FACE"), m_data.m_font_face )
		REG_ENTRY_BOOL( _T("FORMAT_COLOR"), m_data.m_font_color )
		REG_ENTRY_BOOL( _T("FORMAT_BOLD"), m_data.m_font_bold )
		REG_ENTRY_BOOL( _T("FORMAT_ITALIC"), m_data.m_font_italic )
		REG_ENTRY_BOOL( _T("FORMAT_UNDERLINE"), m_data.m_font_underline )
		REG_ENTRY_BOOL( _T("FORMAT_SUPER_SUB"), m_data.m_font_super_sub )
	
		REG_ENTRY_INT( _T("SEGMENTATION_RULE"), m_data.m_segmentation_type )
		REG_ENTRY_INT( _T("SEG_SKIP_DBYTE"), m_data.m_skipJ )
		REG_ENTRY_BOOL( _T("SEG_SKIP_NUMBERS"), m_data.m_skipNumbers )
		REG_ENTRY_STRING( _T("SEGMENT_END_CHARACTERS"),	m_data.m_segChars, SEG_CHAR_SIZE )
		REG_ENTRY_STRING( _T("MANUAL_URL"),	m_data.m_manual_url, MANUAL_URL_SIZE )

		REG_ENTRY_BOOL(_T("SELECT_SPACES"), m_data.m_select_spaces)

		REG_ENTRY_BOOL( _T("FRESH_INSTALL"), m_data.m_freshInstall )

		REG_ENTRY_BOOL(_T("USE_TRANS_HISTORY"), m_data.m_use_trans_hist)
		REG_ENTRY_BOOL(_T("SHORTCUTS_ACTIVE"), m_data.m_shortcuts_active)
		REG_ENTRY_BOOL(_T("REALLY_WANTS_NO_DELIMS"), m_data.m_really_wants_no_delims)
		REG_ENTRY_BOOL( _T("RAISE_FELIX"), m_data.m_raise_felix )

		if (m_data.m_segmentation_type != SEG_TYPE_WORD_DOC &&
			m_data.m_segmentation_type != SEG_TYPE_HTML_DOC )
		{
			m_data.m_segmentation_type = SEG_TYPE_WORD_DOC ;
		}
		if (_tcscmp(m_data.m_segChars, _T("refresh")) == 0)
		{
			set_seg_defaults() ;
		}
	END_REGISTRY_MAP

} ;

