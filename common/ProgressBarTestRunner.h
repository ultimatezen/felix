/*!	Prints results of unit tests.
	@date 2005/05/31
	@author Ryan Ginstrom
 */

#pragma once

#include <shlobj.h>  // IProgressDialog

#if (_WIN32_IE < 0x0500)
#error You must set _WIN32_IE = 0x0500 or greater in order to use the IProgressDialog component.
#endif

#ifndef __ATLSTR_H__	// CString
	#include <atlstr.h>  
#endif

#include "Exceptions.h"
#ifdef UNIT_TEST
namespace easyunit
{

	/** Runs unit tests using IProgressBar to show progress.
	*/
	class CProgressBarTestRunner : public TestRunner
	{
		CComPtr< IProgressDialog > m_ProgressDialog ;

		DWORD m_current_test_num ;
		DWORD m_test_count ;
		int m_case_count ;

		int m_num_failures ;
		int m_num_errors ;

		TestCase *m_test_cases ;

		enum { PB_FIRST_LINE = 1, PB_SEC_LINE = 2 } ;

	public:

		CProgressBarTestRunner( ) :
			m_current_test_num(0),
			m_test_count(0),
			m_case_count(0),
			m_num_failures(0),
			m_num_errors(0),
			m_test_cases(NULL)
			{}

		/** The framework will call this with an array of test cases and count value. */
		void run(TestCase *test_cases, int num_cases )
		{
			m_test_cases = test_cases ;
			m_case_count = num_cases ;

			m_test_count = count_tests() ;

			set_up_progress_bar() ;

			run_cases() ;

			clean_up_progress_bar() ;
		}

	private:
		/** Run each test case. */
		void run_cases()
		{
			TestCase *test_case = m_test_cases ;

			for ( int i = 0 ; i < m_case_count ; ++i )
			{
				try
				{
					TRACE(string(test_case->getName().asCharString())) ;
					run_tests( test_case->getTests(), test_case->getTestsCount() ) ;
					test_case = test_case->getNext() ;
				}
				catch(except::CException &e)
				{
					ATLTRACE(e.description()) ;
					TRACE(i) ;
					TRACE(string(test_case->getName().asCharString())) ;
				}
			}

		}

		/** Run each test in the test case, checking for user cancel */
		void run_tests(Test *tests, int	num_tests )
		{
			Test *current_test = tests ;

			CString line_msg ;

			for ( int i = 0 ; i < num_tests ; ++i )
			{
				if ( m_ProgressDialog->HasUserCancelled() )
				{
					return ;
				}

				// Set first line
				line_msg = _T("Test case: " ) ;
				line_msg += current_test->getTestCaseName().asCharString() ;
				line_msg += _T("   Test: " ) ;
				line_msg += current_test->getTestName().asCharString() ;

				m_ProgressDialog->SetLine(PB_FIRST_LINE, CT2W(line_msg), FALSE, NULL);

				current_test->run() ;

				m_num_errors += current_test->getErrorsCount() ;
				m_num_failures += current_test->getFailuresCount() ;

				// Set second line
				line_msg.Format( _T("ERRORS: %d   FAILURES: %d"), 
					m_num_errors, m_num_failures ) ;

				m_ProgressDialog->SetLine(PB_SEC_LINE, CT2W(line_msg), FALSE, NULL);

				// Set progress value
				m_current_test_num++ ;
				m_ProgressDialog->SetProgress(m_current_test_num, m_test_count );

				// next test
				current_test = current_test->getNext() ;
			}
		}

		/** All the setup stuff for our progress bar */
		void set_up_progress_bar()
		{
			HRESULT hr = ::CoCreateInstance( CLSID_ProgressDialog, NULL, CLSCTX_INPROC_SERVER, IID_IProgressDialog, (void **)&m_ProgressDialog ) ;
			ATLASSERT ( SUCCEEDED( hr ) ) ; 
			if ( FAILED( hr ) ) 
			{
				return ;
			}
			
			m_ProgressDialog->SetTitle( L"Running Unit Tests" ) ;
			m_ProgressDialog->StartProgressDialog(::GetActiveWindow(), NULL, PROGDLG_AUTOTIME, NULL ) ;
			m_ProgressDialog->SetCancelMsg(L"Canceling unit tests...", NULL);

			m_ProgressDialog->Timer(PDTIMER_RESET, NULL);		
		}

		/* Clean up the progress bar when done or canceled. */
		void clean_up_progress_bar()
		{
			m_ProgressDialog->StopProgressDialog();
		}


		/** Get the total number of tests.
		This is used to set up the progress bar.
		*/
		DWORD count_tests()
		{
			DWORD num_tests(0) ;

			TestCase *test_case = m_test_cases ;

			for ( int i = 0 ; i < m_case_count ; ++i )
			{
				num_tests += test_case->getTestsCount() ;
				test_case = test_case->getNext() ;
			}
			return num_tests ;
		}
	};
}
#endif