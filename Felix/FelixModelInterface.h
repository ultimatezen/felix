#pragma once

#include "memory_model.h"

class FelixModelInterface
{
public:
	bool m_is_reverse_lookup ;

	typedef boost::shared_ptr<memory_engine::memory_model> model_ptr ;

	model_ptr m_memories ;

	model_ptr create_memory_model()
	{
		return model_ptr(new memory_engine::memory_model_mem()) ;
	}

	FelixModelInterface(void) :
		m_is_reverse_lookup(false)
	{
		m_memories = create_memory_model() ;
	}

	virtual ~FelixModelInterface(void)
	{
	}
};
