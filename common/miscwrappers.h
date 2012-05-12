/*!
	@file MiscWrappers.h
	@brief wrappers for various Win32 classes/functions.

	Classes in this file:
	<ul>
	<li> date -- wrapper for SYSTEMTIME
	<li> version_info -- wrapper for public OSVERSIONINFO
	<li> handle -- wrapper for HANDLE
	</ul>
*/


#pragma once

#include "StringEx.h"
#include <iomanip>
#include "Exceptions.h"
#include "textstream_reader.h"

/**
	@namespace misc_wrappers
	@brief Various wrapper classes for Win32 stuff.
 */
namespace misc_wrappers
{

	/**
	 @class version_info 
	 @brief Wrapper for VERSIONINFO struct.
	 */
	class version_info : public OSVERSIONINFO 
	{
	public:
		 version_info()
		 {
			 dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			 GetVersionEx(this);
		 }
		 bool is_unicode_version()
		 {
			switch( dwMajorVersion ) // Major version number of the operating system. This member can be one of the following values. Operating System Meaning 
			{
			case 4 :	// Windows 95 
				// Windows 98 
				// Windows Me 4 
				// Windows NT 4.0
				if ( dwMinorVersion == 0 ) //  Windows NT 4.0
					return true ;
				return false ;
				
			case 3:		// Windows NT 3.51 
				if ( dwMinorVersion == 51 ) //  Windows NT 3.51
					return true ;
				
			case 5:		// Windows 2000 
				// Windows XP 
				// Windows Server 2003 family
				return true ;
			case 6: // Vista
				return true ;

			default:
				ATLTRACE( "This must be vista: %d - %d\n", this->dwMajorVersion, this->dwMinorVersion ) ;
				return true ;
			}

			ATLASSERT( "WE SHOULD NEVER GET HERE" && false ) ;
			return false ;
			
		 }
	} ;


	/** Wrapper for a Windows HANDLE.
	 */
	class handle
	{
		HANDLE m_handle ;

	public:
		handle( HANDLE handle = INVALID_HANDLE_VALUE )
			: m_handle( handle )
		{
		}

		~handle() 
		{
			if ( is_valid() )
			{
				close() ;
			}
		}

		bool is_valid() const
		{ 
			return ( m_handle != INVALID_HANDLE_VALUE && m_handle != NULL ) ; 
		}

		bool is_null() const
		{ 
			return ( m_handle == NULL ) ; 
		}

		handle &operator=( HANDLE h )
		{
			m_handle = h ;
			return *this ;
		}

		operator HANDLE()
		{
			return m_handle ;
		}

		bool operator == ( HANDLE h )
		{
			return m_handle == h ;
		}

		bool find_close()
		{
			ATLASSERT( is_valid() ) ;
			bool success = !! ::FindClose( m_handle ) ;
			invalidate() ;
			return success ;
		}
		bool close()
		{
			ATLASSERT( is_valid() ) ;
			bool success = !! ::CloseHandle( m_handle ) ;
			invalidate() ;
			return success ;
		}
		void invalidate()
		{
			m_handle = INVALID_HANDLE_VALUE ;
		}
	};								 


	inline __int64 Delta(const SYSTEMTIME st1, const SYSTEMTIME st2)
	{
		__int64 ft1 = 0;
		__int64 ft2 = 0;

		SystemTimeToFileTime(&st1, (LPFILETIME)&ft1);
		SystemTimeToFileTime(&st2, (LPFILETIME)&ft2);

		return ft1 - ft2;
	}

const static int TIME_BUF_SIZE = 50 ;
	/** Wrapper for SYSTEMTIME struct 
    */
	struct date : public SYSTEMTIME
	{
		// constructors 
		date( )
		{
			ZeroMemory( this, sizeof( SYSTEMTIME ) ) ;
		}
		date(WORD year, WORD month, WORD day, WORD hour=0, WORD minute=0, WORD second=0)
		{
			ZeroMemory( this, sizeof( SYSTEMTIME ) ) ;
			this->wYear = year ;
			this->wMonth = month ;
			this->wDay = day ;
			this->wHour = hour ;
			this->wMinute = minute ;
			this->wSecond = second ;
		}
		template< class CHAR_T >
		explicit date( const std::basic_string< CHAR_T > &date_string )
		{
			internal_convert( date_string ) ;
		}
		date( const date &copy )
		{ 
			internal_copy( &copy ) ;
		}
		explicit date( const SYSTEMTIME &copy )
		{ 
			internal_copy( &copy ) ;
		}
		explicit date( const FILETIME &copy )
		{
			FileTimeToSystemTime(
				&copy,	// pointer to file time to convert
				this    // pointer to structure to receive 
						// system time
				);
		}
		explicit date( const DATE &copy )
		{
			VariantTimeToSystemTime( copy, this ) ;
		}

