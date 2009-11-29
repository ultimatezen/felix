/*!
	@file font_tabulator.h
	@brief interface for the font_tabulator class.
	@date 2005/06/25
	Time: 14:47:34
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once


#include "StringEx.h"
#include <set>
#include "ArmadilloStuff.h"

/**
	@class font_tabulator  
	@brief Tabulates fonts in HTML text into a set.
 */
class font_tabulator  
{
public:
	typedef std::set< wstring > font_set ;
	font_set &get_font_set() { return m_font_set ; }

	// Function name	: tabulate_fonts
	// Description	    : 
	// Return type		: bool 
	bool tabulate_fonts( const wstring &text )
	{
		textstream_reader< wchar_t > reader ;
		reader.set_buffer( text.c_str() ) ;

		while ( reader.find( L"<font", true ) ) 
		{
			wstring tag ;
			reader.getline( tag, L'>', true ) ;
			textstream_reader< wchar_t > tag_reader ;
			tag_reader.set_buffer( tag.c_str() ) ;
			if ( tag_reader.find( L"face" ), true ) 
			{
				tag_reader.jump_to_first_of( L'"', true ) ;
				wstring face_name ;
				tag_reader.getline( face_name, L'"', true ) ;
				m_font_set.insert( face_name ) ;
			}
		}

		return true ;
	}

private:
	font_set m_font_set ;

};

