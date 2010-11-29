/*!
	@brief Filesystem wrappers

  Classes in this file:
  <ul> 
	<li> @ref name
	<li> @ref view
	<li> @ref file
	<li> @ref directory
	<li> @ref OpenDlgList
  </ul>

 */


#pragma once

#include "StringEx.h"			// for string conversion
#include <vector>				// for vectors of CStrings
#include "StringConversions.h"	// convert to/from wstrings/strings/CStrings

#include "Exceptions.h"		// exception support
#include "DebugUtilities.h"	// TRACE et al

#include "MiscWrappers.h"	// misc_wrappers::handle
#include "atlstr.h"

#include "Path.h"

// shell API support
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

#include "stdio.h"
#include "stdarg.h"


/**
	@namespace file
	@brief File and directory-related classes.
 */
namespace file
{

typedef std::vector< CString > cstring_vector ;

/////////////////////////////////////////////////////////////////
// Standard file wrapper

// Win32 File wrapper class
// Important: Don't make the destructor "virtual" because we need
//            the class v-table to look like the HANDLE type!
// NOTE: Win 95/98 only supports < 4Gb files; see Q250301

/**
	@class CFileT
	@brief Thin wrapper for file handle.
 */
template< bool t_bManaged >
class CFileT
{
public:
	HANDLE m_hFile;

	CFileT(HANDLE hFile = INVALID_HANDLE_VALUE) 
	{
		m_hFile = hFile;
	}
	CFileT(const CFileT<t_bManaged>& other) 
	{
		m_hFile = INVALID_HANDLE_VALUE;
		DuplicateHandle(other.m_hFile);
	}
	~CFileT()
	{ 
		if( t_bManaged ) Close(); 
	}
	operator HFILE() const 
	{ 
		return (HFILE) m_hFile; 
	}
	operator HANDLE() const 
	{ 
		return m_hFile; 
	}
	const CFileT<t_bManaged>& operator=(const CFileT<t_bManaged>& other)
	{
		DuplicateHandle(other.m_hFile);
		return *this;
	}
	BOOL Open(LPCTSTR pstrFileName, 
		const DWORD dwAccess = GENERIC_READ, 
		const DWORD dwShareMode = FILE_SHARE_READ, 
		const DWORD dwFlags = OPEN_EXISTING,
		const DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL)
	{
		Close();
		// Attempt file creation
		HANDLE hFile = ::CreateFile(pstrFileName, 
			dwAccess, 
			dwShareMode, 
			NULL,
			dwFlags, 
			dwAttributes, 
			NULL);
		if( hFile == INVALID_HANDLE_VALUE ) return FALSE;
		m_hFile = hFile;
		return TRUE;
	}
	BOOL Create(LPCTSTR pstrFileName,
		const DWORD dwAccess = GENERIC_WRITE, 
		const DWORD dwShareMode = 0 /*DENY ALL*/, 
		const DWORD dwFlags = CREATE_ALWAYS,
		const DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL)
	{
		return Open(pstrFileName, dwAccess, dwShareMode, dwFlags, dwAttributes);
	}
	void Close()
	{
		if( m_hFile == INVALID_HANDLE_VALUE ) return;
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	BOOL IsOpen() const
	{
		return m_hFile != INVALID_HANDLE_VALUE;
	}
	void Attach(HANDLE hHandle)
	{
		Close();
		m_hFile = hHandle;
	}   
	HANDLE Detach()
	{
		HANDLE h = m_hFile;
		m_hFile = INVALID_HANDLE_VALUE;
		return h;
	}
	BOOL Read(LPVOID lpBuf, const DWORD nCount)
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		_ASSERTE(lpBuf!=NULL);
		_ASSERTE(!::IsBadWritePtr(lpBuf, nCount));
		if( nCount == 0 ) return TRUE;   // avoid Win32 "null-read"
		DWORD dwRead;
		if( !::ReadFile(m_hFile, lpBuf, nCount, &dwRead, NULL) ) return FALSE;
		return TRUE;
	}
	BOOL Read(LPVOID lpBuf, const DWORD nCount, LPDWORD pdwRead)
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		_ASSERTE(lpBuf);
		_ASSERTE(!::IsBadWritePtr(lpBuf, nCount));
		_ASSERTE(pdwRead);
		*pdwRead = 0;
		if( nCount == 0 ) return TRUE;   // avoid Win32 "null-read"
		if( !::ReadFile(m_hFile, lpBuf, nCount, pdwRead, NULL) ) return FALSE;
		return TRUE;
	}
	BOOL Write(LPCVOID lpBuf, const DWORD nCount)
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		_ASSERTE(lpBuf!=NULL);
		_ASSERTE(!::IsBadReadPtr(lpBuf, nCount));   
		if( nCount == 0 ) return TRUE; // avoid Win32 "null-write" option
		DWORD dwWritten;
		if( !::WriteFile(m_hFile, lpBuf, nCount, &dwWritten, NULL) ) return FALSE;
		return TRUE;
	}
	BOOL Write(LPCVOID lpBuf, const DWORD nCount, LPDWORD pdwWritten)
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		_ASSERTE(lpBuf);
		_ASSERTE(!::IsBadReadPtr(lpBuf, nCount));
		_ASSERTE(pdwWritten);    
		*pdwWritten = 0;
		if( nCount == 0 ) return TRUE; // avoid Win32 "null-write" option
		if( !::WriteFile(m_hFile, lpBuf, nCount, pdwWritten, NULL) ) return FALSE;
		return TRUE;
	}
	DWORD Seek(const LONG lOff, const UINT nFrom, LONG *pos2=NULL)
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		DWORD dwNew = ::SetFilePointer(m_hFile, lOff, pos2, (DWORD) nFrom);
		if( dwNew == INVALID_SET_FILE_POINTER ) return (DWORD) -1;
		return dwNew;
	}
	DWORD GetPosition() const
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		const DWORD dwPos = ::SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
		if( dwPos == INVALID_SET_FILE_POINTER ) return INVALID_FILE_SIZE ;
		return dwPos;
	}
	BOOL Lock(const DWORD dwOffset, const DWORD dwSize)
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		return ::LockFile(m_hFile, dwOffset, 0, dwSize, 0);
	}
	BOOL Unlock(const DWORD dwOffset, const DWORD dwSize)
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		return ::UnlockFile(m_hFile, dwOffset, 0, dwSize, 0);
	}
	BOOL SetEOF()
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		return ::SetEndOfFile(m_hFile);
	}
	BOOL Flush()
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		return ::FlushFileBuffers(m_hFile);
	}
	DWORD GetSize() const
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		return ::GetFileSize(m_hFile, NULL);
	}
	DWORD GetType() const
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		return ::GetFileType(m_hFile);
	}
	BOOL GetFileTime(FILETIME* ftCreate, FILETIME* ftAccess, FILETIME* ftModified)
	{
		_ASSERTE(m_hFile!=INVALID_HANDLE_VALUE);
		return ::GetFileTime(m_hFile, ftCreate, ftAccess, ftModified);
	}
	BOOL DuplicateHandle(HANDLE hOther)
	{
		ATLASSERT(m_hFile==INVALID_HANDLE_VALUE);
		ATLASSERT(hOther!=INVALID_HANDLE_VALUE);
		HANDLE process = ::GetCurrentProcess();
		const BOOL res = ::DuplicateHandle(process, hOther, process, &m_hFile, NULL, FALSE, DUPLICATE_SAME_ACCESS);
		_ASSERTE(res);
		return res;
	}
	static BOOL FileExists(LPCTSTR pstrFileName)
	{
		_ASSERTE(!::IsBadStringPtr(pstrFileName, MAX_PATH));
		const DWORD dwErrMode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
		const BOOL bRes = ::GetFileAttributes(pstrFileName) != 0xFFFFFFFF;
		::SetErrorMode(dwErrMode);
		return bRes;
	}
	static BOOL Delete(LPCTSTR pstrFileName)
	{
		_ASSERTE(!::IsBadStringPtr(pstrFileName, MAX_PATH));
		return ::DeleteFile(pstrFileName);
	}
	static BOOL Rename(LPCTSTR pstrSourceFileName, LPCTSTR pstrTargetFileName)
	{
		_ASSERTE(!::IsBadStringPtr(pstrSourceFileName, MAX_PATH));
		_ASSERTE(!::IsBadStringPtr(pstrTargetFileName, MAX_PATH));
		return ::MoveFile(pstrSourceFileName, pstrTargetFileName);
	}
};

