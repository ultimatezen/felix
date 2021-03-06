#include "stdafx.h"
#include "ManagerViewStart.h"
#include "text_templates.h"
#include "numberfmt.h"

namespace mgrview
{

	wstring make_mem_list(model_iface_ptr mems, 
										const wstring memtype )
	{
		cpptempl::data_map data ;

		data[L"memtype"] = cpptempl::make_data(memtype) ;

		cpptempl::data_list tms ;
		for(size_t i = 0 ; i < mems->size() ; ++i)
		{
			mem_engine::memory_pointer mem = mems->memory_at(i) ;

			cpptempl::data_map item ;
	
			wstring memsize = fmt_num(mem->size()) ;
			item[L"size"] = cpptempl::make_data(memsize) ;

			// up/down arrows
			item[L"has_up"] = item_bool(i > 0 && mems->size() > 1) ;
			item[L"has_down"] = item_bool(i < mems->size() - 1 && mems->size() > 1) ;
			item[L"is_active"] = item_bool(mem->is_active()) ;

			item[L"name"] = cpptempl::make_data(get_memname(mem)) ;

			tms.push_back(cpptempl::make_data(item)) ;
		}
		data[L"tms"] = cpptempl::make_data(tms) ;

		wstring tpl_text = cpptempl::get_template_text(_T("manager/datalist.txt")) ;
		return cpptempl::parse(tpl_text, data) ;
	}

	// ManagerViewStart

	void ManagerViewStart::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewStart::show_content()
	{
		cpptempl::data_map data ;

		// tms
		set_template_data(data);

		// fill in the template
		wstring tpl_text = cpptempl::get_template_text(_T("manager/start.txt")) ;
		m_view->set_text(cpptempl::parse(tpl_text, data)) ;
	}

	void ManagerViewStart::set_template_data( cpptempl::data_map &data )
	{
		wstring memlist = make_mem_list(m_mem_model, L"mem") ;
		data[L"memlist"] = cpptempl::make_data(memlist) ;

		wstring glosslist = make_mem_list(m_gloss_model, L"gloss") ;
		data[L"glosslist"] = cpptempl::make_data(glosslist) ;
		data[L"message"] = cpptempl::make_data(m_window_listener->get_message()) ;
	}
	// ManagerViewStartMem
	void ManagerViewStartMem::show_content()
	{
		cpptempl::data_map data ;

		set_template_data(data);

		// fill in the template
		wstring tpl_text = cpptempl::get_template_text(_T("manager/startmem.txt")) ;
		m_view->set_text(cpptempl::parse(tpl_text, data)) ;
	}

	void ManagerViewStartMem::set_template_data( cpptempl::data_map &data )
	{
		// tms
		wstring memlist = make_mem_list(m_mem_model, L"mem") ;
		data[L"memlist"] = cpptempl::make_data(memlist) ;
		data[L"message"] = cpptempl::make_data(m_window_listener->get_message()) ;
	}
	// ManagerViewStartGloss
	void ManagerViewStartGloss::show_content()
	{
		cpptempl::data_map data ;

		set_template_data(data);

		// fill in the template
		wstring tpl_text = cpptempl::get_template_text(_T("manager/startgloss.txt")) ;
		m_view->set_text(cpptempl::parse(tpl_text, data)) ;
	}

	void ManagerViewStartGloss::set_template_data( cpptempl::data_map &data )
	{
		// tms
		wstring glosslist = make_mem_list(m_gloss_model, L"gloss") ;
		data[L"glosslist"] = cpptempl::make_data(glosslist) ;
		data[L"message"] = cpptempl::make_data(m_window_listener->get_message()) ;
	}
}