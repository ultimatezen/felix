#pragma once

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

private:
	void doSleep();
	UINT mSleepTime;
};
