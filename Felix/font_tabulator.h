/*!
	@file font_tabulator.h
	@brief interface for the font_tabulator class.
	@date 2005/06/25
	Time: 14:47:34
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

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
	bool tabulate_fonts( const wstring text );

private:
	font_set m_font_set ;

};

