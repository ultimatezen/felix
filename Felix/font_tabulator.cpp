#include "stdafx.h"
#include "font_tabulator.h"

bool font_tabulator::tabulate_fonts( const wstring text )
{
	textstream_reader< wchar_t > reader ;
	reader.set_buffer( text.c_str() ) ;

	while ( reader.find( L"<font", true ) ) 
	{
		const wstring tag = reader.getline(L'>', true) ;
		textstream_reader< wchar_t > tag_reader ;
		tag_reader.set_buffer( tag.c_str() ) ;
		if ( tag_reader.find( L"face", true) ) 
		{
			tag_reader.jump_to_first_of( L'"', true ) ;
			const wstring face_name = tag_reader.getline(L'"', true ) ;
			m_font_set.insert( face_name ) ;
		}
	}

	return true ;
}