#include "stdafx.h"
#include "view_state_initial.h"
#include "view_interface_fake.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	// windows_ui
	TEST( view_state_initial_test, handle_toggle_edit_mode_false )
	{
		view_interface_fake view ;
		ASSERT_TRUE(! view.is_edit_mode()) ;

		ViewStateInitial state ;
		state.set_view_interface(&view) ;
		state.handle_toggle_edit_mode() ;

		ASSERT_TRUE(view.is_edit_mode()) ;
	}

	TEST( view_state_initial_test, handle_toggle_edit_mode_true )
	{
		view_interface_fake view ;
		view.put_edit_mode(true) ;
		ASSERT_TRUE(view.is_edit_mode()) ;

		ViewStateInitial state ;
		state.set_view_interface(&view) ;
		state.handle_toggle_edit_mode() ;

		ASSERT_TRUE(! view.is_edit_mode()) ;
	}

}


#endif // #ifdef UNIT_TEST