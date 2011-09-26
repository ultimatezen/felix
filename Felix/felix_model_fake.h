#pragma once

#include "FelixModelInterface.h"
#include "search_match.h"
#include "felix_factory.h"

class FelixModelInterfaceFake : public FelixModelInterface
{
public:
	DECLARE_SENSING_VAR ;

	typedef boost::shared_ptr<mem_engine::memory_model> model_ptr ;
	typedef mem_engine::memory_pointer memory_type ;

	model_ptr m_model ;
	bool m_is_reverse_lookup ;

	FelixModelInterfaceFake() :
		m_is_reverse_lookup(false)
	{
		app_props::props_ptr props = FelixFactory().make_props() ;
		m_model = model_ptr(new mem_engine::memory_model_mem(&props->m_mem_props,
			&props->m_gloss_props,
			&props->m_alg_props)) ;
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
	model_ptr create_memory_model(bool)
	{
		app_props::props_ptr props = FelixFactory().make_props() ;
		return model_ptr(new mem_engine::memory_model_mem(&props->m_mem_props,
			&props->m_gloss_props,
			&props->m_alg_props)) ;
	}
};
