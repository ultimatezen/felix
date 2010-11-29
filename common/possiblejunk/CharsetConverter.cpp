// CharsetConverter.cpp: implementation of the CharsetConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharsetConverter.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


#define NUL         0
#define NL          10
#define CR          13
#define ESC         27
#define SS2         142
#define PERIOD      '.'
#define SJIS1(A)    ((A >= 129 && A <= 159) || (A >= 224 && A <= 239))
#define SJIS2(A)    (A >= 64 && A <= 252)
#define HANKATA(A)  (A >= 161 && A <= 223)
#define ISMARU(A)   (A >= 202 && A <= 206)
#define ISNIGORI(A) ((A >= 182 && A <= 196) || (A >= 202 && A <= 206) || (A == 179))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CharsetConverter::CharsetConverter()
	: forcedelesc(false), incode(EUC), outcode(SJIS), 
	  makeoutfile(false), delesc( false ),
	  repairjis(false), tofullsize(false), setincode(false), 
	  docheck(false)
{

}

CharsetConverter::~CharsetConverter()
{

}

void CharsetConverter::DoCheck( bool setting )
{
	docheck = setting ;
}

void CharsetConverter::ToFullSize( bool setting )
{
	tofullsize= setting ;
}
void CharsetConverter:: SetInputCode ( int InputCode )
{ 
	ATLASSERT( InputCode > NOTSET && InputCode <= ASCII ) ;
    setincode = true ;
	incode = InputCode ;
}

void CharsetConverter:: SetOutputCode ( int OutputCode )
{ 
	ATLASSERT( OutputCode > NOTSET && OutputCode <= ASCII ) ;
	outcode = OutputCode ;
	switch( outcode )
	{
	case NEW :
		ki = "$B" ;
		ko = "(J" ;
		break ;
	case OLD:
		ki = "$@" ;
		ko = "(J" ;
		break ;
	case NEC:
		ki = "K" ;
		ko = "H" ;
		break ;
	}
}
void CharsetConverter::SetToRepair(  bool setting )
{
	ATLASSERT( ! delesc ) ;
	repairjis = setting ;
}
void CharsetConverter::DelEsc( bool setting )
{
	ATLASSERT( ! repairjis ) ;
	delesc = setting ;
}
void CharsetConverter::ForceDelEsc( bool setting )
{
	forcedelesc = setting ;
}
void CharsetConverter::DoTable( int setting )
{
	switch ( setting )
	{
	case EUC:
		doeuctable() ;
		break ;
	case JIS :
	case NEW :
	case OLD :
		dojistable();
		break;
	case SJIS :
		dosjistable();
		break;
	default :
		dojistable();            
		dosjistable();
		doeuctable();
		break;
	}
}
void CharsetConverter::InFileName( string &name )
{
	infilename = ( name ) ;
}
void CharsetConverter::OutFileName( string &name )
{
	outfilename = ( name ) ;
}

string CharsetConverter::printcode(int code)
{
  switch (code) {
    case OLD :
      return string(("Old-JIS\n"));
      break;
    case NEW :
      return string(( "New-JIS\n" ));
      break;
    case NEC :
      return string(( "NEC-JIS\n" ));
      break;
    case EUC :
      return string(( "EUC\n" ));
      break;
    case SJIS :
      return string(( "Shift-JIS\n" ));
      break;
    case EUCORSJIS :
      return string(( "ambiguous (Shift-JIS or EUC)\n" ));
      break;
    case ASCII :
      return string(( "ASCII (no Japanese)\n" ));
      break;
    case NOTSET :
      return string(( "unknown\n" ));
      break;
    default :
       return string(( "unknown\n" ));
     break;
  }
}


