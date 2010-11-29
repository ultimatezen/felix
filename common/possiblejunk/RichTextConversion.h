// RichTextConversion.h: interface for the RichTextConversion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHTEXTCONVERSION_H__E1DAB527_EF2B_4191_B552_DEA3636EF8CE__INCLUDED_)
#define AFX_RICHTEXTCONVERSION_H__E1DAB527_EF2B_4191_B552_DEA3636EF8CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stringex.h" // string extensions
#include "ATLBASE.H" // CSimpleArray
#include "MiscWrappers.h" // MyFont et al
#include "RefPointer.h" // Referenced pointers

namespace rtf
{
	// colors (from word)
	enum 
	{	AUTO,
		BLACK, BLUE, CYAN, GREEN, MAGENTA, RED, YELLOW, WHITE, DK_BLUE,		
		DK_CYAN, DK_GREEN, DK_MAGENTA, DK_RED, DK_YELLOW, DK_GRAY, LT_GRAY 
	} ;

	const CString MS_P_GOTHIC = TEXT("‚l‚r ‚oƒSƒVƒbƒN") ;
	const CString nl = TEXT("\r ") ;
	const CString par = TEXT("\\par ") ;
	const CString vbnl = TEXT("\r\n ") ;
	const CString vbCrLf = TEXT("\r\n ") ;


	// class FontTable
	class FontTable
	{	
	private:
		// struct FontTableEntry
		struct FontTableEntry
		{
			CString	mFamily ;
			CString mCharset ;
			CString mPitch ;
			CString mName ;
			FontTableEntry( const TCHAR * const name = NULL ) : mName(name) {}
			FontTableEntry( FontTableEntry &entry ) 
				: mFamily(entry.mFamily), mCharset(entry.mCharset), mPitch(entry.mPitch), mName(entry.mName)
			{}
			FontTableEntry & operator= (FontTableEntry &entry) ;
			bool operator==( FontTableEntry &entry ) ;
			void FillPitchInfo( BYTE pitch ) ;
			void FillFamilyInfo( BYTE family ) ; 
			void SetCharset( int charset ) ;
			void SetName ( const TCHAR * const name ) ;
		} ;
		CSimpleArray< FontTableEntry > mFonts ;

		public:
		operator CString() { return GetTable() ; }
		CString GetTable( void ) ;
		CString GetFontCode(CString &name) ;
		FontTable( ) ;
		FontTable(const CString &DefaultFont ) ;
		void SetDefaultFont( CString &DefaultFont ) ;
		int AddFont( const CString &name ) ;
		int AddFont( FontTableEntry &entry ) ;
		FontTableEntry GetEntryFromName( const CString &name ) ;

	} ;

	//struct format_package
	struct format_package
	{
		bool			mIsBold ;
		bool			mIsItalic ;
		bool			mIsUnderline ;
		CString			mFont ;
		int				mFontSize ;
		int				mFgColor ;
		int				mBgColor ;
		CString			mJustification ;
		CString			mText ;

		format_package();
		format_package( const format_package &package) ;
		format_package &operator =( const format_package &package ) ;
		bool HasText( void ) ;
	} ;

	// class RichTextFormatter
	class RichTextFormatter
	{
		typedef ref_pointer< format_package > package_pointer ;

		package_pointer	mPackage ;
		FontTable		mFontTable ;
		CSimpleArray< package_pointer > mTextNodes ;
		CString			mPlainText ;
		CString			mHtmlText ;
	public:
		void add_html_text( const TCHAR *text );
		CString get_plain_text( void );
		bool FormatMarkedUpText( CString &MarkUp );
		bool FormatMarkedUpText( const TCHAR * const MarkUp );
		operator CString (void){ return get_formatted_text() ; }
		bool format ( format_package &package ) ;
		RichTextFormatter( format_package &package ) ;
		RichTextFormatter(const TCHAR *text =_T("") ) ;
		RichTextFormatter(const CString &text) ;
		// set formatting values
		void bold(const bool setting = true) ;
		void italic(const bool setting = true) ;
		void underline(const bool setting = true) ;
		void font_size(const int size) ;
		void font(const TCHAR * const font) ;
		void font(const CString &font) ;
		void back_color(const int color) ; 
		void fore_color(const int color) ;
		void right(void) ;
		void center(void) ;
		void left(void) ;
		void justify( const TCHAR * const justification ) ;
		void justify( const CString &justification ) ;

		// set text
		void text(const TCHAR * const text ) ;
		void text(const CString &text) ;

		// construction/initialization
		CString get_formatted_text(void) ;
		void clear(void) ;
		void clear_text( void ) ;
		void add(const TCHAR * const text ) ;
		void add(CString &text) ;
		void reset_formatting(void) ;
	private: // private helper functions
		const TCHAR * get_html_symbol( const TCHAR *text );
		const TCHAR * format_html( const TCHAR *text);
		// AddCurrentPackage
		void AddCurrentPackage( void );
		// DoFormatting
		CString DoFormatting(int index) ;
	} ;


} // namespace rtf

inline	rtf::RichTextFormatter & operator <<(rtf::RichTextFormatter &format, const CString &TextToAdd)
{
	format.text ( TextToAdd ) ;
	return format ;
}

inline rtf::RichTextFormatter & operator <<(rtf::RichTextFormatter &format, const TCHAR *const TextToAdd)
{
	format.text ( TextToAdd ) ;
	return format ;
}


#endif // !defined(AFX_RICHTEXTCONVERSION_H__E1DAB527_EF2B_4191_B552_DEA3636EF8CE__INCLUDED_)
