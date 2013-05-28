/*!
	@file WordAutomationObject.cpp
	@brief Header for Word automation object wrappers
	@date 2004/12/17
	Time: 8:46:37
	@author Ryan Ginstrom
 */

#include "stdafx.h"
#include "WordAutomationObject.h"
#include "Exceptions.h"
#include "StringEx.h"
#include "StringConversions.h"
#include "ComUtilities.h"
#include <boost/static_assert.hpp>
#include <map>

#define WORD_TRY try {

#define WORD_CATCH } catch(...){ if( m_member ) { m_member = NULL ; } throw ; }


/** Keeps a static symbol_converter so we don't have to initialize the map lots of times */
wchar_t symbol2unicode( _variant_t &unicode_symbol )
{
	static symbol_converter converter ;

	if ( unicode_symbol.vt != VT_I4 )
	{
		unicode_symbol.ChangeType( VT_I4 ) ;
	}
	
	return converter.convert_symbol( unicode_symbol.lVal ) ;
}

// -------------------------------
// WordApplication functions
// -------------------------------

bool WordApplication::is_active()
{
	WORD_TRY

		if ( ! is_instantiated() )
		{
			deactivate() ;
			return false ;
		}
		
		if ( ! m_active )
		{
			return false ;
		}

		HWND active_window = ::GetFocus() ;
		if ( active_window != m_active_window )
		{
			return false ;
		}

	WORD_CATCH	

	return true ;
}

void WordApplication::set_app( word_app_ptr word_app )
{
	WORD_TRY

		// store our pointer back to Word
		m_member = word_app ;

	WORD_CATCH 
}

bool WordApplication::selection_is_empty()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
		
		WordSelection selection = get_selection() ;

		return selection.get_start() == selection.get_end() ;

	WORD_CATCH 
}

word_sel_ptr WordApplication::get_selection()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
	
		word_sel_ptr selection ;
		WORD_ENFORCE( m_member->get_Selection( &selection ), ("Failed to retrieve Selection property") ) ;
		ATLASSERT( selection ) ;

		return selection ;
	
	WORD_CATCH 
}

void WordApplication::activate()
{ 
		m_active = true ;
		m_active_window = ::GetFocus() ;
}


void WordApplication::deactivate()
{ 
		m_active = false ;
		m_active_window = NULL ;
}


// -------------------------------
// WordSelection functions
// -------------------------------

long WordSelection::get_start()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;

		long start = 0 ;
		WORD_ENFORCE( m_member->get_Start( &start), ("Failed to retrieve Start property") ) ;
		ATLASSERT( start >= 0 ) ;

		return start ;

	WORD_CATCH 
}

void WordSelection::set_start(long new_start)
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
		ATLASSERT( new_start >= 0 ) ;
		
		WORD_ENFORCE( m_member->put_Start( new_start), ("Failed to set Start property") ) ;

	WORD_CATCH 
}

long WordSelection::get_story_length()
{ 
	WORD_TRY
		ATLASSERT( is_instantiated( ) ) ;
		
		long story_len = 0 ;
		WORD_ENFORCE( m_member->get_StoryLength( &story_len ), ("Failed to get story length") ) ;
		ATLASSERT( story_len >= 0 ) ;
		return story_len ;

	WORD_CATCH 
}

long WordSelection::get_end()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
	
		long sel_end = 0 ;
		WORD_ENFORCE( m_member->get_End( &sel_end ), ("Failed to get selection endpoint") ) ;
		ATLASSERT( sel_end >= 0 ) ;
		return sel_end ;

	WORD_CATCH 
}


_bstr_t WordSelection::get_text()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
		
		return m_member->Text ;

	WORD_CATCH 
}


void WordSelection::type_paragraph()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
		
		WORD_ENFORCE( m_member->TypeParagraph( ), ("Call to TypeParagraph() failed.") ) ;

	WORD_CATCH 
}

void WordSelection::type_text(const _bstr_t &text)
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;

		WORD_ENFORCE( m_member->TypeText( _bstr_t( (LPCWSTR)text) ), ("Call to TypeText() failed.") ) ;
	
	WORD_CATCH 
}

word_chars_ptr WordSelection::get_characters()
{ 
	WORD_TRY
		ATLASSERT( is_instantiated( ) ) ;
		
		word_chars_ptr chars ;
		WORD_ENFORCE( m_member->get_Characters( &chars ), ("Call to Characters failed.") ) ;
		ATLASSERT( chars ) ;
		return chars ;

	WORD_CATCH 
}

word_range_ptr WordSelection::get_range()
{ 
	WORD_TRY
		
		ATLASSERT( is_instantiated( ) ) ;
		
		word_range_ptr range ;
		WORD_ENFORCE( m_member->get_Range( &range ), ("Failed to retrieve Range member") ) ;
		ATLASSERT( range ) ;
		return range ;

	WORD_CATCH 
}


