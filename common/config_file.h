#pragma once

#include "output_device.h"
#include "input_device.h"

CString get_config_filename(CString filename, output_device_ptr output) ;
wstring get_config_text(CString filename, output_device_ptr output, input_device_ptr input) ;