		DATE get_variant_time()
		{
			DATE var_date ;
			SystemTimeToVariantTime( this, &var_date ) ;
			return var_date ;
		}

		template< class CHAR_T >
		unsigned short str2ushort(const std::basic_string< CHAR_T > &setting)
		{
			if (setting.empty())
			{
				return 0 ;
			}
			return static_cast<unsigned short>(boost::lexical_cast< long >( setting )) ;
		}
		// set 
		template< class CHAR_T >
		int set_year( const std::basic_string< CHAR_T > &setting ) 
		{ 
			this->wYear = str2ushort(setting) ;
			return this->wYear ;
		}
		template< class CHAR_T >
		int set_month( const std::basic_string< CHAR_T > &setting )
		{
			this->wMonth = str2ushort( setting ) ;
			return this->wMonth ;
		}
		template< class CHAR_T >
		int set_day( const std::basic_string< CHAR_T > &setting )
		{
			this->wDay = str2ushort( setting ) ;
			return this->wDay ;
		}
		template< class CHAR_T >
		int set_hour( const std::basic_string< CHAR_T > &setting )
		{
			this->wHour = str2ushort( setting ) ;
			return this->wHour ;
		}
		template< class CHAR_T >
		int set_minute( const std::basic_string< CHAR_T > &setting )
		{
			this->wMinute = str2ushort( setting ) ;
			return this->wMinute ;
		}
		template< class CHAR_T >
		int set_second( const std::basic_string< CHAR_T > &setting )
		{
			this->wSecond = str2ushort( setting ) ;
			return this->wSecond ;
		}
		template< class CHAR_T >
		int set_milliseconds( const std::basic_string< CHAR_T > &setting )
		{
			this->wMilliseconds = str2ushort( setting ) ;
			return this->wMilliseconds ;
		}
		const tstring get_date_time_string( LPCTSTR date_format = TEXT("yyyy'/'MM'/'dd"), LPCTSTR time_format = TEXT("HH':'mm':'ss"), LPCTSTR separator = TEXT(" ") ) const
		{
			return get_date_time_string( tstring( date_format ), tstring( time_format ), tstring( separator ) ) ;
		}
		const tstring get_date_time_string( const tstring &date_format, const tstring &time_format, const tstring &separator ) const
		{
			return get_date_string( date_format ) + separator + get_time_string( time_format ) ;
		}
		// get
		const tstring get_time_string( const tstring &time_format = tstring( TEXT("HH':'mm':'ss") ) ) const 
		{
			TCHAR time_buf[TIME_BUF_SIZE] = {0};
			int len = ::GetTimeFormat( LOCALE_USER_DEFAULT, 
				LOCALE_USE_CP_ACP, 
				static_cast< const SYSTEMTIME * >( this ), 
				time_format.c_str(), 
				time_buf, 
				TIME_BUF_SIZE ) ;
			if (len == 0)
			{
				THROW_WIN_EXCEPTION(ATL::CString("Failed to obtain PC time")) ;
			}
			time_buf[len] = 0 ;
			return tstring(time_buf) ;
		}
		const tstring get_date_string( const tstring &date_format = tstring( TEXT("yyyy'/'MM'/'dd") ) ) const 
		{
			TCHAR date_buf[TIME_BUF_SIZE] = {0};
			int len = ::GetDateFormat( LOCALE_USER_DEFAULT, 
				LOCALE_USE_CP_ACP, 
				static_cast< const SYSTEMTIME * >( this ), 
				date_format.c_str(), 
				date_buf, 
				TIME_BUF_SIZE ) ;
			if (len == 0)
			{
				THROW_WIN_EXCEPTION(ATL::CString("Failed to obtain PC date")) ;
			}
			date_buf[len] = 0 ;
			return tstring(date_buf) ;
		}
		const tstring get_default_date_time_string( LPCTSTR sep = _T(", ")) const
		{
			TCHAR time_buf[TIME_BUF_SIZE] = {0};
			::GetTimeFormat( LOCALE_USER_DEFAULT, 
				LOCALE_NOUSEROVERRIDE, 
				static_cast< const SYSTEMTIME * >( this ), 
				NULL, 
				time_buf, 
				TIME_BUF_SIZE ) ;
			tstring output_str = time_buf ;

			if ( sep == NULL )
			{
				output_str += _T(", ") ;
			}
			else
			{
				output_str += sep ;
			}

			TCHAR date_buf[TIME_BUF_SIZE] = {0};
			::GetDateFormat( LOCALE_USER_DEFAULT, 
				LOCALE_NOUSEROVERRIDE, 
				static_cast< const SYSTEMTIME * >( this ), 
				NULL, 
				date_buf, 
				TIME_BUF_SIZE ) ;
			output_str += date_buf ;

			return output_str ;
		}
		void set_to_local_time( )
		{
			::GetLocalTime( this ) ;
		}
		template< class CHAR_T >
		bool parse_formatted_string ( const std::basic_string< CHAR_T > &date_string, 
									  const bool pass_along_exceptions=false )
		{
			try
			{
				internal_convert( date_string ) ;
			}
			catch ( CException e )
			{
				e.add_to_message( TEXT("Date string: ") + string2tstring( date_string ) ) ;
				init_values() ;
				if ( pass_along_exceptions ) throw except::CException( e ) ;
				return false ;
			}
			return true ;
		}
		// extractors
		LPSYSTEMTIME operator&() { return (LPSYSTEMTIME)this ; }
		SYSTEMTIME &as_system_time() { return *this ; }