/** DTOR closes file handle.
 */
typedef CFileT<true> CFile;
/** DTOR does not close file handle.
 */
typedef CFileT<false> CFileHandle;

/**
	@class CFileExtension
	@brief Wrapper for file extension.
 */
class CFileExtension
{
	CString m_ext ;
public:
	CFileExtension( const CString extension )
	{
		internal_copy(extension) ;
	}
	CFileExtension& operator=( const CString extension )
	{
		internal_copy(extension) ;
		return *this ;
	}
	void internal_copy(const CString &extension)
	{
		CPath path = extension ;
		m_ext = path.FindExtension() ;
	}
	operator const CString () const
	{
		return m_ext ;
	}
	const CString get() const
	{
		return m_ext ;
	}
	bool equals( const CString &ext ) const
	{
		if ( ext.IsEmpty() )
		{
			return m_ext.IsEmpty() ;
		}
		CString cmp ;
		if ( ext.GetAt(0) == _T('.') )
		{
			cmp = ext ;
		}
		else
		{
			cmp = _T(".") + ext ;
		}
		return ( 0 == _tcsicmp( m_ext, cmp ) ) ;
	}
	bool is_html() const 
	{
		if ( equals(_T(".htm")) || equals(_T(".html") ) || equals(_T(".shtml") ) ) 
		{
			return true ; 
		}
		return false ; 
	}
	bool is_xml() const
	{
		return equals( _T(".xml") ) ;
	}
	bool is_txt() const
	{
		return equals( _T(".txt") ) ;
	}
	bool is_doc() const
	{
		return equals( _T(".doc") ) ;
	}
	bool is_bat() const
	{
		return equals( _T(".bat") ) ;
	}
	bool is_css() const
	{
		return equals( _T(".css") ) ;
	}
	bool is_cpp() const
	{
		return equals( _T(".h") ) || equals( _T(".hpp") ) || 
			equals( _T(".cpp") ) || equals( _T(".cxx") ) ;
	}
	bool is_python() const
	{
		return equals( _T(".py") ) || equals( _T(".pyw") ) ;
	}
	bool is_perl() const
	{
		return equals( _T(".pl") ) || equals( _T(".cgi") ) ;
	}
	bool is_php() const
	{
		if ( equals(_T(".php")) || equals(_T(".php3") ) || equals(_T(".php4") ) || equals(_T(".php5") ) || equals(_T(".inc") ) ) 
		{
			return true ; 
		}
		return false ; 
	}
	bool is_js() const
	{
		return equals( _T(".js") ) ;
	}
	bool is_vbs() const
	{
		return equals( _T(".vbs") ) ;
	}
	bool is_script() const
	{
		return is_js() || is_vbs()  || equals( _T(".au3") ) || is_perl() || is_python() ;
	}
	bool is_rtf() const
	{
		return equals( _T(".rtf") ) ;
	}
protected:
private:
};

