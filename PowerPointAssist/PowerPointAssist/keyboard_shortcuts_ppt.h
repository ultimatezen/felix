#pragma once

#include "config_file.h"
#include "keyboard_shortcuts.h"
#include "key_mapper_ppt.h"
#include "output_device.h"

const static CString SHORTCUTS_FILE = _T("shortcuts_ppt.xml") ;

void write_default_shortcuts_file(CString filename, OutputDevice *output) ;
string get_default_file_text(void) ;
string get_shortcuts_text(CString base_filename) ;


