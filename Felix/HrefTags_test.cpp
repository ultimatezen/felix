#include "StdAfx.h"
#include ".\hreftags.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG
 
namespace easyunit
{
	TEST( CHrefTagsTestCase, TestGetTagNameRegGloss )
	{
		CHrefTags tags ;

		ASSERT_EQUALS( CHrefTags::TAG_REG_GLOSS_SET_SOURCE, tags.get_tag_code(L"reg_gloss_set_source") ) ;
		ASSERT_EQUALS( CHrefTags::TAG_REG_GLOSS_SET_TRANS, tags.get_tag_code(L"reg_gloss_set_trans") ) ;
		ASSERT_EQUALS( CHrefTags::TAG_REG_GLOSS_BACK, tags.get_tag_code(L"reg_gloss_back") ) ;
		ASSERT_EQUALS( CHrefTags::TAG_REG_GLOSS_ADD, tags.get_tag_code(L"reg_gloss_add") ) ;

		ASSERT_EQUALS( CHrefTags::TAG_REG_GLOSS_SET_SOURCE, tags.get_tag_code(L"REG_GLOSS_SET_SOURCE") ) ;
		ASSERT_EQUALS( CHrefTags::TAG_REG_GLOSS_SET_TRANS, tags.get_tag_code(L"REG_gloss_set_trans") ) ;
		ASSERT_EQUALS( CHrefTags::TAG_REG_GLOSS_BACK, tags.get_tag_code(L"reg_gloss_BACK") ) ;
		ASSERT_EQUALS( CHrefTags::TAG_REG_GLOSS_ADD, tags.get_tag_code(L"reg_GLOSS_ADD") ) ;

		ASSERT_EQUALS( CHrefTags::TAG_SHOW_ABOUT, tags.get_tag_code(L"show_about") ) ;

		ASSERT_EQUALS( CHrefTags::TAG_EDIT, tags.get_tag_code(L"edit") ) ;
		ASSERT_EQUALS( CHrefTags::TAG_DELETE, tags.get_tag_code(L"delete") ) ;
		ASSERT_EQUALS( CHrefTags::TAG_ADD_GLOSS, tags.get_tag_code(L"add_gloss") ) ;
		ASSERT_EQUALS( CHrefTags::TAG_ADD_TO_GLOSS, tags.get_tag_code(L"add_to_gloss") ) ;

		ASSERT_EQUALS( CHrefTags::TAG_UNKNOWN, tags.get_tag_code(L"nonexistent_tag") ) ;

	}

}


#endif 
