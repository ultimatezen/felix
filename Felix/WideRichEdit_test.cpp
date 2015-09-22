#include "StdAfx.h"
#include "WideRichEdit.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCWideRichEdit )

	struct BufferHolder
	{
		wstring m_buffer ;
		int SetTextBuffer(LPCWSTR text)
		{
			m_buffer = text ;
			return 0 ;
		}
	};
	BOOST_AUTO_TEST_CASE( PreTranslateMessage)
	{
		CWideRichEdit edit ;
		BOOST_CHECK(! edit.PreTranslateMessage(NULL)) ;
	}



BOOST_AUTO_TEST_SUITE_END()

#endif