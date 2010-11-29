/*!
	@file HtmlDocument.h
	@brief Wrappers for IHTML classes
 */

#pragma once


#ifndef _STLP_NEW_PLATFORM_SDK
	#define _STLP_NEW_PLATFORM_SDK // needed for new SDK
#endif
#pragma warning( disable : 4146 ) // unary minus operator applied to unsigned type, result still unsigned

#import "mshtml.tlb" no_auto_exclude, named_guids, rename( "TranslateAccelerator", "HtmlTranslateAccelerator" )
#include "mshtml.h"

#pragma warning( default : 4146 )
#include "ExDisp.h"


#include "StringEx.h"			// string extensions
#include "StringConversions.h"	// convert to/from wstrings/strings/tstrings
#include "Exceptions.h"			// custom exception classes/support


namespace html
{

typedef CComQIPtr< IWebBrowser2 >				browser_ptr ;
typedef CComQIPtr< MSHTML::IHTMLTxtRange >			text_range_ptr ;
typedef CComPtr< MSHTML::IHTMLElementCollection >	collection_ptr ;
typedef CComPtr< MSHTML::IHTMLSelectionObject >		sel_obj_ptr ;
typedef CComPtr< MSHTML::IHTMLElement >			element_ptr ;
typedef CComQIPtr< MSHTML::IHTMLElement >		qi_element_ptr ;
typedef CComPtr< MSHTML::IHTMLElement3 >		element3_ptr ;
typedef CComPtr< IOleCommandTarget >			command_target_ptr ;
typedef CComPtr< MSHTML::IHTMLDocument2 >		document_ptr ;
typedef CComPtr< MSHTML::IHTMLDocument3 >		document3_ptr ;
typedef CComPtr< MSHTML::IHTMLWindow2 >			window_ptr ;
typedef CComPtr< MSHTML::IHTMLBodyElement >		body_ptr ;

typedef CComPtr< MSHTML::IHTMLEventObj >				event_obj_ptr ;
typedef CComQIPtr< MSHTML::IHTMLControlRange >			control_range_ptr ;


/** Wrapper for a CComQIPtr< IWebBrowser2 >.
 *	class CHtmlWebBrowser
 */
class CHtmlWebBrowser
{
	browser_ptr m_browser ;
public:
	CHtmlWebBrowser( browser_ptr browser ) ;
	bool is_instantiated() ;

	void navigate( const _bstr_t &url ) ;
	void navigate( const wstring &url ) ;
	command_target_ptr get_command_target() ;
} ;

/** The CHtmlEntity class. 
 *	A base class implementing functionality common to the various HTML entity wrappers.
 */
template < class object_type, class entity_type >
class CHtmlEntity
{
protected:
	entity_type m_entity ;
	
public:
	/// bare-bones constructor.
	CHtmlEntity(){}
	
	/// copy constructor.
	CHtmlEntity( const entity_type cpy ) : m_entity( cpy ) {}

	/// assignment operator.
	object_type& operator=( const entity_type cpy )
	{
		m_entity = ( cpy ) ;
		object_type* obj = static_cast< object_type* >(this) ;
		return *obj ;
	}

	/// Tests whether the entity is NULL.
	/// Will not catch a corrupted pointer.
	const bool is_instantiated() const
	{
		return !! m_entity ;
	}
	void uninstantiate() ///< Not normally needed. 
	{
		if ( is_instantiated() ) 
		{
			m_entity.Release() ;
		}
	}
	operator entity_type() { return m_entity ; }
} ;

/** Wrapper for CComPtr< IHTMLElement >.
 * Inherits basic functionality from CHtmlEntity.
 */
class CHtmlElement : public CHtmlEntity< CHtmlElement, element_ptr >
{
	typedef CHtmlEntity< CHtmlElement, element_ptr > base_entity ;

public:
	/** default constructor. */
	CHtmlElement(){}

	/** copy constructor. */
	CHtmlElement( const element_ptr cpy ) : base_entity( cpy ) {}

	/** get the tag attribute value for the key. */
	_variant_t get_attribute( const wstring &key ) ;
	/** Set the tag attribute value for the key. */
	void set_attribute( const wstring &key, const wstring &val ) ;

	/** get the parent element */
	element_ptr get_parent() ;

	/** explicitly retrieve the element pointer. */
	element_ptr get_element() ;
	/** Use this to get us a pointer to an IHTMLElement3. */
	element3_ptr get_element3() ;

	/** Get the value of the ID tag, if any. */
	_bstr_t get_id( ) const ;
	void get_id( wstring &id ) const ;
	void set_id( const wstring &id) ;
	_bstr_t get_tag_name( ) const ;

	void scroll_into_view( const bool top = true ) ;
	//--------
	// outer html
	const _bstr_t get_outer_html() const ;
	void put_outer_html( const wstring &html_text) ;
	void put_outer_html( LPCWSTR html_text) ;

	//--------
	// inner html
	const _bstr_t get_inner_html() const ;
	void put_inner_html( const wstring &html_text) ;
	void put_inner_html( const _bstr_t &html_text ) ;

	 //--------
	// text
	void get_text( _bstr_t &text ) const ;
	const _bstr_t get_text() const ;
	void get_text( wstring &text ) ;
	void put_text( const wstring &text) ;
	void put_text( const _bstr_t &text ) ;
} ;

/** Wrapper for CComQIPtr< IHTMLTxtRange >. 	
 * Inherits basic functionality from CHtmlEntity.
*/
class CHtmlTextRange : public CHtmlEntity< CHtmlTextRange, text_range_ptr >
{
	typedef CHtmlEntity< CHtmlTextRange, text_range_ptr > base_entity ;

public:
	CHtmlTextRange(){}
	CHtmlTextRange( const text_range_ptr cpy ) : base_entity( cpy ) {}

