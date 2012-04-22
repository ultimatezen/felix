#include "stdafx.h"
#include "QCFormParser.h"
#include "document_wrapper_fake.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( QCFormParserTests )

	BOOST_AUTO_TEST_CASE(test_numbers)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"numbers")->set_attribute(L"checked", L"true") ;

		mgrview::QCFormParser parser(doc) ;

		BOOST_CHECK(parser.check_numbers());
	}
	BOOST_AUTO_TEST_CASE(test_all_caps_true)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"numbers")->set_attribute(L"allcaps", L"true") ;

		mgrview::QCFormParser parser(doc) ;

		BOOST_CHECK(parser.check_all_caps());
	}
	BOOST_AUTO_TEST_CASE(test_all_caps_false)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"numbers")->set_attribute(L"allcaps", L"false") ;

		mgrview::QCFormParser parser(doc) ;

		BOOST_CHECK(parser.check_all_caps());
	}
	BOOST_AUTO_TEST_CASE(test_check_gloss)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"gloss")->set_attribute(L"checked", L"true") ;

		mgrview::QCFormParser parser(doc) ;

		BOOST_CHECK(parser.check_gloss());
	}
	BOOST_AUTO_TEST_CASE(test_live_check)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"live")->set_attribute(L"checked", L"true") ;

		mgrview::QCFormParser parser(doc) ;

		BOOST_CHECK(parser.live_check());
	}
	BOOST_AUTO_TEST_CASE(test_checked_glosses)
	{
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		doc3_wrapper_ptr doc(wrapper) ;

		wrapper->get_element_by_id(L"1")->set_attribute(L"checked", L"true") ;
		wrapper->get_element_by_id(L"2")->set_attribute(L"checked", L"false") ;
		wrapper->get_element_by_id(L"3")->set_attribute(L"checked", L"true") ;

		mgrview::QCFormParser parser(doc) ;

		std::vector<size_t> memids ;
		memids.push_back(1u);
		memids.push_back(2u);
		memids.push_back(3u);
		std::vector<size_t> existids ;
		parser.checked_glosses(memids, existids) ;
		BOOST_CHECK_EQUAL(existids.size(), 2u);
		BOOST_CHECK_EQUAL(existids[0], 1u);
		BOOST_CHECK_EQUAL(existids[1], 3u);
	}


BOOST_AUTO_TEST_SUITE_END()

#endif 
