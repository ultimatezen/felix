#pragma once

#include "MainFrm.h"			// main app window class CMainFrame
#include "FelixModelInterface.h"

/**
@class app
@brief Singleton class for mainframe.
*/
class app
{
	// Operations
public:
	static app_props::props_ptr get_props()
	{
		static app_props::props_ptr m_props(new app_props::properties) ;

		return m_props ;
	}

	static model_iface_ptr get_model_interface()
	{
		app_props::props_ptr props = get_props(); 
		return model_iface_ptr(new FelixModelInterface(&props->m_mem_props,
			&props->m_gloss_props,
			&props->m_alg_props) ;
	}
	/** Get the MainFrame.
	*/
	static CMainFrame& get_app(void) 
	{ 
		static model_iface_ptr the_model = get_model_interface();
		app_props::props_ptr props = get_props(); 
		static CMainFrame main_frame( the_model, props ) ;

		return main_frame ; 
	}

};
