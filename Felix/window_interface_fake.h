#pragma once

#include "window_interface.h"

class WindowInterfaceFake : public WindowInterface
{
public:
	DECLARE_SENSING_VAR ;

	long m_height, m_width, m_left, m_top ;

	WindowInterfaceFake() : m_height(0), m_width(0), m_left(0), m_top(0)
	{

	}

	long get_height() 
	{
		SENSE("get_height") ;
		return m_height ;
	}
	void set_height(long height) 
	{
		SENSE("set_height") ;
		SENSE(int2string(height)) ;
		m_height = height ;

	}
	long get_width()
	{
		SENSE("get_width") ;
		return m_width ;
	}
	void set_width(long width)
	{
		SENSE("set_width") ;
		SENSE(int2string(width)) ;
		m_width = width ;

	}

	long get_left() 
	{
		SENSE("get_left") ;
		return m_left ;
	}
	void set_left(long left)
	{
		SENSE("set_left") ;
		SENSE(int2string(left)) ;
		m_left = left ;

	}

	long get_top() 
	{
		SENSE("get_top") ;
		return m_top ;
	}
	void set_top(long top) 
	{
		SENSE("set_top") ;
		SENSE(int2string(top)) ;
		m_top = top ;
	}

	void raise()
	{
		SENSE("raise") ;
	}

};