/** A simple class for parsing filenames */
class name
{
	CString m_name ;	// the filename

public:

	/** default constructor. 
	*/
	name()
	{
	}
	/** constructor. 
	@param rhs parameter. 
	*/
	name( const CString &rhs )
	{
		internal_copy( rhs ) ;
	}

	/** constructor. 
	@param rhs parameter. 
	*/
	name( const name &rhs)
	{
		m_name = rhs.m_name ;
	}

	/** assignment operator. 
	@return name & reference to self. 
	@param rhs parameter. 
	*/
	name & operator=(const name &rhs)
	{
		m_name = rhs.m_name ;
		return *this ;
	}

	/** assignment operator. 
	@return name a file name. 
	@param name parameter. 
	*/
	name & operator=( const CString &rhs )
	{
		internal_copy( rhs ) ;
		return *this ;
	}
	// data access

	/** brief. 
	@return CString & description. 
	*/
	const CString & file_name() const
	{
		return m_name ;
	}

	/** brief. 
	@return CString description. 
	@param void parameter. 
	*/
	CString file_name_part( ) const
	{
		const int pos = m_name.ReverseFind( TEXT('.') ) ;
		if (pos == -1)
		{
			return m_name ;
		}
		return m_name.Left( pos ) ;
	}

	/** Get the extension.
	Returns the file extension, including the dot separator (e.g. ".txt").
	@return CString description. 
	@param  parameter. 
	*/
	CString extension_part( ) const
	{
		return CString( ::PathFindExtension( (LPCTSTR)m_name ) ) ;
	}

	/** Finds whether the path has a file extension. 
	@return bool whether the path has a file extension. 
	@param  parameter. 
	*/
	bool has_extension( ) const
	{
		return ( *(::PathFindExtension( (LPCTSTR)m_name )) != 0 ) ;
	}

	/** brief. 
	@return operator CString& description. 
	*/
	operator const CString& () const
	{ 
		return file_name() ;
	}
private:
	// private helper function

	/** brief. 
	@return void description. 
	@param &name_label parameter. 
	*/
	void internal_copy( const CString &rhs )
	{
		int pos = rhs.ReverseFind( TEXT('\\') ) ;
		if ( pos == -1 )
		{
			m_name = rhs ;
		}
		else
		{
			m_name = rhs.Mid( pos+1 ) ;
		}
	}

} ;

/** Wrapper for file handle */
class file 
{
	CString	m_file_name ;
	misc_wrappers::handle m_file ;

public:

	enum BYTE_ORDER_MARK { LE_BOM, BE_BOM, UTF8_BOM, UTF7_BOM, UNKNOWN_BOM };


	/** detach the current file handle from the class. 
	@return HANDLE description. 
	@param  parameter. 
	*/
	HANDLE detach( ) 
	{
		HANDLE ret = m_file ;
		m_file.invalidate() ;
		return ret ;
	}


	/** default constructor. 
	*/
	file() 
	{
	}

	/** constructor. 
	@param h parameter. 
	*/
	file( misc_wrappers::handle h )
		:	m_file( h )
	{

	}

	/** destructor. 
	*/
	~file()
	{
		close() ;
	}


	/** Get the time the file was created. 
	Wraps GetFileTime().
	@return BOOL success. 
	@param lpCreationTime the file time is stored here. 
	*/
	BOOL get_creation_time( LPFILETIME lpCreationTime )
	{
		ATLASSERT( m_file.is_valid() ) ;
		return GetFileTime(
			m_file,			// handle to the file
			lpCreationTime,	// address of creation time
			NULL,			// address of last access time
			NULL			// address of last write time
			);

	}

	/** Get the last time the file was accessed. 
	@return BOOL success. 
	@param lpLastAccessTime parameter. 
	*/
	BOOL get_last_access_time( LPFILETIME lpLastAccessTime )
	{
		ATLASSERT( m_file.is_valid() ) ;
		return GetFileTime(
			m_file,				// handle to the file
			NULL,				// address of creation time
			lpLastAccessTime,	// address of last access time
			NULL				// address of last write time
			);

	}

