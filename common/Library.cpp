/*!
	@file Library.cpp
	@brief implementation of the CLibrary class
 */

#include "stdafx.h"
#include "Library.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


CLibrary::CLibrary(const TCHAR *lib_name)
	: m_library_instance( NULL )
{
	if ( lib_name != NULL ) load( lib_name ) ;
}

CLibrary::~CLibrary()
{
	dispose() ;
}


/** load a DLL.
  @retval bool 
  @param const TCHAR *lib_name
*/
bool CLibrary::load(const TCHAR *lib_name)
{
	if (!lib_name)
	{
		return is_loaded() ;
	}

	dispose() ;

	m_library_instance = ::LoadLibrary( lib_name ) ;

	return is_loaded() ;

}


/** Free the dll.
  @retval void 
  */
void CLibrary::dispose()
{
	if ( is_loaded() )
	{
		::FreeLibrary( m_library_instance ) ;
	}

	m_library_instance = NULL ;
}


/** Get the process address.	 
  @retval FARPROC 
  @param const TCHAR *proc_name
  */
FARPROC CLibrary::get_proc_address(const char *proc_name)
{
	ATLASSERT( is_loaded() ) ;

    return ::GetProcAddress(m_library_instance, proc_name );  
}

/** Is a library loaded?	 
	@retval bool 
	@param const TCHAR *proc_name
*/
bool CLibrary::is_loaded() const
{
	return m_library_instance != NULL ;
}
