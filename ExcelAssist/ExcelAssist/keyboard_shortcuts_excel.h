#pragma once

#include "config_file.h"
#include "keyboard_shortcuts.h"
#include "key_mapper_excel.h"
#include "input_device.h"
#include "output_device.h"

const static CString SHORTCUTS_FILE = _T("shortcuts_excel.xml") ;

void write_default_shortcuts_file(CString filename, output_device_ptr output) ;
string get_default_file_text(void) ;
string get_shortcuts_text(CString basename, input_device_ptr input, output_device_ptr output) ;

