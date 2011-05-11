#pragma once

#include "FelixModelInterface.h"
#include "search_match.h"

class FelixModelInterfaceFake : public FelixModelInterface
{
public:
	DECLARE_SENSING_VAR ;

	typedef boost::shared_ptr<mem_engine::memory_model> model_ptr ;
	typedef mem_engine::memory_pointer memory_type ;

	app_props::properties_memory mem_props ;
	app_props::properties_glossary gloss_props ;
	app_props::properties_algorithm algo_props ;

	model_ptr m_model ;
	bool m_is_reverse_lookup ;

	FelixModelInterfaceFake() :
		m_is_reverse_lookup(false)
	{
		m_model = model_ptr(new mem_engine::memory_model_mem(&mem_props,
			&gloss_props,
			&algo_props)) ;
	}
	
	void get_memories_needing_saving( memory_list &memories )
	{
		m_model->get_memories_needing_saving(memories) ;
	}

	model_ptr get_memories()
	{
		SENSE("get_memories") ;
		return m_model ;
	}
	void set_reverse_lookup(bool setting)
	{
		SENSE("set_reverse_lookup") ;
		m_is_reverse_lookup = setting ;
	}
	bool is_reverse_lookup()
	{
		SENSE("is_reverse_lookup") ;
		return m_is_reverse_lookup ;
	}
	int get_first_mem_id()
	{
		SENSE("get_first_mem_id") ;
		return m_model->get_first_memory()->get_id() ; 
	}
	mem_engine::memory_pointer get_memory_by_id(int id)
	{
		SENSE("get_memory_by_id") ;
		return m_model->get_memory_by_id(id) ;
	}

};
