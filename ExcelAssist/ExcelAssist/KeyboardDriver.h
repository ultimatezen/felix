/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

/*!
 * \brief
 * Write brief comment for CKeyboardDriver here.
 * 
 * Write detailed description for CKeyboardDriver here.
 * 
 * \remarks
 * Write remarks for CKeyboardDriver here.
 * 
 * \see
 * Separate items with the '|' character.
 */
class CKeyboardDriver
{
public:
	CKeyboardDriver(void);
public:
	~CKeyboardDriver(void);

	void clearAltKey();
	void clearControlKey();
	void doShiftLeft();
	void doShiftRight();
	void doRight();
	void doLeft();
	void doReturn(void);
	void doTab(void);

private:
	void doSleep();
	/*!
	 * \brief
	 * Write brief comment for mSleepTime here.
	 * 
	 * Write detailed description for mSleepTime here.
	 * 
	 * \remarks
	 * Write remarks for mSleepTime here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	UINT mSleepTime;
};