		// assignment operator
		template< class CHAR_T >
		date &operator=( const std::basic_string< CHAR_T > &date_string)
		{
			try
			{
				internal_convert( date_string ) ;
			}
			catch (except::CException &)
			{
				init_values() ;
			}
			return *this ;
		}
		date &operator= ( const date &copy)
		{
			internal_copy( &copy ) ;
			return *this ;
		}
		date &operator= ( const SYSTEMTIME &copy)
		{
			internal_copy( &copy ) ;
			return *this ;
		}
		date &operator= ( const FILETIME &ft )
		{
			FileTimeToSystemTime( &ft, this ) ;
			return *this ;
		}
	private:
		void internal_copy( const SYSTEMTIME *copy )
		{
			memcpy( this, copy, sizeof( SYSTEMTIME ) ) ;
		}
		void init_values(  )
		{
			ZeroMemory( this, sizeof( SYSTEMTIME ) ) ;
		}
		// internal_convert
		// Parameters
		//		const std::basic_string< CHAR_T > &date_string	-- formatted date/time string
		// Action
		//		Converts a formatted std::string into a SYSTEMTIME struct
		// Requirements
		//		DateString MUST have this format: yyyy/MM/dd HH:mm:ss[:mmm]
		template< class CHAR_T >
		bool internal_convert( const std::basic_string< CHAR_T > &date_string )
		{
			typedef CHAR_T						char_type ;
			typedef textstream_reader< CHAR_T > reader_type ;
			typedef std::basic_string< CHAR_T >	str_type ;

			init_values() ;

			reader_type reader ;
			reader.set_buffer( date_string.c_str() ) ;

			// wYear
			// yyyy/MM/dd HH:mm:ss[:mmm]
			const str_type year_chunk = reader.getline(char_type( '/' )) ;
			check_buffer_format( year_chunk ) ;
			set_year( year_chunk ) ;

			// wMonth
			// MM/dd HH:mm:ss[:mmm]
			const str_type month_chunk = reader.getline(char_type( '/' )) ;
			check_buffer_format( month_chunk ) ;
			set_month( month_chunk ) ;

			// wDay
			// dd HH:mm:ss[:mmm]
			const str_type day_chunk = reader.getline(char_type( ' ' )) ;
			check_buffer_format( day_chunk ) ;
			set_day( day_chunk ) ;

			// wHour
			// HH:mm:ss[:mmm]
			const str_type hour_chunk = reader.getline(char_type( ':' )) ;
			check_buffer_format( hour_chunk ) ;
			set_hour( hour_chunk ) ;

			// wMinute
			// mm:ss[:mmm]
			const str_type min_chunk = reader.getline(char_type( ':' )) ;
			check_buffer_format( min_chunk ) ;
			set_minute( min_chunk ) ;

			// wSecond
			// ss[:mmm]
			const str_type sec_chunk = reader.getline(char_type( ':' )) ;
			check_buffer_format( sec_chunk ) ;
			set_second( sec_chunk ) ;

			// wMilliseconds
			str_type msec_chunk = boost::trim_right_copy(reader.getline()) ;
			if ( msec_chunk.empty() == false )
			{
				check_buffer_format( msec_chunk ) ;
				set_milliseconds( msec_chunk ) ;
			}
			return true ;
		} ;

		template< class CHAR_T >
		void check_buffer_format( const std::basic_string< CHAR_T > &buf )
		{
			if ( ! str::is_int_rep( buf ) )
			{
				throw except::CException( TEXT("date: Bad date string format") ) ;
			}
		}
	} ;

