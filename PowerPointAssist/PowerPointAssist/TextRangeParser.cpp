#include "StdAfx.h"
#include "TextRangeParser.h"
#include "PPTFormatter.h"
#include "globalApp.h"
#include "abbreviations.h"

CTextRangeParser::CTextRangeParser(app_state *properties, Abbreviations *abbreviations) : 
	m_start(0),
	m_properties(properties),
	m_abbreviations(abbreviations)
{
}

CTextRangeParser::~CTextRangeParser(void)
{
}

_bstr_t CTextRangeParser::getSelectedText(bool isPlainText)
{
	PowerPoint::SelectionPtr sel = globalApp::get()->ActiveWindow->Selection ;
	m_currentSegment = globalApp::get()->ActiveWindow->Selection->TextRange ;
	m_start = m_currentSegment->Start ;

	if (m_currentSegment->Length == 0)
	{
		return _bstr_t() ;
	}

	if ( isPlainText == false )
	{
		return selectionToRichText();
	}

	return selectionToPlainText();
}

_bstr_t CTextRangeParser::selectionToRichText()
{
	CPPTFormatter formatter( m_currentSegment ) ;
	return _bstr_t( formatter.formatRangeText().c_str() ) ;
}

_bstr_t CTextRangeParser::selectionToPlainText()
{
	wstring plainText = BSTR2wstring( m_currentSegment->Text ) ;
	boost::replace_all( plainText, L"&", L"&amp;" ) ;
	boost::replace_all( plainText, L"<", L"&lt;" ) ;
	return string2BSTR( plainText ) ;
}

bool CTextRangeParser::extendSelection(void)
{
	PowerPoint::SelectionPtr sel = globalApp::get()->ActiveWindow->Selection ;
	if ( ppSelectionText != sel->Type )
	{
		return false ;
	}

	setRangeFromSelection() ;

	int oldStart = m_currentSegment->Start ;

	if ( ! selectNext() )
	{
		return false ;
	}

	int newLen = m_currentSegment->Start - oldStart ;
	newLen += m_currentSegment->Length ;

	m_currentSegment = m_range->Characters( oldStart, newLen ) ;
	m_currentSegment->Select() ;

	ATLASSERT( oldStart == m_currentSegment->Start ) ;
	m_start = m_currentSegment->Start ;

	return true ;
}

bool CTextRangeParser::selectNext(void)
{
	PowerPoint::SelectionPtr sel = globalApp::get()->ActiveWindow->Selection ;
	if ( ppSelectionText != sel->Type )
	{
		return false ;
	}
	setRangeFromSelection() ;

	collapseSelection( ) ;

	// make up for the 1-based index by adding 1 to the length
	// subtracting 1 from start will select first character twice if start=1
	long endPoint = (m_range->Length - m_start ) + 1 ;
	endPoint = max(endPoint, 0) ;
	TextRangePtr characters_pre = m_range->Characters( m_start, endPoint ) ;
	ATLASSERT( characters_pre ) ;

	TextRangePtr characters = eatWhitespace( characters_pre ) ;

	// the start of our query after trimming the whitespace
	m_start = characters->Start ;

	wstring delimiters(this->m_properties->m_data.m_segChars) ;
	TRACE( delimiters ) ;

	long len = characters->Length ;

	if ( len == 0 ) // nothing to select
	{
		return false ;
	}

	bool last_char_was_period = false ;

	wchar_t this_char ;

	long i ;
	TextRangePtr character ;
	for ( i=1 ; i<=len ; ++i )
	{
		character = characters->Characters( i, 1 ) ;

		this_char = ((LPCWSTR)character->Text)[0] ;

		if ( delimiters.find( this_char ) != wstring::npos ) 
		{
			if ( this_char == L'.' ) 
			{
				last_char_was_period = true ;
				PowerPoint::TextRangePtr chars_so_far = characters->Characters( 1, i ) ;
				const wstring text_so_far = BSTR2wstring(chars_so_far->Text) ;
				foreach(wstring word, m_abbreviations->m_abbreviations)
				{
					if (boost::ends_with(text_so_far, word))
					{
						last_char_was_period = false ;
						break ;
					}
				}
			}
			else
			{
				last_char_was_period = false ;
				break ;
			}
		}
		else if ( this_char == L' ')
		{
			if ( last_char_was_period ) 
			{
				if (! m_properties->m_data.m_select_spaces)
				{
					--i ;
				}
				break ;
			}
			last_char_was_period = false ;
		}
		else if (this_char == L')' && last_char_was_period)
		{
			break ;
		}
		else if (iswspace(this_char))
		{
			--i ; // don't select the whitespace char itself
			break ;
		}
		else
		{
			last_char_was_period = false ;
		}

	}

	// we need this because selecting a Notes Page textrange draws an exception!
	try
	{
		characters->Characters( 1, i )->Select() ;
	}
	catch ( _com_error &e ) 
	{
		logging::log_error("Failed to select text") ;
		logging::log_exception(e) ;
		ATLTRACE( "COM ERROR Selecting Text! -- %s\n", e.ErrorMessage() ) ;
	}
	// have we selected something?
	m_currentSegment = characters->Characters( 1, i ) ;
	return ( m_currentSegment->Length > 0 ) ;
}

