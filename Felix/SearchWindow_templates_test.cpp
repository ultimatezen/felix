#include "StdAfx.h"
#include "SearchWindow.h"
#include "text_templates.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_replace_match )


	using namespace text_tmpl ;
	// replace_match
	BOOST_AUTO_TEST_CASE( simple)
	{
		CTextTemplate text_tmpl ;

		// message
		text_tmpl.Assign(L"message", L"") ;

		// page stuff
		text_tmpl.Assign(L"match_num", L"1") ;
		text_tmpl.Assign(L"num_matches", L"100") ;

		text_tmpl::DictPtr found = text_tmpl.CreateDict() ;
		text_tmpl::DictPtr result = text_tmpl.CreateDict() ;

		// found
		text_tmpl.Assign(found, L"source", L"source") ;
		text_tmpl.Assign(found, L"trans", L"get_trans_rich") ;
		text_tmpl.Assign(found, L"context", L"get_context_rich") ;
		text_tmpl.Assign(found, L"created", L"get_created().get_date_time_string") ;
		text_tmpl.Assign(found, L"date_created", L"get_created().get_date_time_string") ;
		text_tmpl.Assign(found, L"modified", L"get_modified().get_date_time_string") ;
		text_tmpl.Assign(found, L"last_modified", L"get_modified().get_date_time_string") ;
		text_tmpl.Assign(found, L"reliability", L"get_reliability") ;
		text_tmpl.Assign(found, L"validated", L"is_validated") ;

		text_tmpl.Assign(found, L"creator", L"get_creator") ;
		text_tmpl.Assign(found, L"created_by", L"get_creator") ;
		text_tmpl.Assign(found, L"modified_by", L"get_modified_by") ;
		text_tmpl.Assign(found, L"refcount", L"get_refcount") ;
		text_tmpl.Assign(found, L"ref_count", L"get_refcount") ;

		// result
		text_tmpl.Assign(result, L"source", L"source") ;
		text_tmpl.Assign(result, L"trans", L"get_trans_rich") ;
		text_tmpl.Assign(result, L"context", L"get_context_rich") ;
		text_tmpl.Assign(result, L"created", L"get_created().get_date_time_string") ;
		text_tmpl.Assign(result, L"date_created", L"get_created().get_date_time_string") ;
		text_tmpl.Assign(result, L"modified", L"get_modified().get_date_time_string") ;
		text_tmpl.Assign(result, L"last_modified", L"get_modified().get_date_time_string") ;
		text_tmpl.Assign(result, L"reliability", L"get_reliability") ;
		text_tmpl.Assign(result, L"validated", L"is_validated") ;

		text_tmpl.Assign(result, L"creator", L"get_creator") ;
		text_tmpl.Assign(result, L"created_by", L"get_creator") ;
		text_tmpl.Assign(result, L"modified_by", L"get_modified_by") ;
		text_tmpl.Assign(result, L"refcount", L"get_refcount") ;
		text_tmpl.Assign(result, L"ref_count", L"get_refcount") ;


		text_tmpl.Assign(L"found", found) ;
		text_tmpl.Assign(L"result", result) ;

		wstring text = text_tmpl.Fetch(get_template_text(_T("replace_match.txt"))) ;

		BOOST_CHECK(text.find(L"<h3>Match 1 of 100</h3>") != wstring::npos) ;
		BOOST_CHECK(text.find(L"<th>Now</th>") != wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( not_found)
	{
		CTextTemplate text_tmpl ;

		// message
		text_tmpl.Assign(L"message", L"") ;

		// page stuff
		text_tmpl.Assign(L"found", L"") ;

		wstring text = text_tmpl.Fetch(get_template_text(_T("replace_match.txt"))) ;

		BOOST_CHECK(text.find(L"<h3>Match 1 of 100</h3>") == wstring::npos) ;
		BOOST_CHECK(text.find(L"<th>Now</th>") == wstring::npos) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_replacelinks )

	using namespace text_tmpl ;

	BOOST_AUTO_TEST_CASE( found)
	{
		CTextTemplate text_tmpl ;

		// message
		text_tmpl.Assign(L"found", L"foo") ;

		wstring text = text_tmpl.Fetch(get_template_text(_T("replacelinks.txt"))) ;

		BOOST_CHECK(text.find(L"<a href=\"replace_replace\" id=\"replace-replace\" class=\"ui-state-default ui-corner-all\">&nbsp;Replace&nbsp;</a>") != wstring::npos) ;
	}

	BOOST_AUTO_TEST_CASE( not_found)
	{
		CTextTemplate text_tmpl ;

		// message
		text_tmpl.Assign(L"found", L"") ;

		wstring text = text_tmpl.Fetch(get_template_text(_T("replacelinks.txt"))) ;

		BOOST_CHECK(text.find(L"<a href=\"replace_replace\" id=\"replace-replace\" class=\"ui-state-default ui-corner-all\">&nbsp;Replace&nbsp;</a>") == wstring::npos) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif