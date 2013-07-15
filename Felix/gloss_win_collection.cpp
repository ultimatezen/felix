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
	first()->set_main_on() ;
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
	first()->add_record( gloss_record ) ;
}

wstring GlossWinCollection::get_glossary_entry( short index )
{
	if (this->empty())
	{
		return wstring() ;
	}
	return first()->get_glossary_entry(index) ;
}

bool GlossWinCollection::empty()
{
	return m_glossary_windows.empty() ;
}

size_t GlossWinCollection::size()
{
	return m_glossary_windows.size() ;
}

void GlossWinCollection::set_ui_language()
{
	remove_destroyed_gloss_windows() ;
	BOOST_FOREACH(gloss_window_pointer gloss, m_glossary_windows)
	{
		gloss->set_ui_language() ;
	}
}

void GlossWinCollection::save_prefs()
{
	if (empty())
	{
		return ;
	}
	first()->save_prefs() ;
}

void GlossWinCollection::remove_gloss_record( mem_engine::record_pointer rec )
{
	if (empty())
	{
		return ;
	}
	first()->delete_record(rec) ;
}

mem_engine::felix_query * GlossWinCollection::get_current_matches()
{
	if (empty())
	{
		return NULL ;
	}
	return first()->get_current_matches() ;
}

int GlossWinCollection::get_show_command( BOOL is_visible )
{
	if (is_visible)
	{
		return SW_SHOWNOACTIVATE ;
	}
	return SW_HIDE ;
}

void GlossWinCollection::put_show_marking( const VARIANT_BOOL setting )
{
	FOREACH(gloss_window_pointer gloss, m_glossary_windows)
	{
		gloss->put_show_marking(setting) ;
	}
}

bool GlossWinCollection::pre_shutdown_save_check()
{
	FOREACH(gloss_window_pointer gloss, m_glossary_windows)
	{
		if ( false == gloss->pre_shutdown_save_check() )
		{
			return false ;
		}
	}
	return true ;
}

void GlossWinCollection::look_up( const wstring query )
{
	FOREACH(gloss_window_pointer gloss, m_glossary_windows)
	{
		gloss->lookup( query ) ;
	}
}

gloss_window_pointer GlossWinCollection::first()
{
	return m_glossary_windows[0] ;
}

void GlossWinCollection::show_view_content()
{
	FOREACH( gloss_window_pointer gloss_win, m_glossary_windows)
	{
		gloss_win->show_view_content() ;
	}
}

void GlossWinCollection::put_visibility( int visibility )
{
	FOREACH(gloss_window_pointer gloss, m_glossary_windows)
	{
		gloss->show_window(visibility) ;
	}
}

bool GlossWinCollection::gloss_view_switch( HWND child )
{
	remove_destroyed_gloss_windows() ;
	if (size() < 2)
	{
		return false ;
	}
	for ( auto pos = m_glossary_windows.begin() ; pos != m_glossary_windows.end() ; ++pos )
	{
		gloss_window_pointer gloss = *pos ;
		if ( gloss->m_hWnd == child )
		{
			++pos ;
			if ( pos != m_glossary_windows.end() )
			{
				gloss = *pos ;
				gloss->set_focus() ;
			}
			else
			{
				first()->set_focus() ;
			}
			return true ;
		}
	}
	return false ;
}

void GlossWinCollection::on_file_save()
{
	remove_destroyed_gloss_windows() ;
	FOREACH( gloss_window_pointer gloss_win, m_glossary_windows)
	{
		gloss_win->on_file_save() ;
	}
}

void GlossWinCollection::clear_glossaries()
{
	FOREACH(gloss_window_pointer gloss, m_glossary_windows)
	{
		gloss->clear_memory() ;
	}
}

void GlossWinCollection::exit_silently()
{
	remove_destroyed_gloss_windows() ;
	FOREACH( gloss_window_pointer gloss_win, m_glossary_windows)
	{
		gloss_win->exit_silently() ;
	}
}

bool GlossWinCollection::has_clashes( CString location )
{
	remove_destroyed_gloss_windows() ;
	FOREACH( gloss_window_pointer gloss_win, m_glossary_windows)
	{
		if (gloss_win->m_model->has_name_clash(location))
		{
			return true ;
		}
	}
	return false ;
}