	/** brief. 
	@return BOOL success. 
	@param lpLastWriteTime parameter. 
	*/
	BOOL get_last_write_time( LPFILETIME lpLastWriteTime )
	{
		ATLASSERT( m_file.is_valid() ) ;
		return GetFileTime(
			m_file,				// handle to the file
			NULL,				// address of creation time
			NULL,				// address of last access time
			lpLastWriteTime		// address of last write time
			);

	}

	//
	// static
	//

	/** brief. 
	@return bool delete_file description. 
	@param file_name parameter. 
	*/
	static bool delete_file ( const CString &file_name )
	{
		return !! ::DeleteFile( static_cast< LPCTSTR >( file_name ) );
	}

	/** brief. 
	@return bool success. 
	@param file_name parameter. 
	*/
	static bool exists( const CString &file_name )
	{
		if ( file_name.IsEmpty() ) 
			return false ;

		return  ( !! ::PathFileExists( static_cast< LPCTSTR >( file_name ) ) && 
			      ! ::PathIsDirectory( static_cast< LPCTSTR >( file_name ) ) ) ;
	}

	/** brief. 
	@return unsigned int description. 
	@param &file_name parameter. 
	*/
	static unsigned int size( const CString &file_name )
	{
		ATLASSERT( PathFileExists( static_cast< LPCTSTR >( file_name ) ) && 
			! PathIsDirectory( static_cast< LPCTSTR >( file_name ) ) ) ;

		const DWORD disposition = OPEN_EXISTING, 
			flags = FILE_ATTRIBUTE_NORMAL, 
			generic_privileges = GENERIC_READ | GENERIC_WRITE  ;

		misc_wrappers::handle file_handle  = 
			::CreateFile( static_cast< LPCTSTR >( file_name ), generic_privileges, 0, NULL, disposition, flags, NULL );

		ATLASSERT( file_handle.is_valid() ) ;

		if ( ! file_handle.is_valid() )
		{
			THROW_FILE_EXCEPTION( _T("Failed to open file [") + file_name + _T("]") ) ;
		}

		const DWORD file_size = ::GetFileSize( file_handle, NULL ) ; 

		ATLASSERT( file_size != INVALID_FILE_SIZE ) ;

		if ( file_size == INVALID_FILE_SIZE )
		{
			THROW_FILE_EXCEPTION( _T("Failed to get size of file ") + file_name ) ;
		}

		file_handle.close() ;
		return (unsigned int)file_size ;

	}

	/** wrapper for SetEndOfFile. 
	Returns false if the file is not open or SetEndOfFile fails.
	@return bool success. 
	*/
	bool write_eof()
	{
		ATLASSERT( is_open() ) ;
		if ( ! is_open() )
			return false ;

		bool set_eof_success(false) ;
		set_eof_success =  ( FALSE != ::SetEndOfFile( m_file ) ) ;

		ATLASSERT( set_eof_success ) ;

		return set_eof_success ;
	}

	/** brief. 
	@return unsigned int description. 
	*/
	unsigned int size()
	{ 
		ATLASSERT( is_open() ) ;

		const DWORD file_size = ::GetFileSize( m_file, NULL ) ; 

		ATLASSERT( file_size != INVALID_FILE_SIZE ) ;

		if ( file_size == INVALID_FILE_SIZE )
		{
			THROW_FILE_EXCEPTION( _T("Failed to get size of file ") + m_file_name ) ;
		}

		return file_size ;
	}

	/** brief. 
	@return bool success. 
	*/
	bool is_open() const
	{ 
		return m_file.is_valid() ;
	}

	/** creates a a file, deleting the existing one if any. 
	@return bool success. 
	@param &file_name parameter. 
	*/
	bool create( const CString &file_name )
	{
		ATLASSERT( ! PathFileExists( file_name ) ) ;
		ATLASSERT( ! PathIsDirectory( file_name ) ) ;

		m_file_name = file_name ;
		return internal_open( CREATE_ALWAYS ) ;
	}
	/** creates a non_existing file. 
	@return bool success. 
	@param &file_name parameter. 
	*/
	bool create_new( const CString &file_name )
	{
		ATLASSERT( ! PathFileExists( file_name ) ) ;
		ATLASSERT( ! PathIsDirectory( file_name ) ) ;

		m_file_name = file_name ;
		return internal_open( CREATE_NEW ) ;
	}

	/** brief. 
	@return bool success. 
	@param &file_name parameter. 
	*/
	bool open( const CString &file_name )
	{
		CPath path = file_name ;
		ATLASSERT( path.Path() == file_name ) ;
		ATLASSERT( ! path.IsDirectory() ) ;

		if ( path.FileExists() )
		{
			return open_existing( file_name ) ;
		}
		else
		{
			return create_new( file_name ) ;
		}
	}

