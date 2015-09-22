// ScintillaEdit.cpp: implementation of the CScintillaEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ScintillaEdit.h"

#include "Path.h"
#include "File.h"
#include "MultiLanguage.h"
#include "textstream_reader.h"
#include "logging.h"

using namespace except ;

const char htmlKeyWords[] = 
"a abbr acronym address applet area b base basefont "
"bdo big blockquote body br button caption center "
"cite code col colgroup dd del dfn dir div dl dt em "
"fieldset font form frame frameset h1 h2 h3 h4 h5 h6 "
"head hr html i iframe img input ins isindex kbd label "
"legend li link map menu meta noframes noscript "
"object ol optgroup option p param pre q s samp "
"script select small span strike strong style sub sup "
"table tbody td textarea tfoot th thead title tr tt u ul "
"var xmlns "
"abbr accept-charset accept accesskey action align alink "
"alt archive axis background bgcolor border "
"cellpadding cellspacing char charoff charset checked cite "
"class classid clear codebase codetype color cols colspan "
"compact content coords "
"data datafld dataformats datapagesize datasrc datetime "
"declare defer dir disabled enctype "
"face for frame frameborder "
"headers height href hreflang hspace http-equiv "
"id ismap label lang language link longdesc "
"marginwidth marginheight maxlength media method multiple "
"name nohref noresize noshade nowrap "
"object onblur onchange onclick ondblclick onfocus "
"onkeydown onkeypress onkeyup onload onmousedown "
"onmousemove onmouseover onmouseout onmouseup "
"onreset onselect onsubmit onunload "
"profile prompt readonly rel rev rows rowspan rules "
"scheme scope shape size span src standby start style "
"summary tabindex target text title type usemap "
"valign value valuetype version vlink vspace width "
"text password checkbox radio submit reset "
"file hidden image "
"public !doctype xml";

const char jsKeyWords[] = 
"break case catch continue default "
"do else for function if return throw try var while";

const char vbsKeyWords[] = 
"and as byref byval case call const "
"continue dim do each else elseif end error exit false for function global "
"goto if in loop me new next not nothing on optional or private public "
"redim rem resume select set sub then to true type while with "
"boolean byte currency date double integer long object single string type "
"variant";

UINT get_textfile_encoding( const file::BYTE_ORDER_MARK bom, LPSTR ftext, const size_t fsize, bool is_tagged) 
{
	switch( bom )
	{
	case file::BOM_LE:
		return CP_UNICODE ;
	case file::BOM_BE:
		return CP_UNICODE_BE ;
	case file::BOM_UTF8:
		return CP_UTF8 ;
	case file::BOM_UTF7:
		return CP_UTF7 ;
	default:
		try
		{

			CMultiLanguage multi ;

			DWORD flag = 0 ;

			if ( is_tagged )
			{
				flag |= MLDETECTCP_HTML ;
			}

			DetectEncodingInfo detect = multi.detect_encoding( ftext, fsize, flag ) ;

			// catch spurious utf-16 as 1252
			if (detect.nCodePage != CP_UNICODE && detect.nCodePage != CP_UNICODE_BE)
			{
				size_t count=0 ;
				for (size_t i=0 ; i < 100 && i < fsize ; ++i)
				{
					if (ftext[i] == 0)
					{
						++count ;
					}
				}
				if (count > 30)
				{
					return CP_UNICODE ;
				}
			}
			return detect.nCodePage ;


		}
		catch ( CComException &e ) 
		{
			logging::log_error("Failed to detect input character encoding for text file") ;
			logging::log_exception(e) ;
			return CP_ACP ;
		}
	}
}

