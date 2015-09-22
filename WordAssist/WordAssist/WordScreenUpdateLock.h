#pragma once

#include "WordAutomationObject.h"

class CWordScreenUpdateLock
{
	WordApplication	m_app ;
public:
	CWordScreenUpdateLock( WordApplication &word_app );
	~CWordScreenUpdateLock(void);
};