void CTextRangeParser::setRange(TextRangePtr range)
{
	m_range = range ;
	if ( ! range )
	{
		m_currentSegment = NULL ;
		m_start = 0 ;
		return ;
	}
	m_currentSegment = m_range->Characters( 1, 0 ) ;
	m_start = m_range->Start ;
	m_range->Characters(1,0)->Select() ;
}

bool CTextRangeParser::selectForSet(void)
{
	return false;
}

TextRangePtr CTextRangeParser::eatWhitespace(TextRangePtr& range)
{
	long len = range->Length ;

	PowerPoint::TextRangePtr character ;

	long i ;
	for ( i=1 ; i<=len ; ++i )
	{
		character = range->Characters( i, 1 ) ;

		if ( ! iswspace( ((LPCWSTR)character->Text)[0] ) ) 
		{
			break ;
		}
	}

	m_currentSegment = range->Characters( i, (len-i)+1 ) ;
	return m_currentSegment ;
}

void CTextRangeParser::collapseSelection( )
{
	long newStart  = m_currentSegment->Start + m_currentSegment->Length ;

	TextRangePtr endRange = m_currentSegment->Characters( newStart, 0 ) ;

	// we need this because selecting a Notes Page textrange draws an exception!
	try
	{
		endRange->Select() ;
	}
	catch ( _com_error &e ) 
	{
		ATLTRACE( "COM ERROR Selecting Text! -- %s\n", e.ErrorMessage() ) ; ;
		logging::log_error("Failed to select text") ;
		logging::log_exception(e) ;
	}
	m_currentSegment = endRange ;

	m_start = endRange->Start ;
	ATLASSERT( endRange->Length == 0 ) ;
}

TextRangePtr CTextRangeParser::getSetRange(void)
{
	setRangeFromSelection() ;

	if ( m_currentSegment->Length == 0 )
	{
		long len = m_currentSegment->Start - m_start  ; 
		m_currentSegment = m_range->Characters( m_start, len ) ;
		m_currentSegment->Select() ;
	}
	return m_currentSegment ;
}

void CTextRangeParser::setRangeFromSelection(void)
{
	PowerPoint::SelectionPtr sel = globalApp::get()->ActiveWindow->Selection ;

	m_currentSegment = sel->TextRange ;

	// calling m_currentSegment->Parent may fail if we have selected a Notes segment
	try
	{
		TextFramePtr frame = m_currentSegment->Parent ;
		if (frame)
		{
			ShapePtr shape = frame->Parent ;
			if (shape && shape->HasTextFrame && m_range != shape->TextFrame->TextRange )
			{
				m_start = m_currentSegment->Start ;
				m_range = shape->TextFrame->TextRange ;
			}
		}
		else if (sel->ShapeRange->Count > 0)
		{
			_variant_t item( 1 ) ;
			item.vt = VT_I4 ;
			ShapePtr shape = sel->ShapeRange->Item( item ) ;

			if (shape && shape->HasTextFrame)
			{
				if (m_range != shape->TextFrame->TextRange)
				{
					m_start = m_currentSegment->Start ;
					m_range = shape->TextFrame->TextRange ;
				}
			}
			else if (shape->HasTable)
			{
				PowerPoint::TablePtr table = shape->Table ;
				long num_rows = table->Rows->Count ;
				long num_cols = table->Columns->Count ;
				for ( int row = 1 ; row <= num_rows ; ++row )
				{
					for ( int col = 1 ; col <= num_cols ; ++col )
					{
						PowerPoint::CellPtr cell = table->Cell( row, col ) ;
						if (cell->Selected)
						{
							m_start = m_currentSegment->Start ;
							m_range = cell->Shape->TextFrame->TextRange ;
						}
					}
				}

			}
		}
		if (!m_range)
		{
			m_range = m_currentSegment ;
		}

	}
	catch ( _com_error &e) 
	{
		logging::log_error("Failed to set range from selection") ;
		logging::log_exception(e) ;
	}
}
