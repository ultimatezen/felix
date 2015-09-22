#pragma once

class match_score
{
private:
	double				m_base_score ;
	double				m_formatting_penalty ;

public:
	match_score(void);
	~match_score(void);

	match_score( const match_score &cpy ) :
		m_base_score( cpy.m_base_score ),
		m_formatting_penalty( cpy.m_formatting_penalty )
	{}

	match_score& operator= ( const match_score &cpy );
	void SetBaseScore(double score) ;
	void SetFormattingPenalty(double formatting_penalty) ;

	double get_score() const ;

	double GetFormattingPenalty() const ;
	double GetBaseScore() const ;

	bool HasFormattingPenalty(void) const ;
};
