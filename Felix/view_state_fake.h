#pragma once
#include "view_state.h"

#ifdef UNIT_TEST

class ViewStateFake : public ViewState
{
public:
	DECLARE_SENSING_VAR ;

	void handle_toggle_edit_mode()
	{
		SENSE("handle_toggle_edit_mode") ;
	}
	void show_content() 
	{
		SENSE("show_content") ;

	}
	void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec) 
	{
		new_rec ;
		SENSE("retrieve_edit_record") ;
		SENSE(int2string(mem_id)) ;
	}

};

#endif
