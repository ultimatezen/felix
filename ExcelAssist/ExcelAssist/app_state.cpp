// app_state.cpp: implementation of the app_state class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app_state.h"


/*!
* CTOR. Sets default values.
*/
app_state::app_state() : 
		m_toolbar_visible(TRUE),
		m_preferred_gui_lang(PREF_LANG_ENGLISH),
		m_skipNumbers(TRUE),
		m_skipJ(NO_SKIP),
		m_freshInstall(TRUE),
		m_use_trans_hist(FALSE),
		m_shortcuts_active(TRUE)
	{
		set_default_lang() ;
	}


	/*!
	* Sets default values.
	*/
	void app_state::setDefaults()
	{
		BANNER( "app_state::setDefaults" ) ;
		m_toolbar_visible = TRUE ;

		set_default_lang();

		m_skipNumbers = TRUE ;
		m_skipJ = NO_SKIP ;
		m_use_trans_hist = FALSE ;

		m_freshInstall = TRUE ;
		m_shortcuts_active = TRUE ;
		m_raise_felix = FALSE ;

	}

	/*! Copy constructor
	*/
	app_state::app_state( const app_state &rhs )
	{
		m_toolbar_visible = rhs.m_toolbar_visible ;
		m_preferred_gui_lang = rhs.m_preferred_gui_lang ;
		m_skipNumbers = rhs.m_skipNumbers ;
		m_skipJ = rhs.m_skipJ ;
		m_freshInstall = rhs.m_freshInstall; // is this useful?
		m_use_trans_hist = rhs.m_use_trans_hist;
		m_raise_felix = rhs.m_raise_felix ;
	}

	/*! Assignment operator.
	*/
	app_state &app_state::operator=( const app_state &rhs )
	{
		BANNER( "app_state::operator=( const app_state &rhs )" ) ;

		m_toolbar_visible = rhs.m_toolbar_visible ;
		m_preferred_gui_lang = rhs.m_preferred_gui_lang ;
		m_skipNumbers = rhs.m_skipNumbers ;
		m_skipJ = rhs.m_skipJ ;
		m_freshInstall = rhs.m_freshInstall; // is this useful?
		m_use_trans_hist = rhs.m_use_trans_hist;
		m_raise_felix = rhs.m_raise_felix ;

		return *this ;
	}

	void app_state::set_default_lang()
	{
		// preferred GUI language
		LANGID lid = ::GetUserDefaultLangID() ;
		WORD preflang = (WORD)PRIMARYLANGID( lid ) ;

		if ( preflang == LANG_JAPANESE )
		{
			m_preferred_gui_lang = PREF_LANG_JAPANESE ;
		}
		else
		{
			m_preferred_gui_lang = PREF_LANG_ENGLISH ;
		}
	}