void CharsetConverter::dojistable(void)
{
  ATLTRACE("JIS Code Specifications:\n");
  ATLTRACE("                             DECIMAL                  HEXADECIMAL\n");
  ATLTRACE("Two-byte BYTEacter escape sequences\n");
  ATLTRACE(" JIS C 6226-1978 (Old-JIS)   027 036 064              1B 24 40\n");
  ATLTRACE(" JIS C 6226-1978 (NEC-JIS)   027 075                  1B 4B\n");
  ATLTRACE(" JIS X 0208-1983 (New-JIS)   027 036 066              1B 24 42\n");
  ATLTRACE(" JIS X 0208-1990             027 038 064 027 036 066  1B 26 40 1B 24 42\n");
  ATLTRACE(" JIS X 0212-1990             027 036 040 068          1B 24 28 44\n");
  ATLTRACE("Two-byte BYTEacters\n");
  ATLTRACE(" first byte range            033-126                  21-7E\n");
  ATLTRACE(" second byte range           033-126                  21-7E\n");
  ATLTRACE("One-byte BYTEacter escape sequences\n");
  ATLTRACE(" JIS-Roman                   027 040 074              1B 28 4A\n");
  ATLTRACE(" JIS-Roman (NEC)             027 072                  1B 48\n");
  ATLTRACE(" ASCII                       027 040 066              1B 28 42\n");
  ATLTRACE(" half-width katakana         027 040 073              1B 28 49\n");
  ATLTRACE("JIS7 half-width katakana\n");
  ATLTRACE(" byte range                  033-095                  21-5F\n");
  ATLTRACE("JIS8 half-width katakana\n");
  ATLTRACE(" shift-out                   014                      0E\n");
  ATLTRACE(" byte range                  161-223                  A1-DF\n");
  ATLTRACE(" shift-in                    015                      0F\n\n");
  ATLTRACE("NOTE: This version of the tool does not support the escape sequences\n");
  ATLTRACE("      for JIS X 0212-1990 and JIS X 0208-1990, nor any of the half-\n");
  ATLTRACE("      width katakana specifications\n\n\n");
}

void CharsetConverter::doeuctable(void)
{
  ATLTRACE("EUC Code Specifications (Packed format):\n");
  ATLTRACE("                             DECIMAL                  HEXADECIMAL\n");
  ATLTRACE("Code set 0 (ASCII)\n");
  ATLTRACE(" byte range                  033-126                  21-7E\n");
  ATLTRACE("Code set 1 (JIS X 0208-1990)\n");
  ATLTRACE(" first byte range            161-254                  A1-FE\n");
  ATLTRACE(" second byte range           161-254                  A1-FE\n");
  ATLTRACE("Code set 2 (Half-width katakana)\n");
  ATLTRACE(" first byte                  142                      8E\n");
  ATLTRACE(" second byte range           161-223                  A1-DF\n");
  ATLTRACE("Code set 3 (JIS X 0212-1990)\n");
  ATLTRACE(" first byte                  143                      8F\n");
  ATLTRACE(" second byte range           161-254                  A1-FE\n");
  ATLTRACE(" third byte range            161-254                  A1-FE\n\n");
  ATLTRACE("EUC Code Specifications (Complete 2-byte format):\n");
  ATLTRACE("                             DECIMAL                  HEXADECIMAL\n");
  ATLTRACE("Code set 0 (ASCII)\n");
  ATLTRACE(" first byte                  000                      00\n");
  ATLTRACE(" second byte range           033-126                  21-7E\n");
  ATLTRACE("Code set 1 (JIS X 0208-1990)\n");
  ATLTRACE(" first byte range            161-254                  A1-FE\n");
  ATLTRACE(" second byte range           161-254                  A1-FE\n");
  ATLTRACE("Code set 2 (Half-width katakana)\n");
  ATLTRACE(" first byte                  000                      00\n");
  ATLTRACE(" second byte range           161-223                  A1-DF\n");
  ATLTRACE("Code set 3 (JIS X 0212-1990)\n");
  ATLTRACE(" first byte range            161-254                  A1-FE\n");
  ATLTRACE(" second byte range           033-126                  21-7E\n\n");
  ATLTRACE("NOTE: This version of the tool does not support code set 3 nor\n");
  ATLTRACE("      the Complete 2-byte format\n\n\n");
}

void CharsetConverter::dosjistable(void)
{
  ATLTRACE("Shift-JIS Code Specifications:\n");
  ATLTRACE("                             DECIMAL                  HEXADECIMAL\n");
  ATLTRACE("Two-byte BYTEacters\n");
  ATLTRACE(" first byte range            129-159, 224-239         81-9F, E0-EF\n");
  ATLTRACE(" second byte range           064-126, 128-252         40-7E, 80-FC\n");
  ATLTRACE("One-byte BYTEacters\n");
  ATLTRACE(" Half-width katakana         161-223                  A1-DF\n");
  ATLTRACE(" ASCII/JIS-Roman             033-126                  21-7E\n\n\n");
}

