#include "StdAfx.h"
#include "PPTFormatter.h"


CPPTFormatter::CPPTFormatter(PowerPoint::TextRangePtr range) :
	m_range( range )
{
	m_properties.read_from_registry() ;
}

CPPTFormatter::~CPPTFormatter(void)
{
}


wstring CPPTFormatter::formatRangeText( ) 
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
		PowerPoint::TextRangePtr current_char  = m_range->Characters( i, 1 ) ;

		// ----------------------
		// parse text formatting
		// ----------------------

		PowerPoint::FontPtr font ;
		current_char->get_Font( &font ) ;
		ATLASSERT( font ) ;

		Office::MsoTriState setting_val = Office::msoFalse ;

		if (m_properties.get_font_bold())
		{
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
		}

		if (m_properties.get_font_italic())
		{
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
		}

		if (m_properties.get_font_underline())
		{
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
		}

		if (m_properties.get_font_color())
		{
			// do the color

			PowerPoint::ColorFormatPtr color ;
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
		}


		// now write the character
		m_formatter.add_text( (LPCWSTR)( current_char->Text ) ) ;

	}

	return m_formatter.get_text() ;
}


// ---------------------------
// parse paragraph formatting
// ---------------------------
void CPPTFormatter::set_paragraph_formatting()
{
	PowerPoint::ParagraphFormatPtr paragraph_format = m_range->GetParagraphFormat( ) ;
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

	case PowerPoint::ppAlignJustify:
		m_formatter.set_justification(html_formatter::JUST_NONE) ;
		break ;

	default:
		ATLASSERT( FALSE && "Unsupported justification code!" ) ; 
	}


}

