#include "stdafx.h"
#include ".\wordscreenupdatelock.h"
#include "logging.h"

CWordScreenUpdateLock::CWordScreenUpdateLock( WordApplication &word_app ) :
	m_app( word_app ) 
{
	try
	{
		m_app.set_screen_updating_off() ;
	}
	catch (...)
	{
		logging::log_error("Failed to turn off screen updating in CWordScreenUpdateLock CTOR") ;		
	}
}

CWordScreenUpdateLock::~CWordScreenUpdateLock(void)
{
	try
	{
		m_app.set_screen_updating_on() ;
	}
	catch (...)
	{
		logging::log_error("Failed to turn screen updating on in CWordScreenUpdateLock destructor") ;		
	}
}
