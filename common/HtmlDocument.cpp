/*!
	@file HtmlDocument.cpp
	@brief Implementations of IHTML wrapper classes
	@date 2004/12/15
	@time 22:24:04
	@author Ryan Ginstrom
 */

#include "stdafx.h"
#include "HtmlDocument.h"
#include <stdio.h>
#include "DebugUtilities.h"

typedef CComPtr< IDispatch > dispatch_ptr ;

namespace html
{
	using namespace except ;

	/************************************************************************/
	/* CHtmlWebBrowser                                                      */
	/************************************************************************/

	CHtmlWebBrowser::CHtmlWebBrowser( browser_ptr browser ) : 
		m_browser( browser ) 
	{
	}

	bool CHtmlWebBrowser::is_instantiated() 
	{ 
		return m_browser != NULL ; 
	}

	/**
	*	navigate to the specified URL.
	*/
	void CHtmlWebBrowser::navigate( const _bstr_t &url )
	{
		_variant_t	v_url( url ) ;

		_variant_t	ve1, ve2, ve3, ve4 ; // dummies
		COM_ENFORCE ( m_browser->Navigate2( &v_url, &ve1, &ve2, &ve3, &ve4 ), ( CString("Failed navigate to url: ") + CString( (LPCTSTR)url ) ) ) ;
	}

	/**
	*	a wstring version for convenience.
	*/
	void CHtmlWebBrowser::navigate( const wstring &url )
	{
		_variant_t	v_url( url.c_str() ) ;

		_variant_t	ve1, ve2, ve3, ve4 ; // dummies
		COM_ENFORCE ( m_browser->Navigate2( &v_url, &ve1, &ve2, &ve3, &ve4 ), ( CString("Failed navigate to url: ") + CString( url.c_str() ) ) ) ;
	}	

	command_target_ptr CHtmlWebBrowser::get_command_target()
	{
		command_target_ptr command_target ;
		COM_ENFORCE( m_browser.QueryInterface( &command_target ), CString("Failed to retrieve command target from Web browser object") ) ;

		return command_target ;
	}


	/** If two or more attributes have the same name (differing only in uppercase and lowercase letters),
	this method retrieves values only for the last attribute (the last to be created with this name). 
	All other attributes of the same name are ignored.  
	*/
	_variant_t CHtmlElement::get_attribute( const wstring &key )
	{
		return m_entity->getAttribute( _bstr_t( key.c_str() ), FALSE ) ;
	}

	element_ptr CHtmlElement::get_parent()
	{
		element_ptr e ;
		COM_ENFORCE( m_entity->get_parentElement( &e ), CString("Failed to get parent element") ) ;
		return e ;		
	}

	/** @param key The attribute key
	@param val The attribute value */
	void CHtmlElement::set_attribute( const wstring &key, const wstring &val )
	{
		/**		HRESULT setAttribute(
		//			BSTR strAttributeName,		// String specifying the name of the attribute to set. 
		//			VARIANT AttributeValue,		// VARIANT structure containing a string, number, or Boolean value to assign to the attribute.
		//			VARIANT_BOOL fCaseSensitive	// Boolean value specifying whether to use a case-sensitive search to locate the attribute. If TRUE, the uppercase and lowercase letters in strAttributeName must match exactly those in the attribute name. 
		//			);
		*/
		COM_ENFORCE( m_entity->setAttribute( _bstr_t( key.c_str() ), _variant_t( val.c_str() ), 0 ), 
			CString("MSHTML: Failed to set attribute value") ) ; 

	}

	element_ptr CHtmlElement::get_element()
	{
		return m_entity ;
	}

	element3_ptr CHtmlElement::get_element3()
	{
		qi_element_ptr qi( m_entity ) ;

		element3_ptr e3 ;
		COM_ENFORCE( qi.QueryInterface( &e3 ), CString("Call to QueryInterface of IHtmlElement failed") ) ;

		return e3 ;
	}

