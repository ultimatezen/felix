#include "StdAfx.h"
#include "ImportMultitermFile.h"
#include "MockListener.h"
#include "memory_local.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "input_device_fake.h"

BOOST_AUTO_TEST_SUITE( TestCImportMultitermFile )

	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE( instantiate )
	{
		CMockListener listener ;
		CImportMultitermFile importer(&listener) ;
		BOOST_CHECK(true) ;
	}

	BOOST_AUTO_TEST_CASE( get_multiterm55_line)
	{
		CMockListener listener ;
		CImportMultitermFile importer(&listener) ;
		string text = "<English>Foo\r\n" ;
		c_reader reader ;
		reader.set_buffer(text.c_str()) ;
		const strcols cols = importer.get_multiterm55_line(reader) ;
		BOOST_CHECK_EQUAL("English", string(cols.get<0>().c_str())) ;
		BOOST_CHECK_EQUAL("Foo", string(cols.get<1>().c_str())) ;
	}
	BOOST_AUTO_TEST_CASE( import_multiterm_55_text)
	{
		CMockListener listener ;
		CImportMultitermFile importer(&listener) ;
		string text = "<English>Foo\r\n" ;
		text += "<Japanese>Japanese\r\n" ;
		text += "<Notes>Context\r\n" ;
		text += "**" ;
		text += "<English>Bar\r\n" ;
		text += "<Japanese>Chinese\r\n" ;
		text += "<Notes>Context\r\n" ;
		text += "**" ;
		c_reader reader ;
		reader.set_buffer(text.c_str()) ;
		string source_lang = "English" ;
		string trans_lang = "Japanese" ;
		mem_engine::memory_pointer mem(new memory_local()) ;

		importer.import_multiterm_55_text(reader, source_lang, trans_lang, mem) ;
		BOOST_CHECK_EQUAL(2u, mem->size()) ;
		//BOOST_CHECK_EQUAL(L"English", mem->get_memory_info()->get_source_language()) ;
	}
	BOOST_AUTO_TEST_CASE( import_multiterm_6_text)
	{
		CMockListener listener ;
		CImportMultitermFile importer(&listener) ;
		wstring text = L"Japanese\tEnglish\tNotes\r\n" ;
		text += L"適用\tApply\tsyngo Basic\r\n" ;
		text += L"スキャン数\tNumber of Scans\tsyngo Basic\r\n" ;

		importer.import_multiterm_6_text(text.c_str()) ;
		BOOST_CHECK_EQUAL(2u, importer.m_memory->size()) ;
		BOOST_CHECK_EQUAL(L"English", importer.m_memory->get_memory_info()->get_source_language()) ;
	}
	BOOST_AUTO_TEST_CASE( get_multiterm6_line)
	{
		CMockListener listener ;
		CImportMultitermFile importer(&listener) ;
		wstring text = L"spam\teggs\tcontext\r\n" ;
		textstream_reader<wchar_t> reader ;
		reader.set_buffer(text.c_str()) ;
		const wstrcols cols = importer.get_multiterm6_line(reader) ;
		BOOST_CHECK_EQUAL("spam", string(string2string(cols.get<0>()).c_str())) ;
		BOOST_CHECK_EQUAL("eggs", string(string2string(cols.get<1>()).c_str())) ;
		BOOST_CHECK_EQUAL("context", string(string2string(cols.get<2>()).c_str())) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef UNIT_TEST
