#pragma once

#include "FrameListener.h"


class FrameListenerFake : public FrameListener
{
public:
	DECLARE_SENSING_VAR ;
	bool set_window_title()
	{
		SENSE("set_window_title") ;
		return true ;
	}
	void save_memory_as(mem_engine::memory_pointer)
	{
		SENSE("save_memory_as") ;
	}
	void get_qc_messages(mem_engine::record_pointer, std::vector<wstring> &){}
};