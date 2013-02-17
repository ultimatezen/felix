#include "StdAfx.h"
#include ".\resource_string.h"

CStringA resource_string_a( UINT id )
{
	CStringA res_str ;
	if(! res_str.LoadString( id ))
	{
		logging::log_warn("Failed to load resource ID " + int2string(id)) ;
	}
	return res_str ;
}

CStringW resource_string_w( UINT id )
{
	CStringW res_str ;
	if (! res_str.LoadString( id ))
	{
		logging::log_warn("Failed to load resource ID " + int2string(id)) ;
	}
	return res_str ;
}
