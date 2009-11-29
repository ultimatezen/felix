#include "StdAfx.h"
#include "SearchWindow.h"
#include "text_templates.h"
#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{

	// replace_match
	TEST(test_replace_match, simple)
	{
		CTextTemplate text_template ;

		// message
		text_template.Assign(L"message", L"") ;

		// page stuff
		text_template.Assign(L"match_num", L"1") ;
		text_template.Assign(L"num_matches", L"100") ;

		CTextTemplate::DictPtr found = text_template.CreateDict() ;
		CTextTemplate::DictPtr result = text_template.CreateDict() ;

		// found
		text_template.Assign(found, L"source", L"source") ;
		text_template.Assign(found, L"trans", L"get_trans_rich") ;
		text_template.Assign(found, L"context", L"get_context_rich") ;
		text_template.Assign(found, L"created", L"get_created().get_date_time_string") ;
		text_template.Assign(found, L"date_created", L"get_created().get_date_time_string") ;
		text_template.Assign(found, L"modified", L"get_modified().get_date_time_string") ;
		text_template.Assign(found, L"last_modified", L"get_modified().get_date_time_string") ;
		text_template.Assign(found, L"reliability", L"get_reliability") ;
		text_template.Assign(found, L"validated", L"is_validated") ;

		text_template.Assign(found, L"creator", L"get_creator") ;
		text_template.Assign(found, L"created_by", L"get_creator") ;
		text_template.Assign(found, L"modified_by", L"get_modified_by") ;
		text_template.Assign(found, L"refcount", L"get_refcount") ;
		text_template.Assign(found, L"ref_count", L"get_refcount") ;

		// result
		text_template.Assign(result, L"source", L"source") ;
		text_template.Assign(result, L"trans", L"get_trans_rich") ;
		text_template.Assign(result, L"context", L"get_context_rich") ;
		text_template.Assign(result, L"created", L"get_created().get_date_time_string") ;
		text_template.Assign(result, L"date_created", L"get_created().get_date_time_string") ;
		text_template.Assign(result, L"modified", L"get_modified().get_date_time_string") ;
		text_template.Assign(result, L"last_modified", L"get_modified().get_date_time_string") ;
		text_template.Assign(result, L"reliability", L"get_reliability") ;
		text_template.Assign(result, L"validated", L"is_validated") ;

		text_template.Assign(result, L"creator", L"get_creator") ;
		text_template.Assign(result, L"created_by", L"get_creator") ;
		text_template.Assign(result, L"modified_by", L"get_modified_by") ;
		text_template.Assign(result, L"refcount", L"get_refcount") ;
		text_template.Assign(result, L"ref_count", L"get_refcount") ;


		text_template.Assign(L"found", found) ;
		text_template.Assign(L"result", result) ;

		wstring text = text_template.Fetch(get_template_text(_T("replace_match.txt"))) ;

		ASSERT_TRUE(text.find(L"<h3>Match 1 of 100</h3>") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"<th>Now</th>") != wstring::npos) ;
	}
	TEST(test_replace_match, not_found)
	{
		CTextTemplate text_template ;

		// message
		text_template.Assign(L"message", L"") ;

		// page stuff
		text_template.Assign(L"found", L"") ;

		wstring text = text_template.Fetch(get_template_text(_T("replace_match.txt"))) ;

		ASSERT_TRUE(text.find(L"<h3>Match 1 of 100</h3>") == wstring::npos) ;
		ASSERT_TRUE(text.find(L"<th>Now</th>") == wstring::npos) ;
	}

	TEST(test_replacelinks, found)
	{
		CTextTemplate text_template ;

		// message
		text_template.Assign(L"found", L"foo") ;

		wstring text = text_template.Fetch(get_template_text(_T("replacelinks.txt"))) ;

		ASSERT_TRUE(text.find(L"<a href=\"replace_replace\" id=\"replace-replace\" class=\"ui-state-default ui-corner-all\">&nbsp;Replace&nbsp;</a>") != wstring::npos) ;
	}

	TEST(test_replacelinks, not_found)
	{
		CTextTemplate text_template ;

		// message
		text_template.Assign(L"found", L"") ;

		wstring text = text_template.Fetch(get_template_text(_T("replacelinks.txt"))) ;

		ASSERT_TRUE(text.find(L"<a href=\"replace_replace\" id=\"replace-replace\" class=\"ui-state-default ui-corner-all\">&nbsp;Replace&nbsp;</a>") == wstring::npos) ;
	}

}

#endif