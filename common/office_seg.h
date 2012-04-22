/*
	Represents an Office segment. 
	Used for generic queries about the current segment state, and to manipulate the segment selection.
*/
#pragma once

class OfficeSeg
{
public:

	virtual wchar_t get_last_char() = 0 ;
	virtual wstring get_seg_text() = 0 ;
};

typedef boost::shared_ptr<OfficeSeg> office_seg_ptr ;
