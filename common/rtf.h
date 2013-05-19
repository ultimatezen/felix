/*!
	@file rtf.h
	@brief RichText wrappers
	@date 2005/06/23
	Time: 16:17:27
	@author Ryan Ginstrom
 */
#pragma once

#include "Exceptions.h"


/**
	@namespace rtf
	@brief For RichText stuff.
 */
namespace rtf
{
	
#define FLAG_CASE( var, x ) if ( var & x ) { WRITE_TABS ATLTRACE( #x "\n" ) ; return x ; }


void set_default_font_table(textstream_reader< char > &reader) ;
void advance_to_next_font(textstream_reader< char > &reader) ;
/** Enumerate systems fonts.
*/
BOOL CALLBACK EnumFontsProc(LPLOGFONT lplf, LPNEWTEXTMETRIC lpntm, DWORD FontType, LPVOID aFontCount) ;

/**
 * \struct my_font
 *	Wrapper for LOGFONT.
 */
struct my_font : public LOGFONT
{
public:

	/** default constructor. 
	*/
	my_font();

	/** constructor. 
	@param font LOGFONT. 
	*/
	my_font( const LOGFONT &font );

	/** constructor. 
	@param font my_font. 
	*/
	my_font( const my_font &font );

	/** Assignment operator. 
	@return my_font &operator description. 
	@param font parameter. 
	*/
	my_font &operator =( const LOGFONT &font );

	/** Assignment operator. 
	@return my_font& reference to self. 
	@param font The font to copy. 
	*/
	my_font &operator =( const my_font &font );

	/** get the font face. 
	@return tstring the name of the font face. 
	*/
	tstring get_name() const ;

	/** set the font information. 
	@param font parameter. 
	*/
	void set_font( const LOGFONT *font );

	/** initialize the font struct. 
	*/
	void init_font_info();

	/** get the font height. 
	@return LONG the height. 
	*/
	LONG get_height(  ) const;

	/** get the font width. 
	@return LONG the width. 
	*/
	LONG get_width( ) const;

	/** get the font weight. 
	@return LONG the weight. 
	*/
	LONG get_weight( ) const;

	/** set the font height. 
	@param height parameter. 
	*/
	void set_height( LONG height );

	/** set the font width. 
	@param width parameter. 
	*/
	void set_width( LONG width );

	/** set the font weight. 
	@param weight parameter. 
	*/
	void set_weight( LONG weight );

	/** Set the charset. 
	@param charset A byte charset value. 
	*/
	void set_charset( BYTE charset );

	/** set the font pitch. 
	@param pitch parameter. 
	*/
	void set_pitch ( BYTE pitch );

	/** set the font family. 
	@param family parameter. 
	*/
	void set_family ( BYTE family ) { lfPitchAndFamily |= family ; }

	/** clear the font pitch and family. 
	@param  parameter. 
	*/
	void clear_pitch_and_family( ) { lfPitchAndFamily = 0 ; }

	/** set the font face. 
	@param lfFaceName parameter. 
	@param face_name parameter. 
	*/
    void set_name( LPCTSTR face_name );

	/** look up the font. 
	@param hwnd Used to get the DC. 
	*/
	void lookup_font( HWND hwnd = ::GetActiveWindow() );

	/** get the font family. 
	@return BYTE the font family. 
	*/
	BYTE get_font_family() const ;

	/** get the font pitch. 
	@return BYTE description. 
	*/
	BYTE get_font_pitch() const ;

	/** Get the charset. 
	@return BYTE  
	*/
	BYTE get_charset() const ;
} ;




/**
 Representation of a font table entry.
 This is for the \\fonttable attribute of an rtf header. 
 */
struct font_table_entry
{
	tstring	m_family ;
	tstring	m_charset ;
	tstring	m_pitch ;
	tstring	m_name ;
	tstring	m_code ;

	my_font	m_font ;


	/** constructor. 
	@param entry parameter. 
	*/
	font_table_entry( const font_table_entry &entry )
		:	m_code( entry.m_code ), 
			m_family(entry.m_family), 
			m_charset(entry.m_charset), 
			m_pitch(entry.m_pitch), 
			m_name(entry.m_name), 
			m_font( entry.m_font )
	{
	}

	/** brief. 
	@return font_table_entry description. 
	@param font parameter. 
	*/
	explicit font_table_entry ( const my_font &font ) :
		m_font( font )
	{
		FillInfoFromFont( ) ;
	}