long WordSelection::extend_end(MSWord::WdUnits unit, long count)
{ 
	WORD_TRY

		ATLASSERT( is_instantiated() ) ;
		
		_variant_t extend_unit( unit ) ;
		_variant_t extend_count( count ) ;
		_variant_t extend_flag( MSWord::wdExtend ) ;
		long prop = 0 ;
		WORD_ENFORCE( m_member->raw_MoveRight( &extend_unit, &extend_count, &extend_flag, &prop ), ("Call to MoveRight() failed") ) ;
		return prop ;

	WORD_CATCH 
}

void WordSelection::put_start( const long start )
{ 
	WORD_TRY
		
		ATLASSERT( is_instantiated() ) ;
		ATLASSERT( start >= 0 ) ;
		
		WORD_ENFORCE( m_member->put_Start( start ), ("Failed to set selection start") ) ;

	WORD_CATCH 
}

// -------------------------------
// WordCharacters functions
// -------------------------------


long WordCharacters::get_count()
{ 
	WORD_TRY
		
		ATLASSERT( is_instantiated( ) ) ;
		
		long count = 0 ;
		WORD_ENFORCE( m_member->get_Count( &count ), ("Failed to retrieve Count property") ) ;
		ATLASSERT( count >= 0 ) ;
		return count ;

	WORD_CATCH 
}

// 1-based index!
word_range_ptr WordCharacters::item(long index)
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
		ATLASSERT( index >= 1 ) ;
		ATLASSERT( index <= get_count() ) ;

		word_range_ptr range ;
		WORD_ENFORCE( m_member->raw_Item( index, &range ), ("Failed to retrieve Item member") ) ;
		ATLASSERT( range ) ;

		return range ;

	WORD_CATCH 
}

// -------------------------------
// WordWords functions
// -------------------------------


long WordWords::get_count()
{ 
	WORD_TRY
	
		ATLASSERT( is_instantiated( ) ) ;

		long count = 0 ;
		WORD_ENFORCE( m_member->get_Count( &count ), ("Failed to retrieve Count property") ) ;
		ATLASSERT( count >= 0 ) ;
		return count ;

	WORD_CATCH 
}

// 1-based index!
word_range_ptr WordWords::item(long index)
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
		ATLASSERT( index >= 1 ) ;
		ATLASSERT( index <= get_count() ) ;

		word_range_ptr range ;
		WORD_ENFORCE( m_member->raw_Item( index, &range ), ("Failed to retrieve Item member (Words)") ) ;
		ATLASSERT( range ) ;

		return range ;

	WORD_CATCH 
}



// -------------------------------
// WordRange functions
// -------------------------------

_bstr_t WordRange::get_text()
{ 
	WORD_TRY
		
		ATLASSERT( is_instantiated( ) ) ;
		
		return m_member->Text ;

	WORD_CATCH 
}

_bstr_t WordRange::get_unicode_char()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
		
		WORD_ENFORCE( m_member->Select( ), ("Call to Select() method failed.") ) ;
		
		word_app_ptr app ;
		WORD_ENFORCE( m_member->get_Application (&app), ("Failed to get Word application") )  ;
		ATLASSERT( app ) ;

		CComPtr< MSWord::Dialogs > dialogs ;
		WORD_ENFORCE( app->get_Dialogs( &dialogs ), ("Failed to get Word dialogs" ) ) ;
		ATLASSERT( dialogs ) ;

		CComPtr< MSWord::Dialog > insert_symbol_dlg ;
		WORD_ENFORCE( dialogs->raw_Item( MSWord::wdDialogInsertSymbol, &insert_symbol_dlg ), ("Failed to get Word dialogs" ) ) ;

		// Line 3: unicode_symbol = dlg . charnum 
		CDispatchWrapper wrapper( CComPtr<IDispatch>( static_cast< IDispatch* >( insert_symbol_dlg ) ) ) ;
		_variant_t unicode_symbol = wrapper.prop_get( L"charnum" ) ;

		wchar_t symbol[2] = { 0 } ;
		symbol[0] = symbol2unicode( unicode_symbol ) ;

		_bstr_t text(symbol) ;
		return text ;

	WORD_CATCH 
}

word_font_ptr WordRange::get_font()
{ 
	WORD_TRY
		ATLASSERT( is_instantiated( ) ) ;
		
		word_font_ptr font ;

		WORD_ENFORCE( m_member->get_Font( &font), ("Failed to retrieve Font property") ) ;
		ATLASSERT( font ) ;

		return font ;
	WORD_CATCH 
}


