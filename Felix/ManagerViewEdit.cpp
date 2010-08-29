#include "stdafx.h"
#include "ManagerViewEdit.h"
#include "TextTemplate.h"
#include "numberfmt.h"
#include "SearchWindow.h"
#include "MemoryManagerDlg.h"
#include "document_wrapper.h"

#ifdef UNIT_TEST
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#endif

namespace mgrview
{
	void ManagerViewEdit::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewEdit::show_content()
	{
		SENSE("show_content") ;
		if(m_is_memory)
		{
			m_model = m_mem_model ;
		}
		else
		{
			m_model = m_gloss_model ;
		}

		text_tmpl::CTextTemplate engine ;

		mem_engine::memory_pointer mem = m_model->memory_at(m_item) ;
		mem_engine::MemoryInfo *mem_info = mem->get_memory_info() ;

		CNumberFmt number_format ;
		wstring memsize = (LPCWSTR)(number_format.Format(mem->size())) ;
		engine.Assign("size", memsize) ;

		engine.Assign("name", get_memname(mem)) ;
		engine.Assign("memtype", m_is_memory ? L"mem" : L"gloss") ;
		engine.Assign("index", tows(m_item)) ;

		CMemoryManagerDlg dlg ;

		engine.Assign(L"file_name", dlg.get_memory_name(mem)) ;
		engine.Assign(L"creator", dlg.get_creator_name(mem_info)) ;
		engine.Assign(L"field", dlg.get_field_name(mem_info)) ;
		engine.Assign(L"created_on", dlg.get_created_on(mem_info)) ;
		engine.Assign(L"source_language", dlg.get_source_lang(mem_info)) ;
		engine.Assign(L"target_language", dlg.get_target_lang(mem_info)) ;
		engine.Assign(L"client", dlg.get_client_name(mem_info)) ;
		engine.Assign(L"mem_size", dlg.get_mem_size(mem)) ;
		engine.Assign(L"file_size", dlg.get_file_size(mem)) ;
		engine.Assign(L"reliability", dlg.get_reliability_range(mem)) ;
		engine.Assign(L"validated", mem->get_validated_percent()) ;
		engine.Assign(L"locked", bool2wstring( mem->is_locked() )) ;

		wstring tpl_text = text_tmpl::get_template_text(_T("manager/edit.txt")) ;
		m_view->set_text(engine.Fetch(tpl_text)) ;

		m_view->ensure_document_complete() ;
		m_view->ensure_navigation_complete() ;

		m_view->run_script("focusItem", "creator") ;

	}
}