#include "StdAfx.h"
#include ".\extrastringsview.h"

#include "background_processor.h"
#include "resource.h"

using namespace html ;

CExtraStringsView::CExtraStringsView(void)
{
}

CExtraStringsView::~CExtraStringsView(void)
{
}

bool CExtraStringsView::has_focus(TWindow focus_window )
{
	if ( ! m_extra_strings_view->IsWindow() ) 
	{
		return false ;
	}

	if ( m_extra_strings_view->m_hWnd == focus_window ) 
	{
		return true ;
	}

	if ( m_extra_strings_view->IsChild( focus_window ) ) 
	{
		return true ;
	}

	return false;
}

LRESULT CExtraStringsView::send_command(WPARAM wParam, LPARAM lParam)
{
	return m_extra_strings_view->SendMessage( WM_COMMAND, wParam, lParam ) ;
}

void CExtraStringsView::handle_size(void)
{
	RECT rc = { 0 } ;
	m_extra_strings_box.GetWindowRect( &rc ) ;
	m_parent_dlg.ScreenToClient( &rc ) ;
	m_extra_strings_box.ShowWindow( SW_HIDE ) ;

	// move the context view into place
	m_extra_strings_view->SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_NOACTIVATE);

}

void CExtraStringsView::create(HWND box_hwnd, HWND dlg_hwnd )
{
	m_parent_dlg = dlg_hwnd ;

	m_extra_strings_box.Attach( box_hwnd ) ;
	CRect rc(0,0,0,0) ;
	m_extra_strings_box.GetWindowRect( &rc ) ;
	m_parent_dlg.ScreenToClient( &rc ) ;

	// create the html view and move it into place
	m_extra_strings_view = ref_pointer< CHtmlView >() ;
	m_extra_strings_view->AddRef() ;
	m_extra_strings_view->create( m_parent_dlg ) ;

	m_extra_strings_view->MoveWindow( &rc, TRUE ) ;
}

void CExtraStringsView::show_extra_strings(record_pointer rec)
{
	wstring desc ;
	desc.reserve( 1024 ) ;

	desc = L"<table class=\"extra_strings\">\n" ;

	write_table_entries(desc, rec);

	desc << L"</table>" ;

	background_processor backer ;

	while( ! m_extra_strings_view->is_document_complete() || ! m_extra_strings_view->is_navigation_complete() )
	{
		backer.perform_background_processing() ;
	}
	m_extra_strings_view->set_body_text( desc ) ;
}


void CExtraStringsView::fill_user_strings( record_pointer &rec )
{
#ifdef UNIT_TEST
	return ;
#endif
	// get the collection of HTML elements in the doc body
	CHtmlDocument doc = m_extra_strings_view->get_document() ;
	CHtmlElementCollection collection = doc.get_all() ;

	// get the length of the collection
	int len = collection.get_length() ;

	wstring key, value ;

	rec->clear_user_strings() ;

	// loop through each of the elements
	for ( int i=0 ; i<len ; ++i )
	{
		TRACE( i ) ;
		CHtmlElement element = collection.item( i ) ;

		wstring id ;
		element.get_id( id ) ;

		wstring setting ;
		if ( ! id.empty() )
		{
			_bstr_t tag_name = element.get_tag_name() ;
			if ( tag_name != _bstr_t( L"TR" ) )
			{
				if ( id == L"key" )
				{
					element.get_text( setting ) ;

					eliminate_whitespace_and_punctuation(setting, key);				}
				else if ( id == L"value" )
				{
					// key must not be empty
					if ( key.empty() == false )
					{
						// we allow rich text in extra strings
						value = BSTR2wstring( element.get_inner_html( ) ) ;
						rec->set_item( key, value ) ;
					}
				}
			}
		} 
	} // increment i...
}


void CExtraStringsView::eliminate_whitespace_and_punctuation(const std::wstring& setting, std::wstring& key)
{
	// we will eliminate whitespace and punctuation
	textstream_reader< wchar_t > reader ;
	reader.set_buffer( setting.c_str() ) ;
	key.erase() ;

	// scan through buffer...
	while ( reader.empty() == false )
	{
		// looking for whitespace and punct...
		if ( ! reader.is_space() && ! reader.is_punct() ) 
		{
			reader.get( key ) ;
		}
		// ... and replacing them with underscores
		else
		{
			reader.advance() ;
			key += L"_" ;
		}
	}

}

void CExtraStringsView::write_table_entries(std::wstring& desc, record_pointer& rec)
{
	translation_record::record_data_iterator pos ;
	int index = 0 ;
	for ( pos = rec->data_begin() ; pos != rec->data_end() ; ++pos )
	{
		wstring id = pos->first ;

		if ( ! str::equal_nocase( id, R2WSTR( IDS_RELIABILITY_ID ) ) 
			&& ! str::equal_nocase( id, R2WSTR( IDS_VALIDATED_ID ) ) 
			&& ! str::equal_nocase( id, R2WSTR( IDS_REFCOUNT_ID ) ) 
			&& ! pos->second.empty() )
		{
			desc << L"<tr id=\"" << int2wstring( index ) << L"\">"
				<< L"<td id=\"key\"><b>" << id << L"</b></td>"
				<< L"<td id=\"value\">"	<< pos->second  << L"</td></tr>\n" ;

			index++ ;
		}
	}

}

bool CExtraStringsView::is_edit_mode(void)
{
	return  !  m_extra_strings_view->get_edit_mode() ;
}

void CExtraStringsView::put_edit_mode(bool edit_mode)
{
	m_extra_strings_view->put_edit_mode( edit_mode ) ;
}


void CExtraStringsView::cleanup(void)
{
	m_extra_strings_box.Detach() ;

	m_extra_strings_view->DestroyWindow() ;
	m_extra_strings_view->Release() ;
}



void CExtraStringsView::gain_focus(void)
{
	m_extra_strings_view->SetFocus() ;
}

BOOL CExtraStringsView::pre_translate( MSG *pMsg )
{
	return m_extra_strings_view->PreTranslateMessage( pMsg ) ;
}

