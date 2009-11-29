#include "stdafx.h"
#include ".\languageholder.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG

namespace easyunit
{
	TEST( TestlanguageHolder, languages )
	{
		CLanguageHolder holder ;
		std::set<tstring> langs ;
		langs.insert(tstring(_T("English"))) ;
		langs.insert(tstring(_T("Japanese"))) ;
		holder.set_languages(langs) ;

		CComboBox combo ;
		holder.put_langs_into_combo(combo) ;

		ASSERT_EQUALS_V(2, (int)holder.m_languages.size()) ;
	}
	TEST( TestlanguageHolder, set_source )
	{
		CLanguageHolder holder ;
		holder.set_source(_T("Japanese")) ;
		string source = string2string(holder.get_source_plain()) ;
		ASSERT_EQUALS_V("Japanese", SimpleString(source.c_str())) ;
	}
	TEST( TestlanguageHolder, set_trans )
	{
		CLanguageHolder holder ;
		holder.set_trans(_T("Japanese")) ;
		string trans = string2string(holder.get_trans_plain()) ;
		ASSERT_EQUALS_V("Japanese", SimpleString(trans.c_str())) ;
	}

}

#endif