	_bstr_t CHtmlElement::get_id( ) const
	{
		_bstr_t id ;
		COM_ENFORCE( m_entity->get_id( id.GetAddress() ), CString("Failed to get id of HTML element") ) ;
		return id ;
	}

	/** provided for convenience */
	void CHtmlElement::get_id(wstring &id) const
	{
		_bstr_t text = get_id() ;
		id = BSTR2wstring( text ) ;
	}

	/** provided for convenience */
	void CHtmlElement::set_id(const wstring &id)
	{
		_bstr_t bid( id.c_str() ) ;
		COM_ENFORCE( m_entity->put_id( bid ), CString("Failed to set id of HTML element") ) ;
	}

	_bstr_t CHtmlElement::get_tag_name( ) const
	{
		_bstr_t tag_name ;
		COM_ENFORCE( m_entity->get_tagName( tag_name.GetAddress() ), CString("Failed to get tag name of HTML element") ) ;
		return tag_name ;
	}


	/** @param top Whether to place the element at the top of the window.
	If true, the method scrolls the object so that its top is visible at the top of the window. 
	If false, the bottom of the object is visible at the bottom of the window. 
	If no value is given, the object scrolls to the top by default. 
	*/
	void CHtmlElement::scroll_into_view(const bool top)
	{
		_variant_t is_top( top ? VARIANT_TRUE : VARIANT_FALSE ) ;
		COM_ENFORCE( m_entity->scrollIntoView( is_top ), CString("Failed to scroll element into view") ) ;
	}
	//--------
	// outer html

	// get_outer_html
	const _bstr_t CHtmlElement::get_outer_html() const
	{
		_bstr_t body_text ;
		COM_ENFORCE( m_entity->get_outerHTML( body_text.GetAddress() ), CString("Failed to retrieve document body html") ) ;

		return body_text ;
	}
	// put_outer_html
	void CHtmlElement::put_outer_html( const wstring &html_text)
	{
		_bstr_t body_text( html_text.c_str() ) ;
		COM_ENFORCE( m_entity->put_outerHTML( body_text ), CString("Failed to set document body html") ) ;
	}
	// put_outer_html
	void CHtmlElement::put_outer_html( LPCWSTR html_text)
	{
		_bstr_t body_text( html_text ) ;
		COM_ENFORCE( m_entity->put_outerHTML( body_text ), CString("Failed to set document body html") ) ;
	}
	//--------
	// inner html

	// get_inner_html
	const _bstr_t CHtmlElement::get_inner_html() const
	{
		_bstr_t body_text ;
		COM_ENFORCE( m_entity->get_innerHTML( body_text.GetAddress() ), CString("Failed to retrieve document body html") ) ;

		return body_text ;
	}
	// put_inner_html
	void CHtmlElement::put_inner_html( const wstring &html_text)
	{
		_bstr_t body_text( html_text.c_str() ) ;

		put_inner_html( body_text )	;
	}
	void CHtmlElement::put_inner_html( const _bstr_t &html_text )
	{
		TRACE_WINERR( ::GetLastError() ) ;
		CLEAR_WINERRORS ;
		/** will give us an ERROR_FILE_NOT_FOUND (2) error if we load into a blank page... */
		COM_ENFORCE( m_entity->put_innerHTML( html_text ), CString("Failed to set document body html") ) ; 
	}

	//--------
	// text

	// get_text
	void CHtmlElement::get_text( _bstr_t &text ) const
	{
		COM_ENFORCE( m_entity->get_innerText( text.GetAddress() ), CString("Failed to retrieve document body text") ) ;
	}
	const _bstr_t CHtmlElement::get_text() const
	{
		_bstr_t text ;
		get_text( text ) ;
		return text ;
	}
	void CHtmlElement::get_text( wstring &text )
	{
		text = BSTR2wstring( get_text() ) ;
	}

