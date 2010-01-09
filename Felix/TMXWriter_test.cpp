/*!
@brief Unit tests for CTMXWriter class
@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include ".\tmxwriter.h"
#include "ResHtmlFile.h"
#include "record_local.h"
#include "output_device_fake.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{

	SimpleString errMsg( wstring expected, wstring actual )
	{
		wstring msg = L"Expected [" + expected + L"] but actual [" + actual + L"]";
		return SimpleString( CW2A( msg.c_str() ) ) ;
	}
	// write
	TEST(CTMXWriterTestCase, write_footer )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;
		OutputDeviceFake *device = new OutputDeviceFake ;
		tmx_writer.m_file = boost::shared_ptr<OutputDevice>(device) ;

		tmx_writer.write_footer() ;
		SimpleString actual = string2string(device->m_value).c_str() ;
		SimpleString expected = "  </body>\n</tmx>" ;
		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS_V(1, (int)device->m_calls.size()) ;
		ASSERT_EQUALS_V(SimpleString("write_wstring"), device->m_calls[0].c_str()) ;
	}

	// make_tu
	TEST(CTMXWriterTestCase, make_tu_simple )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		wstring text = tmx_writer.make_tu(rec, L"{$srcseg} {$transseg}") ;
		wstring expected = L"spam egg" ;
		ASSERT_EQUALS_M(expected, text, errMsg(expected, text)) ;
	}
	TEST(CTMXWriterTestCase, make_tu_real_trans )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		CResHtmlFile resFile( _T("TMX_TU") ) ;
		wstring tmplText = (LPCWSTR)resFile.text() ;
		wstring text = tmx_writer.make_tu(rec, tmplText) ;
		wstring expected = L"<seg>egg</seg>" ;
		ASSERT_TRUE_M(text.find(expected) != wstring::npos, errMsg(expected, text)) ;
	}
	TEST(CTMXWriterTestCase, make_tu_real_source)
	{
		CProgressListenerDummy dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		CResHtmlFile resFile( _T("TMX_TU") ) ;
		wstring tmplText = (LPCWSTR)resFile.text() ;
		wstring text = tmx_writer.make_tu(rec, tmplText) ;
		wstring expected = L"<seg>spam</seg>" ;
		ASSERT_TRUE_M(text.find(expected) != wstring::npos, errMsg(expected, text)) ;
	}
	TEST(CTMXWriterTestCase, make_tu_real_creationid)
	{
		CProgressListenerDummy dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		rec->set_creator(L"Elephant") ;
		CResHtmlFile resFile( _T("TMX_TU") ) ;
		wstring tmplText = (LPCWSTR)resFile.text() ;
		wstring text = tmx_writer.make_tu(rec, tmplText) ;
		wstring expected = L"creationid=\"Elephant\"" ;
		ASSERT_TRUE_M(text.find(expected) != wstring::npos, errMsg(expected, text)) ;
	}
	TEST(CTMXWriterTestCase, make_tu_real_changeid)
	{
		CProgressListenerDummy dummy ;
		CTMXWriter tmx_writer(static_cast< CProgressListener* >( &dummy ) ) ;

		mem_engine::record_pointer rec(new mem_engine::record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg");
		rec->set_modified_by(L"Snuffles") ;
		CResHtmlFile resFile( _T("TMX_TU") ) ;
		wstring tmplText = (LPCWSTR)resFile.text() ;
		wstring text = tmx_writer.make_tu(rec, tmplText) ;
		wstring expected = L"changeid=\"Snuffles\"" ;
		ASSERT_TRUE_M(text.find(expected) != wstring::npos, errMsg(expected, text)) ;
	}

	TEST( CTmxWriterTestCase, SetSourceAndTrans )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter writer( &dummy ) ;
		writer.set_src_lang( L"jp" ) ;
		ASSERT_EQUALS ( L"JP", writer.m_src_lang ) ; 
		writer.set_target_lang( L"eN-US" ) ;
		ASSERT_EQUALS_M( L"EN-US", writer.m_target_lang, errMsg( writer.m_target_lang, L"EN-US" ) ) ;
	}
	TEST( CTmxWriterTestCase, cleanUpForTmx )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"f\too hi!" ;
		wstring outStr = writer.cleanUpForTMX( inStr ) ;
		wstring expected = L"foo hi!" ;
		ASSERT_EQUALS_M( outStr, expected, errMsg( expected, outStr ) ) ;
	}
	TEST( CTmxWriterTestCase, getSegmentSimple )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"foo" ;
		wstring outStr = writer.get_segment( inStr ) ;
		ASSERT_EQUALS( inStr, outStr ) ;
		// try it twice...
		outStr = writer.get_segment( inStr ) ;
		ASSERT_EQUALS_M( outStr, inStr, errMsg( outStr, inStr ) ) ;
	}
	TEST( CTmxWriterTestCase, getSegmentBr )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"foo<br />" ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"foo<it pos=\"begin\" x=\"1\">&lt;br /&gt;</it>" ;
		ASSERT_EQUALS_M( outStr, expected, errMsg( expected, outStr ) ) ;
	}
	TEST( CTmxWriterTestCase, getSegmentP )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"<p>foo</p>" ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"<bpt i=\"1\" x=\"1\">&lt;p&gt;</bpt>foo<ept i=\"1\">&lt;/p&gt;</ept>" ;
		ASSERT_EQUALS_M( outStr, expected, errMsg( expected, outStr ) ) ;
	}
	TEST( CTmxWriterTestCase, getSegmentDivSpan )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"<div style=\"float:left;\"><span style=\"color:blue;\">foo</span></div>" ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"<bpt i=\"1\" x=\"1\">&lt;div style=\"float:left;\"&gt;</bpt><bpt i=\"2\" x=\"2\">&lt;span style=\"color:blue;\"&gt;</bpt>foo<ept i=\"2\">&lt;/span&gt;</ept><ept i=\"1\">&lt;/div&gt;</ept>" ;
		ASSERT_EQUALS_M( outStr, expected, errMsg( expected, outStr ) ) ;
	}

	TEST( CTmxWriterTestCase, get_segment_bad_html )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"This is < & > not cool..." ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"This is &lt; &amp; &gt; not cool..." ;
		ASSERT_EQUALS_M( outStr, expected, errMsg( expected, outStr ) ) ;
	}
	TEST( CTmxWriterTestCase, get_segment_bad_html_with_tag )
	{
		CProgressListenerDummy dummy ;
		CTMXWriter writer( &dummy ) ;
		wstring inStr = L"This is < & > not cool...<div style=\"float:left;\"><span style=\"color:blue;\">foo</span></div>" ;
		wstring outStr = writer.get_segment( inStr ) ;
		wstring expected = L"This is &lt; &amp; &gt; not cool...<bpt i=\"1\" x=\"1\">&lt;div style=\"float:left;\"&gt;</bpt><bpt i=\"2\" x=\"2\">&lt;span style=\"color:blue;\"&gt;</bpt>foo<ept i=\"2\">&lt;/span&gt;</ept><ept i=\"1\">&lt;/div&gt;</ept>" ;
		ASSERT_EQUALS_M( outStr, expected, errMsg( expected, outStr ) ) ;
	}

}




#endif

