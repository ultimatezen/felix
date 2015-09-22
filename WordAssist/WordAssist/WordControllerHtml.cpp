#include "stdafx.h"
#include ".\wordcontrollerhtml.h"

CWordControllerHtml::CWordControllerHtml(LPCWSTR) //:
	//m_trans_history(history_server)
{
}

CWordControllerHtml::~CWordControllerHtml(void)
{
}

_bstr_t CWordControllerHtml::get_selection_text( bool as_plaintext )
{
	as_plaintext ;

	WordSelection selection = m_word_object.get_selection() ;

	return selection.get_text() ;
}

void CWordControllerHtml::write_translation( const wstring& text, bool as_plaintext )
{
	as_plaintext ;

	// set_status_bar_text( IDS_MENU_GET_TT ) ;
	WordSelection selection = m_word_object.get_selection() ;
	selection.put_text( wstring( ) ) ;

	WordRange word_range = selection.get_range() ;

	wstring plain_text ;
	if ( as_plaintext )
	{
		plain_text = strip_tags( text ) ;
	}
	else
	{
		plain_text = text ;
	}

	word_range.put_text( plain_text.c_str() ) ;
	word_range.collapse_end() ;
	word_range.select() ;
}

bool CWordControllerHtml::OnAnalyze()
{
	MessageBox( NULL, _T("OnMenuAnalyze"), _T("To Be Implemented"), MB_OK ) ;

	return true ;
}