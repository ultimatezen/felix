#pragma once

#include "memory_model.h"

class FelixModelInterface
{
public:
	typedef boost::shared_ptr<mem_engine::memory_model> model_ptr ;
	typedef mem_engine::memory_pointer memory_type ;

	virtual ~FelixModelInterface(){}

	size_t size()
	{
		return this->get_memories()->size() ;
	}
	memory_type memory_at(size_t i)
	{
		return this->get_memories()->get_memory_at(i) ;
	}
	mem_engine::memory_iterator begin()
	{
		return this->get_memories()->begin() ;
	}
	mem_engine::memory_iterator end()
	{
		return this->get_memories()->end() ;
	}
	void remove_memory(memory_type mem)
	{
		this->get_memories()->remove_memory_by_id(mem->get_id()) ;
	}

	void add_memory()
	{
		this->get_memories()->add_memory() ;
	}
	bool perform_search( mem_engine::search_match_container &matches, 
		const mem_engine::search_query_params &params )
	{
		return this->get_memories()->perform_search(matches, params) ;
	}
	virtual mem_engine::memory_pointer get_first_memory()
	{
		return this->get_memories()->get_first_memory() ;
	}

	// pure virtual functions
	virtual model_ptr get_memories() = 0 ;
	virtual void set_reverse_lookup(bool setting) = 0 ;
	virtual bool is_reverse_lookup() = 0 ;
	virtual int get_first_mem_id() = 0 ;
	virtual mem_engine::memory_pointer get_memory_by_id(int id) = 0 ;
};


class FelixModel : public FelixModelInterface
{
public:
	bool m_is_reverse_lookup ;

	model_ptr m_memories ;

	FelixModel(void) :
		m_is_reverse_lookup(false)
	{
		m_memories = create_memory_model() ;
	}

	virtual ~FelixModel(void)
	{
	}	

	model_ptr create_memory_model()
	{
		return model_ptr(new mem_engine::memory_model_mem()) ;
	}

	int get_first_mem_id()
	{
		memory_type mem = m_memories->get_first_memory() ;
		return mem->get_id() ;
	}

	model_ptr get_memories()
	{
		return m_memories ;
	}
	void set_reverse_lookup(bool setting)
	{
		m_is_reverse_lookup = setting ;
	}
	bool is_reverse_lookup()
	{
		return m_is_reverse_lookup ;
	}

	mem_engine::memory_pointer get_memory_by_id(int id)
	{
		return m_memories->get_memory_by_id(id) ;
	}

};
