// harsetConverter.h: interface for the CharsetConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARSETCONVERTER_H__098FCA2A_E025_4E67_83A0_AF89A9B0EE1B__INCLUDED_)
#define AFX_HARSETCONVERTER_H__098FCA2A_E025_4E67_83A0_AF89A9B0EE1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StringEx.h"
#include "File.h"

enum { NOTSET, NEW, JIS, OLD, NEC, EUC, SJIS, EUCORSJIS, ASCII } ;
enum { INPUT, OUTPUT, REPAIR } ;

class CharsetConverter  
{

public:
	bool IsEuc( BYTE b );
	void Convert( const BYTE *text, string &out);
	CharsetConverter();
	virtual ~CharsetConverter();

	// my functions
	void DoCheck( bool setting = true ) ;
	void ToFullSize( bool setting = true ) ;
	void SetInputCode ( int InputCode = EUC ) ;
	void SetOutputCode ( int OutputCode = SJIS ) ;
	void SetToRepair(  bool setting = true ) ;
	void DelEsc( bool setting = true ) ;
	void ForceDelEsc( bool setting = true ) ;
	void DoTable( int setting = SJIS ) ;
	void InFileName( string &name ) ;
	void OutFileName( string &name ) ;

	// Lunde's functions
	void han2zen(const BYTE *in,int *p1,int *p2);
	void sjis2jis(int *p1,int *p2);
	void jis2sjis(int *p1,int *p2);
	void shift2seven(const BYTE *in,string &out);
	void shift2euc(const BYTE *in,string &out);
	void euc2seven(const BYTE *in,string &out);
	void euc2euc(const BYTE *in,string &out);
	void shift2shift(const BYTE *in,string &out );
	void euc2shift(const BYTE *in,string &out );
	void seven2shift(const BYTE *in,string &out);
	void seven2euc(const BYTE *in,string &out);
	void seven2seven(const BYTE *in,string &out);
	void dojistable(void);
	void doeuctable(void);
	void dosjistable(void);
	void jisrepair(const BYTE *in,string &out );
	void removeescape(const BYTE *in,string &out);
	string printcode(int code);
	int toup(int data);
	int SkipESCSeq(const BYTE *in,int temp,int *intwobyte);
	int DetectCodeType(const BYTE *in);
private:  
	int incode ;
	int outcode ;
	int tempincode ;

	bool forcedelesc ;
	bool makeoutfile ;
	bool delesc ;
	bool repairjis ;
	bool tofullsize ;
	bool setincode ;
	bool docheck  ;

	string infilename ;
	string outfilename ;
	string ki ;
	string ko ;
};

#endif // !defined(AFX_HARSETCONVERTER_H__098FCA2A_E025_4E67_83A0_AF89A9B0EE1B__INCLUDED_)
