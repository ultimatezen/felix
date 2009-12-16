#include "StdAfx.h"
#include "WideRichEdit.h"
#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	struct BufferHolder
	{
		wstring m_buffer ;
		int SetTextBuffer(LPCWSTR text)
		{
			m_buffer = text ;
			return 0 ;
		}
	};

	TEST( TestCWideRichEdit, load_document_utf16)
	{
		CWideRichEdit edit ;
		BufferHolder holder ;
		edit.m_set_text = boost::bind(&BufferHolder::SetTextBuffer, &holder, _1) ;

		CString location = "C:\\test\\textfiles\\utf16.txt" ;

		edit.load_document(location) ;

		wstring text = L"日本語 - I love it!" ;
		SimpleString expected = string2string(text, CP_UTF8).c_str() ;
		SimpleString actual = string2string(holder.m_buffer.substr(1), CP_UTF8).c_str() ;

		ASSERT_EQUALS_V(expected, actual) ;

		// now reload it
		edit.reload_document(location) ;

		text = L"日本語 - I love it!" ;
		expected = string2string(text, CP_UTF8).c_str() ;
		actual = string2string(holder.m_buffer.substr(1), CP_UTF8).c_str() ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCWideRichEdit, load_document_utf8)
	{
		CWideRichEdit edit ;
		BufferHolder holder ;
		edit.m_set_text = boost::bind(&BufferHolder::SetTextBuffer, &holder, _1) ;

		CString location = "C:\\test\\textfiles\\utf8.txt" ;

		edit.load_document(location) ;

		wstring text = L"日本語 - I love it!" ;
		SimpleString expected = string2string(text, CP_UTF8).c_str() ;
		SimpleString actual = string2string(holder.m_buffer.substr(1), CP_UTF8).c_str() ;

		ASSERT_EQUALS_V(expected, actual) ;

		// now reload it
		edit.reload_document(location) ;

		text = L"日本語 - I love it!" ;
		expected = string2string(text, CP_UTF8).c_str() ;
		actual = string2string(holder.m_buffer.substr(1), CP_UTF8).c_str() ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCWideRichEdit, load_document_sjis)
	{
		CWideRichEdit edit ;
		BufferHolder holder ;
		edit.m_set_text = boost::bind(&BufferHolder::SetTextBuffer, &holder, _1) ;

		CString location = "C:\\test\\textfiles\\sjis.txt" ;

		edit.load_document(location) ;

		wstring text = L"日本語 - I love it!" ;
		SimpleString expected = string2string(text, CP_UTF8).c_str() ;
		SimpleString actual = string2string(holder.m_buffer, CP_UTF8).c_str() ;

		ASSERT_EQUALS_V(expected, actual) ;

		// now reload it
		edit.reload_document(location) ;

		text = L"日本語 - I love it!" ;
		expected = string2string(text, CP_UTF8).c_str() ;
		actual = string2string(holder.m_buffer, CP_UTF8).c_str() ;

		ASSERT_EQUALS_V(expected, actual) ;
	}


}

#endif