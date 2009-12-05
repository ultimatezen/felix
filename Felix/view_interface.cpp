/*!
	@brief implementation for the view_interface class.
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "view_interface.h"

#include "ViewCollectionWalker.h" // CViewCollectionWalker

using namespace memory_engine ;

/** Set the HTML listener in the view.
 */
void view_interface::set_listener( html::CHtmlViewListener *listener )
{
	m_view.set_listener( listener ) ;
}


/**
	@class view_interface 
	@brief Interface to the view -- adds various functions specific to Felix.
 */
void view_interface::ensure_document_complete(HACCEL accel, HWND hwnd)
{
#ifdef UNIT_TEST
	return ;
#endif
	background_processor backer( 100, accel, hwnd ) ;

	while ( m_view.is_document_complete() == false ) 
	{
		backer.check_iterations() ;
	}
}

void view_interface::ensure_navigation_complete(HACCEL accel, HWND hwnd)
{
	background_processor backer( 100, accel, hwnd ) ;

	while ( m_view.is_navigation_complete() == false ) 
	{
		backer.check_iterations() ;
	}

}

const wstring view_interface::get_selection_text()
{
	html::CHtmlSelection selection = m_view.get_selection() ;
	html::CHtmlTextRange range = selection.create_text_range() ;
	const wstring selection_text = BSTR2wstring(range.get_html_text()) ;
	range.collapse() ;
	range.select() ;
	return selection_text ;
}

void view_interface::create( HWND parent, HWND &client )
{
	client = m_view.create( parent ) ;
	ATLASSERT( TWindow( client ).IsWindow() ) ;
}
void view_interface::set_text( const wstring &text )
{
	m_view.set_body_text( text ) ;
}
void view_interface::load_resource( LPCTSTR resource_name )
{
	m_view.load_from_resource( resource_name ) ;
}
void view_interface::navigate(LPCTSTR url)
{
	m_view.navigate(_bstr_t(url)) ;
}

void view_interface::do_bold() 
{
	m_view.do_bold() ;
}
void view_interface::do_underline()
{
	m_view.do_underline() ;
}
void view_interface::do_italic()
{
	m_view.do_italic() ;
}
void view_interface::do_delete()
{
	m_view.OnEditDelete() ;
}
void view_interface::set_bg_color( const wstring &color ) 
{
	html::CHtmlDocument doc = m_view.get_document() ;
	doc.set_bg_color( color ) ;

	html::collection_ptr collection = doc.get_all() ;

	CStringW tagbase ;
	tagbase.Format(L"background-color: %ls", color.c_str()) ;
	const _bstr_t bg_style = static_cast< LPCWSTR >( tagbase ) ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		TRACE( i ) ;
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		TRACE(element->tagName) ;
		if ( 0 == _wcsicmp(static_cast< LPCWSTR >( element->tagName ), L"TD" ))
		{
			element->style->cssText = bg_style ;
		} 
	} // increment i...


}
wstring view_interface::get_bg_color()
{
#ifdef UNIT_TEST
	return wstring(L"#FFFFFF") ;
#endif

	html::CHtmlDocument doc = m_view.get_document() ;
	return BSTR2wstring( doc.get_bg_color() ) ;
}
html::document_ptr view_interface::get_document()
{
	return m_view.get_document() ;
}

bool view_interface::is_edit_mode()
{
	return m_view.get_edit_mode() ;
}
void view_interface::put_edit_mode( bool setting )
{
	m_view.put_edit_mode( setting ) ;
}

// edit stuff

void view_interface::handle_enter_edit_mode_new_record_glossary( )
{
	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	// the current match...

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		const wstring id = BSTR2wstring( element->id ) ;

		if ( id == L"context_tmp"  )
		{
			element->innerHTML = _bstr_t() ;
			element->id = L"context" ;

			// put the actual browse mode setting
			m_view.put_edit_mode( true ) ;

			return ;
		}
	}

	// put the actual browse mode setting
	m_view.put_edit_mode( true ) ;

	html::CHtmlSelection selection = m_view.get_selection() ;
	html::CHtmlTextRange text_range = selection.create_text_range() ;
	text_range.select() ;
}

void view_interface::handle_enter_edit_mode_concordance_glossary( memory_engine::search_query_glossary &matches )
{
	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	// get the length of the collection
	// both modes will need these...
	CViewCollectionWalker walker ;
	walker.RecordsToElements( matches, collection ) ;

	// put the actual browse mode setting
	m_view.put_edit_mode( true ) ;
	html::CHtmlSelection selection = m_view.get_selection() ;
	html::CHtmlTextRange text_range = selection.create_text_range() ;
	text_range.select() ;
}



