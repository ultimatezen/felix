#include "StdAfx.h"
#include "SearchWindow.h"
#include "text_templates.h"
#include "cpptempl.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_replace_match )

	using namespace cpptempl ;
	// replace_match
	BOOST_AUTO_TEST_CASE( simple)
	{
		cpptempl::data_map data;

		// message
		data[L"message"] = cpptempl::make_data(L"") ;

		// page stuff
		data[L"match_num"] = cpptempl::make_data(L"1") ;
		data[L"num_matches"] = cpptempl::make_data(L"100") ;

		cpptempl::data_map found ;
		cpptempl::data_map result ;

		// found
		found[L"source"] = cpptempl::make_data(L"source") ;
		found[L"trans"] = cpptempl::make_data(L"get_trans_rich") ;
		found[L"context"] = cpptempl::make_data(L"get_context_rich") ;
		found[L"created"] = cpptempl::make_data(L"get_created().get_date_time_string") ;
		found[L"date_created"] = cpptempl::make_data(L"get_created().get_date_time_string") ;
		found[L"modified"] = cpptempl::make_data(L"get_modified().get_date_time_string") ;
		found[L"last_modified"] = cpptempl::make_data(L"get_modified().get_date_time_string") ;
		found[L"reliability"] = cpptempl::make_data(L"get_reliability") ;
		found[L"validated"] = cpptempl::make_data(L"is_validated") ;

		found[L"creator"] = cpptempl::make_data(L"get_creator") ;
		found[L"created_by"] = cpptempl::make_data(L"get_creator") ;
		found[L"modified_by"] = cpptempl::make_data(L"get_modified_by") ;
		found[L"refcount"] = cpptempl::make_data(L"get_refcount") ;
		found[L"ref_count"] = cpptempl::make_data(L"get_refcount") ;

		// result
		result[L"source"] = cpptempl::make_data(L"source") ;
		result[L"trans"] = cpptempl::make_data(L"get_trans_rich") ;
		result[L"context"] = cpptempl::make_data(L"get_context_rich") ;
		result[L"created"] = cpptempl::make_data(L"get_created().get_date_time_string") ;
		result[L"date_created"] = cpptempl::make_data(L"get_created().get_date_time_string") ;
		result[L"modified"] = cpptempl::make_data(L"get_modified().get_date_time_string") ;
		result[L"last_modified"] = cpptempl::make_data(L"get_modified().get_date_time_string") ;
		result[L"reliability"] = cpptempl::make_data(L"get_reliability") ;
		result[L"validated"] = cpptempl::make_data(L"is_validated") ;

		result[L"creator"] = cpptempl::make_data(L"get_creator") ;
		result[L"created_by"] = cpptempl::make_data(L"get_creator") ;
		result[L"modified_by"] = cpptempl::make_data(L"get_modified_by") ;
		result[L"refcount"] = cpptempl::make_data(L"get_refcount") ;
		result[L"ref_count"] = cpptempl::make_data(L"get_refcount") ;


		data[L"found"] = cpptempl::make_data(found) ;
		data[L"result"] = cpptempl::make_data(result) ;

		wstring text = cpptempl::parse(get_template_text(_T("replace_match.txt")), data) ;

		BOOST_CHECK(text.find(L"<h3>Match 1 of 100</h3>") != wstring::npos) ;
		BOOST_CHECK(text.find(L"<th>Now</th>") != wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( not_found)
	{
		cpptempl::data_map data ;

		// message
		data[L"message"] = cpptempl::make_data(L"") ;

		// page stuff
		data[L"found"] = cpptempl::make_data(L"") ;

		wstring text = cpptempl::parse(get_template_text(_T("replace_match.txt")), data) ;

		BOOST_CHECK(text.find(L"<h3>Match 1 of 100</h3>") == wstring::npos) ;
		BOOST_CHECK(text.find(L"<th>Now</th>") == wstring::npos) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_replacelinks )

	using namespace cpptempl ;

	BOOST_AUTO_TEST_CASE( found)
	{
		cpptempl::data_map data ;

		// message
		data[L"found"] = cpptempl::make_data(L"foo") ;

		wstring text = cpptempl::parse(get_template_text(_T("replacelinks.txt")), data) ;

		BOOST_CHECK(text.find(L"<a href=\"replace_replace\" id=\"replace-replace\" class=\"ui-state-default ui-corner-all\">&nbsp;Replace&nbsp;</a>") != wstring::npos) ;
	}

	BOOST_AUTO_TEST_CASE( not_found)
	{
		cpptempl::data_map data ;

		// message
		data[L"found"] = cpptempl::make_data(L"") ;

		wstring text = cpptempl::parse(get_template_text(_T("replacelinks.txt")), data) ;

		BOOST_CHECK(text.find(L"<a href=\"replace_replace\" id=\"replace-replace\" class=\"ui-state-default ui-corner-all\">&nbsp;Replace&nbsp;</a>") == wstring::npos) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif