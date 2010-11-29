#pragma once
#include "config_file.h"

struct Abbreviations 
{
	std::set<wstring> m_abbreviations ;
	Abbreviations();
	void load(const wstring &text);
};
