/*!
	@file GetWinVer.h
	@brief Interface file for GetWinVer function.
	@date 2005/06/25
	Time: 15:20:21
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary

    GetWinVer.h  Version 1.1

    Copyright (C) 2001-2003 Hans Dietrich

    This software is released into the public domain.  
    You are free to use it in any way you like, except
    that you may not sell this source code.

    This software is provided "as is" with no expressed 
    or implied warranty.  I accept no liability for any 
    damage or loss of business that this software may cause. 

*/


#pragma once


#define WUNKNOWNSTR	"unknown Windows version"

#define W95STR			"Windows 95"
#define W95SP1STR		"Windows 95 SP1"
#define W95OSR2STR		"Windows 95 OSR2"
#define W98STR			"Windows 98"
#define W98SP1STR		"Windows 98 SP1"
#define W98SESTR		"Windows 98 SE"
#define WMESTR			"Windows ME"

#define WNT351STR		"Windows NT 3.51"
#define WNT4STR			"Windows NT 4"
#define W2KSTR			"Windows 2000"
#define WXPSTR			"Windows XP"
#define W2003SERVERSTR	"Windows 2003 Server"

#define WCESTR			"Windows CE"


#define WUNKNOWN	0

#define W9XFIRST	1
#define W95			1
#define W95SP1		2
#define W95OSR2		3
#define W98			4
#define W98SP1		5
#define W98SE		6
#define WME			7
#define W9XLAST		99

#define WNTFIRST	101
#define WNT351		101
#define WNT4		102
#define W2K			103
#define WXP			104
#define W2003SERVER	105
#define WNTLAST		199

#define WCEFIRST	201
#define WCE			201
#define WCELAST		299

BOOL GetWinVer(LPSTR pszVersion, int *nVersion, LPSTR pszMajorMinorBuild);