void WordRange::extend_to_story_end()
{ 
	WORD_TRY
		ATLASSERT( is_instantiated() ) ;
		
		_variant_t unit( MSWord::wdStory ) ;
		_variant_t count_sentinel( (long)10000 ) ;
		long prop = 0 ;
		WORD_ENFORCE( m_member->raw_MoveEnd( &unit, &count_sentinel, &prop ), ("Failed to extend Word range") ) ;
	WORD_CATCH 
}



void WordRange::collapse_end()
{ 
	WORD_TRY
		ATLASSERT( is_instantiated() ) ;

		_variant_t direction = (long)MSWord::wdCollapseEnd ;
		WORD_ENFORCE( m_member->Collapse( &direction ), ("Failed to collapse Word range (end)") ) ;
	WORD_CATCH 
}

void WordRange::collapse_start()
{ 
	WORD_TRY
		ATLASSERT( is_instantiated() ) ;

		_variant_t direction = (long)MSWord::wdCollapseStart ;
		WORD_ENFORCE( m_member->Collapse( &direction), ("Failed to collapse Word range (start)") ) ;
	WORD_CATCH 
}

long WordRange::move_end_while(const _bstr_t &pass_chars)
{ 
	WORD_TRY
		ATLASSERT( is_instantiated() ) ;

		_variant_t move_while( pass_chars ) ;
		_variant_t count_sentinel( (long)1000 ) ;
		long prop = m_member->MoveEndWhile( &move_while, &count_sentinel ) ;
		return prop ;
	WORD_CATCH 
}

long WordRange::move_end_until(const _bstr_t &text)
{ 
	WORD_TRY
		ATLASSERT( is_instantiated() ) ;
		
		_variant_t move_until( text ) ;
		_variant_t count_sentinel( (long)1000 ) ;
		long prop = 0 ;
		WORD_ENFORCE( m_member->raw_MoveEndUntil( &move_until, &count_sentinel, &prop ), ("Call to MoveEndUntil() failed") ) ;
		return prop ;
	WORD_CATCH 
}


long WordRange::get_start()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated() ) ;
	
		long count = 0 ;
		WORD_ENFORCE( m_member->get_Start( &count), ("Failed to retrieve Start property") ) ;
		ATLASSERT( count >= 0 ) ;
		return count ;

	WORD_CATCH 
}

long WordRange::get_end()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated() ) ;
	
		long count = 0 ;
		WORD_ENFORCE( m_member->get_End( &count), ("Failed to retrieve End property") ) ;
		ATLASSERT( count >= 0 ) ;
		return count ;

	WORD_CATCH 
}


void WordRange::put_start( long start)
{ 
	WORD_TRY

		ATLASSERT( is_instantiated() ) ;
		ATLASSERT( start >= 0 ) ;
		
		WORD_ENFORCE( m_member->put_Start( start), ("Failed to set Start property") ) ;

	WORD_CATCH 
}

void WordRange::put_end( long end ) 
{ 
	WORD_TRY

		ATLASSERT( is_instantiated() ) ;
		ATLASSERT( end >= 0 ) ;
		ATLASSERT( end >= get_start() ) ;
		
		WORD_ENFORCE( m_member->put_End( end ), ("Failed to set End property") ) ;

	WORD_CATCH 
}
word_chars_ptr WordRange::get_characters()
{
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;

	word_chars_ptr chars ;
	WORD_ENFORCE( m_member->get_Characters( &chars ), ("Failed to put selection text") ) ;
	ATLASSERT( chars ) ;

	return chars ;

	WORD_CATCH 

}

word_words_ptr WordRange::get_words()
{
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;

	word_words_ptr words ;
	WORD_ENFORCE( m_member->get_Words( &words ), ("Failed to put selection text") ) ;
	ATLASSERT( words ) ;

	return words ;

	WORD_CATCH 
}

void WordRange::select_range(const long start_pos, const long end_pos)
{
	WORD_TRY
	ATLASSERT( start_pos >= 0 ) ;
	ATLASSERT( end_pos >= start_pos ) ;

	WORD_ENFORCE( m_member->SetRange(start_pos, end_pos), 
		("Failed to set selection range") ) ;

	select() ;
	WORD_CATCH 
}

word_app_ptr WordRange::get_application()
{ 
	WORD_TRY
		
		ATLASSERT( is_instantiated() ) ;
		
		word_app_ptr app ;
		WORD_ENFORCE( m_member->get_Application( &app), ("Failed to retrieve Application property") ) ;
		ATLASSERT( app ) ;
		
		return app ;

	WORD_CATCH 
}

void WordRange::select()
{ 
	WORD_TRY
	
		ATLASSERT( is_instantiated() ) ;

		WORD_ENFORCE( m_member->Select(), ("Call to Select() method failed") ) ;

	WORD_CATCH 
}


