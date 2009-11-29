/*!
	@file LanguageHolder.h
	@brief Interface definition file for CLanguageHolder
	@date 2005/06/25
	Time: 13:13:17
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "StringEx.h"	// tstring
#include <set>				// std::set
#include <vector>

#ifdef UNIT_TEST
#define CComboBox CComboBoxT<CFakeWindow>
#endif

/**
	@class CLanguageHolder
	@brief We hold the language list in here (encapsulation).
 */
class CLanguageHolder
{
public:
	std::vector<tstring>		m_languages  ;
	tstring						m_source ;
	tstring						m_trans ;

	CLanguageHolder(void);
	~CLanguageHolder(void);

	bool set_languages( std::set<tstring> &languages ) ;

	tstring get_source_plain();
	tstring get_trans_plain();

	void set_source( const tstring &source );
	void set_trans( const tstring &trans );

	void put_langs_into_combo(CComboBox & combo);

	// Extract the current selection from the combo box and set the source to that
	void set_source(CComboBox & combo);
	// Extract the current selection from the combo box and set the trans to that
	void set_trans(CComboBox & combo);
	// Extract the current selection from the combo box and return it as a tstring
	tstring combo_sel_as_str(CComboBox & combo);
};
