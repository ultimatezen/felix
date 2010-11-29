/*!
	@file tag_stripper.h
	@brief Interface for symbol_map struct
	@date 2005/06/25
	Time: 14:59:03
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

#include "textstream_reader.h"
#include <boost/regex.hpp>

#pragma message( "   Processing tag_stripper.h...")
/**
	@struct symbol_map
	@brief Converts HTML symbol tags to Unicode characters.
 */
struct symbol_map
{
	std::map< wstring, wchar_t > m_symbols ;

	bool exists( const wstring &key ) const
	{
		return ( m_symbols.end() != m_symbols.find( key ) ) ;
	}
	const wchar_t get_val( const wstring &key ) const
	{
		std::map< wstring, wchar_t >::const_iterator pos = m_symbols.find(key) ;
		if (pos != m_symbols.end())
		{
			return pos->second ;
		}
		return wchar_t(0) ;
	}
	bool erase(const wstring key)
	{
		if (! exists(key))
		{
			return false ;
		}
		m_symbols.erase(m_symbols.find(key)) ;
		return true ;
	}
	symbol_map()
	{
		m_symbols[L"lt"]   // # LESS THAN
			  = wchar_t(L'<') ;
		m_symbols[L"gt"]   // # GREATER THAN
			  = wchar_t(L'>') ;
		m_symbols[L"amp"]   // # AMPERSAND
			  = wchar_t(L'&') ;
		m_symbols[L"nbsp"] // non-breaking space
				= wchar_t( ' ' ) ; 
		m_symbols[L"quot"]	// double quotation mark
				= wchar_t(L'"') ;
		m_symbols[L"apos"]	// single quotation mark (apostrophe)
				= wchar_t(L'\'') ;

		m_symbols[L"iexcl"] // inverted exclamation
				= wchar_t( 0x00A1 ) ; 
		m_symbols[L"cent"] // cent mark
				= wchar_t( 0x00A2 ) ; 
		m_symbols[L"pound"] // British pound
				= wchar_t( 0x00A3 ) ; 
		m_symbols[L"curren"] // general currency
				= wchar_t( 0x00A4 ) ; 
		m_symbols[L"yen"] // yen
				= wchar_t( 0x00A5 ) ; 

		m_symbols[L"brvbar"] = wchar_t( 0x00A6 ) ; 
		m_symbols[L"ben"] // broken (vertical) bar
				= wchar_t( 0x00A6 ) ; 
		m_symbols[L"sect"] 
				= wchar_t( 0x00A7 ) ; 
		m_symbols[L"Section"] // section sign
				= wchar_t( 0x00A7 ) ; 
			// no 00A8
		m_symbols[L"copy"] 
			= wchar_t( 0x00A9 ) ; 
		m_symbols[L"copyright"] // copyright
				= wchar_t( 0x00A9 ) ; 
		m_symbols[L"ordf"] // ordinal indicator, feminine 
				= wchar_t( 0x00AA ) ; 
		m_symbols[L"laquo"] 
				= wchar_t( 0x00AB ) ; 
		m_symbols[L"LeftGuillemet"] // double angle quotation mark
				= wchar_t( 0x00AB ) ; 
		m_symbols[L"not"] 
				= wchar_t( 0x00AC ) ; 
		m_symbols[L"\\neg"] // negation sign
				= wchar_t( 0x00AC ) ; 
		m_symbols[L"shy"] 
				= wchar_t( 0x00AD ) ; 
		m_symbols[L"\\net"] // soft hyphen
				= wchar_t( 0x00AD ) ; 
		m_symbols[L"reg"] 
				= wchar_t( 0x00AE ) ; 
		m_symbols[L"\\net"] // registered sign
				= wchar_t( 0x00AE ) ; 

		m_symbols[L"deg"] = wchar_t( 0x00B0 ) ;		//	degree sign
		m_symbols[L"plusmn"] = wchar_t( 0x00B1 ) ;	//	/pm B: plus-or-minus sign
		m_symbols[L"sup2"] = wchar_t( 0x00B2 ) ;		//	superscript two
		m_symbols[L"sup3"] = wchar_t( 0x00B3 ) ;		//	superscript three
		m_symbols[L"micro"] = wchar_t( 0x00B5 ) ;		//	micro sign
		m_symbols[L"para"] = wchar_t( 0x00B6 ) ;		//	pilcrow (paragraph sign)
		m_symbols[L"middot"] = wchar_t( 0x00B7 ) ;	//	/centerdot B: middle dot
		m_symbols[L"sup1"] = wchar_t( 0x00B9 ) ;		//	superscript one
		m_symbols[L"ordm"] = wchar_t( 0x00BA ) ;		//	ordinal indicator, masculine
		m_symbols[L"raquo"] = wchar_t( 0x00BB ) ;		//	double angle quotation mark (guillemet), right
		m_symbols[L"frac14"] = wchar_t( 0x00BC ) ;	//	fraction one-quarter
		m_symbols[L"frac12"] = wchar_t( 0x00BD ) ;	//	fraction one-half
		m_symbols[L"half"] = wchar_t( 0x00BD ) ;		//	fraction one-half
		m_symbols[L"frac34"] = wchar_t( 0x00BE ) ;	//	fraction three-quarters
		m_symbols[L"iquest"] = wchar_t( 0x00BF ) ;	//	inverted question mark
		m_symbols[L"AElig"] = wchar_t( 0x00C6 ) ;		//	AE digraph, uppercase
		m_symbols[L"ETH"] = wchar_t( 0x00D0 ) ;		//	D with stroke, uppercase; also, uppercase Eth Eth (Icelandic)
		m_symbols[L"times"] = wchar_t( 0x00D7 ) ;		//	/times B: multiply sign
		m_symbols[L"Oslash"] = wchar_t( 0x00D8 ) ;	//	O with slash, uppercase
		m_symbols[L"THORN"] = wchar_t( 0x00DE ) ;		//	Thorn, uppercase
		m_symbols[L"szlig"] = wchar_t( 0x00DF ) ;		//	s, double
		m_symbols[L"aelig"] = wchar_t( 0x00E6 ) ; //	ae digraph, lowercase
		m_symbols[L"ccedil"] = wchar_t( 0x00E7 ) ; //	voiceless palatal fricative
		m_symbols[L"eth"] = wchar_t( 0x00F0 ) ;		//	eth
		m_symbols[L"Eth"] = wchar_t( 0x00F0 ) ;		//	voiced dental fricative
		m_symbols[L"ntilde"] = wchar_t( 0x00F1 ) ; //	Latin small letter n with tilde
		m_symbols[L"divide"] = wchar_t( 0x00F7 ) ; //	/div B: divide sign
		m_symbols[L"oslash"] = wchar_t( 0x00F8 ) ; //	o with slash, lowercase
		m_symbols[L"thorn"] = wchar_t( 0x00FE ) ; //	thorn, lowercase

		m_symbols[L"forall"]   // # FOR ALL
			  = wchar_t(0x2200) ;
		m_symbols[L"exist"]   // # THERE EXISTS
			  = wchar_t(0x2203) ;
		m_symbols[L"ni"] = wchar_t( 0x220B ) ; 
		m_symbols[L"niv"] = wchar_t( 0x220B ) ; 
		m_symbols[L"ReverseElement"] = wchar_t( 0x220B ) ; 
		m_symbols[L"SuchThat"]   // # CONTAINS AS MEMBER
			  = wchar_t(0x220B) ;
		m_symbols[L"lowast"]   // # ASTERISK OPERATOR
			  = wchar_t(0x2217) ;
		m_symbols[L"minus"]   // # MINUS SIGN
			  = wchar_t(0x2212) ;
		m_symbols[L"cong"]   // # APPROXIMATELY EQUAL TO
			  = wchar_t(0x2245) ;
		m_symbols[L"Alpha"]   // # GREEK CAPITAL LETTER ALPHA
			  = wchar_t(0x391) ;
		m_symbols[L"Beta"]   // # GREEK CAPITAL LETTER BETA
			  = wchar_t(0x392) ;
		m_symbols[L"Chi"]   // # GREEK CAPITAL LETTER CHI
			  = wchar_t(0x3A7) ;
		m_symbols[L"Delta"]   // # GREEK CAPITAL LETTER DELTA
			  = wchar_t(0x394) ;
		m_symbols[L"x2206"]   // # INCREMENT
			  = wchar_t(0x2206) ;
		m_symbols[L"epsilon"]   // # GREEK CAPITAL LETTER EPSILON
			  = wchar_t(0x395) ;
		m_symbols[L"Phi"]   // # GREEK CAPITAL LETTER PHI
			  = wchar_t(0x3A6) ;
		m_symbols[L"Gamma"]   // # GREEK CAPITAL LETTER GAMMA
			  = wchar_t(0x393) ;
		m_symbols[L"Eta"]   // # GREEK CAPITAL LETTER ETA
			  = wchar_t(0x397) ;
		m_symbols[L"Iota"]   // # GREEK CAPITAL LETTER IOTA
			  = wchar_t(0x399) ;
		m_symbols[L"Theta"]   // # GREEK THETA SYMBOL
			  = wchar_t(0x3D1) ;
		m_symbols[L"Kappa"]   // # GREEK CAPITAL LETTER KAPPA
			  = wchar_t(0x39A) ;
		m_symbols[L"Lambda"]   // # GREEK CAPITAL LETTER LAMDA
			  = wchar_t(0x39B) ;
		m_symbols[L"Mu"]   // # GREEK CAPITAL LETTER MU
			  = wchar_t(0x39C) ;
		m_symbols[L"Nu"]   // # GREEK CAPITAL LETTER NU
			  = wchar_t(0x39D) ;
		m_symbols[L"Omicron"]   // # GREEK CAPITAL LETTER OMICRON
			  = wchar_t(0x39F) ;
		m_symbols[L"Pi"]   // # GREEK CAPITAL LETTER PI
			  = wchar_t(0x3A0) ;
		m_symbols[L"Theta"]   // # GREEK CAPITAL LETTER THETA
			  = wchar_t(0x398) ;
		m_symbols[L"Rho"]   // # GREEK CAPITAL LETTER RHO
			  = wchar_t(0x3A1) ;
		m_symbols[L"Sigma"]   // # GREEK CAPITAL LETTER SIGMA
			  = wchar_t(0x3A3) ;
		m_symbols[L"Tau"]   // # GREEK CAPITAL LETTER TAU
			  = wchar_t(0x3A4) ;
		m_symbols[L"Upsilon"]   // # GREEK CAPITAL LETTER UPSILON
			  = wchar_t(0x3A5) ;
		m_symbols[L"Sigma"]   // # GREEK SMALL LETTER FINAL SIGMA
			  = wchar_t(0x3C2) ;
		m_symbols[L"Omega"]   // # GREEK CAPITAL LETTER OMEGA
			  = wchar_t(0x3A9) ;
		m_symbols[L"Ohm"] = wchar_t( 0x2126 ) ; 
		m_symbols[L"ohm"]   // # OHM SIGN
			  = wchar_t(0x2126) ;
		m_symbols[L"Xi"]   // # GREEK CAPITAL LETTER XI
			  = wchar_t(0x39E) ;
		m_symbols[L"Psi"]   // # GREEK CAPITAL LETTER PSI
			  = wchar_t(0x3A8) ;
		m_symbols[L"Zeta"]  // # GREEK CAPITAL LETTER ZETA
			  = wchar_t(0x396) ;
		m_symbols[L"there4"] = wchar_t( 0x2234 ) ; 
		m_symbols[L"therefore"]   // # THEREFORE
			  = wchar_t(0x2234) ;
		m_symbols[L"bottom"]   // # UP TACK
			  = wchar_t(0x22A5) ;
		m_symbols[L"radicalex"]   // # RADICAL EXTENDER
			  = wchar_t(0xF8E5) ;
		m_symbols[L"alpha"]   // # GREEK SMALL LETTER ALPHA
			  = wchar_t(0x3B1) ;
		m_symbols[L"beta"]   // # GREEK SMALL LETTER BETA
			  = wchar_t(0x3B2) ;
		m_symbols[L"chi"]   // # GREEK SMALL LETTER CHI
			  = wchar_t(0x3C7) ;
		m_symbols[L"delta"]   // # GREEK SMALL LETTER DELTA
			  = wchar_t(0x3B4) ;
		m_symbols[L"epsilon"]   // # GREEK SMALL LETTER EPSILON
			  = wchar_t(0x3B5) ;
		m_symbols[L"phi"]   // # GREEK SMALL LETTER PHI
			  = wchar_t(0x3C6) ;
		m_symbols[L"gamma"]   // # GREEK SMALL LETTER GAMMA
			  = wchar_t(0x3B3) ;
		m_symbols[L"eta"]   // # GREEK SMALL LETTER ETA
			  = wchar_t(0x3B7) ;
		m_symbols[L"iota"]   // # GREEK SMALL LETTER IOTA
			  = wchar_t(0x3B9) ;
		m_symbols[L"phi"] // # GREEK PHI SYMBOL
			  = wchar_t(0x3D5) ;
		m_symbols[L"kappa"]   // # GREEK SMALL LETTER KAPPA
			  = wchar_t(0x3BA) ;
		m_symbols[L"lambda"]   // # GREEK SMALL LETTER LAMDA
			  = wchar_t(0x3BB) ;
		m_symbols[L"mu"]   // # MICRO SIGN
			  = wchar_t(0xB5) ;
		m_symbols[L"mgr"]   // # GREEK SMALL LETTER MU
			  = wchar_t(0x3BC) ;
		m_symbols[L"ngr"] = wchar_t( 0x3BD ) ; 
		m_symbols[L"nu"]   // # GREEK SMALL LETTER NU
			  = wchar_t(0x3BD) ;
		m_symbols[L"omicron"]   // # GREEK SMALL LETTER OMICRON
			  = wchar_t(0x3BF) ;
		m_symbols[L"pi"]   // # GREEK SMALL LETTER PI
			  = wchar_t(0x3C0) ;
		m_symbols[L"theta"]   // # GREEK SMALL LETTER THETA
			  = wchar_t(0x3B8) ;
		m_symbols[L"rho"]   // # GREEK SMALL LETTER RHO
			  = wchar_t(0x3C1) ;
		m_symbols[L"sigma"]   // # GREEK SMALL LETTER SIGMA
			  = wchar_t(0x3C3) ;
		m_symbols[L"tau"]   // # GREEK SMALL LETTER TAU
			  = wchar_t(0x3C4) ;
		m_symbols[L"upsilon"]   // # GREEK SMALL LETTER UPSILON
			  = wchar_t(0x3C5) ;
		m_symbols[L"Pgr"]   // # GREEK PI SYMBOL
			  = wchar_t(0x3D6) ;
		m_symbols[L"omega"]   // # GREEK SMALL LETTER OMEGA
			  = wchar_t(0x3C9) ;
		m_symbols[L"xi"]   // # GREEK SMALL LETTER XI
			  = wchar_t(0x3BE) ;
		m_symbols[L"psi"]   // # GREEK SMALL LETTER PSI
			  = wchar_t(0x3C8) ;
		m_symbols[L"zeta"]   // # GREEK SMALL LETTER ZETA
			  = wchar_t(0x3B6) ;
		m_symbols[L"thksim"] = wchar_t( 0x223C ) ; 
		m_symbols[L"sim"]   // # TILDE OPERATOR"
			  = wchar_t(0x223C) ;
		m_symbols[L"euro"]   // # EURO SIGN
			  = wchar_t(0x20AC) ;
		m_symbols[L"upsih"]   // # GREEK UPSILON WITH HOOK SYMBOL"
			  = wchar_t(0x3D2) ;
		m_symbols[L"prime"]   // # PRIME
			  = wchar_t(0x2032) ;
		m_symbols[L"le"]   // # LESS-THAN OR EQUAL TO
			  = wchar_t(0x2264) ;
		m_symbols[L"frasl"]   // # FRACTION SLASH
			  = wchar_t(0x2044) ;
		m_symbols[L"fraction"]   // # DIVISION SLASH
			  = wchar_t(0x2215) ;
		m_symbols[L"infin"]   // # INFINITY
			  = wchar_t(0x221E) ;
		m_symbols[L"fnof"]   // # LATIN SMALL LETTER F WITH HOOK
			  = wchar_t(0x192) ;
		m_symbols[L"clubs"]   // # BLACK CLUB SUIT"
			  = wchar_t(0x2663) ;
		m_symbols[L"hearts"]   // # BLACK DIAMOND SUIT
			  = wchar_t(0x2666) ;
		m_symbols[L"diams"]   // # BLACK HEART SUIT
			  = wchar_t(0x2665) ;
		m_symbols[L"spades"]   // # BLACK SPADE SUIT
			  = wchar_t(0x2660) ;
		m_symbols[L"harr"]   // # LEFT RIGHT ARROW
			  = wchar_t(0x2194) ;
		m_symbols[L"larr"]   // # LEFTWARDS ARROW
			  = wchar_t(0x2190) ;
		m_symbols[L"uarr"]   // # UPWARDS ARROW
			  = wchar_t(0x2191) ;
		m_symbols[L"rarr"]   // # RIGHTWARDS ARROW
			  = wchar_t(0x2192) ;
		m_symbols[L"darr"]   // # DOWNWARDS ARROW
			  = wchar_t(0x2193) ;
		m_symbols[L"Prime"]   // # DOUBLE PRIME"
			  = wchar_t(0x2033) ;
		m_symbols[L"ge"]   // # GREATER-THAN OR EQUAL TO
			  = wchar_t(0x2265) ;
		m_symbols[L"times"]   // # MULTIPLICATION SIGN
			  = wchar_t(0xD7) ;
		m_symbols[L"prop"]   // # PROPORTIONAL TO
			  = wchar_t(0x221D) ;
		m_symbols[L"part"]   // # PARTIAL DIFFERENTIAL
			  = wchar_t(0x2202) ;
		m_symbols[L"bull"]   // # BULLET
			  = wchar_t(0x2022) ;
		m_symbols[L"divide"]   // # DIVISION SIGN
			  = wchar_t(0xF7) ;
		m_symbols[L"ne"]   // # NOT EQUAL TO
			  = wchar_t(0x2260) ;
		m_symbols[L"equiv"]   // # IDENTICAL TO
			  = wchar_t(0x2261) ;
		m_symbols[L"asymp"]   // # ALMOST EQUAL TO
			  = wchar_t(0x2248) ;
		m_symbols[L"hellip"]   // # HORIZONTAL ELLIPSIS
			  = wchar_t(0x2026) ;
		m_symbols[L"arrowvertex"]   // # VERTICAL ARROW EXTENDER
			  = wchar_t(0xF8E6) ;
		m_symbols[L"arrowhorizex"]   // # HORIZONTAL ARROW EXTENDER
			  = wchar_t(0xF8E7) ;
		m_symbols[L"carriagereturn"]   // # DOWNWARDS ARROW WITH CORNER LEFTWARDS
			  = wchar_t(0x21B5) ;
		m_symbols[L"aleph"]   // # ALEF SYMBOL
			  = wchar_t(0x2135) ;
		m_symbols[L"Ifractur"]   // # BLACK-LETTER CAPITAL I
			  = wchar_t(0x2111) ;
		m_symbols[L"Rfractur"]   // # BLACK-LETTER CAPITAL R
			  = wchar_t(0x211C) ;
		m_symbols[L"weierstrass"]   // # SCRIPT CAPITAL P
			  = wchar_t(0x2118) ;
		m_symbols[L"circlemultiply"]   // # CIRCLED TIMES
			  = wchar_t(0x2297) ;
		m_symbols[L"circleplus"]   // # CIRCLED PLUS
			  = wchar_t(0x2295) ;
		m_symbols[L"emptyset"]   // # EMPTY SET
			  = wchar_t(0x2205) ;
		m_symbols[L"intersection"]   // # INTERSECTION
			  = wchar_t(0x2229) ;
		m_symbols[L"cup"]   // # UNION
			  = wchar_t(0x222A) ;
		m_symbols[L"sup"]   // # SUPERSET OF
			  = wchar_t(0x2283) ;
		m_symbols[L"supe"]   // # SUPERSET OF OR EQUAL TO
			  = wchar_t(0x2287) ;
		m_symbols[L"nsub"]   // # NOT A SUBSET OF
			  = wchar_t(0x2284) ;
		m_symbols[L"sub"]   // # SUBSET OF
			  = wchar_t(0x2282) ;
		m_symbols[L"sube"]   // # SUBSET OF OR EQUAL TO
			  = wchar_t(0x2286) ;
		m_symbols[L"isin"]   // # ELEMENT OF
			  = wchar_t(0x2208) ;
		m_symbols[L"notin"]   // # NOT AN ELEMENT OF
			  = wchar_t(0x2209) ;
		m_symbols[L"ang"]   // # ANGLE
			  = wchar_t(0x2220) ;
		m_symbols[L"nabla"]   // # NABLA
			  = wchar_t(0x2207) ;
		m_symbols[L"registerserif"]   // # REGISTERED SIGN SERIF
			  = wchar_t(0xF6DA) ;
		m_symbols[L"copyrightserif"]   // # COPYRIGHT SIGN SERIF
			  = wchar_t(0xF6D9) ;
		m_symbols[L"trademarkserif"]   // # TRADE MARK SIGN SERIF
			  = wchar_t(0xF6DB) ;
		m_symbols[L"product"]   // # N-ARY PRODUCT
			  = wchar_t(0x220F) ;
		m_symbols[L"radic"]   // # SQUARE ROOT
			  = wchar_t(0x221A) ;
		m_symbols[L"sdot"]   // # DOT OPERATOR
			  = wchar_t(0x22C5) ;
		m_symbols[L"not"]   // # NOT SIGN
			  = wchar_t(0xAC) ;
		m_symbols[L"and"]   // # LOGICAL AND
			  = wchar_t(0x2227) ;
		m_symbols[L"or"]   // # LOGICAL OR
			  = wchar_t(0x2228) ;
		m_symbols[L"hArr"]   // # LEFT RIGHT DOUBLE ARROW
			  = wchar_t(0x21D4) ;
		m_symbols[L"lArr"]   // # LEFTWARDS DOUBLE ARROW
			  = wchar_t(0x21D0) ;
		m_symbols[L"uArr"]   // # UPWARDS DOUBLE ARROW"
			  = wchar_t(0x21D1) ;
		m_symbols[L"rArr"]   // # RIGHTWARDS DOUBLE ARROW
			  = wchar_t(0x21D2) ;
		m_symbols[L"dArr"]   // # DOWNWARDS DOUBLE ARROW
			  = wchar_t(0x21D3) ;
		m_symbols[L"loz"]   // # LOZENGE
			  = wchar_t(0x25CA) ;
		m_symbols[L"lang"]   // # LEFT-POINTING ANGLE BRACKET
			  = wchar_t(0x2329) ;
		m_symbols[L"registersans"]   // # REGISTERED SIGN SANS SERIF
			  = wchar_t(0xF8E8) ;
		m_symbols[L"copyrightsans"]   // # COPYRIGHT SIGN SANS SERIF
			  = wchar_t(0xF8E9) ;
		m_symbols[L"trademarksans"]   // # TRADE MARK SIGN SANS SERIF
			  = wchar_t(0xF8EA) ;
		m_symbols[L"summation"]   // # N-ARY SUMMATION
			  = wchar_t(0x2211) ;
		m_symbols[L"parenlefttp"]   // # LEFT PAREN TOP
			  = wchar_t(0xF8EB) ;
		m_symbols[L"parenleftex"]   // # LEFT PAREN EXTENDER
			  = wchar_t(0xF8EC) ;
		m_symbols[L"parenleftbt"]   // # LEFT PAREN BOTTOM
			  = wchar_t(0xF8ED) ;
		m_symbols[L"bracketlefttp"]   // # LEFT SQUARE BRACKET TOP
			  = wchar_t(0xF8EE) ;
		m_symbols[L"bracketleftex"]   // # LEFT SQUARE BRACKET EXTENDER
			  = wchar_t(0xF8EF) ;
		m_symbols[L"bracketleftbt"]   // # LEFT SQUARE BRACKET BOTTOM
			  = wchar_t(0xF8F0) ;
		m_symbols[L"bracelefttp"]   // # LEFT CURLY BRACKET TOP
			  = wchar_t(0xF8F1) ;
		m_symbols[L"braceleftmd"]   // # LEFT CURLY BRACKET MID
			  = wchar_t(0xF8F2) ;
		m_symbols[L"braceleftbt"]   // # LEFT CURLY BRACKET BOTTOM
			  = wchar_t(0xF8F3) ;
		m_symbols[L"braceleftex"]   // # CURLY BRACKET EXTENDER
			  = wchar_t(0xF8F4) ;
		m_symbols[L"angleright"]   // # RIGHT-POINTING ANGLE BRACKET
			  = wchar_t(0x232A) ;
		m_symbols[L"integral"]   // # INTEGRAL
			  = wchar_t(0x222B) ;
		m_symbols[L"integraltp"]   // # TOP HALF INTEGRAL
			  = wchar_t(0x2320) ;
		m_symbols[L"integralex"]   // # INTEGRAL EXTENDER
			  = wchar_t(0xF8F5) ;
		m_symbols[L"integralbt"]   // # BOTTOM HALF INTEGRAL
			  = wchar_t(0x2321) ;
		m_symbols[L"parenrighttp"]   // # RIGHT PAREN TOP
			  = wchar_t(0xF8F6) ;
		m_symbols[L"parenrightex"]   // # RIGHT PAREN EXTENDER
			  = wchar_t(0xF8F7) ;
		m_symbols[L"parenrightbt"]   // # RIGHT PAREN BOTTOM
			  = wchar_t(0xF8F8) ;
		m_symbols[L"bracketrighttp"]   // # RIGHT SQUARE BRACKET TOP
			  = wchar_t(0xF8F9) ;
		m_symbols[L"bracketrightex"]   // # RIGHT SQUARE BRACKET EXTENDER
			  = wchar_t(0xF8FA) ;
		m_symbols[L"bracketrightbt"]   // # RIGHT SQUARE BRACKET BOTTOM
			  = wchar_t(0xF8FB) ;
		m_symbols[L"bracerighttp"]   // # RIGHT CURLY BRACKET TOP
			  = wchar_t(0xF8FC) ;
		m_symbols[L"bracerightmd"]   // # RIGHT CURLY BRACKET MID
			  = wchar_t(0xF8FD) ;
		m_symbols[L"bracerightbt"]   // # RIGHT CURLY BRACKET BOTTOM
			  = wchar_t(0xF8FE) ;

	}

};