void WordRange::put_text(const _bstr_t &text)
{ 
	WORD_TRY
		
		ATLASSERT( is_instantiated( ) ) ;
		
		WORD_ENFORCE( m_member->put_Text( text ), ("Failed to set range text") ) ;
	
	WORD_CATCH 
}

// -------------------------------
// WordFont functions
// -------------------------------

MSOffice::MsoTriState WordFont::is_superscript()
{
	long setting = 0 ;
	
	WORD_TRY
		
		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->get_Superscript( &setting), ("Failed to retrieve Superscript property") ) ;
	
	WORD_CATCH 
		
	return (MSOffice::MsoTriState)setting ;
}

MSOffice::MsoTriState WordFont::is_subscript()
{
	long setting = 0 ;
	
	WORD_TRY
		
		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->get_Subscript( &setting), ("Failed to retrieve Subscript property") ) ;
	
	WORD_CATCH 
		
	return (MSOffice::MsoTriState)setting ;
}

MSOffice::MsoTriState WordFont::is_bold()
{ 
	long setting = 0 ;
	
	WORD_TRY
	
		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->get_Bold( &setting), ("Failed to retrieve Bold property") ) ;

	WORD_CATCH 
	
	return (MSOffice::MsoTriState)setting ;
}

void WordFont::put_bold_on()
{ 
	WORD_TRY
		
		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->put_Bold( MSOffice::msoTrue ), ("Failed to set Bold property") ) ;

	WORD_CATCH 
}

void WordFont::put_bold_off()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->put_Bold( MSOffice::msoFalse ), ("Failed set set Bold property to False") ) ;

	WORD_CATCH 
}

MSOffice::MsoTriState WordFont::is_italic()
{ 
	long setting = 0 ;
	
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->get_Italic( &setting ), ("Failed to get Italic property") ) ;
	
	WORD_CATCH 

	return (MSOffice::MsoTriState)setting ;
}

void WordFont::put_italic_on()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->put_Italic( MSOffice::msoTrue ), ("Failed to set Italic property to True") ) ;

	WORD_CATCH 
}

void WordFont::put_italic_off()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->put_Italic( MSOffice::msoFalse ), ("Failed to set Italic property to False") ) ;

	WORD_CATCH 
}

MSWord::WdUnderline WordFont::get_underline()
{ 
	MSWord::WdUnderline setting = MSWord::wdUnderlineNone ;
	
	WORD_TRY
	
		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->get_Underline( &setting ), ("Failed to get Underline property") ) ;
	
	WORD_CATCH
		
	return setting ;
}

void WordFont::put_underline_on()
{ 
	WORD_TRY
		
		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->put_Underline( MSWord::wdUnderlineSingle ), ("Failed to set Underline property to True") ) ;

	WORD_CATCH 
}

void WordFont::put_underline_off()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
	
		WORD_ENFORCE( m_member->put_Underline( MSWord::wdUnderlineNone ), ("Failed to set Underline property to False") ) ;

	WORD_CATCH 
}

void WordFont::set_rgb_color( const COLORREF color )
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
	
		_variant_t vcolor( (long)color ) ;

		CDispatchWrapper wrapper( CComPtr<IDispatch>( static_cast< IDispatch* >( m_member ) ) ) ;
		wrapper.prop_put( L"Color", vcolor ) ;

	WORD_CATCH 
}

COLORREF WordFont::get_rgb_color()
{ 
	WORD_TRY
	
		ATLASSERT( is_instantiated( ) ) ;
		
		CDispatchWrapper wrapper( CComPtr<IDispatch>( static_cast< IDispatch* >( m_member ) ) ) ;
		_variant_t color = wrapper.prop_get( L"Color" ) ;
	
		COLORREF cref = (COLORREF)color.lVal ;
		
		return cref ;
	
	WORD_CATCH 
}

void WordFont::set_face(const wstring &font_face )
{ 
	WORD_TRY
	
		ATLASSERT( is_instantiated( ) ) ;

		m_member->Name = font_face.c_str() ;

	WORD_CATCH 
}


word_doc_ptr WordApplication::get_active_document()
{ 
	WORD_TRY
		
		ATLASSERT( is_instantiated( ) ) ;
	
		word_doc_ptr doc ;
		WORD_ENFORCE( m_member->get_ActiveDocument( &doc ), ("Failed to get active document") ) ;
		ATLASSERT( doc ) ;

		return doc ;

	WORD_CATCH 
}

