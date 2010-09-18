#pragma once

#include "TranslationMemory.h"

class FrameListener
{
public:
	virtual bool set_window_title() = 0 ;
	virtual void save_memory_as(mem_engine::memory_pointer mem) = 0;
};