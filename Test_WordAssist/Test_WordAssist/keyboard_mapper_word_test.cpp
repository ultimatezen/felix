#include "stdafx.h"
#include "key_mapper_word.h"
#include "key_target_word_fake.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( KeyMapperTest )

	BOOST_AUTO_TEST_CASE(Test_test_nonexistent_command)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(! mapper.map_command("this command does not exist")) ;
	}

	BOOST_AUTO_TEST_CASE(Test_CmdAutoTransAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdAutoTransAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnAutoTransAction", target.m_sensing_variable[0]) ;
	}
	BOOST_AUTO_TEST_CASE(Test_CmdAutoTransFuzzyAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdAutoTransFuzzyAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnAutoTransFuzzyAction", target.m_sensing_variable[0]) ;
	}
	BOOST_AUTO_TEST_CASE(Test_CmdGetAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdGetAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnGetAction", target.m_sensing_variable[0]) ;
	}
	BOOST_AUTO_TEST_CASE(Test_CmdSetAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdSetAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnSetAction", target.m_sensing_variable[0]) ;
	}
	// --
	BOOST_AUTO_TEST_CASE(Test_CmdGetAndNextAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdGetAndNextAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnGetAndNextAction", target.m_sensing_variable[0]) ;
	}
	BOOST_AUTO_TEST_CASE(Test_CmdSetAndNextAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdSetAndNextAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnSetAndNextAction", target.m_sensing_variable[0]) ;
	}
	BOOST_AUTO_TEST_CASE(Test_CmdLookupAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdLookupAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnLookupAction", target.m_sensing_variable[0]) ;
	}
	BOOST_AUTO_TEST_CASE(Test_CmdLookupNextAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdLookupNextAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnLookupNextAction", target.m_sensing_variable[0]) ;
	}
	// --
	BOOST_AUTO_TEST_CASE(Test_CmdConcordanceAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdConcordanceAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnConcordanceAction", target.m_sensing_variable[0]) ;
	}

	BOOST_AUTO_TEST_CASE(Test_CmdExtendLookupAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdExtendLookupAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnExtendLookupAction", target.m_sensing_variable[0]) ;
	}

	BOOST_AUTO_TEST_CASE(Test_CmdToMaruAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdToMaruAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnToMaruAction", target.m_sensing_variable[0]) ;
	}


	// register a glossary entry
	BOOST_AUTO_TEST_CASE(Test_CmdRegisterGlossAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdRegisterGlossAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnRegisterGlossAction", target.m_sensing_variable[0]) ;
	}


	// delete entry
	BOOST_AUTO_TEST_CASE(Test_CmdDeleteAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdDeleteAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnDeleteAction", target.m_sensing_variable[0]) ;
	}


	// navigate through matches
	BOOST_AUTO_TEST_CASE(Test_CmdNextAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdNextAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnNextAction", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdPrevAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdPrevAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnPrevAction", target.m_sensing_variable[0]) ;
	}


	// get glossary entries
	BOOST_AUTO_TEST_CASE(Test_CmdEntry0Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry0Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry0Action", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdEntry1Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry1Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry1Action", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdEntry2Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry2Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry2Action", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdEntry3Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry3Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry3Action", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdEntry4Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry4Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry4Action", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdEntry5Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry5Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry5Action", target.m_sensing_variable[0]) ;
	}

	BOOST_AUTO_TEST_CASE(Test_CmdEntry6Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry6Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry6Action", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdEntry7Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry7Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry7Action", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdEntry8Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry8Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry8Action", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdEntry9Action)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdEntry9Action)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnEntry9Action", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdGlossNAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdGlossNAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnGlossNAction", target.m_sensing_variable[0]) ;
	}


	// look up translation
	BOOST_AUTO_TEST_CASE(Test_CmdLookupTransAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdLookupTransAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnLookupTransAction", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdLookupNextTransAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdLookupNextTransAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnLookupNextTransAction", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdTransConcordanceAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdTransConcordanceAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnTransConcordanceAction", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdExtendTransLookupAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdExtendTransLookupAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnExtendTransLookupAction", target.m_sensing_variable[0]) ;
	}



	BOOST_AUTO_TEST_CASE(Test_CmdCorrectAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdCorrectAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnCorrectAction", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdCorrectAndNextAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdCorrectAndNextAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnCorrectAndNextAction", target.m_sensing_variable[0]) ;
	}



	BOOST_AUTO_TEST_CASE(Test_CmdRestoreAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdRestoreAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnRestoreAction", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdRestoreAndNextAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdRestoreAndNextAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnRestoreAndNextAction", target.m_sensing_variable[0]) ;
	}



	BOOST_AUTO_TEST_CASE(Test_CmdSaveMemoryAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdSaveMemoryAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnSaveMemoryAction", target.m_sensing_variable[0]) ;
	}


	BOOST_AUTO_TEST_CASE(Test_CmdShiftStateAction)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdShiftStateAction)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnShiftStateAction", target.m_sensing_variable[0]) ;
	}

	BOOST_AUTO_TEST_CASE(Test_CmdAnalyze)
	{
		KeyTargetWordFake target ;
		KeyMapperWord mapper ;
		mapper.m_target = &target ;

		BOOST_CHECK(mapper.map_command(CmdAnalyze)) ;
		BOOST_CHECK_EQUAL(1u, target.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnAnalyze", target.m_sensing_variable[0]) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
