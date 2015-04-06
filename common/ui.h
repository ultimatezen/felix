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
		{ L"Felix �p��W", L"*.fgloss" },
		{ L"XML �t�@�C��", L"*.xml" },
		{ L"�}���`�^�[��(Multiterm) 5.5", L"*.txt" },
		{ L"�}���`�^�[��(Multiterm) 6.0", L"*.txt" },
		{ L"�G�N�Z���t�@�C��", L"*.xls" },
		{ L"�^�u��؂�e�L�X�g", L"*.txt" }
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
		{ L"Felix �p��W", L"*.fgloss;*.xml" },
		{ L"�}���`�^�[��(Multiterm)", L"*.txt" },
		{ L"�^�u��؂�e�L�X�g", L"*.txt" },
		{ L"���ׂẴt�@�C��", L"*.*" }
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
		{ L"Felix TM�t�@�C��", L"*.ftm" },
		{ L"XML �t�@�C��", L"*.xml" },
		{ L"TMX �t�@�C��", L"*.tmx" },
		{ L"�g���h�X(Trados)�e�L�X�g�t�@�C��", L"*.txt" },
		{ L"�G�N�Z���t�@�C��", L"*.xls" },
		{ L"�^�u��؂�e�L�X�g", L"*.txt" }

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
		{ L"Felix TM�t�@�C��", L"*.xml;*.ftm" },
		{ L"TMX �t�@�C��", L"*.tmx" },
		{ L"�g���h�X(Trados)�t�@�C��", L"*.txt" },
		{ L"���ׂẴt�@�C��", L"*.*" }
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
		{ L"Felix �ݒ�t�@�C��", L"*.fprefx" }
	};

#ifndef POWERPOINT_ASSIST_APP
	if (get_module_language(_Module) == LANG_JAPANESE)
	{
		return prefs_file_filter_japanese;
	}
#endif
	return prefs_file_filter_english;
}
