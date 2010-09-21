#pragma once

class KeyTargetExcel
{
public:
	virtual bool IsActive() = 0 ;
	virtual void OnLookupNext() = 0 ;
	virtual void OnSet() = 0 ;
	virtual void OnGet() = 0 ;
	virtual void OnPrev() = 0 ;
	virtual void OnNext() = 0 ;
	virtual void OnGetAndNext() = 0 ;
	virtual void OnSetAndNext() = 0 ;
	virtual void OnLookup() = 0 ;
	virtual void OnAutoTransSel() = 0 ;
	virtual void OnEntry( int ) = 0 ;
};
