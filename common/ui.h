/*!
	interface for open/save file dialogs
 */

#pragma once

#include "File.h"
#include "memory_debugging.h"
#include <boost/filesystem.hpp>
#include "resource.h"

#include <vector>
#include <shobjidl.h>

using filter_array = std::vector < COMDLG_FILTERSPEC > ;

template<typename M>
int get_module_language(M &module)
{
	if (boost::icontains(module.get_library(), _T("j")))
	{
		return LANG_JAPANESE;
	}
	return LANG_ENGLISH;
}

inline filter_array get_gloss_save_filter()
{
	static filter_array glossary_file_filter_english{
		{ L"Felix glossary files", L"*.fgloss" },
		{ L"XML files", L"*.xml" },
		{ L"Multiterm 5.5", L"*.txt" },
		{ L"Multiterm 6.0", L"*.txt" },
		{ L"Excel files", L"*.xls" },
		{ L"Tab-separated text files", L"*.txt" }
	};

	static filter_array glossary_file_filter_japanese{
		{ L"Felix 用語集", L"*.fgloss" },
		{ L"XML ファイル", L"*.xml" },
		{ L"マルチターム(Multiterm) 5.5", L"*.txt" },
		{ L"マルチターム(Multiterm) 6.0", L"*.txt" },
		{ L"エクセルファイル", L"*.xls" },
		{ L"タブ区切りテキスト", L"*.txt" }
	};

#ifndef POWERPOINT_ASSIST_APP
	if (get_module_language(_Module) == LANG_JAPANESE)
	{
		return glossary_file_filter_japanese;
	}
#endif
	return glossary_file_filter_english;
}

inline filter_array get_gloss_open_filter()
{
	static filter_array glossary_file_filter_english{
		{ L"Felix glossary files", L"*.fgloss;*.xml" },
		{ L"Multiterm files", L"*.txt" },
		{ L"Tab-separated text files", L"*.txt" },
		{ L"All files", L"*.*" }
	};

	static filter_array glossary_file_filter_japanese{
		{ L"Felix 用語集", L"*.fgloss;*.xml" },
		{ L"マルチターム(Multiterm)", L"*.txt" },
		{ L"タブ区切りテキスト", L"*.txt" },
		{ L"すべてのファイル", L"*.*" }
	};

#ifndef POWERPOINT_ASSIST_APP
	if (get_module_language(_Module) == LANG_JAPANESE)
	{
		return glossary_file_filter_japanese;
	}
#endif
	return glossary_file_filter_english;
}

inline filter_array get_mem_save_filter()
{
	static filter_array memory_file_filter_english{
		{ L"Felix memory files", L"*.ftm" },
		{ L"XML files", L"*.xml" },
		{ L"TMX files", L"*.tmx" },
		{ L"Trados text files", L"*.txt" },
		{ L"Excel files", L"*.xls" },
		{ L"Tab-separated text files", L"*.txt" }

	};

	static filter_array memory_file_filter_japanese{
		{ L"Felix TMファイル", L"*.ftm" },
		{ L"XML ファイル", L"*.xml" },
		{ L"TMX ファイル", L"*.tmx" },
		{ L"トラドス(Trados)テキストファイル", L"*.txt" },
		{ L"エクセルファイル", L"*.xls" },
		{ L"タブ区切りテキスト", L"*.txt" }

	};

#ifndef POWERPOINT_ASSIST_APP
	if (get_module_language(_Module) == LANG_JAPANESE)
	{
		return memory_file_filter_japanese;
	}
#endif
	return memory_file_filter_english;
}

inline filter_array get_mem_open_filter()
{
	static filter_array memory_file_filter_english{
		{ L"Felix memory files", L"*.xml;*.ftm" },
		{ L"TMX files", L"*.tmx" },
		{ L"Trados text files", L"*.txt" },
		{ L"All files", L"*.*" }
	};

	static filter_array memory_file_filter_japanese{
		{ L"Felix TMファイル", L"*.xml;*.ftm" },
		{ L"TMX ファイル", L"*.tmx" },
		{ L"トラドス(Trados)ファイル", L"*.txt" },
		{ L"すべてのファイル", L"*.*" }
	};

#ifndef POWERPOINT_ASSIST_APP
	if (get_module_language(_Module) == LANG_JAPANESE)
	{
		return memory_file_filter_japanese;
	}
#endif
	return memory_file_filter_english;
}

inline filter_array get_prefs_filter()
{
	static filter_array prefs_file_filter_english{
		{ L"Felix preference files", L"*.fprefx" }
	};

	static filter_array prefs_file_filter_japanese{
		{ L"Felix 設定ファイル", L"*.fprefx" }
	};

#ifndef POWERPOINT_ASSIST_APP
	if (get_module_language(_Module) == LANG_JAPANESE)
	{
		return prefs_file_filter_japanese;
	}
#endif
	return prefs_file_filter_english;
}
