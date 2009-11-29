#pragma  once

//#include "AddIn.h"
#include "PowerPointImports.h"

bool load_picture( Office::_CommandBarButtonPtr &button, int button_id );

bool item_needs_killing( _bstr_t caption ) ;