#pragma once

class WindowInterface
{
public:
	virtual long get_height() = 0 ;
	virtual void set_height(long height) = 0 ;

	virtual long get_width() = 0 ;
	virtual void set_width(long width) = 0 ;

	virtual long get_left() = 0 ;
	virtual void set_left(long left) = 0 ;

	virtual long get_top() = 0 ;
	virtual void set_top(long top) = 0 ;

};