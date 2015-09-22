#include "StdAfx.h"
#include "PPTWriter.h"

#include "html_formatter.h"
#include "html_parser.h"


CPPTWriter::CPPTWriter( PowerPoint::TextRangePtr range ) : 
	m_range( range ),
	m_parser(this)
{
	m_properties.read_from_registry() ;
}

CPPTWriter::~CPPTWriter(void)
{
}

void CPPTWriter::write_text( const wstring &text ) 
{
	// --------------
	// put the text
	// --------------

	long start = m_range->Start ;
	m_range->Text = text.c_str() ;

	int text_len = static_cast< int >( text.size() ) ;
	m_range->Select() ;

	// ----------------------
	// apply text formatting
	// ----------------------

	PowerPoint::FontPtr font ;
	m_range->get_Font( &font ) ;
	ATLASSERT( font ) ;

	check_bold(font);


	check_italic(font);


	check_underline(font);

	check_color(font);

	apply_paragraph_formatting();

	select_end(start, text_len);

}

void CPPTWriter::apply_linebreak() 
{
	m_range->put_Text( _bstr_t(L"\13") ) ;
}

void CPPTWriter::apply_paragraph() 
{
	this->apply_linebreak() ;
}

void CPPTWriter::write_html( const wstring html_text )
{
	m_parser.write_html(html_text) ;
}

void CPPTWriter::select_end( long start, int text_len )
{
	PowerPoint::TextRangePtr end_sel = m_range->Characters( start+text_len, 0 ) ;
	ATLASSERT( end_sel ) ;

	m_range = end_sel ;
	m_range->Select() ;
}

void CPPTWriter::apply_paragraph_formatting()
{
	PowerPoint::ParagraphFormatPtr paragraph_format ;
	m_range->get_ParagraphFormat( &paragraph_format ) ;
	ATLASSERT( paragraph_format ) ;

	check_justification(paragraph_format);
}

void CPPTWriter::check_justification( PowerPoint::ParagraphFormatPtr paragraph_format )
{
	switch( m_parser.get_justification() )
	{
	case html_processing::JUST_LEFT:
		paragraph_format->put_Alignment( PowerPoint::ppAlignLeft ) ;
		break ;

	case html_processing::JUST_RIGHT:
		paragraph_format->put_Alignment( PowerPoint::ppAlignRight ) ;
		break ;

	case html_processing::JUST_CENTER:
		paragraph_format->put_Alignment( PowerPoint::ppAlignCenter ) ;
		break ;

	case html_processing::JUST_NONE: // no alignment specification
		break ;

	default:
		ATLASSERT( FALSE && "Unknown justification code!" ) ; 
	}
}

void CPPTWriter::check_color( PowerPoint::FontPtr font )
{
	if (m_properties.get_font_color())
	{
		PowerPoint::ColorFormatPtr color ;
		font->get_Color( &color ) ;
		ATLASSERT( color ) ;

		Office::MsoRGBType rgb_color_value = (Office::MsoRGBType)m_parser.get_fore_color() ;
		color->put_PowerPointRGB( rgb_color_value ) ;
	}
}

void CPPTWriter::check_underline( PowerPoint::FontPtr font )
{
	if (m_properties.get_font_underline())
	{
		if ( m_parser.get_underline() )
		{
			font->put_Underline( Office::msoTrue ) ;
		}
		else
		{
			font->put_Underline( Office::msoFalse ) ;
		}
	}
}

void CPPTWriter::check_italic( PowerPoint::FontPtr font )
{
	if (m_properties.get_font_italic())
	{
		if ( m_parser.get_italic() )
		{
			font->put_Italic( Office::msoTrue ) ;
		}
		else
		{
			font->put_Italic( Office::msoFalse ) ;
		}
	}
}

void CPPTWriter::check_bold( PowerPoint::FontPtr font )
{
	if (m_properties.get_font_bold())
	{
		if ( m_parser.get_bold() )
		{
			font->put_Bold( Office::msoTrue ) ;
		}
		else
		{
			font->put_Bold( Office::msoFalse ) ;
		}
	}
}

