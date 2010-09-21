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
		if (! m_target->IsActive())
		{
			return false ;
		}
		if (boost::iequals(command, CmdLookupNext))
		{
			m_target->OnLookupNext() ;
			return true ;
		}
		if (boost::iequals(command, CmdSet))
		{
			m_target->OnSet() ;
			return true ;
		}
		if (boost::iequals(command, CmdGet))
		{
			m_target->OnGet() ;
			return true ;
		}
		if (boost::iequals(command, CmdPrev))
		{
			m_target->OnPrev() ;
			return true ;
		}
		if (boost::iequals(command, CmdNext))
		{
			m_target->OnNext() ;
			return true ;
		}
		if (boost::iequals(command, CmdGetAndNext))
		{
			m_target->OnGetAndNext() ;
			return true ;
		}
		if (boost::iequals(command, CmdSetAndNext))
		{
			m_target->OnSetAndNext() ;
			return true ;
		}
		if (boost::iequals(command, CmdLookup))
		{
			m_target->OnLookup() ;
			return true ;
		}
		if (boost::iequals(command, CmdAutoTransSel))
		{
			m_target->OnAutoTransSel() ;
			return true ;
		}
		// get glossary entries
		if (boost::iequals(command, CmdEntry0Action ))
		{
			m_target->OnEntry( 0 ) ;
			return true ;
		}
		if (boost::iequals(command, CmdEntry1Action ))
		{
			m_target->OnEntry( 1 ) ;
			return true ;
		}
		if (boost::iequals(command, CmdEntry2Action ))
		{
			m_target->OnEntry( 2 ) ;
			return true ;
		}
		if (boost::iequals(command, CmdEntry3Action ))
		{
			m_target->OnEntry( 3 ) ;
			return true ;
		}
		if (boost::iequals(command, CmdEntry4Action ))
		{
			m_target->OnEntry( 4 ) ;
			return true ;
		}
		if (boost::iequals(command, CmdEntry5Action ))
		{
			m_target->OnEntry( 5 ) ;
			return true ;
		}
		if (boost::iequals(command, CmdEntry6Action ))
		{
			m_target->OnEntry( 6 ) ;
			return true ;
		}
		if (boost::iequals(command, CmdEntry7Action ))
		{
			m_target->OnEntry( 7 ) ;
			return true ;
		}
		if (boost::iequals(command, CmdEntry8Action ))
		{
			m_target->OnEntry( 8 ) ;
			return true ;
		}
		if (boost::iequals(command, CmdEntry9Action ))
		{
			m_target->OnEntry( 9 ) ;
			return true ;
		}
		return false ;
	}
};