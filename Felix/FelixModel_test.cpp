/*!
	@brief Unit tests for FelixModel class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "FelixModel.h"
#include "record_local.h"
#include "memory_local.h"

#ifdef UNIT_TEST
#include <boost/test/unit_test.hpp>

using namespace mem_engine ;

BOOST_AUTO_TEST_SUITE( TestFelixModel )


	void add_record(memory_pointer mem, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		mem->add_record(rec) ;
	}


	/************************************************************************/
	/* tests start here                                                     */
	/************************************************************************/	

	// Tests for CMainFrame

	BOOST_AUTO_TEST_CASE(test_constructor)
	{
		app_props::props_ptr props(new app_props::properties) ;

		BOOST_CHECK_NO_THROW(FelixModel(props, true)) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif 


