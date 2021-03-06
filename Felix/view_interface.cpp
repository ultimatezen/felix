/*!
	@brief implementation for the frame_view class.
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "view_interface.h"

#include "ViewCollectionWalker.h" // CViewCollectionWalker
#include "record_local.h"

using namespace mem_engine ;

const static size_t MAX_VIEW_WAIT_ITERATIONS = 10000;

/** Set the HTML listener in the view.
 */
void frame_view::set_listener( html::CHtmlViewListener *listener )
{
	m_view.set_listener( listener ) ;
}


/**
	@class frame_view 
	@brief Interface to the view -- adds various functions specific to Felix.
 */
void frame_view::ensure_document_complete()
{
#ifdef UNIT_TEST
	return ;
#else
	if (! m_view.IsWindow())
	{
		ATLTRACE("Browser view not created. Cannot ensure document complete.\n") ;
		return ;
	}
	background_processor backer(MAX_VIEW_WAIT_ITERATIONS, m_accel, m_parent) ;

	while ( m_view.is_document_complete() == false) 
	{
		backer.check_iterations() ;
	}
#endif
}

void frame_view::ensure_navigation_complete()
{
	if (! m_view.IsWindow())
	{
		ATLTRACE("Browser view not created. Cannot ensure navigation complete.\n") ;
		return ;
	}
	background_processor backer(MAX_VIEW_WAIT_ITERATIONS, m_accel, m_parent) ;

	while ( m_view.is_navigation_complete() == false ) 
	{
		backer.check_iterations() ;
	}

}

const wstring frame_view::get_selection_text()
{
#ifdef UNIT_TEST
	return L"spam" ;
#else
	if (! m_view.IsWindow())
	{
		ATLTRACE("Browser view not created. Cannot get selection text.\n") ;
		return wstring();
	}
	html::CHtmlSelection selection = m_view.get_selection() ;
	html::CHtmlTextRange range = selection.create_text_range() ;
	const wstring selection_text = BSTR2wstring(range.get_html_text()) ;
	range.collapse() ;
	range.select() ;
	return selection_text ;
#endif
}

void frame_view::create( HWND parent, HWND &client, CString filename )
{
	m_parent = parent ;
	if (filename.GetLength())
	{
		client = m_view.create(parent, filename) ;
	}
	else
	{
		client = m_view.create(parent) ;
	}
	ATLASSERT( CWindow( client ).IsWindow() ) ;
}
void frame_view::set_text( const wstring text )
{
	if (! m_view.IsWindow())
	{
		ATLTRACE("Browser view not created. Cannot set text.\n") ;
		return ;
	}
	m_view.set_body_text( text ) ;
}
void frame_view::load_resource( LPCTSTR resource_name )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot load resource.") ;
		return ;
	}
	m_view.load_from_resource( resource_name ) ;
}
void frame_view::navigate(LPCTSTR url)
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot navigate.") ;
		return ;
	}
	m_view.navigate(_bstr_t(url)) ;
}

void frame_view::do_bold() 
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot set bold.") ;
		return ;
	}
	m_view.do_bold() ;
}
void frame_view::do_underline()
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot set underline.") ;
		return ;
	}
	m_view.do_underline() ;
}
void frame_view::do_italic()
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot set italic.") ;
		return ;
	}
	m_view.do_italic() ;
}
void frame_view::do_delete()
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot delete.") ;
		return ;
	}
	m_view.OnEditDelete() ;
}
void frame_view::set_bg_color( const wstring color ) 
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot set background color.") ;
		return ;
	}
	html::CHtmlDocument doc = m_view.get_document() ;
	doc.set_bg_color( color ) ;

	html::collection_ptr collection = doc.get_all() ;

	CStringW tagbase ;
	tagbase.Format(L"background-color: %ls", color.c_str()) ;
	const _bstr_t bg_style = static_cast< LPCWSTR >( tagbase ) ;

	// Set background color of each table cell
	for ( int i=0 ; i < collection->length ; ++i )
	{
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		if ( 0 == _wcsicmp(static_cast< LPCWSTR >( element->tagName ), L"TD" ))
		{
			element->style->cssText = bg_style ;
		} 
	}


}
wstring frame_view::get_bg_color()
{
#ifdef UNIT_TEST
	return wstring(L"#FFFFFF") ;
#else
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot get background color.") ;
		return wstring();
	}

	html::CHtmlDocument doc = m_view.get_document() ;
	return BSTR2wstring( doc.get_bg_color() ) ;
