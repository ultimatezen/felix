#pragma once

#include "config_file.h"
#include "pugxml/src/pugixml.hpp"
#include "key_mapper.h"
#include "xml_writer.h"

void add_child_text(pugi::xml_node &node, string name, string text) ;
void create_shortcut_node(pugi::xml_node &node, 
						  string stype, 
						  string first, 
						  string second, 
						  string command) ;


inline bool shift_key_is_pressed()
{
	return !! ( ::GetKeyState(VK_SHIFT) & 0x8000 ) ;
}
inline bool control_key_is_pressed()
{
	return !! ( ::GetKeyState(VK_CONTROL) & 0x8000 ) ;
}

inline bool alt_key_is_pressed( WORD key_message )
{
	return !! ( key_message & KF_ALTDOWN ) ;
}
namespace shortcuts
{
	typedef enum 
	{
		SHORTCUT_CTRL,
		SHORTCUT_ALT,
		SHORTCUT_CTRL_ALT,
	} SHORTCUT_TYPE ;

	struct KeyboardShortcut
	{
		const SHORTCUT_TYPE m_type ;
		const wchar_t m_firstkey ;
		const wchar_t m_secondkey ;
		const string m_command ;
		KeyboardShortcut(SHORTCUT_TYPE stype,
						wchar_t firstkey,
						wchar_t secondkey,
						const string &command);
	};

	typedef boost::shared_ptr<KeyboardShortcut> shortcut_ptr ;

	class KeyboardShortcuts
	{
	public:
		typedef std::map<wchar_t, shortcut_ptr> shortcut_map ;
		typedef boost::shared_ptr<shortcut_map> shortcut_map_ptr ;
		typedef std::map<wchar_t, shortcut_map_ptr> key_map ;

		key_map m_ctrl_shortcuts ;
		key_map m_alt_shortcuts ;
		key_map m_ctrl_alt_shortcuts ;

		std::map<string, wchar_t> virtkeys ;
		KeyMapper				*m_mapper ;

		wchar_t					m_second_key ;
		shortcut_map_ptr		m_second_keys ;
		string					m_second_command ;
		bool					m_shortcuts_enabled ;

		boost::function<void(BOOL)> m_on_toggle_shortcuts ;

		KeyboardShortcuts(KeyMapper *mapper);
		void load(const string text);

		void add_shortcut( shortcut_ptr shortcut );
		bool receive_keypress(bool ctrl_pressed, bool alt_pressed, wchar_t key);

		key_map * get_key_map( bool ctrl_pressed, bool alt_pressed );
		shortcut_ptr load_shortcut( pugi::xml_node &shortcut_node );

		SHORTCUT_TYPE get_shortcut_type(const string shortcut_type) ;
		wchar_t get_key(const string shortcut_key) ;
	};
}
