#pragma once

#include "KeyboardListener.h"

#include "keyboard_shortcuts.h"

#include <boost/function.hpp>

BOOL installhook( CKeyboardListener *listener ) ;
BOOL uninstallhook( CKeyboardListener *listener ) ;

void set_listener( CKeyboardListener *listener ) ;
void set_shortcuts_callback(boost::function<void(BOOL)> callback, BOOL current_setting) ;
void set_classic_if_on() ;
void set_classic_if_off() ;