#endif
}
html::document_ptr frame_view::get_document()
{
	return m_view.get_document() ;
}

bool frame_view::is_edit_mode()
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot get edit mode.") ;
		return false ;
	}
	return m_view.get_edit_mode() ;
}
void frame_view::put_edit_mode( bool setting )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot set edit mode.") ;
		return ;
	}
	m_view.put_edit_mode( setting ) ;
}

// edit stuff

void frame_view::handle_enter_edit_mode_new_record_glossary( )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle enter edit mode.") ;
		return ;
	}
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

void frame_view::handle_enter_edit_mode_concordance_glossary( mem_engine::felix_query *matches )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle enter edit mode (glossary concordance).") ;
		return ;
	}
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



bool frame_view::handle_leave_edit_mode_new_record_glossary( MemoryControllerType memories, record_pointer &record )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle leave edit mode.") ;
		return false;
	}
	// set the actual edit mode
	m_view.put_edit_mode( false ) ;

	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	mem_engine::memory_pointer mem = memories->get_first_memory() ;
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


void frame_view::handle_enter_edit_mode_new_record()
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle enter edit mode.") ;
		return ;
	}
	m_view.put_edit_mode( true ) ;

	html::CHtmlSelection selection = m_view.get_selection() ;
	html::CHtmlTextRange text_range = selection.create_text_range() ;
	text_range.select() ;
}

void frame_view::handle_enter_edit_mode_match( mem_engine::felix_query *matches )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle enter edit mode.") ;
		return ;
	}
	// Get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	record_pointer rec = get_match_record(matches);

	CViewCollectionWalker walker ;
	tag_name_holder &tags = tag_name_holder::instance() ;

	app_props::props_ptr props = app_props::get_props() ;
	const bool single_screen_matches = !! props->m_view_props.m_data.m_single_screen_matches ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		const wstring id = BSTR2wstring( element->id ) ;

		if ( id.empty() == false )
		{
			// this is in case we're showing all matches on one screen
			if ( single_screen_matches && str::is_int_rep( id ) )
			{
				mem_engine::search_match_ptr match = matches->at( boost::lexical_cast< long >( id ) ) ;
				rec = match->get_record() ;
			}
			else if ( tags.is_query_tag(id) )	
			{
				element->innerHTML = ( matches->get_query_rich() ).c_str() ;
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

void frame_view::handle_enter_edit_mode_concordance( mem_engine::felix_query *matches )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle enter edit mode.") ;
		return ;
	}
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
void frame_view::handle_leave_edit_mode_new( record_pointer &record )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle leave edit mode.") ;
		return ;
	}
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
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		walker.ElementToRecord( make_element_wrapper(element), record ) ;
	} // increment i...

}

