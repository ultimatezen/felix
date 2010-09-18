#include "stdafx.h"
#include "abbreviations.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( AbbreviationsTest )


BOOST_AUTO_TEST_CASE(test_default)
{
	Abbreviations abbrev ;
	std::vector<wstring> tokens ;
	tokens += L"Mr.", L"Mrs.", L"Ms.", L"Dr.", L"e.g.", L"i.e." ;
	foreach(wstring token, tokens)
	{
		BOOST_CHECK( abbrev.m_abbreviations.find(token) !=  abbrev.m_abbreviations.end()) ;
	}
	BOOST_CHECK( abbrev.m_abbreviations.find(L"foo") ==  abbrev.m_abbreviations.end()) ;
	BOOST_CHECK( abbrev.m_abbreviations.size() ==  tokens.size()) ;

}
BOOST_AUTO_TEST_CASE(test_load)
{
	Abbreviations abbrev ;
	std::vector<wstring> tokens ;
	tokens += L"butter.", L"bitter." ;
	wstring text = L"butter.\nbitter.\n" ;
	abbrev.load(text) ;
	foreach(wstring token, tokens)
	{
		BOOST_CHECK( abbrev.m_abbreviations.find(token) !=  abbrev.m_abbreviations.end()) ;
	}
	BOOST_CHECK( abbrev.m_abbreviations.find(L"Mr.") ==  abbrev.m_abbreviations.end()) ;
	BOOST_CHECK( abbrev.m_abbreviations.size() ==  tokens.size()) ;

}
BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
