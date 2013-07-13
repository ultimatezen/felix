// rtf.cpp: implementation of the rtf class.
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "rtf.h"


namespace rtf
{
	using namespace except ;


	void set_default_font_table(textstream_reader< char > &reader)
	{
		reader.set_buffer( 
			// this seems to be the default font table output by Trados.
			"{\\fonttbl "
			"{\\f1 \\fmodern\\fprq1 \\fcharset128 MS UI Gothic;}"
			"{\\f2 \\fswiss\\fprq2 \\fcharset0 Arial;}"
			"{\\f3 \\froman\\fprq2 {\\*\\panose 02020603050405020304}\\fcharset0 Times New Roman;}"
			"{\\f4 \\froman\\fprq1 {\\*\\panose 02020609040205080304}{\\*\\falt MS Mincho}\\fcharset128 \\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9;}"
			"{\\f5 \\froman\\fprq2 {\\*\\panose 02040604050505020304}\\fcharset0 Century;}"
			"{\\f6 \\froman\\fprq1 {\\*\\panose 02020609040205080304}\\fcharset128 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9;}"
			"{\\f7 \\froman\\fprq2 \\fcharset238 Times New Roman CE;}"
			"{\\f8 \\froman\\fprq2 \\fcharset204 Times New Roman Cyr;}"
			"{\\f9 \\froman\\fprq2 \\fcharset161 Times New Roman Greek;}"
			"{\\f10 \\froman\\fprq2 \\fcharset162 Times New Roman Tur;}"
			"{\\f11 \\froman\\fprq2 \\fcharset177 Times New Roman (Hebrew);}"
			"{\\f12 \\froman\\fprq2 \\fcharset178 Times New Roman (Arabic);}"
			"{\\f13 \\froman\\fprq2 \\fcharset186 Times New Roman Baltic;}"
			"{\\f14 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset0 MS Mincho Western;}"
			"{\\f15 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset238 MS Mincho CE;}"
			"{\\f16 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset204 MS Mincho Cyr;}"
			"{\\f17 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset161 MS Mincho Greek;}"
			"{\\f18 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset162 MS Mincho Tur;}"
			"{\\f19 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset186 MS Mincho Baltic;}"
			"{\\f20 \\froman\\fprq1 \\fcharset0 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Western;}"
			"{\\f21 \\froman\\fprq1 \\fcharset238 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 CE;}"
			"{\\f22 \\froman\\fprq1 \\fcharset204 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Cyr;}"
			"{\\f23 \\froman\\fprq1 \\fcharset161 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Greek;}"
			"{\\f24 \\froman\\fprq1 \\fcharset162 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Tur;}"
			"{\\f25 \\froman\\fprq1 \\fcharset186 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Baltic;}"
			"{\\f26 \\fmodern\\fprq2 {\\*\\panose 020b0600070205080204}\\fcharset128 @MS UI Gothic;}"
			"{\\f27 \\fswiss\\fprq2 \\fcharset238 Arial CE;}"
			"{\\f28 \\fswiss\\fprq2 \\fcharset204 Arial Cyr;}"
			"{\\f29 \\fswiss\\fprq2 \\fcharset161 Arial Greek;}"
			"{\\f30 \\fswiss\\fprq2 \\fcharset162 Arial Tur;}"
			"{\\f31 \\fswiss\\fprq2 \\fcharset177 Arial (Hebrew);}"
			"{\\f32 \\fswiss\\fprq2 \\fcharset178 Arial (Arabic);}"
			"{\\f33 \\fswiss\\fprq2 \\fcharset186 Arial Baltic;}"
			"{\\f34 \\fmodern\\fprq2 \\fcharset0 MS UI Gothic Western;}"
			"{\\f35 \\fmodern\\fprq2 \\fcharset238 MS UI Gothic CE;}"
			"{\\f36 \\fmodern\\fprq2 \\fcharset204 MS UI Gothic Cyr;}"
			"{\\f37 \\fmodern\\fprq2 \\fcharset161 MS UI Gothic Greek;}"
			"{\\f38 \\fmodern\\fprq2 \\fcharset162 MS UI Gothic Tur;}"
			"{\\f39 \\fmodern\\fprq2 \\fcharset186 MS UI Gothic Baltic;}"
			"{\\f40 \\fmodern\\fprq2 \\fcharset0 @MS UI Gothic Western;}"
			"{\\f41 \\fmodern\\fprq2 \\fcharset238 @MS UI Gothic CE;}"
			"{\\f42 \\fmodern\\fprq2 \\fcharset204 @MS UI Gothic Cyr;}"
			"{\\f43 \\fmodern\\fprq2 \\fcharset161 @MS UI Gothic Greek;}"
			"{\\f44 \\fmodern\\fprq2 \\fcharset162 @MS UI Gothic Tur;}"
			"{\\f45 \\fmodern\\fprq2 \\fcharset186 @MS UI Gothic Baltic;}}  " 
			) ;
	}


