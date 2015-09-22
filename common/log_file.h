// log_file.h: interface for the log_file class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_FILE_H__8D95FA18_3437_47CB_9C5D_0130743CDCFA__INCLUDED_)
#define AFX_LOG_FILE_H__8D95FA18_3437_47CB_9C5D_0130743CDCFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Path.h"
#include "File.h"
#include "Exceptions.h"


#define MAX_LINE_LEN 1024


#define LOG_FILE_INFO( x ) \
	{ \
		TCHAR _log_buff[512] ; \
		StringCbPrintf( _log_buff, 512, _T("Time:\t%s\r\nFile:\t%s\r\nLine\t%d\r\n"), \
		 __TIMESTAMP__, __FILE__, __LINE__) ; \
		x.write_text( _log_buff ) ; \
	}

#define LOG_TIMESTAMP( x ) \
	{ \
	TCHAR _log_buff[512] ; \
	StringCbPrintf( _log_buff, 512, _T("Time:\t%s\r\n"), \
	__TIMESTAMP__) ; \
	x.write_text( _log_buff ) ; \
	}

class log_file  
{
	CString m_file_name ;

public:
	void write_version_info();
	void write_timestamp( LPCSTR title = "Time: " );
	void log_exception( CException &e );
	void log_exception( _com_error &ce );
	void write_text( LPCWSTR text );
	void write_text( LPCSTR text );
	void write_line( LPCWSTR line = NULL );
	void write_line( LPCSTR line = NULL );
	void init();
	log_file( LPCTSTR file_name );
	virtual ~log_file();

	// stream in
	log_file &operator << ( LPCSTR buf )
	{
		write_text( buf ) ;
		return *(this) ;
	}
	log_file &operator << ( LPCWSTR buf )
	{
		write_text( buf ) ;
		return *(this) ;
	}

};

#endif // !defined(AFX_LOG_FILE_H__8D95FA18_3437_47CB_9C5D_0130743CDCFA__INCLUDED_)
