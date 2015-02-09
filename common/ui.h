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



/**
	@class windows_ui
	@brief Interface to the Windows UI (built-in dialogs).
 */
class windows_ui
{

	HWND	m_parent_hwnd ;

public:
	windows_ui( HWND parent = GetActiveWindow() ) : m_parent_hwnd( parent ) {}

	bool get_open_files(file::OpenDlgList &list, LPCTSTR prompt = TEXT("Open"), LPCTSTR filter = NULL, LPCTSTR def_ext = NULL);
	CString get_open_file(LPCTSTR prompt = TEXT("Open"), LPCTSTR filter = NULL, LPCTSTR def_ext = NULL, LPCTSTR default_file = NULL);
} ;

/**
	@class file_dlg
	@brief Wrapper for the system file dialog.
 */
class file_dlg
{

public:
	CFileDialog m_dialog ;
	HWND	m_parent_hwnd ;

	file_dlg( BOOL is_open, HWND parent ) :
		m_dialog(is_open),
		m_parent_hwnd(parent)
		{
			// owner
			m_dialog.m_ofn.hwndOwner = m_parent_hwnd ;

			// filter
			m_dialog.m_ofn.lpstrFilter = 	NULL  ;

			// default extension
			m_dialog.m_ofn.lpstrDefExt = NULL  ;
	}

		int get_selected_index()
		{
			return m_dialog.m_ofn.nFilterIndex ;
		}
		void set_prompt( tstring prompt )
		{
			_tcsncpy_s((LPTSTR)m_dialog.m_szFileTitle, 64, (LPCTSTR)prompt.c_str(), prompt.size() ) ;
		}
		void set_filter( LPCTSTR filter )
		{
			// filter
			m_dialog.m_ofn.lpstrFilter = 	filter  ;
		}
		void set_default_file( tstring default_file )
		{
			_tcsncpy_s((LPTSTR)m_dialog.m_szFileName, _MAX_PATH, (LPCTSTR)default_file.c_str(), default_file.size() ) ;
		}
		void set_initial_dir( tstring initial_dir )
		{
			m_dialog.m_ofn.lpstrInitialDir = m_dialog.m_szFileName ;
			_tcsncpy_s((LPTSTR)m_dialog.m_szFileName, _MAX_PATH, (LPCTSTR)initial_dir.c_str(), initial_dir.size() ) ;
		}

};

/**
	@class open_file_dlg 
	@brief Wrapper for system open file dialog.
 */
class open_file_dlg 
	: public file_dlg
{

public:
	open_file_dlg( HWND parent = GetActiveWindow() ) :
		file_dlg(TRUE, parent)
		{
			// flags
			m_dialog.m_ofn.Flags |= OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;

			// title
			tstring title = _T("Open") ;
			_tcsncpy_s((LPTSTR)m_dialog.m_szFileTitle, 64, (LPCTSTR)title.c_str(), title.size() ) ;
		}

		bool get_open_files( file::OpenDlgList &list ) ;
		CString get_open_file( ) ;

};



//////////////////////////////////////////////////////////////////////
// member function definitions
//////////////////////////////////////////////////////////////////////

inline bool windows_ui::get_open_files( file::OpenDlgList &flist, LPCTSTR prompt, LPCTSTR filter, LPCTSTR def_ext )
{

	CFileDialog dialog(TRUE) ; // TRUE for FileOpen, FALSE for FileSaveAs
	// owner
	dialog.m_ofn.hwndOwner = m_parent_hwnd ;
	// flags
	dialog.m_ofn.Flags |= OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;
	// title
	dialog.m_ofn.lpstrTitle = prompt  ;
	// filter
	dialog.m_ofn.lpstrFilter = 	filter  ;
	// default extension
	dialog.m_ofn.lpstrDefExt = def_ext  ;

#ifndef UNIT_TEST
	if ( dialog.DoModal(m_parent_hwnd) == IDCANCEL)
	{
		return false ;
	}
	flist = (LPCTSTR)dialog.m_ofn.lpstrFile ;
	return true ;
#else
	flist ;
	return false ;
#endif
}


inline CString windows_ui::get_open_file( LPCTSTR prompt , LPCTSTR filter, LPCTSTR def_ext, LPCTSTR default_file ) 
{
	CString file_name ;

	CFileDialog dialog(TRUE) ; // TRUE for FileOpen, FALSE for FileSaveAs
	// owner
	dialog.m_ofn.hwndOwner = m_parent_hwnd ;
	// flags
	dialog.m_ofn.Flags |= OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;
	// title
	dialog.m_ofn.lpstrTitle = prompt  ;
	// filter
	dialog.m_ofn.lpstrFilter = 	filter  ;
	// default extension
	dialog.m_ofn.lpstrDefExt = def_ext  ;
	if ( default_file ) 
	{
		tstring title = (LPCTSTR)default_file;
		_tcsncpy_s(dialog.m_szFileName, _MAX_PATH, (LPCTSTR)title.c_str(), title.size() ) ;
	}

	switch ( dialog.DoModal()  )
	{
	case IDOK:
			file_name.ReleaseBuffer() ;
			return file_name ;
	}
	return CString() ;
}



inline bool open_file_dlg::get_open_files( file::OpenDlgList &flist  )
{
#ifdef UNIT_TEST
	flist ;
	return false ;
#else
	// flags
	m_dialog.m_ofn.Flags |= OFN_ALLOWMULTISELECT ;

	if ( m_dialog.m_ofn.lpstrFilter == NULL ) 
	{
		m_dialog.m_ofn.lpstrFilter = NULL ;
	}

	if ( IDOK == m_dialog.DoModal(m_parent_hwnd)  )
	{
		flist = (LPCTSTR)m_dialog.m_ofn.lpstrFile ;
		return true ;
	}

	return false ;
#endif
}


inline CString open_file_dlg::get_open_file( ) 
{
	if ( m_dialog.m_ofn.lpstrFilter == NULL ) 
	{
		m_dialog.m_ofn.lpstrFilter = NULL;
	}

	if ( IDOK == m_dialog.DoModal(m_parent_hwnd)  )
	{
		return CString( m_dialog.m_ofn.lpstrFile ) ;
	}

	return CString() ;
}


