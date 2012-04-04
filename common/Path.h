/*!
	@file Path.h
	@brief interface and implementation of the CPath class.
	@date 2005/06/25
	Time: 15:29:12
	@author Ryan Ginstrom
 */

#pragma once

#include "DebugUtilities.h"

namespace file
{

	/**
	@class CPath  
	@brief Wrapper for Path API functions.
	 */
	class CPath  
	{

		CString m_path ;

	public:

		CPath& operator=( const CPath &path )
		{
			if ( m_path == path.m_path ) 
			{
				return *this ;
			}

			m_path = path.m_path ;
			return *this ;
		}
		CPath& operator=(const _bstr_t &path )
		{
			CW2CT szMyString( path );
			m_path = szMyString ;
			return *this ;
		}
		CPath& operator=( const CString &path )
		{
			m_path = path ;
			return *this ;
		}

		const CString Path() const
		{
			return m_path ;
		}
		
		void make( const CString &drive, const CString &dir, const CString &file_spec, const CString &ext )
		{
			ATLASSERT( drive.GetLength() + dir.GetLength() + file_spec.GetLength() + ext.GetLength() + 2 /* for slashes */ < _MAX_PATH ) ;

			TCHAR arg1[_MAX_PATH] ;

			_tmakepath_s( arg1, MAX_PATH, drive, dir, file_spec, ext ) ;
			
			m_path = arg1 ;
		}

		void split( CString &root, CString &dir, CString &file_spec, CString &ext )
		{
			TCHAR drive_arg[_MAX_DRIVE] = { 0 } ;
			TCHAR dir_arg[_MAX_DIR] = { 0 } ;
			TCHAR file_spec_arg[_MAX_FNAME] = { 0 } ;
			TCHAR ext_arg[_MAX_EXT]  = { 0 } ;

			_tsplitpath_s( m_path, drive_arg, _MAX_DRIVE, dir_arg, _MAX_DIR, file_spec_arg, _MAX_FNAME, ext_arg, _MAX_EXT ) ;

			root		= drive_arg ;
			dir			= dir_arg ;
			file_spec	= file_spec_arg ;
			ext			= ext_arg ;
		}

		BOOL AddBackslash()
		{
			int len_needed = m_path.GetLength() + 1 ;
			// result will point to the end of the string
			LPCTSTR result = PathAddBackslash( m_path.GetBuffer( len_needed ) ) ;
			m_path.ReleaseBuffer() ;

			ATLASSERT( m_path.Right(1) == _T("\\") ) ;
			ATLASSERT( *result == 0 ) ;

			return *result == 0 ;
		}
		BOOL AddExtension( const CString &extension )
		{
			ATLASSERT( extension.GetAt(0) == _T('.') ) ;
			int len_needed = m_path.GetLength() + extension.GetLength() + 1	; // +1 is for dot
			BOOL res = PathAddExtension( m_path.GetBuffer( len_needed ), extension )  ;
			ATLASSERT( res ) ;

			m_path.ReleaseBuffer() ;
			return res ;
		}

		/*! Appends one path to the end of another. 
		* This function automatically inserts a backslash between the two 
		* strings, if one is not already present.
		*/
		BOOL Append( const CString appended_path )
		{
			int len_needed = m_path.GetLength() + appended_path.GetLength() + 2 ;
			BOOL res = PathAppend( m_path.GetBuffer( len_needed ), appended_path ) ;
			ATLASSERT( res ) ;
			
			m_path.ReleaseBuffer() ;
			return res ;
		}
		BOOL BuildRoot( int drive_num )
		{
			TCHAR _buf[4] = _T("") ;
			m_path = ::PathBuildRoot( _buf, drive_num ) ;
			ATLASSERT( ! m_path.IsEmpty() ) ;
			
			return ! m_path.IsEmpty()  ;
		}
		BOOL Canonicalize()
		{
			int len_needed = MAX_PATH ;
			CString src = m_path ;
			BOOL res = PathCanonicalize( m_path.GetBuffer( len_needed ), src ) ;
			ATLASSERT( res ) ;
			
			m_path.ReleaseBuffer() ;
			return res ;
		}
			