	// put_text
	void CHtmlElement::put_text( const wstring &text)
	{
		put_text( _bstr_t( text.c_str() ) ) ;
	}

	void CHtmlElement::put_text( const _bstr_t &text )
	{
		COM_ENFORCE( m_entity->put_innerText( text ), CString("Failed to set document body text") ) ;
	}


	// =======================
	// class CHtmlTextRange  
	// =======================

	/** 
	@param how Describes the end point to compare. Can be one of these values: 
	* StartToEnd   
	* StartToStart   
	* EndToStart   
	* EndToEnd
	@param comp Text range object specifying the range from which the source end point is to be taken.  
	*/
	bool CHtmlTextRange::is_equal( const _bstr_t how, const CHtmlTextRange &comp ) const
	{
		long retval = 0 ;
		COM_ENFORCE( m_entity->raw_compareEndPoints( how, comp.m_entity, &retval ), CString("Failed to compare text ranges") ) ;

		return retval == 0 ;
	}

	/** 
	@param range The text range to test 
	*/
	bool CHtmlTextRange::is_in_range( const CHtmlTextRange &range ) const
	{
		VARIANT_BOOL retval ;
		COM_ENFORCE( m_entity->raw_inRange( range.m_entity, &retval ), CString("Failed to compare text ranges") ) ;
		return retval != VARIANT_FALSE ;

	}

	/** @return text_range_ptr The duplicate range */
	text_range_ptr CHtmlTextRange::duplicate()
	{
		text_range_ptr copy ;
		COM_ENFORCE( m_entity->raw_duplicate( &copy ), CString("Failed to duplicate text range") ) ;
		return copy ;
	}

	element_ptr CHtmlTextRange::get_parent()
	{
		element_ptr element ;
		COM_ENFORCE( m_entity->raw_parentElement( &element ), CString("Failed to get parent element of text range") ) ;
		return element ;
	}
	void CHtmlTextRange::select()
	{
		COM_ENFORCE( m_entity->select(), CString("Failed to select text range") ) ;
	}

	/** 
	@param how Describes the end point to compare. Can be one of these values: 
	* StartToEnd   
	* StartToStart   
	* EndToStart   
	* EndToEnd
	@param comp Text range object specifying the range from which the source end point is to be taken.  
	*/
	void CHtmlTextRange::set_end_point( const _bstr_t &how, const CHtmlTextRange &comp )
	{
		COM_ENFORCE( m_entity->raw_setEndPoint( how, comp.m_entity ), CString("Failed to move end of text range") ) ;
	}

	/// This feature might not be available on non-Win32 platforms. 
	/// See article Q172976 in the Microsoft Knowledge Base for the latest 
	/// information on Internet Explorer cross-platform compatibility. 
	void CHtmlTextRange::move_to_element_text( element_ptr &element )
	{
		COM_ENFORCE( m_entity->moveToElementText( element ), CString("Failed to move text range to element") ) ;		
	}

	/**
	@param unit The unit by which to move. possible values:
	* Character
	* Word
	* Sentence
	* Textedit
	@param count number of units to move (pos or negative allowed)
	*/						
	long CHtmlTextRange::move_end( const _bstr_t &unit, const long count )
	{
		long actual_count(0) ;

		COM_ENFORCE( m_entity->raw_moveEnd( unit, count, &actual_count ), CString("Failed to move end of text range") ) ;

		return actual_count ;
	}

	/**
	@param unit The unit by which to move. possible values:
	* Character
	* Word
	* Sentence
	* Textedit
	@param count number of units to move (pos or negative allowed)
	*/						
	long CHtmlTextRange::move_start( const _bstr_t &unit, const long count )
	{
		long actual_count(0) ;

		COM_ENFORCE( m_entity->raw_moveStart( unit, count, &actual_count ), CString("Failed to move start of text range") ) ;

		return actual_count ;
	}

