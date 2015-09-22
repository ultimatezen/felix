#include "stdafx.h"
#include "textstream_reader.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( textstream_reader_test )

	using namespace std ;

	BOOST_AUTO_TEST_CASE( TestConstructorCopy )
	{
		// test with char
		std::string afoo = "foo" ;
		c_reader creader( afoo.c_str() ) ;
		c_reader ccopy( creader ) ;
		BOOST_CHECK_EQUAL( CStringA( afoo.c_str() ), ccopy.get_buffer() ) ;

		string abar ;
		creader.getline( abar ) ;
		BOOST_CHECK_EQUAL( CStringA( abar.c_str() ), "foo" ) ;

		c_reader ccopy2( creader ) ;
		BOOST_CHECK_EQUAL( CStringA( "foo" ), creader.get_buffer() ) ;
		BOOST_CHECK_EQUAL( CStringA( "foo" ), ccopy2.get_buffer() ) ;
		BOOST_CHECK( NULL == creader.get_current_pos() ) ;
		BOOST_CHECK(NULL == ccopy2.get_current_pos() ) ;

		// test with wchar_t
		wstring wfoo = L"foo" ;
		wc_reader wreader( wfoo.c_str() ) ;
		wc_reader wcopy( wreader ) ;
		BOOST_CHECK_EQUAL( CStringA( wfoo.c_str() ), CStringA( wcopy.get_buffer() ) ) ;
		BOOST_CHECK_EQUAL( CStringA( wfoo.c_str() ), CStringA( wcopy.get_current_pos() ) ) ;
	}


	/** constructor. 
	*/
	BOOST_AUTO_TEST_CASE( TestConstructorCharPtrEmpty )
	{
		// test with char
		c_reader	creader ;
		BOOST_CHECK( NULL == creader.get_buffer()) ;

		// test with wchar_t
		wc_reader	wreader( NULL ) ;
		BOOST_CHECK(NULL == wreader.get_buffer()) ;

	}

	BOOST_AUTO_TEST_CASE( TestConstructorCharPtrWithText )
	{
		// test with char
		string afoo = "foo" ;
		c_reader creader( afoo.c_str() ) ;
		BOOST_CHECK_EQUAL( CStringA( "foo" ), creader.get_buffer() ) ;

		// test with wchar_t
		wstring wfoo = L"foo" ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK_EQUAL( CStringA( "foo" ), CStringA( wreader.get_buffer() ) ) ;
	}
	/************************************************************************/
	/* functions                                                            */
	/************************************************************************/
	BOOST_AUTO_TEST_CASE( split )
	{
		vector<string> items ;
		string buffer = "a b c" ;
		// test with char
		c_reader creader(buffer.c_str()) ;

		BOOST_CHECK_EQUAL( 3, (int)creader.split(items).size() ) ;
		BOOST_CHECK_EQUAL( "a", string(items[0].c_str()) ) ;
		BOOST_CHECK_EQUAL( "b", string(items[1].c_str()) ) ;
		BOOST_CHECK_EQUAL( "c", string(items[2].c_str()) ) ;
	}


	/** Get the length of the buffer. 
	The buffer's length is constant, not dependent on current pos
	*/
	BOOST_AUTO_TEST_CASE( TestLength )
	{
		// test with char
		c_reader	creader ;
		BOOST_CHECK_EQUAL( 0u, creader.length()) ;
		string afoo = "foo" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK_EQUAL( 3u, creader.length()) ;

		string abar ;
		creader.getline( abar ) ;
		BOOST_CHECK_EQUAL( CStringA( abar.c_str() ), "foo" ) ;
		BOOST_CHECK_EQUAL( 3u, creader.length()) ;


		// test with wchar_t
		wstring wfoo = L"foo" ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK_EQUAL( CStringA( "foo" ), CStringA( wreader.get_buffer() ) ) ;

		int expected = 3 ;
		int actual = wreader.length() ;
		CStringA msg ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		wreader.advance() ;

		expected = 3 ;
		actual = wreader.length() ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		wreader.advance() ;

		expected = 3 ;
		actual = wreader.length() ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		wreader.advance() ;

		expected = 3 ;
		actual = wreader.length() ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		wreader.set_buffer( NULL ) ;
		expected = 0 ;
		actual = wreader.length() ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}



	/** Get the length from the current pos to the end of the buffer. 
	*/
	BOOST_AUTO_TEST_CASE( TestLengthAtPos )
	{
		// test with char
		c_reader	creader ;
		BOOST_CHECK_EQUAL( 0u, creader.length_at_pos()) ;
		string afoo = "foo" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK_EQUAL( 3u, creader.length_at_pos()) ;

		string abar ;
		creader.getline( abar ) ;
		BOOST_CHECK_EQUAL( CStringA( abar.c_str() ), "foo" ) ;
		BOOST_CHECK_EQUAL( 0u, creader.length_at_pos()) ;


		// test with wchar_t
		wstring wfoo = L"foo" ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK_EQUAL( CStringA( "foo" ), CStringA( wreader.get_buffer() ) ) ;

		int expected = 3 ;
		int actual = wreader.length_at_pos() ;
		CStringA msg ;

		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		wreader.advance() ;

		expected = 2 ;
		actual = wreader.length_at_pos() ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		wreader.advance() ;

		expected = 1 ;
		actual = wreader.length_at_pos() ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		wreader.advance() ;

		expected = 0 ;
		actual = wreader.length_at_pos() ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		wreader.set_buffer( NULL ) ;

		expected = 0 ;
		actual = wreader.length_at_pos() ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}



	/** Set the buffer. 
	*/
	BOOST_AUTO_TEST_CASE( Testset_buffer )
	{
		// test with char
		c_reader	creader ;
		BOOST_CHECK( NULL == creader.get_buffer() ) ;
		string afoo = "foo" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK_EQUAL( CStringA("foo"), creader.get_buffer() ) ;

		wc_reader	wcreader ;
		BOOST_CHECK_EQUAL( CStringA(), CStringA( wcreader.get_buffer() ) ) ;
		wstring wfoo = L"foo" ;
		wcreader.set_buffer( wfoo.c_str() ) ;
		BOOST_CHECK_EQUAL( CStringA("foo"), CStringA( wcreader.get_buffer() ) ) ;

	}

	// assignment operators


	/** Assignment operator. 
	*/
	BOOST_AUTO_TEST_CASE( TestOpAssign )
	{
		// test with char
		c_reader	creader ;
		BOOST_CHECK( NULL == creader.get_buffer() ) ;
		string afoo = "foo" ;
		creader = afoo.c_str() ;
		BOOST_CHECK_EQUAL( CStringA("foo"), creader.get_buffer() ) ;

		// test with wchar_t
		wc_reader	wcreader ;
		BOOST_CHECK( NULL == wcreader.get_buffer() ) ;
		wstring wfoo = L"foo" ;
		wcreader = ( wfoo.c_str() ) ;
		BOOST_CHECK_EQUAL( CStringA("foo"), CStringA( wcreader.get_buffer() ) ) ;
	}


	/** Assignment operator. 
	*/
	BOOST_AUTO_TEST_CASE( TestOpAssignReader )
	{
		// test with char
		c_reader	creader ;
		string afoo = "foo" ;
		creader = afoo.c_str() ;
		c_reader ccopy = creader ;
		BOOST_CHECK_EQUAL( CStringA("foo"), ccopy.get_buffer() ) ;

		// test with wchar_t
		wc_reader	wcreader ;
		wstring wfoo = L"foo" ;
		wcreader = ( wfoo.c_str() ) ;
		wc_reader wcopy = wcreader ;
		BOOST_CHECK_EQUAL( CStringA("foo"), CStringA( wcopy.get_buffer() ) ) ;
	}



	/** Whether the buffer is empty. 
	*/
	BOOST_AUTO_TEST_CASE( TestEmpty )
	{
		// test with char
		c_reader	creader ;
		BOOST_CHECK( creader.empty() ) ;
		string afoo = "foo" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK( ! creader.empty() ) ;

		string abar ;
		creader.getline( abar ) ;
		BOOST_CHECK_EQUAL( CStringA( abar.c_str() ), "foo" ) ;
		BOOST_CHECK( creader.empty() ) ;


		// test with wchar_t
		wstring wfoo = L"foo" ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK( ! wreader.empty() ) ;
		wreader.advance() ;
		BOOST_CHECK( ! wreader.empty() ) ;
		wreader.advance() ;
		BOOST_CHECK( ! wreader.empty() ) ;
		wreader.advance() ;
		BOOST_CHECK( wreader.empty() ) ;
		wreader.set_buffer( NULL ) ;
		BOOST_CHECK( wreader.empty() ) ;

	}

	/** Determines if the present character is a whitespace character. 
	*/
	BOOST_AUTO_TEST_CASE( Testis_space )
	{
		c_reader creader ;
		BOOST_CHECK( ! creader.is_space() ) ; // NULL

		// test with wchar_t
		wstring wfoo = L"a b\nc\td" ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK( ! wreader.is_space() ) ; // a
		wreader.advance() ;
		BOOST_CHECK( wreader.is_space() ) ; // space
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_space() ) ; // b
		wreader.advance() ;
		BOOST_CHECK( wreader.is_space() ) ; // \n
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_space() ) ; // c
		wreader.advance() ;
		BOOST_CHECK( wreader.is_space() ) ; // \t
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_space() ) ; // d
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_space() ) ; // NULL
	}



	/** Determines if the present character is a punctuation character. 
	*/
	BOOST_AUTO_TEST_CASE( is_punct_empty )
	{
		c_reader reader ;
		BOOST_CHECK(! reader.is_punct()) ;
	}
	BOOST_AUTO_TEST_CASE( Testis_punct )
	{
		c_reader creader ;
		BOOST_CHECK( ! creader.is_punct() ) ; // NULL

		string afoo = ",;a b.c(d)!\t" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK( creader.is_punct() ) ; // ,
		creader.advance() ;
		BOOST_CHECK( creader.is_punct() ) ; // ;
		creader.advance() ;
		BOOST_CHECK( ! creader.is_punct() ) ; // a
		creader.advance() ;
		BOOST_CHECK( ! creader.is_punct() ) ; // space
		creader.advance() ;
		BOOST_CHECK( ! creader.is_punct() ) ; // b
		creader.advance() ;
		BOOST_CHECK( creader.is_punct() ) ; // .
		creader.advance() ;
		BOOST_CHECK( ! creader.is_punct() ) ; // c
		creader.advance() ;
		BOOST_CHECK( creader.is_punct() ) ; // (
		creader.advance() ;
		BOOST_CHECK( ! creader.is_punct() ) ; // d
		creader.advance() ;
		BOOST_CHECK( creader.is_punct() ) ; // )
		creader.advance() ;
		BOOST_CHECK( creader.is_punct() ) ; // !
		creader.advance() ;
		BOOST_CHECK( ! creader.is_punct() ) ; // \t
		creader.advance() ;
		BOOST_CHECK( ! creader.is_punct() ) ; // NULL

		// test with wchar_t
		wstring wfoo = L"a b.c(d)!\t" ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK( ! wreader.is_punct() ) ; // a
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_punct() ) ; // space
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_punct() ) ; // b
		wreader.advance() ;
		BOOST_CHECK( wreader.is_punct() ) ; // .
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_punct() ) ; // c
		wreader.advance() ;
		BOOST_CHECK( wreader.is_punct() ) ; // (
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_punct() ) ; // d
		wreader.advance() ;
		BOOST_CHECK( wreader.is_punct() ) ; // )
		wreader.advance() ;
		BOOST_CHECK( wreader.is_punct() ) ; // !
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_punct() ) ; // \t
	}

	/** Determines if the present character is a digit. 
	*/
	BOOST_AUTO_TEST_CASE( is_digit_empty )
	{
		c_reader reader ;
		BOOST_CHECK(! reader.is_digit()) ;
	}
	BOOST_AUTO_TEST_CASE( Testis_digit )
	{
		// test with wchar_t
		wstring wfoo = L"1a " ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK( wreader.is_digit() ) ; // 1
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_digit() ) ; // a
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_digit() ) ; // space
		wreader.advance() ;
		BOOST_CHECK( ! wreader.is_digit() ) ; // NULL
	}

	BOOST_AUTO_TEST_CASE( is_hexint_digit_empty )
	{
		c_reader reader ;
		BOOST_CHECK(! reader.is_hexint_digit()) ;
	}
	BOOST_AUTO_TEST_CASE( Testis_hexint_digit )
	{
		c_reader creader ;
		BOOST_CHECK( ! creader.is_hexint_digit() ) ; // NULL

		string text = "1A b.\t" ;
		creader.set_buffer( text.c_str() ) ;
		BOOST_CHECK( creader.is_hexint_digit() ) ; // 1
		creader.advance() ;
		BOOST_CHECK( creader.is_hexint_digit() ) ; // A
		creader.advance() ;
		BOOST_CHECK( ! creader.is_hexint_digit() ) ; // space
		creader.advance() ;
		BOOST_CHECK( creader.is_hexint_digit() ) ; // b
		creader.advance() ;
		BOOST_CHECK( ! creader.is_hexint_digit() ) ; // .
		creader.advance() ;
		BOOST_CHECK( ! creader.is_hexint_digit() ) ; // \t
		creader.advance() ;
		BOOST_CHECK( ! creader.is_hexint_digit() ) ; // NULL
	}

	/** Determines if the present character is a the specified character. 
	*/
	BOOST_AUTO_TEST_CASE( Testcurrent_is )
	{
		c_reader creader ;
		BOOST_CHECK( ! creader.current_is( 'a' ) ) ; // NULL
		BOOST_CHECK( ! creader.current_is( 'b' ) ) ; // NULL

		string afoo = "12A b.C5D6!\t" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK( creader.current_is( '1' ) ) ; // 1
		BOOST_CHECK( ! creader.current_is( '2' ) ) ; // 1
		creader.advance() ;
		BOOST_CHECK( creader.current_is( '2' ) ) ; // 2
		BOOST_CHECK( ! creader.current_is( '1' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( creader.current_is( 'A' ) ) ; // A
		BOOST_CHECK( creader.current_is( 'A' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( creader.current_is( ' ' ) ) ; // space
		BOOST_CHECK( creader.current_is( ' ' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( ! creader.current_is( 'B' ) ) ; // b
		BOOST_CHECK( creader.current_is( 'b' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( ! creader.current_is( ',' ) ) ; // .
		BOOST_CHECK( creader.current_is( '.' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( creader.current_is( 'C' ) ) ; // C
		BOOST_CHECK( ! creader.current_is( 'c' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( ! creader.current_is( 't' ) ) ; // 5
		BOOST_CHECK( creader.current_is( '5' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( creader.current_is( 'D' ) ) ; // D
		BOOST_CHECK( ! creader.current_is( 'd' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( creader.current_is( '6' ) ) ; // 6
		BOOST_CHECK( ! creader.current_is( 't' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( creader.current_is( '!' ) ) ; // !
		BOOST_CHECK( ! creader.current_is( ' ' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( ! creader.current_is( 'a' ) ) ; // \t
		BOOST_CHECK( creader.current_is( '\t' ) ) ; // 
		creader.advance() ;
		BOOST_CHECK( ! creader.current_is( 'b' ) ) ; // NULL
		BOOST_CHECK( ! creader.current_is( 'a' ) ) ; // 

		// test with wchar_t
		wstring wfoo = L"123abc" ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK( wreader.current_is(L'1') ) ; // 1
		wreader.advance() ;
		BOOST_CHECK( wreader.current_is(L'2') ) ; // 2
		wreader.advance() ;
		BOOST_CHECK( wreader.current_is(L'3') ) ; // 3
		wreader.advance() ;
		BOOST_CHECK( wreader.current_is(L'a') ) ; // a
		wreader.advance() ;
		BOOST_CHECK( wreader.current_is(L'b') ) ; // b
		wreader.advance() ;
		BOOST_CHECK( wreader.current_is(L'c') ) ; // c
		wreader.advance() ;
		BOOST_CHECK( ! wreader.current_is(L'a') ) ; // NULL
		BOOST_CHECK( ! wreader.current_is(L'3') ) ; // NULL
	}



	/** Determines if the string at the current pos matches the specified string. 
	*/
	BOOST_AUTO_TEST_CASE( Testcurrent_isPtrThrows )
	{
		wstring wfoo = L"123abc" ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK_THROW(wreader.current_is( (wchar_t *)NULL ), std::exception) ;
	}

	BOOST_AUTO_TEST_CASE( Testcurrent_isPtr )
	{
		c_reader reader ;
		BOOST_CHECK( ! reader.current_is("1") ) ; // 1
		BOOST_CHECK( ! reader.current_is("12") ) ; // 12

		// test with wchar_t
		wstring wfoo = L"123abc" ;
		wc_reader wreader( wfoo.c_str() ) ;
		BOOST_CHECK( wreader.current_is(L"1") ) ; // 1
		BOOST_CHECK( wreader.current_is(L"12") ) ; // 12
		BOOST_CHECK( wreader.current_is(L"123") ) ; // 123
		wreader.advance() ;
		BOOST_CHECK( wreader.current_is(L"2") ) ; // 2
		BOOST_CHECK( wreader.current_is(L"23") ) ; // 23
		BOOST_CHECK( wreader.current_is(L"23a") ) ; // 23a
		BOOST_CHECK( wreader.current_is(L"23abc") ) ; // 23abc

		wstring bar ;
		wreader.getline( bar ) ;
		BOOST_CHECK( ! wreader.current_is(L"2") ) ; // 2
		BOOST_CHECK( ! wreader.current_is(L"23") ) ; // 23
		BOOST_CHECK( ! wreader.current_is(L"23a") ) ; // 23a
		BOOST_CHECK( ! wreader.current_is(L"23abc") ) ; // 23abc
	}


	/** Take a peek at the current character, without eating it. 
	*/
	BOOST_AUTO_TEST_CASE( peek_empty )
	{
		c_reader creader ;
		BOOST_CHECK_EQUAL( char(0), creader.peek( ) ) ; // NULL
		BOOST_CHECK_EQUAL( char(0), creader.peek( ) ) ; // NULL
	}
	BOOST_AUTO_TEST_CASE( peek_a_b )
	{
		c_reader creader ;
		string afoo = "a b" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK_EQUAL( 'a', creader.peek( ) ) ; // a
		BOOST_CHECK_EQUAL( 'a', creader.peek( ) ) ; // a
		creader.advance() ;
		BOOST_CHECK_EQUAL( ' ', creader.peek( ) ) ; // 
		creader.advance() ;
		BOOST_CHECK_EQUAL( 'b', creader.peek( ) ) ; // b
		creader.advance() ;
		BOOST_CHECK_EQUAL( char(0), creader.peek( ) ) ; // NULL
	}


	/** Advance the specified number of steps. 
	*/
	BOOST_AUTO_TEST_CASE( Testadvance )
	{
		wc_reader wcreader ;

		wstring wfoo = L"a b" ;
		wcreader.set_buffer( wfoo.c_str() ) ;

		BOOST_CHECK_EQUAL( L'a', wcreader.peek( ) ) ; // a
		wcreader.advance() ;

		BOOST_CHECK_EQUAL( L' ', wcreader.peek( ) ) ; // a
		wcreader.advance() ;

		BOOST_CHECK_EQUAL( L'b', wcreader.peek( ) ) ; // b
		wcreader.advance() ;

		int expected = 0 ;
		int actual = (int)wcreader.peek() ;
		CStringA msg ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;

		BOOST_CHECK( wcreader.empty() ) ;
		wcreader.advance() ;
		BOOST_CHECK( wcreader.empty() ) ;

		expected = 0 ;
		actual = (int)wcreader.peek() ;
		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;

		// redo...

		wcreader.set_buffer( wfoo.c_str() ) ;
		BOOST_CHECK_EQUAL( L'a', wcreader.peek( ) ) ; // a
		wcreader.advance( 50 ) ;
		BOOST_CHECK_EQUAL( wchar_t(0), wcreader.peek( ) ) ; //
		BOOST_CHECK( wcreader.empty() ) ;

		wcreader.set_buffer( wfoo.c_str() ) ;
		BOOST_CHECK_EQUAL( L'a', wcreader.peek( ) ) ; // a
		wcreader.advance( 2 ) ;
		BOOST_CHECK_EQUAL( L'b', wcreader.peek( ) ) ; //
		BOOST_CHECK( ! wcreader.empty() ) ;

	}


	/** Get the next character. 
	*/
	BOOST_AUTO_TEST_CASE( Testnextc )
	{
		c_reader creader ;
		BOOST_CHECK_EQUAL( char(0), creader.nextc( ) ) ; // NULL
		BOOST_CHECK_EQUAL( char(0), creader.nextc( ) ) ; // NULL

		string afoo = "a b" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK_EQUAL( ' ', creader.nextc( ) ) ; // space
		BOOST_CHECK_EQUAL( ' ', creader.peek( ) ) ; // space
		BOOST_CHECK_EQUAL( 'b', creader.nextc( ) ) ; // space
		BOOST_CHECK_EQUAL( 'b', creader.peek( ) ) ; // space
		BOOST_CHECK_EQUAL( char(0), creader.nextc( ) ) ; // NULL
		BOOST_CHECK_EQUAL( char(0), creader.peek( ) ) ; //
		BOOST_CHECK_EQUAL( char(0), creader.nextc( ) ) ; // NULL
	}


	/** Get the current character, and advance. 
	*/
	BOOST_AUTO_TEST_CASE( TestgetRetVal )
	{
		c_reader creader ;
		BOOST_CHECK_EQUAL( char(0), creader.get( ) ) ; // NULL
		BOOST_CHECK_EQUAL( char(0), creader.get( ) ) ; // NULL

		string afoo = "a b" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK_EQUAL( 'a', creader.peek( ) ) ; // a
		BOOST_CHECK_EQUAL( 'a', creader.get( ) ) ; // a

		BOOST_CHECK_EQUAL( ' ', creader.peek( ) ) ; // a
		BOOST_CHECK_EQUAL( ' ', creader.get( ) ) ; // a

		BOOST_CHECK_EQUAL( 'b', creader.peek( ) ) ; // a
		BOOST_CHECK_EQUAL( 'b', creader.get( ) ) ; // a

		BOOST_CHECK_EQUAL( char(0), creader.peek( ) ) ; // a
		BOOST_CHECK_EQUAL( char(0), creader.get( ) ) ; // a
	}

	/** Get the current character, and advance. 
	*/
	BOOST_AUTO_TEST_CASE( Testget )
	{
		wc_reader wcreader ;
		wchar_t c ;
		wcreader.get(c) ;
		BOOST_CHECK_EQUAL( wchar_t(0), c ) ; // NULL

		wstring wfoo = L"a b" ;
		wcreader.set_buffer( wfoo.c_str() ) ;

		BOOST_CHECK_EQUAL( L'a', wcreader.peek( ) ) ; // a
		wcreader.get(c) ;
		BOOST_CHECK_EQUAL( L'a', c ) ; // a
		wcreader.get(c) ;
		BOOST_CHECK_EQUAL( L' ', c ) ; // space
		wcreader.get(c) ;
		BOOST_CHECK_EQUAL( L'b', c ) ; // b
		BOOST_CHECK( wcreader.empty() ) ;

		// test on empty...
		BOOST_CHECK( ! wcreader.get(c) ) ;
		wcreader.get(c) ;
		BOOST_CHECK_EQUAL( wchar_t(0), c ) ; // NULL
		wcreader.get(c) ;
		BOOST_CHECK_EQUAL( wchar_t(0), c ) ; //

	}


	/** Get the current character, and advance. 
	*/
	BOOST_AUTO_TEST_CASE( TestgetString )
	{
		c_reader creader ;
		string c ;
		creader.get(c) ;
		BOOST_CHECK_EQUAL( CStringA( "" ), c.c_str() ) ; // NULL

		string foo = "abc" ;
		creader.set_buffer( foo.c_str() ) ;

		creader.get(c) ;
		BOOST_CHECK_EQUAL( CStringA( "a" ), c.c_str() ) ; // a

		creader.get(c) ;
		BOOST_CHECK_EQUAL( CStringA( "ab" ), c.c_str() ) ; // ab

		creader.get(c) ;
		BOOST_CHECK_EQUAL( CStringA( "abc" ), c.c_str() ) ; // abc

		// empty now...

		creader.get(c) ;
		BOOST_CHECK_EQUAL( CStringA( "abc" ), c.c_str() ) ; // abc
		creader.get(c) ;
		BOOST_CHECK_EQUAL( CStringA( "abc" ), c.c_str() ) ; // abc

	}

	/** Get the specified number of characters. broken into a separate function, instead of default param, for speed of base case
	*/
	BOOST_AUTO_TEST_CASE( TestgetStrLen )
	{
		c_reader creader ;
		string c ;
		creader.get(c) ;
		BOOST_CHECK_EQUAL( CStringA( "" ), c.c_str() ) ; // NULL

		string foo = "abc" ;
		creader.set_buffer( foo.c_str() ) ;

		creader.get(c, 1) ;
		BOOST_CHECK_EQUAL( CStringA( "a" ), c.c_str() ) ; // a

		creader.get(c, 2) ;
		BOOST_CHECK_EQUAL( CStringA( "abc" ), c.c_str() ) ; // abc

		// empty now...

		creader.get(c, 2) ;
		BOOST_CHECK_EQUAL( CStringA( "abc" ), c.c_str() ) ; // abc
		creader.get(c, 1) ;
		BOOST_CHECK_EQUAL( CStringA( "abc" ), c.c_str() ) ; // abc

		// try again...
		creader.set_buffer( foo.c_str() ) ;
		c = "" ;
		BOOST_CHECK_EQUAL( CStringA( "" ), c.c_str() ) ; // abc
		creader.get(c, 3) ;
		BOOST_CHECK_EQUAL( CStringA( "abc" ), c.c_str() ) ; // abc

		// try again...
		creader.set_buffer( foo.c_str() ) ;
		c = "" ;
		BOOST_CHECK_EQUAL( CStringA( "" ), c.c_str() ) ; // abc
		creader.get(c, 50) ;
		BOOST_CHECK_EQUAL( CStringA( "abc" ), c.c_str() ) ; // abc
	}


	/** Back up the specified number of steps. 
	*/
	BOOST_AUTO_TEST_CASE( Testunget )
	{
		c_reader creader ;
		BOOST_CHECK_EQUAL( char(0), creader.unget( ) ) ; // NULL
		BOOST_CHECK_EQUAL( char(0), creader.unget( 3 ) ) ; // NULL

		string afoo = "a b" ;
		creader.set_buffer( afoo.c_str() ) ;
		BOOST_CHECK_EQUAL( 'a', creader.peek( ) ) ; // a
		BOOST_CHECK_EQUAL( char(0), creader.unget( ) ) ; // NULL

		creader.get() ;
		BOOST_CHECK_EQUAL( 'a', creader.unget( ) ) ; // a
		BOOST_CHECK_EQUAL( char(0), creader.unget( ) ) ; // NULL

		creader.get() ;
		creader.get() ;
		creader.get() ;
		BOOST_CHECK_EQUAL( 'b', creader.unget( ) ) ; // a
		BOOST_CHECK_EQUAL( ' ', creader.unget( ) ) ; // a
		BOOST_CHECK_EQUAL( 'a', creader.unget( ) ) ; // a
		BOOST_CHECK_EQUAL( char(0), creader.unget( ) ) ; // NULL
	}


	/** Get the buffer. 
	*/
	BOOST_AUTO_TEST_CASE( Testget_buffer )
	{
		c_reader creader ;
		BOOST_CHECK( NULL == creader.get_buffer() ) ; // NULL

		string foo = "abc" ;
		creader.set_buffer( foo.c_str() ) ;

		BOOST_CHECK_EQUAL( CStringA( "abc" ), creader.get_buffer() ) ; // a
	}


	/** Eat if the current character is the specified character. 
	*/
	BOOST_AUTO_TEST_CASE( Testeat_if )
	{
		wc_reader wcreader ;

		wstring wfoo = L"abcabc" ;
		wcreader.set_buffer( wfoo.c_str() ) ;

		BOOST_CHECK( wcreader.current_is( L'a' ) ) ; // a
		BOOST_CHECK( wcreader.eat_if( L'a' ) ) ; // a

		BOOST_CHECK_EQUAL( L'b', wcreader.peek( ) ) ; // b
		BOOST_CHECK( ! wcreader.eat_if( L'a' ) ) ; // 
		BOOST_CHECK_EQUAL( L'b', wcreader.peek( ) ) ; // 
		BOOST_CHECK( wcreader.eat_if( L'b' ) ) ; // 

		BOOST_CHECK_EQUAL( L'c', wcreader.peek( ) ) ; // c
		BOOST_CHECK( ! wcreader.eat_if( L'a' ) ) ; // 
		BOOST_CHECK( wcreader.current_is( L'c' ) ) ; // c
		BOOST_CHECK( wcreader.eat_if( L'c' ) ) ; // 
		BOOST_CHECK( wcreader.current_is( L'a' ) ) ; // a

		BOOST_CHECK( wcreader.eat_if( L'a' ) ) ; // 
		BOOST_CHECK( ! wcreader.eat_if( L'a' ) ) ; // 
		BOOST_CHECK( wcreader.eat_if( L'b' ) ) ; // 
		BOOST_CHECK( wcreader.eat_if( L'c' ) ) ; // 

		BOOST_CHECK( wcreader.empty() ) ; // NULL
		BOOST_CHECK( ! wcreader.eat_if( L'a' ) ) ; // 
		BOOST_CHECK( ! wcreader.eat_if( L'b' ) ) ; // 
	}


	/** Eat until the next non-whitespace character. 
	*/
	BOOST_AUTO_TEST_CASE( Testeat_whitespace )
	{
		wc_reader wcreader ;
		BOOST_CHECK( ! wcreader.eat_whitespace() ) ; // NULL

		wstring wfoo = L"  a" ;
		wcreader.set_buffer( wfoo.c_str() ) ;

		BOOST_CHECK( wcreader.eat_whitespace() ) ; // NULL
		BOOST_CHECK( wcreader.current_is( L'a' ) ) ; // a

		// no spaces...
		wfoo = L"a" ;
		wcreader.set_buffer( wfoo.c_str() ) ;

		BOOST_CHECK( wcreader.eat_whitespace() ) ; // NULL
		BOOST_CHECK( wcreader.current_is( L'a' ) ) ; // a

	}

	/** Jump to the first of the specified character.
	If the function fails, no jump is made and returns false. 
	Set eat to true to jump past char being jumped to
	*/
	BOOST_AUTO_TEST_CASE( Testjump_to_first_ofEmpty )
	{
		c_reader creader ;
		BOOST_CHECK( ! creader.jump_to_first_of( '\n' ) ) ;
		BOOST_CHECK( ! creader.jump_to_first_of( '\n', true ) ) ;
	}
	BOOST_AUTO_TEST_CASE( Testjump_to_first_ofReg )
	{
		c_reader creader ;
		string foo = "123456" ;
		creader.set_buffer( foo.c_str() ) ;
		BOOST_CHECK( ! creader.jump_to_first_of( '\n' ) ) ;
		BOOST_CHECK_EQUAL( '1', creader.peek() ) ;

		BOOST_CHECK( creader.jump_to_first_of( '1' ) ) ;
		BOOST_CHECK_EQUAL( '1', creader.peek() ) ;

		BOOST_CHECK( creader.jump_to_first_of( '1', true ) ) ;
		BOOST_CHECK_EQUAL( '2', creader.peek() ) ;

		BOOST_CHECK( ! creader.jump_to_first_of( '\n' ) ) ;
		BOOST_CHECK_EQUAL( '2', creader.peek() ) ;

		BOOST_CHECK( creader.jump_to_first_of( '5' ) ) ;
		BOOST_CHECK_EQUAL( '5', creader.peek() ) ;
	}

	/** Jump to the first of the specified string.
	If the function fails, no jump is made and returns false. 
	Set eat to true to jump past string being jumped to
	*/
	BOOST_AUTO_TEST_CASE( Testjump_to_first_ofStrEmpty )
	{
		c_reader creader ;
		BOOST_CHECK( ! creader.jump_to_first_of( "\n\t" ) ) ;
		BOOST_CHECK( ! creader.jump_to_first_of( "\n\t", true ) ) ;
	}
	BOOST_AUTO_TEST_CASE( Testjump_to_first_ofStrReg )
	{
		c_reader creader ;
		string foo = "123456" ;
		creader.set_buffer( foo.c_str() ) ;
		BOOST_CHECK( ! creader.jump_to_first_of( "\n\t" ) ) ;
		BOOST_CHECK_EQUAL( '1', creader.peek() ) ;

		BOOST_CHECK( creader.jump_to_first_of( "15" ) ) ;
		BOOST_CHECK_EQUAL( '1', creader.peek() ) ;

		BOOST_CHECK( creader.jump_to_first_of( "1", true ) ) ;
		BOOST_CHECK_EQUAL( '2', creader.peek() ) ;

		BOOST_CHECK( ! creader.jump_to_first_of( "\n\t1" ) ) ;
		BOOST_CHECK_EQUAL( '2', creader.peek() ) ;

		BOOST_CHECK( creader.jump_to_first_of( "\n\t5" ) ) ;
		BOOST_CHECK_EQUAL( '5', creader.peek() ) ;
	}


	/** Gets the text up to the end-of-line marker (specified by user). 
	If the delim is not found, gets the rest of the buffer.
	The pos ends up 1 beyond the characters in the line.
	*/
	BOOST_AUTO_TEST_CASE( Testgetline )
	{
		c_reader creader ;
		string foo = "123\n456" ;
		creader = foo.c_str() ;

		string line ;
		BOOST_CHECK( creader.getline( line ) ) ;
		BOOST_CHECK_EQUAL( CStringA( "123" ), line.c_str() ) ; // 123

		BOOST_CHECK( creader.getline( line ) ) ;
		BOOST_CHECK_EQUAL( CStringA( "456" ), line.c_str() ) ; // 456

		BOOST_CHECK( ! creader.getline( line ) ) ;
		BOOST_CHECK_EQUAL( CStringA( "" ), line.c_str() ) ; // NULL
	}

	/** Gets the text up to the end-of-line marker (specified by user). 
	If the delim is not found, gets the rest of the buffer.
	The pos ends up 1 beyond the characters in the line.
	Any text already in the string will be erased.
	*/
	BOOST_AUTO_TEST_CASE( TestgetlineStr )
	{
		c_reader creader ;
		string foo = "123\n456" ;
		creader = foo.c_str() ;

		string line ;
		BOOST_CHECK( creader.getline( line, "4\n" ) ) ;
		BOOST_CHECK_EQUAL( CStringA( "123" ), line.c_str() ) ; // 123

		BOOST_CHECK( creader.getline( line, "\n\t" ) ) ;
		BOOST_CHECK_EQUAL( CStringA( "456" ), line.c_str() ) ; // 456

		BOOST_CHECK( ! creader.getline( line, "4\n" ) ) ;
		BOOST_CHECK_EQUAL( CStringA( "" ), line.c_str() ) ; // NULL
	}


	/** Set the buffer back to the beginning. 
	*/
	BOOST_AUTO_TEST_CASE( Testrewind )
	{
		c_reader creader ;
		BOOST_CHECK( NULL == creader.get_buffer() ) ; // NULL

		string foo = "abc" ;
		creader.set_buffer( foo.c_str() ) ;

		creader.advance( 50 ) ;
		BOOST_CHECK( creader.empty() ) ;

		creader.rewind() ;

		BOOST_CHECK_EQUAL( CStringA( "abc" ), creader.get_buffer() ) ; // abc
		BOOST_CHECK_EQUAL( CStringA( "abc" ), creader.get_current_pos() ) ; // abc
	}


	/** Find the specified string. 
	If the string is not found, the pos does not move. 
	*/
	BOOST_AUTO_TEST_CASE( Testfind )
	{
		c_reader creader ;
		string foo = "123\n456" ;
		creader = foo.c_str() ;

		string line ;
		BOOST_CHECK( creader.find( "123" ) ) ;
		BOOST_CHECK( creader.find( "123", true ) ) ;
		BOOST_CHECK( creader.current_is( '\n' ) ) ; // \n
		BOOST_CHECK( ! creader.find( "123" ) ) ;

		BOOST_CHECK( creader.current_is( '\n' ) ) ; // \n
		BOOST_CHECK( ! creader.find( "123" ) ) ;
		BOOST_CHECK( creader.current_is( '\n' ) ) ; // \n

		BOOST_CHECK( creader.find( "456" ) ) ;
		BOOST_CHECK( creader.current_is( '4' ) ) ; // 4
		BOOST_CHECK( creader.find( "456", true ) ) ;

		BOOST_CHECK( creader.empty() ) ;
		BOOST_CHECK( ! creader.find( "123" ) ) ;
	}


	/** Jump the specified offset.
	For textstream_reader< char >, this is in bytes, rather than characters. 
	To jump forward the specified number of characters, use @ref advance( size_t ).
	There is no difference for wchar_t, except that unlike advance, 
	this function allows you to jump forwards or backwards.
	*/
	BOOST_AUTO_TEST_CASE( Testjump )
	{
		c_reader creader ;
		string foo = "123\n456" ;
		creader = foo.c_str() ;

		string line ;
		BOOST_CHECK( creader.find( "123", true ) ) ;

		BOOST_CHECK( creader.current_is( '\n' ) ) ; // \n
		BOOST_CHECK_EQUAL( 3u, creader.get_offset() ) ;

		creader.jump( -1 ) ;
		BOOST_CHECK( creader.current_is( '3' ) ) ; // 3
		BOOST_CHECK_EQUAL( 2u, creader.get_offset() ) ;

		creader.jump( -2 ) ;
		BOOST_CHECK( creader.current_is( '1' ) ) ; // 1

		creader.jump( 6 ) ;
		BOOST_CHECK( creader.current_is( '6' ) ) ; // 6
	}


	// for bookmarking positions in the buffer

	/** The current pos. 
	*/
	BOOST_AUTO_TEST_CASE( Testget_current_pos )
	{
		c_reader creader ;
		string foo = "abc" ;
		creader.set_buffer( foo.c_str() ) ;

		BOOST_CHECK_EQUAL( CStringA( "abc" ), creader.get_buffer() ) ; // abc
		BOOST_CHECK_EQUAL( CStringA( "abc" ), creader.get_current_pos() ) ; // abc

		creader.advance() ;
		BOOST_CHECK_EQUAL( CStringA( "abc" ), creader.get_buffer() ) ; // abc
		BOOST_CHECK_EQUAL( CStringA( "bc" ), creader.get_current_pos() ) ; // abc
	}


	/** Set the current pos. Usually with a bookmark that was previously saved. 
	*/
	BOOST_AUTO_TEST_CASE( Testset_pos )
	{
		c_reader creader ;
		string foo = "abc" ;
		creader.set_buffer( foo.c_str() ) ;

		BOOST_CHECK_EQUAL( CStringA( "abc" ), creader.get_buffer() ) ; // abc
		BOOST_CHECK_EQUAL( CStringA( "abc" ), creader.get_current_pos() ) ; // abc

		c_reader::bookmark_type bm = creader.get_current_pos() ;
		bm++ ;
		creader.set_pos( bm ) ;

		BOOST_CHECK_EQUAL( CStringA( "abc" ), creader.get_buffer() ) ; // abc
		BOOST_CHECK_EQUAL( CStringA( "bc" ), creader.get_current_pos() ) ; // abc
	}

	/** Get the offset of the specified bookmark_type from the beginning of the buffer. 
	A bit computation-intensive...
	*/
	BOOST_AUTO_TEST_CASE( Testget_offset_bm )
	{
		c_reader creader ;
		string foo = "abc" ;
		creader.set_buffer( foo.c_str() ) ;

		c_reader::bookmark_type bm = creader.get_current_pos() ;

		BOOST_CHECK_EQUAL( 0u, creader.get_offset( bm ) ) ; // abc 

		bm++ ;

		BOOST_CHECK_EQUAL( 1u, creader.get_offset( bm ) ) ; // abc 
	}


	/** Get the offset of the current pos from the specified offset from the beginning of the buffer. 
	For example, if you already know that the pos is at least 10, you can specify 10 so that
	it does not count back from the beginning. 
	A bit computation-intensive...
	*/
	BOOST_AUTO_TEST_CASE( Testget_offsetThrows )
	{
		c_reader creader ;
		string foo = "abc" ;
		creader.set_buffer( foo.c_str() ) ;

		BOOST_CHECK_THROW(creader.get_offset( 5 ), std::exception) ;
	}
	BOOST_AUTO_TEST_CASE( Testget_offset )
	{
		c_reader creader ;
		string foo = "abc" ;
		creader.set_buffer( foo.c_str() ) ;

		int expected = 0 ;
		int actual = creader.get_offset( (size_t)0 ) ;
		CStringA msg ;

		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;

		creader.advance() ;

		expected = 1 ;
		actual = creader.get_offset( (size_t)0 ) ;

		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = 1 ;
		actual = creader.get_offset( (size_t)1 ) ;

		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		creader.advance() ;

		expected = 2 ;
		actual = creader.get_offset( (size_t)0 ) ;

		msg.Format( "Expected %d but actual %d", expected, actual ) ;

		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = 2 ;
		actual = creader.get_offset( (size_t)1 ) ;

		msg.Format( "Expected %d but actual %d", expected, actual ) ;

	}

BOOST_AUTO_TEST_SUITE_END()

#endif
