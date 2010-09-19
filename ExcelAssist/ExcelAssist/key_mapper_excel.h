#pragma once

#include "key_mapper.h"
#include "key_target_excel.h"
#include "keyboard_shortcuts.h"

const static string CmdLookupNext  = "Look up Next" ;
const static string CmdSet  = "Set" ;
const static string CmdGet  = "Get" ;
const static string CmdPrev  = "Previous" ;
const static string CmdNext  = "Next" ;
const static string CmdGetAndNext  = "Get and Next" ;
const static string CmdSetAndNext  = "Set and Next" ;
const static string CmdLookup  = "Look up" ;
const static string CmdAutoTransSel  = "Auto Translate Selection" ;
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


class KeyMapperExcel : public KeyMapper
{
public:

	KeyTargetExcel	*m_target ;
	bool map_command(string command)
	{
		if (command == CmdLookupNext)
		{
			m_target->OnLookupNext() ;
			return true ;
		}
		if (command == CmdSet)
		{
			m_target->OnSet() ;
			return true ;
		}
		if (command == CmdGet)
		{
			m_target->OnGet() ;
			return true ;
		}
		if (command == CmdPrev)
		{
			m_target->OnPrev() ;
			return true ;
		}
		if (command == CmdNext)
		{
			m_target->OnNext() ;
			return true ;
		}
		if (command == CmdGetAndNext)
		{
			m_target->OnGetAndNext() ;
			return true ;
		}
		if (command == CmdSetAndNext)
		{
			m_target->OnSetAndNext() ;
			return true ;
		}
		if (command == CmdLookup)
		{
			m_target->OnLookup() ;
			return true ;
		}
		if (command == CmdAutoTransSel) 
		{
			m_target->OnAutoTransSel() ;
			return true ;
		}
		// get glossary entries
		if (command == CmdEntry0Action )
		{
			m_target->OnEntry( 0 ) ;
		}
		if (command == CmdEntry1Action )
		{
			m_target->OnEntry( 1 ) ;
		}
		if (command == CmdEntry2Action )
		{
			m_target->OnEntry( 2 ) ;
		}
		if (command == CmdEntry3Action )
		{
			m_target->OnEntry( 3 ) ;
		}
		if (command == CmdEntry4Action )
		{
			m_target->OnEntry( 4 ) ;
		}
		if (command == CmdEntry5Action )
		{
			m_target->OnEntry( 5 ) ;
		}
		if (command == CmdEntry6Action )
		{
			m_target->OnEntry( 6 ) ;
		}
		if (command == CmdEntry7Action )
		{
			m_target->OnEntry( 7 ) ;
		}
		if (command == CmdEntry8Action )
		{
			m_target->OnEntry( 8 ) ;
		}
		if (command == CmdEntry9Action )
		{
			m_target->OnEntry( 9 ) ;
		}
		return false ;
	}
};