int CharsetConverter::SkipESCSeq(const BYTE *text,int temp,int *intwobyte)
{
  int tempdata = *intwobyte;
	int increment = 0 ;
  if (temp == '$' || temp == '(')
	 increment++ ;
  if (temp == 'K' || temp == '$')
    *intwobyte = TRUE;
  else
    *intwobyte = FALSE;
  if (tempdata == *intwobyte)
    return increment;
  else
    return increment;
}
 
void CharsetConverter::removeescape(const BYTE *text,string &out)
{
  unsigned long count = 0,other = 0;

  while ( *text ) {
	  BYTE p1 = *text ; text++ ;
    if (p1 == ESC) {
      BYTE p2 = *text ; text++ ;
      if (p2 == '(') {
        BYTE p3 = *text ; text++ ;
        switch (p3) {
          case 'J' :
          case 'B' :
          case 'H' :
			  out += p2 ; out += p3 ;
            count++;
            break;
          default :
            if (forcedelesc)
			{
			  out += p2 ; out += p3 ;
			}
            else
			{
			  out += p1 ; out += p2 ; out += p3 ;
			}
            other++;
            break;
        }
      }
      else if (p2 == '$') {
        BYTE p3 = *text ; text++ ;
        switch (p3) {
          case 'B' :
          case '@' :
			  out += p2 ; out += p3 ;
            count++;
            break;
          default :
            if (forcedelesc)
			{
			  out += p2 ; out += p3 ;
			}
            else
			{
				out += p1 ; out += p2 ; out += p3 ;
			}
            other++;
            break;
        }
      }
      else {
        if (forcedelesc)
			out += p2 ;
        else
		{
			out += p1 ; out += p2 ;
		}
        other++;
      }
    }
    else
		out += p1 ;
  }
	ATLTRACE("Number of valid escape BYTEacters removed: %lu\n",count);
	ATLTRACE("Number of other escape BYTEacters forced %s: %lu\n", forcedelesc ? "removed" : "detected", other);
}

void CharsetConverter::jisrepair(const BYTE *text,string &out )
{
  int intwobyte = FALSE;
  unsigned long count = 0;

  while ( *text ) {
	  int c1 = *text ; text++ ;
    if (intwobyte) {
      if (c1 == ESC) {
		  int c2 = *text ; text++ ;
        if (c2 == '(') {
			int c3 = *text ; text++ ;
          switch ( c3 ) {
            case 'J' :
            case 'B' :
            case 'H' :
              intwobyte = FALSE;
              switch (outcode) {
                case NEC :
                case NEW :
                case OLD :
                  out += ESC ; out += ko ;
                  break;
                default :
                  break;
              }
              break;
            default :
				out += c1 ; out += c2 ; out += c3 ;
              break;
          }
        }
        else if (c2 == 'H') {
          intwobyte = FALSE;
          switch (outcode) {
            case NEC :
            case NEW :
            case OLD :
                  out += ESC ; out += ko ;
              break;
            default :
              break;
          }
        }
        else
		{
			out += c1 ; out += c2 ;
		}
      }
      else if (c1 == '(') {
		  int c2 = *text ; text++ ;
        switch (c2) {
          case 'J' :
          case 'B' :
          case 'H' :
            intwobyte = FALSE;
            switch (outcode) {
              case NEC :
              case NEW :
              case OLD :
                  out += ESC ; out += ko ;
                break;
              default :
                break;
            }
            count++;
            break;
          default :
            switch (outcode) {
              case NEC :
              case NEW :
              case OLD :
				out += c1 ; out += c2 ;
                break;
              case EUC :
				out += c1 ; out += c2 ;
                break;
              case SJIS :
                jis2sjis(&c1,&c2);
				out += c1 ; out += c2 ;
                break;
            }
            break;
        }
      }
      else if (c1 == NL || c1 == 13 ) {
        switch (outcode) {
          case NEC :
          case NEW :
          case OLD :
			  out += ESC ; out += ko ; out += c1 ;
			  text++ ;
            break;
          default :
			  out += c1 ;
			  text++ ;
            break;
        }
        count++;
        intwobyte = FALSE;
      }
      else {
		  int c1 = *text ; text++ ;
		  int c2 = *text ; text++ ;
        switch (outcode) {
          case NEC :
          case NEW :
          case OLD :
			  out += c1 ; out += c2 ;
            break;
          case EUC :
            c1 += 128;
            c2 += 128;
			out += c1 ; out += c2 ;
            break;
          case SJIS :
            jis2sjis(&c1,&c2);
			out += c1 ; out += c2 ;
            break;
        }
      }
    }
    else {
      if (c1 == ESC) {
		int c2 = *text ; text++ ;
		if (c2 == '$') {
			int c3 = *text ; text++ ;
          switch (c3) {
            case 'B' :
            case '@' :
              intwobyte = TRUE;
              switch (outcode) {
                case NEC :
                case NEW :
                case OLD :
					out += ESC ; out += ki ;
                  break;
                default :
                  break;
              }
              break;
            default :
				out += c1 ; out += c2 ; out += c3 ;
              break;
          }
        }
        else if (c2 == 'K') {
          intwobyte = TRUE;
          switch (outcode) {
            case NEC :
            case NEW :
            case OLD :
				out += ESC ; out += ki ;
              break;
            default :
              break;
          }
        }
        else
		{
			out += c1 ; out += c2  ;
		}
      }
      else if (c1 == '$') {
		int c2 = *text ; text++ ;
        switch (c2) {
          case 'B' :
          case '@' :
            intwobyte = TRUE;
            switch (outcode) {
              case NEC :
              case NEW :
              case OLD :
				out += ESC ; out += ki ;
                break;
              default :
                break;
            }
            count++;
            break;
          default :
            switch (outcode) {
              case NEC :
              case NEW :
              case OLD :
				out += c1 ; out += c2 ;
                break;
              case EUC :
				out += c1 ; out += c2 ;
                break;
              case SJIS :
				out += c1 ; out += c2 ;
                break;
            }
            break;
        }
      }
      else
		out += c1 ;
    }
  }
  if (intwobyte) {
    switch (outcode) {
      case NEC :
      case NEW :
      case OLD :
		  out += ESC ; out += ko ;
        count++;
        break;
      default :
        break;
    }
  }
  ATLTRACE("Number of escape BYTEacters restored: %lu\n",count);
}