		//		lpszDest
		//			[out] Pointer to a buffer with the null-terminated string to hold the combined path string. You should set the size of this buffer to MAX_PATH to ensure that it is large enough to hold the returned string. 
		//		lpszDir
		//			[in] Pointer to a null-terminated string of maximum length MAX_PATH that contains the directory path. 
		//		lpszFile
		//			[in] Pointer to a null-terminated string of maximum length MAX_PATH that contains the file path. 
		//		Return Value
		//			Returns a pointer to a string with the concatenated path if successful, or NULL otherwise. 
		//		
		//		Remarks
		//			The directory path should be in the form of A:,B:, ..., Z:. The file path should be in a correct form that represents the file part of the path. The file path must not be null, and if it ends with a backslash, the backslash will be maintained. 
		BOOL Combine( const CString &directory_part, const CString &file_part )
		{
			int len_needed = directory_part.GetLength() + file_part.GetLength() + 1 ;
			LPCTSTR res = PathCombine( m_path.GetBuffer( len_needed ), 
				directory_part, 
				file_part ) ;
			ATLASSERT( res != 0 ) ;
			
			m_path.ReleaseBuffer() ;
			return res != 0 ;
		}

		// dx = width of string in pixels
		BOOL CompactPath( HDC dc, UINT dx )
		{
			int len_needed = m_path.GetLength() + 1 ;
			BOOL res = PathCompactPath( dc, m_path.GetBuffer( len_needed ), dx ) ;
			ATLASSERT( res ) ;
			
			m_path.ReleaseBuffer() ;
			return res ;
		}

		// Compact to no more than cchMax characters
		// value cchMax includes terminating NULL
		BOOL CompactPathEx( UINT cchMax  )
		{
			int len_needed = m_path.GetLength() + 1 ;
			CString out_buf ;
			BOOL res = PathCompactPathEx( out_buf.GetBuffer( len_needed ), m_path, cchMax, 0 ) ;
			ATLASSERT( res ) ;
			
			out_buf.ReleaseBuffer() ;
			m_path = out_buf ;
			return res ;
		}

		const CString CommonPrefix( const CString &rhs ) const
		{
			TCHAR out_buf[MAX_PATH] ;
			LPTSTR buf = out_buf ;
			int len_copied = 0 ;
			len_copied = PathCommonPrefix( m_path, rhs, buf ) ;
			if ( buf == NULL )
			{
				ATLASSERT( len_copied == 0 ) ;
				return CString() ;
			}
			ATLASSERT( len_copied == lstrlen( buf ) ) ;
			return CString( buf ) ;
		}

		BOOL FileExists() const
		{
			return PathFileExists( m_path ) ;
		}

		const CString FindExtension() const
		{
			LPCTSTR ext = PathFindExtension( m_path ) ;
			if ( ext == NULL )
			{
				return CString() ;
			}
			return CString( ext ) ;
		}

		const CString FindFileName() const
		{
			LPCTSTR fname = PathFindFileName( m_path ) ;
			if ( fname == NULL )
			{
				return CString() ;
			}
			return CString( fname ) ;
		}

		const CString FindNextComponent() const
		{
			LPCTSTR next_comp = PathFindNextComponent( m_path ) ;
			if ( next_comp == NULL )
			{
				return CString() ;
			}
			return CString( next_comp ) ;
		}

		HRESULT CreateFromUrl( const CString &url )
		{
			DWORD len = _MAX_PATH ;

			HRESULT hr = PathCreateFromUrl(  url,
			m_path.GetBuffer( _MAX_PATH ),
			&len,
			NULL
			);
			
			ASSERT_HRESULT( hr ) ;

			m_path.ReleaseBuffer( len ) ;

			return hr ;
		}
		
