#pragma once

#include "view_interface.h"
#include "FelixModelInterface.h"
#include "TranslationMemory.h"
#include "search_match.h"

namespace mgrview
{
	// turns a bool into a true/false string ("true" or empty string)
	cpptempl::data_ptr item_bool(bool value) ;

	class FelixManagerWindowListener
	{
	public:
		virtual wstring get_message() = 0 ;
		virtual std::vector<mem_engine::search_match_ptr> & get_qc_matches() = 0 ;
	};

	class ManagerView
	{
		public:
			typedef mem_engine::search_match_ptr search_match_ptr ;
			typedef mem_engine::search_match search_match ;
			typedef mem_engine::record_pointer record_pointer ;

			view_interface *m_view ;
			model_iface_ptr m_mem_model ;
			model_iface_ptr m_gloss_model ;
			FelixManagerWindowListener *m_window_listener ;

			ManagerView():
				m_view(NULL),
				m_window_listener(NULL)
			{
			}

			virtual ~ManagerView(){}

			void set_view(view_interface *view);
			void set_mem_model(model_iface_ptr model);
			void set_gloss_model(model_iface_ptr model);
			void set_listener(FelixManagerWindowListener *listener);

			// pure virtual
			virtual void activate() = 0 ;
			virtual void show_content() = 0 ;

	};
	void set_mem_tmpl_info( mem_engine::memory_pointer mem, 
							cpptempl::data_map &data, 
							bool is_memory,
							size_t index ) ;
	void set_record_info( cpptempl::data_map &item, mem_engine::record_pointer record ) ;
	void set_memory_info( mem_engine::memory_pointer mem, cpptempl::data_map &item ) ;
	wstring get_memname(mem_engine::memory_pointer mem) ;
	void set_index_info( cpptempl::data_map &item, size_t i ) ;

	typedef std::shared_ptr<ManagerView> mgr_ptr ;

}