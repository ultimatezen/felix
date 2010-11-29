#pragma warning( disable : 4996 ) 

#include "StdAfx.h"
#include "Scintilla_encodings.h"
#include "StringEx.h"
#include "StringConversions.h"

struct sci_encoding_string_pair 
{
	const TCHAR* encoding_string ;
	UINT encoding ;
	const TCHAR* encoding_description ;
};

const sci_encoding_string_pair encodings[] =
{
	_T("DIN_66003"),		20106,	_T("IA5 (German)"),
	_T("NS_4551-1"),		20108,	_T("IA5 (Norwegian)"),
	_T("SEN_850200_B"),		20107,	_T("IA5 (Swedish)"),
	_T("_autodetect"),		50932,	_T("Japanese (Auto Select)"),
	_T("_autodetect_kr"),	50949,	_T("Korean (Auto Select)"),
	_T("big5"),				950,	_T("Chinese Traditional (Big5)"), 
	_T("csISO2022JP"),		50221,	_T("Japanese (JIS-Allow 1 byte Kana)"),
	_T("euc-kr"),			51949,	_T("Korean (EUC)"),
	_T("gb2312"),			936,	_T("Chinese Simplified (GB2312)"),
	_T("hz-gb-2312"),		52936,	_T("Chinese Simplified (HZ)"),
	_T("ibm852"),			852,	_T("Central European (DOS)"),
	_T("ibm866"),			866,	_T("Cyrillic Alphabet (DOS)"),
	_T("irv"),				20105,	_T("IA5 (IRV)"),
	_T("iso-2022-jp"),		50220,	_T("Japanese (JIS)"),
	_T("iso-2022-jp"),		50222,	_T("Japanese (JIS-Allow 1 byte Kana)"),
	_T("iso-2022-kr"),		50225,	_T("Korean (ISO)"),
	_T("iso-8859-1"),		1252,	_T("Western Alphabet"),
	_T("iso-8859-1"),		28591,	_T("Western Alphabet (ISO)"),
	_T("iso-8859-2"),		28592,	_T("Central European Alphabet (ISO)"),
	_T("iso-8859-3"),		28593,	_T("Latin 3 Alphabet (ISO)"),
	_T("iso-8859-4"),		28594,	_T("Baltic Alphabet (ISO)"),
	_T("iso-8859-5"),		28595,	_T("Cyrillic Alphabet (ISO)"),
	_T("iso-8859-6"),		28596,	_T("Arabic Alphabet (ISO)"),
	_T("iso-8859-7"),		28597,	_T("Greek Alphabet (ISO)"),
	_T("iso-8859-8"),		28598,	_T("Hebrew Alphabet (ISO)"),
	_T("koi8-r"),			20866,	_T("Cyrillic Alphabet (KOI8-R)"),
	_T("ks_c_5601"),		949,	_T("Korean"),
	_T("sjis"),				932,	_T("Japanese (Shift-JIS)"),
	_T("unicode"),			1200,	_T("Universal Alphabet"),
	_T("unicodeFEFF"),		1201,	_T("Universal Alphabet (Big-Endian)"),
	_T("utf-7"),			65000,	_T("Universal Alphabet (UTF-7)"),
	_T("utf-8"),			65001,	_T("Universal Alphabet (UTF-8)"),
	_T("windows-1250"),		1250,	_T("Central European Alphabet (Windows)"),
	_T("windows-1251"),		1251,	_T("Cyrillic Alphabet (Windows)"),
	_T("windows-1253"),		1253,	_T("Greek Alphabet (Windows)"),
	_T("windows-1254"),		1254,	_T("Turkish Alphabet"),
	_T("windows-1255"),		1255,	_T("Hebrew Alphabet (Windows)"),
	_T("windows-1256"),		1256,	_T("Arabic Alphabet (Windows)"),
	_T("windows-1257"),		1257,	_T("Baltic Alphabet (Windows)"),
	_T("windows-1258"),		1258,	_T("Vietnamese Alphabet (Windows)"),
	_T("windows-874"),		874,	_T("Thai (Windows)"),
	_T("x-euc"),			51932,	_T("Japanese (EUC)")
} ;

UINT sci_encoding_from_encoding_string( const string &encoding_str ) 
{
	for ( int i=0 ; i< sizeof( encodings ) / sizeof( sci_encoding_string_pair )	; ++i )
	{
		if ( boost::iequals( encoding_str, string2string( encodings[i].encoding_string ) ) )
		{
			return encodings[i].encoding ;
		}
	}

	if ( 
		boost::iequals( encoding_str, "shift_jis" ) || 
		boost::iequals( encoding_str, "s-jis" )     || 
		boost::iequals( encoding_str, "shift-jis" ) 
		)
	{
		return 932 ;
	}
	if ( 
		boost::iequals( encoding_str, "utf8" )  || 
		boost::iequals( encoding_str, "utf-8" ) || 
		boost::iequals( encoding_str, "utf_8" ) 
		)
	{
		return 932 ;
	}

	return CP_ACP ;
}

CString sci_description_from_encoding( UINT encoding ) 
{
	for ( int i=0 ; i< sizeof( encodings ) / sizeof( sci_encoding_string_pair )	; ++i )
	{
		if ( encodings[i].encoding == encoding ) 
		{
			return CString( encodings[i].encoding_description ) ;
		}
	}

	return CString( _T("Universal Alphabet (UTF-8)" ) ) ;
}

CString sci_encoding_string_from_encoding( UINT encoding ) 
{
	for ( int i=0 ; i< sizeof( encodings ) / sizeof( sci_encoding_string_pair )	; ++i )
	{
		if ( encodings[i].encoding == encoding ) 
		{
			return CString( encodings[i].encoding_string ) ;
		}
	}

	return CString( _T("utf-8" ) ) ;
}
