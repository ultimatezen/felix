#include "stdafx.h"
#include "config_file.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
#include "input_device_fake.h"
#include "output_device_fake.h"

BOOST_AUTO_TEST_SUITE( config_file_tests )

	BOOST_AUTO_TEST_CASE(test_get_config_filename)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output(fake_output) ;

		CString filename = get_config_filename(_T("foo.xml"), output) ;

		const string actual(static_cast<LPCSTR>(CT2A(filename))) ;
		const string expected = "D:\\Users\\Ryan\\AppData\\Local\\Felix\\prefs\\foo.xml" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_config_filename_directory_existence_ensured)
	{
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output(fake_output) ;

		CString filename = get_config_filename(_T("foo.xml"), output) ;

		const string expected = "D:\\Users\\Ryan\\AppData\\Local\\Felix\\prefs" ;
		BOOST_CHECK_EQUAL(2u, fake_output->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("ensure_dirs", fake_output->m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(expected, fake_output->m_sensing_variable[1]) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_config_text_doesnt_exist)
	{
		InputDeviceFake *fake_input = new InputDeviceFake ;
		fake_input->m_exists = false ;
		input_device_ptr input(fake_input) ;
		OutputDeviceFake *fake_output = new OutputDeviceFake ;
		output_device_ptr output(fake_output) ;

		const wstring actual = get_config_text(_T("foo.xml"), output, input) ;
		const wstring expected = L"" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