	/**
	@param unit The unit by which to move. possible values:<BR>
	<CODE>Character</CODE>  Moves one or more characters. <BR> 
	<CODE>Word</CODE>  Moves one or more words. 
	A word is a collection of characters terminated by a space or other white space character.  <BR>
	<CODE>Sentence</CODE>  Moves one or more sentences. 
	A sentence is a collection of words terminated by a punctuation character, such as a period.  <BR>
	<CODE>Textedit</CODE>  Moves to the start or end of the original range.  <BR>
	@param count number of units to move (pos or negative allowed)
	*/						
	long CHtmlTextRange::move( const _bstr_t &unit, const long count )
	{
		long actual_count(0) ;
		COM_ENFORCE( m_entity->raw_move( unit, count, &actual_count ), CString("Failed to move selection range") ) ;
		return actual_count ;
	}

	/**
	@param unit The unit by which to move. possible values:
	* Character
	* Word
	* Sentence
	* Textedit
	*/			
	bool CHtmlTextRange::expand( const _bstr_t &unit )
	{
		VARIANT_BOOL success = VARIANT_FALSE ;
		COM_ENFORCE( m_entity->raw_expand( unit, &success ), CString("Failed to expand text range") ) ;
		return ( success != VARIANT_FALSE ) ;
	}

	/// @param start
	/// true to collapse to the beginning, false to collapse to the end
	void CHtmlTextRange::collapse( bool start /* = true */ )
	{
		ATLASSERT( is_instantiated() ) ;
		COM_ENFORCE( m_entity->collapse( ( start ? VARIANT_TRUE : VARIANT_FALSE ) ), CString("Failed to collapse text range") ) ;
	}

	// ---------
	// get/set
	// ---------
	_bstr_t CHtmlTextRange::get_plain_text() const
	{
		// get the type
		_bstr_t text ;
		get_plain_text( text.GetAddress() ) ;
		return text ;
	}
	void CHtmlTextRange::get_plain_text( BSTR *text ) const
	{
		COM_ENFORCE( m_entity->get_text( text ), CString("Failed to get plain text for range") );
	}

	// Function name	: put_plain_text
	void CHtmlTextRange::put_plain_text( const wstring &text)
	{
		ATLASSERT( text.length() == wcslen( text.c_str() ) ) ; 
		ATLASSERT( text.find_first_of( wchar_t(0) ) == wstring::npos ) ;
		// get the type
		_bstr_t btext( text.c_str() ) ;

		put_plain_text( btext ) ;
	}

	void CHtmlTextRange::put_plain_text( const _bstr_t &text )
	{
		ATLASSERT( is_instantiated() ) ;
		COM_ENFORCE( m_entity->put_text( text ), CString("Failed to set plaintext") );
	}

	/** 
	@param to_find [in] specifies the text to find.
	@param flags [in] specifies one or more of the following flags to indicate the type of search: 
	0	Default. Match partial words.
	1	Match backwards.
	2	Match whole words only.
	4	Match case.
	131072	Match bytes.
	536870912	Match diacritical marks.
	1073741824	Match Kashida character.
	2147483648	Match AlefHamza character.
	@param count specifies the number of characters to search from the starting point of the range. 
	A positive integer indicates a forward search; 
	a negative integer indicates a backward search.
	@return whether the text was found
	true: The search text was found.
	false: The search text was not found.
	*/						
	bool CHtmlTextRange::find_text( const _bstr_t &to_find, long flags /* = 0 */, long count /* = 1 */ )
	{
		ATLASSERT( is_instantiated() ) ;

		if ( to_find.length() == 0 ) 
		{
			return false ;
		}

		VARIANT_BOOL result ;
		COM_ENFORCE( m_entity->raw_findText( to_find, count, flags, &result ), CString("Call to find text failed") ) ;

		return ( result != VARIANT_FALSE ) ;
	}
	/** wstring interface for convenience.
	* @sa CHtmlTextRange::find_text( const _bstr_t &, long, long )
	*/
	bool CHtmlTextRange::find_text( const wstring &to_find, long flags /* = 0 */, long count /* = 1 */ )
	{
		_bstr_t bto_find( to_find.c_str() ) ;

		return find_text( bto_find, flags, count ) ;
	}

