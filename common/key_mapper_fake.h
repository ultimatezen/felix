#pragma once

#include "key_mapper.h"

class KeyMapperFake : public KeyMapper
{
public:
	DECLARE_SENSING_VAR ;
	bool map_command(string command)
	{
		command ;
		SENSE(command) ;
		return true ;
	}
};