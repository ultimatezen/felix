#pragma once

#include "MainFrm.h"			// main app window class CMainFrame
#include "MainFrameModel.h"

/**
@class app
@brief Singleton class for mainframe.
*/
class app
{
	// Operations
public:

	/** Get the MainFrame.
	*/
	static CMainFrame& get_app(void) 
	{ 
		static MainFrameModel the_model ;
		static CMainFrame the_view( &the_model ) ;

		return the_view ; 
	}

};
