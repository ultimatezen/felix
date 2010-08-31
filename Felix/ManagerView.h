#pragma once

#include "view_interface.h"
#include "FelixModelInterface.h"
#include "TranslationMemory.h"
#include "TextTemplate.h"

namespace mgrview
{
	class FelixManagerWindowListener
	{
	public:

	};

	class ManagerView
	{
		public:
			typedef mem_engine::search_match_ptr search_match_ptr ;
			typedef mem_engine::search_match search_match ;
			typedef mem_engine::record_pointer record_pointer ;

			view_interface *m_view ;
			FelixModelInterface *m_mem_model ;
			FelixModelInterface *m_gloss_model ;
			FelixManagerWindowListener *m_window_listener ;
			wstring m_message ;

			ManagerView():
				m_view(NULL),
				m_mem_model(NULL),
				m_gloss_model(NULL),
				m_window_listener(NULL)
			{
			}

			virtual ~ManagerView(){}

			void set_view(view_interface *view);
			void set_mem_model(FelixModelInterface *model);
			void set_gloss_model(FelixModelInterface *model);
			void set_listener(FelixManagerWindowListener *listener);

			// pure virtual
			virtual void activate() = 0 ;
			virtual void show_content() = 0 ;

	};
	void set_mem_tmpl_info( mem_engine::memory_pointer mem, 
							text_tmpl::CTextTemplate &engine, 
							bool is_memory,
							size_t index ) ;

	wstring get_memname(mem_engine::memory_pointer mem) ;

	typedef boost::shared_ptr<ManagerView> mgr_ptr ;
}