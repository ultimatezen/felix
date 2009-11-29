#pragma once

#include "PowerPointInterface.h"
#include <boost/function.hpp>

bool shift_key_is_pressed() ;

BOOL installhook( CPowerPointInterface *addy ) ;
BOOL uninstallhook( CPowerPointInterface *addy ) ;
void set_shortcuts_callback(boost::function<void(BOOL)> callback, BOOL current_setting) ;

CPowerPointInterface* getInterface() ;
