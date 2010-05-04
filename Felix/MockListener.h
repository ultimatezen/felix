#pragma once

#include "ProgressListener.h"

class CMockListener : public CProgressListener
{
public:
	DECLARE_SENSING_VAR ;

	bool m_should_bail ;
	CMockListener() : m_should_bail(false){}

	void OnProgressInit( const CString &/*file_name*/, size_t /*min_val*/, size_t /*max_val*/ )
	{
		SENSE("OnProgressInit") ;
	}
	bool OnProgressLoadUpdate( size_t /*current_val*/ ) 
	{
		SENSE("OnProgressLoadUpdate") ;
		return true ;
	}
	bool OnProgressWriteUpdate( size_t /*current_val*/ ) 
	{
		SENSE("OnProgressWriteUpdate") ;
		return true ;
	}
	void OnProgressDoneWrite( size_t /*final_val*/ ) 
	{
		SENSE("OnProgressDoneWrite") ;
	}
	void OnProgressDoneLoad( size_t /*final_val*/ )
	{
		SENSE("OnProgressDoneLoad") ;
	}
	virtual bool ShouldBailFromException()
	{
		SENSE("ShouldBailFromException") ;
		return m_should_bail ;
	}
} ;
