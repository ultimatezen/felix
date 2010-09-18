#include "stdafx.h"
#include "key_mapper_word.h"
#include "key_target_word_fake.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( KeyMapperTest )

BOOST_AUTO_TEST_CASE(test_nonexistent_command)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(! mapper.map_command("this command does not exist")) ;
}

BOOST_AUTO_TEST_CASE(Test_CMD_EXTEND_LOOKUP)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_EXTEND_LOOKUP)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnExtendLookupAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_LOOK_UP_NEXT_TRANS)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_LOOK_UP_NEXT_TRANS)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnLookupNextTransAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_CORRECT_TRANS)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_CORRECT_TRANS)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnCorrectTransAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_LOOK_UP_TRANS)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_LOOK_UP_TRANS)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnLookupTransAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_TRANS_CONCORDANCE)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_TRANS_CONCORDANCE)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnTransConcordanceAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_LOOK_UP_NEXT)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_LOOK_UP_NEXT)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnLookupNextAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_SET)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_SET)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnSetAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_GET)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_GET)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnGetAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_PREVIOUS)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_PREVIOUS)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnPrevAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_NEXT)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_NEXT)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnNextAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_GET_AND_NEXT)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_GET_AND_NEXT)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnGetAndNextAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_SET_AND_NEXT)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_SET_AND_NEXT)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnSetAndNextAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_LOOK_UP)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_LOOK_UP)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnLookupAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_AUTO_TRANS_TO_FUZZY)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_AUTO_TRANS_TO_FUZZY)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnAutoTransFuzzyAction", target.m_sensing_variable[0]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_DELETE)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_DELETE)) ;
	BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnDeleteAction", target.m_sensing_variable[0]) ;
}
/************************************************************************/
/* glossary entries                                                     */
/************************************************************************/
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_0)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_0)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("0", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_1)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_1)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("1", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_2)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_2)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("2", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_3)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_3)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("3", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_4)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_4)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("4", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_5)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_5)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("5", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_6)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_6)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("6", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_7)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_7)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("7", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_8)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_8)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("8", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_CASE(Test_CMD_ENTRY_9)
{
	KeyTargetWordFake target ;
	KeyMapperWord mapper ;
	mapper.m_target = &target ;

	BOOST_CHECK(mapper.map_command(CMD_ENTRY_9)) ;
	BOOST_CHECK_EQUAL(2u, target.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL("OnEntryAction", target.m_sensing_variable[0]) ;
	BOOST_CHECK_EQUAL("9", target.m_sensing_variable[1]) ;
}
BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
