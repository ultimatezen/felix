#pragma once

#include "CStringInterface.h"
#include "Path.h"

namespace fileops
{

void addExtensionAsNeeded( CString &fileName, const CString ext ) ;

enum FileFormat {
	FORMAT_HTML,
	FORMAT_XML,
	FORMAT_TEXT,
} ;

unsigned int detect_codepage(LPCSTR text, FileFormat file_format) ;

CString get_local_appdata_folder() ;
}