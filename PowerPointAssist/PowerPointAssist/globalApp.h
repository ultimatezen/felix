#pragma once

#include "PowerPointImports.h"

class globalApp
{
public:
	static void set( PowerPoint::_ApplicationPtr app ) ;
	static PowerPoint::_ApplicationPtr get() ;
};