inline wstring strip_tags_only(const wstring &text)
{
	static boost::wregex stripper( L"<[!\\w/][\\s\\S]*?>") ;
	return boost::regex_replace(text, stripper, L"") ;
}

inline wstring convert_entities(const wstring &text)
{
	static symbol_map symbols ;

	wc_reader reader ;

	reader.set_buffer( text.c_str() ) ;

	wstring stripped_text ;
	stripped_text.reserve( text.size() ) ;

	while ( reader.empty() == false ) 
	{
		wstring chunk ;
		reader.getline( chunk, L'&', false ) ;
		if ( chunk.empty() == false )
		{
			stripped_text += chunk ;
		}

		if ( reader.peek() == L'&' )
		{
			reader.advance() ;
			reader.getline( chunk, L"; <", false ) ;
			if ( chunk.empty() == false )
			{
				if ( chunk[0] == L'#' )
				{
					reader.eat_if( L';' ) ;
					ATLASSERT ( chunk.size() > 1 ) ; 
					if ( chunk[1] == L'x' || chunk[1] == L'X' ) 
					{
						stripped_text += (wchar_t)string2ulong( chunk.substr(2), 16 ) ;
					}
					else
					{
						stripped_text += (wchar_t)string2ulong( chunk.substr(1) ) ;
					}
				}
				else
				{
					if ( symbols.exists( chunk ) )
					{
						reader.eat_if( L';' ) ;
						stripped_text += symbols.get_val( chunk ) ;
					}
					else // it was not a symbol tag
					{
						stripped_text += L"&" ;
						stripped_text += chunk ;
					}
				}
			}
			else
			{
				stripped_text += L"&" ;
			}
		}
	}

	return stripped_text ;
}

inline wstring strip_tags( const wstring &raw_string )
{
	const wstring no_tags = strip_tags_only(raw_string) ;
	return convert_entities(no_tags) ;
}
