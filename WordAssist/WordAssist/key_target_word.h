#pragma once

class KeyTargetWord
{
public:

	virtual bool IsActive() = 0 ;

	virtual bool OnAutoTransAction ( bool as_plaintext ) = 0 ;
	virtual bool OnAutoTransFuzzyAction ( bool as_plaintext ) = 0 ;

	// get and set translations
	virtual bool OnGetAction( bool as_plaintext ) = 0 ;
	virtual bool OnSetAction( bool as_plaintext ) = 0 ;
	virtual bool OnGetAndNextAction( bool as_plaintext ) = 0 ;
	virtual bool OnSetAndNextAction( bool as_plaintext ) = 0 ;

	// look up source segment
	virtual bool OnLookupAction( bool as_plaintext ) = 0 ;
	virtual bool OnLookupNextAction( bool as_plaintext ) = 0 ;
	virtual bool OnConcordanceAction ( bool as_plaintext ) = 0 ;
	virtual bool OnExtendLookupAction ( bool as_plaintext ) = 0 ;
	virtual bool OnToMaruAction ( bool as_plaintext ) = 0 ;

	// register a glossary entry
	virtual bool OnRegisterGlossAction ( bool as_plaintext ) = 0 ;
	// delete entry
	virtual bool OnDeleteAction ( ) = 0 ;
	// navigate through matches
	virtual bool OnNextAction ( ) = 0 ;
	virtual bool OnPrevAction ( ) = 0 ;
	// get glossary entries
	virtual bool OnEntry0Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry1Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry2Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry3Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry4Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry5Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry6Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry7Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry8Action ( bool as_plaintext ) = 0 ;
	virtual bool OnEntry9Action ( bool as_plaintext ) = 0 ;
	virtual bool OnGlossNAction( bool as_plaintext ) = 0 ;

	// look up translation
	virtual bool OnLookupTransAction ( bool as_plaintext ) = 0 ;
	virtual bool OnLookupNextTransAction ( bool as_plaintext ) = 0 ;
	virtual bool OnTransConcordanceAction ( bool as_plaintext ) = 0 ;
	virtual bool OnExtendTransLookupAction ( bool as_plaintext ) = 0 ;

	virtual bool OnCorrectAction ( bool as_plaintext ) = 0 ;
	virtual bool OnCorrectAndNextAction ( bool as_plaintext ) = 0 ;

	virtual bool OnRestoreAction ( bool as_plaintext ) = 0 ;
	virtual bool OnRestoreAndNextAction ( bool as_plaintext ) = 0 ;

	virtual bool OnSaveMemoryAction( ) = 0 ;
	virtual bool OnShiftStateAction( ) = 0 ;

	virtual bool OnAnalyze() = 0 ;
};