	bool open_read( const CString &file_name )
	{
		m_file_name = file_name ;
		return internal_open(OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, GENERIC_READ) ;
	}
	/** Open the file, create if doesn't exist.
	Returns true if file is successfully opened, throws on fail. 
	@return bool success. 
	@param &file_name parameter. 
	@param flags parameter. 
	*/
	bool open_always(const CString &file_name, 
					 const DWORD flags = FILE_ATTRIBUTE_NORMAL)
	{
		m_file_name = file_name ;
		return internal_open( OPEN_ALWAYS, flags ) ;
	}


	/** brief. 
	@return bool success. 
	@param &file_name parameter. 
	*/
	bool open_existing( const CString &file_name )
	{
		ATLASSERT( PathFileExists( file_name ) && ! PathIsDirectory( file_name ) ) ;
		m_file_name = file_name ;

		return internal_open( OPEN_EXISTING ) ;
	}

	/** brief. 
	@return bool success. 
	*/
	bool close()
	{
		if ( ! is_open() )
			return true ; // ok to close a file twice, just like it's ok to delete a null pointer

		m_file_name.Empty()  ;
		return m_file.close() ;
	}

	/** brief. 
	@return CString description. 
	@param void parameter. 
	*/
	const CString &file_name( void ) const
	{ 
		return m_file_name ; 
	}

	/** move to end of file. 
	@return unsigned long description. 
	*/
	const unsigned long end()
	{
		ATLASSERT( is_open() ) ;

		if ( ! is_open() )
			return 0 ;

		return set_file_pos( 0, FILE_END ) ;
	}

	/** move to beginning of file. 
	@return unsigned long description. 
	*/
	const unsigned long begin()
	{
		ATLASSERT( is_open() ) ;

		if ( ! is_open() )
			return 0 ;

		return set_file_pos( 0 ) ;
	}

	/** set_file_pos (seek)
	*/
	const unsigned long set_file_pos(const LONG pos1, 
									 const DWORD how = FILE_BEGIN, 
									 LONG *pos2=NULL)
	{
		ATLASSERT( is_open() ) ;

		SetLastError( 0 ) ;
		const unsigned long pos = ::SetFilePointer(
			m_file,			// handle of file
			(LONG) pos1,	// number of bytes to move file pointer
			pos2,			// pointer to high-order DWORD of distance to move
			how				// how to move
			);

		//	If the function fails and lpDistanceToMoveHigh is NULL, the return value is 0xFFFFFFFF. 
		ATLASSERT( pos != INVALID_FILE_SIZE ) ;

		if ( pos == INVALID_FILE_SIZE )
		{
			if ( ::GetLastError() != 0 )
				throw except::CFileException( _T("Failed to set file pointer of file ") + m_file_name ) ;
		}
		return pos ;
	}

	// reading / writing
	// writing to file

	/** brief. 
	@return bool success. 
	*/
	bool write_16le_bom()
	{
		ATLASSERT( is_open() ) ;

		if ( ! is_open() )
			return false ;

		begin() ;

		BYTE buf[] = { 0xFF, 0xFE } ;

		return ( 2 == write( buf, 2 ) ) ;
	}

	/** brief. 
	@return bool success. 
	*/
	bool write_16be_bom()
	{
		ATLASSERT( is_open() ) ;

		if ( ! is_open() )
			return false ;

		begin() ;

		BYTE buf[] = { 0xFE, 0xFF } ;

		return ( 2 == write( buf, 2 ) ) ;
	}

	/** brief. 
	@return bool success. 
	*/
	bool write_utf8_bom()
	{
		ATLASSERT( is_open() ) ;

		if ( ! is_open() )
			return false ;

		begin() ;

		BYTE buf[] = { 0xEF, 0xBB, 0xBF } ;

		return ( 3 == write( buf, 3 ) ) ;

	}

	/** Get the byte-order mark. 
	@return BYTE_ORDER_MARK description. 
	*/
	BYTE_ORDER_MARK get_bom()
	{
		ATLASSERT( is_open() ) ;

		if ( ! is_open() )
			return UNKNOWN_BOM ;

		BYTE b[5] ;

		ZeroMemory( b, sizeof( b ) ) ;

		begin() ;

		read( b, 5 ) ;

		switch( b[0] )
		{
		case 0xFE:
			if ( b[1] == 0xFF )
			{
				set_file_pos( 2 ) ;
				return BE_BOM ;
			}
			begin() ;
			return UNKNOWN_BOM ;

		case 0xEF:
			if ( b[1] == 0xBB && b[2] == 0xBF )
			{
				set_file_pos( 3 ) ;
				return UTF8_BOM ;
			}

			begin() ;
			return UNKNOWN_BOM ;

		// some apps mistakenly set this BOM.
		// we will let it slide for now...
		case 0x3F:
			if ( b[1] == 0xBB && b[2] == 0xBF )
			{
				set_file_pos( 3 ) ;
				return UTF8_BOM ;
			}

			begin() ;
			return UNKNOWN_BOM ;

		case 0x2B:
			if ( b[1] == 0x2F &&
				b[2] == 0x76 &&
				b[3] == 0x38 &&
				b[4] == 0x2D 
				)
			{
				return UTF7_BOM ;
			}

			begin() ;

			return UNKNOWN_BOM ;

		case 0xFF:
			if ( b[1] == 0xFE )
			{
				set_file_pos( 2 ) ;
				return LE_BOM ;
			}

			begin() ;
			return UNKNOWN_BOM ;

		default:
			begin() ;
			return UNKNOWN_BOM ;
		}

	}

