#include "StdAfx.h"
#include ".\resource_string.h"

CStringA resource_string_a( UINT id )
{
	CStringA res_str ;
	ATLVERIFY(res_str.LoadString( id )) ;
	return res_str ;
}

CStringW resource_string_w( UINT id )
{
	CStringW res_str ;
	ATLVERIFY(res_str.LoadString( id )) ;
	return res_str ;
}
