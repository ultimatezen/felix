#pragma once
#include "config_file.h"

// Holds a list of abbreviations that do not
// signify segment-end
struct Abbreviations 
{
	std::set<wstring> m_abbreviations ;
	Abbreviations();
	void load(const wstring &text);
};