	/** constructor. 
	@param  parameter. 
	*/
	font_table_entry( )  
	{
	}


	// Function name	: operator= 
	// Description	    : 
	// Return type		: font_table_entry & 
	// Argument         : const my_font &font

	/** Assignment operator. 
	@return font_table_entry & description. 
	@param font parameter. 
	*/
	font_table_entry & operator= ( const my_font &font );

	// Function name	: operator= 
	// Description	    : 
	// Return type		: font_table_entry & 
	// Argument         : const font_table_entry &entry

	/** Assignment operator. 
	@return font_table_entry & description. 
	@param entry parameter. 
	*/
	font_table_entry & operator= (const font_table_entry &entry);

	// Function name	: operator==
	// Description	    : 
	// Return type		: bool 
	// Argument         : const font_table_entry &entry

	/** Test for equality. 
	@return bool description. 
	@param entry parameter. 
	*/
	bool operator==( const font_table_entry &entry );

	/** brief. 
	@param code parameter. 
	*/
	void set_code( int code );

	/** brief. 
	@param pitch parameter. 
	*/
	void fill_pitch_info( BYTE pitch );

	/** brief. 
	@param family parameter. 
	*/
	void fill_family_info( BYTE family );

	/** brief. 
	@param charset parameter. 
	*/
	void set_charset( BYTE charset );

private:

	void FillInfoFromFont( );

} ;


/**
 *	Represents the \\fonttable attribute in an rtf header.
 */
class font_table
{	
	typedef std::vector< font_table_entry > font_list ; ///< The container for the entries in the font table.

	font_list	m_fonts ;
	bool		m_has_default ;

public:
	/** constructor. 
	@param has_default parameter. 
	*/
	font_table( bool has_default = false );

	void clear();
	size_t size() const;
	// add_font

	/** brief. 
	@return tstring description. 
	@param code parameter. 
	*/
	tstring get_font_name( size_t code ) const ;

	/** brief. 
	@return font_table_entry description. 
	@param code parameter. 
	*/
	font_table_entry &get_font_entry( size_t code ) ;

	/** brief. 
	@return font_table_entry description. 
	@param name parameter. 
	*/
	font_table_entry &get_font_entry( tstring name ) ;

	/** brief. 
	@return font_table_entry description. 
	@param code parameter. 
	*/
	font_table_entry &get_font_entry_from_code( tstring code ) ;

	/** brief. 
	@return bool description. 
	@param name parameter. 
	*/
	bool is_font_in_table( tstring name );

	/** brief. 
	@return int description. 
	@param name parameter. 
	*/
	int add_font( tstring name );

	/** brief. 
	@return int description. 
	@param font parameter. 
	*/
	int add_font( const my_font &font );

	/** brief. 
	@return int description. 
	@param entry parameter. 
	*/
	int add_font( font_table_entry &entry );

	/** brief. 
	@return font_table_entry description. 
	@param font_name parameter. 
	*/
	font_table_entry get_entry_from_name( tstring font_name ) const;

	/** brief. 
	@return string description. 
	*/
	string get_table() const;

	/** brief. 
	@return BYTE description. 
	@param font_name parameter. 
	*/
	static BYTE get_charset_code( tstring font_name ) ;

	void parse_fonttbl( c_reader &reader );

	void readFontInfo( my_font &font, c_reader &reader );
	void setDefaultFontTable() ;
} ;


UINT getCodepageFromCharset( BYTE charset ) ;

