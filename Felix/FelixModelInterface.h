#pragma once

#include "memory_model.h"
#include "file.h"

class FelixModelInterface
{
public:
	typedef boost::shared_ptr<mem_engine::memory_model> model_ptr ;
	typedef mem_engine::memory_pointer memory_type ;
	typedef std::list< memory_type > memory_list ;

	virtual ~FelixModelInterface();

	// querying
	size_t size();
	bool empty();
	bool has_name_clash(CString location);
	void record_loaded_memories(std::vector<wstring> &local, std::vector<wstring> &remote);

	// getting
	mem_engine::memory_iterator begin();
	memory_type memory_at(size_t i);
	virtual mem_engine::memory_pointer get_first_memory();

	// removing
	void remove_memory(memory_type mem);
	bool remove_memory_by_id( const size_t memid );
	virtual void clear();

	void remove_record(mem_engine::record_pointer record);

	// adding
	void insert_memory(memory_type mem);
	memory_type add_memory();
	memory_type create_memory();

	// searching
	bool perform_search( mem_engine::search_match_container &matches, 
		const mem_engine::search_query_params &params );
	bool find_matches ( mem_engine::trans_match_container &matches, 
		const mem_engine::search_query_params &params );
	bool find_trans_matches ( mem_engine::trans_match_container &matches, 
		const mem_engine::search_query_params &params );

	//////////////////////////////////////////////////////////////////////////
	// pure virtual functions
	//////////////////////////////////////////////////////////////////////////
	virtual model_ptr get_memories() = 0 ;
	virtual void set_reverse_lookup(bool setting) = 0 ;
	virtual bool is_reverse_lookup() = 0 ;
	virtual size_t get_first_mem_id() = 0 ;
	virtual mem_engine::memory_pointer get_memory_by_id(size_t id) = 0 ;
	virtual void get_memories_needing_saving( memory_list &memories ) = 0 ;
	virtual model_ptr create_memory_model(bool is_memory) = 0 ;
};


typedef boost::shared_ptr<FelixModelInterface> model_iface_ptr ;