		const CString GetArgs() const
		{
			LPCTSTR args = PathGetArgs( m_path ) ;
			if ( args == NULL )
			{
				return CString() ;
			}
			return CString( args ) ;
		}

		//	GCT_INVALID The character is not valid in a path.  
		//	GCT_LFNCHAR The character is valid in a long file name.  
		//	GCT_SEPARATOR The character is a path separator.  
		//	GCT_SHORTCHAR The character is valid in a short (8.3) file name.  
		//	GCT_WILD The character is a wildcard character.  
		UINT GetCharTypeAt( const size_t index ) const
		{
			ATLASSERT( index < static_cast< size_t >( m_path.GetLength() ) ) ;

			if( index >= static_cast< size_t >( m_path.GetLength() )  )
			{
				return GCT_INVALID ;
			}

			return PathGetCharType( m_path.GetAt(static_cast< int >( index ) ) ) ;
		}

		// static methods
		static UINT GetCharType( const TCHAR c )
		{
			return PathGetCharType( c ) ;
		}
		static bool is_invalid_char_type( const TCHAR c )
		{
			return GCT_INVALID == PathGetCharType( c ) ;
		}
		static bool is_long_filename_char_type( const TCHAR c )
		{
			return GCT_LFNCHAR == PathGetCharType( c ) ;
		}
		static bool is_separator_char_type( const TCHAR c )
		{
			return GCT_SEPARATOR == PathGetCharType( c ) ;
		}
		static bool is_short_filename_char_type( const TCHAR c )
		{
			return GCT_SHORTCHAR == PathGetCharType( c ) ;
		}
		static bool is_wild_char_type( const TCHAR c )
		{
			return GCT_WILD == PathGetCharType( c ) ;
		}


