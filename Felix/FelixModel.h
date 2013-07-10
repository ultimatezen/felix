#pragma once

#include "FelixModelInterface.h"

class FelixModel : public FelixModelInterface
{
public:
	bool m_is_reverse_lookup ;

	model_ptr m_memories ;

	app_props::props_ptr m_props; 


	FelixModel(app_props::props_ptr props,
		bool is_memory=true);

	virtual ~FelixModel(void);	

	model_ptr create_memory_model(bool is_memory=false);

	size_t get_first_mem_id();

	model_ptr get_memories();
	void set_reverse_lookup(bool setting);
	bool is_reverse_lookup();

	mem_engine::memory_pointer get_memory_by_id(size_t id);
	void get_memories_needing_saving( memory_list &memories );

};
