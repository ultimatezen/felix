#include "mainframe_tester.h"
#include "StdAfx.h"
#include "mainfrm.h"

#ifdef _DEBUG


void test_create()
{
	ATLTRACE( "." ) ;
	CMainFrame &wndMain = app::get_app() ;

	ATLASSERT ( wndMain.CreateEx() != NULL) ;
	ATLASSERT ( wndMain.IsWindow() ) ; 
	ATLASSERT ( wndMain.DestroyWindow() ) ;
	ATLASSERT ( ! wndMain.IsWindow() ) ; 

}

void test_register_gloss()
{
	CMainFrame main_frame ;
	main_frame.lookup( L"query" ) ;
	ATLASSERT ( main_frame.get_num_matches() == 0 ) ;
	ATLASSERT ( main_frame.get_glossary_window()->num_matches() == 0 ) ; 
	
	main_frame.register_trans_to_glossary( L"trans" ) ;
	main_frame.lookup( L"query" ) ;
	ATLASSERT ( main_frame.get_num_matches() == 0 ) ;
	ATLASSERT ( main_frame.get_glossary_window()->num_matches() == 1 ) ; 
	ATLASSERT ( L"trans" == main_frame.get_glossary_entry( 0 ) ) ; 
	
	main_frame.get_glossary_window()->clear_memory() ; 
	main_frame.lookup( L"query" ) ;
	ATLASSERT ( main_frame.get_num_matches() == 0 ) ;
	ATLASSERT ( main_frame.get_glossary_window()->num_matches() == 0 ) ; 

	record_pointer rec ;
	rec->set_rich_source( wstring(L"query") ) ;
	rec->set_rich_trans( wstring(L"trans") ) ;
	main_frame.get_glossary_window()->add_record( rec ) ;

	ATLASSERT ( main_frame.get_glossary_window()->num_matches() == 1 ) ; 
	ATLASSERT ( L"trans" == main_frame.get_glossary_entry( 0 ) ) ; 

}

void test_mainframe()
{
	// set up
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame main_frame ;
	main_frame.OnUnitTest() ;

	/************************************************************************/
	/* Our tests                                                            */
	/************************************************************************/

	test_register_gloss() ;

	// do this last due to the PostQuitMessage when the window is destroyed
	test_create() ;
	//////////////////////////////////////////////////////////////////////////


	// tear down
	_Module.RemoveMessageLoop();

}

#endif // #ifdef _DEBUG
