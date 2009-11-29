#include "StdAfx.h"
#include ".\hreftags.h"

CHrefTags::CHrefTags(void)
{
}

CHrefTags::~CHrefTags(void)
{
}

CHrefTags::TAG_NAME_CODE CHrefTags::get_tag_code( const wstring &tag_name )
{
	// register glossary entries page
	if( str::equal_nocase( tag_name,  L"reg_gloss_set_source") )
	{
		return TAG_REG_GLOSS_SET_SOURCE ;
	}
	if( str::equal_nocase( tag_name,  L"reg_gloss_set_trans") )
	{
		return TAG_REG_GLOSS_SET_TRANS ;
	}
	if( str::equal_nocase( tag_name,  L"reg_gloss_back") )
	{
		return TAG_REG_GLOSS_BACK ;
	}
	if( str::equal_nocase( tag_name,  L"reg_gloss_add") )
	{
		return TAG_REG_GLOSS_ADD ;
	}
	// startup page
	if( str::equal_nocase( tag_name,  L"show_about") )
	{
		return TAG_SHOW_ABOUT ;
	}
	
	// added trans page
	if( str::equal_nocase( tag_name,  L"edit") )
	{
		return TAG_EDIT ;
	}
	if( str::equal_nocase( tag_name,  L"delete") )
	{
		return TAG_DELETE ;
	}
	if( str::equal_nocase( tag_name,  L"add_gloss") )
	{
		return TAG_ADD_GLOSS ;
	}
	if( str::equal_nocase( tag_name,  L"add_to_gloss") )
	{
		return TAG_ADD_TO_GLOSS ;
	}
	

	return TAG_UNKNOWN ;
}





