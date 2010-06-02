/*!
@brief Unit tests for CTMXWriter class
@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include ".\tmxwriter.h"
#include "ResHtmlFile.h"
#include "record_local.h"
#include "output_device_fake.h"
#include "MockListener.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CTMXWriterTestCase )

	// write
	BOOST_AUTO_TEST_CASE( write_footer )
	{
		CMockListener dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		tmx_writer.m_file = boost::shared_ptr<OutputDevice>(device) ;

		tmx_writer.write_footer() ;
		string actual = string2string(device->m_value).c_str() ;
		string expected = "  </body>\n</tmx>" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(1u, device->m_calls.size()) ;
		BOOST_CHECK_EQUAL(string("write_wstring"), device->m_calls[0].c_str()) ;
	}
	BOOST_AUTO_TEST_CASE( write_tu)
	{
		CMockListener dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		tmx_writer.m_file = boost::shared_ptr<OutputDevice>(device) ;

		tmx_writer.tmplText = L"{$srcseg} {$transseg}" ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		tmx_writer.write_tu(rec) ;
		string actual = string2string(device->m_value).c_str() ;
		string expected = "spam egg" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(1u, device->m_calls.size()) ;
		BOOST_CHECK_EQUAL(string("write_wstring"), device->m_calls[0].c_str()) ;
	}
	BOOST_AUTO_TEST_CASE( write_header)
	{
		CMockListener dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		tmx_writer.m_file = boost::shared_ptr<OutputDevice>(device) ;

		tmx_writer.m_src_lang = L"Pekinese" ;
		
		tmx_writer.write_header() ;
		const wstring text = device->m_value ;

		BOOST_CHECK(text.find(L"<!DOCTYPE tmx SYSTEM \"tmx14.dtd\">") != wstring::npos) ;
		BOOST_CHECK(text.find(L"<tmx version=\"1.4\">") != wstring::npos) ;
		BOOST_CHECK(text.find(L"<header") != wstring::npos) ;
		BOOST_CHECK(text.find(L"creationtool=\"Felix\"") != wstring::npos) ;
		BOOST_CHECK(text.find(L"srclang=\"Pekinese\"") != wstring::npos) ;
		BOOST_CHECK(text.find(L"o-tmf=\"Felix\"") != wstring::npos) ;
		BOOST_CHECK_EQUAL(1u, device->m_calls.size()) ;
		BOOST_CHECK_EQUAL(string("write_wstring"), device->m_calls[0].c_str()) ;
	}

	// make_tu
	BOOST_AUTO_TEST_CASE( make_tu_simple )
	{
		CMockListener dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		wstring text = tmx_writer.make_tu(rec, L"{$srcseg} {$transseg}") ;
		wstring expected = L"spam egg" ;
		BOOST_CHECK_EQUAL(expected, text) ;
	}
	BOOST_AUTO_TEST_CASE( make_tu_real_trans )
	{
		CMockListener dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		CResHtmlFile resFile( _T("TMX_TU") ) ;
		wstring tmplText = (LPCWSTR)resFile.text() ;
		wstring text = tmx_writer.make_tu(rec, tmplText) ;
		wstring expected = L"<seg>egg</seg>" ;
		BOOST_CHECK(text.find(expected) != wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( make_tu_real_source)
	{
		CMockListener dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		CResHtmlFile resFile( _T("TMX_TU") ) ;
		wstring tmplText = (LPCWSTR)resFile.text() ;
		wstring text = tmx_writer.make_tu(rec, tmplText) ;
		wstring expected = L"<seg>spam</seg>" ;
		BOOST_CHECK(text.find(expected) != wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( make_tu_real_creationid)
	{
		CMockListener dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		rec->set_creator(L"Elephant") ;
		CResHtmlFile resFile( _T("TMX_TU") ) ;
		wstring tmplText = (LPCWSTR)resFile.text() ;
		wstring text = tmx_writer.make_tu(rec, tmplText) ;
		wstring expected = L"creationid=\"Elephant\"" ;
		BOOST_CHECK(text.find(expected) != wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( make_tu_real_changeid)
	{
		CMockListener dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		rec->set_modified_by(L"Snuffles") ;
		CResHtmlFile resFile( _T("TMX_TU") ) ;
		wstring tmplText = (LPCWSTR)resFile.text() ;
		wstring text = tmx_writer.make_tu(rec, tmplText) ;
		wstring expected = L"changeid=\"Snuffles\"" ;
		BOOST_CHECK(text.find(expected) != wstring::npos) ;
	}

	BOOST_AUTO_TEST_CASE( SetSourceAndTrans )
	{
		CMockListener dummy ;
		CTMXWriter writer( &dummy ) ;
		writer.set_src_lang( L"jp" ) ;
		BOOST_CHECK_EQUAL ( L"JP", writer.m_src_lang ) ; 
		writer.set_target_lang( L"eN-US" ) ;
		BOOST_CHECK_EQUAL( L"EN-US", writer.m_target_lang) ;
	}
	BOOST_AUTO_TEST_CASE( cleanUpForTmx )
	{
		CMockListener dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"f\too hi!" ;
		wstring outStr = writer.cleanUpForTMX( inStr ) ;
		wstring expected = L"foo hi!" ;
		BOOST_CHECK_EQUAL( outStr, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( CTmxWriterTestCase )
	// get_segment
	BOOST_AUTO_TEST_CASE( getSegmentSimple )
	{
		CMockListener dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"foo" ;
		wstring outStr = writer.get_segment( inStr ) ;
		BOOST_CHECK_EQUAL( inStr, outStr ) ;
		// try it twice...
		outStr = writer.get_segment( inStr ) ;
		BOOST_CHECK_EQUAL( outStr, inStr) ;
	}
	BOOST_AUTO_TEST_CASE( getSegmentBr )
	{
		CMockListener dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"foo<br />" ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"foo<it pos=\"begin\" x=\"1\">&lt;br /&gt;</it>" ;
		BOOST_CHECK_EQUAL( outStr, expected) ;
	}
	BOOST_AUTO_TEST_CASE( getSegmentP )
	{
		CMockListener dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"<p>foo</p>" ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"<bpt i=\"1\" x=\"1\">&lt;p&gt;</bpt>foo<ept i=\"1\">&lt;/p&gt;</ept>" ;
		BOOST_CHECK_EQUAL( outStr, expected) ;
	}
	BOOST_AUTO_TEST_CASE( getSegmentDivSpan )
	{
		CMockListener dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"<div style=\"float:left;\"><span style=\"color:blue;\">foo</span></div>" ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"<bpt i=\"1\" x=\"1\">&lt;div style=\"float:left;\"&gt;</bpt><bpt i=\"2\" x=\"2\">&lt;span style=\"color:blue;\"&gt;</bpt>foo<ept i=\"2\">&lt;/span&gt;</ept><ept i=\"1\">&lt;/div&gt;</ept>" ;
		BOOST_CHECK_EQUAL( outStr, expected) ;
	}

	BOOST_AUTO_TEST_CASE( get_segment_bad_html )
	{
		CMockListener dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"This is < & > not cool..." ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"This is &lt; &amp; &gt; not cool..." ;
		BOOST_CHECK_EQUAL( outStr, expected) ;
	}
	BOOST_AUTO_TEST_CASE( get_segment_bad_html_with_tag )
	{
		CMockListener dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"This is < & > not cool...<div style=\"float:left;\"><span style=\"color:blue;\">foo</span></div>" ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"This is &lt; &amp; &gt; not cool...<bpt i=\"1\" x=\"1\">&lt;div style=\"float:left;\"&gt;</bpt><bpt i=\"2\" x=\"2\">&lt;span style=\"color:blue;\"&gt;</bpt>foo<ept i=\"2\">&lt;/span&gt;</ept><ept i=\"1\">&lt;/div&gt;</ept>" ;
		BOOST_CHECK_EQUAL( outStr, expected) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
