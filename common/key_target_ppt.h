#pragma once

class KeyTargetPpt
{
public:
	virtual bool IsActive() = 0 ;
	virtual HRESULT OnExtendLookupAction( bool shift_pressed ) = 0 ;
	virtual HRESULT OnLookupNextTransAction( bool shift_pressed )  = 0 ;
	virtual HRESULT OnCorrectTransAction( bool shift_pressed )  = 0 ;
	virtual HRESULT OnExtendTransLookupAction( bool shift_pressed )  = 0 ;
	virtual HRESULT OnLookupTransAction( bool shift_pressed )  = 0 ;
	virtual HRESULT OnTransConcordanceAction( bool shift_pressed )  = 0 ;
	virtual HRESULT OnLookupNextAction( bool shift_pressed )  = 0 ;
	virtual HRESULT OnSetAction( bool shift_pressed ) = 0 ;
	virtual HRESULT OnGetAction( bool shift_pressed )  = 0 ;
	virtual HRESULT OnPrevAction( ) = 0 ;
	virtual HRESULT OnNextAction( ) = 0 ;
	virtual HRESULT OnGetAndNextAction( bool shift_pressed ) = 0 ;
	virtual HRESULT OnSetAndNextAction( bool shift_pressed ) = 0 ;
	virtual HRESULT OnLookupAction( bool shift_pressed ) = 0 ;
	virtual HRESULT OnAutoTransFuzzyAction( ) = 0 ;
	virtual HRESULT OnDeleteAction( ) = 0 ;
	virtual HRESULT OnEntryAction( int n, bool shift_pressed ) = 0 ;
};
