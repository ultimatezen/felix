#pragma once

#include "output_device.h"
#include "input_device.h"

CString get_config_filename(CString filename, output_device_ptr output) ;
CString get_local_appdata() ;
string get_config_text(CString filename, output_device_ptr output, input_device_ptr input) ;
string get_file_text(CString filename, input_device_ptr input) ;
