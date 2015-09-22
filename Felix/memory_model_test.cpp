/*!
@brief Unit tests for memory class
@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "memory_model.h"
#include "record_local.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( test_memory_model_gloss )
	using namespace mem_engine ;
	using namespace except ;
	using namespace app_props ;

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

	BOOST_AUTO_TEST_CASE( create_memory )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem = mm.create_memory() ;
		BOOST_CHECK(mem) ;
	}
	// getting memories
	BOOST_AUTO_TEST_CASE( get_memory_by_name )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->set_location("first memory") ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->set_location("second memory") ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		memory_pointer retrieved1 = mm.get_memory_by_name("first memory") ;
		CStringA actual = retrieved1->get_location() ;
		BOOST_CHECK_EQUAL("first memory", string(actual)) ;
		BOOST_CHECK_EQUAL(retrieved1->get_id(), mem1->get_id()) ;

		memory_pointer retrieved2 = mm.get_memory_by_name("second memory") ;
		actual = retrieved2->get_location() ;
		BOOST_CHECK_EQUAL("second memory", string(actual)) ;
		BOOST_CHECK_EQUAL(retrieved2->get_id(), mem2->get_id()) ;
	}
	BOOST_AUTO_TEST_CASE( get_memory_by_id )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		size_t first_id = mem1->get_id() ;
		memory_pointer mem2 = mm.create_memory() ;
		size_t second_id = mem2->get_id() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		BOOST_CHECK(first_id != second_id) ;
		memory_pointer retrieved1 = mm.get_memory_by_id(first_id) ;
		BOOST_CHECK_EQUAL(first_id, retrieved1->get_id()) ;
		memory_pointer retrieved2 = mm.get_memory_by_id(second_id) ;
		BOOST_CHECK_EQUAL(second_id, retrieved2->get_id()) ;
	}
	BOOST_AUTO_TEST_CASE( get_memory_by_id_not_found )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		size_t first_id = mem1->get_id() ;
		memory_pointer mem2 = mm.create_memory() ;
		size_t second_id = mem2->get_id() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		size_t third_id = first_id + second_id ;
		BOOST_CHECK(third_id != first_id) ;
		BOOST_CHECK(third_id != second_id) ;
		try
		{
			mm.get_memory_by_id(third_id) ;
			BOOST_FAIL("Should have thrown on nonexistent memory") ;
		}
		catch (CProgramException &e)
		{
			CStringA expected ;
			expected.Format("Memory not found with id %u", third_id) ;
			CStringA actual(e.what()) ;
			BOOST_CHECK_EQUAL(string(expected), string(actual)) ;
			BOOST_CHECK_EQUAL(2u, mm.size()) ;
		}
	}
	// removing memories
	BOOST_AUTO_TEST_CASE( remove_memory_by_name )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->set_location("first memory") ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->set_location("second memory") ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		mm.remove_memory_by_name("second memory") ;
		memory_pointer retrieved = mm.get_first_memory() ;
		CStringA actual = retrieved->get_location() ;
		BOOST_CHECK_EQUAL("first memory", string(actual)) ;
		BOOST_CHECK_EQUAL(1u, mm.size()) ;
	}
	BOOST_AUTO_TEST_CASE( reduce_size_to )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("spam 1", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("spam 1", "egg 1")) ;
		mem2->add_record(make_rec("bogus record source", "bogus record trans")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		mm.reduce_size_to(2) ;
		BOOST_CHECK_EQUAL(mem2->get_id(), mm.get_first_memory()->get_id()) ;
		BOOST_CHECK_EQUAL(1u, mm.size()) ;
	}

	// adding and removing records
	BOOST_AUTO_TEST_CASE( add_record_to_first )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		mm.add_record(make_rec("spam", "egg"), mem1->get_id()) ;
		BOOST_CHECK_EQUAL(1u, mem1->size()) ;
		BOOST_CHECK_EQUAL(0u, mem2->size()) ;
	}
	BOOST_AUTO_TEST_CASE( add_record_to_second )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		mm.add_record(make_rec("spam", "egg"), mem2->get_id()) ;
		BOOST_CHECK_EQUAL(1u, mem2->size()) ;
		BOOST_CHECK_EQUAL(0u, mem1->size()) ;
	}
	BOOST_AUTO_TEST_CASE( remove_record )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem = mm.create_memory() ;
		mm.insert_memory(mem) ;

		record_pointer rec = make_rec("spam", "egg") ;
		mm.add_record(rec, mem->get_id()) ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
		mm.remove_record(rec); 
		BOOST_CHECK_EQUAL(0u, mem->size()) ;
	}
	BOOST_AUTO_TEST_CASE( remove_record_by_id )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		record_pointer rec = make_rec("spam", "egg") ;
		mm.add_record(rec, mem1->get_id()) ;
		mm.add_record(rec, mem2->get_id()) ;

		mm.remove_record(rec, mem1->get_id()) ;
		BOOST_CHECK_EQUAL(1u, mem2->size()) ;
		BOOST_CHECK_EQUAL(0u, mem1->size()) ;
	}
	// setting properties
	BOOST_AUTO_TEST_CASE( set_properties_algo )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		BOOST_CHECK_EQUAL(IDC_ALGO_AUTO, mem1->m_algo_properties->m_data.m_match_algo) ;
		BOOST_CHECK_EQUAL(IDC_ALGO_AUTO, mem2->m_algo_properties->m_data.m_match_algo) ;

		get_props()->m_alg_props.m_data.m_match_algo = IDC_ALGO_WORD ;

		BOOST_CHECK_EQUAL(IDC_ALGO_WORD, mem1->m_algo_properties->m_data.m_match_algo) ;
		BOOST_CHECK_EQUAL(IDC_ALGO_WORD, mem2->m_algo_properties->m_data.m_match_algo) ;
	}
	BOOST_AUTO_TEST_CASE( set_properties_gloss )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		get_props()->m_gloss_props.m_data.m_min_score = 50 ;

		BOOST_CHECK_EQUAL(50u, mem1->m_gloss_properties->m_data.m_min_score) ;
		BOOST_CHECK_EQUAL(50u, mem2->m_gloss_properties->m_data.m_min_score) ;
	}
	BOOST_AUTO_TEST_CASE( set_properties_memory )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		get_props()->m_mem_props.m_data.m_min_score = 0 ;

		BOOST_CHECK_EQUAL(0u, mem1->m_mem_properties->m_data.m_min_score) ;
		BOOST_CHECK_EQUAL(0u, mem2->m_mem_properties->m_data.m_min_score) ;

		get_props()->m_mem_props.m_data.m_min_score = 50 ;

		BOOST_CHECK_EQUAL(50u, mem1->m_mem_properties->m_data.m_min_score) ;
		BOOST_CHECK_EQUAL(50u, mem2->m_mem_properties->m_data.m_min_score) ;
	}
	// finding matches
	BOOST_AUTO_TEST_CASE( find_trans_matches )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("spam 1", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("spam 2", "egg 2")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		trans_match_container matches;
		search_query_params params ;
		params.m_rich_source = L"egg 3" ;
		mm.find_trans_matches(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( find_matches )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("spam 1", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("spam 2", "egg 2")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		trans_match_container matches;
		search_query_params params ;
		params.m_rich_source = L"spam 3" ;
		mm.find_matches(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( get_glossary_matches )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("Spam", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		mem1->m_gloss_properties->set_min_score(100) ;
		BOOST_CHECK(mem1->is_active()) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("eggy", "egg 2")) ;
		mem2->m_gloss_properties->set_min_score(100) ;
		BOOST_CHECK(mem2->is_active()) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		search_match_container matches;
		search_query_params params ;
		params.m_ignore_case = true ;
		params.m_source = L"Spam and eggy weggies" ;
		mm.get_glossary_matches(matches, params) ;
		size_t num_matches = matches.size() ;
		BOOST_CHECK_EQUAL(2u, num_matches) ;
	}
	BOOST_AUTO_TEST_CASE( perform_search )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("lovely spam", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("wonderful spam", "egg 2")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		search_match_container matches;
		search_query_params params ;
		params.m_source = L"spam" ;
		mm.perform_search(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}

	// memory info
	BOOST_AUTO_TEST_CASE( total_memory_size_0 )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		BOOST_CHECK_EQUAL(0u, mm.total_memory_size()) ;
	}
	BOOST_AUTO_TEST_CASE( total_memory_size_3 )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->add_record(make_rec("spam 1", "egg 1")) ;
		mem1->add_record(make_rec("bogus record source", "bogus record trans")) ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->add_record(make_rec("spam 2", "egg 2")) ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		BOOST_CHECK_EQUAL(3u, mm.total_memory_size()) ;
	}
	BOOST_AUTO_TEST_CASE( empty_true )
	{
		memory_model_gloss mm(get_props()) ;

		BOOST_CHECK(mm.empty()) ;
	}
	BOOST_AUTO_TEST_CASE( empty_false )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		BOOST_CHECK(! mm.empty()) ;
	}
	BOOST_AUTO_TEST_CASE( size_0 )
	{
		memory_model_gloss mm(get_props()) ;

		BOOST_CHECK_EQUAL(0u, mm.size()) ;
	}
	BOOST_AUTO_TEST_CASE( size_2 )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		memory_pointer mem2 = mm.create_memory() ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;

		BOOST_CHECK_EQUAL(2u, mm.size()) ;
	}
	BOOST_AUTO_TEST_CASE( get_memories_needing_saving )
	{
		memory_model_gloss mm(get_props()) ;
		memory_pointer mem1 = mm.create_memory() ;
		mem1->set_location("first memory") ;
		memory_pointer mem2 = mm.create_memory() ;
		mem2->set_location("second memory") ;
		mm.insert_memory(mem1) ;
		mm.insert_memory(mem2) ;
		mm.add_record(make_rec("spam", "eggs"), mem1->get_id()) ;

		memory_list mems ;
		mm.get_memories_needing_saving(mems) ;
		BOOST_CHECK_EQUAL(1u, mems.size()) ;
		memory_pointer retrieved = *(mems.begin()) ;
		CStringA actual = retrieved->get_location() ;
		BOOST_CHECK_EQUAL("first memory", string(actual)) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
