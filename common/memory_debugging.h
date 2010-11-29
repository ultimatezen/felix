/*!
	@file memory_debugging.h
	@brief header for memory debugging stuff
	@date 2004/12/30
	Time: 8:15:18
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

#ifdef _DEBUG_42 // the answer

#define _CRTDBG_MAP_ALLOC
#define DEBUG_NEW new ( _NORMAL_BLOCK, __FILE__, __LINE__) 

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <crtdbg.h>

template< class ptr_type >
inline bool IsValidWritePointer( ptr_type *ptr, size_t pointer_size = 1 )
{
	if ( ptr == NULL )
		return false ;

	return 
		1 == _CrtIsValidPointer( 
			static_cast< const void* >( ptr ), 
			sizeof(ptr_type) * static_cast< unsigned int >( pointer_size ), 
			true ) ;
}
template< class ptr_type >
inline bool IsValidReadPointer( ptr_type *ptr, size_t pointer_size = 1  )
{
	return 
	( 
		1 == _CrtIsValidPointer
		( 
			static_cast< const void* >( ptr ), 
			sizeof(ptr_type) * static_cast< unsigned int >( pointer_size ), 
			false 
		) 
	) ;
}

#endif