bool view_interface::handle_leave_edit_mode_new_record_glossary( MemoryControllerType memories, record_pointer &record )
{
	// set the actual edit mode
	m_view.put_edit_mode( false ) ;

	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	memory_engine::memory_pointer mem = memories->get_first_memory() ;
	mem->erase( record ) ;

	// the *@!# DHTML will convert our nice relative URLs to absolute ones!
	clean_up_urls(collection) ;

	CViewCollectionWalker walker ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		walker.ElementToRecord( make_element_wrapper(element), record ) ;
	} // increment i...

	// ====================
	// show the edited records
	// ====================

	// make sure we have something to show
	if ( false == record->is_valid_record()) 
	{
		return false ;
	} 

	return true ;
}

bool view_interface::handle_leave_edit_mode_concordance_glossary( MemoryControllerType memories, memory_engine::search_query_glossary &matches )
{

	// set the actual edit mode
	m_view.put_edit_mode( false ) ;
	if (matches.empty())
	{
		return false ;
	}

	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	// both modes will need these...
	record_pointer	record(new record_local()) ;
	SearchMatchType	match = matches.at(0) ;
	MatchListType	match_list ;

	// the *@!# DHTML will convert our nice relative URLs to absolute ones!
	clean_up_urls(collection) ;

	CViewCollectionWalker walker ;
	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		TRACE( i ) ;
		MSHTML::IHTMLElementPtr element = collection->item( _variant_t( i ), _variant_t(0) ) ;

		const wstring id = BSTR2wstring( element->id ) ;

		if ( ! id.empty() )
		{
			if ( str::is_int_rep( id ) )
			{
				if ( record->is_valid_record() )
				{
					walker.AddRecordToList(record, memories, match, match_list);
				}
				// get current match setting, and erase it from memory
				walker.EraseCurrentRecord(match, matches, id, memories);

				// set to current record
				record = match->get_record() ;
			}
			else
			{
				walker.ElementToRecord( make_element_wrapper(element), record ) ;
			}
		} 
	} // increment i...

	if ( record->is_valid_record() )
	{
		walker.AddRecordToList(record, memories, match, match_list);
	}

	matches.set_matches( match_list ) ;

	if ( matches.empty() ) 
	{
		return false ;
	}

	return true ;
}



void view_interface::handle_enter_edit_mode_new_record()
{
	m_view.put_edit_mode( true ) ;

	html::CHtmlSelection selection = m_view.get_selection() ;
	html::CHtmlTextRange text_range = selection.create_text_range() ;
	text_range.select() ;
}

void view_interface::handle_enter_edit_mode_match( TransMatchQueryTrans &matches )
{
	// Get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	record_pointer rec = get_match_record(matches);

	CViewCollectionWalker walker ;
	tag_name_holder &tags = tag_name_holder::instance() ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		TRACE( i ) ;
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		const wstring id = BSTR2wstring( element->id ) ;

		if ( id.empty() == false )
		{
			if ( id == tags.query_tag )	
			{
				element->innerHTML = ( matches.get_query_rich() ).c_str() ;
				// we need to re-calculate the length, because we've been mucking with the html
			}
			else
			{
				walker.RecordToElement( element, rec )  ;
			}
		}
	}

	// put the actual browse mode setting
	m_view.put_edit_mode( true ) ;

	html::CHtmlSelection selection = m_view.get_selection() ;
	html::CHtmlTextRange text_range = selection.create_text_range() ;
	text_range.select() ;
}

void view_interface::handle_enter_edit_mode_concordance( memory_engine::search_query_mainframe &matches )
{
	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	CViewCollectionWalker walker ;
	walker.RecordsToElements( matches, collection ) ;

	// put the actual browse mode setting
	m_view.put_edit_mode( true ) ;
}



// =========================
// for leaving edit mode
// =========================
void view_interface::handle_leave_edit_mode_new( record_pointer &record )
{
	// set the actual edit mode
	m_view.put_edit_mode( false ) ;

	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	// the *@!# DHTML will convert our nice relative URLs to absolute ones!
	clean_up_urls(collection) ;

	CViewCollectionWalker walker ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		TRACE( i ) ;
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		walker.ElementToRecord( make_element_wrapper(element), record ) ;
	} // increment i...

}

