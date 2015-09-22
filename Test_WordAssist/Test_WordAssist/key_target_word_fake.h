#pragma once

#include "key_target_word.h"

class KeyTargetWordFake : public KeyTargetWord
{
public:
	DECLARE_SENSING_VAR ;

	void set_auto_off()
	{ 
		SENSE("set_auto_off") ; 
	}

	bool IsActive()
	{ 
		return true ; 
	}

	bool OnAutoTransAction ( bool )
	{ 
		SENSE("OnAutoTransAction") ; 
		return true ; 
	}
	bool OnAutoTransFuzzyAction ( bool )
	{ 
		SENSE("OnAutoTransFuzzyAction") ; 
		return true ; 
	}

	// get and set translations
	bool OnGetAction( bool ) 
	{ 
		SENSE("OnGetAction") ; 
		return true ; 
	}
	bool OnSetAction( bool )
	{ 
		SENSE("OnSetAction") ; 
		return true ; 
	}
	bool OnGetAndNextAction( bool ) 
	{ 
		SENSE("OnGetAndNextAction") ; 
		return true ; 
	}
	bool OnSetAndNextAction( bool ) 
	{ 
		SENSE("OnSetAndNextAction") ; 
		return true ; 
	}

	// look up source segment
	bool OnLookupAction( bool ) 
	{ 
		SENSE("OnLookupAction") ; 
		return true ; 
	}
	bool OnLookupNextAction( bool ) 
	{ 
		SENSE("OnLookupNextAction") ; 
		return true ; 
	}
	bool OnConcordanceAction ( bool ) 
	{ 
		SENSE("OnConcordanceAction") ; 
		return true ; 
	}
	bool OnExtendLookupAction ( bool ) 
	{ 
		SENSE("OnExtendLookupAction") ; 
		return true ; 
	}
	bool OnToMaruAction ( bool )
	{ 
		SENSE("OnToMaruAction") ; 
		return true ; 
	}

	// register a glossary entry
	bool OnRegisterGlossAction ( bool ) 
	{ 
		SENSE("OnRegisterGlossAction") ; 
		return true ; 
	}
	// delete entry
	bool OnDeleteAction ( ) 
	{ 
		SENSE("OnDeleteAction") ; 
		return true ; 
	}
	// navigate through matches
	bool OnNextAction ( ) 
	{ 
		SENSE("OnNextAction") ; 
		return true ; 
	}
	bool OnPrevAction ( ) 
	{ 
		SENSE("OnPrevAction") ; 
		return true ; 
	}
	// get glossary entries
	bool OnEntry0Action ( bool ) 
	{ 
		SENSE("OnEntry0Action") ; 
		return true ; 
	}
	bool OnEntry1Action ( bool ) 
	{ 
		SENSE("OnEntry1Action") ; 
		return true ; 
	}
	bool OnEntry2Action ( bool ) 
	{ 
		SENSE("OnEntry2Action") ; 
		return true ; 
	}
	bool OnEntry3Action ( bool )
	{ 
		SENSE("OnEntry3Action") ; 
		return true ; 
	}
	bool OnEntry4Action ( bool ) 
	{ 
		SENSE("OnEntry4Action") ; 
		return true ; 
	}
	bool OnEntry5Action ( bool ) 
	{ 
		SENSE("OnEntry5Action") ; 
		return true ; 
	}
	bool OnEntry6Action ( bool ) 
	{ 
		SENSE("OnEntry6Action") ; 
		return true ; 
	}
	bool OnEntry7Action ( bool ) 
	{ 
		SENSE("OnEntry7Action") ; 
		return true ; 
	}
	bool OnEntry8Action ( bool ) 
	{ 
		SENSE("OnEntry8Action") ; 
		return true ; 
	}
	bool OnEntry9Action ( bool ) 
	{ 
		SENSE("OnEntry9Action") ; 
		return true ; 
	}
	bool OnGlossNAction( bool ) 
	{ 
		SENSE("OnGlossNAction") ; 
		return true ; 
	}

	// look up translation
	bool OnLookupTransAction ( bool ) 
	{ 
		SENSE("OnLookupTransAction") ; 
		return true ; 
	}
	bool OnLookupNextTransAction ( bool ) 
	{ 
		SENSE("OnLookupNextTransAction") ; 
		return true ; 
	}
	bool OnTransConcordanceAction ( bool ) 
	{ 
		SENSE("OnTransConcordanceAction") ; 
		return true ; 
	}
	bool OnExtendTransLookupAction ( bool ) 
	{ 
		SENSE("OnExtendTransLookupAction") ; 
		return true ; 
	}

	bool OnCorrectAction ( bool ) 
	{ 
		SENSE("OnCorrectAction") ; 
		return true ; 
	}
	bool OnCorrectAndNextAction ( bool ) 
	{ 
		SENSE("OnCorrectAndNextAction") ; 
		return true ; 
	}

	bool OnRestoreAction ( bool )
	{ 
		SENSE("OnRestoreAction") ; 
		return true ; 
	}
	bool OnRestoreAndNextAction ( bool ) 
	{ 
		SENSE("OnRestoreAndNextAction") ; 
		return true ; 
	}

	bool OnSaveMemoryAction( ) 
	{ 
		SENSE("OnSaveMemoryAction") ; 
		return true ; 
	}
	bool OnShiftStateAction( ) 
	{ 
		SENSE("OnShiftStateAction") ; 
		return true ; 
	}

	bool OnAnalyze() 
	{ 
		SENSE("OnAnalyze") ; 
		return true ; 
	}
};