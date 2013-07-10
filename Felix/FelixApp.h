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
	static app_props::props_ptr get_props();

	static model_iface_ptr get_model_interface();
	/** Get the MainFrame.
	*/
	static MemoryWindowFrame& get_app(void);

};
