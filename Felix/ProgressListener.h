#pragma once

#include "atlstr.h"

/**
	@class CProgressListener
	@brief Allows various sub-processes tell the interface about progress.
 */
class CProgressListener
{
public:

	CProgressListener(void)
	{
	}

	virtual ~CProgressListener(void)
	{
	}

	virtual void OnProgressInit( const CString &file_name, size_t min_val, size_t max_val ) = 0 ;
	virtual bool OnProgressLoadUpdate( size_t current_val ) = 0 ; // true to continue
	virtual bool OnProgressWriteUpdate( size_t current_val ) = 0 ; // true to continue
	virtual void OnProgressDoneWrite( size_t final_val ) = 0 ;
	virtual void OnProgressDoneLoad( size_t final_val ) = 0 ;
	virtual bool ShouldBailFromException()
	{
		return true ;
	}
};

#ifdef UNIT_TEST


	/**
		@class CProgressListenerDummy 
		@brief mock object for unit testing.
	 */
	class CProgressListenerDummy 
		: public CProgressListener
	{
		std::vector< string > m_call_history ;
		char print_buf[MAX_PATH*2] ;

	public:
		void sense(string value)
		{
			m_call_history += value ;
		}
		void OnProgressInit( const CString &file_name, size_t min_val, size_t max_val ) 
		{
			sprintf_s( print_buf, MAX_PATH*2, "OnProgressInit: [%s] - min = %d - max = %d", (LPCSTR)CStringA(file_name), min_val, max_val ) ;
			sense(print_buf) ;
		}
		bool OnProgressLoadUpdate( size_t current_val ) // true to continue
		{
			sprintf_s( print_buf, MAX_PATH*2, "OnProgressLoadUpdate: current_val = %d", current_val ) ;
			sense(print_buf) ;
			return true ;
		}	
		bool OnProgressWriteUpdate( size_t current_val ) // true to continue
		{
			sprintf_s( print_buf, MAX_PATH*2, "OnProgressWriteUpdate: current_val = %d", current_val ) ;
			sense(print_buf) ;
			return true ;
		}	
		void OnProgressDoneLoad( size_t final_val ) 
		{
			sprintf_s( print_buf, MAX_PATH*2, "OnProgressDoneLoad: final_val = %d", final_val ) ;
			sense(print_buf) ;
		}
		void OnProgressDoneWrite( size_t final_val ) 
		{
			sprintf_s( print_buf, MAX_PATH*2, "OnProgressDoneWrite: final_val = %d", final_val ) ;
			sense(print_buf) ;
		}

	};

#endif // #ifdef UNIT_TEST
