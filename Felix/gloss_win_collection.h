#pragma once

#include "GlossaryWindowFrame.h"

typedef std::vector< gloss_window_pointer > gloss_window_list ;

class GlossWinCollection
{
public:
	gloss_window_list m_glossary_windows ;

	boost::function<gloss_window_pointer(app_props::props_ptr props)> m_add ;

	GlossWinCollection()
	{
		m_add = boost::bind(&GlossWinCollection::add, this, _1) ;
	}
	void remove_destroyed_gloss_windows();
	bool pre_translate(MSG *msg);
	gloss_window_pointer add(app_props::props_ptr props)
	{
		gloss_window_pointer gloss_window(new GlossaryWindowFrame(props)) ;
		m_glossary_windows.push_back(gloss_window) ;
		m_glossary_windows[0]->set_main_on() ;
		return gloss_window ;
	}
	bool create(gloss_window_pointer gloss, HWND parent)
	{
		if(!gloss->m_create( parent ))
		{
			except::CWinException e(_T("Failed to create glossary window")) ;
			logging::log_exception(e) ;
			return false ;
		}
		return true ;
	}

};