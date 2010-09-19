#pragma once

#include "KeyboardListener.h"

#include "keyboard_shortcuts.h"
#include "key_target_word.h"

#include <boost/function.hpp>

BOOL installhook( shortcuts::KeyboardShortcuts *listener ) ;
BOOL uninstallhook( shortcuts::KeyboardShortcuts *listener ) ;

void set_listener( shortcuts::KeyboardShortcuts *listener ) ;
