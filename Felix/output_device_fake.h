#pragma once

#include "output_device.h"

class OutputDeviceFake : public OutputDevice
{
public:
	wstring m_value ;

	void write(const char* text, size_t len)
	{
		len ;
		m_value += string2wstring(string(text)) ;
	}
	void write(const string &text)
	{
		m_value += string2wstring(text) ;
	}
	void write(const wstring &text)
	{
		m_value += text ;
	}
};