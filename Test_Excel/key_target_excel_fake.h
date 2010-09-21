#pragma once

#include "key_target_excel.h"

class KeyTargetExcelFake : public KeyTargetExcel
{
public:
	DECLARE_SENSING_VAR ;
	bool IsActive()
	{
		return true ;
	}
	void OnLookupNext()
	{ 
		SENSE("OnLookupNext") ; 
	}
	void OnSet() 
	{ 
		SENSE("OnSet") ; 
	}
	void OnGet() 
	{ 
		SENSE("OnGet") ; 
	}
	void OnPrev() 
	{ 
		SENSE("OnPrev") ; 
	}
	void OnNext() 
	{ 
		SENSE("OnNext") ; 
	}
	void OnGetAndNext() 
	{ 
		SENSE("OnGetAndNext") ; 
	}
	void OnSetAndNext() 
	{ 
		SENSE("OnSetAndNext") ; 
	}
	void OnLookup() 
	{ 
		SENSE("OnLookup") ; 
	}
	void OnAutoTransSel() 
	{ 
		SENSE("OnAutoTransSel") ; 
	}
	void OnEntry( int n ) 
	{ 
		SENSE("OnEntry") ; 
		SENSE(int2string(n)) ;
	}
};