	/** @sa CHtmlTextRange::get_html_text( _bstr_t &text ) */
	_bstr_t CHtmlTextRange::get_html_text() const
	{
		// get the type
		_bstr_t text ;
		get_html_text( text ) ;

		return text ;
	}

	void CHtmlTextRange::get_html_text( _bstr_t &text ) const
	{
		ATLASSERT( is_instantiated() ) ;
		COM_ENFORCE( m_entity->get_htmlText( text.GetAddress() ), CString("Failed to get text-range HTML") );
	}

	// Function name	: put_html_text
	void CHtmlTextRange::put_html_text( const wstring &text )
	{
		// no embedded nulls, please!
		ATLASSERT( text.length() == wcslen( text.c_str() ) ) ; 
		ATLASSERT( text.find_first_of( wchar_t(0) ) == wstring::npos ) ;

		// get the type
		_bstr_t btext( text.c_str() ) ;
		put_html_text( btext ) ;
	}

	void CHtmlTextRange::put_html_text( const _bstr_t &text )
	{
		ATLASSERT( is_instantiated() ) ;
		COM_ENFORCE( m_entity->pasteHTML( text ), CString("Failed to get range type") );
	}

	// Function name	: scroll_into_view
	void CHtmlTextRange::scroll_into_view( bool start /* = true */ )
	{
		ATLASSERT( is_instantiated() ) ;
		COM_ENFORCE( m_entity->scrollIntoView(  ( start ? VARIANT_TRUE : VARIANT_FALSE ) ), CString("Failed to scroll selection into view") ) ;
	}


	// =======================
	// class CHtmlElementCollection  
	// =======================

	// ============
	// get/set
	// ============

	long CHtmlElementCollection::get_length()
	{
		ATLASSERT( is_instantiated() ) ;

		long length ;
		COM_ENFORCE( m_entity->get_length( &length ), CString("Failed to get length of HTML Element collection") ) ;
		return length ;
	}
	collection_ptr CHtmlElementCollection::get_tags( const wstring &tag_name )
	{
		ATLASSERT( is_instantiated() ) ;

		dispatch_ptr coll_disp ;
		COM_ENFORCE( m_entity->raw_tags( _variant_t( tag_name.c_str() ), &coll_disp ), CString("Failed to get HTML Elements from collection with tag ") + CString( tag_name.c_str() ) ) ;
		collection_ptr collection ;
		COM_ENFORCE( coll_disp.QueryInterface( &collection ), CString("Cannot retrieve HTML ELement Collection from dispatch interface") )  ;
		return collection ;
	}
	element_ptr CHtmlElementCollection::item( long index )
	{
		ATLASSERT( is_instantiated() ) ;

		dispatch_ptr coll_disp ;
		COM_ENFORCE( m_entity->raw_item( _variant_t( index ), _variant_t( 0 ), &coll_disp ), CString("Failed to get HTML Element at position ") + int2tstring( index ).c_str() ) ;
		CComQIPtr< MSHTML::IHTMLElement > element( coll_disp ) ;
		return element ;
	}
	collection_ptr CHtmlElementCollection::items( const wstring &id )
	{
		ATLASSERT( is_instantiated() ) ;

		dispatch_ptr coll_disp ;
		COM_ENFORCE( m_entity->raw_item( _variant_t( id.c_str() ), _variant_t( 0 ), &coll_disp), CString("Failed to get HTML elements with id ") + CString( id.c_str() ) ) ;
		// there were no elements with that id...
		if ( coll_disp == NULL ) return NULL ;

		collection_ptr collection ;
		COM_ENFORCE( coll_disp.QueryInterface( &collection ), CString("Cannot retrieve HTML ELement Collection from dispatch interface") )  ;
		return collection ;
	}

