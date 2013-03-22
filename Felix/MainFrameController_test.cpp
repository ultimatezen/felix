
#include "StdAfx.h"
#include "mainfrm.h"
#include "record_local.h"
#include "view_interface_fake.h"
#include "window_listener_fake.h"
#include "memory_local.h"
#include "record_local.h"
#include "felix_model_fake.h"
#include "felix_factory.h"
#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( MainFrameControllerTests )


	using namespace mem_engine ;
	using namespace except ;

	frame_ptr make_fake_frame()
	{
		app_props::props_ptr props(new app_props::properties); 
		model_iface_ptr the_model(new FelixModelInterfaceFake);
		return frame_ptr(new CMainFrame( the_model, props )) ;
	}


BOOST_AUTO_TEST_SUITE_END()

#endif 


