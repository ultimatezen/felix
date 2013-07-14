#include "stdafx.h"
#include "FelixModel.h"

using namespace mem_engine ;

FelixModel::FelixModel( app_props::props_ptr props, bool is_memory/*=true*/ ) :
m_is_reverse_lookup(false),
	m_props(props)
{
	m_memories = create_memory_model(is_memory) ;
}

FelixModel::~FelixModel( void )
{

}

model_ptr FelixModel::create_memory_model( bool is_memory/*=false*/ )
{
	if (is_memory)
	{
		return model_ptr(new mem_engine::memory_model_mem(m_props)) ;
	}
	else
	{
		return model_ptr(new mem_engine::memory_model_gloss(m_props)) ;
	}
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