	// =======================
	// class CHtmlSelection  
	// =======================

	// ---------
	// get/set
	// ---------
	_bstr_t CHtmlSelection::get_type()
	{
		ATLASSERT( is_instantiated() ) ;

		// get the type
		_bstr_t type ;
		COM_ENFORCE( m_entity->get_type( type.GetAddress() ), CString("Failed to get selection type") );

		return type ;
	}
	text_range_ptr CHtmlSelection::create_text_range()
	{
		ATLASSERT( get_type() == _bstr_t( L"Text" ) || get_type() == _bstr_t( L"None" ) ) ;
		ATLASSERT( is_instantiated() ) ;

		// create the range
		dispatch_ptr disp ;
		COM_ENFORCE( m_entity->raw_createRange(&disp), CString("Failed to create range") );
		// get a text range from it
		text_range_ptr spText( disp );
		if ( spText==NULL ) throw CComException( CString("Failed to create text range"), E_NOINTERFACE );

		return spText ;
	}


	// =======================
	// class CHtmlDocument  
	// =======================

	// create

	element_ptr CHtmlDocument::create_element( const wstring &tag_name )
	{

		ATLASSERT( is_instantiated() ) ;

		element_ptr element ;
		COM_ENFORCE( m_entity->raw_createElement( string2BSTR( tag_name ), &element ), CString("Failed to create element with tag ") + CString( tag_name.c_str() ) ) ;
		return element ;
	}

	// ---------
	// get/set
	// ---------



	// Function name	: set_charset
	// Description	    : 
	// Return type		: void 
	// Argument         : const _bstr_t &charset
	void CHtmlDocument::set_charset( const _bstr_t &charset )
	{

		ATLASSERT( is_instantiated() ) ;

		COM_ENFORCE( m_entity->put_charset( charset ), CString("Failed to set charset of HTML document") ) ;
	}

	// Function name	: get_charset
	// Description	    : 
	// Return type		: wstring 
	_bstr_t CHtmlDocument::get_charset() const
	{

		ATLASSERT( is_instantiated() ) ;

		_bstr_t charset ;
		COM_ENFORCE( m_entity->get_charset( charset.GetAddress() ), CString("Failed to get charset of HTML document") ) ;

		return charset ;

	}

	// Function name	: set_bg_color
	void CHtmlDocument::set_bg_color( const wstring color )
	{
		// no embedded nulls, please!
		ATLASSERT( color.length() == wcslen( color.c_str() ) ) ; 
		ATLASSERT( color.find_first_of( wchar_t(0) ) == wstring::npos ) ;

		SENSE("set_bg_color") ;
		SENSE(string2string(color)) ;
#ifdef UNIT_TEST
		return ;
#else
		m_entity->bgColor = _variant_t(color.c_str()) ;
#endif
	}
	void CHtmlDocument::set_bg_color( const _variant_t &color )
	{
		ATLASSERT( is_instantiated() ) ;

		m_entity->bgColor = color ;
	}

	// Function name	: get_bg_color
	_bstr_t CHtmlDocument::get_bg_color() const
	{

		ATLASSERT( is_instantiated() ) ;

		return _bstr_t( m_entity->bgColor.bstrVal ) ;
	}

	// Function name	: get_url
	_bstr_t CHtmlDocument::get_url() const
	{

		ATLASSERT( is_instantiated() ) ;

		_bstr_t url ;
		COM_ENFORCE( m_entity->get_url( url.GetAddress() ), CString("Failed to retrieve document url") ) ;
		return url ;
	}

	// all elements in collection
	collection_ptr CHtmlDocument::get_all( )
	{

		ATLASSERT( is_instantiated() ) ;

		collection_ptr collection ;
		COM_ENFORCE( m_entity->get_all( &collection ), CString("Failed to get collection of HTML elements from HTML document") ) ;
		return collection ;
	}	

