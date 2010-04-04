#include "stdafx.h"
#include "view_state_initial.h"
#include "view_interface_fake.h"
#include "window_listener_fake.h"
#include "felix_model_fake.h"
#include "memory_local.h"
#include "record_local.h"
#include "mainfrm.h"
#include "GlossaryDialog.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine ;
	TEST( window_state_tests_mainframe, retrieve_edit_record)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
	}

}


#endif // #ifdef UNIT_TEST