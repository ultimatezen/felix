#include "StdAfx.h"
#include ".\matchstring.h"

CMatchString::CMatchString(void)
{
}

CMatchString::~CMatchString(void)
{
}

std::wstring CMatchString::mark_up_string()
{
	const static wstring nomatch_tag_open(L"<b><span style=\"background:red\">") ;
	const static wstring nomatch_tag_close(L"</span></b>") ;

	// format packages
	wstring marked_up_string ;

	// we start off in the same state as the upper left-hand corner of the matrix
	MatchStates match_state = m_stack.top().second ;
	// store the current text wstring here
	wstring current_text_buffer ;
	// pop each member of the stack, and mark it up as match/no match
	for ( ; ! m_stack.empty() ; m_stack.pop() )
	{
		// if the state of this pair does not match the current state,
		// dump any text in the buffer into the current-state node,
		// then set the current state to the pair state
		if ( m_stack.top().second != match_state )
		{
			// is there text in the buffer?
			if ( current_text_buffer.empty() == false )
			{
				if( match_state == NOMATCH)
				{
					marked_up_string += nomatch_tag_open + current_text_buffer + nomatch_tag_close ;
				}
				else
				{
					marked_up_string += current_text_buffer ;
				}
				match_state = m_stack.top().second ;
				current_text_buffer.erase() ; // empty the buffer
			}
		}
		// add the current pair to the buffer
		if ( m_stack.top().first == L'<' )	// we need to convert back to &lt; for display purposes
			current_text_buffer += L"&lt;" ;
		else
			current_text_buffer += m_stack.top().first ;

	}

	// do we have a buffer left?
	if ( current_text_buffer.empty() == false )
	{
		if( match_state == NOMATCH)
		{
			marked_up_string += nomatch_tag_open + current_text_buffer + nomatch_tag_close ;
		}
		else
		{
			marked_up_string += current_text_buffer ;
		}
	}

	return marked_up_string ;
}