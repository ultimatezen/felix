#pragma once
#include "window_listener.h"
#include "record_local.h"

class WindowListenerFake : public WindowListener
{
	typedef mem_engine::record_pointer rec_ptr ;
	typedef mem_engine::record_local rec_type ;
public:
	DECLARE_SENSING_VAR ;
	rec_ptr new_rec ;

	WindowListenerFake() : 
		new_rec(rec_ptr(new rec_type))
	{

	}

	void show_edit_dialog( rec_ptr &record, 
		const int memory_id, 
		UINT title_id )
	{
		record ;
		memory_id ;
		title_id ;
		SENSE("show_edit_dialog") ;
	}
	void set_new_record(rec_ptr rec)
	{
		new_rec = rec ;
		SENSE("set_new_record") ;
	}
	rec_ptr get_new_record()
	{
		SENSE("get_new_record") ;
		return new_rec ;
	}

};