void CScintillaEdit::set_correct_lexer(const CString &location)
{
	if ( location.IsEmpty() ) return ;

	const file::CFileExtension ext( location ) ;

	// html files
	if ( ext.is_html() ) 
	{
		SENSE("lexer:html") ;
		SetLexer( SCLEX_HTML ) ;
	}
	// xml files
	else if ( ext.is_xml() ) 
	{
		SENSE("lexer:xml") ;
		SetLexer( SCLEX_XML ) ;	
	}
	// PHP files
	else if ( ext.is_php() ) 
	{
		SENSE("lexer:php") ;
		SetLexer( SCLEX_PHP ) ;	
	}
	// Python files
	else if ( ext.is_python() ) 
	{
		SENSE("lexer:python") ;
		SetLexer( SCLEX_PYTHON ) ;	
	}
	// css files
	else if ( ext.is_css() )
	{
		SENSE("lexer:css") ;
		SetLexer( SCLEX_CSS ) ;
	}
	// c++ files
	else if ( ext.is_cpp() )
	{
		SENSE("lexer:cpp") ;
		SetLexer( SCLEX_CPP ) ;
	}
	// perl files
	else if ( ext.is_perl() )
	{
		SENSE("lexer:perl") ;
		SetLexer( SCLEX_PERL ) ;
	}
	// makefiles
	else if ( ext.equals( _T(".mak") ) )
	{
		SENSE("lexer:mak") ;
		SetLexer( SCLEX_MAKEFILE ) ;
	}
	// batch files
	else if ( ext.equals( _T(".bat") ) )
	{
		SENSE("lexer:bat") ;
		SetLexer( SCLEX_BATCH ) ;
	}
	// visual basic files
	else if (	ext.equals( _T(".bas") )  || 
				ext.equals( _T(".fm") )  || 
				ext.equals( _T(".cls") )
			)
	{
		SENSE("lexer:vb") ;
		SetLexer( SCLEX_VB ) ;
	}
	// vbscript files
	else if ( ext.equals( _T(".vbs") ) )
	{
		SENSE("lexer:vbs") ;
		SetLexer( SCLEX_VBSCRIPT ) ;
	}
	// javascript
	else if ( ext.equals( _T(".js" ) ) )
	{
		SENSE("lexer:js") ;
		SetLexer( SCLEX_VBSCRIPT ) ;
	}
	// asp
	else if ( ext.equals( _T(".asp") ) )
	{
		SENSE("lexer:asp") ;
		SetLexer( SCLEX_ASP ) ;
	}
	// default
	else
	{
		SENSE("lexer:null") ;
		ATLTRACE( "Unknown file extension!\n" ) ;
		SetLexer( SCLEX_NULL );
	}
	
}

void CScintillaEdit::set_working_encoding(const UINT codepage)
{
	m_working_encoding = codepage ;
}



UINT CScintillaEdit::get_working_encoding()
{
	return m_working_encoding ;
}