		int GetDriveNumber( ) const
		{
			return PathGetDriveNumber( m_path ) ;
		}
		BOOL IsContentType( const CString &content_type ) const
		{
			return PathIsContentType( m_path, content_type ) ;
		}
		BOOL IsDirectory() const
		{
			return PathIsDirectory( m_path ) ;
		}
		BOOL IsFileSpec() const
		{
			return PathIsFileSpec( m_path ) ;
		}
		BOOL IsHTMLFile() const
		{
			return PathIsHTMLFile( m_path ) ;
		}
		BOOL IsLFNFileSpec() const
		{
			return PathIsLFNFileSpec( m_path ) ;
		}
		BOOL IsNetworkPath() const
		{
			return PathIsNetworkPath( m_path ) ;
		}
		BOOL IsPrefix( const CString &prefix ) const
		{
			return PathIsPrefix ( prefix, m_path ) ;
		}
		BOOL IsRelative() const
		{
			return PathIsRelative( m_path ) ;
		}
		BOOL IsRoot() const
		{
			return PathIsRoot( m_path ) ;
		}
		BOOL IsSameRoot( const CString &cmp_root ) const
		{
			return PathIsSameRoot( m_path, cmp_root ) ;
		}
		BOOL IsSystemFolder() const
		{
			return PathIsSystemFolder( m_path, 0 ) ;
		}
		BOOL IsUNC() const
		{
			return PathIsUNC( m_path ) ;
		}
		BOOL IsUNCServer() const
		{
			return PathIsUNCServer ( m_path ) ;
		}
		BOOL IsUNCServerShare () const
		{
			return PathIsUNCServerShare ( m_path ) ;
		}
		BOOL IsURL() const
		{
			return PathIsURL( m_path ) ;
		}
		BOOL MakePretty()
		{
			int len_needed = m_path.GetLength() ;
			BOOL ret = PathMakePretty( m_path.GetBuffer( len_needed ) ) ;
			ATLASSERT( ret ) ;

			m_path.ReleaseBuffer() ;
			return ret ;
		}
		BOOL MakeSystemFolder()
		{
			BOOL ret = PathMakeSystemFolder( m_path.GetBuffer( MAX_PATH ) ) ;
			ATLASSERT( ret ) ;
			
			m_path.ReleaseBuffer() ;
			return ret ;
		}
		BOOL MatchSpec( const CString &spec ) const
		{
			return PathMatchSpec( m_path, spec ) ;
		}
		int ParseIconLocation( )
		{
			int index = PathParseIconLocation( m_path.GetBuffer( m_path.GetLength() ) ) ;
			m_path.ReleaseBuffer() ;
			return index ;
		}
		void QuoteSpaces()
		{
			int len_needed = m_path.GetLength() + 2 ;
			PathQuoteSpaces( m_path.GetBuffer( len_needed ) ) ;
			m_path.ReleaseBuffer() ;
		}
		const CString RelativePathTo( const CString &to ) const
		{
			DWORD attr_from(0) ;
			if ( IsDirectory() )
				attr_from = FILE_ATTRIBUTE_DIRECTORY ;

			CPath to_path( to ) ;

			DWORD attr_to(0) ;
			if ( to_path.IsDirectory() )
				attr_to = FILE_ATTRIBUTE_DIRECTORY ;

			CString out_path ;

			BOOL ret = PathRelativePathTo( out_path.GetBuffer( MAX_PATH ), m_path, attr_from, to, attr_to ) ;
			ATLASSERT( ret ) ;
			
			if ( ! ret )
			{
				return CString() ;
			}

			out_path.ReleaseBuffer() ;

			return out_path ;
		}
		void RemoveArgs()
		{
			int len_needed = m_path.GetLength() ;
			PathRemoveArgs ( m_path.GetBuffer( len_needed ) ) ;
			m_path.ReleaseBuffer() ;
		}
		BOOL RemoveBackslash()
		{
			int len_needed = m_path.GetLength() ;
			LPCTSTR res = ::PathRemoveBackslash( m_path.GetBuffer( len_needed ) ) ;
			
			m_path.ReleaseBuffer() ;

			return ( *res == 0 ) ;
		}
		void RemoveBlanks()
		{
			int len_needed = m_path.GetLength() ;
			PathRemoveBlanks( m_path.GetBuffer( len_needed ) ) ;
			m_path.ReleaseBuffer() ;
		}
		void RemoveExtension()
		{
			int len_needed = m_path.GetLength() ;
			PathRemoveExtension( m_path.GetBuffer( len_needed ) ) ;
			m_path.ReleaseBuffer() ;
		}
		BOOL RemoveFileSpec()
		{
			int len_needed = m_path.GetLength() ;
			BOOL res = PathRemoveFileSpec( m_path.GetBuffer( len_needed ) ) ;
			ATLASSERT( res ) ;
			
			m_path.ReleaseBuffer() ;
			return res ;
		}
		BOOL RenameExtension( const CString &ext )
		{
			int len_needed = m_path.GetLength() + ext.GetLength() ;
			BOOL res = PathRenameExtension( m_path.GetBuffer( len_needed ), ext ) ;

			m_path.ReleaseBuffer() ;
			
			ATLASSERT( res ) ;
			ATLASSERT( m_path.Right(ext.GetLength()) == ext ) ;
			ATLASSERT( m_path.Find( _T('.') ) != -1 ) ;

			return res ;
		}
		void SetDlgItemPath( HWND hDlg, int ctl_id )
		{
			PathSetDlgItemPath( hDlg, ctl_id, m_path ) ;
		}
		CString SkipRoot() const
		{
			LPCTSTR skipped = PathSkipRoot( m_path ) ;
			return CString( skipped ) ;
		}
		void StripPath()
		{
			int len_needed = m_path.GetLength() ;
			PathStripPath( m_path.GetBuffer( len_needed ) ) ;
			m_path.ReleaseBuffer() ;
		}
		BOOL StripToRoot()
		{
			int len_needed = m_path.GetLength() ;
			BOOL res = PathStripToRoot( m_path.GetBuffer( len_needed ) ) ;
			ATLASSERT( res ) ;

			m_path.ReleaseBuffer() ;
			return res ;
		}
		void Undecorate()
		{
			int len_needed = m_path.GetLength() ;
			::PathUndecorate( m_path.GetBuffer( len_needed ) ) ;
			m_path.ReleaseBuffer() ;
		}
		const CString UnExpandEnvStrings() const
		{
			CString unexpanded_string ;
			BOOL res = PathUnExpandEnvStrings( m_path, unexpanded_string.GetBuffer( _MAX_PATH ), _MAX_PATH ) ;
			ATLASSERT( res ) ;

			if ( ! res )
			{
				return CString() ;
			}

			unexpanded_string.ReleaseBuffer() ;

			return unexpanded_string ;

		}
		BOOL UnmakeSystemFolder()
		{
			int len_needed = m_path.GetLength() ;
			BOOL res = PathUnmakeSystemFolder( m_path.GetBuffer( len_needed ) ) ;
			ATLASSERT( res ) ;
			
			m_path.ReleaseBuffer() ;
			return res ;
		}
		void UnquoteSpaces()
		{
			int len_needed = m_path.GetLength() ;
			PathUnquoteSpaces( m_path.GetBuffer( len_needed ) ) ;
			m_path.ReleaseBuffer() ;
		}

