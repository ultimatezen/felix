#pragma once

#include "view_interface.h"
#include "HtmlView.h"
#include "HtmlDocument.h"
#include <vector>
#include "data_converter.h"

class view_interface_ta :
	public view_interface,
	public tag_name_holder
{
	// view
	CHtmlView	m_view ;

public:
	void ensure_document_complete(HACCEL accel, HWND hwnd)
	{
		background_processor backer( 100, accel, hwnd ) ;

		while ( m_view.is_document_complete() == false ) 
		{
			backer.check_iterations() ;
		}
	}

	void ensure_navigation_complete(HACCEL accel, HWND hwnd)
	{
		background_processor backer( 100, accel, hwnd ) ;

		while ( m_view.is_navigation_complete() == false ) 
		{
			backer.check_iterations() ;
		}

	}

	wstring get_selection_text()
	{
		CHtmlSelection selection = m_view.get_selection() ;
		CHtmlTextRange range = selection.create_text_range() ;
		wstring selection_text = range.get_html_text() ;
		range.collapse() ;
		range.select() ;
		return selection_text ;
	}

	void create( HWND parent, HWND &client )
	{
		client = m_view.create( parent ) ;
		ATLASSERT( IsWindow( client ) ) ;
	}
	void set_text( const wstring &text )
	{
		m_view.set_body_text( text ) ;
	}
	void load_resource( LPCTSTR resource_name )
	{
		m_view.load_from_resource( resource_name ) ;
	}
	void do_bold() 
	{
		m_view.do_bold() ;
	}
	void do_underline()
	{
		m_view.do_underline() ;
	}
	void do_italic()
	{
		m_view.do_italic() ;
	}
	void do_delete()
	{
		m_view.OnEditDelete() ;
	}
	void set_bg_color( const wstring &color ) 
	{
		CHtmlDocument doc = m_view.get_document() ;
		doc.set_bg_color( color ) ;
	}
	wstring get_bg_color()
	{
		CHtmlDocument doc = m_view.get_document() ;
		return BSTR2wstring( doc.get_bg_color() ) ;
	}
	document_ptr get_document()
	{
		return m_view.get_document() ;
	}

	bool is_edit_mode()
	{
		return m_view.get_edit_mode() ;
	}
	void put_edit_mode( bool setting )
	{
		m_view.put_edit_mode( setting ) ;
	}

	// edit stuff

	void handle_enter_edit_mode_new_record_glossary( )
	{
		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		// the current match...

		// get the length of the collection
		int len = collection.get_length() ;
		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( id == L"context_tmp"  )
			{
				element.put_inner_html( wstring() ) ;
				element.set_id( L"context" ) ;

				// put the actual browse mode setting
				m_view.put_edit_mode( true ) ;

				return ;
			}
		}

		// put the actual browse mode setting
		m_view.put_edit_mode( true ) ;

		CHtmlSelection selection = m_view.get_selection() ;
		CHtmlTextRange text_range = selection.create_text_range() ;
		text_range.select() ;
	}

	void handle_enter_edit_mode_concordance_glossary( search_query_glossary &matches )
	{
		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		// get the length of the collection
		// both modes will need these...
		record_pointer					rec ;
		search_match_ptr				match ;

		// get the length of the collection
		int len = collection.get_length() ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( id.empty() == false )
			{
				if ( str::is_int_rep( id ) )
				{
					match = matches.at( string2long( id )  ) ;
					rec = match->get_record() ;
				}
				else if ( id == source_tag )	
				{
					element.put_inner_html( rec->get_rich_source() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
				else if ( id == trans_tag )		
				{
					element.put_inner_html( rec->get_rich_trans() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
				else if ( id == context_tag )		
				{
					element.put_inner_html( rec->get_rich_context() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
			}
		}

		// put the actual browse mode setting
		m_view.put_edit_mode( true ) ;
		CHtmlSelection selection = m_view.get_selection() ;
		CHtmlTextRange text_range = selection.create_text_range() ;
		text_range.select() ;

	}


	
	bool handle_leave_edit_mode_new_record_glossary( memory_controller &memories, record_pointer &record )
	{
		// set the actual edit mode
		m_view.put_edit_mode( false ) ;

		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		memory_pointer mem = memories.get_first_memory() ;
		mem->erase( record ) ;

		// the *@!# DHTML will convert our nice relative URLs to absolute ones!
		clean_up_urls(collection) ;

		// get the length of the collection
		int len = collection.get_length() ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( id.empty() == false )
			{
				if ( id == source_tag )	
				{
					record->set_rich_source( BSTR2wstring( element.get_inner_html() ) );
				}
				else if ( id == trans_tag )		
				{
					record->set_rich_trans( BSTR2wstring( element.get_inner_html() ) ) ;

				}
				else if ( id == context_tag )		
				{
					record->set_rich_context( BSTR2wstring( element.get_inner_html() ) ) ;

				}
				else if ( id == reliability_tag ) 
				{
					CComBSTR rel_text = element.get_text() ;
					if ( rel_text.IsEmpty() == false ) 
					{
						size_t reliability = static_cast< size_t >( string2ulong( static_cast< LPCWSTR >( rel_text ) ) )  ;
						record->set_reliability( reliability ) ;
					}
				}
				else if ( id == validated_tag )	
				{
					wstring val ;
					element.get_text( val ) ;

					str::make_lower( val ) ;

					if ( val == L"true" || val == L"yes" )	
					{
						record->set_validated_on() ;
					}
					else
					{
						record->set_validated_off() ;
					}
				}
			} 
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

	bool handle_leave_edit_mode_concordance_glossary( memory_controller &memories, search_query_glossary &matches )
	{

		// set the actual edit mode
		m_view.put_edit_mode( false ) ;

		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		// both modes will need these...
		record_pointer					rec ;
		search_match_ptr				match ;
		std::vector< search_match_ptr > match_list ;

		// the *@!# DHTML will convert our nice relative URLs to absolute ones!
		clean_up_urls(collection) ;

		// get the length of the collection
		int len = collection.get_length() ;

		memory_pointer mem = memories.get_first_memory() ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			TRACE( i ) ;
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( ! id.empty() )
			{
				if ( str::is_int_rep( id ) )
				{
					if ( rec->get_rich_source().empty() == false &&
						rec->get_rich_trans().empty() == false )
					{
						// get the plaintext versions of our entries
						rec->modify() ;
						mem->add_record( rec ) ;

						match->set_record( rec ) ;
						match->set_values_to_record() ;

						// add the match to our match list
						match_list.push_back( match ) ;
					}
					// get current match setting, and erase it from memory
					match = matches.at( string2long( id ) ) ;
					rec = match->get_record() ;
					memories.remove_record( rec, match->get_memory_id() ) ;
				}
				else if ( id == source_tag )	
				{
					rec->set_rich_source( element.get_inner_html() ) ;
				}
				else if ( id == trans_tag )		
				{
					rec->set_rich_trans( element.get_inner_html() ) ;
				}
				else if ( id == context_tag )		
				{
					rec->set_rich_context( element.get_inner_html() ) ;
				}
				else if ( id == reliability_tag ) 
				{
					CComBSTR rel_text = element.get_inner_html() ;
					if ( rel_text.IsEmpty() == false ) 
					{
						size_t reliability = static_cast< size_t >( string2ulong( static_cast< LPCWSTR >( rel_text ) ) )  ;
						rec->set_reliability( reliability ) ;
					}
				}
				else if ( id == validated_tag )	
				{
					wstring val ;
					element.get_text( val ) ;

					str::make_lower( val ) ;

					if ( val == L"true" || val == L"yes" )	
					{
						rec->set_validated_on() ;
					}
					else
					{
						rec->set_validated_off() ;
					}
				}
			} 
		} // increment i...

		if ( rec->get_rich_source().empty() == false &&
			rec->get_rich_trans().empty() == false )
		{
			// get the plaintext versions of our entries
			rec->modify() ;
			mem->add_record( rec ) ;

			match->set_record( rec ) ;

			match->set_values_to_record() ;

			// add the match to our match list
			match_list.push_back( match ) ;
		}

		matches.set_matches( match_list ) ;

		if ( matches.empty() ) 
		{
			return false ;
		}

		return true ;
	}


	
	void handle_enter_edit_mode_register( )
	{
		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		// we need to clear out our background formatting...
		TRACE( m_view.get_body_text() ) ;

		// get the length of the collection
		int len = collection.get_length() ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			TRACE( i ) ;
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( id.empty() == false )
			{
				if ( id == R2W( IDS_GLOSS_SOURCE_TMP_ID ) )	
				{
					element.put_inner_html( wstring() ) ;
					element.set_id( R2W( IDS_GLOSS_SOURCE_ID ) ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
				else if ( id == R2W( IDS_GLOSS_TRANS_TMP_ID ) )	
				{
					element.put_inner_html( wstring() ) ;
					element.set_id( R2W( IDS_GLOSS_TRANS_ID ) ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
			}
		}

		// put the actual browse mode setting
		m_view.put_edit_mode( true ) ;

		CHtmlSelection selection = m_view.get_selection() ;
		CHtmlTextRange text_range = selection.create_text_range() ;
		text_range.select() ;
	}

	void handle_enter_edit_mode_new_record()
	{
		m_view.put_edit_mode( true ) ;

		CHtmlSelection selection = m_view.get_selection() ;
		CHtmlTextRange text_range = selection.create_text_range() ;
		text_range.select() ;
	}

	void handle_enter_edit_mode_match( translation_match_query_trans &matches )
	{
		// Get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		// The current match...
		search_match_ptr match ;
		if ( false == matches.empty() ) // If we have some matches...
		{
			// we set it to the current match
			match = matches.current() ;
		}

		// extract the record from our match
		record_pointer rec = match->get_record() ;

		// get the length of the collection
		int len = collection.get_length() ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			TRACE( i ) ;
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( id.empty() == false )
			{
				if ( id == query_tag )	
				{
					element.put_inner_html( matches.get_rich_query() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
				else if ( id == source_tag )	
				{
					element.put_inner_html( rec->get_rich_source() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
				else if ( id == trans_tag )		
				{
					element.put_inner_html( rec->get_rich_trans() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
				else if ( id == R2WSTR( IDS_DUMMY_SOURCE_ID ) )
				{
					element.put_inner_html( wstring() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
				else if ( id == R2WSTR( IDS_DUMMY_TRANS_ID ) )
				{
					element.put_inner_html( wstring() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
			}
		}

		// put the actual browse mode setting
		m_view.put_edit_mode( true ) ;

		CHtmlSelection selection = m_view.get_selection() ;
		CHtmlTextRange text_range = selection.create_text_range() ;
		text_range.select() ;
	}

	void handle_enter_edit_mode_concordance( search_query_mainframe &matches )
	{
		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		// get the length of the collection
		// both modes will need these...
		record_pointer					rec ;
		search_match_ptr				match ;

		// we need to clear out our background formatting...
		TRACE( m_view.get_body_text() ) ;

		// get the length of the collection
		int len = collection.get_length() ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			TRACE( i ) ;
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			// Here, we are going to paste in our records, because the 
			// HTML in the view may be marked up.
			if ( id.empty() == false )
			{
				if ( str::is_int_rep( id ) )
				{
					match = matches.at( string2long( id ) ) ;
					rec = match->get_record() ;
				}
				else if ( id == source_tag )	
				{
					element.put_inner_html( rec->get_rich_source() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
				else if ( id == trans_tag )		
				{
					element.put_inner_html( rec->get_rich_trans() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
				else if ( id == context_tag )		
				{
					element.put_inner_html( rec->get_rich_context() ) ;
					// we need to re-calculate the length, because we've been mucking with the html
					len = collection.get_length() ;
				}
			}
		}

		// put the actual browse mode setting
		m_view.put_edit_mode( true ) ;

	}


	
	// =========================
	// for leaving edit mode
	// =========================

	void handle_leave_edit_mode_match( memory_controller &memories, translation_match_query_trans &matches )
	{
		// set the actual edit mode
		m_view.put_edit_mode( false ) ;

		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		// the current match...
		search_match_ptr match ;
		if ( false == matches.empty() ) // we have 0 matches, just a query
		{
			// we set it to the current match
			match = matches.current() ;
		}
		// extract the record from the match
		record_pointer rec = match->get_record() ;

		// if this is not an empty record...
		if ( rec->get_rich_source().empty() == false && rec->get_rich_trans().empty() == false )
		{
			// ... then erase it
			memories.remove_record( rec, match->get_memory_id() ) ;
		}

		// the *@!# DHTML will convert our nice relative URLs to absolute ones!
		clean_up_urls(collection) ;

		// get the length of the collection
		int len = collection.get_length() ;

		std::list< int > marked_for_death ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			TRACE( i ) ;
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( ! id.empty() )
			{
				if ( str::is_int_rep( id ) )
				{
					int match_pos = string2long( id ) ;

					if ( false == rec->is_valid_record() ) 
					{
						marked_for_death.push_front( match_pos ) ;
					}

					// get current match setting, and erase it from memory
					match = matches.at( match_pos ) ;
					rec = match->get_record() ;
					memories.remove_record( rec, match->get_memory_id() ) ;
				}
				else if ( id == query_tag )	
				{
					matches.set_rich_query( element.get_inner_html() ) ;
				}
				else if ( id == source_tag )	
				{
					ATLASSERT( matches.empty() == false ) ;

					rec->set_rich_source( element.get_inner_html() ) ;
				}
				else if ( id == trans_tag )		
				{
					ATLASSERT( matches.empty() == false ) ;

					rec->set_rich_trans( element.get_inner_html() ) ;
				}
				else if ( id == R2W( IDS_DUMMY_SOURCE_ID ) )
				{
					ATLASSERT( matches.empty() ) ;
					ATLASSERT( rec->get_rich_source().empty() ) ;

					rec->set_rich_source( element.get_inner_html() ) ;
				}
				else if ( id == R2W( IDS_DUMMY_TRANS_ID ) )
				{
					ATLASSERT( matches.empty() ) ;
					ATLASSERT( rec->get_rich_trans().empty() ) ;

					rec->set_rich_trans( element.get_inner_html() ) ;
				}
				else if ( id == context_tag )		
				{
					rec->set_rich_context( element.get_inner_html() ) ;

				}
				else if ( id == reliability_tag ) 
				{
					CComBSTR rel_text = element.get_text() ;
					if ( rel_text.IsEmpty() == false ) 
					{
						size_t reliability = static_cast< size_t >( string2ulong( static_cast< LPCWSTR >( rel_text ) ) )  ;
						rec->set_reliability( reliability ) ;
					}
				}
				else if ( id == validated_tag )	
				{
					wstring val = element.get_text() ;
					str::make_lower( val ) ;

					if ( val == L"true" || val == L"yes" )		
					{
						rec->set_validated_on() ;
					}
					else
					{
						rec->set_validated_off() ;
					}
				}
				else if ( id != R2WSTR( IDS_REFCOUNT_ID ) 
					&& id != R2WSTR( IDS_CREATED_ID ) 
					&& id != R2WSTR( IDS_MODIFIED_ID ) 
					&& id != R2WSTR( IDS_MEMORY_ID ) 
					&& id != R2WSTR( IDS_RICH_QUERY_ID ) 
					&& id != R2WSTR( IDS_RICH_SOURCE_ID ) 
					)
				{
					CComBSTR tag_name = element.get_tag_name() ;

					wstring tag_str = BSTR2wstring( tag_name ) ;

					if ( str::equal_nocase( tag_str, L"td" ) )
					{
						wstring val = element.get_text( ) ;
						rec->set_item( id, val ) ;
					}
				}
			} 
		} // increment i...

		// ====================
		// show the edited records
		// ====================

		// make sure we have something to show

		// if the source or trans is empty and we have a match, erase it
		std::list< int >::iterator pos ;
		for ( pos = marked_for_death.begin() ; pos != marked_for_death.end() ; ++pos )
		{
			matches.erase_at( *pos ) ;
		}

	}

	bool handle_leave_edit_mode_concordance( memory_controller &memories, search_query_mainframe &matches )
	{
		if ( memories.empty() ) 
		{
			return false ;
		}

		// set the actual edit mode
		m_view.put_edit_mode( false ) ;

		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		// both modes will need these...
		record_pointer					rec ;
		search_match_ptr				match ;
		std::vector< search_match_ptr > match_list ;

		// the *@!# DHTML will convert our nice relative URLs to absolute ones!
		clean_up_urls(collection) ;

		// get the length of the collection
		int len = collection.get_length() ;

		memory_pointer mem = memories.get_first_memory() ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			TRACE( i ) ;
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( ! id.empty() )
			{
				if ( str::is_int_rep( id ) )
				{
					if ( rec->is_valid_record() )
					{
						// get the plaintext versions of our entries
						rec->modify() ;
						mem->add_record( rec ) ;

						match->set_record( rec ) ;
						match->set_values_to_record() ;

						// add the match to our match list
						match_list.push_back( match ) ;
					}
					// get current match setting, and erase it from memory
					match = matches.at( string2long( id ) ) ;
					rec = match->get_record() ;
					memories.remove_record( rec, match->get_memory_id() ) ;
				}
				else if ( id == source_tag )	
				{
					rec->set_rich_source( element.get_inner_html() ) ;
				}
				else if ( id == trans_tag )		
				{
					rec->set_rich_trans( element.get_inner_html() ) ;

				}
				else if ( id == context_tag )		
				{
					rec->set_rich_context( element.get_inner_html() ) ;

				}
				else if ( id == reliability_tag ) 
				{
					CComBSTR rel_text = element.get_text() ;
					if ( rel_text.IsEmpty() == false ) 
					{
						size_t reliability = static_cast< size_t >( string2ulong( static_cast< LPCWSTR >( rel_text ) ) )  ;
						rec->set_reliability( reliability ) ;
					}
				}
				else if ( id == validated_tag )	
				{
					wstring val = id ;
					str::make_lower( val ) ;

					if ( val == L"true" || val == L"yes" )	
					{
						rec->set_validated_on() ;
					}
					else
					{
						rec->set_validated_off() ;
					}
				}
			} 
		} // increment i...

		if ( rec->is_valid_record() )
		{
			// get the plaintext versions of our entries
			rec->modify() ;
			mem->add_record( rec ) ;

			match->set_record( rec ) ;

			match->set_context( rec->get_rich_context() ) ;
			match->set_source( rec->get_rich_source() ) ;
			match->set_trans( rec->get_rich_trans() ) ;

			// add the match to our match list
			match_list.push_back( match ) ;
		}

		matches.set_matches( match_list ) ;

		// make sure we have something to show
		if ( matches.empty() )
		{
			return false ;
		}

		return true ;
	}


	bool handle_leave_edit_mode_register( search_match &glossary_registration_top, search_match glossary_registration_bottom )
	{
		// set the actual edit mode
		m_view.put_edit_mode( false ) ;

		// get the main record
		record_pointer main_record = glossary_registration_top.get_record() ;
		// get the gloss record
		record_pointer gloss_record = glossary_registration_bottom.get_record() ;

		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;
		// the *@!# DHTML will convert our nice relative URLs to absolute ones!
		clean_up_urls( collection ) ;

		// get the length of the collection
		int len = collection.get_length() ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			TRACE( i ) ;
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( ! id.empty() )
			{
				if ( id == R2W( IDS_GLOSS_SOURCE_ID ) )
				{
					gloss_record->set_rich_source( element.get_inner_html() ) ;
				}
				else if ( id == R2W( IDS_GLOSS_TRANS_ID ) )
				{
					gloss_record->set_rich_trans( element.get_inner_html() ) ;

				}
				else if ( id == R2W( IDS_SOURCE_ID ) )	
				{
					main_record->set_rich_source( element.get_inner_html() ) ;
				}
				else if ( id == R2W( IDS_TRANS_ID ) )		
				{
					main_record->set_rich_trans( element.get_inner_html() ) ;

				}
			} // if ( ! id.empty() )
		} // increment i...

		// make sure we have something to show
		if ( false == main_record->is_valid_record() )
		{
			return false ;
		}

		return true ;
	}


	
	void scroll_element_into_view( const wstring &current_id )
	{
		// get the collection of HTML elements in the doc body
		CHtmlElementCollection collection = get_element_collection() ;

		// get the length of the collection
		int len = collection.get_length() ;

		// loop through each of the elements
		for ( int i=0 ; i<len ; ++i )
		{
			CHtmlElement element = collection.item( i ) ;

			wstring id = BSTR2wstring( element.get_id() ) ;

			if ( id.empty() == false )
			{
				if ( current_id == id )	
				{
					element.scroll_into_view( true ) ;
					return ;
				}
			}
		}
	}

	
	BOOL PreTranslateMessage( LPMSG pMsg )
	{
		return m_view.PreTranslateMessage( pMsg ) ;
	}

	BOOL ProcessWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT &lResult)
	{
		return m_view.ProcessWindowMessage( hWnd, uMsg, wParam, lParam, lResult ) ;
	}
	void Move( LPRECT rect )
	{
		m_view.MoveWindow( rect, TRUE ) ;
	}

#ifdef _DEBUG

	void run_test_suite() 
	{
		ATLTRACE( "Running test suite for view_interface_ta...\n" ) ;
	}

#endif // #ifdef _TEST_RIG

	view_interface_ta(void);
	~view_interface_ta(void);

private:
	CComPtr< IHTMLElementCollection > get_element_collection()
	{
		CHtmlDocument doc = m_view.get_document() ;
		return doc.get_all() ;
	}

	void clean_up_urls( CHtmlElementCollection &collection )
	{
		BANNER( "CCommonWindowFunctionality::clean_up_urls" ) ;

		CHtmlDocument doc = m_view.get_document() ;
		wstring doc_url = doc.get_url() ;
		TRACE( doc_url ) ;

		size_t pos = doc_url.find_last_of( L'/' )	;
		// we add one to the length to get the slash
		wstring doc_path = str::left( doc_url, pos + 1 ) ;
		// get the length of the collection
		int len = collection.get_length() ;

		for ( int i=0 ; i<len ; ++i )
		{
			TRACE( i ) ;
			CHtmlElement element = collection.item( i ) ;
			wstring tag_name = element.get_tag_name() ;

			if ( str::equal_nocase( tag_name, L"A" ))
			{
				wstring key = L"HREF" ;
				CComVariant var_href = element.get_attribute( key ) ;
				ATLASSERT( var_href.vt == VT_BSTR ) ;
				wstring href ( (LPCWSTR)var_href.bstrVal, SysStringLen( var_href.bstrVal) )  ;
				if ( str::left( href, doc_path.length() ) == doc_path )
				{
					wstring relative_url = href.substr( doc_path.length() )	;
					element.set_attribute( key, relative_url ) ;
				}
			}
		} // increment i...
	}
};
