// WordAutomationObject.h: interface for the WordAutomationObject class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable:4146) // unary minus operator applied to unsigned type

#import "C:/dev/cpp/Common DLLs/Word 2000 OLBs/MSO.DLL"	\
	rename_namespace("MSOffice") rename("RGB", "OfficeRGB") rename( "SearchPath", "OfficeSearchPath" ) rename("DocumentProperties", "OfficeDocumentProperties") named_guids    

#import "D:\Program Files (x86)\Common Files\microsoft shared\VBA\VBA6\\VBE6EXT.olb" \
	rename_namespace("VBE6") 

#import "C:/dev/cpp/Common DLLs/Word 2000 OLBs/MSWORD9.olb" \
	rename_namespace("MSWord") rename("FindText", "WordFindText") rename("ExitWindows","MSWordExitWindows") \
	rename( "RGB", "WordRBG" ) rename( "ReplaceText", "WordReplaceText" )

#include "html_processing.h"

#include "OleAutoWrapper.h"
#include "Localizer.h"

#include "logging.h"

#pragma warning(default:4146) // unary minus operator applied to unsigned type

#define WORD_ENFORCE( expr, fail_msg ) \
{							\
	HRESULT _hr = (expr) ;							\
	if( FAILED( _hr ) )								\
	{														\
		TRACE_HRESULT( _hr ) ;						\
		CString _msg( fail_msg ) ;				\
		throw except::CComException( _msg, _hr, m_member, _T(__FILE__), __LINE__  );	\
	}														\
}

// typedefs
typedef MSOffice::_CommandBarButtonPtr		command_button_ptr ;
typedef MSOffice::_CommandBarsPtr			office_cmd_bars ;
typedef MSOffice::CommandBarControlsPtr		office_cmd_bar_ctls ;
typedef MSOffice::CommandBarControlPtr		office_cmd_bar_ctl ;
typedef MSWord::_ApplicationPtr				word_app_ptr ;
typedef MSWord::SelectionPtr				word_sel_ptr ;
typedef MSWord::CharactersPtr				word_chars_ptr ;
typedef MSWord::WordsPtr					word_words_ptr ;
typedef MSWord::RangePtr					word_range_ptr ;
typedef MSWord::_FontPtr					word_font_ptr ;
typedef MSWord::BookmarksPtr				word_bkms_ptr ;
typedef MSWord::BookmarkPtr					word_bkm_ptr ;
typedef MSWord::_DocumentPtr				word_doc_ptr ;

template < class object_type, class member_type >
class WordObject 
{
protected:
	member_type m_member ;

public:
	WordObject(){}
	WordObject( const member_type cpy ) : m_member( cpy ) {}
	object_type& operator=( const member_type cpy )
	{
		m_member = ( cpy ) ;
		object_type* obj = static_cast< object_type* >(this) ;
		return *obj ;
	}
	object_type& operator=( const object_type &cpy ) 
	{
		m_member = ( cpy.m_member ) ;
		object_type* obj = static_cast< object_type* >(this) ;
		return *obj ;
	}
	virtual bool is_instantiated()
	{
		return !! m_member ;
	}
	operator member_type() { return m_member ; }
} ;

class WordFont : public WordObject< WordFont, word_font_ptr >
{
	typedef WordObject< WordFont, word_font_ptr > base_object ;
public:
	_bstr_t get_face();

	void set_face( const wstring &face );
	
	COLORREF get_rgb_color();
	void set_rgb_color( const COLORREF color );

	void put_underline_off();
	void put_underline_on();

	void put_italic_off();
	void put_italic_on();

	void put_bold_off();
	void put_bold_on();

	bool is_underline_on( ) { return ( MSWord::wdUnderlineNone != get_underline() ) ; }
	bool is_bold_on( ) { return ( MSOffice::msoTrue == is_bold() ) ; }
	bool is_italic_on( ) { return ( MSOffice::msoTrue == is_italic() ) ; }
	bool is_superscript_on( ) { return ( MSOffice::msoTrue == is_superscript() ) ; }
	bool is_subscript_on( ) { return ( MSOffice::msoTrue == is_subscript() ) ; }

	MSWord::WdUnderline get_underline() ;
	MSOffice::MsoTriState is_bold() ;
	MSOffice::MsoTriState is_italic() ;
	MSOffice::MsoTriState is_superscript() ;
	MSOffice::MsoTriState is_subscript() ;
		
	void set_v_align( const html_processing::VERT_ALIGNMENT aligment )
	{
		switch( aligment )
		{
		case html_processing::VA_BASELINE:
			m_member->put_Subscript( 0 ) ;
			m_member->put_Superscript( 0 ) ;
			break ;
		case html_processing::VA_SUBSCRIPT:
			m_member->put_Subscript( VARIANT_TRUE ) ;
			m_member->put_Superscript( 0 ) ;
			break ;
		case html_processing::VA_SUPERSCRIPT:
			m_member->put_Subscript( 0 ) ;
			m_member->put_Superscript( VARIANT_TRUE ) ;
			break ;
		}
	}
	html_processing::VERT_ALIGNMENT get_v_align()
	{
		if ( is_subscript_on() )
		{
			return html_processing::VA_SUBSCRIPT ;
		}

		if ( is_superscript_on() )
		{
			return html_processing::VA_SUPERSCRIPT ;
		}

		return html_processing::VA_BASELINE ;
	}
	WordFont(){}
	WordFont( const word_font_ptr cpy ) : base_object( cpy ) {}
	WordFont( const WordFont &cpy ) : base_object( cpy ) {}
	
};