	void advance_to_next_font(textstream_reader< char > &reader)
	{
		// eat the }
		ATLASSERT( reader.current_is( '}' ) ) ;
		reader.advance() ; 
		// advance to next brace
		reader.eat_whitespace() ;
	}

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	my_font::my_font()
	{
		init_font_info() ;
	}

	my_font::my_font( const LOGFONT &font )
	{
		set_font( &font ) ;
	}

	my_font::my_font( const my_font &font )
	{
		set_font( (LOGFONT*)&font ) ;
	}
	
	// assignment
	my_font & my_font::operator=( const LOGFONT &font )
	{
		set_font( &font ) ; 
		return *this ;
	}

	my_font & my_font::operator=( const my_font &font )
	{
		set_font( (LOGFONT*)&font ) ; 
		return *this ;
	}
	
	// getter
	tstring my_font::get_name() const
	{
		return tstring( lfFaceName ) ;
	}

	void my_font::set_font( const LOGFONT *font )
	{
		memcpy( this, font, sizeof(LOGFONT) ) ;
	}

	LONG my_font::get_height() const
	{
		return lfHeight  ;
	}

	void my_font::init_font_info()
	{
		ZeroMemory(this, sizeof(LOGFONT) ) ;
		lfCharSet = DEFAULT_CHARSET ;
	}

	LONG my_font::get_width() const
	{
		return lfWidth  ;
	}

	LONG my_font::get_weight() const
	{
		return lfWeight ;
	}

	void my_font::set_height( LONG height )
	{
		lfHeight = height ;
	}

	void my_font::set_width( LONG width )
	{
		lfWidth = width ;
	}

	void my_font::set_weight( LONG weight )
	{
		lfWeight = weight ;
	}

	void my_font::set_charset( BYTE charset )
	{
		lfCharSet = charset ;
	}

	void my_font::set_pitch( BYTE pitch )
	{
		lfPitchAndFamily |= pitch ;
	}

	void my_font::set_name( LPCTSTR face_name )
	{
		ATLASSERT( face_name != NULL ) ;
		if (!face_name)
		{
			return ;
		}
		ZeroMemory( lfFaceName, sizeof( TCHAR ) * LF_FACESIZE ) ;
		::_tcsncpy_s( lfFaceName, LF_FACESIZE, face_name, LF_FACESIZE-1 ) ;
	}

	void my_font::lookup_font( HWND hwnd /*= ::GetActiveWindow() */ )
	{
		CDC dc( GetDC( hwnd ) ) ;
		::EnumFontFamiliesEx(dc, this, (FONTENUMPROC)EnumFontsProc, (LPARAM)this, 0);
	}

	BYTE my_font::get_font_family() const
	{
		BYTE family = lfPitchAndFamily ;

		if ( family & FF_DECORATIVE ) return FF_DECORATIVE ; 
		if ( family & FF_MODERN ) return FF_MODERN ; 
		if ( family & FF_ROMAN ) return FF_ROMAN ; 
		if ( family & FF_SCRIPT ) return FF_SCRIPT ; 
		if ( family & FF_SWISS ) return FF_SWISS ; 

			return FF_DONTCARE ;
	}

