// memory_model.cpp: implementation of the memory_model class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "memory_model.h"
#include "memory_local.h"

namespace mem_engine
{
	using namespace except ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

memory_model::memory_model(app_props::props_ptr props) :
	m_props(props)
{

}

memory_model::~memory_model()
{

}

memory_pointer memory_model::get_first_memory()
{
	if ( m_memories.empty() )
	{
		return add_memory() ;
	}

	return *(m_memories.begin()) ;
}

memory_pointer memory_model::get_memory_by_name( const CString mem_name )
{
	FOREACH(memory_pointer mem, m_memories)
	{
		CString cmp_name = mem->get_location(); 

		if ( 0 == mem_name.CompareNoCase( cmp_name ) )
		{
			return mem ;
		}
	}
	
	return add_memory() ;
}


/** Sum of sizes of all memories
 */
size_t memory_model::total_memory_size()
{
	size_t total_size = 0 ;

	FOREACH(memory_pointer mem, m_memories)
	{
		total_size += mem->size() ;
	}

	return total_size ;
}

size_t memory_model::size()
{
	return m_memories.size() ;
}

bool memory_model::empty()
{
	return m_memories.empty() ;
}

/** The memories that are dirty.
 */
void memory_model::get_memories_needing_saving(memory_list &memories)
{
	FOREACH(memory_pointer mem, m_memories)
	{
		if ( mem->is_saved() == false )
		{
			memories.push_back(mem) ;
		}
	}
}

bool memory_model::remove_memory_by_name( const CString &mem_name )
{
	for ( memory_iterator pos = begin() ; pos != end() ; ++pos )
	{
		memory_pointer mem = *pos ;
		CString cmp_name = mem->get_location(); 
		
		if ( 0 == mem_name.CompareNoCase( cmp_name ) )
		{
			m_memories.erase( pos ) ;
			return true ;
		}
	}
	return false ;
}

bool memory_model::remove_memory_by_id( const size_t id )
{
	for ( memory_iterator pos = begin() ; pos != end() ; ++pos )
	{
		memory_pointer mem = *pos ;
		size_t cmp_id = mem->get_id(); 
		
		if ( cmp_id == id )
		{
			m_memories.erase( pos ) ;
			return true ;
		}
	}
	
	return false ;
}

void memory_model::reduce_size_to(const size_t max_size)
{
	while ( total_memory_size() > max_size )
	{
		m_memories.pop_back() ;
	}

}

memory_pointer memory_model::add_memory( )
{
	memory_pointer mem = create_memory() ;

	m_memories.push_front( mem ) ;

	return mem ;
}

void memory_model::insert_memory( memory_pointer mem )
{
	m_memories.push_front( mem ) ;
}

bool memory_model::get_glossary_matches( search_match_container &matches, const search_query_params &params )
{
	bool search_success = false ;
	memory_iterator pos ;
	for ( pos = m_memories.begin() ; pos != m_memories.end() ; ++pos )
	{
		memory_pointer mem = *pos ;
		if( mem->get_glossary_matches( matches, params ) )
		{
			search_success = true ;
		}
	}

	return search_success ;
}

bool memory_model::perform_search( search_match_container &matches, const search_query_params &params )
{
	bool search_success = false ;
	FOREACH ( memory_pointer mem, m_memories )
	{
		if( mem->perform_search( matches, params ) )
		{
			search_success = true ;
		}
	}
	
	return search_success ;
}

// finding matches

bool memory_model::find_trans_matches( trans_match_container &matches, const search_query_params &params )
{
	bool search_success = false ;
	FOREACH(memory_pointer mem, m_memories)
	{
		if( mem->find_trans_matches( matches, params ) )
		{
			search_success = true ;
		}
	}
	return search_success ;
}

bool memory_model::find_matches( trans_match_container &matches, const search_query_params &params )
{
	bool search_success = false ;
	FOREACH(memory_pointer mem, m_memories)
	{
		if( mem->find_matches( matches, params ) )
		{
			search_success = true ;
		}
	}
	return search_success ;
}

// operations on records

void memory_model::remove_record( record_pointer record, const size_t id )
{
	memory_pointer mem = get_memory_by_id(id) ;
	mem->erase( record ) ;
}

void memory_model::remove_record( record_pointer record )
{
	FOREACH(memory_pointer mem, m_memories)
	{
		mem->erase(record) ;
	}
}

void memory_model::swap_memories(memory_list &memories)
{
	m_memories.swap( memories ) ;
}



void memory_model::add_record( const record_pointer &record, const size_t memory_id)
{
	memory_pointer mem ;
	try
	{
		mem = get_memory_by_id(memory_id) ;
	}
	catch (CProgramException& )
	{
		mem = get_first_memory() ;
	}

	mem->add_record( record ) ;
}

mem_engine::memory_pointer memory_model::get_memory_by_id( const size_t cmp_id )
{
	FOREACH(memory_pointer mem, m_memories)
	{
		if ( cmp_id == mem->get_id() )
		{
			return mem ;
		}
	}
	
	CString msg ;
	msg.Format(_T("Memory not found with id %d"), cmp_id) ;
	throw CProgramException(msg) ;
}

mem_engine::memory_pointer memory_model::get_memory_at( const size_t i )
{
	memory_iterator pos = this->begin() ;
	std::advance(pos, i) ;
	return *pos ;
}

memory_list& memory_model::get_memories()
{
	return m_memories ;
}
memory_pointer memory_model_mem::create_memory()
{
	memory_pointer mem(new mem_engine::memory_local(m_props)) ;
	mem->set_is_memory( true ) ;
	return mem ;
}

memory_pointer memory_model_gloss::create_memory()
{
	memory_pointer mem(new mem_engine::memory_local(m_props)) ;
	mem->set_is_memory( false ) ;
	return mem ;
}
}