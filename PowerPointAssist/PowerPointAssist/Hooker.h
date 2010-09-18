#pragma once

#include "PowerPointInterface.h"
#include <boost/function.hpp>

#include "keyboard_shortcuts.h"

BOOL installhook( shortcuts::KeyboardShortcuts *keys ) ;
BOOL uninstallhook( shortcuts::KeyboardShortcuts *keys ) ;
void set_shortcuts_callback(boost::function<void(BOOL)> callback, BOOL current_setting) ;

shortcuts::KeyboardShortcuts* getInterface() ;
