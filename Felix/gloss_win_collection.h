#pragma once

#include "GlossaryWindowFrame.h"

typedef std::vector< gloss_window_pointer > gloss_window_list ;

class GlossWinCollection
{
public:
	gloss_window_list m_glossary_windows ;

	void remove_destroyed_gloss_windows();
	bool pre_translate(MSG *msg)
	{
		FOREACH(gloss_window_pointer gloss, m_glossary_windows)
		{
			if(gloss->m_pre_translate_msg(msg))
			{
				return true ;
			}
		}
		return false ;
	}
	bool add(app_props::props_ptr props)
	{
		gloss_window_pointer gloss_window(new GlossaryWindowFrame(props)) ;
		m_glossary_windows.push_back(gloss_window) ;
		m_glossary_windows[0]->set_main_on() ;
		return gloss_window ;
	}

};