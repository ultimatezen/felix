// stdafx.cpp : source file that includes just the standard includes
//	Felix.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#ifndef STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_DEPRECATE
#endif

#include "StdAfx.h"

#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#if (_ATL_VER < 0x0700)
#include <statreg.cpp>
#endif //(_ATL_VER < 0x0700)
#endif //_ATL_STATIC_REGISTRY