void CScintillaEdit::set_up_standard_syntax_coloring()
{
	SetCodePage( SC_CP_UTF8 ) ;
	SetStyleBits( 7 ) ;

	SetKeyWords( 0, htmlKeyWords ) ;
	SetKeyWords( 1, jsKeyWords ) ;
	SetKeyWords( 2, vbsKeyWords ) ;

	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetAStyle( STYLE_DEFAULT, COLOR_BLACK, COLOR_WHITE, 11, "MS UI Gothic") ;
	StyleClearAll( ) ;	// Copies global style to all others
	SetWrapMode( SC_WRAP_WORD ) ;


	// Hypertext default is used for all the document's text
	SetAStyle( SCE_H_DEFAULT, COLOR_BLACK, COLOR_WHITE, 11, "MS UI Gothic");

	// Unknown tags and attributes are highlighted in COLOR_RED. 
	// If a tag is actually OK, it should be added in lower case to the htmlKeyWords string.

	SetAStyle(SCE_H_TAG,				COLOR_DARK_BLUE,		COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_TAGUNKNOWN,			COLOR_RED,			COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_ATTRIBUTE,			COLOR_DARK_BLUE,		COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_ATTRIBUTEUNKNOWN,	COLOR_RED,			COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_NUMBER,				COLOR_VIOLET,			COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_DOUBLESTRING,		RGB(0,0x80,0),	COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_SINGLESTRING,		RGB(0,0x80,0),	COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_OTHER,				COLOR_VIOLET,			COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_COMMENT,			COLOR_GOLD,			COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_ENTITY,				COLOR_VIOLET,			COLOR_WHITE, 11, "MS UI Gothic");

	SetAStyle(SCE_H_TAGEND,		COLOR_DARK_BLUE, COLOR_WHITE, 11, "MS UI Gothic");
	SetAStyle(SCE_H_XMLSTART,	COLOR_DARK_BLUE, COLOR_WHITE, 11, "MS UI Gothic");		// <?
	SetAStyle(SCE_H_XMLEND,		COLOR_DARK_BLUE, COLOR_WHITE, 11, "MS UI Gothic");		// ?>
	SetAStyle(SCE_H_SCRIPT,		COLOR_DARK_BLUE );									// <script
	SetAStyle(SCE_H_ASP,		RGB(0x4F, 0x4F, 0), RGB(0xFF, 0xFF, 0));	// <% ... %>
	SetAStyle(SCE_H_ASPAT,		RGB(0x4F, 0x4F, 0), RGB(0xFF, 0xFF, 0));	// <%@ ... %>

	SetAStyle(SCE_HB_DEFAULT,		COLOR_BLACK) ;
	SetAStyle(SCE_HB_COMMENTLINE,	COLOR_DARK_GREEN) ;
	SetAStyle(SCE_HB_NUMBER,		COLOR_TURQUOISE) ;
	SetAStyle(SCE_HB_WORD,			COLOR_DARK_BLUE) ;	
		StyleSetBold( SCE_HB_WORD, true ) ;
	SetAStyle(SCE_HB_STRING,		COLOR_VIOLET) ;
	SetAStyle(SCE_HB_IDENTIFIER,	COLOR_BLACK) ;

	// Show the whole section of VBScript with light blue background
	for (int bstyle=SCE_HB_DEFAULT; bstyle<=SCE_HB_STRINGEOL; bstyle++) 
	{
		StyleSetFont( bstyle,"MS UI Gothic" ) ;
		StyleSetBack( bstyle, COLOR_LIGHT_BLUE ) ;

		// This call extends the backround colour of the last style on the line to the edge of the window
		StyleSetEOLFilled( bstyle, true ) ;
	}

	StyleSetBack( SCE_HB_STRINGEOL, RGB(0x7F,0x7F,0xFF) ) ;
	StyleSetFont( SCE_HB_COMMENTLINE, "MS UI Gothic" ) ;

	SetAStyle(SCE_HBA_DEFAULT, COLOR_BLACK);
	SetAStyle(SCE_HBA_COMMENTLINE, COLOR_DARK_GREEN);
	SetAStyle(SCE_HBA_NUMBER, COLOR_TURQUOISE);
	SetAStyle(SCE_HBA_WORD, COLOR_DARK_BLUE);
	StyleSetBold( SCE_HBA_WORD, true ) ;
	SetAStyle(SCE_HBA_STRING, COLOR_VIOLET);
	SetAStyle(SCE_HBA_IDENTIFIER, COLOR_BLACK);
	
	// Show the whole section of ASP VBScript with bright yellow background
	for (int bastyle=SCE_HBA_DEFAULT; bastyle<=SCE_HBA_STRINGEOL; bastyle++) 
	{
		StyleSetFont( bastyle,"MS UI Gothic" ) ;
		StyleSetBack( bastyle, RGB(0xFF, 0xFF, 0) ) ;

		// This call extends the background color of the last style on the line to the edge of the window
		StyleSetEOLFilled( bastyle, true ) ;
	}

	StyleSetBack( SCE_HBA_STRINGEOL, RGB(0xCF,0xCF,0x7F) ) ;
	StyleSetFont( SCE_HBA_STRINGEOL, "MS UI Gothic" ) ;

	// If there is no need to support embedded JavaScript, the following code can be dropped.
	// JavaScript will still be correctly processed but will be displayed in just the default style.

	SetAStyle(SCE_HJ_START, COLOR_GOLD);
	SetAStyle(SCE_HJ_DEFAULT, COLOR_BLACK);
	SetAStyle(SCE_HJ_COMMENT, COLOR_DARK_GREEN);
	SetAStyle(SCE_HJ_COMMENTLINE, COLOR_DARK_GREEN);
	SetAStyle(SCE_HJ_COMMENTDOC, COLOR_DARK_GREEN);
	SetAStyle(SCE_HJ_NUMBER, COLOR_TURQUOISE);
	SetAStyle(SCE_HJ_WORD, COLOR_BLACK);
	SetAStyle(SCE_HJ_KEYWORD, COLOR_DARK_BLUE);
	SetAStyle(SCE_HJ_DOUBLESTRING, COLOR_VIOLET);
	SetAStyle(SCE_HJ_SINGLESTRING, COLOR_VIOLET);
	SetAStyle(SCE_HJ_SYMBOLS, COLOR_BLACK);

	SetAStyle(SCE_HJA_START, COLOR_GOLD);
	SetAStyle(SCE_HJA_DEFAULT, COLOR_BLACK);
	SetAStyle(SCE_HJA_COMMENT, COLOR_DARK_GREEN);
	SetAStyle(SCE_HJA_COMMENTLINE, COLOR_DARK_GREEN);
	SetAStyle(SCE_HJA_COMMENTDOC, COLOR_DARK_GREEN);
	SetAStyle(SCE_HJA_NUMBER, COLOR_TURQUOISE);
	SetAStyle(SCE_HJA_WORD, COLOR_BLACK);
	SetAStyle(SCE_HJA_KEYWORD, COLOR_DARK_BLUE);
	SetAStyle(SCE_HJA_DOUBLESTRING, COLOR_VIOLET);
	SetAStyle(SCE_HJA_SINGLESTRING, COLOR_VIOLET);
	SetAStyle(SCE_HJA_SYMBOLS, COLOR_BLACK);

	// Show the whole section of JavaScript with off white background
	for (int jstyle=SCE_HJ_DEFAULT; jstyle<=SCE_HJ_SYMBOLS; jstyle++) 
	{
		StyleSetFont( jstyle,"MS UI Gothic" ) ;
		StyleSetBack( jstyle, COLOR_OFFWHITE ) ;

		// This call extends the background color of the last style on the line to the edge of the window
		StyleSetEOLFilled( jstyle, true ) ;
	}

	StyleSetBack( SCE_HJ_STRINGEOL, RGB(0xDF, 0xDF, 0x7F) ) ;
	StyleSetEOLFilled( SCE_HJ_STRINGEOL, true ) ;

	// Show the whole section of JavaScript with brown background
	for (int jastyle=SCE_HJA_DEFAULT; jastyle<=SCE_HJA_SYMBOLS; jastyle++) 
	{
		StyleSetFont( jastyle,"MS UI Gothic" ) ;
		StyleSetBack( jastyle, RGB(0xDF, 0xDF, 0x7F) ) ;

		// This call extends the backround colour of the last style on the line to the edge of the window
		StyleSetEOLFilled( jastyle, true ) ;
	}

	StyleSetBack( SCE_HJA_STRINGEOL, RGB(0x0,0xAF,0x5F) ) ;
	StyleSetEOLFilled( SCE_HJA_STRINGEOL, true ) ;

	// set the display for indentation guides to on - this displays vertical dotted lines from the beginning of 
	// a code block to the end of the block
	SetIndentationGuides( true ) ;

	// set tab width to 3
	SetTabWidth( 3 ) ;

	// set indention to 3
	SetIndent( 3 ) ;

	// set the caret blinking time to 400 milliseconds
	SetCaretPeriod( 400 ) ;

	// source folding section
	// tell the lexer that we want folding information - the lexer supplies "folding levels"

	SetProperty( "fold",						"1");
	SetProperty( "fold.html",					"1");
	SetProperty( "fold.html.preprocessor",		"1");
	SetProperty( "fold.comment",				"1");
	SetProperty( "fold.at.else",				"1");
	SetProperty( "fold.flags",					"1");
	SetProperty( "fold.preprocessor",			"1");
	SetProperty( "fold.compact",				"0");
	SetProperty( "styling.within.preprocessor", "1");
	SetProperty( "asp.default.language",		"1");

	// Tell scintilla to draw folding lines UNDER the folded line
	SetFoldFlags(16);

	// Set margin 2 = folding margin to display folding symbols
	SetMarginMaskN( 2, SC_MASK_FOLDERS);

	// allow notifications for folding actions
	SetModEventMask( SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT);

	// make the folding margin sensitive to folding events = if you click into the margin you get a notification event
	SetMarginSensitiveN( 2, TRUE);

	// define a set of markers to display folding symbols
	MarkerDefine(  SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS );
	MarkerSetFore( SC_MARKNUM_FOLDEROPEN, RGB(0xff, 0xff, 0xff) );
	MarkerSetBack( SC_MARKNUM_FOLDEROPEN, RGB(0, 0, 0) ) ;

	MarkerDefine(  SC_MARKNUM_FOLDER, SC_MARK_PLUS );
	MarkerSetFore( SC_MARKNUM_FOLDER, RGB(0xff, 0xff, 0xff) );
	MarkerSetBack( SC_MARKNUM_FOLDER, RGB(0, 0, 0) ) ;

	MarkerDefine( SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
	MarkerDefine( SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);
	MarkerDefine( SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
	MarkerDefine( SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
	MarkerDefine( SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);

	// set end of line mode to CRLF
	ConvertEOLs( 2 );
	SetEOLMode( 2 );

	// set markersymbol for marker type 0 - bookmark
	MarkerDefine( 0, SC_MARK_CIRCLE);

	// display all margins
//	int lWidth = TextWidth( STYLE_LINENUMBER, "9" );
	SetMarginWidthN( SCINTILLA_MARGIN_LINENUM, 0 /* 5 * lWidth */ );
	SetMarginWidthN( SCINTILLA_MARGIN_SELECTION, 16 );
	SetMarginWidthN( SCINTILLA_MARGIN_FOLD, 16);
}
