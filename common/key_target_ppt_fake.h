#pragma once

#include "key_target_ppt.h"

class KeyTargetPptFake : public KeyTargetPpt
{
public:
	DECLARE_SENSING_VAR ;
	bool IsActive()
	{
		return true ;
	}

	HRESULT OnExtendLookupAction( bool ) 
	{ 
		SENSE("OnExtendLookupAction") ; 
		return S_OK ; 
	}
	HRESULT OnLookupNextTransAction( bool) 
	{ 
		SENSE("OnLookupNextTransAction") ; 
		return S_OK ; 
	}
	HRESULT OnCorrectTransAction( bool )
	{ 
		SENSE("OnCorrectTransAction") ; 
		return S_OK ; 
	}
	HRESULT OnExtendTransLookupAction( bool shift_pressed )
	{ 
		SENSE("OnExtendTransLookupAction") ; 
		return S_OK ; 
	}
	HRESULT OnLookupTransAction( bool ) 
	{
		SENSE("OnLookupTransAction") ; 
		return S_OK ; 
	}
	HRESULT OnTransConcordanceAction( bool ) 
	{ 
		SENSE("OnTransConcordanceAction") ; return S_OK ; 
	}
	HRESULT OnLookupNextAction( bool )  
	{ SENSE("OnLookupNextAction") ; return S_OK ; }
	HRESULT OnSetAction( bool ) 
	{ SENSE("OnSetAction") ; return S_OK ; }
	HRESULT OnGetAction( bool )  
	{ SENSE("OnGetAction") ; return S_OK ; }
	HRESULT OnPrevAction( )  
	{ SENSE("OnPrevAction") ; return S_OK ; }
	HRESULT OnNextAction( ) 
	{ SENSE("OnNextAction") ; return S_OK ; }
	HRESULT OnGetAndNextAction( bool ) 
	{ SENSE("OnGetAndNextAction") ; return S_OK ; }
	HRESULT OnSetAndNextAction( bool ) 
	{ SENSE("OnSetAndNextAction") ; return S_OK ; }
	HRESULT OnLookupAction( bool) 
	{ SENSE("OnLookupAction") ; return S_OK ; }
	HRESULT OnAutoTransFuzzyAction( ) 
	{ SENSE("OnAutoTransFuzzyAction") ; return S_OK ; }
	HRESULT OnDeleteAction( ) 
	{ SENSE("OnDeleteAction") ; return S_OK ; }
	HRESULT OnEntryAction( int n, bool) 
	{ SENSE("OnEntryAction") ; SENSE(int2string(n)) ; return S_OK ; }
};