	enum 
		{	RTF_AUTO,
			RTF_BLACK, RTF_BLUE, RTF_CYAN, RTF_GREEN, RTF_MAGENTA, RTF_RED, RTF_YELLOW, RTF_WHITE, RTF_DK_BLUE,		
			RTF_DK_CYAN, RTF_DK_GREEN, RTF_DK_MAGENTA, RTF_DK_RED, RTF_DK_YELLOW, RTF_DK_GRAY, RTF_LT_GRAY 
		} ;

#define RICH_TEXT_HEADER L"{\\rtf1\\ansi\\ansicpg932\\deff0\\deflang1033\\deflangfe1041" 
		
static const wchar_t* RICH_TEXT_COLOR_TABLE = L"{\\colortbl ;"													
					L"\\red0\\green0\\blue0;" 					
					L"\\red0\\green0\\blue255;"	 		
					L"\\red0\\green255\\blue255;" 			
					L"\\red0\\green255\\blue0;"	 			
					L"\\red255\\green0\\blue255;" 				
					L"\\red255\\green0\\blue0;" 				
					L"\\red255\\green255\\blue0;" 				
					L"\\red255\\green255\\blue255;" 		
					L"\\red0\\green0\\blue128;"	 				
					L"\\red0\\green128\\blue128;" 		
					L"\\red0\\green128\\blue0;"	 		
					L"\\red128\\green0\\blue128;" 		 
					L"\\red128\\green0\\blue0;" 				
					L"\\red128\\green128\\blue0;" 	  
					L"\\red128\\green128\\blue128;" 	
					L"\\red192\\green192\\blue192;" 		
				L"}" ;


#define RICH_TEXT_PREAMBLE L"\\viewkind4 \\lang1041" 
#define  RICH_TEXT_FOOTER 	L"}" 

	/** struct format_package */
	struct format_package
	{
		bool			mIsBold ;
		bool			mIsItalic ;
		bool			mIsUnderline ;
		wstring			mFont ;
		int				mFontSize ;
		int				mFgColor ;
		int				mBgColor ;
		wstring			mJustification ;
		wstring			mText ;

	// +--------------------------------------------
	// |
	// |			format_package Methods
	// |
	// +--------------------------------------------

	/** default constructor. 
	*/
		format_package() : 
			mFont( L"Arial" )  , 
			mFontSize( 14 ) ,
			mIsBold ( 0 ),
			mIsItalic ( 0 ),
			mIsUnderline ( 0 ),
			mFgColor ( 0 ),
			mBgColor ( 0 ),
			mJustification( L"\\ql " ) 
		{}

	/** constructor. 
	@param package parameter. 
	*/
		format_package( const format_package &package ) 
			: mFont( package.mFont ) , 
			mFontSize( package.mFontSize ) ,
			mIsBold ( package.mIsBold ),
			mIsItalic ( package.mIsItalic ),
			mIsUnderline ( package.mIsUnderline ),
			mFgColor ( package.mFgColor ),
			mBgColor ( package.mBgColor ),
			mJustification( package.mJustification ) ,
			mText( package.mText )
		{}


	/** brief. 
	@return bool description. 
	@param void parameter. 
	*/
		bool HasText( void ) 
		{ 
			return ( mText.length() != 0 ) ; 
		}

	/** Assignment operator. 
	@return format_package &format_package::operator description. 
	@param package parameter. 
	*/
		format_package &operator =( const format_package &package )
		{
			mFont = package.mFont ;
			mFontSize = package.mFontSize ;
			mIsBold = package.mIsBold ;
			mIsItalic = package.mIsItalic ;
			mIsUnderline = package.mIsUnderline ;
			mFgColor = package.mFgColor ;
			mBgColor = package.mBgColor ;
			mJustification = package.mJustification ;
			mText =  package.mText  ;

			return *(this) ;
		}
	} ;


	// class rich_text_formatter
	class rich_text_formatter
	{
	VISIBLE_TO_TESTS

		typedef boost::shared_ptr< format_package > package_pointer ;
		typedef std::vector< package_pointer > package_list ;

		package_pointer	m_package ;
		font_table		m_font_table ;
		package_list	m_text_nodes ;
		wstring			mPlainText ;
		wstring			mHtmlText ;
	public:

		// CTORS
		rich_text_formatter(const package_pointer package);
		rich_text_formatter(const wchar_t *text = L"");
		rich_text_formatter(const wstring &text);
		// set formatting values
		bool format ( const package_pointer package );

		void bold(const bool setting = true );
		void italic(const bool setting = true );
		void underline(const bool setting = true );
		void font_size(const int size = true );
		void font(const wchar_t * const font );
		void font( const wstring &font );
		void back_color(const int color);
		void fore_color(const int color);
		void right(void);
		void center(void);
		void left(void);
		void justify( const wchar_t * const justification );
		void justify( const wstring &justification );
		// set text
		void text(const wchar_t * const text );
		void text(const wstring &text);

		// construction/initialization
		wstring get_formatted_text(void);
		void clear_text( void );
		void add(const wchar_t * const text );
		void add(wstring &text);

		// private helper functions

		void AddCurrentPackage();

		// DoFormatting
		wstring DoFormatting(int index);
	} ;

}

