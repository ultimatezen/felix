#include "stdafx.h"
#include "ManagerView.h"
#include "Path.h"

namespace mgrview
{

	wstring get_memname(mem_engine::memory_pointer mem)
	{
		file::CPath path(mem->get_location()) ;
		path.StripPath() ;
		path.RemoveExtension() ;
		return wstring((LPCWSTR)path.Path()) ;
	}

	void mgrview::ManagerView::set_view( view_interface *view )
	{
		m_view = view ;
	}

	void mgrview::ManagerView::set_mem_model( FelixModelInterface *model )
	{
		m_mem_model = model ;
	}

	void mgrview::ManagerView::set_gloss_model( FelixModelInterface *model )
	{
		m_gloss_model = model ;
	}

	void mgrview::ManagerView::set_listener( FelixManagerWindowListener *listener )
	{
		m_window_listener = listener ;
	}
}