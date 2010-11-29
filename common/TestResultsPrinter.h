/*! Prints results of unit tests.
	@date 2005/05/31
	@author Ryan Ginstrom
 */

#pragma once

#include "easyunit/testharness.h"
#include "numberfmt.h"
#include <boost/timer.hpp>

namespace easyunit
{

	/** Custom test runner with support for file(line): desc type reporting. */
	class CATLResultsPrinter : public TestPrinter
	{
		boost::timer t ;
		const TestResult *m_res ;
	public:
		/* The function we have to support as a test printer */
		void print( const TestResult *res )
		{
			m_res = res ;

			executive_summary() ;
			pass_fail_message() ;

			if ( ! all_tests_successful() )
			{
				failure_details() ;
				failure_report() ;
			}
			ATLTRACE("Finished in %.2f seconds\n\n", t.elapsed()) ;
		}
	private:
		/** Report number of passed and failed tests. */
		void failure_report() const
		{
            CString message ;
			message.Format( _T("FAILURE!\r\rFailed Tests: %d\rErrors: %d\n"), get_total_failures(), get_total_errors() ) ; 
            ATLTRACE( message ) ;
		}

		/** The detailed test result stats.
		*/
		void executive_summary() const 
		{
			ATLTRACE( "UNIT TEST RESULTS\n" ) ;
			ATLTRACE( "Test cases:       %6d\n", m_res->getTestCaseCount() ) ;
			CNumberFmt fmt ;
			ATLTRACE( "Successful tests: %6s\n", CStringA(fmt.Format(get_total_successes())) ) ;
			ATLTRACE( "Failures:         %6d\n", get_total_failures() ) ;
			ATLTRACE( "Errors:           %6d\n", get_total_errors() ) ;
		}

		/** The good news, or the bad news. */
		void pass_fail_message() const
		{
			if ( all_tests_successful() )
			{
				ATLTRACE( "\nok.\n\n" ) ;
			}
			else
			{
				ATLTRACE( "\n******** FAILURE!!! ********\n\n" ) ;
			}
		}

		bool all_tests_successful() const
		{
			return get_total_failures() == 0 && get_total_errors() == 0;
		}

		// success count
		const int get_total_successes() const
		{
			int total_successes = 0 ;

			const TestCase *test_case = m_res->getTestCases() ;

			for ( int i = 0 ; i < m_res->getTestCaseCount() ; ++i )
			{
				total_successes += get_case_successes( test_case ) ;
				test_case = test_case->getNext() ;
			}

			return total_successes ;
		}

		const int get_case_successes(const TestCase *test_case) const
		{
			int total_successes = 0 ;

			Test* tests = test_case->getTests() ;

			for ( int j = 0 ; j < test_case->getTestsCount() ; ++j )
			{
				total_successes += tests->getSuccessesCount() ;
				tests = tests->getNext() ;
			}

			return total_successes ;
		}
		
		// failure count
		const int get_total_failures() const
		{
			int total_failures = 0 ;

			TestCase *test_case = m_res->getTestCases() ;

			for ( int i = 0 ; i < m_res->getTestCaseCount() ; ++i )
			{
				total_failures += get_case_failures( test_case ) ;

				test_case = test_case->getNext() ;
			}

			return total_failures ;
		}
		const int get_case_failures(const TestCase *test_case) const
		{
			int total_failures = 0 ;

			Test* tests = test_case->getTests() ;

			for ( int j = 0 ; j < test_case->getTestsCount() ; ++j )
			{
				total_failures += tests->getFailuresCount() ;
				tests = tests->getNext() ;
			}

			return total_failures ;
		}

		// error count
		const int get_total_errors() const
		{
			int total_errors = 0 ;

			TestCase *test_case = m_res->getTestCases() ;

			for ( int i = 0 ; i < m_res->getTestCaseCount() ; ++i )
			{
				total_errors += get_case_errors(test_case) ;

				test_case = test_case->getNext() ;
			}

			return total_errors ;
		}
		const int get_case_errors(const TestCase *test_case) const
		{
			int total_errors = 0 ;

			Test* tests = test_case->getTests() ;

			for ( int j = 0 ; j < test_case->getTestsCount() ; ++j )
			{
				total_errors += tests->getErrorsCount() ;
				tests = tests->getNext() ;
			}

			return total_errors ;
		}

		// detailed results
		void failure_details() const
		{
			ATLTRACE( "********   FAILURE DETAILS   ********\n\n" ) ;

			const TestCase *test_case = m_res->getTestCases() ;

			for ( int i = 0 ; i < m_res->getTestCaseCount() ; ++i )
			{
					print_failed_tests( test_case ) ;
					test_case = test_case->getNext() ;
			}
		}
		/** Print all failed tests. */
		void print_failed_tests( const TestCase *test_case ) const
		{
			const Test *test = test_case->getTests() ;
		
			for ( int i = 0 ; i < test_case->getTestsCount() ; ++i )
			{
				print_failed_test( test ) ;
				test = test->getNext() ;
			}
		}
		/** Print failed test line, formatted for click-navigation. */
		void print_failed_test( const Test *test ) const
		{
			TestPartResult *part = test->getTestPartResult() ;
			int num_parts = get_total_num_tests( test ) ;
			for ( int i = 0 ; i < num_parts ; ++i )
			{
				if ( part->getType() == success ) 
				{
					part = part->getNext() ;
					continue ;
				}

				ATLTRACE( "Test Case: %s\tTest: %s\n",
					part->getTest()->getTestCaseName().asCharString(),
					part->getTest()->getTestName().asCharString() ) ;

				ATLTRACE("%s(%d): %s\n", 
					part->getFileName().asCharString(), 
					part->getLineNumber(), 
					part->getMessage().asCharString() ) ;

				part = part->getNext() ;
			}
		}

		int get_total_num_tests( const Test *test  ) const
		{
			return test->getErrorsCount() + test->getFailuresCount() + test->getSuccessesCount() ;
		}
	};

}
