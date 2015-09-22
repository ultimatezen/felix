#pragma once

namespace logical
{
	bool any(bool arg1)
	{
		return arg1 ;
	}
	bool any(bool arg1, bool arg2)
	{
		return any(arg1) || arg2 ;
	}
	bool any(bool arg1, bool arg2, bool arg3)
	{
		return any(arg1, arg2) || arg3 ;
	}
	bool any(bool arg1, bool arg2, bool arg3, bool arg4)
	{
		return any(arg1, arg2, arg3) || arg4 ;
	}
	bool any(bool arg1, bool arg2, bool arg3, bool arg4, bool arg5)
	{
		return any(arg1, arg2, arg3, arg4) || arg5 ;
	}
	bool any(bool arg1, bool arg2, bool arg3, bool arg4, bool arg5, bool arg6)
	{
		return any(arg1, arg2, arg3, arg4, arg5) || arg6 ;
	}

	// all
	bool all(bool arg1)
	{
		return arg1 ;
	}
	bool all(bool arg1, bool arg2)
	{
		return all(arg1) && arg2 ;
	}
	bool all(bool arg1, bool arg2, bool arg3)
	{
		return all(arg1, arg2) && arg3 ;
	}
	bool all(bool arg1, bool arg2, bool arg3, bool arg4)
	{
		return all(arg1, arg2, arg3) && arg4 ;
	}
	bool all(bool arg1, bool arg2, bool arg3, bool arg4, bool arg5)
	{
		return all(arg1, arg2, arg3, arg4) && arg5 ;
	}
	bool all(bool arg1, bool arg2, bool arg3, bool arg4, bool arg5, bool arg6)
	{
		return all(arg1, arg2, arg3, arg4, arg5) && arg6 ;
	}

}

