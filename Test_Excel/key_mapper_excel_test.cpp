#include "stdafx.h"
#include "key_mapper_excel.h"
#include "key_target_excel_fake.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( KeyMapperTest )

BOOST_AUTO_TEST_CASE(Test_test_nonexistent_command)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(! mapper.map_command("this command does not exist")) ;
}

BOOST_AUTO_TEST_CASE(Test_CmdLookupNext)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdLookupNext)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnLookupNext", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdSet)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdSet)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnSet", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdGet)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdGet)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnGet", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdPrev)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdPrev)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnPrev", target.m_sensing_variable[0]) ;
}
// --
BOOST_AUTO_TEST_CASE(Test_CmdNext)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdNext)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnNext", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdGetAndNext)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdGetAndNext)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnGetAndNext", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdGetAndNext_lower)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(boost::to_lower_copy(CmdGetAndNext))) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnGetAndNext", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdSetAndNext)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdSetAndNext)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnSetAndNext", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdLookup)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdLookup)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnLookup", target.m_sensing_variable[0]) ;
}
// --
BOOST_AUTO_TEST_CASE(Test_CmdAutoTransSel)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdAutoTransSel)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnAutoTransSel", target.m_sensing_variable[0]) ;
}

BOOST_AUTO_TEST_CASE(Test_CmdEntry0Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry0Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("0", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry0Action_upper)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(boost::to_upper_copy(CmdEntry0Action))) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("0", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry1Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry1Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("1", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry2Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry2Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("2", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry3Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry3Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("3", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry4Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry4Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("4", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry5Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry5Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("5", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry6Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry6Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("6", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry7Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry7Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("7", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry8Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry8Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("8", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CmdEntry9Action)
{
	KeyTargetExcelFake target ;
	KeyMapperExcel mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CmdEntry9Action)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntry", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("9", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
