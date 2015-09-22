#pragma once

#include "FrameListener.h"

class FrameListenerFake : public FrameListener
{
	app_props::props_ptr m_props ;
public:
	DECLARE_SENSING_VAR ;

	FrameListenerFake() : m_props(new app_props::properties){}
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
	INT_PTR check_save_memory(mem_engine::memory_pointer )
	{
		return IDOK ;
	}
	app_props::props_ptr get_properties() 
	{
		return m_props ;
	}
};