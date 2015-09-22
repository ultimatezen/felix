#pragma once

class KeyMapper
{
public:
	virtual ~KeyMapper(){}
	virtual bool map_command(string command) = 0 ;
};