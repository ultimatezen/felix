#include "stdafx.h"
#include "ManagerViewQCSettings.h"
#include "SearchWindow.h"
#include "document_wrapper.h"
#include "numberfmt.h"

#ifdef UNIT_TEST
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#endif

namespace mgrview
{
	void ManagerViewQCSettings::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewQCSettings::show_content()
	{
		SENSE("show_content") ;

		cpptempl::data_map data ;
		set_template_data(data);

		wstring tpl_text = cpptempl::get_template_text(_T("manager/qc_settings.txt")) ;
		m_view->set_text(cpptempl::parse(tpl_text, data)) ;

		m_view->ensure_document_complete() ;
		m_view->ensure_navigation_complete() ;

		m_view->run_script("focusItem", "numbers") ;

	}

	void ManagerViewQCSettings::set_template_data( cpptempl::data_map &data )
	{
		data[L"message"] = cpptempl::make_data(m_window_listener->get_message()) ;
		data[L"numbers"] = cpptempl::make_data(m_props->m_qc_props.m_data.m_check_numbers ? L"true" : L"") ;
		data[L"allcaps"] = cpptempl::make_data(m_props->m_qc_props.m_data.m_check_all_caps ? L"true" : L"") ;
		data[L"gloss"] = cpptempl::make_data(m_props->m_qc_props.m_data.m_check_gloss ? L"true" : L"") ;
		data[L"live"] = cpptempl::make_data(m_props->m_qc_props.m_data.m_live_checking ? L"true" : L"") ;

		make_mem_list(m_gloss_model, data) ;
	}

	void ManagerViewQCSettings::make_mem_list( model_iface_ptr mems, cpptempl::data_map &data )
	{
		cpptempl::data_list tms ;
		for(size_t i = 0 ; i < mems->size() ; ++i)
		{
			mem_engine::memory_pointer mem = mems->memory_at(i) ;
			cpptempl::data_map item ;

			wstring name = mem->get_fullpath() ;
			item[L"name"] = cpptempl::make_data(name) ;
			item[L"id"] = cpptempl::make_data(fmt_num(mem->get_id())) ;

			if (m_props->m_qc_props.check_gloss_name(name))
			{
				item[L"checked"] = cpptempl::make_data(L"true") ;
			}
			else
			{
				item[L"checked"] = cpptempl::make_data(L"") ;
			}

			tms.push_back(cpptempl::make_data(item)) ;
		}
		data[L"glosses"] = cpptempl::make_data(tms) ;
	}
}