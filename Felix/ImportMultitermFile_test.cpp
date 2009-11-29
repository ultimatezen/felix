#include "StdAfx.h"
#include "ImportMultitermFile.h"
#include "MockListener.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( TestCImportMultitermFile, instantiate )
	{
		CMockListener listener ;
		CImportMultitermFile importer(&listener) ;
		ASSERT_TRUE(true) ;
	}

	TEST( TestCImportMultitermFile, get_multiterm55_line)
	{
		CMockListener listener ;
		CImportMultitermFile importer(&listener) ;
		string text = "<English>Foo\r\n" ;
		c_reader reader ;
		reader.set_buffer(text.c_str()) ;
		string col, val ;
		importer.get_multiterm55_line(reader, col, val) ;
		ASSERT_EQUALS_V("English", SimpleString(col.c_str())) ;
		ASSERT_EQUALS_V("Foo", SimpleString(val.c_str())) ;
	}
	TEST( TestCImportMultitermFile, import_multiterm_55_text)
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
		memory_engine::memory_pointer mem(new memory_local()) ;

		importer.import_multiterm_55_text(reader, source_lang, trans_lang, mem) ;
		ASSERT_EQUALS_V(2, (int)mem->size()) ;
		ASSERT_EQUALS(L"English", mem->get_header().get_source_language()) ;
	}
	TEST( TestCImportMultitermFile, import_multiterm_6_text)
	{
		CMockListener listener ;
		CImportMultitermFile importer(&listener) ;
		wstring text = L"Japanese\tEnglish\tNotes\r\n" ;
		text += L"適用\tApply\tsyngo Basic\r\n" ;
		text += L"スキャン数\tNumber of Scans\tsyngo Basic\r\n" ;

		importer.import_multiterm_6_text(text.c_str()) ;
		ASSERT_EQUALS_V(2, (int)importer.m_memory->size()) ;
		ASSERT_EQUALS(L"English", importer.m_memory->get_header().get_source_language()) ;
	}

}


#endif // #ifdef _DEBUG