#pragma once

#include "StringEx.h"

class CHrefTags
{
public:
	enum TAG_NAME_CODE
	{
		TAG_UNKNOWN = 0x01,
		TAG_REG_GLOSS_SET_SOURCE,
		TAG_REG_GLOSS_SET_TRANS ,
		TAG_REG_GLOSS_BACK,
		TAG_REG_GLOSS_ADD,
		TAG_SHOW_ABOUT,
		TAG_EDIT,
		TAG_DELETE,
		TAG_ADD_GLOSS,
		TAG_ADD_TO_GLOSS,
	};

	CHrefTags(void);
	~CHrefTags(void);
	TAG_NAME_CODE get_tag_code( const wstring tag_name ) ;
};
