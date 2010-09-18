#pragma once

#include "key_target_word.h"

class KeyTargetWordFake : public KeyTargetWord
{
public:
	DECLARE_SENSING_VAR ;


	virtual bool IsActive()
	{ 
		SENSE("IsActive") ; 
		return true ; 
	}

	virtual bool OnAutoTransAction ( bool )
	{ 
		SENSE("OnAutoTransAction") ; 
		return true ; 
	}
	virtual bool OnAutoTransFuzzyAction ( bool )
	{ 
		SENSE("OnAutoTransFuzzyAction") ; 
		return true ; 
	}

	// get and set translations
	virtual bool OnGetAction( bool ) 
	{ 
		SENSE("OnGetAction") ; 
		return true ; 
	}
	virtual bool OnSetAction( bool )
	{ 
		SENSE("OnSetAction") ; 
		return true ; 
	}
	virtual bool OnGetAndNextAction( bool ) 
	{ 
		SENSE("OnGetAndNextAction") ; 
		return true ; 
	}
	virtual bool OnSetAndNextAction( bool ) 
	{ 
		SENSE("OnSetAndNextAction") ; 
		return true ; 
	}

	// look up source segment
	virtual bool OnLookupAction( bool ) 
	{ 
		SENSE("OnLookupAction") ; 
		return true ; 
	}
	virtual bool OnLookupNextAction( bool ) 
	{ 
		SENSE("OnLookupNextAction") ; 
		return true ; 
	}
	virtual bool OnConcordanceAction ( bool ) 
	{ 
		SENSE("OnConcordanceAction") ; 
		return true ; 
	}
	virtual bool OnExtendLookupAction ( bool ) 
	{ 
		SENSE("OnExtendLookupAction") ; 
		return true ; 
	}
	virtual bool OnToMaruAction ( bool )
	{ 
		SENSE("OnToMaruAction") ; 
		return true ; 
	}

	// register a glossary entry
	virtual bool OnRegisterGlossAction ( bool ) 
	{ 
		SENSE("OnRegisterGlossAction") ; 
		return true ; 
	}
	// delete entry
	virtual bool OnDeleteAction ( ) 
	{ 
		SENSE("OnDeleteAction") ; 
		return true ; 
	}
	// navigate through matches
	virtual bool OnNextAction ( ) 
	{ 
		SENSE("OnNextAction") ; 
		return true ; 
	}
	virtual bool OnPrevAction ( ) 
	{ 
		SENSE("OnPrevAction") ; 
		return true ; 
	}
	// get glossary entries
	virtual bool OnEntry0Action ( bool ) 
	{ 
		SENSE("OnEntry0Action") ; 
		return true ; 
	}
	virtual bool OnEntry1Action ( bool ) 
	{ 
		SENSE("OnEntry1Action") ; 
		return true ; 
	}
	virtual bool OnEntry2Action ( bool ) 
	{ 
		SENSE("OnEntry2Action") ; 
		return true ; 
	}
	virtual bool OnEntry3Action ( bool )
	{ 
		SENSE("OnEntry3Action") ; 
		return true ; 
	}
	virtual bool OnEntry4Action ( bool ) 
	{ 
		SENSE("OnEntry4Action") ; 
		return true ; 
	}
	virtual bool OnEntry5Action ( bool ) 
	{ 
		SENSE("OnEntry5Action") ; 
		return true ; 
	}
	virtual bool OnEntry6Action ( bool ) 
	{ 
		SENSE("OnEntry6Action") ; 
		return true ; 
	}
	virtual bool OnEntry7Action ( bool ) 
	{ 
		SENSE("OnEntry7Action") ; 
		return true ; 
	}
	virtual bool OnEntry8Action ( bool ) 
	{ 
		SENSE("OnEntry8Action") ; 
		return true ; 
	}
	virtual bool OnEntry9Action ( bool ) 
	{ 
		SENSE("OnEntry9Action") ; 
		return true ; 
	}
	virtual bool OnGlossNAction( bool ) 
	{ 
		SENSE("OnGlossNAction") ; 
		return true ; 
	}

	// look up translation
	virtual bool OnLookupTransAction ( bool ) 
	{ 
		SENSE("OnLookupTransAction") ; 
		return true ; 
	}
	virtual bool OnLookupNextTransAction ( bool ) 
	{ 
		SENSE("OnLookupNextTransAction") ; 
		return true ; 
	}
	virtual bool OnTransConcordanceAction ( bool ) 
	{ 
		SENSE("OnTransConcordanceAction") ; 
		return true ; 
	}
	virtual bool OnExtendTransLookupAction ( bool ) 
	{ 
		SENSE("OnExtendTransLookupAction") ; 
		return true ; 
	}

	virtual bool OnCorrectAction ( bool ) 
	{ 
		SENSE("OnCorrectAction") ; 
		return true ; 
	}
	virtual bool OnCorrectAndNextAction ( bool ) 
	{ 
		SENSE("OnCorrectAndNextAction") ; 
		return true ; 
	}

	virtual bool OnRestoreAction ( bool )
	{ 
		SENSE("OnRestoreAction") ; 
		return true ; 
	}
	virtual bool OnRestoreAndNextAction ( bool ) 
	{ 
		SENSE("OnRestoreAndNextAction") ; 
		return true ; 
	}

	virtual bool OnSaveMemoryAction( ) 
	{ 
		SENSE("OnSaveMemoryAction") ; 
		return true ; 
	}
	virtual bool OnShiftStateAction( ) 
	{ 
		SENSE("OnShiftStateAction") ; 
		return true ; 
	}

	virtual bool OnAnalyze() 
	{ 
		SENSE("OnAnalyze") ; 
		return true ; 
	}
};