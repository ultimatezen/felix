#include "stdafx.h"
#include "ManagerView.h"
#include "Path.h"
#include "MemoryManagerDlg.h"
#include "numberfmt.h"
#include "SearchWindow.h"
#include "query.h"
namespace mgrview
{

	wstring get_memname(mem_engine::memory_pointer mem)
	{
		return mem_engine::get_fname_from_loc(static_cast<const wchar_t *>(mem->get_location())) ;
	}

	void mgrview::ManagerView::set_view( view_interface *view )
	{
		m_view = view ;
	}

	void mgrview::ManagerView::set_mem_model( model_iface_ptr model )
	{
		m_mem_model = model ;
	}

	void mgrview::ManagerView::set_gloss_model( model_iface_ptr model )
	{
		m_gloss_model = model ;
	}

	void mgrview::ManagerView::set_listener( FelixManagerWindowListener *listener )
	{
		m_window_listener = listener ;
	}

	void set_mem_tmpl_info( mem_engine::memory_pointer mem, 
							cpptempl::data_map &data, 
							bool is_memory,
							size_t index) 
	{
		mem_engine::MemoryInfo *mem_info = mem->get_memory_info() ;

		wstring memsize = fmt_num(mem->size()) ;
		data[L"size"] = cpptempl::make_data(memsize) ;

		data[L"name"] = cpptempl::make_data(get_memname(mem)) ;
		data[L"memtype"] = cpptempl::make_data(is_memory ? L"mem" : L"gloss") ;
		data[L"index"] = cpptempl::make_data(tows(index)) ;

		CMemoryManagerDlg dlg(app_props::get_props()) ;

		data[L"file_name"] = cpptempl::make_data(dlg.get_memory_name(mem)) ;
		data[L"creator"] = cpptempl::make_data(mem_info->get_creator()) ;
		data[L"field"] = cpptempl::make_data(mem_info->get_field()) ;
		data[L"created_on"] = cpptempl::make_data(mem_info->get_created_on()) ;
		data[L"source_language"] = cpptempl::make_data(mem_info->get_source_language()) ;
		data[L"target_language"] = cpptempl::make_data(mem_info->get_target_language()) ;
		data[L"client"] = cpptempl::make_data(mem_info->get_client()) ;
		data[L"mem_size"] = cpptempl::make_data(ulong2formatted_wstring(mem->size())) ;
		data[L"file_size"] = cpptempl::make_data(dlg.get_file_size(mem)) ;
		data[L"reliability"] = cpptempl::make_data(dlg.get_reliability_range(mem)) ;
		data[L"validated"] = cpptempl::make_data(mem->get_validated_percent()) ;
		data[L"locked"] = cpptempl::make_data(bool2wstring( mem->is_locked() )) ;
		data[L"default_context"] = cpptempl::make_data(mem->get_default_context()) ;
	}

	void set_record_info( cpptempl::data_map &item, mem_engine::record_pointer record ) 
	{
		item[L"source"] = cpptempl::make_data(record->get_source_rich()) ;
		item[L"trans"] = cpptempl::make_data(record->get_trans_rich()) ;
		item[L"context"] = cpptempl::make_data(record->get_context_rich()) ;
		item[L"created"] = cpptempl::make_data(record->get_created().get_date_time_string()) ;
		item[L"modified"] = cpptempl::make_data(record->get_modified().get_date_time_string()) ;
		item[L"reliability"] = cpptempl::make_data(tows(record->get_reliability())) ;
		item[L"validated"] = cpptempl::make_data(bool2wstring(record->is_validated())) ;

		item[L"creator"] = cpptempl::make_data(record->get_creator()) ;
		item[L"modified_by"] = cpptempl::make_data(record->get_modified_by()) ;

		item[L"refcount"] = cpptempl::make_data(tows(record->get_refcount())) ;
		item[L"ref_count"] = cpptempl::make_data(tows(record->get_refcount())) ;
	}
	void set_memory_info( mem_engine::memory_pointer mem, cpptempl::data_map &item ) 
	{
		file::CPath filename(mem->get_location()) ;
		wstring loc ;
		if ( filename.Path().IsEmpty() )
		{
			loc = R2WSTR( IDS_NEW ) ;
		}
		else
		{
			loc = filename.FindFileName() ;		
		}

		item[L"mem"] = cpptempl::make_data(loc) ;
		item[L"memory"] = cpptempl::make_data(loc) ;
	}

	void set_index_info( cpptempl::data_map &item, size_t i ) 
	{
		item[L"num0"] = cpptempl::make_data(tows(i)) ;
		item[L"num"] = cpptempl::make_data(tows(i+1)) ;
	}

}