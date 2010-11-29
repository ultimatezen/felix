#include "stdafx.h"
#include "keyboard_shortcuts.h"
#include "pugxml/src/pugixml.hpp"


void add_child_text(pugi::xml_node &node, string name, string text)
{
	pugi::xml_node child_node = node.append_child();
	child_node.set_name(name.c_str()) ;
	child_node.append_child(pugi::node_pcdata).set_value(text.c_str());
}

void create_shortcut_node(pugi::xml_node &node, string stype, string first, string second, string command)
{
	pugi::xml_node shortcut_node = node.append_child();
	shortcut_node.set_name("shortcut") ;
	add_child_text(shortcut_node, "type", stype) ;
	add_child_text(shortcut_node, "first", first) ;
	add_child_text(shortcut_node, "second", second) ;
	add_child_text(shortcut_node, "command", command) ;
}

namespace shortcuts
{

	KeyboardShortcut::KeyboardShortcut( SHORTCUT_TYPE stype, 
										wchar_t firstkey, 
										wchar_t secondkey,
										const string &command ) :
		m_type(stype),
		m_firstkey(firstkey),
		m_secondkey(secondkey),
		m_command(command)
	{

	}

	void KeyboardShortcuts::load(const string text)
	{
		if (text.empty())
		{
			return ;
		}
		m_ctrl_shortcuts.clear() ;
		m_alt_shortcuts.clear() ;
		m_ctrl_alt_shortcuts.clear() ;

		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load(text.c_str());
		pugi::xml_node nodes = doc.child("shortcuts") ;

		for (pugi::xml_node shortcut_node = nodes.child("shortcut"); 
			shortcut_node; 
			shortcut_node = shortcut_node.next_sibling("shortcut"))
		{
			add_shortcut(load_shortcut(shortcut_node));
		}

		result ;
	}
	bool KeyboardShortcuts::receive_keypress( bool ctrl_pressed, bool alt_pressed, wchar_t key )
	{
		if (m_second_key)
		{
			if (m_second_keys->find(key) != m_second_keys->end())
			{
				m_second_key = 0 ;
				m_second_command = (*m_second_keys)[key]->m_command ;
				if (m_second_command == "ToggleShortcuts")
				{
					m_shortcuts_enabled = !! m_shortcuts_enabled ;
					if (m_on_toggle_shortcuts)
					{
						m_on_toggle_shortcuts(!! m_shortcuts_enabled) ;
					}
					return true;
				}
				else
				{
					return m_mapper->map_command(m_second_command) ;
				}
			}
			m_second_key = 0 ;
			return false;
		}
		if (!ctrl_pressed && !alt_pressed)
		{
			return false;
		}

		key_map *cuts = get_key_map(ctrl_pressed, alt_pressed);
		if (cuts->find(key) == cuts->end())
		{
			key = static_cast<wchar_t>(MapVirtualKey(
				static_cast<UINT>(key),
				MAPVK_VK_TO_CHAR
				));
			if (cuts->find(key) == cuts->end())
			{
				return false;
			}
		}
		shortcut_map_ptr keys_for_letter = (*cuts)[key] ;

		// second-key combo
		// There are keys for this letter, and 0 (no second key) isn't one of them
		if (! keys_for_letter->empty() && keys_for_letter->find(0) == keys_for_letter->end())
		{
			m_second_key = key ;
			m_second_keys = keys_for_letter ;
			return true;
		}

		// get single-key combo
		if (keys_for_letter->find(0) == keys_for_letter->end())
		{
			return false;
		}

		shortcut_ptr cut = (*keys_for_letter)[0] ;
		ATLASSERT(cut->m_secondkey == 0) ;

		if (! m_shortcuts_enabled)
		{
			// special case
			if (cut->m_command == "ToggleShortcuts")
			{
				m_shortcuts_enabled = true ;
				if (m_on_toggle_shortcuts)
				{
					m_on_toggle_shortcuts(!! m_shortcuts_enabled) ;
				}
				return true;
			}
		}
		else
		{
			if (cut->m_command == "ToggleShortcuts")
			{
				m_shortcuts_enabled = false ;
				if (m_on_toggle_shortcuts)
				{
					m_on_toggle_shortcuts(!! m_shortcuts_enabled) ;
				}
			}
			else
			{
				return m_mapper->map_command(cut->m_command) ;
			}
			return true;
		}
		return false;
	}

