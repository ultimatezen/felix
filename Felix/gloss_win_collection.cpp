#include "stdafx.h"
#include "gloss_win_collection.h"

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
