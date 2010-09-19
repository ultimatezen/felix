#pragma once

#include "PowerPointInterface.h"
#include <boost/function.hpp>
#include "keyboard_shortcuts.h"

BOOL installhook( shortcuts::KeyboardShortcuts *keys ) ;
BOOL uninstallhook( shortcuts::KeyboardShortcuts *keys ) ;

shortcuts::KeyboardShortcuts* getInterface() ;
