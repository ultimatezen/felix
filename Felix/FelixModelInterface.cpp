#include "stdafx.h"
#include "FelixModelInterface.h"
#include "query.h"

using namespace mem_engine ;

FelixModelInterface::~FelixModelInterface()
{

}

size_t FelixModelInterface::size()
{
	return this->get_memories()->size() ;
}

FelixModelInterface::memory_type FelixModelInterface::memory_at( size_t i )
{
	return this->get_memories()->get_memory_at(i) ;
}

mem_engine::memory_iterator FelixModelInterface::begin()
{
	return this->get_memories()->begin() ;
}

void FelixModelInterface::remove_memory( memory_type mem )
{
	this->get_memories()->remove_memory_by_id(mem->get_id()) ;
}

bool FelixModelInterface::remove_memory_by_id( const size_t memid )
{
	return get_memories()->remove_memory_by_id(memid) ;
}

FelixModelInterface::memory_type FelixModelInterface::add_memory()
{
	return this->get_memories()->add_memory() ;
}

bool FelixModelInterface::perform_search( mem_engine::search_match_container &matches, const mem_engine::search_query_params &params )
{
	return this->get_memories()->perform_search(matches, params) ;
}

mem_engine::memory_pointer FelixModelInterface::get_first_memory()
{
	return this->get_memories()->get_first_memory() ;
}

void FelixModelInterface::clear()
{
	this->get_memories()->clear() ;
}

bool FelixModelInterface::empty()
{
	return get_memories()->empty() ;
}

void FelixModelInterface::insert_memory( memory_type mem )
{
	get_memories()->insert_memory(mem) ;
}

FelixModelInterface::memory_type FelixModelInterface::create_memory()
{
	return get_memories()->create_memory() ;
}

bool FelixModelInterface::find_matches( trans_match_container &matches, const search_query_params &params )
{
	return get_memories()->find_matches(matches, params) ;
}

bool FelixModelInterface::find_trans_matches( trans_match_container &matches, const search_query_params &params )
{
	return get_memories()->find_trans_matches(matches, params) ;
}

void FelixModelInterface::remove_record( mem_engine::record_pointer record )
{
	get_memories()->remove_record(record) ;
}

bool FelixModelInterface::get_glossary_matches( mem_engine::search_match_container &matches, const mem_engine::search_query_params &params )
{
	return get_memories()->get_glossary_matches(matches, params) ;
}

bool FelixModelInterface::has_name_clash( CString location )
{
	FOREACH(memory_type mem, get_memories()->get_memories())
	{
		if (file::is_same_file(location, mem->get_location()))
		{
			return true ;
		}
	}
	return false ;
}

void FelixModelInterface::record_loaded_memories( std::vector<wstring> &local, std::vector<wstring> &remote )
{
	FOREACH(memory_pointer mem, get_memories()->get_memories())
	{
		tstring location = (LPCTSTR)mem->get_fullpath();
		if (! mem->is_local())
		{
			remote.push_back(location);
		}
		else
		{
			local.push_back(location);
		}
	}
}

mem_engine::memory_pointer FelixModelInterface::get_memory_by_name( CString name )
{
	return get_memories()->get_memory_by_name(name) ;
}

// Retrieves the names (locations) of the TMs in the collection.
// List is ordered by the order of the TMs.
// We only wan the filenames, not the full paths.
std::vector<wstring>& FelixModelInterface::get_memory_names( std::vector<wstring>& names )
{
	FOREACH(memory_pointer mem, get_memories()->get_memories())
	{
		CString location = mem->get_location();
		names.push_back(mem_engine::get_fname_from_loc((LPCWSTR)location));
	}
	return names ;
}

size_t FelixModelInterface::total_memory_size()
{
	return get_memories()->total_memory_size() ;
}

void FelixModelInterface::reduce_size_to( const size_t max_size )
{
	get_memories()->reduce_size_to(max_size) ;
}
