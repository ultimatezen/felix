/*!
	@file Scintilla_encodings.h
	@brief various encoding <==> string conversion functions.
	@date 2005/06/25
	Time: 15:30:42
	@author Ryan Ginstrom
 */

#pragma once

#include "Windows.h"
#include "atlstr.h"
#include <string>

using std::string ;

UINT sci_encoding_from_encoding_string( const string &encoding_str ) ;