void CharsetConverter::sjis2jis(int *p1, int *p2)
{
  unsigned int c1 = *p1;
  unsigned int c2 = *p2;
  int adjust = c2 < 159;
  int rowOffset = c1 < 160 ? 112 : 176;
  int cellOffset = adjust ? (c2 > 127 ? 32 : 31) : 126;

  *p1 = ((c1 - rowOffset) << 1) - adjust;
  *p2 -= cellOffset;
}

void CharsetConverter::jis2sjis(int *p1, int *p2)
{
  unsigned int c1 = *p1;
  unsigned int c2 = *p2;
  int rowOffset = c1 < 95 ? 112 : 176;
  int cellOffset = c1 % 2 ? (c2 > 95 ? 32 : 31) : 126;

  *p1 = ((c1 + 1) >> 1) + rowOffset;
  *p2 += cellOffset;
}

void CharsetConverter::shift2seven(const BYTE *text,string &out)
{
	if ( outcode == NOTSET || outcode == SJIS )
		SetOutputCode( NEW ) ;
  int p1,p2,intwobyte = FALSE;

  while (*text) {
	  p1 = *text ; text++ ;
    if (p1 == NL || p1 == CR) {
      if (intwobyte) {
        intwobyte = FALSE;
		out += ESC ; out += ko ;
      }
	  out += p1 ;
    }
    else if (SJIS1(p1)) {
      p2 = *text ; text++ ;
      if (SJIS2(p2)) {
        sjis2jis(&p1,&p2);
        if (!intwobyte) {
          intwobyte = TRUE;
		  out += ESC ; out += ki ;
        }
      }
	  out += p1 ; out += p2 ;
    }
    else if (HANKATA(p1)) {
      han2zen(text,&p1,&p2 );
      sjis2jis(&p1,&p2);
      if (!intwobyte) {
        intwobyte = TRUE;
		  out += ESC ; out += ki ;
      }
	  out += p1 ; out += p2 ;
    }
    else {
      if (intwobyte) {
        intwobyte = FALSE;
		  out += ESC ; out += ko ;
      }
	  out += p1 ;
    }
  }
  if (intwobyte)
  {
	  out += ESC ; out += ko ;
  }
}

