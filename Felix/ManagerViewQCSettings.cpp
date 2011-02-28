#include "stdafx.h"
#include "ManagerViewQCSettings.h"
#include "SearchWindow.h"
#include "document_wrapper.h"

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
	}
}