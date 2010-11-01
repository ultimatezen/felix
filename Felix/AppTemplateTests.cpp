#include "stdafx.h"
#include "ResHtmlFile.h"
#include "file.h"
#include "cpptempl.h"
#include "textstream_reader.h"
#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( AppTemplateTest )

using namespace cpptempl ;

BOOST_AUTO_TEST_CASE( ReplaceTestTu )
	{
		data_map data ;
		data[L"id"] = cpptempl::make_data(L"1" ) ;
		data[L"srclang"] = cpptempl::make_data(L"EN-US" ) ;
		data[L"translang"] = cpptempl::make_data(L"JP" ) ;
		data[L"srcseg"] = cpptempl::make_data(L"foo" ) ;
		data[L"transseg"] = cpptempl::make_data(L"<b>bar</b>" ) ;
		data[L"creationdate"] = cpptempl::make_data(L"20060119T133635Z" ) ;
		data[L"creationid"] = cpptempl::make_data(L"Ryan" ) ;
		data[L"lastusagedate"] = cpptempl::make_data(L"20060119T133635Z" ) ;
		data[L"changedate"] = cpptempl::make_data(L"20060119T133635Z" ) ;
		data[L"changeid"] = cpptempl::make_data(L"Ryan" ) ;
		data[L"usagecount"] = cpptempl::make_data(L"1" ) ;

		file::view raw_view ;

		LPCWSTR rawData = static_cast< LPCWSTR >( raw_view.create_view( _T("C:\\Dev\\Test Files\\tatemplates\\tmx_tu_out.xml") ) ) ;
		BOOST_CHECK( rawData != NULL ) ;
		textstream_reader<wchar_t> reader ;
		reader.set_buffer(rawData) ;
		reader.jump_to_first_of(L'<') ;
		wstring expected(reader.get_current_pos()) ;
		LPCWSTR resFile = L"<tu tuid=\"{$id}\" srclang=\"{$srclang}\">\r\n"
			L"<tuv xml:lang=\"{$srclang}\">\r\n"
			L"<seg>{$srcseg}</seg>\r\n"
			L"</tuv>\r\n"
			L"<tuv xml:lang=\"{$translang}\" creationdate=\"{$creationdate}\" creationid=\"{$creationid}\" lastusagedate=\"{$lastusagedate}\" changedate=\"{$changedate}\" changeid=\"{$changeid}\" usagecount=\"{$usagecount}\">\r\n"
			L"<seg>{$transseg}</seg>\r\n"
			L"</tuv>\r\n"
			L"</tu>"
			 ;

		wstring FetchedData = parse( resFile, data ) ;
		BOOST_CHECK( FetchedData == expected ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
