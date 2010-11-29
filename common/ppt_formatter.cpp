#include "stdafx.h"
#include "FelixInterface.h"
#include "ui.h"
// #include "InputDlg.h"
// #include "app_properties.h"

#include "label_name.h"
#include "html_formatter.h"
#include "html_writer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

wstring ppt_formatter::format_range_text( ) 
{
	ATLASSERT( m_range ) ;

	set_paragraph_formatting() ;
	
	long start = 1 ;
//	m_range->get_Start( &start ) ;
//	ATLASSERT( start != 0 ) ;

	long range_len = 0 ;
	m_range->get_Length( &range_len ) ;
	ATLASSERT( range_len != 0 ) ;

	long range_end = start + range_len ;

	for ( long i = start ; i < range_end ; ++i )
	{
		CComPtr< PowerPoint::TextRange > current_char ;
		m_range->Characters( i, 1, &current_char ) ;
		
		// ----------------------
		// parse text formatting
		// ----------------------
		
		CComPtr< PowerPoint::Font > font ;
		current_char->get_Font( &font ) ;
		ATLASSERT( font ) ;
		
		Office::MsoTriState setting_val = Office::msoFalse ;

		font->get_Bold( &setting_val ) ;
		if ( setting_val == Office::msoFalse )
		{
			m_formatter.set_bold( false ) ;
		}
		else
		{
			ATLASSERT( setting_val == Office::msoTrue ) ;
			m_formatter.set_bold( true ) ;
		}

		font->get_Italic( &setting_val ) ;
		if ( setting_val == Office::msoFalse )
		{
			m_formatter.set_italic( false ) ;
		}
		else
		{
			ATLASSERT( setting_val == Office::msoTrue ) ;
			m_formatter.set_italic( true ) ;
		}

		font->get_Underline( &setting_val ) ;
		if ( setting_val == Office::msoFalse )
		{
			m_formatter.set_underline( false ) ;
		}
		else
		{
			ATLASSERT( setting_val == Office::msoTrue ) ;
			m_formatter.set_underline( true ) ;
		}

		// do the color
		
		CComPtr< PowerPoint::ColorFormat > color ;
		font->get_Color( &color ) ;
		ATLASSERT( color ) ;
		
		Office::MsoRGBType rgb_color_value = 0 ;
		color->get_PowerPointRGB( &rgb_color_value ) ;
		
		if ( rgb_color_value == 0 )
		{
			m_formatter.clear_fore_color() ;
		}
		else
		{
			m_formatter.set_fore_color( CColorRef( (COLORREF)rgb_color_value ) ) ;
		}
		

		// now write the character
		CComBSTR text ;
		current_char->get_Text( &text ) ;

		m_formatter.add_text( label_name( text ) ) ;
		
	}
	
	return m_formatter.get_text() ;
}


// ---------------------------
// parse paragraph formatting
// ---------------------------
void ppt_formatter::set_paragraph_formatting()
{
	CComPtr< PowerPoint::ParagraphFormat > paragraph_format ;
	m_range->get_ParagraphFormat( &paragraph_format ) ;
	ATLASSERT( paragraph_format ) ;
	
	PowerPoint::PpParagraphAlignment alignment = PowerPoint::ppAlignLeft ;
	paragraph_format->get_Alignment( &alignment ) ;
	
	switch( alignment )
	{
	case PowerPoint::ppAlignLeft:
		m_formatter.set_justification( html_formatter::JUST_LEFT ) ;
		break ;
		
	case PowerPoint::ppAlignRight:
		m_formatter.set_justification( html_formatter::JUST_RIGHT ) ;
		break ;
		
	case PowerPoint::ppAlignCenter:
		m_formatter.set_justification( html_formatter::JUST_CENTER ) ;
		break ;
		
	default:
		ATLASSERT( FALSE && "Unsupported justification code!" ) ; 
	}
	
	
}