		const CString GetLongName() const
		{
			// get the size needed
			// setting the second param to NULL violates function spec, but setting the third to 0
			// will still guarantee that the needed size is returned.
			CString long_path ;
			size_t size_needed = ::GetLongPathName( m_path, long_path.GetBuffer(0), 0 ) ;

			// get the long path name
			int len = ::GetLongPathName( m_path, long_path.GetBuffer( static_cast< int >( size_needed ) ), static_cast< DWORD >( size_needed )  ) ;
			long_path.ReleaseBuffer( len ) ;
		
			return long_path ;
		}
		const CString GetShortName() const
		{
			// get the size needed
			CString short_path ;
			size_t size_needed = ::GetShortPathName( m_path, short_path.GetBuffer(0), 0 ) ;
			
			// get the long path name
			int len = ::GetShortPathName( m_path, short_path.GetBuffer( static_cast< int >( size_needed ) ), static_cast< DWORD >( size_needed ) ) ;
			short_path.ReleaseBuffer( len ) ;
			
			return short_path ;
		}
		void MakeLong()
		{
			CString path = GetLongName() ;
			m_path = path ;
		}
		void MakeShort()
		{
			CString path = GetShortName() ;
			m_path = path ;
		}

		void ReplaceFileSpec( const CString &new_spec )
		{
			RemoveFileSpec() ;
			Append( new_spec ) ;
		}

		DWORD GetModFileName( HINSTANCE hInst )
		{
			DWORD len = ::GetModuleFileName( hInst, m_path.GetBuffer( _MAX_PATH ), _MAX_PATH ) ;
			m_path.ReleaseBuffer( len ) ;
			return len ;
		}
		
		void GetModulePath( HINSTANCE hInst )
		{
			GetModFileName( hInst ) ;
			RemoveFileSpec() ;
			AddBackslash() ;
		}

		BOOL Delete()
		{
			BOOL success = FALSE ;

			if ( IsDirectory() )
			{
				success = ::RemoveDirectory( m_path ) ;
			}
			else if ( FileExists() )
			{
				success =::DeleteFile( m_path ) ;
			}

			ATLASSERT( success ) ;

			return success ;
		}

		// construction 
		CPath() 
		{
		}
		CPath( const CPath &path ) : 
			m_path( path.m_path ) 
		{
		}
		CPath( const CString &path ) : 
			m_path( path ) 
		{
		}
		CPath( const _bstr_t &path ) :
			m_path( (LPCTSTR)path )
		{
		}


	};

}