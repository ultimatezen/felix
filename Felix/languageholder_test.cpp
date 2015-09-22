#include "stdafx.h"
#include ".\languageholder.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestlanguageHolder )

	BOOST_AUTO_TEST_CASE( languages )
	{
		CLanguageHolder holder ;
		std::set<tstring> langs ;
		langs.insert(tstring(_T("English"))) ;
		langs.insert(tstring(_T("Japanese"))) ;
		holder.set_languages(langs) ;

		CComboBox combo ;
		holder.put_langs_into_combo(combo) ;

		BOOST_CHECK_EQUAL(2u, holder.m_languages.size()) ;
	}
	BOOST_AUTO_TEST_CASE( set_source )
	{
		CLanguageHolder holder ;
		holder.set_source(_T("Japanese")) ;
		string source = string2string(holder.get_source_plain()) ;
		BOOST_CHECK_EQUAL("Japanese", string(source.c_str())) ;
	}
	BOOST_AUTO_TEST_CASE( set_trans )
	{
		CLanguageHolder holder ;
		holder.set_trans(_T("Japanese")) ;
		string trans = string2string(holder.get_trans_plain()) ;
		BOOST_CHECK_EQUAL("Japanese", string(trans.c_str())) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif