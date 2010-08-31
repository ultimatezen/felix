#pragma once

#include "ManagerView.h"

namespace mgrview
{
	class ManagerViewEdit : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		size_t m_item ;
		bool m_is_memory ;
		FelixModelInterface *m_model ;

		ManagerViewEdit(size_t item, bool is_memory=true) : 
		m_item(item),
			m_is_memory(is_memory),
			m_model(NULL),
			ManagerView()
		{}

		void activate();
		void show_content();


	};
}