	// =============================
	// operator <
	// =============================
	inline bool operator < ( const date &lhs, const SYSTEMTIME &rhs )
	{
		if ( lhs.wYear < rhs.wYear )
			return true ;
		if ( lhs.wYear > rhs.wYear )
			return false ;

		if ( lhs.wMonth < rhs.wMonth )
			return true ;
		if ( lhs.wMonth > rhs.wMonth )
			return false ;

		if ( lhs.wDay < rhs.wDay )
			return true ;
		if ( lhs.wDay > rhs.wDay )
			return false ;

		if ( lhs.wHour < rhs.wHour )
			return true ;
		if ( lhs.wHour > rhs.wHour )
			return false ;

		if ( lhs.wMinute < rhs.wMinute )
			return true ;
		if ( lhs.wMinute > rhs.wMinute )
			return false ;

		if ( lhs.wSecond < rhs.wSecond )
			return true ;
		if ( lhs.wSecond > rhs.wSecond )
			return false ;

		return ( lhs.wMilliseconds < rhs.wMilliseconds ) ;
	}

	inline bool operator < ( const SYSTEMTIME &lhs, const date &rhs )
	{
		if ( lhs.wYear < rhs.wYear )
			return true ;
		if ( lhs.wYear > rhs.wYear )
			return false ;

		if ( lhs.wMonth < rhs.wMonth )
			return true ;
		if ( lhs.wMonth > rhs.wMonth )
			return false ;

		if ( lhs.wDay < rhs.wDay )
			return true ;
		if ( lhs.wDay > rhs.wDay )
			return false ;

		if ( lhs.wHour < rhs.wHour )
			return true ;
		if ( lhs.wHour > rhs.wHour )
			return false ;

		if ( lhs.wMinute < rhs.wMinute )
			return true ;
		if ( lhs.wMinute > rhs.wMinute )
			return false ;

		if ( lhs.wSecond < rhs.wSecond )
			return true ;
		if ( lhs.wSecond > rhs.wSecond )
			return false ;

		return ( lhs.wMilliseconds < rhs.wMilliseconds ) ;
	}

	inline bool operator < ( const date &lhs, const date &rhs )
	{
		if ( lhs.wYear < rhs.wYear )
			return true ;
		if ( lhs.wYear > rhs.wYear )
			return false ;

		if ( lhs.wMonth < rhs.wMonth )
			return true ;
		if ( lhs.wMonth > rhs.wMonth )
			return false ;

		if ( lhs.wDay < rhs.wDay )
			return true ;
		if ( lhs.wDay > rhs.wDay )
			return false ;

		if ( lhs.wHour < rhs.wHour )
			return true ;
		if ( lhs.wHour > rhs.wHour )
			return false ;

		if ( lhs.wMinute < rhs.wMinute )
			return true ;
		if ( lhs.wMinute > rhs.wMinute )
			return false ;

		if ( lhs.wSecond < rhs.wSecond )
			return true ;
		if ( lhs.wSecond > rhs.wSecond )
			return false ;

		return ( lhs.wMilliseconds < rhs.wMilliseconds ) ;
	}


	inline bool operator >= ( const date &lhs, const date &rhs )
	{
		return ! ( lhs < rhs ) ;
	}

	// =============================
	// operator >
	// =============================

	inline bool operator > ( const date &lhs, const date &rhs )
	{
		if ( lhs.wYear > rhs.wYear )
			return true ;
		if ( lhs.wYear < rhs.wYear )
			return false ;
		
		if ( lhs.wMonth > rhs.wMonth )
			return true ;
		if ( lhs.wMonth < rhs.wMonth )
			return false ;
		
		if ( lhs.wDay > rhs.wDay )
			return true ;
		if ( lhs.wDay < rhs.wDay )
			return false ;
		
		if ( lhs.wHour > rhs.wHour )
			return true ;
		if ( lhs.wHour < rhs.wHour )
			return false ;
		
		if ( lhs.wMinute > rhs.wMinute )
			return true ;
		if ( lhs.wMinute < rhs.wMinute )
			return false ;
		
		if ( lhs.wSecond > rhs.wSecond )
			return true ;
		if ( lhs.wSecond < rhs.wSecond )
			return false ;
		
		return ( lhs.wMilliseconds > rhs.wMilliseconds ) ;
	}

	inline bool operator <= ( const date &lhs, const date &rhs )
	{
		return ! ( lhs > rhs ) ;
	}

	inline bool operator == ( const date &lhs, const date &rhs )
	{
		if ( lhs < rhs ) 
		{
			return false ;
		}

		if ( lhs > rhs ) 
		{
			return false ;
		}

		return true ;
	}

	inline bool operator != ( const date &lhs, const date &rhs )
	{
		if ( lhs == rhs ) 
		{
			return false ;
		}

		return true ;
	}

}

