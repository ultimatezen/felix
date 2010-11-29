// RichTextConversion.cpp: implementation of the RichTextConversion class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "RichTextConversion.h"
#include "StringEx.h"
#include "Exceptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// << TCHAR *
namespace rtf
{
	const TCHAR * const RIGHT_JUSTIFICATION	=TEXT("\\qr ") ; 
	const TCHAR * const CENTER_JUSTIFICATION	=TEXT("\\qc ") ; 
	const TCHAR * const LEFT_JUSTIFICATION	=TEXT("\\ql ") ; 

	const CString RICH_TEXT_HEADER = TEXT("{\\rtf1\\ansi\\ansicpg932\\deff0\\deflang1033\\deflangfe1041")  ;
		
	const CString RICH_TEXT_COLOR_TABLE = TEXT("{\\colortbl ;")													
					TEXT("\\red0\\green0\\blue0;") 			// BLACK		
					TEXT("\\red0\\green0\\blue255;"	) 		// BLUE		
					TEXT("\\red0\\green255\\blue255;") 		// CYAN		
					TEXT("\\red0\\green255\\blue0;"	) 		// GREEN		
					TEXT("\\red255\\green0\\blue255;") 		// MAGENTA		
					TEXT("\\red255\\green0\\blue0;"	) 		// RED			
					TEXT("\\red255\\green255\\blue0;") 		// YELLOW		
					TEXT("\\red255\\green255\\blue255;"	) 	// WHITE		
					TEXT("\\red0\\green0\\blue128;"	) 		// DK_BLUE		
					TEXT("\\red0\\green128\\blue128;") 		// DK_CYAN	
					TEXT("\\red0\\green128\\blue0;"	) 		// DK_GREEN	
					TEXT("\\red128\\green0\\blue128;") 		// DK_MAGENTA 
					TEXT("\\red128\\green0\\blue0;") 			// DK_RED		
					TEXT("\\red128\\green128\\blue0;") 		// DK_YELLOW  
					TEXT("\\red128\\green128\\blue128;") 		// DK_GRAY	
					TEXT("\\red192\\green192\\blue192;") 		// LT_GRAY		
				TEXT("}") ;


	const CString RICH_TEXT_PREAMBLE =	TEXT("\\viewkind4 \\lang1041") ;
	const CString RICH_TEXT_FOOTER =	TEXT("}") ;

	// +--------------------------------------------
	// |
	// |			FontTableEntry Methods
	// |
	// +--------------------------------------------

	FontTable::FontTableEntry & FontTable::FontTableEntry::operator= (FontTableEntry &entry)
	{
		mFamily = entry.mFamily ;
		mCharset= entry.mCharset ;
		mPitch = entry.mPitch ; 
		mName = entry.mName ;
		return *this ;
	}
	bool FontTable::FontTableEntry::operator==( FontTableEntry &entry )
	{
		if( mFamily != entry.mFamily ) 
			return false ;
		if( mCharset!= entry.mCharset ) 
			return false ;
		if( mPitch != entry.mPitch ) 
			return false ; 
		if( mName != entry.mName ) 
			return false ;
		return true ;
	}
	void FontTable::FontTableEntry::FillPitchInfo( BYTE pitch )
	{
		switch ( pitch )
		{
		case FIXED_PITCH:
			mPitch = TEXT("\\fprq1") ;
			break ;
		case VARIABLE_PITCH :
			mPitch = TEXT("\\fprq2") ;
			break ;
		default:
			mPitch = TEXT("") ;
		}
	}
	void FontTable::FontTableEntry::FillFamilyInfo( BYTE family ) 
	{
		switch ( family )
		{
			case FF_ROMAN:
				mFamily = TEXT("\\froman") ;
				break ;
			case FF_SCRIPT:
				mFamily = TEXT("\\fscript") ;
				break ;
			case FF_SWISS :
				mFamily = TEXT("\\fswiss") ;
				break ;
			case FF_DECORATIVE:
				mFamily = TEXT("\\fdecor") ;
				break ;
			case FF_MODERN:
				mFamily = TEXT("\\fmodern") ;
				break ;
			default:
				mFamily = TEXT("") ;
		}
	}
	void FontTable::FontTableEntry::SetCharset( int charset ) 
	{
		mCharset = TEXT("\\charset") ;
		mCharset += int2string( charset ) ;
	}
	void FontTable::FontTableEntry::SetName ( const TCHAR * const name )
	{
		mName = name ;
	}

