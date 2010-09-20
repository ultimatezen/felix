#pragma once

#include "key_mapper.h"
#include "key_target_word.h"
#include "keyboard_shortcuts.h"

const static string CmdAutoTransAction = "Auto Trans" ;
const static string CmdAutoTransFuzzyAction = "Auto Trans to Fuzzy" ;

// get and set translations
const static string CmdGetAction = "Get" ;
const static string CmdSetAction = "Set" ;
const static string CmdGetAndNextAction = "Get and Next" ;
const static string CmdSetAndNextAction = "Set and Next" ;

// look up source segment
const static string CmdLookupAction = "Look up" ;
const static string CmdLookupNextAction = "Look up Next" ;
const static string CmdConcordanceAction  = "Concordance" ;
const static string CmdExtendLookupAction  = "Extend Lookup" ;
const static string CmdToMaruAction  = "To Period Character" ;

// register a glossary entry
const static string CmdRegisterGlossAction  = "Register Glossary" ;
// delete entry
const static string CmdDeleteAction  = "Delete Entry" ;
// navigate through matches
const static string CmdNextAction  = "Next" ;
const static string CmdPrevAction  = "Previous" ;

// get glossary entries
const static string CmdEntry0Action  = "Entry 0" ;
const static string CmdEntry1Action  = "Entry 1" ;
const static string CmdEntry2Action  = "Entry 2" ;
const static string CmdEntry3Action  = "Entry 3" ;
const static string CmdEntry4Action  = "Entry 4" ;
const static string CmdEntry5Action  = "Entry 5" ;
const static string CmdEntry6Action  = "Entry 6" ;
const static string CmdEntry7Action  = "Entry 7" ;
const static string CmdEntry8Action  = "Entry 8" ;
const static string CmdEntry9Action  = "Entry 9" ;
const static string CmdGlossNAction  = "Specify Entry Number" ;

// look up translation
const static string CmdLookupTransAction  = "Look up Translation" ;
const static string CmdLookupNextTransAction  = "Look up Next Translation" ;
const static string CmdTransConcordanceAction  = "Trans Concordance" ;
const static string CmdExtendTransLookupAction  = "Extend Trans Lookup" ;
const static string CmdCorrectAction  = "Correct" ;
const static string CmdCorrectAndNextAction  = "Correct and Next" ;
const static string CmdRestoreAction  = "Restore" ;
const static string CmdRestoreAndNextAction  = "Restore and Next" ;
const static string CmdSaveMemoryAction = "Save Memory" ;
const static string CmdShiftStateAction = "Shift State" ;
const static string CmdAnalyze = "Analyze" ;


class KeyMapperWord : public KeyMapper
{
public:

	KeyTargetWord	*m_target ;
	bool map_command(string command)
	{

		if (boost::iequals(command, CmdAutoTransAction))
		{
			return m_target->OnAutoTransAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdAutoTransFuzzyAction))
		{
			return m_target->OnAutoTransFuzzyAction( shift_key_is_pressed() ) ;
		}

		// get and set translations
		if (boost::iequals(command, CmdGetAction))
		{
			return m_target->OnGetAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdSetAction))
		{
			return m_target->OnSetAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdGetAndNextAction))
		{
			return m_target->OnGetAndNextAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdSetAndNextAction))
		{
			return m_target->OnSetAndNextAction( shift_key_is_pressed() ) ;
		}

		// look up source segment
		if (boost::iequals(command, CmdLookupAction))
		{
			return m_target->OnLookupAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdLookupNextAction))
		{
			return m_target->OnLookupNextAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdConcordanceAction ))
		{
			return m_target->OnConcordanceAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdExtendLookupAction ))
		{
			if (shift_key_is_pressed())
			{
				return false ;
			}
			return m_target->OnExtendLookupAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdToMaruAction ))
		{
			return m_target->OnToMaruAction( shift_key_is_pressed() ) ;
		}

		// register a glossary entry
		if (boost::iequals(command, CmdRegisterGlossAction ))
		{
			return m_target->OnRegisterGlossAction( shift_key_is_pressed() ) ;
		}
		// delete entry
		if (boost::iequals(command, CmdDeleteAction ))
		{
			return m_target->OnDeleteAction( ) ;
		}
		// navigate through matches
		if (boost::iequals(command, CmdNextAction ))
		{
			return m_target->OnNextAction( ) ;
		}
		if (boost::iequals(command, CmdPrevAction ))
		{
			return m_target->OnPrevAction( ) ;
		}
		// get glossary entries
		if (boost::iequals(command, CmdEntry0Action ))
		{
			return m_target->OnEntry0Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdEntry1Action ))
		{
			return m_target->OnEntry1Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdEntry2Action ))
		{
			return m_target->OnEntry2Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdEntry3Action ))
		{
			return m_target->OnEntry3Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdEntry4Action ))
		{
			return m_target->OnEntry4Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdEntry5Action ))
		{
			return m_target->OnEntry5Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdEntry6Action ))
		{
			return m_target->OnEntry6Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdEntry7Action ))
		{
			return m_target->OnEntry7Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdEntry8Action ))
		{
			return m_target->OnEntry8Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdEntry9Action ))
		{
			return m_target->OnEntry9Action( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdGlossNAction))
		{
			return m_target->OnGlossNAction( shift_key_is_pressed() ) ;
		}

		// look up translation
		if (boost::iequals(command, CmdLookupTransAction ))
		{
			return m_target->OnLookupTransAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdLookupNextTransAction ))
		{
			return m_target->OnLookupNextTransAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdTransConcordanceAction ))
		{
			return m_target->OnTransConcordanceAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdExtendTransLookupAction ))
		{
			return m_target->OnExtendTransLookupAction( shift_key_is_pressed() ) ;
		}

		if (boost::iequals(command, CmdCorrectAction ))
		{
			return m_target->OnCorrectAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdCorrectAndNextAction ))
		{
			return m_target->OnCorrectAndNextAction( shift_key_is_pressed() ) ;
		}

		if (boost::iequals(command, CmdRestoreAction ))
		{
			return m_target->OnRestoreAction( shift_key_is_pressed() ) ;
		}
		if (boost::iequals(command, CmdRestoreAndNextAction ))
		{
			return m_target->OnRestoreAndNextAction( shift_key_is_pressed() ) ;
		}

		if (boost::iequals(command, CmdSaveMemoryAction))
		{
			return m_target->OnSaveMemoryAction( ) ;
		}
		if (boost::iequals(command, CmdShiftStateAction))
		{
			return m_target->OnShiftStateAction( ) ;
		}

		if (boost::iequals(command, CmdAnalyze))
		{
			return m_target->OnAnalyze( ) ;
		}

		return false ;
	}
};
