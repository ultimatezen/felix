/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "FelixExcelif.h"
#include <boost/function.hpp>

BOOL installhook( CFelixExcelIF *addy ) ;
BOOL uninstallhook( CFelixExcelIF *addy ) ;
void set_shortcuts_callback(boost::function<void(BOOL)> callback, BOOL current_setting) ;

CFelixExcelIF* getInterface() ;
