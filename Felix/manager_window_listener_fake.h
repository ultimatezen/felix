#pragma once

#include "ManagerView.h"

class ManagerWindowListenerFake : public mgrview::FelixManagerWindowListener
{
public:
	wstring m_message ;
	std::vector<mem_engine::search_match_ptr> m_qc_matches; 
	std::vector<mem_engine::search_match_ptr> & get_qc_matches()
	{
		return m_qc_matches ;
	}
	wstring get_message()
	{
		return m_message ;
	}
};
