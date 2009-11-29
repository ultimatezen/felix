/*!
	@brief implementation file for search_match class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "search_match.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif
namespace memory_engine
{

// formatting penalty

void search_match::set_formatting_penalty(double formatting_pentaly)
{
	m_score.SetFormattingPenalty( formatting_pentaly ) ;
}
const double search_match::get_formatting_penalty() const
{
	return m_score.GetFormattingPenalty() ;
}

const bool search_match::has_formatting_penalty() const 
{
	return m_score.HasFormattingPenalty() ;
}

// base score
void search_match::set_base_score(double score)
{
	m_score.SetBaseScore( score ) ;
}
const double search_match::get_base_score() const
{
	return m_score.GetBaseScore() ;
}


// total score
const double search_match::get_score() const
{
	return m_score.get_score() ;
}

// record

record_pointer search_match::get_record()
{
	return m_record ;
}
void search_match::set_record( record_pointer rec )
{
	m_record = rec ;
}

// markup
markup_ptr search_match::get_markup()
{
	return m_markup ;
}

// memory
void search_match::set_memory_location(const wstring& memory_location)
{
	m_memory_location = memory_location;
}

const wstring& search_match::get_memory_location() const
{
	return m_memory_location;
}

//
void search_match::set_values_to_record()
{
	m_markup->SetSource( m_record->get_source_rich() ) ;
	m_markup->SetTrans( m_record->get_trans_rich() ) ;
	m_markup->SetContext( m_record->get_context_rich() ) ;
}

}