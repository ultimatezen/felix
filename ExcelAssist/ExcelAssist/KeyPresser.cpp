/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include "StdAfx.h"
#include "KeyPresser.h"

/*!
 * \brief
 * Write brief comment for CKeyPresser here.
 * 
 * \param key
 * Description of parameter key.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for CKeyPresser here.
 * 
 * \remarks
 * Write remarks for CKeyPresser here.
 * 
 * \see
 * Separate items with the '|' character.
 */
CKeyPresser::CKeyPresser(BYTE key) :
 mKey( key )
{
	keybd_event(mKey, 0, 0, 0);
}

/*!
 * \brief
 * Write brief comment for ~CKeyPresser here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for ~CKeyPresser here.
 * 
 * \remarks
 * Write remarks for ~CKeyPresser here.
 * 
 * \see
 * Separate items with the '|' character.
 */
CKeyPresser::~CKeyPresser(void)
{
	keybd_event(mKey, 0, KEYEVENTF_KEYUP, 0);
}
