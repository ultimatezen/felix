#pragma once

#include "office_seg.h"

class OfficeSegFake : public OfficeSeg
{
public:
	wchar_t last_char ;
	wstring seg_text ;

	wchar_t get_last_char()
	{
		return this->last_char ;
	}
	wstring get_seg_text()
	{
		return this->seg_text ;
	}
};