word_bkms_ptr WordDocument::get_bookmarks()
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
		
		word_bkms_ptr bookmarks ;
		WORD_ENFORCE( m_member->get_Bookmarks( &bookmarks ), ("Failed to get bookmarks collection") ) ;
		ATLASSERT( bookmarks ) ;
		
		return bookmarks ;

	WORD_CATCH 
}

word_bkm_ptr WordBookmarks::add_bookmark(const _bstr_t &name, WordRange &range)
{ 
	WORD_TRY

		ATLASSERT( is_instantiated( ) ) ;
		
		word_bkm_ptr bkmark ;
		_variant_t vrange( (word_range_ptr)range ) ;
		WORD_ENFORCE( m_member->raw_Add( name, &vrange, &bkmark ), ("Failed to add bookmark") ) ;
		ATLASSERT( bkmark ) ;
		
		return bkmark ;

	WORD_CATCH 
}

word_range_ptr WordBookmark::get_range()
{ 
	WORD_TRY
		ATLASSERT( is_instantiated( ) ) ;
		
		word_range_ptr range ;
		WORD_ENFORCE( m_member->get_Range( &range ), ("Failed to get bookmark range") ) ;
		ATLASSERT( range ) ;
		return range ;

	WORD_CATCH 
}

void WordBookmark::delete_bookmark()
{ 
	WORD_TRY
	
		ATLASSERT( is_instantiated( ) ) ;
		
		WORD_ENFORCE( m_member->Delete( ), ("Failed to delete bookmark") ) ;

	WORD_CATCH 
}

_bstr_t WordFont::get_face()
{ 
	WORD_TRY
		ATLASSERT( is_instantiated( ) ) ;
		
		return m_member->Name ;

	WORD_CATCH 
}

void WordSelection::put_text(const wstring &text)
{ 
	WORD_TRY
	
		ATLASSERT( is_instantiated( ) ) ;

		m_member->Text = text.c_str() ;

	WORD_CATCH 
}

bool WordApplication::active_window_has_focus()
{
	HWND focus_hwnd = ::GetFocus() ;
	return ( focus_hwnd == m_active_window ) ;
}


