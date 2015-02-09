/*!
	@brief Tests the Array class
 */

#include "StdAfx.h"
#include "Array.h"

#include "qc_checker.h"
#include "number_check.h"
#include "allcaps_check.h"
#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

using namespace qc ;
using namespace std ;

class FailsRule : public CheckRule
{
public:
	bool check(const wstring&, const wstring&)
	{
		return false ;
	}
	void add_message(wstring messsage)
	{
		m_messages.push_back(messsage) ;
	}
};

BOOST_AUTO_TEST_SUITE( QcCheckerTests )

	BOOST_AUTO_TEST_CASE( ensure_init )
	{
		std::vector<rule_ptr> rules; 
		QcChecker checker(rules) ;

		vector<wstring> messages ;
		checker.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL( 0u, messages.size() ) ;
	}
	BOOST_AUTO_TEST_CASE( empty_source_and_target )
	{
		std::vector<rule_ptr> rules; 
		NumberCheckRule *rule1 = new NumberCheckRule;
		rules.push_back(std::shared_ptr<CheckRule>(rule1)) ;

		QcChecker checker(rules) ;

		wstring source = L"" ;
		wstring target = L"" ;
		BOOST_CHECK( checker.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( fails_rule )
	{
		std::vector<rule_ptr> rules; 
		FailsRule *rule1 = new FailsRule;
		rule1->add_message(L"Failure message") ;
		rules.push_back(std::shared_ptr<CheckRule>(rule1)) ;

		QcChecker checker(rules) ;

		BOOST_CHECK( ! checker.check(L"", L"") ) ;
		vector<wstring> messages ;
		checker.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL(1u, messages.size()) ;
		wstring message = messages[0] ;
		wstring expected = L"Failure message" ;
		BOOST_CHECK_EQUAL(message, expected) ;
	}
	BOOST_AUTO_TEST_CASE( number_check_rule_non_matching_numbers )
	{
		std::vector<rule_ptr> rules; 
		NumberCheckRule *rule1 = new NumberCheckRule;
		rules.push_back(std::shared_ptr<CheckRule>(rule1)) ;

		QcChecker checker(rules) ;
		wstring source = L"I have 5 numbers." ;
		wstring target = L"I have 7 numbers." ;
		BOOST_CHECK( ! checker.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( number_check_and_all_caps_check_fail )
	{
		std::vector<rule_ptr> rules; 
		NumberCheckRule *rule1 = new NumberCheckRule;
		AllCapsCheckRule *rule2 = new AllCapsCheckRule;
		rules.push_back(std::shared_ptr<CheckRule>(rule1)) ;
		rules.push_back(std::shared_ptr<CheckRule>(rule2)) ;

		QcChecker checker(rules) ;

		wstring source = L"I have 5 NUMBERS." ;
		wstring target = L"I have 7 numbers." ;
		BOOST_CHECK( ! checker.check(source, target) ) ;
		vector<wstring> messages ;
		checker.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL(2u, messages.size()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
