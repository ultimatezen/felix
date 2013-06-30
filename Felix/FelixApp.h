#pragma once

#include "MemoryWindowFrame.h"			// main app window class CMainFrame
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
		return app_props::get_props() ;
	}

	static model_iface_ptr get_model_interface()
	{
		app_props::props_ptr props = get_props(); 
		return model_iface_ptr(new FelixModel(props)) ;
	}
	/** Get the MainFrame.
	*/
	static MemoryWindowFrame& get_app(void) 
	{ 
		static model_iface_ptr the_model = get_model_interface();
		app_props::props_ptr props = get_props(); 
		static MemoryWindowFrame main_frame( the_model, props ) ;

		return main_frame ; 
	}

};
