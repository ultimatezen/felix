#include "StdAfx.h"
#include "text_templates.h"
#include "ManagerViewQCSettings.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "felix_model_fake.h"
#include "manager_window_listener_fake.h"
#include "view_interface_fake.h"

using namespace mem_engine ;
using namespace mgrview ;
using namespace cpptempl ;

// This sets up and initializes the view class
struct qcsettings_view_setup
{
	ManagerViewQCSettings view; 
	model_iface_ptr model ;
	ManagerWindowListenerFake listener ;
	view_interface_fake interface_view ;

	qcsettings_view_setup(app_props::props_ptr props) : 
	view(props),
		model(new FelixModelInterfaceFake)
	{
		listener.m_message = L"message from setup" ;

		view.set_view(&interface_view) ;
		view.set_mem_model(model) ;
		view.set_gloss_model(model) ;
		view.set_listener(&listener) ;
	}
};

BOOST_AUTO_TEST_SUITE(test_text_template_locations)

	BOOST_AUTO_TEST_CASE( start_file )
	{
		CStringA filename = get_template_filename(_T("start.html")) ;
		BOOST_CHECK_EQUAL("D:\\Users\\Ryan\\AppData\\Local\\Felix\\html\\en\\start.html", filename) ;
	}
	BOOST_AUTO_TEST_CASE( glossary_start )
	{
		CStringA filename = get_template_filename(_T("start_gloss.html")) ;
		BOOST_CHECK_EQUAL("D:\\Users\\Ryan\\AppData\\Local\\Felix\\html\\en\\start_gloss.html", filename) ;
	}
	BOOST_AUTO_TEST_CASE( get_template_text_match_none )
	{
		wstring text = get_template_text(_T("match_none.txt")) ;
		BOOST_CHECK(boost::contains(text, L"{$query}")) ;
	}
	BOOST_AUTO_TEST_CASE( test_calculate_template_filename )
	{
		string filename = (LPCSTR)CStringA(calculate_template_filename("foo.txt")) ;
		string actual = boost::to_lower_copy(filename) ;
		string expected = "d:\\users\\ryan\\appdata\\local\\felix\\html\\en\\foo.txt" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( test_calculate_module_template_filename )
	{
		string filename = (LPCSTR)CStringA(calculate_module_template_filename("foo.txt")) ;
		string actual = boost::to_lower_copy(filename) ;
		string expected = "d:\\dev\\cpp\\assistant suite\\felix\\testing\\html\\en\\foo.txt" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(test_text_templates_english)

	BOOST_AUTO_TEST_CASE(qc_settings_no_glosses)
	{
		app_props::props_ptr props(new app_props::properties) ;
		qcsettings_view_setup setup(props) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data, &props->m_qc_props.m_data) ;

		const CString full_path = "D:\\dev\\cpp\\Assistant Suite\\Felix\\html\\en\\manager\\qc_settings.txt" ;
		file::view file_view ;
		string raw_text(static_cast<LPCSTR>(file_view.create_view_readonly(full_path))) ;
		wstring tpl_text = string2wstring(raw_text, CP_UTF8) ;

		wstring full_text = cpptempl::parse(tpl_text, data) ;

		wstring numbers = L"<input type=\"checkbox\" name=\"numbers\" id=\"numbers\" value=\"numbers\" />" ;
		wstring allcaps = L"<input type=\"checkbox\" name=\"allcaps\" id=\"allcaps\" value=\"allcaps\" />" ;
		wstring gloss_placer = L"<input type=\"checkbox\" name=\"gloss\" id=\"gloss\" value=\"gloss\" />" ;
		wstring live = L"<input type=\"checkbox\" name=\"live\" id=\"live\" value=\"live\" />" ;

		BOOST_CHECK(full_text.find(numbers) != wstring::npos) ;
		BOOST_CHECK(full_text.find(allcaps) != wstring::npos) ;
		BOOST_CHECK(full_text.find(gloss_placer) != wstring::npos) ;
		BOOST_CHECK(full_text.find(live) != wstring::npos) ;
		BOOST_CHECK_EQUAL(full_text.find(L"Glossaries to use in check"), wstring::npos) ;
	}

	BOOST_AUTO_TEST_CASE(qc_settings_glosses)
	{
		app_props::props_ptr props(new app_props::properties) ;

		app_props::properties_qc *qcprops = &props->m_qc_props ;

		qcprops->m_data.m_check_numbers = TRUE ;
		qcprops->m_data.m_check_all_caps = TRUE ;
		qcprops->m_data.m_check_gloss = TRUE ;
		qcprops->m_data.m_live_checking = TRUE ;
		qcprops->m_qc_glosses.push_back(L"c:\\first\\location") ;

		qcsettings_view_setup setup(props) ;

		memory_pointer mem1 = setup.model->add_memory() ;
		memory_pointer mem2 = setup.model->add_memory() ;

		mem1->set_location("c:\\first\\location") ;
		size_t id1 = mem1->get_id() ;
		mem2->set_location("c:\\second\\location") ;
		size_t id2 = mem2->get_id() ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data, &props->m_qc_props.m_data) ;

		const CString full_path = "D:\\dev\\cpp\\Assistant Suite\\Felix\\html\\en\\manager\\qc_settings.txt" ;
		file::view file_view ;
		string raw_text(static_cast<LPCSTR>(file_view.create_view_readonly(full_path))) ;
		wstring tpl_text = string2wstring(raw_text, CP_UTF8) ;

		wstring full_text = cpptempl::parse(tpl_text, data) ;

		wstring numbers = L"<input type=\"checkbox\" name=\"numbers\" id=\"numbers\" value=\"numbers\" checked=\"checked\" />" ;
		wstring allcaps = L"<input type=\"checkbox\" name=\"allcaps\" id=\"allcaps\" value=\"allcaps\" checked=\"checked\" />" ;
		wstring gloss_placer = L"<input type=\"checkbox\" name=\"gloss\" id=\"gloss\" value=\"gloss\" checked=\"checked\" />" ;
		wstring live = L"<input type=\"checkbox\" name=\"live\" id=\"live\" value=\"live\" checked=\"checked\" />" ;

		wstring gloss1 = (wformat( L"<input type=\"checkbox\" name=\"%d\" id=\"%d\" value=\"%d\" />" ) % id2 % id2 % id2).str() ;
		wstring gloss2 =(wformat(  L"<input type=\"checkbox\" name=\"%d\" id=\"%d\" value=\"%d\" checked=\"checked\" />" ) % id1 % id1 % id1).str() ;

		BOOST_CHECK(full_text.find(numbers) != wstring::npos) ;
		BOOST_CHECK(full_text.find(allcaps) != wstring::npos) ;
		BOOST_CHECK(full_text.find(gloss_placer) != wstring::npos) ;
		BOOST_CHECK(full_text.find(live) != wstring::npos) ;
		BOOST_CHECK(full_text.find(gloss1) != wstring::npos) ;
		BOOST_CHECK(full_text.find(gloss2) != wstring::npos) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_text_templates_japanese)

	BOOST_AUTO_TEST_CASE(qc_settings_no_glosses)
	{
		app_props::props_ptr props(new app_props::properties) ;
		qcsettings_view_setup setup(props) ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data, &props->m_qc_props.m_data) ;

		const CString full_path = "D:\\dev\\cpp\\Assistant Suite\\Felix\\html\\jp\\manager\\qc_settings.txt" ;
		file::view file_view ;
		string raw_text(static_cast<LPCSTR>(file_view.create_view_readonly(full_path))) ;
		wstring tpl_text = string2wstring(raw_text, CP_UTF8) ;

		wstring full_text = cpptempl::parse(tpl_text, data) ;

		wstring numbers = L"<input type=\"checkbox\" name=\"numbers\" id=\"numbers\" value=\"numbers\" />" ;
		wstring allcaps = L"<input type=\"checkbox\" name=\"allcaps\" id=\"allcaps\" value=\"allcaps\" />" ;
		wstring gloss = L"<input type=\"checkbox\" name=\"gloss\" id=\"gloss\" value=\"gloss\" />" ;
		wstring live = L"<input type=\"checkbox\" name=\"live\" id=\"live\" value=\"live\" />" ;

		BOOST_CHECK(full_text.find(numbers) != wstring::npos) ;
		BOOST_CHECK(full_text.find(allcaps) != wstring::npos) ;
		BOOST_CHECK(full_text.find(gloss) != wstring::npos) ;
		BOOST_CHECK(full_text.find(live) != wstring::npos) ;
		BOOST_CHECK_EQUAL(full_text.find(L"チェックに利用する用語集"), wstring::npos) ;
	}

	BOOST_AUTO_TEST_CASE(qc_settings_glosses)
	{
		app_props::props_ptr props(new app_props::properties) ;

		app_props::properties_qc *qcprops = &props->m_qc_props ;

		qcprops->m_data.m_check_numbers = TRUE ;
		qcprops->m_data.m_check_all_caps = TRUE ;
		qcprops->m_data.m_check_gloss = TRUE ;
		qcprops->m_data.m_live_checking = TRUE ;
		qcprops->m_qc_glosses.push_back(L"c:\\first\\location") ;

		qcsettings_view_setup setup(props) ;

		memory_pointer mem1 = setup.model->add_memory() ;
		memory_pointer mem2 = setup.model->add_memory() ;

		mem1->set_location("c:\\first\\location") ;
		size_t id1 = mem1->get_id() ;
		mem2->set_location("c:\\second\\location") ;
		size_t id2 = mem2->get_id() ;

		cpptempl::data_map data ;
		setup.view.set_template_data(data, &props->m_qc_props.m_data) ;

		const CString full_path = "D:\\dev\\cpp\\Assistant Suite\\Felix\\html\\jp\\manager\\qc_settings.txt" ;
		file::view file_view ;
		string raw_text(static_cast<LPCSTR>(file_view.create_view_readonly(full_path))) ;
		wstring tpl_text = string2wstring(raw_text, CP_UTF8) ;

		wstring full_text = cpptempl::parse(tpl_text, data) ;

		wstring numbers = L"<input type=\"checkbox\" name=\"numbers\" id=\"numbers\" value=\"numbers\" checked=\"checked\" />" ;
		wstring allcaps = L"<input type=\"checkbox\" name=\"allcaps\" id=\"allcaps\" value=\"allcaps\" checked=\"checked\" />" ;
		wstring gloss = L"<input type=\"checkbox\" name=\"gloss\" id=\"gloss\" value=\"gloss\" checked=\"checked\" />" ;
		wstring live = L"<input type=\"checkbox\" name=\"live\" id=\"live\" value=\"live\" checked=\"checked\" />" ;

		wstring gloss1 = (wformat( L"<input type=\"checkbox\" name=\"%d\" id=\"%d\" value=\"%d\" />" ) % id2 % id2 % id2).str() ;
		wstring gloss2 =(wformat(  L"<input type=\"checkbox\" name=\"%d\" id=\"%d\" value=\"%d\" checked=\"checked\" />" ) % id1 % id1 % id1).str() ;

		BOOST_CHECK(full_text.find(numbers) != wstring::npos) ;
		BOOST_CHECK(full_text.find(allcaps) != wstring::npos) ;
		BOOST_CHECK(full_text.find(gloss) != wstring::npos) ;
		BOOST_CHECK(full_text.find(live) != wstring::npos) ;
		BOOST_CHECK(full_text.find(gloss1) != wstring::npos) ;
		BOOST_CHECK(full_text.find(gloss2) != wstring::npos) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif