#pragma once

#include "config_file.h"
#include "keyboard_shortcuts.h"
#include "key_mapper_ppt.h"

const static CString SHORTCUTS_FILE = _T("shortcuts_word.xml") ;

void write_default_shortcuts_file(CString filename) ;
string get_default_file_text(void) ;
string get_shortcuts_text(void) ;

