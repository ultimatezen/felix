#include "stdafx.h"
#include "TextTemplate.h"
#include "ResHtmlFile.h"
#include "file.h"

#include "textstream_reader.h"
#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( AppTemplateTest )

using namespace text_tmpl ;

BOOST_AUTO_TEST_CASE( ReplaceTestTu )
	{
		CTextTemplate textTemplate ;
		textTemplate.SetMarker(L'@') ;
		textTemplate.Assign( L"id", L"1" ) ;
		textTemplate.Assign( L"srclang", L"EN-US" ) ;
		textTemplate.Assign( L"translang", L"JP" ) ;
		textTemplate.Assign( L"srcseg", L"foo" ) ;
		textTemplate.Assign( L"transseg", L"<b>bar</b>" ) ;
		textTemplate.Assign( L"creationdate", L"20060119T133635Z" ) ;
		textTemplate.Assign( L"creationid", L"Ryan" ) ;
		textTemplate.Assign( L"lastusagedate", L"20060119T133635Z" ) ;
		textTemplate.Assign( L"changedate", L"20060119T133635Z" ) ;
		textTemplate.Assign( L"changeid", L"Ryan" ) ;
		textTemplate.Assign( L"usagecount", L"1" ) ;

		file::view raw_view ;

		LPCWSTR rawData = static_cast< LPCWSTR >( raw_view.create_view( _T("C:\\Dev\\Test Files\\tatemplates\\tmx_tu_out.xml") ) ) ;
		BOOST_CHECK( rawData != NULL ) ;
		textstream_reader<wchar_t> reader ;
		reader.set_buffer(rawData) ;
		reader.jump_to_first_of(L'<') ;
		wstring expected(reader.get_current_pos()) ;
		LPCWSTR resFile = L"<tu tuid=\"{@id}\" srclang=\"{@srclang}\">\r\n"
			L"<tuv xml:lang=\"{@srclang}\">\r\n"
			L"<seg>{@srcseg}</seg>\r\n"
			L"</tuv>\r\n"
			L"<tuv xml:lang=\"{@translang}\" creationdate=\"{@creationdate}\" creationid=\"{@creationid}\" lastusagedate=\"{@lastusagedate}\" changedate=\"{@changedate}\" changeid=\"{@changeid}\" usagecount=\"{@usagecount}\">\r\n"
			L"<seg>{@transseg}</seg>\r\n"
			L"</tuv>\r\n"
			L"</tu>"
			 ;

		wstring FetchedData = textTemplate.Fetch( resFile ) ;
		BOOST_CHECK( FetchedData == expected ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
