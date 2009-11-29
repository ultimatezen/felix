#include "stdafx.h"
#include "MiniVersion.h"
#include "file.h"
#include "path.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG

namespace easyunit
{
	TEST( TestCMiniVersion, instantiate)
	{
		file::CPath path ;
		path.GetModFileName(_Module.get_m_hInst()) ;
		CMiniVersion ver((LPCSTR)CT2A(path.Path())) ;
		WORD filever = 0;
		ASSERT_EQUALS_V(1, (int)ver.GetFileVersion(&filever)) ;
	}

}

#endif