#include "stdafx.h"
#include "view_state.h"


void ViewState::set_view( view_interface *view )
{
	m_view = view ;
}

void ViewState::set_model( FelixModelInterface *model )
{
	m_model = model ;
}

void ViewState::set_window_listener( WindowListener *listener )
{
	m_window_listener = listener ;
}

void ViewState::on_user_edit()
{
	search_match_ptr match = this->get_current_match() ;
	m_window_listener->set_item_under_edit(match) ;
	ATLASSERT( match->get_memory_id() != 0 ) ;
	m_window_listener->show_edit_dialog( match->get_record(), match->get_memory_id(), get_edit_record_title() ) ;
}

void ViewState::set_current( size_t num )
{
	num ;
}

size_t ViewState::get_current()
{
	return 0 ;
}

int ViewState::get_edit_record_title()
{
	return IDS_EDIT_RECORD_TITLE;
}

wstring ViewState::retrieve_record_trans( record_pointer rec, record_string_prefs prefs )
{
	rec->increment_refcount() ;

	wstring trans ;
	if (prefs.m_plain_text)
	{
		trans = rec->get_trans_plain() ;
		boost::replace_all(trans, L"&", L"&amp;") ;
		boost::replace_all(trans, L">", L"&gt;") ;
		boost::replace_all(trans, L"<", L"&lt;") ;
	}
	else
	{
		trans = rec->get_trans_rich() ;
	}

	if (prefs.m_to_lower)
	{
		boost::to_lower(trans) ;
	}

	return trans ;
}