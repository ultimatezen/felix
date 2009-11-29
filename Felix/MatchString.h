/*!
	@file MatchString.h
	@brief Header file for CMatchString
	@date 2005/08/20
	Time: 17:13:25
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */
#pragma once

#include <stack>						// for stacks of match pairs
// match pairs for marked-up distance strings

class CMatchString
{
public:
	enum MatchStates { MATCH, NOMATCH, PLACEMENT } ;
	typedef std::pair< OLECHAR, MatchStates > match_pair ;
	typedef std::stack< match_pair > match_pair_stack ;

	match_pair_stack		m_stack ;

public:
	CMatchString(void);
	~CMatchString(void);
	void cell_match( OLECHAR c ){	m_stack.push( match_pair( c, MATCH ) ) ; }
	void cell_mismatch( OLECHAR c ) { m_stack.push( match_pair( c, NOMATCH ) ) ; }
	wstring mark_up_string();
};
