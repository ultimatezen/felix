#include "StdAfx.h"
#include "view_factory.h"
#include "view_interface_ta.h"
#include "view_interface_mock.h"

view_interface* create_view( memory_type mem_type )
{
	mem_type ;

#ifdef _TEST_RIG
	return static_cast< view_interface* >( new view_interface_mock() ) ;
#else
	return static_cast< view_interface* >( new view_interface_ta() ) ;
#endif
}