void CharsetConverter::shift2euc(const BYTE *text, string &out)
{
  int p1,p2;
  
  while (*text) 
  {
	  p1 = *text ; text++ ;
    if (SJIS1(p1)) {
		p2 = *text ; text++ ;
      if (SJIS2(p2)) {
        sjis2jis(&p1,&p2);
        p1 += 128;
        p2 += 128;
      }
	  out += p1 ; out += p2 ;
    }
    else if (HANKATA(p1)) {
      if (tofullsize) {
        han2zen(text,&p1,&p2);
        sjis2jis(&p1,&p2);
        p1 += 128;
        p2 += 128;
      }
      else {
        p2 = p1;
        p1 = SS2;
      }
	  out += p1 ; out += p2 ;
    }
    else
		out += p1 ;
  }
}
 
void CharsetConverter::euc2seven(const BYTE *text,string &out)
{
	if ( outcode == NOTSET || outcode == SJIS )
		SetOutputCode( NEW ) ;
  int p1,p2,intwobyte = FALSE;

  while (*text) {
	  p1 = *text ; text++ ;
    if (p1 == NL || p1 == CR) {
      if (intwobyte) {
        intwobyte = FALSE;
		out += ESC ; out += ko ;
      }
	  out += p1 ;
    }
    else {
      if (IsEuc(p1)) {
        p2 = *text ; text++ ;
        if (IsEuc(p2)) {
          p1 -= 128;
          p2 -= 128;
          if (!intwobyte) 
		  {
            intwobyte = TRUE;
			out += ESC ; out += ki ;
          }
        }
		out += p1 ; out += p2 ;
      }
      else if (p1 == SS2) {
        p2 = *text ; ++text ;
        if (HANKATA(p2)) {
          p1 = p2;
          han2zen(text,&p1,&p2);
          sjis2jis(&p1,&p2);
          if (!intwobyte) {
            intwobyte = TRUE;
			out += ESC ; out += ki ;
          }
        }
		out += p1 ; out += p2 ;
      }
      else {
        if (intwobyte) {
          intwobyte = FALSE;
		  out += ESC ; out += ko ;
        }
		out += p1 ;
      }
    }
  }
  if (intwobyte)
	  out += ESC ; out += ko ;
}
 
void CharsetConverter::euc2shift(const BYTE *text,string &out)
{
	SetOutputCode( SJIS ) ;
  int p1,p2;

  while (*text) {
	  p1 = *text ; text++ ;
    if (IsEuc(p1)) {
      p2 = *text ; text++ ;
      if (IsEuc(p2)) {
        p1 -= 128;
        p2 -= 128;
        jis2sjis(&p1,&p2);
      }
		out += (char)p1 ; out += (char)p2 ;
    }
    else if (p1 == SS2) {
      p2 = *text ; text++ ;
      if (HANKATA(p2)) {
        p1 = p2;
        if (tofullsize) {
          han2zen(text,&p1,&p2);
		  out += p1 ; out += p2 ;
        }
        else 
		{
			out += p1 ;
        }
      }
      else
		  out += p1 ; out += p2 ;
    }
    else
		out += (char)p1 ;
  }
}

void CharsetConverter::euc2euc(const BYTE *text,string &out)
{
  int p1,p2;

  while (*text) 
  {
	  p1 = *text ; text++ ;
    if (IsEuc(p1)) {
		p2 = *text ; text++ ;
      if (IsEuc(p2))
		  out += p1 ; out += p2 ;
    }
    else if (p1 == SS2) {
		p2 = *text ; text++ ;
      if (HANKATA(p2) && tofullsize) {
        p1 = p2;
        han2zen(text,&p1,&p2);
        sjis2jis(&p1,&p2);
        p1 += 128;
        p2 += 128;
      }
		out += p1 ; out += p2 ;
    }
    else
		out += p1 ;
  }
}

void CharsetConverter::shift2shift(const BYTE *text,string &out)
{
  int p1,p2;
  
  while (*text) 
  {
	  p1 = *text ; ++text ;
    if (SJIS1(p1)) {
      p2 = *text ; ++text ;
      if (SJIS2(p2))
		  out += p1 ; out += p2 ;
    }
    else if (HANKATA(p1) && tofullsize) 
	{
      han2zen(text,&p1,&p2);
		  out += p1 ; out += p2 ;
    }
    else
		  out += p1 ;
  }
}

