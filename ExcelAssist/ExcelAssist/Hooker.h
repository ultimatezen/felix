/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "FelixExcelif.h"
#include <boost/function.hpp>
#include "keyboard_shortcuts.h"

BOOL installhook( shortcuts::KeyboardShortcuts *keys ) ;
BOOL uninstallhook( shortcuts::KeyboardShortcuts *keys ) ;

shortcuts::KeyboardShortcuts* getInterface() ;