	// selection
	sel_obj_ptr CHtmlDocument::get_selection()
	{

		ATLASSERT( is_instantiated() ) ;

		sel_obj_ptr selection ;
		COM_ENFORCE( m_entity->get_selection( &selection ), CString("Failed to retrieve selected element from HTML document object model") ) ;
		return selection ;
	}

	// ---------
	// i/o
	void CHtmlDocument::write( const wstring &text )
	{

		ATLASSERT( is_instantiated() ) ;

		// create a safe array
		SAFEARRAY *sfArray;

		// Creates a new one-dimensional array
		sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);

		TRUE_ENFORCE( NULL != sfArray, CString("Failed to create SafeArray") ) ;
		VARIANT *param ;
		COM_ENFORCE( SafeArrayAccessData(sfArray, out_ptr< VARIANT >( &param ) ), CString("Failed to access SafeArray data") ) ;
		param->vt = VT_BSTR;
		_bstr_t var( text.c_str() ) ;
		param->bstrVal = var;
		COM_ENFORCE( SafeArrayUnaccessData(sfArray), CString("Failed to unaccess SafeArray data") ) ;

		COM_ENFORCE( m_entity->write( sfArray ), CString("Failed to write text to HTML document") ) ;

		if (sfArray != NULL)
		{
			SafeArrayDestroy(sfArray);
		}
	}

	// ---------
	// body

	// get_body
	element_ptr CHtmlDocument::get_body()
	{
		ATLASSERT( is_instantiated() ) ;

		element_ptr elem ;
		COM_ENFORCE( m_entity->get_body( &elem ), CString("Failed to retrieve document body") ) ;
		return elem ;
	}
	// ---------
	// title

	// get_title
	_bstr_t CHtmlDocument::get_title() const
	{
		ATLASSERT( is_instantiated() ) ;

		_bstr_t title ;
		COM_ENFORCE( m_entity->get_title( title.GetAddress() ), CString("Failed to retrieve document title") ) ;
		return title ;
	}
	// set_title
	void CHtmlDocument::set_title( const wstring &wtitle )
	{
		ATLASSERT( is_instantiated() ) ;

		// no embedded nulls, please!
		ATLASSERT( wtitle.length() == wcslen( wtitle.c_str() ) ) ; 
		ATLASSERT( wtitle.find_first_of( wchar_t(0) ) == wstring::npos ) ;

		_bstr_t btitle( wtitle.c_str() ) ;
		COM_ENFORCE( m_entity->put_title( btitle ), CString("Failed to retrieve document title") ) ;

		ATLASSERT( wtitle.c_str() == get_title() ) ;
	}
	// browse mode
	void CHtmlDocument::put_browse_mode( const bool mode )
	{
		ATLASSERT( is_instantiated() ) ;

		if ( mode )
		{
			COM_ENFORCE( m_entity->put_designMode( L"On" ), CString("Failed to enable design mode") ) ;
		}
		else
		{
			COM_ENFORCE( m_entity->put_designMode( L"Off" ), CString("Failed to disable design mode") ) ;
		}
	}
	bool CHtmlDocument::get_browse_mode( ) const
	{
		ATLASSERT( is_instantiated() ) ;

		_bstr_t mode ;
		COM_ENFORCE( m_entity->get_designMode( mode.GetAddress() ), CString("Failed to retrieve design mode setting") ) ;

		return ( mode == _bstr_t( L"On" ) ) ;
	}

	void CHtmlDocument::replace_selection(const wstring &text)
	{
		ATLASSERT( is_instantiated() ) ;

		// get the current selection
		CHtmlSelection selection = get_selection() ;
		CHtmlTextRange range = selection.create_text_range() ;

		range.put_html_text( text ) ;

	}

}