void CharsetConverter::seven2shift(const BYTE *text,string &out)
{
  int temp,p1,p2,intwobyte = FALSE;

  while (*text) 
  {
	  p1 = *text ; ++text ;
    if (p1 == ESC) 
	{
      temp = *text ; ++text ;
      SkipESCSeq(text,temp,&intwobyte);
    }
    else if (p1 == NL || p1 == CR) 
	{
      if (intwobyte)
        intwobyte = FALSE;
		  out += p1  ;
    }
    else 
	{
      if (intwobyte) 
	  {
        p2 = *text ; ++text ;
        jis2sjis(&p1,&p2);
		  out += p1 ; out += p2 ;
      }
      else
		  out += p1  ;
    }
  }
}
  
void CharsetConverter::seven2euc(const BYTE *text,string &out)
{
  int temp,p1,p2,intwobyte = FALSE;

  while (*text) 
  {
	  p1 = *text ; ++text ;
	  if (p1 == ESC) 
		{
		  temp = *text ; ++text;
		  SkipESCSeq(text,temp,&intwobyte);
		}
		else if (p1 == NL || p1 == CR) 
		{
		  if (intwobyte)
			intwobyte = FALSE;
		  out += p1 ;
		}
		else 
		{
		  if (intwobyte) 
		  {
			p2 = *text ; ++text ;
			p1 += 128;
			p2 += 128;
			out += p1 ; out += p2 ;
		  }
		  else
			  out += p1 ;
		}
  }
}

void CharsetConverter::seven2seven(const BYTE *text,string &out)
{
	if ( outcode == NOTSET || outcode == SJIS )
		SetOutputCode( NEW ) ;
	 int temp,p1,p2,change,intwobyte = FALSE;

  while (*text) 
  {
	  p1 = *text ; ++text  ;
    if (p1 == ESC) 
	{
      temp = *text ; ++text;
      change = SkipESCSeq(text,temp,&intwobyte);
      if ((intwobyte) && (change))
	  {
		  out += ESC ; out += ki ;
	  }
      else if (change)
	  {
		  out += ESC ; out += ko ;
	  }
    }
    else if (p1 == NL || p1 == CR) 
	{
      if (intwobyte) 
	  {
		  intwobyte = FALSE;
		  out += ESC ; out += ko ;
	  }
	  out += p1 ;
    }
    else 
	{
      if (intwobyte) 
	  {
        p2 = *text ; ++text ;
		out += p1 ; out += p2 ;
      }
      else
		  out += p1 ;
    }
  }
  if (intwobyte)
  {
		  out += ESC ; out += ko ;
  }
}

int CharsetConverter::DetectCodeType(const BYTE *text)
{
  int c = 0, whatcode = ASCII;

  ATLASSERT( NULL != text ) ;
  while ((whatcode == EUCORSJIS || whatcode == ASCII) && *text ) 
  {
    if (*text) 
	{
 	  c = *text ; ++text ;
     if (c == ESC) 
	  {
		 ATLASSERT( *text != 0 ) ;
		 c = *text ; ++text ;
        if (c == '$') 
		{
		 c = *text ; ++text ;
          if (c == 'B')
            return NEW;
          else if (c == '@')
		  {
			  SetInputCode( OLD ) ;
	            return OLD;
		  }
        }
        else if (c == 'K') 
		{
			SetInputCode( NEC ) ;
			return NEC;
		}
	 }
	 else if ((c >= 129 && c <= 141) || (c >= 143 && c <= 159))
	 {
		SetInputCode( SJIS ) ;
		return SJIS;
	 }  
      else if (c == SS2) 
	  {
		 c = *text ; ++text ;
        if ((c >= 64 && c <= 126) || (c >= 128 && c <= 160) || (c >= 224 && c <= 252))
		{
			SetInputCode( SJIS ) ;
			return SJIS;
		}
        else if (c >= 161 && c <= 223)
          whatcode = EUCORSJIS;
      }
      else if (c >= 161 && c <= 223) 
	  {
		 c = *text ; ++text ;
        if (c >= 240 && c <= 254)
		{
			SetInputCode( EUC ) ;
			return EUC ;
		}
        else if (c >= 161 && c <= 223)
          whatcode = EUCORSJIS;
        else if (c >= 224 && c <= 239) 
		{
          whatcode = EUCORSJIS;
          while (c >= 64 && /* c != EOF && */ c != 0 && whatcode == EUCORSJIS) 
		  {
            if (c >= 129) 
			{
              if (c <= 141 || (c >= 143 && c <= 159))
		{
			SetInputCode( SJIS ) ;
			return SJIS;
		}
              else if (c >= 253 && c <= 254)
			{
				SetInputCode( EUC ) ;
				return EUC;
			}
            }
		 c = *text ; ++text ;
          }
        }
        else if (c <= 159)
		{
			SetInputCode( SJIS ) ;
			return SJIS;
		}
      }
      else if (c >= 240 && c <= 254)
		{
			SetInputCode( EUC ) ;
			return EUC;
		}
      else if (c >= 224 && c <= 239) 
	  {
		 c = *text ; ++text ;
        if ((c >= 64 && c <= 126) || (c >= 128 && c <= 160))
		{
			SetInputCode( SJIS ) ;
			return SJIS;
		}
        else if (c >= 253 && c <= 254)
		{
			SetInputCode( EUC ) ;
			return EUC;
		}
        else if (c >= 161 && c <= 252)
          whatcode = EUCORSJIS;
      }
    }
  }
  return whatcode;
}