	/** brief. 
	@return operator HANDLE description. 
	*/
	operator HANDLE ()
	{
		return m_file ;
	}

	/** stream in. 
	@return file &operator description. 
	@param buf parameter. 
	*/
	file &operator << ( LPCSTR buf )
	{
		write( buf, strlen( buf ) ) ;
		return *(this) ;
	}


	/** brief. 
	@return file &operator description. 
	@param &buf parameter. 
	*/
	file &operator << ( const string &buf )
	{
		write( buf ) ;
		return *(this) ;
	}


	/** brief. 
	@return file &operator description. 
	@param &buf parameter. 
	*/
	file &operator << ( const wstring &buf )
	{
		write( buf ) ;
		return *(this) ;
	}


	/** brief. 
	@return file &operator description. 
	@param i parameter. 
	*/
	file &operator << ( const int i )
	{
		write( int2string( i ) ) ;
		return *(this) ;
	}


	/** brief. 
	@return file &operator description. 
	@param d parameter. 
	*/
	file &operator << ( const double d )
	{
		write( double2string( d ) ) ;
		return *(this) ;
	}

	// write

	/** brief. 
	@return DWORD write description. 
	@param &buf parameter. 
	*/
	DWORD write ( const wstring &buf )
	{
		return write ( ( const LPBYTE )(buf.c_str()), 2 * buf.length() ) ;
	}


	/** brief. 
	@return DWORD write description. 
	@param &buf parameter. 
	*/
	DWORD write ( const string &buf )
	{
		return write ( ( const LPBYTE )buf.c_str(), buf.size() ) ;
	}


	/** brief. 
	@return DWORD description. 
	@param *text parameter. 
	@param size parameter. 
	*/
	DWORD write( const char *text, const size_t size )
	{
		ATLASSERT( text != NULL || size == 0 ) ;

		return write( ( const LPBYTE )text, size ) ;
	}


	/** brief. 
	@return DWORD description. 
	@param text parameter. 
	@param size parameter. 
	*/
	DWORD write( const LPBYTE text, const size_t size )
	{
		ATLASSERT( is_open() ) ;

		if ( text == NULL )
		{
			ATLASSERT( size == 0 ) ;
			return 0 ;
		}
		DWORD written ;

		if ( FALSE == ::WriteFile( m_file, text, static_cast< DWORD >( size ), &written, NULL ) )
			THROW_FILE_EXCEPTION( _T("Failed to write to file ") + m_file_name ) ;

		return written ;
	}

	//////////////////////////////////////////////////////////////////////////
	// reading from file
	//////////////////////////////////////////////////////////////////////////

	/** read the specified number of characters into the buffer. 
	@return unsigned long description. 
	@param buf parameter. 
	@param size parameter. 
	*/
	const unsigned long read ( LPSTR buf, const unsigned long size ) 
	{
		ATLASSERT( is_open() ) ;

		return read( ( BYTE * )buf, size ) ;
	}

	/** read the specified number of bytes into the buffer. 
	@return unsigned long description. 
	@param read_buf parameter. 
	@param read_size parameter. 
	*/
	const unsigned long read( LPBYTE read_buf, const unsigned long read_size)
	{
		ATLASSERT( is_open() ) ;

		unsigned long bytes_read ;
		BOOL read_file_success = ::ReadFile(m_file, read_buf, read_size, &bytes_read, NULL ) ;

		ATLASSERT( read_file_success ) ;

		if ( ! read_file_success )
		{
			CString error_msg( _T("Failed to read from file ") ) ;
			error_msg += m_file_name ;
			THROW_FILE_EXCEPTION( error_msg )	;
		}

		return bytes_read ;
	}
	// private helper functions


	/** get the file pos. 
	@return unsigned long description. 
	*/
	const unsigned long file_pos()
	{
		ATLASSERT( is_open() ) ;

		const unsigned long fptr = ::SetFilePointer( 
			m_file,			// must have GENERIC_READ and/or GENERIC_WRITE 
			0,				// do not move pointer 
			NULL,			// m_file is not large enough to need this pointer 
			FILE_CURRENT);  // provides offset from current position 

		WINERROR_SENTINEL ;

		//	If the function fails and lpDistanceToMoveHigh is NULL, the return value is 0xFFFFFFFF. 
		ATLASSERT( fptr != INVALID_FILE_SIZE ) ;

		if ( fptr == INVALID_FILE_SIZE )
			THROW_FILE_EXCEPTION( _T("Failed to set file pointer of file ") + m_file_name ) ;

		return fptr ;
	}

