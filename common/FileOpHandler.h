#pragma once

#include "CStringInterface.h"
#include "Path.h"

namespace fileops
{

void add_extension_as_needed( CString &fileName, const CString ext ) ;

CString get_local_appdata_folder() ;
}