bool frame_view::handle_leave_edit_mode_concordance_glossary( MemoryControllerType memories, mem_engine::felix_query *matches )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle leave edit mode (glossary concordance).") ;
		return false;
	}

	// set the actual edit mode
	m_view.put_edit_mode( false ) ;
	if (matches->empty())
	{
		return false ;
	}

	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	// both modes will need these...
	record_pointer	record(new record_local()) ;
	SearchMatchType	match = matches->at(0) ;
	MatchListType	match_list ;

	// the *@!# DHTML will convert our nice relative URLs to absolute ones!
	clean_up_urls(collection) ;

	CViewCollectionWalker walker ;
	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		MSHTML::IHTMLElementPtr element = collection->item( _variant_t( i ), _variant_t(0) ) ;

		const wstring id = BSTR2wstring( element->id ) ;

		if ( ! id.empty() )
		{
			if ( str::is_int_rep( id ) )
			{
				if ( record->is_valid_record() )
				{
					walker.AddRecordToList(record, memories->get_memories(), match, match_list);
				}
				// get current match setting, and erase it from memory
				walker.EraseCurrentRecord(match, matches, id, memories->get_memories());

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
		walker.AddRecordToList(record, memories->get_memories(), match, match_list);
	}

	matches->set_matches( match_list ) ;

	if ( matches->empty() ) 
	{
		return false ;
	}

	return true ;
}
void frame_view::handle_leave_edit_mode_match( MemoryControllerType memories, 
											   mem_engine::felix_query *matches )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle leave edit mode.") ;
		return ;
	}
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
	app_props::props_ptr props = app_props::get_props() ;
	const bool single_screen_matches = !! props->m_view_props.m_data.m_single_screen_matches ;

	// loop through each of the elements
	for ( int i=0 ; i < collection->length ; ++i )
	{
		MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

		const wstring id = BSTR2wstring( element->id ) ;

		if ( ! id.empty() )
		{
			if ( str::is_int_rep( id ) )
			{
				// this is in case we're showing all matches on one screen
				if ( single_screen_matches )
				{
					mem_engine::search_match_ptr match = matches->at( boost::lexical_cast< long >( id ) ) ;
					rec = match->get_record() ;
				}
				int pos = boost::lexical_cast< long >( id ) ;
				if ( false == rec->is_valid_record() ) 
				{
					marked_for_death.push_front(pos) ;
				}

				// set to current record
				rec = matches->at(pos)->get_record() ;
			}
			else if ( tags.is_query_tag(id) )	
			{
				matches->set_query( BSTR2wstring(element->innerHTML) ) ;
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
	// if we are just showing a query with no matches->..
	if ( matches->empty())
	{
		return ;
	}
	// if the source or trans is empty and we have a match, erase it
	std::list< int >::iterator pos ;
	for ( pos = marked_for_death.begin() ; pos != marked_for_death.end() ; ++pos )
	{
		matches->erase_at( *pos ) ;
	}
}

bool frame_view::handle_leave_edit_mode_concordance( MemoryControllerType memories, mem_engine::felix_query *matches )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot handle leave edit mode (glossary concordance).") ;
		return false;
	}
	m_view.put_edit_mode( false ) ;
	if ( memories->empty() ) 
	{
		logging::log_debug("Leaving edit mode. No memories.") ;
		return false ;
	}
	if (matches->empty())
	{
		logging::log_debug("Leaving edit mode. No matches.") ;
		return false ;
	}

	// get the collection of HTML elements in the doc body
	html::collection_ptr collection = get_element_collection() ;

	// the *@!# DHTML will convert our nice relative URLs to absolute ones!
	clean_up_urls(collection) ;

	SearchMatchType	match = matches->at(0);
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
				if ( rec->is_valid_record() )
				{
					walker.AddRecordToList(rec, memories->get_memories(), match, match_list);
				}
				// get current match setting, and erase it from memory
				walker.EraseCurrentRecord(match, matches, id, memories->get_memories());

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
		walker.AddRecordToList(rec, memories->get_memories(), match, match_list);
	}

	matches->set_matches( match_list ) ;

	// make sure we have something to show
	if ( matches->empty() )
	{
		return false ;
	}

	return true ;
}


void frame_view::scroll_element_into_view( const wstring current_id )
{
	if (! m_view.IsWindow())
	{
		logging::log_warn("Browser view not created. Cannot scroll element into view.") ;
		return ;
	}
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


BOOL frame_view::PreTranslateMessage( LPMSG pMsg )
{
	if (! m_view.IsWindow())
	{
		ATLTRACE("Browser view not created. Cannot pre-translate message.\n") ;
		return FALSE;
	}
	return m_view.PreTranslateMessage( pMsg ) ;
}

BOOL frame_view::ProcessWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT &lResult)
{
	if (! m_view.IsWindow())
	{
		ATLTRACE("Browser view not created. Cannot process window message.\n") ;
		return FALSE;
	}
	return m_view.ProcessWindowMessage( hWnd, uMsg, wParam, lParam, lResult ) ;
}
void frame_view::Move( LPRECT rect )
{
	if (! m_view.IsWindow())
	{
		ATLTRACE("Browser view not created. Cannot move window.\n") ;
		return ;
	}
	m_view.MoveWindow( rect, TRUE ) ;
}

html::collection_ptr frame_view::get_element_collection()
{
	html::CHtmlDocument doc = m_view.get_document() ;
	return doc.get_all() ;
}

void frame_view::clean_up_urls( html::collection_ptr &collection )
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

wstring frame_view::get_doc_path( const wstring doc_url )
{
	wstring local_docurl(doc_url) ;
	boost::replace_all(local_docurl, L"\\", L"/") ;

	size_t pos = local_docurl.find_last_of( L'/' )	;
	// we add one to the length to get the slash
	return str::left( local_docurl, pos + 1 ) ;
}

mem_engine::record_pointer frame_view::get_match_record( mem_engine::felix_query *matches )
{
	// The current match...
	if ( false == matches->empty() ) // If we have some matches->..
	{
		// we set it to the current match
		return matches->current()->get_record() ;
	}
	else
	{
		record_pointer record = record_pointer(new mem_engine::record_local) ;
		record->set_source(matches->get_query_rich()) ;
		return record ;
	}
}

void frame_view::destroy()
{
	m_view.DestroyWindow() ;
}