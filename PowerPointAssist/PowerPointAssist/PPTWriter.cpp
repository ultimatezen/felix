#include "StdAfx.h"
#include "PPTWriter.h"

#include "html_formatter.h"
#include "html_writer.h"


CPPTWriter::CPPTWriter( PowerPoint::TextRangePtr range ) : 
	m_range( range )
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

	if (m_properties.get_font_bold())
	{
		if ( get_bold() )
		{
			font->put_Bold( Office::msoTrue ) ;
		}
		else
		{
			font->put_Bold( Office::msoFalse ) ;
		}
	}

	if (m_properties.get_font_italic())
	{
		if ( get_italic() )
		{
			font->put_Italic( Office::msoTrue ) ;
		}
		else
		{
			font->put_Italic( Office::msoFalse ) ;
		}
	}

	if (m_properties.get_font_underline())
	{
		if ( get_underline() )
		{
			font->put_Underline( Office::msoTrue ) ;
		}
		else
		{
			font->put_Underline( Office::msoFalse ) ;
		}
	}

	if (m_properties.get_font_color())
	{
		PowerPoint::ColorFormatPtr color ;
		font->get_Color( &color ) ;
		ATLASSERT( color ) ;

		Office::MsoRGBType rgb_color_value = (Office::MsoRGBType)get_fore_color() ;
		color->put_PowerPointRGB( rgb_color_value ) ;
	}


	// ---------------------------
	// apply paragraph formatting
	// ---------------------------

	PowerPoint::ParagraphFormatPtr paragraph_format ;
	m_range->get_ParagraphFormat( &paragraph_format ) ;
	ATLASSERT( paragraph_format ) ;

	switch( get_justification() )
	{
	case JUST_LEFT:
		paragraph_format->put_Alignment( PowerPoint::ppAlignLeft ) ;
		break ;

	case JUST_RIGHT:
		paragraph_format->put_Alignment( PowerPoint::ppAlignRight ) ;
		break ;

	case JUST_CENTER:
		paragraph_format->put_Alignment( PowerPoint::ppAlignCenter ) ;
		break ;

	case JUST_NONE: // no alignment specification
		break ;

	default:
		ATLASSERT( FALSE && "Unknown justification code!" ) ; 
	}



	PowerPoint::TextRangePtr end_sel = m_range->Characters( start+text_len, 0 ) ;
	ATLASSERT( end_sel ) ;

	m_range = end_sel ;
	m_range->Select() ;

}

void CPPTWriter::apply_linebreak() 
{
	m_range->put_Text( _bstr_t(L"\13") ) ;
}

void CPPTWriter::apply_paragraph() 
{
	m_range->put_Text( _bstr_t(L"\13") ) ;
}

