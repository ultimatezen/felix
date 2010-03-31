/*!
	@file memory_model.h
	@brief interface for the memory_model class.
 */

#pragma once

#include "TranslationMemory.h"

namespace mem_engine
{

enum memory_type { MEMORY_TYPE_MEMORY, MEMORY_TYPE_GLOSSARY } ;

/**
	@class memory_model  
	@brief Holder for multiple memories.
 */
class memory_model  
{
	memory_list				m_memories ;

public:
	void add_record( const record_pointer &record, const int memory_id );
	void swap_memories( memory_list &memories );
	void remove_record( record_pointer record, const int id );
	void remove_record( record_pointer record );

	void set_properties_memory( const app_props::properties_memory &props ) ;
	void set_properties_gloss( const app_props::properties_glossary &props ) ;
	void set_properties_algo( const app_props::properties_algorithm &props ) ;

	void clear(){m_memories.clear();}
	bool empty();
	size_t size();
	memory_iterator begin() { return m_memories.begin() ; }
	memory_iterator end() { return m_memories.end() ; }

	bool get_glossary_matches( search_match_container &matches, const search_query_params &params );
	bool perform_search( search_match_container &matches, const search_query_params &params ) ;
	bool find_trans_matches ( trans_match_container &matches, const search_query_params &params ) ;
	bool find_matches ( trans_match_container &matches, const search_query_params &params ) ;
	
	memory_list& get_memories()
	{
		return m_memories ;
	}
	void insert_memory( memory_pointer mem );
	memory_pointer add_memory( );
	void reduce_size_to( const int max_size );
	bool remove_memory_by_name( const CString &mem_name ) ;
	bool remove_memory_by_id( const int id ) ;
	memory_pointer get_memory_by_name( const CString mem_name ) ;
	memory_pointer get_memory_by_id( const int id );
	void get_memories_needing_saving( memory_list &memories );
	
	memory_pointer get_first_memory();
	size_t total_memory_size() ;

	memory_model( );
	virtual ~memory_model();

	/************************************************************************/
	/* pure virtual functions for polymorphism                              */
	/************************************************************************/

	virtual memory_pointer create_memory() = 0 ;
};

/**
	@class memory_model_mem 
	@brief mainframe memories.
 */
class memory_model_mem : public memory_model
{
public:
	memory_pointer create_memory() ;
};

/**
	@class memory_model_gloss 
	@brief glossary dialog memories.
 */
class memory_model_gloss : public memory_model
{
public:
	memory_pointer create_memory() ;
};

}