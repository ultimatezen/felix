#pragma once

#include "record.h"

class WindowListener
{
public:
	virtual void show_edit_dialog( mem_engine::record_pointer &record, 
								   const int memory_id, 
								   UINT title_id ) = 0 ;
	virtual void set_new_record(mem_engine::record_pointer rec) = 0 ;
	virtual mem_engine::record_pointer get_new_record() = 0 ;

};