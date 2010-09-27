#include "stdafx.h"
#include ".\wordcontrollerword.h"

using namespace except ;

CWordControllerWord::CWordControllerWord()
{
}

CWordControllerWord::~CWordControllerWord(void)
{
}

bool CWordControllerWord::OnAnalyze()
{
	CWordScreenUpdateLock screen_lock( m_word_object ) ;

	WordSelection selection = m_word_object.get_selection() ;

	Felix::IAppPtr assistant ;
	assistant.CreateInstance( L"Felix.App" ) ;

	boost::shared_ptr<WordParser> parser 
		= boost::shared_ptr<WordParser>(new WordParser( selection, &m_properties, &m_abbreviations )) ;
	try
	{
		parser = boost::shared_ptr<WordParser>() ;
	}
	catch (CException& e)
	{
		logging::log_error("OnAnalyze :: Error destroying WordParser object") ;
		logging::log_exception(e) ;
	}
	while ( parser->select_next_sentence( ) )
	{
		OnLookupAction( false ) ;
		if ( FLOAT_EQ( 1.0, assistant->Score ) )
		{
			word_sel_ptr sel = m_word_object.get_selection() ;
			sel->Range->HighlightColorIndex = MSWord::wdGray25 ;
		}
		else if ( assistant->Score > 0.95 )
		{
			word_sel_ptr sel = m_word_object.get_selection() ;
			sel->Range->HighlightColorIndex = MSWord::wdTurquoise ;
		}
		else
		{
			word_sel_ptr sel = m_word_object.get_selection() ;
			sel->Range->HighlightColorIndex = MSWord::wdYellow ;
		}
	}

	SetQueryStart( selection.get_start() ) ;
	ATLASSERT ( selection.get_start() >= 0 ) ; 

	return true ;
}




