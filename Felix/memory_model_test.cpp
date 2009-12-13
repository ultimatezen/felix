/*!
@brief Unit tests for memory class
@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "memory_model.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace memory_engine ;

	record_pointer make_rec(string source, string trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		return rec ;
	}

	void add_record(memory_pointer mem, string source, string trans)
	{
		mem->add_record(make_rec(source, trans)) ;
	}

	TEST( test_memory_model_gloss, create_memory )
	{
		memory_model_gloss mm ;
		memory_pointer mem = mm.create_memory() ;
		ASSERT_TRUE(mem) ;
	}
	// getting memories
	TEST( test_memory_model_gloss, get_memory_by_name )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->set_location("first memory") ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->set_location("second memory") ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		memory_pointer retrieved1 = mm.get_memory_by_name("first memory") ;
		CStringA actual = retrieved1->get_location() ;
		ASSERT_EQUALS_V("first memory", SimpleString(actual)) ;
		ASSERT_EQUALS_V(retrieved1->get_id(), mem1->get_id()) ;

		memory_pointer retrieved2 = mm.get_memory_by_name("second memory") ;
		actual = retrieved2->get_location() ;
		ASSERT_EQUALS_V("second memory", SimpleString(actual)) ;
		ASSERT_EQUALS_V(retrieved2->get_id(), mem2->get_id()) ;
	}
	TEST( test_memory_model_gloss, get_memory_by_id )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		int first_id = mem1->get_id() ;
		memory_pointer mem2 = mm.create_memory() ;
		int second_id = mem2->get_id() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		ASSERT_TRUE(first_id != second_id) ;
		memory_pointer retrieved1 = mm.get_memory_by_id(first_id) ;
		ASSERT_EQUALS_V(first_id, retrieved1->get_id()) ;
		memory_pointer retrieved2 = mm.get_memory_by_id(second_id) ;
		ASSERT_EQUALS_V(second_id, retrieved2->get_id()) ;
	}
	TEST( test_memory_model_gloss, get_memory_by_id_not_found )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		int first_id = mem1->get_id() ;
		memory_pointer mem2 = mm.create_memory() ;
		int second_id = mem2->get_id() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		int third_id = first_id + second_id ;
		ASSERT_TRUE(third_id != first_id) ;
		ASSERT_TRUE(third_id != second_id) ;
		try
		{
			mm.get_memory_by_id(third_id) ;
			FAIL_M("Should have thrown on nonexistent memory") ;
		}
		catch (CProgramException &e)
		{
			CStringA expected ;
			expected.Format("Memory not found with id %d", third_id) ;
			CStringA actual(e.what()) ;
			ASSERT_EQUALS_V(SimpleString(expected), SimpleString(actual)) ;
			ASSERT_EQUALS_V(2, (int)mm.size()) ;
		}
	}
	// removing memories
	TEST( test_memory_model_gloss, remove_memory_by_name )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->set_location("first memory") ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->set_location("second memory") ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		mm.remove_memory_by_name("second memory") ;
		memory_pointer retrieved = mm.get_first_memory() ;
		CStringA actual = retrieved->get_location() ;
		ASSERT_EQUALS_V("first memory", SimpleString(actual)) ;
		ASSERT_EQUALS_V(1, (int)mm.size()) ;
	}
	TEST( test_memory_model_gloss, reduce_size_to )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("spam 1", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("spam 1", "egg 1")) ;
		mem2->add_record(make_rec("bogus record source", "bogus record trans")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		mm.reduce_size_to(2) ;
		ASSERT_EQUALS_V(mem2->get_id(), mm.get_first_memory()->get_id()) ;
		ASSERT_EQUALS_V(1, (int)mm.size()) ;
	}

	// adding and removing records
	TEST( test_memory_model_gloss, add_record_to_first )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		mm.add_record(make_rec("spam", "egg"), mem1->get_id()) ;
		ASSERT_EQUALS_V(1, (int)mem1->size()) ;
		ASSERT_EQUALS_V(0, (int)mem2->size()) ;
	}
	TEST( test_memory_model_gloss, add_record_to_second )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		mm.add_record(make_rec("spam", "egg"), mem2->get_id()) ;
		ASSERT_EQUALS_V(1, (int)mem2->size()) ;
		ASSERT_EQUALS_V(0, (int)mem1->size()) ;
	}
	TEST( test_memory_model_gloss, remove_record )
	{
		memory_model_gloss mm ;
		memory_pointer mem = mm.create_memory() ;
		mm.insert_memory(mem) ;

		record_pointer rec = make_rec("spam", "egg") ;
		mm.add_record(rec, mem->get_id()) ;
		ASSERT_EQUALS_V(1, (int)mem->size()) ;
		mm.remove_record(rec); 
		ASSERT_EQUALS_V(0, (int)mem->size()) ;
	}
	TEST( test_memory_model_gloss, remove_record_by_id )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		record_pointer rec = make_rec("spam", "egg") ;
		mm.add_record(rec, mem1->get_id()) ;
		mm.add_record(rec, mem2->get_id()) ;

		mm.remove_record(rec, mem1->get_id()) ;
		ASSERT_EQUALS_V(1, (int)mem2->size()) ;
		ASSERT_EQUALS_V(0, (int)mem1->size()) ;
	}
	// setting properties
	TEST( test_memory_model_gloss, set_properties_algo )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		ASSERT_EQUALS_V(IDC_ALGO_AUTO, mem1->m_algo_properties.m_data.m_match_algo) ;
		ASSERT_EQUALS_V(IDC_ALGO_AUTO, mem2->m_algo_properties.m_data.m_match_algo) ;
		app_props::properties_algorithm properties ;
		properties.m_data.m_match_algo = IDC_ALGO_WORD ;
		mm.set_properties_algo(properties) ;
		ASSERT_EQUALS_V(IDC_ALGO_WORD, mem1->m_algo_properties.m_data.m_match_algo) ;
		ASSERT_EQUALS_V(IDC_ALGO_WORD, mem2->m_algo_properties.m_data.m_match_algo) ;
	}
	TEST( test_memory_model_gloss, set_properties_gloss )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		app_props::properties_glossary properties ;
		properties.m_data.m_min_score = 50 ;
		mm.set_properties_gloss(properties) ;
		ASSERT_EQUALS_V(50, (int)mem1->m_gloss_properties.m_data.m_min_score) ;
		ASSERT_EQUALS_V(50, (int)mem2->m_gloss_properties.m_data.m_min_score) ;
	}
	TEST( test_memory_model_gloss, set_properties_memory )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		ASSERT_EQUALS_V(50, (int)mem1->m_properties.m_data.m_min_score) ;
		ASSERT_EQUALS_V(50, (int)mem2->m_properties.m_data.m_min_score) ;
		app_props::properties_memory properties ;
		properties.m_data.m_min_score = 0 ;
		mm.set_properties_memory(properties) ;
		ASSERT_EQUALS_V(0, (int)mem1->m_properties.m_data.m_min_score) ;
		ASSERT_EQUALS_V(0, (int)mem2->m_properties.m_data.m_min_score) ;
	}
	// finding matches
	TEST( test_memory_model_gloss, find_trans_matches )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("spam 1", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("spam 2", "egg 2")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		TransMatchContainer matches;
		search_query_params params ;
		params.m_rich_source = L"egg 3" ;
		mm.find_trans_matches(matches, params) ;
		ASSERT_EQUALS_V(2, (int)matches.size()) ;
	}
	TEST( test_memory_model_gloss, find_matches )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("spam 1", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("spam 2", "egg 2")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		TransMatchContainer matches;
		search_query_params params ;
		params.m_rich_source = L"spam 3" ;
		mm.find_matches(matches, params) ;
		ASSERT_EQUALS_V(2, (int)matches.size()) ;
	}
	TEST( test_memory_model_gloss, get_glossary_matches )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("Spam", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		mem1->m_gloss_properties.set_min_score(100) ;
		ASSERT_TRUE(mem1->is_active()) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("eggy", "egg 2")) ;
		mem2->m_gloss_properties.set_min_score(100) ;
		ASSERT_TRUE(mem2->is_active()) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		search_match_multiset matches;
		search_query_params params ;
		params.m_ignore_case = true ;
		params.m_source = L"Spam and eggy weggies" ;
		mm.get_glossary_matches(matches, params) ;
		int num_matches = (int)matches.size() ;
		ASSERT_EQUALS_V(2, num_matches) ;
	}
	TEST( test_memory_model_gloss, perform_search )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("lovely spam", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("wonderful spam", "egg 2")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		search_match_multiset matches;
		search_query_params params ;
		params.m_source = L"spam" ;
		mm.perform_search(matches, params) ;
		ASSERT_EQUALS_V(2, (int)matches.size()) ;
	}

	// memory info
	TEST( test_memory_model_gloss, total_memory_size_0 )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		ASSERT_EQUALS_V(0, (int)mm.total_memory_size()) ;
	}
	TEST( test_memory_model_gloss, total_memory_size_3 )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("spam 1", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("spam 2", "egg 2")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		ASSERT_EQUALS_V(3, (int)mm.total_memory_size()) ;
	}
	TEST( test_memory_model_gloss, empty_true )
	{
		memory_model_gloss mm ;

		ASSERT_TRUE(mm.empty()) ;
	}
	TEST( test_memory_model_gloss, empty_false )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		ASSERT_TRUE(! mm.empty()) ;
	}
	TEST( test_memory_model_gloss, size_0 )
	{
		memory_model_gloss mm ;

		ASSERT_EQUALS_V(0, (int)mm.size()) ;
	}
	TEST( test_memory_model_gloss, size_2 )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		ASSERT_EQUALS_V(2, (int)mm.size()) ;
	}
	TEST( test_memory_model_gloss, get_memories_needing_saving )
	{
		memory_model_gloss mm ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->set_location("first memory") ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->set_location("second memory") ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;
		mm.add_record(make_rec("spam", "eggs"), mem1->get_id()) ;

		memory_list mems ;
		mm.get_memories_needing_saving(mems) ;
		ASSERT_EQUALS_V(1, (int)mems.size()) ;
		memory_pointer retrieved = *(mems.begin()) ;
		CStringA actual = retrieved->get_location() ;
		ASSERT_EQUALS_V("first memory", SimpleString(actual)) ;
	}

}


#endif // #ifdef _DEBUG