	// +--------------------------------------------
	// |
	// |			FontTable Methods
	// |
	// +--------------------------------------------
	FontTable::FontTable( ) 
	{
		AddFont( CString( TEXT("Arial") ) ) ;
	}
	FontTable::FontTable(const CString &DefaultFont ) 
	{ 
		AddFont( DefaultFont ) ; 
	}
	void FontTable::SetDefaultFont( CString &DefaultFont )
	{
		FontTableEntry entry = mFonts[0] ;
		mFonts[0] = GetEntryFromName( DefaultFont ) ;
		AddFont( entry ) ;
	}
	// AddFont
	int FontTable::AddFont( const CString &name )
	{
		ATLASSERT( name.GetLength() != 0 ) ;

		int size = mFonts.GetSize() ;
		for ( int i = 0 ; i < size ; ++i )
		{	
			if ( mFonts[i].mName == name )
				return i ;
		}
		FontTableEntry entry = GetEntryFromName( name ) ;
		mFonts.Add( entry ) ;
		return i ;
	}
	int FontTable::AddFont( FontTableEntry &entry )
	{
		int size = mFonts.GetSize() ;
		for ( int i = 0 ; i < size ; ++i )
		{	
			if ( mFonts[i] == entry )
				return i ;
		}
		mFonts.Add( entry ) ;
		return i ;
	}
	FontTable::FontTableEntry FontTable::GetEntryFromName( const CString &name ) 
	{
		MyFont font( name ) ;
		FontTableEntry entry(name) ;
		entry.FillFamilyInfo( font.GetFontFamily() ) ;
		entry.FillPitchInfo( font.GetFontPitch() ) ;
		entry.SetCharset( font.GetCharset() ) ;
		return entry ;
	}
	CString FontTable::GetTable() 
	{
		CString table = TEXT("{\\fonttbl") ;
		int size = mFonts.GetSize() ;
		// special treatment for font 0
		FontTableEntry entry = mFonts[0] ;
		table << TEXT("{\\f") << int2string( 0 ) << TEXT("\\fnil")  << entry.mCharset << entry.mPitch << TEXT(" ") << entry.mName << TEXT("}") ;
		for ( int i = 1 ; i < size ; ++i )
		{	
			entry = mFonts[i] ;
			table << TEXT("{\\f") << int2string( i ) << entry.mFamily << entry.mCharset << entry.mPitch << TEXT(" ") << entry.mName << TEXT("}") ;
		}
		table += TEXT("}") ;
		return table ;
	}
	CString FontTable::GetFontCode(CString &name)
	{
		int size = mFonts.GetSize() ;
		for ( int i = 0 ; i < size ; ++i )
		{
			if ( mFonts[i].mName == name ) 
				return CString( TEXT("\\f") ) + int2string( i ) + CString(TEXT(" ") ) ;
		}
		return CString( TEXT("\\f0 ") ) ;
	}

	// +--------------------------------------------
	// |
	// |			format_package Methods
	// |
	// +--------------------------------------------
	format_package::format_package() : 
		mFont( TEXT("Arial") ) , 
		mFontSize( 14 ) ,
		mIsBold ( 0 ),
		mIsItalic ( 0 ),
		mIsUnderline ( 0 ),
		mFgColor ( AUTO ),
		mBgColor ( AUTO ),
		mJustification( LEFT_JUSTIFICATION ) 
	{}
	format_package::format_package( const format_package &package ) 
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

	bool format_package::HasText( void ) 
	{ 
		return ( mText.GetLength() != 0 ) ; 
	}
	format_package &format_package::operator =( const format_package &package )
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

	bool RichTextFormatter::FormatMarkedUpText(CString &MarkUp)
	{
		add( MarkUp ) ;
		return true ;
	}
	bool RichTextFormatter::FormatMarkedUpText(const TCHAR * const MarkUp)
	{
		add( MarkUp ) ;
		return true ;
	}

	CString RichTextFormatter::get_plain_text()
	{
		return mPlainText ;
	}

	void RichTextFormatter::add_html_text(const TCHAR *text)
	{
		try // look out for access violations
		{
			ATLASSERT( text != NULL ) ;
			mHtmlText = text ;
			while ( *text )
			{
				if ( *text == TEXT('<') )
				{
					text = format_html( text ) ;
				}
				else if ( *text == TEXT('&') )
				{
					text = get_html_symbol( text ) ;
				}
				else 
				{
					mPackage().mText << *text ;
					mPlainText << *text ;
				}
				++text ;
			}
		}
		catch ( Exception e )
		{
			e.HandleException( true, TEXT("Make sure html text is formatted properly") ) ;
		}
	}
	const TCHAR * RichTextFormatter::format_html(const TCHAR *text)
	{
		ATLASSERT( *text == TEXT('<') ) ;
		++ text ;
		CString symbol_string ;
		while ( *text != TEXT('>' ) )
		{
			ATLASSERT( *text != 0 ) ;
			symbol_string << *text ;
			++text ;
		}
		// is it a closing tag?
		bool setting = true ;
		if ( symbol_string[0] == TEXT('/') )// closing tag
		{
			setting = false ;
			symbol_string = symbol_string.Right( symbol_string.GetLength() - 1 ) ;
		}
		// supported format tags:
		//	b
		//	i
		//	u
		if ( symbol_string == TEXT("b") )
			bold( setting ) ;
		else if ( symbol_string == TEXT("i") )
			italic( setting ) ;
		else if ( symbol_string == TEXT("u") )
			underline( setting ) ;

		ATLASSERT( *text == TEXT('>') ) ;
		// eat the closing bracket 
		++ text ;
		return text ;
	}

	const TCHAR * RichTextFormatter::get_html_symbol(const TCHAR *text)
	{
		ATLASSERT( *text == TEXT('&') ) ;
		++text ; // eat the &
		// base case -- it's an ampersand
		if ( *text == TEXT('&') )
		{
			mPlainText << TEXT('&') ;
			mPackage().mText << TEXT('&') ;
			++text ;
			return text ;
		}
		// ok, what's the symbol then?
		CString symbol_string ;
		while ( *text != TEXT(';') )
		{
			ATLASSERT( *text != 0 ) ;
			symbol_string << *text ;
			++text ;
		}
		// supported symbol tags:
		//	lt
		//	gt
		//	quot
		if ( symbol_string == TEXT("lt") )
		{
			mPlainText << TEXT('<') ;
			mPackage().mText << TEXT('<') ;
		}
		else if ( symbol_string == TEXT("gt") )
		{
			mPlainText << TEXT('>') ;
			mPackage().mText << TEXT('>') ;
		}
		else if ( symbol_string == TEXT("quot") )
		{
			mPlainText << TEXT('\"') ;
			mPackage().mText << TEXT('\"') ;
		}
		ATLASSERT( *text == TEXT(';') ) ;
		// eat the closing ; 
		++ text ;
		return text ;

	}

} // namespace rtf


