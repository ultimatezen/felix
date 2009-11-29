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
	size_t num_lingos = m_languages.size() ;
	for( size_t i=0 ; i<num_lingos ; ++i )
	{
		CString lang = m_languages[i].c_str() ;
		combo.AddString( lang ) ;
	}
}

bool CLanguageHolder::set_languages( std::set<tstring> &languages )
{
	m_languages.clear() ;
	std::copy( languages.begin(), languages.end(), std::back_inserter( m_languages ) ) ;
	return true ;
}

// Extract the current selection from the combo box and set the source to that
void CLanguageHolder::set_source(CComboBox & combo)
{

#ifndef UNIT_TEST
	m_source = combo_sel_as_str( combo ) ;
#endif
}

void CLanguageHolder::set_source( const tstring &source )
{
	m_source = source ;
}
// Extract the current selection from the combo box and set the source to that
void CLanguageHolder::set_trans(CComboBox & combo)
{
#ifndef UNIT_TEST
	m_trans = combo_sel_as_str( combo ) ;
#endif
}

void CLanguageHolder::set_trans( const tstring &trans )
{
	m_trans = trans ;
}

tstring CLanguageHolder::combo_sel_as_str(CComboBox & combo)
{
	CString combo_text ;
	int sel = combo.GetCurSel() ;
	combo.GetLBText( sel, combo_text ) ;
	return tstring( static_cast< LPCTSTR >( combo_text ) ) ;
}

tstring CLanguageHolder::get_source_plain()
{
	return m_source ;
}

tstring CLanguageHolder::get_trans_plain()
{
	return m_trans ;
}