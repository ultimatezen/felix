#pragma once

#include "mainfrm.h"
#include "memory_local.h"

typedef boost::shared_ptr<CMainFrame> frame_ptr ;

class FelixFactory
{
public:
	app_props::props_ptr make_props()
	{
		static app_props::props_ptr props ;
		if (! props)
		{
			props = app_props::props_ptr(new app_props::properties) ;
		}
		return props ;
	}

	static model_iface_ptr make_model_interface(app_props::props_ptr props)
	{
		return model_iface_ptr(new FelixModel(&props->m_mem_props,
			&props->m_gloss_props,
			&props->m_alg_props)) ;
	}
	/** Get the MainFrame.
	*/
	frame_ptr make_app(void) 
	{ 
		app_props::props_ptr props = make_props(); 
		model_iface_ptr the_model = make_model_interface(props);
		return frame_ptr(new CMainFrame( the_model, props )) ;
	}

	mem_engine::memory_pointer make_memory()
	{
		app_props::props_ptr props = make_props(); 
		mem_engine::memory_pointer mem(new mem_engine::memory_local(&props->m_mem_props)) ;
		return mem ;
	}
};

inline frame_ptr make_frame()
{
	return FelixFactory().make_app() ;
}