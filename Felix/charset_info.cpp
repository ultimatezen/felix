// charset_info.cpp: implementation of the charset_info class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "charset_info.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

charset_info::charset_info()
{
	codes.insert( code_pair(  "AF", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "SQ", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "AR", 1256 ) ) ;	//	Arabic (Windows)
	codes.insert( code_pair(  "EU", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "BG", 1251 ) ) ;	//	Cyrillic (Windows)
	codes.insert( code_pair(  "BE", 1251 ) ) ;	//	Cyrillic (Windows)
	codes.insert( code_pair(  "CA", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "ZH-TW", 950 ) ) ;	//	Chinese Traditional (Big5)
	codes.insert( code_pair(  "ZH-CN", 936 ) ) ;	//	Chinese Simplified (GB2312)
	codes.insert( code_pair(  "ZH", 950 ) ) ;	//	Chinese Traditional (Big5)
	codes.insert( code_pair(  "HR", 1250 ) ) ;	//	central European
	codes.insert( code_pair(  "CS", 1250 ) ) ;	//	central European
	codes.insert( code_pair(  "DA", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "NL", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "EN", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "ET", 1257 ) ) ;	//	Baltic (Windows) ) ;	
	codes.insert( code_pair(  "FO", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "FI", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "FR", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "GA", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "DE", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "EL", 1253 ) ) ;	//	Greek (Windows) ) ;
	codes.insert( code_pair(  "IW",  1255 ) ) ;	//	Hebrew (Windows) ) ;
	codes.insert( code_pair(  "HU", 1250 ) ) ;	//	Central European
	codes.insert( code_pair(  "IS", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "IN",  1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "Gaelic", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "IT", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "JA", 932 ) ) ;	// sjis
	codes.insert( code_pair(  "KA", 1258 ) ) ;	// Vietnamese
	codes.insert( code_pair(  "KO", 1361 ) ) ;	//	Korean (Johab)
	codes.insert( code_pair(  "LO", 874 ) ) ;	//	Thai (Windows)
	codes.insert( code_pair(  "LV", 1257 ) ) ;	//	Baltic (Windows)	
	codes.insert( code_pair(  "LT", 1257 ) ) ;	//	Baltic (Windows)	
	codes.insert( code_pair(  "MK", 1251 ) ) ;	//	Cyrillic (Windows)
	codes.insert( code_pair(  "MT", 28593  ) ) ;	// iso-8859-3  Latin 3 Alphabet (ISO) 
	codes.insert( code_pair(  "MI", 1257 ) ) ;
	codes.insert( code_pair(  "NO", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "PL", 1250 ) ) ;	//	central European
	codes.insert( code_pair(  "PT", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "PA", 57011  ) ) ;	// punjabi
	codes.insert( code_pair(  "RM", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "RO", 1250 ) ) ;	//	central European
	codes.insert( code_pair(  "RU", 1251 ) ) ;	//	Cyrillic Alphabet (Windows) 
	codes.insert( code_pair(  "SH", 1250 ) ) ;	//	central European
	codes.insert( code_pair(  "SK", 1250 ) ) ;	//	central European
	codes.insert( code_pair(  "SL", 1250 ) ) ;	//	central European
	codes.insert( code_pair(  "SO", 1250 ) ) ;	//	central European
	codes.insert( code_pair(  "ES", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "SV", 1252 ) ) ;	// iso-8859-1  Western Alphabet 
	codes.insert( code_pair(  "TH", 874 ) ) ;	//	Thai (Windows)
	codes.insert( code_pair(  "TS",  28593 ) ) ;
	codes.insert( code_pair(  "TR", 1254 ) ) ;
	codes.insert( code_pair(  "UK", 1251 ) ) ;	//	Cyrillic Alphabet (Windows) 
	codes.insert( code_pair(  "UR", 1256 ) ) ;	//	Arabic (Windows) ) ;
	codes.insert( code_pair(  "VI", 1258 ) ) ;	//	Vietnamese (Windows)      163
}

charset_info::~charset_info()
{
}


unsigned int charset_info::cp_from_charset( const BYTE charset ) const
{
	switch( charset )
	{
		case ANSI_CHARSET: 
			return  CP_ACP ;
		case DEFAULT_CHARSET: 
			return  CP_ACP ;
		case SYMBOL_CHARSET	: 
			return  CP_SYMBOL ;	// SYMBOL translations
		case SHIFTJIS_CHARSET: 
			return  932    ;		//	Japanese (Shift-JIS;
		case HANGEUL_CHARSET: 
			return  949	 ;		//	Korean         129
		case GB2312_CHARSET: 
			return  936	 ;		//	Chinese Simplified (GB2312;
		case CHINESEBIG5_CHARSET: 
			return  950	 ;		//	Chinese Traditional (Big5;     136
		case OEM_CHARSET: 
			return  CP_OEMCP ;	//	default to OEM  code page
		case JOHAB_CHARSET: 
			return  1361	 ;		//	Korean (Johab;
		case HEBREW_CHARSET: 
			return  1255	 ;		//	Hebrew (Windows;
		case ARABIC_CHARSET: 
			return  1256	 ;		//	Arabic (Windows;
		case GREEK_CHARSET: 
			return  1253	 ;		//	Greek (Windows;
		case TURKISH_CHARSET: 
			return  1254	 ;		//	Turkish (Windows;
		case VIETNAMESE_CHARSET: 
			return  1258	 ;		//	Vietnamese (Windows;      163
		case THAI_CHARSET: 
			return  874	 ;		//	Thai (Windows;
		case EASTEUROPE_CHARSET: 
			return  1250	 ;		//	central European
		case RUSSIAN_CHARSET: 
			return  1251	 ;		//	Cyrillic Alphabet (Windows; 
		case MAC_CHARSET: 
			return  CP_ACP ;		//	???
		case BALTIC_CHARSET: 
			return  1257	 ;		//	Baltic (Windows;
		default:
			ATLASSERT( FALSE && "Unknown character set!!" ) ;
	}
	return CP_ACP ;
}


// Function name	: charset_info::cp_from_lang_str
// Description	    : 
// Return type		: unsigned int 
// Argument         : string lang
unsigned int charset_info::cp_from_lang_str( const string lang ) const
{
	// first element is the string, second is the code
	foreach(code_pair this_pair, codes)
	{
		const string code_string = this_pair.first ;
		if ( boost::iequals( code_string, lang ) == true ) 
		{
			return this_pair.second ;
		}
	}
	foreach(code_pair this_pair, codes)
	{
		const string code_string = this_pair.first ;
		const string for_comp_chunk = str::left( lang, code_string.size() ) ;
		if ( boost::iequals( lang, code_string ) == true ) 
		{
			return this_pair.second ;
		}
	}

	return CP_ACP ;
}
