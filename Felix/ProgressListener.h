#pragma once

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

