#include "stdafx.h"
#include "FelixModel.h"

using namespace mem_engine ;

FelixModel::FelixModel( app_props::props_ptr props, bool is_memory/*=true*/ ) :
m_is_reverse_lookup(false),
	m_props(props)
{
	if (is_memory)
	{
		m_memories = mem_engine::model_ptr(new mem_engine::memory_model_mem(m_props)) ;
	}
	else
	{
		m_memories = mem_engine::model_ptr(new mem_engine::memory_model_gloss(m_props)) ;
	}

}

FelixModel::~FelixModel( void )
{

}

size_t FelixModel::get_first_mem_id()
{
	memory_type mem = m_memories->get_first_memory() ;
	return mem->get_id() ;
}

model_ptr FelixModel::get_memories()
{
	return m_memories ;
}

void FelixModel::set_reverse_lookup( bool setting )
{
	m_is_reverse_lookup = setting ;
}

bool FelixModel::is_reverse_lookup()
{
	return m_is_reverse_lookup ;
}

memory_pointer FelixModel::get_memory_by_id( size_t id )
{
	return m_memories->get_memory_by_id(id) ;
}

memory_list & FelixModel::get_memories_needing_saving( memory_list &memories )
{
	m_memories->get_memories_needing_saving(memories) ;
	return memories ;
}

mem_engine::memory_pointer FelixModel::get_memory_at( size_t index )
{
	return m_memories->get_memory_at(index) ;
}

