#pragma once

#include "key_mapper.h"
#include "key_target_word.h"
#include "keyboard_shortcuts.h"

const static string CMD_EXTEND_LOOKUP = "Extend Lookup" ;
const static string CMD_EXTEND_TRANS_LOOKUP = "Extend Trans Lookup" ;
const static string CMD_LOOK_UP_NEXT_TRANS = "Look up Next Trans" ;
const static string CMD_CORRECT_TRANS = "Correct Trans" ;
const static string CMD_LOOK_UP_TRANS = "Look up Trans" ;
const static string CMD_TRANS_CONCORDANCE = "Trans Concordance" ;
const static string CMD_LOOK_UP_NEXT = "Look up Next" ;
const static string CMD_SET = "Set" ;
const static string CMD_GET = "Get" ;
const static string CMD_PREVIOUS = "Previous" ;
const static string CMD_NEXT = "Next" ;
const static string CMD_GET_AND_NEXT = "Get and Next" ;
const static string CMD_SET_AND_NEXT = "Set and Next" ;
const static string CMD_LOOK_UP = "Look up" ;
const static string CMD_AUTO_TRANS_TO_FUZZY = "Auto Trans to Fuzzy" ;
const static string CMD_DELETE = "Delete" ;

const static string CMD_ENTRY_0 = "Entry 0" ;
const static string CMD_ENTRY_1 = "Entry 1" ;
const static string CMD_ENTRY_2 = "Entry 2" ;
const static string CMD_ENTRY_3 = "Entry 3" ;
const static string CMD_ENTRY_4 = "Entry 4" ;
const static string CMD_ENTRY_5 = "Entry 5" ;
const static string CMD_ENTRY_6 = "Entry 6" ;
const static string CMD_ENTRY_7 = "Entry 7" ;
const static string CMD_ENTRY_8 = "Entry 8" ;
const static string CMD_ENTRY_9 = "Entry 9" ;

class KeyMapperWord : public KeyMapper
{
public:

	KeyTargetWord	*m_target ;
	bool map_command(string command)
	{
		if (command == CMD_EXTEND_LOOKUP)
		{
			if ( shift_key_is_pressed() )
			{
				return false ;
			}
			return SUCCEEDED( m_target->OnExtendLookupAction( shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_LOOK_UP_NEXT_TRANS)
		{
			return SUCCEEDED( m_target->OnLookupNextTransAction( shift_key_is_pressed() )  ) ;
		}
		if (command == CMD_CORRECT_TRANS)
		{
			return SUCCEEDED( m_target->OnCorrectTransAction( shift_key_is_pressed() )  ) ;
		}

		if (command == CMD_EXTEND_TRANS_LOOKUP)
		{
			return SUCCEEDED( m_target->OnExtendTransLookupAction( shift_key_is_pressed() )  ) ;
		}
		if (command == CMD_LOOK_UP_TRANS)
		{
			return SUCCEEDED( m_target->OnLookupTransAction( shift_key_is_pressed() )  ) ;
		}
		if (command == CMD_TRANS_CONCORDANCE)
		{
			return SUCCEEDED( m_target->OnTransConcordanceAction( shift_key_is_pressed() )  ) ;
		}
		if (command == CMD_LOOK_UP_NEXT)
		{
			return SUCCEEDED( m_target->OnLookupNextAction( shift_key_is_pressed() )  ) ;
		}
		if (command == CMD_SET)
		{
			return SUCCEEDED( m_target->OnSetAction( shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_GET)
		{
			return SUCCEEDED( m_target->OnGetAction( shift_key_is_pressed() )  ) ;
		}

		if (command == CMD_PREVIOUS)
		{
			return SUCCEEDED( m_target->OnPrevAction( )  ) ;
		}
		if (command == CMD_NEXT)
		{
			return SUCCEEDED( m_target->OnNextAction( )  ) ;
		}
		if (command == CMD_GET_AND_NEXT)
		{
			return SUCCEEDED( m_target->OnGetAndNextAction( shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_SET_AND_NEXT)
		{
			return SUCCEEDED( m_target->OnSetAndNextAction( shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_LOOK_UP)
		{
			return SUCCEEDED( m_target->OnLookupAction( shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_AUTO_TRANS_TO_FUZZY)
		{
			return SUCCEEDED( m_target->OnAutoTransFuzzyAction( ) ) ;
		}
		if (command == CMD_DELETE)
		{
			return SUCCEEDED( m_target->OnDeleteAction( ) ) ;
		}
		// entries
		if (command == CMD_ENTRY_0)
		{
			return SUCCEEDED( m_target->OnEntryAction( 0, shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_ENTRY_1)
		{
			return SUCCEEDED( m_target->OnEntryAction( 1, shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_ENTRY_2)
		{
			return SUCCEEDED( m_target->OnEntryAction( 2, shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_ENTRY_3)
		{
			return SUCCEEDED( m_target->OnEntryAction( 3, shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_ENTRY_4)
		{
			return SUCCEEDED( m_target->OnEntryAction( 4, shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_ENTRY_5)
		{
			return SUCCEEDED( m_target->OnEntryAction( 5, shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_ENTRY_6)
		{
			return SUCCEEDED( m_target->OnEntryAction( 6, shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_ENTRY_7)
		{
			return SUCCEEDED( m_target->OnEntryAction( 7, shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_ENTRY_8)
		{
			return SUCCEEDED( m_target->OnEntryAction( 8, shift_key_is_pressed() ) ) ;
		}
		if (command == CMD_ENTRY_9)
		{
			return SUCCEEDED( m_target->OnEntryAction( 9, shift_key_is_pressed() ) ) ;
		}
		return false ;
	}
};
