#include "StdAfx.h"
#include ".\languageholder.h"

CLanguageHolder::CLanguageHolder(void)
{
}

CLanguageHolder::~CLanguageHolder(void)
{
}

void CLanguageHolder::put_langs_into_combo(CComboBox & combo)
{
	FOREACH(tstring lang, m_languages)
	{
		combo.AddString( lang.c_str() ) ;
	}
}

bool CLanguageHolder::set_languages( std::set<tstring> &languages )
{
	m_languages.assign(languages.begin(), languages.end()) ;
	return true ;
}

// Extract the current selection from the combo box and set the source to that
void CLanguageHolder::set_source(CComboBox & combo)
{
	m_source = combo_sel_as_str( combo ) ;
}

void CLanguageHolder::set_source( const tstring &source )
{
	m_source = source ;
}
// Extract the current selection from the combo box and set the source to that
void CLanguageHolder::set_trans(CComboBox & combo)
{
	m_trans = combo_sel_as_str( combo ) ;
}

void CLanguageHolder::set_trans( const tstring &trans )
{
	m_trans = trans ;
}

const tstring CLanguageHolder::combo_sel_as_str(CComboBox & combo) const
{
	CString combo_text ;
#ifndef UNIT_TEST
	int sel = combo.GetCurSel() ;
	combo.GetLBText( sel, combo_text ) ;
#else
	combo ;
#endif	
	return tstring( static_cast< LPCTSTR >( combo_text ) ) ;
}

const tstring CLanguageHolder::get_source_plain() const
{
	return m_source ;
}

const tstring CLanguageHolder::get_trans_plain() const
{
	return m_trans ;
}