symbol_converter::symbol_converter()
{
	m_symbols[0x22]     // # FOR ALL
		= 0x2200 ;
	m_symbols[0x24]     // # THERE EXISTS
		= 0x2203 ;
	m_symbols[0x27]     // # CONTAINS AS MEMBER
		= 0x220B ;
	m_symbols[0x2A]     // # ASTERISK OPERATOR
		= 0x2217 ;
	m_symbols[0x2D]     // # MINUS SIGN
		= 0x2212 ;
	m_symbols[0x40]     // # APPROXIMATELY EQUAL TO
		= 0x2245 ;
	m_symbols[0x41]     // # GREEK CAPITAL LETTER ALPHA
		= 0x391 ;
	m_symbols[0x42]     // # GREEK CAPITAL LETTER BETA
		= 0x392 ;
	m_symbols[0x43]     // # GREEK CAPITAL LETTER CHI
		= 0x3A7 ;
	m_symbols[0x44]     // # GREEK CAPITAL LETTER DELTA
		= 0x394 ;
	m_symbols[0x44]     // # INCREMENT
		= 0x2206 ;
	m_symbols[0x45]     // # GREEK CAPITAL LETTER EPSILON
		= 0x395 ;
	m_symbols[0x46]     // # GREEK CAPITAL LETTER PHI
		= 0x3A6 ;
	m_symbols[0x47]     // # GREEK CAPITAL LETTER GAMMA
		= 0x393 ;
	m_symbols[0x48]     // # GREEK CAPITAL LETTER ETA
		= 0x397 ;
	m_symbols[0x49]     // # GREEK CAPITAL LETTER IOTA
		= 0x399 ;
	m_symbols[0x4A]     // # GREEK THETA SYMBOL
		= 0x3D1 ;
	m_symbols[0x4B]     // # GREEK CAPITAL LETTER KAPPA
		= 0x39A ;
	m_symbols[0x4C]     // # GREEK CAPITAL LETTER LAMDA
		= 0x39B ;
	m_symbols[0x4D]     // # GREEK CAPITAL LETTER MU
		= 0x39C ;
	m_symbols[0x4E]     // # GREEK CAPITAL LETTER NU
		= 0x39D ;
	m_symbols[0x4F]     // # GREEK CAPITAL LETTER OMICRON
		= 0x39F ;

	// 0x5
	m_symbols[0x50]     // # GREEK CAPITAL LETTER PI
	= 0x3A0 ;
	m_symbols[0x51]     // # GREEK CAPITAL LETTER THETA
	= 0x398 ;
	m_symbols[0x52]     // # GREEK CAPITAL LETTER RHO
	= 0x3A1 ;
	m_symbols[0x53]     // # GREEK CAPITAL LETTER SIGMA
	= 0x3A3 ;
	m_symbols[0x54]     // # GREEK CAPITAL LETTER TAU
	= 0x3A4 ;
	m_symbols[0x55]     // # GREEK CAPITAL LETTER UPSILON
	= 0x3A5 ;
	m_symbols[0x56]     // # GREEK SMALL LETTER FINAL SIGMA
	= 0x3C2 ;
	m_symbols[0x57]     // # OHM SIGN
	= 0x2126 ;
	m_symbols[0x58]     // # GREEK CAPITAL LETTER XI
	= 0x39E ;
	m_symbols[0x59]     // # GREEK CAPITAL LETTER PSI
	= 0x3A8 ;
	m_symbols[0x5A]     // # GREEK CAPITAL LETTER ZETA
	= 0x396 ;
	m_symbols[0x5C]     // # THEREFORE
	= 0x2234 ;
	m_symbols[0x5E]     // # UP TACK
	= 0x22A5 ;

	// 0x6
	m_symbols[0x60]     // # RADICAL EXTENDER
	= 0xF8E5 ;
	m_symbols[0x61]     // # GREEK SMALL LETTER ALPHA
	= 0x3B1 ;
	m_symbols[0x62]     // # GREEK SMALL LETTER BETA
	= 0x3B2 ;
	m_symbols[0x63]     // # GREEK SMALL LETTER CHI
	= 0x3C7 ;
	m_symbols[0x64]     // # GREEK SMALL LETTER DELTA
	= 0x3B4 ;
	m_symbols[0x65]     // # GREEK SMALL LETTER EPSILON
	= 0x3B5 ;
	m_symbols[0x66]     // # GREEK SMALL LETTER PHI
	= 0x3C6 ;
	m_symbols[0x67]     // # GREEK SMALL LETTER GAMMA
	= 0x3B3 ;
	m_symbols[0x68]     // # GREEK SMALL LETTER ETA
	= 0x3B7 ;
	m_symbols[0x69]     // # GREEK SMALL LETTER IOTA
	= 0x3B9 ;
	m_symbols[0x6A]     // # GREEK PHI SYMBOL
	= 0x3D5 ;
	m_symbols[0x6B]     // # GREEK SMALL LETTER KAPPA
	= 0x3BA ;
	m_symbols[0x6C]     // # GREEK SMALL LETTER LAMDA
	= 0x3BB ;
	m_symbols[0x6D]     // # MICRO SIGN
	= 0xB5 ;
	m_symbols[0x6E]     // # GREEK SMALL LETTER NU
	= 0x3BD ;
	m_symbols[0x6F]     // # GREEK SMALL LETTER OMICRON
	= 0x3BF ;

	// 0x7
	m_symbols[0x70]     // # GREEK SMALL LETTER PI
	= 0x3C0 ;
	m_symbols[0x71]     // # GREEK SMALL LETTER THETA
	= 0x3B8 ;
	m_symbols[0x72]     // # GREEK SMALL LETTER RHO
	= 0x3C1 ;
	m_symbols[0x73]     // # GREEK SMALL LETTER SIGMA
	= 0x3C3 ;
	m_symbols[0x74]     // # GREEK SMALL LETTER TAU
	= 0x3C4 ;
	m_symbols[0x75]     // # GREEK SMALL LETTER UPSILON
	= 0x3C5 ;
	m_symbols[0x76]     // # GREEK PI SYMBOL
	= 0x3D6 ;
	m_symbols[0x77]     // # GREEK SMALL LETTER OMEGA
	= 0x3C9 ;
	m_symbols[0x78]     // # GREEK SMALL LETTER XI
	= 0x3BE ;
	m_symbols[0x79]     // # GREEK SMALL LETTER PSI
	= 0x3C8 ;
	m_symbols[0x7A]     // # GREEK SMALL LETTER ZETA
	= 0x3B6 ;
	m_symbols[0x7E]     // # TILDE OPERATOR
	= 0x223C ;

	// 0xA
	m_symbols[0xA0]     // # EURO SIGN
	= 0x20AC ;
	m_symbols[0xA1]     // # GREEK UPSILON WITH HOOK SYMBOL
	= 0x3D2 ;
	m_symbols[0xA2]     // # PRIME
	= 0x2032 ;
	m_symbols[0xA3]     // # LESS-THAN OR EQUAL TO
	= 0x2264 ;
	m_symbols[0xA4]     // # DIVISION SLASH
	= 0x2215 ;
	m_symbols[0xA5]     // # INFINITY
	= 0x221E ;
	m_symbols[0xA6]     // # LATIN SMALL LETTER F WITH HOOK
	= 0x192 ;
	m_symbols[0xA7]     // # BLACK CLUB SUIT
	= 0x2663 ;
	m_symbols[0xA8]     // # BLACK DIAMOND SUIT
	= 0x2666 ;
	m_symbols[0xA9]     // # BLACK HEART SUIT
	= 0x2665 ;
	m_symbols[0xAA]     // # BLACK SPADE SUIT
	= 0x2660 ;
	m_symbols[0xAB]     // # LEFT RIGHT ARROW
	= 0x2194 ;
	m_symbols[0xAC]     // # LEFTWARDS ARROW
	= 0x2190 ;
	m_symbols[0xAD]     // # UPWARDS ARROW
	= 0x2191 ;
	m_symbols[0xAE]     // # RIGHTWARDS ARROW
	= 0x2192 ;
	m_symbols[0xAF]     // # DOWNWARDS ARROW
	= 0x2193 ;

	// 0xB
	m_symbols[0xB0]     // # DEGREE SYMBOL
	= 0x00B0 ;
	m_symbols[0xB1]     // # PLUS-MINUS
	= 0x00B1 ;
	m_symbols[0xB2]     // # DOUBLE PRIME
	= 0x2033 ;
	m_symbols[0xB3]     // # GREATER-THAN OR EQUAL TO
	= 0x2265 ;
	m_symbols[0xB4]     // # MULTIPLICATION SIGN
	= 0xD7 ;
	m_symbols[0xB5]     // # PROPORTIONAL TO
	= 0x221D ;
	m_symbols[0xB6]     // # PARTIAL DIFFERENTIAL
	= 0x2202 ;
	m_symbols[0xB7]     // # BULLET
	= 0x2022 ;
	m_symbols[0xB8]     // # DIVISION SIGN
	= 0xF7 ;
	m_symbols[0xB9]     // # NOT EQUAL TO
	= 0x2260 ;
	m_symbols[0xBA]     // # IDENTICAL TO
	= 0x2261 ;
	m_symbols[0xBB]     // # ALMOST EQUAL TO
	= 0x2248 ;
	m_symbols[0xBC]     // # HORIZONTAL ELLIPSIS
	= 0x2026 ;
	m_symbols[0xBD]     // # VERTICAL ARROW EXTENDER
	= 0xF8E6 ;
	m_symbols[0xBE]     // # HORIZONTAL ARROW EXTENDER
	= 0xF8E7 ;
	m_symbols[0xBF]     // # DOWNWARDS ARROW WITH CORNER LEFTWARDS
	= 0x21B5 ;

	// 0xC
	m_symbols[0xC0]     // # ALEF SYMBOL
	= 0x2135 ;
	m_symbols[0xC1]     // # BLACK-LETTER CAPITAL I
	= 0x2111 ;
	m_symbols[0xC2]     // # BLACK-LETTER CAPITAL R
	= 0x211C ;
	m_symbols[0xC3]     // # SCRIPT CAPITAL P
	= 0x2118 ;
	m_symbols[0xC4]     // # CIRCLED TIMES
	= 0x2297 ;
	m_symbols[0xC5]     // # CIRCLED PLUS
	= 0x2295 ;
	m_symbols[0xC6]     // # EMPTY SET
	= 0x2205 ;
	m_symbols[0xC7]     // # INTERSECTION
	= 0x2229 ;
	m_symbols[0xC8]     // # UNION
	= 0x222A ;
	m_symbols[0xC9]     // # SUPERSET OF
	= 0x2283 ;
	m_symbols[0xCA]     // # SUPERSET OF OR EQUAL TO
	= 0x2287 ;
	m_symbols[0xCB]     // # NOT A SUBSET OF
	= 0x2284 ;
	m_symbols[0xCC]     // # SUBSET OF
	= 0x2282 ;
	m_symbols[0xCD]     // # SUBSET OF OR EQUAL TO
	= 0x2286 ;
	m_symbols[0xCE]     // # ELEMENT OF
	= 0x2208 ;
	m_symbols[0xCF]     // # NOT AN ELEMENT OF
	= 0x2209 ;

	// 0xD
	m_symbols[0xD0]     // # ANGLE
	= 0x2220 ;
	m_symbols[0xD1]     // # NABLA
	= 0x2207 ;
	m_symbols[0xD2]     // # REGISTERED SIGN SERIF
	= 0xF6DA ;
	m_symbols[0xD3]     // # COPYRIGHT SIGN SERIF
	= 0xF6D9 ;
	m_symbols[0xD4]     // # TRADE MARK SIGN SERIF
	= 0xF6DB ;
	m_symbols[0xD5]     // # N-ARY PRODUCT
	= 0x220F ;
	m_symbols[0xD6]     // # SQUARE ROOT
	= 0x221A ;
	m_symbols[0xD7]     // # DOT OPERATOR
	= 0x22C5 ;
	m_symbols[0xD8]     // # NOT SIGN
	= 0xAC ;
	m_symbols[0xD9]     // # LOGICAL AND
	= 0x2227 ;
	m_symbols[0xDA]     // # LOGICAL OR
	= 0x2228 ;
	m_symbols[0xDB]     // # LEFT RIGHT DOUBLE ARROW
	= 0x21D4 ;
	m_symbols[0xDC]     // # LEFTWARDS DOUBLE ARROW
	= 0x21D0 ;
	m_symbols[0xDD]     // # UPWARDS DOUBLE ARROW
	= 0x21D1 ;
	m_symbols[0xDE]     // # RIGHTWARDS DOUBLE ARROW
	= 0x21D2 ;
	m_symbols[0xDF]     // # DOWNWARDS DOUBLE ARROW
	= 0x21D3 ;

	// 0xE
	m_symbols[0xE0]     // # LOZENGE
	= 0x25CA ;
	m_symbols[0xE1]     // # LEFT-POINTING ANGLE BRACKET
	= 0x2329 ;
	m_symbols[0xE2]     // # REGISTERED SIGN SANS SERIF
	= 0xF8E8 ;
	m_symbols[0xE3]     // # COPYRIGHT SIGN SANS SERIF
	= 0xF8E9 ;
	m_symbols[0xE4]     // # TRADE MARK SIGN SANS SERIF
	= 0xF8EA ;
	m_symbols[0xE5]     // # N-ARY SUMMATION
	= 0x2211 ;
	m_symbols[0xE6]     // # LEFT PAREN TOP
	= 0xF8EB ;
	m_symbols[0xE7]     // # LEFT PAREN EXTENDER
	= 0xF8EC ;
	m_symbols[0xE8]     // # LEFT PAREN BOTTOM
	= 0xF8ED ;
	m_symbols[0xE9]     // # LEFT SQUARE BRACKET TOP
	= 0xF8EE ;
	m_symbols[0xEA]     // # LEFT SQUARE BRACKET EXTENDER
	= 0xF8EF ;
	m_symbols[0xEB]     // # LEFT SQUARE BRACKET BOTTOM
	= 0xF8F0 ;
	m_symbols[0xEC]     // # LEFT CURLY BRACKET TOP
	= 0xF8F1 ;
	m_symbols[0xED]     // # LEFT CURLY BRACKET MID
	= 0xF8F2 ;
	m_symbols[0xEE]     // # LEFT CURLY BRACKET BOTTOM
	= 0xF8F3 ;
	m_symbols[0xEF]     // # CURLY BRACKET EXTENDER
	= 0xF8F4 ;

	// 0xF
	m_symbols[0xF1]     // # RIGHT-POINTING ANGLE BRACKET
	= 0x232A ;
	m_symbols[0xF2]     // # INTEGRAL
	= 0x222B ;
	m_symbols[0xF3]     // # TOP HALF INTEGRAL
	= 0x2320 ;
	m_symbols[0xF4]     // # INTEGRAL EXTENDER
	= 0xF8F5 ;
	m_symbols[0xF5]     // # BOTTOM HALF INTEGRAL
	= 0x2321 ;
	m_symbols[0xF6]     // # RIGHT PAREN TOP
	= 0xF8F6 ;
	m_symbols[0xF7]     // # RIGHT PAREN EXTENDER
	= 0xF8F7 ;
	m_symbols[0xF8]     // # RIGHT PAREN BOTTOM
	= 0xF8F8 ;
	m_symbols[0xF9]     // # RIGHT SQUARE BRACKET TOP
	= 0xF8F9 ;
	m_symbols[0xFA]     // # RIGHT SQUARE BRACKET EXTENDER
	= 0xF8FA ;
	m_symbols[0xFB]     // # RIGHT SQUARE BRACKET BOTTOM
	= 0xF8FB ;
	m_symbols[0xFC]     // # RIGHT CURLY BRACKET TOP
	= 0xF8FC ;
	m_symbols[0xFD]     // # RIGHT CURLY BRACKET MID
	= 0xF8FD ;
	m_symbols[0xFE]     // # RIGHT CURLY BRACKET BOTTOM
	= 0xF8FE ;
}

wchar_t symbol_converter::convert_symbol( const long charcode )
{
	BOOST_STATIC_ASSERT(L'(' == 40) ;
	if ( charcode == 40 )
	{
		return L'(' ;
	}
	long symbol = charcode + 0x1000 ;

	ATLASSERT( charcode != 40 ) ;

	if ( m_symbols.end() == m_symbols.find( symbol ) )
	{
		return static_cast< wchar_t >( charcode )  ;
	}

	return m_symbols[symbol] ;
}