	KeyboardShortcuts::key_map * KeyboardShortcuts::get_key_map( bool ctrl_pressed, bool alt_pressed )
	{
		if (ctrl_pressed && alt_pressed)
		{
			return &m_ctrl_alt_shortcuts ;
		}
		else if (alt_pressed)
		{
			return &m_alt_shortcuts ;
		}
		return &m_ctrl_shortcuts ;
	}

	void KeyboardShortcuts::add_shortcut( shortcut_ptr shortcut )
	{
		if (! shortcut->m_firstkey)
		{
			return ;
		}
		key_map *keys = 0 ;
		switch(shortcut->m_type)
		{
		case SHORTCUT_CTRL:
			keys = &m_ctrl_shortcuts ;
			break ;
		case SHORTCUT_ALT:
			keys = &m_alt_shortcuts ;
			break ;
		default:
			keys = &m_ctrl_alt_shortcuts ;
		}
		if (keys->find(shortcut->m_firstkey) == keys->end())
		{
			(*keys)[shortcut->m_firstkey] = shortcut_map_ptr(new shortcut_map) ;
		}
		shortcut_map_ptr keys_for_letter = (*keys)[shortcut->m_firstkey] ;
		(*keys_for_letter)[shortcut->m_secondkey] = shortcut ;
	}
	shortcut_ptr KeyboardShortcuts::load_shortcut( pugi::xml_node &shortcut_node )
	{
		return shortcut_ptr(new KeyboardShortcut(get_shortcut_type(shortcut_node.child_value("type")), 
			get_key(shortcut_node.child_value("first")), 
			get_key(shortcut_node.child_value("second")),
			shortcut_node.child_value("command"))) ;
	}
	SHORTCUT_TYPE KeyboardShortcuts::get_shortcut_type(const string shortcut_type)
	{
		if (boost::to_lower_copy(shortcut_type) == "ctrl")
		{
			return SHORTCUT_CTRL ;
		}
		if (boost::to_lower_copy(shortcut_type) == "alt")
		{
			return SHORTCUT_ALT;
		}
		return SHORTCUT_CTRL_ALT ;
	}
	wchar_t KeyboardShortcuts::get_key(const string shortcut_key)
	{
		if (shortcut_key.empty())
		{
			return 0 ;
		}
		if (shortcut_key.size() == 1)
		{
			return boost::to_upper_copy(string2wstring(shortcut_key))[0]; 
		}

		return virtkeys[boost::to_upper_copy(shortcut_key)] ;
	}

