/*!
	@file Library.h
	@brief interface for the CLibrary class
 */

#pragma once

/**
	@class CLibrary  
	@brief Wrapper for DLL HINSTANCE.
 */
class CLibrary  
{
    HINSTANCE m_library_instance ;

public:
	bool load( const TCHAR *lib_name );
	FARPROC get_proc_address( const char *proc_name );
	bool is_loaded() const;
	void dispose();

	/** construction */
	CLibrary( const TCHAR *lib_name = NULL ) ;
	/** destruction */
	~CLibrary();

};