	BYTE my_font::get_font_pitch() const
	{
		BYTE pitch = lfPitchAndFamily ;
		if ( pitch & FIXED_PITCH ) return FIXED_PITCH ; 
		if ( pitch & VARIABLE_PITCH ) return VARIABLE_PITCH ; 

			return DEFAULT_PITCH ;
	}

	BYTE my_font::get_charset() const 
	{
		return lfCharSet ;
	}
	
	
	
	// Function name	: EnumFontsProc
	// Description	    : 
	// Return type		: BOOL CALLBACK 
	// Argument         : LPLOGFONT lplf
	// Argument         : LPNEWTEXTMETRIC lpntm
	// Argument         : DWORD FontType
	// Argument         : LPVOID fontclass
	BOOL CALLBACK EnumFontsProc(LPLOGFONT lplf, LPNEWTEXTMETRIC lpntm, DWORD FontType, LPVOID fontclass ) 
	{ 
		
		// this is mainly just to keep the compiler happy by touching all the vals.
		if ( FontType == TRUETYPE_FONTTYPE )
		{
			lplf->lfCharSet = lpntm->tmCharSet ;
		}
		
		// set the font info
		static_cast< my_font* >( fontclass )->set_font( lplf ) ;
		// just get the first matching font
		return FALSE ;
	} 

	UINT getCodepageFromCharset( BYTE charset )
	{
		switch( charset )
		{
			// ANSI_CHARSET
		case 0:
			return 1252 ; // Latin I

			// SYMBOL_CHARSET
		case 2:
			{
				//CSSFileDialog dlg(FALSE) ;
				//if ( dlg.IsEnhancedWinVer() ) 
				//{
					return CP_SYMBOL ; // Only good on Win 2000/XP !
				//}
				//return CP_OEMCP ;
			}

			// SHIFTJIS_CHARSET
		case 128:
			return 932 ; // ANSI/OEM - Japanese, Shift-JIS 

			// Hanguel
		case 129 :  
			return 949 ; // ANSI/OEM - Korean (Unified Hanguel Code) 

			// Hanguel (Johab)
		case 130: 
			return 1361 ; // Korean (Johab) 

			// Chinese_GB2312
		case 134:
			return 936 ; // ANSI/OEM - Simplified Chinese (PRC, Singapore) 

			//  Chinese_BIG5     
		case 136:
			return 950 ; // ANSI/OEM - Traditional Chinese (Taiwan; Hong Kong SAR, PRC)  

			// GREEK_CHARSET
		case 161:
			return 1253 ; // ANSI - Greek 

			// TURKISH_CHARSET
		case 162:
			return 1254 ; // ANSI - Turkish 

			// HEBREW_CHARSET
		case 177:
			return 1255 ; // ANSI - Hebrew 

			// ARABICSIMPLIFIED_CHARSET
		case 178:
			return 708 ; // Arabic - ASMO 708 

			// ARABICTRADITIONAL_CHARSET
		case 179:
			return 709 ; // Arabic - ASMO 449+, BCON V4 

			// ARABICUSER_CHARSET
		case 180:
			return 864 ; //OEM - Arabic 
			// 710 Arabic - Transparent Arabic 

			// HEBREWUSER_CHARSET
		case 181:
			return 862 ; // OEM - Hebrew 

		case 186:
			return 28594 ; // Baltic Alphabet (ISO) 
			// CYRILLIC_CHARSET
		case 204:
			return 1251 ; // ANSI - Cyrillic 

			// EASTERNEUROPE_CHARSET
		case 238:
			return 1250 ; // ANSI - Latin II 

			// PC437_CHARSET ????
		case 254:
			return CP_ACP ; // ** DON'T KNOW WHAT THIS IS, SO PUNT

			// OEM_CHARSET
		case 255:
			return CP_OEMCP ; 

		default:
			return CP_ACP ;
		}
	}