void CharsetConverter::han2zen(const BYTE *text,int *p1,int *p2)
{
  int tmp = *p1,junk,maru = FALSE,nigori = FALSE;
  static int mtable[][2] = {
    {129,66},{129,117},{129,118},{129,65},{129,69},{131,146},{131,64},
    {131,66},{131,68},{131,70},{131,72},{131,131},{131,133},{131,135},
    {131,98},{129,91},{131,65},{131,67},{131,69},{131,71},{131,73},
    {131,74},{131,76},{131,78},{131,80},{131,82},{131,84},{131,86},
    {131,88},{131,90},{131,92},{131,94},{131,96},{131,99},{131,101},
    {131,103},{131,105},{131,106},{131,107},{131,108},{131,109},
    {131,110},{131,113},{131,116},{131,119},{131,122},{131,125},
    {131,126},{131,128},{131,129},{131,130},{131,132},{131,134},
    {131,136},{131,137},{131,138},{131,139},{131,140},{131,141},
    {131,143},{131,147},{129,74},{129,75}
  };

  if (incode == SJIS) 
  {
	  *p2 = *text ; ++text ;
	  switch( *p2 )
	  {
	  case 222:
		  if (ISNIGORI(*p1))
			  nigori = TRUE ;
		  else
			  --text ;	
		  break ;
	  case 223:
		  if (ISMARU(*p1))
			  maru = TRUE ;
		  else
			  --text ;   
		  break ;
	  default:
		  --text ;
	  }
  }
  else if (incode == EUC) 
  {
	  junk = *text ;
	  --text ;
	  if (junk == SS2) 
	  {
		*p2 = *text ; ++text ;
		if (*p2 == 222) 
		{
			if (ISNIGORI(*p1))
				nigori = TRUE;
			else 
			{
				--text ; --text ;
			}
		}
		else if (*p2 == 223) 
		{
			if (ISMARU(*p1))
				maru = TRUE;
			else 
			{
				--text ;
				--text ;
			}
		}
		else 
		{
			--text ;
			--text ;
		}
	}
	else
		--text ;
  }
  if (*p1 >= 161 && *p1 <= 223) {
    *p1 = mtable[tmp - 161][0];
    *p2 = mtable[tmp - 161][1];
  }
  if (nigori) {
    if ((*p2 >= 74 && *p2 <= 103) || (*p2 >= 110 && *p2 <= 122))
      (*p2)++;
    else if (*p1 == 131 && *p2 == 69)
      *p2 = 148;
  }
  else if (maru && *p2 >= 110 && *p2 <= 122)
    *p2 += 2;
}

void CharsetConverter::Convert(const BYTE *text, string &out)
{
	if ( incode == NOTSET )
		incode = DetectCodeType( text ) ;
	ATLASSERT( incode != EUCORSJIS ) ;
	switch( incode )
	{
	case SJIS:
		shift2shift( text, out ) ;
		return ;
	case EUC:
		euc2shift( text, out ) ;
		return ;
	case JIS:
	case NEW:
	case OLD:
		seven2shift( text, out ) ;
	}
	ATLASSERT( "No cases matched!" == NULL ) ;
}

bool CharsetConverter::IsEuc(BYTE b)
{
	return (b >= 161 && b <= 254) ;
}
