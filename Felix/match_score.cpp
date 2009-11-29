#include "StdAfx.h"
#include ".\match_score.h"


match_score::match_score(void) :
	m_base_score(0.f),
	m_formatting_penalty(0.f)
{
}

match_score::~match_score(void)
{
}

bool match_score::HasFormattingPenalty(void) const
{
	return ! FLOAT_EQ( GetFormattingPenalty(), 0.f ) ;
}

void match_score::SetBaseScore(double score)
{
	m_base_score = score;
}

void match_score::SetFormattingPenalty(double formatting_penalty)
{
	m_formatting_penalty = formatting_penalty;
}

double match_score::get_score() const
{
	return GetBaseScore() - GetFormattingPenalty() ;
}

double match_score::GetFormattingPenalty() const
{
	return m_formatting_penalty;
}
double match_score::GetBaseScore() const
{
	return m_base_score ;
}