void view_interface::handle_leave_edit_mode_match( MemoryControllerType memories, TransMatchQueryTrans &matches )
{
	// set the actual edit mode
	m_view.put_edit_mode( false ) ;

	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	record_pointer rec = get_match_record(matches) ;

	// the *@!# DHTML will convert our nice relative URLs to absolute ones!
	clean_up_urls(collection) ;

	std::list< int > marked_for_death ;

	tag_name_holder &tags = tag_name_holder::instance() ;

	CViewCollectionWalker walker ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		TRACE( i ) ;
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		const wstring id = BSTR2wstring( element->id ) ;

		if ( ! id.empty() )
		{
			if ( str::is_int_rep( id ) )
			{
				int pos = boost::lexical_cast< long >( id ) ;
				if ( false == rec->is_valid_record() ) 
				{
					marked_for_death.push_front(pos) ;
				}

				// set to current record
				rec = matches.at(pos)->get_record() ;
			}
			else if ( id == tags.query_tag )	
			{
				matches.set_query_rich( BSTR2wstring(element->innerHTML) ) ;
			}
			else
			{
				walker.ElementToRecord( make_element_wrapper(element), rec ) ;
			}
		} 
	} 

	// ====================
	// show the edited records
	// ====================

	// make sure we have something to show
	// if we are just showing a query with no matches...
	if ( matches.empty())
	{
		return ;
	}
	// if the source or trans is empty and we have a match, erase it
	std::list< int >::iterator pos ;
	for ( pos = marked_for_death.begin() ; pos != marked_for_death.end() ; ++pos )
	{
		matches.erase_at( *pos ) ;
	}
}

bool view_interface::handle_leave_edit_mode_concordance( MemoryControllerType memories, memory_engine::search_query_mainframe &matches )
{
	m_view.put_edit_mode( false ) ;
	if ( memories->empty() ) 
	{
		return false ;
	}
	if (matches.empty())
	{
		return false ;
	}

	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	// the *@!# DHTML will convert our nice relative URLs to absolute ones!
	clean_up_urls(collection) ;
	std::list< int > marked_for_death ;

	SearchMatchType	match = matches.at(0);
	MatchListType	match_list ;

	record_pointer rec = record_pointer(new record_local()) ;
	CViewCollectionWalker walker ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		const wstring id = BSTR2wstring( element->id ) ;

		if ( ! id.empty() )
		{
			if ( str::is_int_rep( id ) )
			{
				ATLTRACE("\n ... Found an id\n") ;
				TRACE( i ) ;
				if ( rec->is_valid_record() )
				{
					ATLTRACE(" ... Adding to list\n") ;
					walker.AddRecordToList(rec, memories, match, match_list);
				}
				// get current match setting, and erase it from memory
				ATLTRACE(" ... Erasing\n") ;
				walker.EraseCurrentRecord(match, matches, id, memories);

				// set to current record
				rec = match->get_record() ;
			}
			else
			{
				walker.ElementToRecord( make_element_wrapper(element), rec );
			}
		} 
	} // increment i...

	if ( rec->is_valid_record() )
	{
		walker.AddRecordToList(rec, memories, match, match_list);
	}

	matches.set_matches( match_list ) ;

	// make sure we have something to show
	if ( matches.empty() )
	{
		return false ;
	}

	return true ;
}


void view_interface::scroll_element_into_view( const wstring &current_id )
{
	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		const wstring id = BSTR2wstring( element->id ) ;

		if ( id == current_id )
		{
			element->scrollIntoView( _variant_t( VARIANT_TRUE ) ) ;
			return ;
		}
	}
}


BOOL view_interface::PreTranslateMessage( LPMSG pMsg )
{
	return m_view.PreTranslateMessage( pMsg ) ;
}

BOOL view_interface::ProcessWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT &lResult)
{
	return m_view.ProcessWindowMessage( hWnd, uMsg, wParam, lParam, lResult ) ;
}
void view_interface::Move( LPRECT rect )
{
	m_view.MoveWindow( rect, TRUE ) ;
}

html::collection_ptr view_interface::get_element_collection()
{
	html::CHtmlDocument doc = m_view.get_document() ;
	return doc.get_all() ;
}

void view_interface::clean_up_urls( html::collection_ptr &collection )
{
	BANNER( "CCommonWindowFunctionality::clean_up_urls" ) ;

	html::CHtmlDocument doc = m_view.get_document() ;

	const wstring doc_path = get_doc_path(BSTR2wstring(doc.get_url()));

	CViewCollectionWalker walker ;

	for ( long i=0 ; i < collection->length ; ++i )
	{
		element_wrapper_ptr element = make_element_wrapper(collection->item(_variant_t(i))) ;
		walker.CheckLinkUrl(element, doc_path);
	} // increment i...
}

wstring view_interface::get_doc_path( const wstring doc_url )
{
	wstring local_docurl(doc_url) ;
	str::replace_all(local_docurl, L"\\", L"/") ;

	size_t pos = local_docurl.find_last_of( L'/' )	;
	// we add one to the length to get the slash
	return str::left( local_docurl, pos + 1 ) ;
}

view_interface::record_pointer view_interface::get_match_record( TransMatchQueryTrans &matches )
{
	// The current match...
	if ( false == matches.empty() ) // If we have some matches...
	{
		// we set it to the current match
		return matches.current()->get_record() ;
	}
	else
	{
		record_pointer record = record_pointer(new memory_engine::record_local) ;
		record->set_source(matches.get_query_rich()) ;
		return record ;
	}
}
