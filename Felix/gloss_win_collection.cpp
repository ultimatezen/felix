#include "stdafx.h"
#include "gloss_win_collection.h"
#include "record.h"

using namespace mem_engine ;

GlossWinCollection::GlossWinCollection()
{
	m_add = boost::bind(&GlossWinCollection::add, this, _1) ;
}

void GlossWinCollection::remove_destroyed_gloss_windows()
{
	gloss_window_list live_windows ;
	FOREACH(gloss_window_pointer gloss, m_glossary_windows)
	{
		if (gloss->is_window())
		{
			live_windows.push_back(gloss) ;
		}
	}
	m_glossary_windows.swap(live_windows) ;
}

bool GlossWinCollection::pre_translate( MSG *msg )
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

gloss_window_pointer GlossWinCollection::add( app_props::props_ptr props )
{
	gloss_window_pointer gloss_window(new GlossaryWindowFrame(props)) ;
	m_glossary_windows.push_back(gloss_window) ;
	m_glossary_windows[0]->set_main_on() ;
	return gloss_window ;
}

bool GlossWinCollection::create( gloss_window_pointer gloss, HWND parent )
{
	if(!gloss->m_create( parent ))
	{
		except::CWinException e(_T("Failed to create glossary window")) ;
		logging::log_exception(e) ;
		return false ;
	}
	return true ;
}

void GlossWinCollection::add_record( record_pointer record )
{
	if ( m_glossary_windows.empty() )
	{
		return ;
	}
	record_pointer gloss_record(record->clone()) ;
	m_glossary_windows[0]->add_record( gloss_record ) ;
}

bool GlossWinCollection::empty()
{
	return m_glossary_windows.empty() ;
}