	KeyboardShortcuts::KeyboardShortcuts(KeyMapper *mapper) : 
		m_mapper(mapper),
		m_second_key(0),
		m_shortcuts_enabled(true)
	{
		virtkeys["LBUTTON"]				=	VK_LBUTTON ;
		virtkeys["RBUTTON"]				=	VK_RBUTTON ;
		virtkeys["CANCEL"]				=	VK_CANCEL ;
		virtkeys["MBUTTON"]				=	VK_MBUTTON ;
		virtkeys["XBUTTON1"]			=	VK_XBUTTON1 ;
		virtkeys["XBUTTON2"]			=	VK_XBUTTON2 ;
		virtkeys["BACK"]				=	VK_BACK ;
		virtkeys["TAB"]					=	VK_TAB ;
		virtkeys["CLEAR"]				=	VK_CLEAR ;
		virtkeys["RETURN"]				=	VK_RETURN ;
		virtkeys["SHIFT"]				=	VK_SHIFT ;
		virtkeys["CONTROL"]				=	VK_CONTROL ;
		virtkeys["MENU"]				=	VK_MENU ;
		virtkeys["PAUSE"]				=	VK_PAUSE ;
		virtkeys["CAPITAL"]				=	VK_CAPITAL ;
		virtkeys["KANA"]				=	VK_KANA ;
		virtkeys["HANGEUL"]				=	VK_HANGEUL ;
		virtkeys["HANGUL"]				=	VK_HANGUL ;
		virtkeys["JUNJA"]				=	VK_JUNJA ;
		virtkeys["FINAL"]				=	VK_FINAL ;
		virtkeys["HANJA"]				=	VK_HANJA ;
		virtkeys["KANJI"]				=	VK_KANJI ;
		virtkeys["ESCAPE"]				=	VK_ESCAPE ;
		virtkeys["CONVERT"]				=	VK_CONVERT ;
		virtkeys["NONCONVERT"]			=	VK_NONCONVERT ;
		virtkeys["ACCEPT"]				=	VK_ACCEPT ;
		virtkeys["MODECHANGE"]			=	VK_MODECHANGE ;
		virtkeys["SPACE"]				=	VK_SPACE ;
		virtkeys["PRIOR"]				=	VK_PRIOR ;
		virtkeys["NEXT"]				=	VK_NEXT ;
		virtkeys["END"]					=	VK_END ;
		virtkeys["HOME"]				=	VK_HOME ;
		virtkeys["LEFT"]				=	VK_LEFT ;
		virtkeys["UP"]					=	VK_UP ;
		virtkeys["RIGHT"]				=	VK_RIGHT ;
		virtkeys["DOWN"]				=	VK_DOWN ;
		virtkeys["SELECT"]				=	VK_SELECT ;
		virtkeys["PRINT"]				=	VK_PRINT ;
		virtkeys["EXECUTE"]				=	VK_EXECUTE ;
		virtkeys["SNAPSHOT"]			=	VK_SNAPSHOT ;
		virtkeys["INSERT"]				=	VK_INSERT ;
		virtkeys["DELETE"]				=	VK_DELETE ;
		virtkeys["HELP"]				=	VK_HELP ;
		virtkeys["LWIN"]				=	VK_LWIN ;
		virtkeys["RWIN"]				=	VK_RWIN ;
		virtkeys["APPS"]				=	VK_APPS ;
		virtkeys["SLEEP"]				=	VK_SLEEP ;
		virtkeys["NUMPAD0"]				=	VK_NUMPAD0 ;
		virtkeys["NUMPAD1"]				=	VK_NUMPAD1 ;
		virtkeys["NUMPAD2"]				=	VK_NUMPAD2 ;
		virtkeys["NUMPAD3"]				=	VK_NUMPAD3 ;
		virtkeys["NUMPAD4"]				=	VK_NUMPAD4 ;
		virtkeys["NUMPAD5"]				=	VK_NUMPAD5 ;
		virtkeys["NUMPAD6"]				=	VK_NUMPAD6 ;
		virtkeys["NUMPAD7"]				=	VK_NUMPAD7 ;
		virtkeys["NUMPAD8"]				=	VK_NUMPAD8 ;
		virtkeys["NUMPAD9"]				=	VK_NUMPAD9 ;
		virtkeys["MULTIPLY"]			=	VK_MULTIPLY ;
		virtkeys["ADD"]					=	VK_ADD ;
		virtkeys["SEPARATOR"]			=	VK_SEPARATOR ;
		virtkeys["SUBTRACT"]			=	VK_SUBTRACT ;
		virtkeys["DECIMAL"]				=	VK_DECIMAL ;
		virtkeys["DIVIDE"]				=	VK_DIVIDE ;
		virtkeys["F1"]					=	VK_F1 ;
		virtkeys["F2"]					=	VK_F2 ;
		virtkeys["F3"]					=	VK_F3 ;
		virtkeys["F4"]					=	VK_F4 ;
		virtkeys["F5"]					=	VK_F5 ;
		virtkeys["F6"]					=	VK_F6 ;
		virtkeys["F7"]					=	VK_F7 ;
		virtkeys["F8"]					=	VK_F8 ;
		virtkeys["F9"]					=	VK_F9 ;
		virtkeys["F10"]					=	VK_F10 ;
		virtkeys["F11"]					=	VK_F11 ;
		virtkeys["F12"]					=	VK_F12 ;
		virtkeys["F13"]					=	VK_F13 ;
		virtkeys["F14"]					=	VK_F14 ;
		virtkeys["F15"]					=	VK_F15 ;
		virtkeys["F16"]					=	VK_F16 ;
		virtkeys["F17"]					=	VK_F17 ;
		virtkeys["F18"]					=	VK_F18 ;
		virtkeys["F19"]					=	VK_F19 ;
		virtkeys["F20"]					=	VK_F20 ;
		virtkeys["F21"]					=	VK_F21 ;
		virtkeys["F22"]					=	VK_F22 ;
		virtkeys["F23"]					=	VK_F23 ;
		virtkeys["F24"]					=	VK_F24 ;
		virtkeys["NUMLOCK"]				=	VK_NUMLOCK ;
		virtkeys["SCROLL"]				=	VK_SCROLL ;
		virtkeys["OEM_NEC_EQUAL"]		=	VK_OEM_NEC_EQUAL ;
		virtkeys["OEM_FJ_JISHO"]		=	VK_OEM_FJ_JISHO ;
		virtkeys["OEM_FJ_MASSHOU"]		=	VK_OEM_FJ_MASSHOU ;
		virtkeys["OEM_FJ_TOUROKU"]		=	VK_OEM_FJ_TOUROKU ;
		virtkeys["OEM_FJ_LOYA"]			=	VK_OEM_FJ_LOYA ;
		virtkeys["OEM_FJ_ROYA"]			=	VK_OEM_FJ_ROYA ;
		virtkeys["LSHIFT"]				=	VK_LSHIFT ;
		virtkeys["RSHIFT"]				=	VK_RSHIFT ;
		virtkeys["LCONTROL"]			=	VK_LCONTROL ;
		virtkeys["RCONTROL"]			=	VK_RCONTROL ;
		virtkeys["LMENU"]				=	VK_LMENU ;
		virtkeys["RMENU"]				=	VK_RMENU ;
		virtkeys["BROWSER_BACK"]		=	VK_BROWSER_BACK ;
		virtkeys["BROWSER_FORWARD"]		=	VK_BROWSER_FORWARD ;
		virtkeys["BROWSER_REFRESH"]		=	VK_BROWSER_REFRESH ;
		virtkeys["BROWSER_STOP"]		=	VK_BROWSER_STOP ;
		virtkeys["BROWSER_SEARCH"]		=	VK_BROWSER_SEARCH ;
		virtkeys["BROWSER_FAVORITES"]	=	VK_BROWSER_FAVORITES ;
		virtkeys["BROWSER_HOME"]		=	VK_BROWSER_HOME ;
		virtkeys["VOLUME_MUTE"]			=	VK_VOLUME_MUTE ;
		virtkeys["VOLUME_DOWN"]			=	VK_VOLUME_DOWN ;
		virtkeys["VOLUME_UP"]			=	VK_VOLUME_UP ;
		virtkeys["MEDIA_NEXT_TRACK"]	=	VK_MEDIA_NEXT_TRACK ;
		virtkeys["MEDIA_PREV_TRACK"]	=	VK_MEDIA_PREV_TRACK ;
		virtkeys["MEDIA_STOP"]			=	VK_MEDIA_STOP ;
		virtkeys["MEDIA_PLAY_PAUSE"]	=	VK_MEDIA_PLAY_PAUSE ;
		virtkeys["LAUNCH_MAIL"]			=	VK_LAUNCH_MAIL ;
		virtkeys["LAUNCH_MEDIA_SELECT"]	=	VK_LAUNCH_MEDIA_SELECT ;
		virtkeys["LAUNCH_APP1"]			=	VK_LAUNCH_APP1 ;
		virtkeys["LAUNCH_APP2"]			=	VK_LAUNCH_APP2 ;
		virtkeys["OEM_1"]				=	VK_OEM_1 ;
		virtkeys["OEM_PLUS"]			=	VK_OEM_PLUS ;
		virtkeys["OEM_COMMA"]			=	VK_OEM_COMMA ;
		virtkeys["OEM_MINUS"]			=	VK_OEM_MINUS ;
		virtkeys["OEM_PERIOD"]			=	VK_OEM_PERIOD ;
		virtkeys["OEM_2"]				=	VK_OEM_2 ;
		virtkeys["OEM_3"]				=	VK_OEM_3 ;
	}
}
