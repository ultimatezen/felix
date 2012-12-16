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
	app_props::props_ptr m_props ;

public:
	memory_model(app_props::props_ptr props);
	virtual ~memory_model();

	void add_record( const record_pointer &record, const size_t memory_id );
	void swap_memories( memory_list &memories );
	void remove_record( record_pointer record, const size_t id );
	void remove_record( record_pointer record );

	void clear(){m_memories.clear();}
	bool empty();
	size_t size();
	memory_iterator begin() { return m_memories.begin() ; }
	memory_iterator end() { return m_memories.end() ; }

	bool get_glossary_matches( search_match_container &matches, const search_query_params &params );
	bool perform_search( search_match_container &matches, const search_query_params &params ) ;
	bool find_trans_matches ( trans_match_container &matches, const search_query_params &params ) ;
	bool find_matches ( trans_match_container &matches, const search_query_params &params ) ;
	
	memory_list& get_memories();
	void insert_memory( memory_pointer mem );
	memory_pointer add_memory( );
	void reduce_size_to( const size_t max_size );
	bool remove_memory_by_name( const CString &mem_name ) ;
	bool remove_memory_by_id( const size_t id ) ;
	memory_pointer get_memory_by_name( const CString mem_name ) ;
	memory_pointer get_memory_by_id( const size_t id );
	memory_pointer get_memory_at(const size_t i);
	void get_memories_needing_saving( memory_list &memories );
	
	memory_pointer get_first_memory();
	size_t total_memory_size() ;

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
	memory_model_mem(app_props::props_ptr props):
		memory_model(props)
	{

	}
	memory_pointer create_memory() ;
};

/**
	@class memory_model_gloss 
	@brief glossary dialog memories.
 */
class memory_model_gloss : public memory_model
{
public:
	memory_model_gloss(app_props::props_ptr props):
		memory_model(props)
	{

	}
	memory_pointer create_memory() ;
};

typedef boost::shared_ptr<memory_model> model_ptr ;

}