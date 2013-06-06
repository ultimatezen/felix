#include "stdafx.h"
#include "view_state.h"

#ifdef UNIT_TEST
	#include "document_wrapper_fake.h"
#endif

wstring get_no_match_text() 
{
	return L"<center><h1>" + R2WSTR( IDS_NO_MATCHES ) + L"</h1></center>";
}


void ViewState::set_view( view_interface *view )
{
	m_view = view ;
}

void ViewState::set_model( model_iface_ptr model )
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

doc3_wrapper_ptr ViewState::get_doc3()
{
#ifndef UNIT_TEST
	return make_doc3_wrapper(m_view->get_document3());
#else
	return get_fake_search_doc(); 
#endif
}

void ViewState::set_div_content( const wstring div_name, const wstring &div_content )
{
	doc3_wrapper_ptr doc = get_doc3() ;
	element_wrapper_ptr div = doc->get_element_by_id(div_name) ;
	div->set_inner_text(div_content) ;
}


void ViewState::erase_from_memory( mem_engine::search_match_ptr match )
{
	mem_engine::memory_pointer = get_memory_from_match(match);
	if (! mem)
	{
		logging::log_error("Failed to retrieve memory") ;
		return ;
	}
	mem->erase(match->get_record()) ;
}

mem_engine::memory_pointer ViewState::get_memory_from_match( mem_engine::search_match_ptr match )
{
	try
	{
		return m_model->get_memory_by_id(match->get_memory_id()) ;
	}
	catch (except::CProgramException& e)
	{
		logging::log_error("Program exception") ;
		logging::log_exception(e) ;
		return m_model->get_first_memory() ;
	}
}

