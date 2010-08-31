#include "stdafx.h"
#include "ManagerView.h"
#include "Path.h"
#include "MemoryManagerDlg.h"
#include "numberfmt.h"
#include "SearchWindow.h"

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

	void set_mem_tmpl_info( mem_engine::memory_pointer mem, 
							text_tmpl::CTextTemplate &engine, 
							bool is_memory,
							size_t index) 
	{
		mem_engine::MemoryInfo *mem_info = mem->get_memory_info() ;

		CNumberFmt number_format ;
		wstring memsize = (LPCWSTR)(number_format.Format(mem->size())) ;
		engine.Assign("size", memsize) ;

		engine.Assign("name", get_memname(mem)) ;
		engine.Assign("memtype", is_memory ? L"mem" : L"gloss") ;
		engine.Assign("index", tows(index)) ;

		CMemoryManagerDlg dlg ;

		engine.Assign(L"file_name", dlg.get_memory_name(mem)) ;
		engine.Assign(L"creator", mem_info->get_creator()) ;
		engine.Assign(L"field", mem_info->get_field()) ;
		engine.Assign(L"created_on", mem_info->get_created_on()) ;
		engine.Assign(L"source_language", mem_info->get_source_language()) ;
		engine.Assign(L"target_language", mem_info->get_target_language()) ;
		engine.Assign(L"client", mem_info->get_client()) ;
		engine.Assign(L"mem_size", ulong2formatted_wstring(mem->size())) ;
		engine.Assign(L"file_size", dlg.get_file_size(mem)) ;
		engine.Assign(L"reliability", dlg.get_reliability_range(mem)) ;
		engine.Assign(L"validated", mem->get_validated_percent()) ;
		engine.Assign(L"locked", bool2wstring( mem->is_locked() )) ;
	}
}