	/** Get the size of a given BOM. 
	@return UINT the size of the BOM. 
	*/
	static UINT bom_size( const BYTE_ORDER_MARK bom )
	{
		switch( bom ) 
		{
		case BE_BOM:
			return 2u ;
		case LE_BOM:
			return 2u ;
		case UTF8_BOM:
			return 3u ;
		case UTF7_BOM:
			return 5u ;
		default:
			return 0u ;
		}
	}

private:

	/** brief. 
	@return bool success. 
	@param disposition parameter. 
	@param flags parameter. 
	@param generic_privileges parameter. 
	*/
	bool internal_open( 
		const DWORD disposition, 
		const DWORD flags = FILE_ATTRIBUTE_NORMAL, 
		const DWORD desired_access = GENERIC_WRITE  )
	{
		// FILE_ATTRIBUTE_NORMAL valid only if used alone
		if ( is_open() ) 
		{
			CString tmp_name = m_file_name ;
			close() ;
			m_file_name = tmp_name ;
		}

		m_file = ::CreateFile( (LPCTSTR)m_file_name, 
							   desired_access, 
							   0, 
							   NULL, 
							   disposition, 
							   flags, 
							   NULL );

		ATLASSERT( is_open() ) ;

		if ( ! m_file.is_valid() )
			THROW_FILE_EXCEPTION( _T("Failed to open file ") + m_file_name ) ;

		return true ;
	}

} ;



/** Wrapper for file mapping */
class view
{
	LPVOID		m_view ;

		public:

	/** default constructor. 
	*/
	view() : 
		m_view( NULL )
	{
	}


	/** destructor. 
	*/
	~view() 
	{ 
		dispose_of_view() ; 
	}


	/** brief. 
	@return LPVOID description. 
	*/
	LPVOID get_open_view()
	{
		return m_view ;
	}


	/** brief. 
	@return LPVOID description. 
	@param file_name parameter. 
	*/
	LPVOID create_view( const CString & file_name )
	{
		dispose_of_view() ;

		misc_wrappers::handle file_handle = ::CreateFile( (LPCTSTR)file_name, 
														  GENERIC_READ | GENERIC_WRITE,	
														  0, 
														  NULL, 
														  OPEN_EXISTING, 
														  FILE_ATTRIBUTE_NORMAL, 
														  NULL );
		ATLASSERT( file_handle.is_valid()  ) ;
		if ( ! file_handle.is_valid() )
		{
			THROW_FILE_EXCEPTION( _T("Failed to create file ") + file_name ) ;
		}

		misc_wrappers::handle map_handle = ::CreateFileMapping( file_handle, 
																NULL, 
																PAGE_WRITECOPY, 
																0, 
																0, 
																NULL );
		ATLASSERT( ! map_handle.is_null()  ) ;
		if ( map_handle.is_null() )
		{
			THROW_FILE_EXCEPTION( _T("Failed to create file mapping for file ") + file_name ) ;
		}

		m_view = ::MapViewOfFile( map_handle, FILE_MAP_COPY, 0, 0, 0 );
		ATLASSERT( m_view != NULL ) ;
		if ( m_view == NULL )
		{
			THROW_FILE_EXCEPTION( _T("Failed to map view of file ") + file_name ) ;
		}

		return m_view ;
	}

	/** brief. 
	@return LPVOID description. 
	@param file_name parameter. 
	*/
	const LPVOID create_view_readonly( const CString & file_name )
	{
		dispose_of_view() ;

		misc_wrappers::handle file_handle = ::CreateFile( (LPCTSTR)file_name, 
														  GENERIC_READ, 
														  0, 
														  NULL, 
														  OPEN_EXISTING, 
														  FILE_ATTRIBUTE_READONLY, 
														  NULL );
		ATLASSERT( file_handle.is_valid()  ) ;
		if ( ! file_handle.is_valid() )
		{
			THROW_FILE_EXCEPTION( _T("Failed to create file ") + file_name ) ;
		}

		misc_wrappers::handle map_handle = ::CreateFileMapping( file_handle, NULL, PAGE_READONLY, 0, 0, NULL );
		ATLASSERT( ! map_handle.is_null()  ) ;
		if ( map_handle.is_null() )
		{
			THROW_FILE_EXCEPTION( _T("Failed to create file mapping for file ") + file_name ) ;
		}

		m_view = ::MapViewOfFile( map_handle, FILE_MAP_READ, 0, 0, 0 );
		ATLASSERT( m_view != NULL ) ;
		if ( m_view == NULL )
		{
			THROW_FILE_EXCEPTION( _T("Failed to map view of file ") + file_name ) ;
		}

		return m_view ;
	}
	/** brief. 
	@return bool success. 
	*/
	bool dispose_of_view()
	{
		BOOL file_unmapping_success = TRUE ;

		CLEAR_WINERRORS ;

		if ( m_view != NULL )
		{
			file_unmapping_success = ::UnmapViewOfFile( m_view ) ;
		}
		ATLASSERT( file_unmapping_success ) ;

		WINERROR_SENTINEL ;

		m_view = NULL ;

		return file_unmapping_success != FALSE ;
	}


