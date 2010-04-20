#include "StdAfx.h"
#include ".\hreftags.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CHrefTagsTestCase )


	BOOST_AUTO_TEST_CASE(TestGetTagNameRegGloss )
	{
		CHrefTags tags ;

		BOOST_CHECK_EQUAL( CHrefTags::TAG_REG_GLOSS_SET_SOURCE, tags.get_tag_code(L"reg_gloss_set_source") ) ;
		BOOST_CHECK_EQUAL( CHrefTags::TAG_REG_GLOSS_SET_TRANS, tags.get_tag_code(L"reg_gloss_set_trans") ) ;
		BOOST_CHECK_EQUAL( CHrefTags::TAG_REG_GLOSS_BACK, tags.get_tag_code(L"reg_gloss_back") ) ;
		BOOST_CHECK_EQUAL( CHrefTags::TAG_REG_GLOSS_ADD, tags.get_tag_code(L"reg_gloss_add") ) ;

		BOOST_CHECK_EQUAL( CHrefTags::TAG_REG_GLOSS_SET_SOURCE, tags.get_tag_code(L"REG_GLOSS_SET_SOURCE") ) ;
		BOOST_CHECK_EQUAL( CHrefTags::TAG_REG_GLOSS_SET_TRANS, tags.get_tag_code(L"REG_gloss_set_trans") ) ;
		BOOST_CHECK_EQUAL( CHrefTags::TAG_REG_GLOSS_BACK, tags.get_tag_code(L"reg_gloss_BACK") ) ;
		BOOST_CHECK_EQUAL( CHrefTags::TAG_REG_GLOSS_ADD, tags.get_tag_code(L"reg_GLOSS_ADD") ) ;

		BOOST_CHECK_EQUAL( CHrefTags::TAG_SHOW_ABOUT, tags.get_tag_code(L"show_about") ) ;

		BOOST_CHECK_EQUAL( CHrefTags::TAG_EDIT, tags.get_tag_code(L"edit") ) ;
		BOOST_CHECK_EQUAL( CHrefTags::TAG_DELETE, tags.get_tag_code(L"delete") ) ;
		BOOST_CHECK_EQUAL( CHrefTags::TAG_ADD_GLOSS, tags.get_tag_code(L"add_gloss") ) ;
		BOOST_CHECK_EQUAL( CHrefTags::TAG_ADD_TO_GLOSS, tags.get_tag_code(L"add_to_gloss") ) ;

		BOOST_CHECK_EQUAL( CHrefTags::TAG_UNKNOWN, tags.get_tag_code(L"nonexistent_tag") ) ;

	}

BOOST_AUTO_TEST_SUITE_END()


#endif 