	wstring rich_text_formatter::DoFormatting( int index )
	{
		format_package &package = *(m_text_nodes[index]) ;

		wstring output(package.mJustification) ;
		output << L"\\highlight" ;
		output += int2wstring(package.mBgColor) ;
		output +=L" " ;
		// text block
		output +=L"{" ;
		output << L"\\cf" << int2wstring(package.mFgColor) ; 
		output << L"\\fs" << int2wstring(package.mFontSize) ; 
		m_font_table.add_font( string2tstring( package.mFont ) ) ;
		output << string2wstring( m_font_table.get_font_entry( string2tstring( package.mFont ) ).m_code ) ; 
		if ( package.mIsBold )
			output +=L"\\b " ;
		if ( package.mIsItalic )
			output +=L"\\I " ;
		if ( package.mIsUnderline )
			output +=L"\\ul " ;
		output << package.mText << RICH_TEXT_FOOTER ;
		return output ;
	}

	void rich_text_formatter::AddCurrentPackage()
	{
		ATLASSERT ( m_package->HasText() ) ;

		// At the formatted text to our array
		m_text_nodes += m_package ;
		package_pointer package(new format_package(*m_package)) ;
		std::swap(package, m_package) ;
		clear_text() ;
	}

	void rich_text_formatter::add( const wstring &text )
	{
		if ( m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mText = text  ;
	}

	void rich_text_formatter::add( const wchar_t * const text )
	{
		wstring wtext(text) ;
		add( wtext ) ;
	}

	void rich_text_formatter::clear_text( void )
	{
		m_package->mText.erase() ;
	}

	wstring rich_text_formatter::get_formatted_text( void )
	{
		wstring text ;
		text << RICH_TEXT_HEADER << string2wstring( m_font_table.get_table() ) << RICH_TEXT_COLOR_TABLE << RICH_TEXT_PREAMBLE ;
		if ( m_package->HasText() )
			AddCurrentPackage() ;

		size_t num_nodes = m_text_nodes.size() ;
		for ( size_t i = 0 ; i < num_nodes ; ++i )
		{
			text += DoFormatting( static_cast< int >( i ) ) ;
		}
		text += RICH_TEXT_FOOTER ;
		return text ;
	}

	void rich_text_formatter::text( const wstring &text )
	{
		m_package->mText += text ;
	}

	void rich_text_formatter::text( const wchar_t * const text )
	{
		m_package->mText += text ;
	}

	void rich_text_formatter::justify( const wstring &justification )
	{
		ATLASSERT( m_package->mJustification == L"\\qr " ||
			m_package->mJustification == L"\\qc " ||
			m_package->mJustification == L"\\ql " ) ;

		if ( justification != m_package->mJustification && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mJustification = justification ;
	}

	void rich_text_formatter::justify( const wchar_t * const justification )
	{
		if ( justification != m_package->mJustification && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mJustification = justification ;
	}

	void rich_text_formatter::left( void )
	{
		if ( L"\\ql " != m_package->mJustification && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mJustification = L"\\ql " ;
	}

	void rich_text_formatter::center( void )
	{
		if ( L"\\qc " != m_package->mJustification && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mJustification = L"\\qc " ;
	}

	void rich_text_formatter::right( void )
	{
		if ( L"\\qr " != m_package->mJustification && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mJustification = L"\\qr " ;
	}

	void rich_text_formatter::fore_color( const int color )
	{
		ATLASSERT ( RTF_AUTO <= color && color <= RTF_LT_GRAY ) ;
		if ( color != m_package->mFgColor && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mFgColor = color ;
	}

	void rich_text_formatter::back_color( const int color )
	{
		ATLASSERT ( RTF_AUTO <= color && color <= RTF_LT_GRAY ) ;
		if ( color != m_package->mBgColor && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mBgColor = color  ;
	}

	void rich_text_formatter::font( const wstring &font )
	{
		if ( font != m_package->mFont && m_package->HasText() )
			AddCurrentPackage() ;

		m_font_table.add_font( string2tstring( font ) ) ;
		m_package->mFont = font ;
	}

	void rich_text_formatter::font( const wchar_t * const font )
	{
		this->font( wstring( font ) ) ;
	}

	void rich_text_formatter::font_size( const int size /*= true */ )
	{
		if ( size != m_package->mFontSize && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mFontSize = size  ;
	}

	bool rich_text_formatter::format( const package_pointer package )
	{
		if ( m_package->HasText() )
		{
			AddCurrentPackage() ;
		}

		back_color( package->mBgColor ) ;
		fore_color( package->mFgColor ) ;
		font( package->mFont ) ;
		font_size( package->mFontSize ) ;
		bold( package->mIsBold ) ;
		italic( package->mIsItalic ) ;
		underline( package->mIsUnderline ) ;
		justify( package->mJustification ) ;
		if ( package->HasText() )
		{
			text( package->mText ) ;
		}
		return true ;
	}

	rich_text_formatter::rich_text_formatter( const package_pointer package ) :
		m_package( package ), 
		m_font_table( true )
	{
		m_font_table.add_font( _T("Arial") ) ;
	}

	rich_text_formatter::rich_text_formatter( const wchar_t *text /*= L"" */ ) : 
		m_font_table( true ),
		m_package(new format_package)
	{
		m_package->mText = text ; 
		m_font_table.add_font( _T("Arial") ) ;
	}

	rich_text_formatter::rich_text_formatter( const wstring &text ) : 
		m_font_table( true ),
		m_package(new format_package)
	{
		m_package->mText = text ; 
		m_font_table.add_font( _T("Arial") ) ;
	}

	void rich_text_formatter::bold( const bool setting /*= true */ )
	{
		if ( setting != m_package->mIsBold && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mIsBold = setting  ;
	}

	void rich_text_formatter::italic( const bool setting /*= true */ )
	{
		if ( setting != m_package->mIsItalic && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mIsItalic = setting ;
	}

	void rich_text_formatter::underline( const bool setting /*= true */ )
	{
		if ( setting != m_package->mIsUnderline && m_package->HasText() )
			AddCurrentPackage() ;

		m_package->mIsUnderline = setting ;
	}

	/************************************************************************/
	/* font_table_entry                                                     */
	/************************************************************************/
	font_table_entry & font_table_entry::operator=( const my_font &font )
	{
		m_font = font ;
		FillInfoFromFont() ;
		return *this ;
	}

	font_table_entry & font_table_entry::operator=( const font_table_entry &entry )
	{
		m_font = entry.m_font ; 
		m_family = entry.m_family ;
		m_charset= entry.m_charset ;
		m_pitch = entry.m_pitch ; 
		m_name = entry.m_name ;
		m_code = entry.m_code ;
		return *this ;
	}

	bool font_table_entry::operator==( const font_table_entry &entry )
	{
		if( m_family != entry.m_family ) 
			return false ;
		if( m_charset!= entry.m_charset ) 
			return false ;
		if( m_pitch != entry.m_pitch ) 
			return false ; 
		if( m_name != entry.m_name ) 
			return false ;

		return true ;
	}

	void font_table_entry::set_code( int code )
	{
		m_code = _T("\\f") + int2tstring( code ) + _T(" ") ;
	}

	void font_table_entry::fill_pitch_info( BYTE pitch )
	{
		switch ( pitch )
		{
		case FIXED_PITCH:
				m_pitch = _T("\\fprq1 ") ;
			break ;
		case VARIABLE_PITCH :
				m_pitch = _T("\\fprq2 ") ;
			break ;
		default:
			m_pitch = _T(" ") ;
		}
	}

	void font_table_entry::fill_family_info( BYTE family )
	{
		switch ( family )
		{
		case FF_ROMAN:
				m_family = _T("\\froman") ;
			break ;
			case FF_SCRIPT :
				m_family = _T("\\fscript") ;
			break ;
		case FF_SWISS :
				m_family = _T("\\fswiss") ;
			break ;
		case FF_DECORATIVE:
				m_family = _T("\\fdecor") ;
			break ;
		case  FF_MODERN :
				m_family = _T("\\fmodern") ;
			break ;
		default:
			m_family = _T("") ;
		}
	}

	void font_table_entry::set_charset( BYTE charset )
	{
		m_charset = _T("\\charset") ;
		m_charset += int2tstring( charset ) ;
		m_charset += _T(" ") ;
	}

	void font_table_entry::FillInfoFromFont()
	{
		m_name = m_font.get_name() ;
		fill_family_info( m_font.get_font_family() ) ;
		fill_pitch_info( m_font.get_font_pitch() ) ;
		set_charset( m_font.get_charset() ) ;
	}

	/************************************************************************/
	/* font_table                                                           */
	/************************************************************************/
	font_table::font_table( bool has_default /*= false */ ) : 
		m_has_default( has_default )
	{

	}

	void font_table::clear()
	{
		m_fonts.clear() ;
	}

	size_t font_table::size() const
	{
		return m_fonts.size() ;
	}


	tstring font_table::get_font_name( const size_t code ) const
	{
		if ( code >= m_fonts.size() )
		{
			throw except::CException( TEXT("Font not found") ) ;
		}
		return m_fonts[code].m_name ;
	}

	font_table_entry & font_table::get_font_entry( const tstring name )
	{
		ATLASSERT( name.empty() == false ) ;
		for ( size_t i = 0 ; i < m_fonts.size() ; ++i )
		{	
			if (  boost::iequals( m_fonts[i].m_name, name ) )
				return m_fonts[i] ;
		}
		throw except::CException( _T("Font not found") ) ;
	}

	font_table_entry & font_table::get_font_entry( const size_t code )
	{
		if ( code >= m_fonts.size() )
		{
			throw except::CException( TEXT("Font not found") ) ;
		}
		return m_fonts[code] ;
	}

	font_table_entry & font_table::get_font_entry_from_code( const tstring code )
	{
		for ( size_t i = 0 ; i < m_fonts.size() ; ++i )
		{	
			if ( boost::iequals( m_fonts[i].m_code, code ) )
			{
				return m_fonts[i] ;
			}
		}
		throw except::CException( TEXT("Font not found (") + CString(code.c_str()) + TEXT(")")  ) ;
	}

	bool font_table::is_font_in_table( const tstring name )
	{
		if ( name.empty() ) return false ;

		for ( size_t i = 0 ; i < m_fonts.size() ; ++i )
		{	
			if ( boost::iequals( m_fonts[i].m_name, name ) ) return true ;
		}
		return false ;
	}

	int font_table::add_font( const tstring name )
	{
		if ( name.empty() ) return 0 ;

		size_t i ;
		for ( i = 0 ; i < m_fonts.size() ; ++i )
		{	
			if ( boost::iequals( m_fonts[i].m_name, name ) )
			{
				return static_cast< int >( i ) ;
			}
		}
		font_table_entry entry( get_entry_from_name( name ) ) ;
		entry.set_code( ( m_has_default ? static_cast< int >( i ) : static_cast< int >( i ) + 1 ) ) ;
		m_fonts += entry ;
		return static_cast< int >( i ) ;
	}

	int font_table::add_font( const my_font &font )
	{
		size_t i ;
		for ( i = 0 ; i < m_fonts.size() ; ++i )
		{	
			if ( m_fonts[i].m_name == font.get_name() )
			{
				return static_cast< int >( i ) ;
			}
		}
		font_table_entry entry( font ) ;
		entry.set_code( ( m_has_default ? static_cast< int >( i ) : static_cast< int >( i ) + 1 ) ) ;
		m_fonts += entry ;
		return static_cast< int >( i ) ;
	}

	int font_table::add_font( font_table_entry &entry )
	{
		if ( entry.m_code.empty() )
		{
			size_t i ;
			for ( i = 0 ; i < m_fonts.size() ; ++i )
			{	
				if ( m_fonts[i] == entry )
					return static_cast< int >( i ) ;
			}
			entry.set_code( ( m_has_default ? static_cast< int >( i ) : static_cast< int >( i ) + 1 ) ) ;
		}

		m_fonts += entry ;
		return static_cast< int >( m_fonts.size() ) - 1 ;
	}

	rtf::font_table_entry font_table::get_entry_from_name( const tstring font_name ) const
	{
		my_font font ;
		font.set_name( font_name.c_str() ) ;
		font.lookup_font() ;

		font_table_entry entry( font ) ;
		return entry ;
	}

	string font_table::get_table() const
	{
		string table = ("{\\fonttbl") ;
		for ( size_t i = 0 ; i < m_fonts.size() ; ++i )
		{	
			string line ;
			line << "\r\n{" 
				<< CT2A( m_fonts[i].m_code.c_str() ) 
				<< CT2A( m_fonts[i].m_family.c_str() )  
				<< CT2A( m_fonts[i].m_pitch.c_str() )  
				<< CT2A( m_fonts[i].m_charset.c_str() )  
				<< escape_string( m_fonts[i].m_name ) 
				<< ";}" ;
			boost::replace_all( line, string("  "), string(" ") ) ;
			table << line ;
		}
		table += ("}\r\n") ;
		return table ;
	}

	BYTE font_table::get_charset_code( const tstring font_name )
	{
		my_font font ;
		font.set_name( font_name.c_str() ) ;
		font.lookup_font() ;
		return font.get_charset() ;
	}

	void font_table::parse_fonttbl( c_reader &reader )
	{
		if ( reader.find( "{\\fonttbl", true ) == false ) // some Trados files have no font table
		{
			reader.rewind() ;

			setDefaultFontTable();

			return ;
		}


		reader.jump_to_first_of('{', false ) ;

		while ( reader.current_is( '{' ) )
		{
			// eat the {
			reader.advance() ;


			// we know that the first tag is font num
			const string code = reader.getline(' ', true ) ;

			rtf::font_table_entry entry ;	// the entry
			entry.m_code = string2tstring( code, CP_ACP ) ;

			rtf::my_font font ;				// the font
			readFontInfo( font, reader );

			font.lookup_font() ;
			entry = font ;
			add_font( entry ) ;
			font.init_font_info() ;

			advance_to_next_font(reader) ;
		}
	}

	void font_table::setDefaultFontTable()
	{
		c_reader tmp_reader ;
		set_default_font_table(tmp_reader) ;
		parse_fonttbl(tmp_reader) ;
	}

	void font_table::readFontInfo( my_font &font, c_reader &reader )
	{
		while ( ! reader.current_is( '}' ) )
		{
			reader.jump_to_first_of( char('\\'), true ) ;
			string tag ;
			reader.getline( tag, " \\{", false ) ;

			// check for font family
			if ( tag == "fdecorative" || tag == "fdecor" )
				font.set_family( FF_DECORATIVE ) ;
			else if ( tag == "fmodern" )
				font.set_family( FF_MODERN ) ;
			else if ( tag == "froman" )
				font.set_family( FF_ROMAN ) ;
			else if ( tag == "fscript" )
				font.set_family( FF_SCRIPT ) ;
			else if ( tag == "fswiss" )
				font.set_family( FF_SWISS ) ;
			// check for pitch 
			else if ( tag == "fprq1" )
				font.set_pitch( FIXED_PITCH ) ;
			else if ( tag == "fprq2" )
				font.set_pitch( VARIABLE_PITCH ) ;
			else if ( str::left(tag, 8 ) == "fcharset" || str::left(tag,7) == "charset" )
			{
				if ( str::left(tag, 8 ) == "fcharset" )
				{
					font.set_charset( static_cast< unsigned char >(boost::lexical_cast< short >( tag.substr(8) ) ) ) ;
				}
				else
				{
					font.set_charset( static_cast< unsigned char >(boost::lexical_cast< short >( tag.substr(7) ) ) ) ;
				}
				reader.advance() ;
				tag = reader.getline(';', true ) ;
				font.set_name( CA2T( unescape_string( tag ).c_str() ) ) ;
			}

			if( reader.current_is( '{' ) )
			{
				reader.jump_to_first_of( char('}'), true ) ; 
			}
		}
	}
}