	/** brief. 
	@return static bool success. 
	@param file_a parameter. 
	@param file_b parameter. 
	*/
	static bool files_are_equal( const CString& file_a, const CString& file_b )
	{
		// create the first file
		CLEAR_WINERRORS ;
		misc_wrappers::handle file_handle = ::CreateFile( file_a, GENERIC_READ | GENERIC_WRITE,	0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,	NULL );
		WINERROR_SENTINEL ;
		ATLASSERT( file_handle.is_valid() ) ;

		// map it
		misc_wrappers::handle map_handle = ::CreateFileMapping( file_handle, NULL, PAGE_WRITECOPY, 0, 0, NULL );
		WINERROR_SENTINEL ;
		ATLASSERT( map_handle.is_null() == false ) ;

		LPVOID view_a = ::MapViewOfFile( map_handle, FILE_MAP_COPY, 0, 0, 0 ) ;
		WINERROR_SENTINEL ;
		ATLASSERT( view_a != NULL ) ;
		
		// we don't need these guys any more
		file_handle.close() ;
		map_handle.close() ;

		// create the second file
		file_handle = ::CreateFile( file_b, GENERIC_READ | GENERIC_WRITE,	0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,	NULL );
		ATLASSERT( file_handle.is_valid() ) ;
		WINERROR_SENTINEL ;
		// etc...
		map_handle = ::CreateFileMapping( file_handle, NULL, PAGE_WRITECOPY, 0, 0, NULL );
		WINERROR_SENTINEL ;
		ATLASSERT( map_handle.is_null() == false ) ;

		LPVOID view_b = ::MapViewOfFile( map_handle, FILE_MAP_COPY, 0, 0, 0 ) ;
		WINERROR_SENTINEL ;
		ATLASSERT( view_b != NULL ) ;

		// compare the files using strcmp
		bool retval ;
		if ( view_a == view_b ) retval = true ;
		else if ( ! ( view_a && view_b ) ) retval = false ;
		else retval = ( ! strcmp( (const char*)view_a, (const char*)view_b ) ) ;

		// dispose of our views
		BOOL file_unmapping_success = FALSE ;

		if (view_a)
		{
			file_unmapping_success = ::UnmapViewOfFile( view_a ) ;
			WINERROR_SENTINEL ;
			ATLASSERT( file_unmapping_success ) ;
		}
		if (view_b)
		{
			file_unmapping_success = ::UnmapViewOfFile( view_b ) ;
			WINERROR_SENTINEL ;
			ATLASSERT( file_unmapping_success ) ;
		}

		// return result
		return retval ;
	}


} ;

/** A wrapper for the Windows Open File dialog box's file list.
 The open file dialog tells you about multiple files selected
 by giving you a bunch of CStrings in a TCHAR array, each null-terminated.
 The final CString is terminated by two null chars.
 This class breaks that down into a directory and vector of file names.
 */
class OpenDlgList
{
	CPath			m_directory ;
public:
	typedef cstring_vector::iterator iterator ;
	typedef cstring_vector::const_iterator const_iterator ;
	cstring_vector	m_filenames ;

	/** constructor. 
	@param file_directory_and_specs parameter. 
	*/
	OpenDlgList( LPCTSTR file_directory_and_specs = NULL )
	{
		if ( file_directory_and_specs != NULL )
		{
			parse_file_spec( (LPCTSTR)file_directory_and_specs ) ;
		}
	}

	/** brief. 
	@return OpenDlgList description. 
	@param file_directory_and_specs parameter. 
	*/
	OpenDlgList &operator=( LPCTSTR file_directory_and_specs )
	{
		parse_file_spec( file_directory_and_specs ) ;
		return *this ;
	}


	/** brief. 
	@return const CString description. 
	@param  parameter. 
	*/
	const CString directory( ) const
	{ 
		return m_directory.Path() ; 
	}

	/** The size of the vector. 
	*/
	size_t size( ) const
	{ 
		return ( m_filenames.size() ) ; 
	}

	/** Indexed access. 
	*/
	const CString operator[] (size_t index ) const
	{
		return m_filenames[index] ;
	}

private:

	/** parse the string into a directory and file specs. 
	@return void description. 
	@param file_directory_and_specs parameter. 
	*/
	void parse_file_spec( LPCTSTR file_directory_and_specs )
	{
		if ( file_directory_and_specs == NULL )
		{
			throw except::CProgramException( _T("NULL file spec") ) ;
		}
		m_filenames.clear() ;

		m_directory = CString( file_directory_and_specs ) ;

		// forward past the directory
		file_directory_and_specs += m_directory.Path().GetLength() + 1 ; // +1 is to eat the NULL

		while ( *file_directory_and_specs )
		{
			CString file_spec( file_directory_and_specs ) ;
			CPath full_path ;
			full_path.Combine( m_directory.Path(), file_spec ) ;
			m_filenames.push_back( full_path.Path() ) ;

			file_directory_and_specs += file_spec.GetLength() + 1 ; // +1 is to eat the NULL
		}

		// Only 1 file
		if ( m_filenames.empty() )
		{
			m_filenames.push_back( m_directory.Path() ) ;
			m_directory.RemoveFileSpec() ;
		}

		m_directory.AddBackslash() ;

	} 

} ;

}