class WordRange : public WordObject< WordRange, word_range_ptr >
{
	typedef WordObject< WordRange, word_range_ptr > base_object ;
public:
	void select_range( const long start_pos, const long end_pos );
	word_words_ptr get_words();
	word_chars_ptr get_characters();
	void put_text( const _bstr_t &text );
	void select();
	void extend_to_story_end();

	long  move_end_until( const _bstr_t &text );
	long  move_end_while( const _bstr_t &pass_chars );

	void put_end( long end );
	long get_end();
	void put_start( long start );
	long get_start();
	_bstr_t next_char()
	{
		long end = this->get_end() ;
		this->put_start(end) ;
		this->put_end(end+1) ;
		return this->get_text() ;
	}

	word_app_ptr get_application();

	void collapse_start();
	void collapse_end();
	
	word_font_ptr get_font();
	_bstr_t get_unicode_char();
	_bstr_t get_text();
	long get_count();
	
	WordRange(){}
	WordRange( const word_range_ptr cpy ) : base_object( cpy ) {}
	WordRange( const WordRange &cpy ) : base_object( cpy ) {}
	
};

class WordWords : public WordObject< WordWords, word_words_ptr >
{
	typedef WordObject< WordWords, word_words_ptr > base_object ;
public:
	word_range_ptr item( long index );
	long get_count();
	
	WordWords(){}
	WordWords( const word_words_ptr cpy ) : base_object( cpy ) {}
	WordWords( const WordWords &cpy ) : base_object( cpy ) {}
	
};

class WordCharacters : public WordObject< WordCharacters, word_chars_ptr >
{
	typedef WordObject< WordCharacters, word_chars_ptr > base_object ;
public:
	word_range_ptr item( long index );
	long get_count();
	
	WordCharacters(){}
	WordCharacters( const word_chars_ptr cpy ) : base_object( cpy ) {}
	WordCharacters( const WordCharacters &cpy ) : base_object( cpy ) {}
	
};


class WordSelection : public WordObject< WordSelection, word_sel_ptr >
{
	typedef WordObject< WordSelection, word_sel_ptr > base_object ;
public:
	void put_text( const wstring &text );
	void put_start( const long start );
	void type_text( const _bstr_t &text );
	void type_paragraph();
	long extend_end( MSWord::WdUnits unit, long count );
	word_range_ptr get_range();
	word_chars_ptr get_characters();
	_bstr_t get_text();
	void set_start( long new_start );
	long get_end();
	long get_story_length();
	long get_start();

	WordSelection(){}
	WordSelection( const word_sel_ptr cpy ) : base_object( cpy ) {}
	WordSelection( const WordSelection &cpy ) : base_object( cpy ) {}

};

class WordBookmark : public WordObject< WordBookmark, word_bkm_ptr > 
{
	typedef WordObject< WordBookmark, word_bkm_ptr > base_object ;
	
public:
	word_range_ptr get_range();
	void delete_bookmark() ;
	
	WordBookmark() {}
	WordBookmark( const word_bkm_ptr cpy ) : base_object( cpy ) {}
	WordBookmark( const WordBookmark &cpy ) : base_object( cpy ) {}
};

class WordBookmarks : public WordObject< WordBookmarks, word_bkms_ptr > 
{
	typedef WordObject< WordBookmarks, word_bkms_ptr > base_object ;
	
public:
	word_bkm_ptr add_bookmark( const _bstr_t &name, WordRange &range );
	
	WordBookmarks() {}
	WordBookmarks( const word_bkms_ptr cpy ) : base_object( cpy ) {}
	WordBookmarks( const WordBookmarks &cpy ) : base_object( cpy ) {}
};

class WordDocument : public WordObject< WordDocument, word_doc_ptr > 
{
	typedef WordObject< WordDocument, word_doc_ptr > base_object ;

public:
	word_bkms_ptr get_bookmarks();
	
	WordDocument() {}
	WordDocument( const word_doc_ptr cpy ) : base_object( cpy ) {}
	WordDocument( const WordDocument &cpy ) : base_object( cpy ) {}
};

class WordApplication : public WordObject< WordApplication, word_app_ptr > 
{
	typedef WordObject< WordApplication, word_app_ptr > base_object ;
	bool	m_active ;
	HWND	m_active_window ;
public:
	bool active_window_has_focus();
	word_doc_ptr get_active_document();
	void deactivate();
	void activate();
	word_sel_ptr get_selection();
	bool selection_is_empty();
	void set_app( word_app_ptr word_app );
	bool is_active();

	MSWord::_DocumentPtr open( const _bstr_t file_to_open ) ;

	void set_screen_updating_on()
	{
		ATLASSERT( is_instantiated( ) ) ;

		WORD_ENFORCE( m_member->put_ScreenUpdating( VARIANT_TRUE ), _("Failed to turn on screen updating") ) ;
	}
	void set_screen_updating_off()
	{
		ATLASSERT( is_instantiated( ) ) ;
		
		WORD_ENFORCE( m_member->put_ScreenUpdating( VARIANT_FALSE ), _("Failed to turn off screen updating")  ) ;
	}

	void set_status_bar( const _bstr_t &text )
	{
		HRESULT hr = m_member->put_StatusBar( text ) ;
		ASSERT_HRESULT( hr ) ;
		if (! SUCCEEDED(hr) )
		{
			_com_error e(hr) ;
			logging::log_error("Failed to set status bar text.") ;
			logging::log_exception(e) ;
		}
		WordSelection selection = get_selection() ;
		WordRange range = selection.get_range() ;
		range.select() ;
	}
	WordApplication() : m_active(true), m_active_window(NULL) {}
	WordApplication( const word_app_ptr cpy ) : m_active(true), m_active_window(NULL), base_object( cpy ) {}
	WordApplication( const WordApplication &cpy ) : m_active(true), m_active_window(NULL), base_object( cpy ) {}

};
