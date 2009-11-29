#pragma once

#include "ProgressListener.h"

class CMockListener : public CProgressListener
{
public:
	void sense(string value)
	{
		m_calls += value ;
	}
	std::vector< string > m_calls ;
	void OnProgressInit( const CString &/*file_name*/, size_t /*min_val*/, size_t /*max_val*/ )
	{
		sense("OnProgressInit") ;
	}
	bool OnProgressLoadUpdate( size_t /*current_val*/ ) 
	{
		sense("OnProgressLoadUpdate") ;
		return true ;
	}
	bool OnProgressWriteUpdate( size_t /*current_val*/ ) 
	{
		sense("OnProgressWriteUpdate") ;
		return true ;
	}
	void OnProgressDoneWrite( size_t /*final_val*/ ) 
	{
		sense("OnProgressDoneWrite") ;
	}
	void OnProgressDoneLoad( size_t /*final_val*/ )
	{
		sense("OnProgressDoneLoad") ;
	}
	virtual bool RecoverFromExceptions()
	{
		sense("RecoverFromExceptions") ;
		return false ;
	}
} ;