	/** tests whether two ranges are equal.  
	@todo make the method (how) an enum.
	*/
	bool is_equal( const _bstr_t how, const CHtmlTextRange &comp ) const ;
	/** Tests whether another range is within this one. */
	bool is_in_range( const CHtmlTextRange &range ) const ;

	/** Duplicate the text range. */
	text_range_ptr duplicate() ;

	/** Get the parent element of this one */
	element_ptr get_parent() ;
	/** Select this text range in the browser. */
	void select() ;
	/** Sets the end point by the method specified in \c how.   
	@todo make the method (how) an enum.
	*/
	void set_end_point( const _bstr_t &how, const CHtmlTextRange &comp ) ;

	/** Moves the text range so that the start and end positions of the range encompass the text in the given element.  */
	void move_to_element_text( CComPtr< MSHTML::IHTMLElement > &element ) ;

	/** Moves the end of the text range.  
	@todo make the unit an enum.
	*/						
	long move_end( const _bstr_t &unit, const long count ) ;
	/** Moves the start of the text range.  
	@todo make the unit an enum.
	*/						
	long move_start( const _bstr_t &unit, const long count ) ;
	/** Collapses the given text range and moves the empty range by the given number of units.  
	@todo make the unit an enum.
	*/
	long move( const _bstr_t &unit, const long count ) ;

	/** Expands the current selection. 
	@todo make the unit an enum.
	*/						
	bool expand( const _bstr_t &unit ) ;
	/** Collapses the current selection. */
	void collapse( bool start = true ) ;

	/** The range text without tags. */
	_bstr_t get_plain_text() const ;
	/** The range text without tags (more efficient). */
	void get_plain_text( BSTR *text ) const ;
	/** Set plaintext in the range. */
	void put_plain_text( const wstring &text) ;
	/** Set plaintext in the range (more efficient). */
	void put_plain_text( const _bstr_t &text ) ;

	/** Finds text in the text range (more efficient). */						
	bool find_text( const _bstr_t &to_find, long flags = 0, long count = 1 ) ;
	/** Finds text in the text range. */						
	bool find_text( const wstring &to_find, long flags = 0, long count = 1 ) ;

	/** Get the html text. */
	_bstr_t get_html_text() const ;
	/** Get the html text (more efficient). */
	void get_html_text( _bstr_t &text ) const ;

	/** Set html in the range. */
	void put_html_text( const wstring &text ) ;
	/** Set html in the range (more efficient). */
	void put_html_text( const _bstr_t &text ) ;

	/** Scrolls the selection into view */
	void scroll_into_view( bool start = true ) ;

} ;

/** Wrapper for CComPtr< IHTMLElementCollection >.
* Inherits basic functionality from CHtmlEntity.
 */
class CHtmlElementCollection : public CHtmlEntity< CHtmlElementCollection, collection_ptr >
{
	typedef CHtmlEntity< CHtmlElementCollection, collection_ptr > base_entity ;

public:
	CHtmlElementCollection(){}
	CHtmlElementCollection( const collection_ptr cpy ) : base_entity( cpy ) {}

	long get_length() ;
	collection_ptr get_tags( const wstring &tag_name ) ;
	element_ptr item( long index ) ;
	collection_ptr items( const wstring &id ) ;
} ;

/** Wrapper for CComPtr< IHTMLSelectionObject >.
* Inherits basic functionality from CHtmlEntity.
 */
class CHtmlSelection : public CHtmlEntity< CHtmlSelection, sel_obj_ptr >
{
	typedef CHtmlEntity< CHtmlSelection, sel_obj_ptr > base_entity ;

public:
	CHtmlSelection(){}
	CHtmlSelection( const sel_obj_ptr cpy ) : base_entity( cpy ) {}
	// ---------
	// get/set
	// ---------
	_bstr_t get_type() ;
	text_range_ptr create_text_range() ;
} ;

/** Wrapper for CComPtr< IHTMLDocument2 >.
 * Inherits basic functionality from CHtmlEntity.
 */
class CHtmlDocument : public CHtmlEntity< CHtmlDocument, document_ptr >
{
	typedef CHtmlEntity< CHtmlDocument, document_ptr > base_entity ;
public:
	DECLARE_SENSING_VAR ;

	CHtmlDocument(){}
	CHtmlDocument( const document_ptr cpy ) : base_entity( cpy ) {}

	void replace_selection( const wstring &text ) ;

	// create

	element_ptr create_element( const wstring &tag_name ) ;

	// ---------
	// get/set
	// ---------

	void set_charset( const _bstr_t &charset ) ;
	_bstr_t get_charset() const ;

	void set_bg_color( wstring color ) ;
	void set_bg_color( const _variant_t &color ) ;

	_bstr_t get_bg_color() const ;
	_bstr_t get_url() const ;

	collection_ptr get_all( ) ;

	// selection
	sel_obj_ptr get_selection() ;

	// ---------
	// i/o
	void write( const wstring &text ) ;

	// ---------
	// body

	// get_body
	element_ptr get_body() ;

	// ---------
	// title

	// get_title
	_bstr_t get_title() const ;
	// set_title
	void set_title( const wstring &wtitle ) ;
	// browse mode
	void put_browse_mode( const bool mode ) ;
	bool get_browse_mode( ) const ;

};


} ;

