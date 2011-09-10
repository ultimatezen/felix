/*!
	@file ui.h
	@brief interface for the windows_ui class
	@date 2005/06/25
	Time: 14:53:47
	@author Ryan Ginstrom
 */

#pragma once

#include "File.h"
#include "atldlgs.h"
#include "memory_debugging.h"

#define CPP_FILE_FILTER		TEXT("CPP Files (*.h; *.cpp)\0*.cpp;*.h\0All files (*.*)\0*.*\0")
#define CPP_FILE_EXT		TEXT("h") 

#define XML_FILE_FILTER		TEXT("Xml Files (*.xml)\0*.xml\0All files (*.*)\0*.*\0")
#define XML_FILE_EXT		TEXT("xml") 

#define XML_FILES_FILTER	XML_FILE_FILTER

#define TXT_FILE_FILTER		TEXT("Text Files (*.txt)\0*.txt\0All files (*.*)\0*.*\0")
#define TXT_FILE_EXT		TEXT("txt") 

#define TXT_FILES_FILTER	TXT_FILE_FILTER

#define RC_FILE_FILTER		TEXT("RC Files (*.rc)\0*.rc\0All files (*.*)\0*.*\0")
#define RC_FILE_EXT			TEXT("rc") 

#define RC_FILES_FILTER		RC_FILE_FILTER

#define PPT_FILES_FILTER	TEXT("PowerPoint Files (*.ppt)\0*.ppt\0All files (*.*)\0*.*\0")
#define PPT_FILE_EXT		TEXT("ppt") 

#define HTML_FILE_FILTER	TEXT("HTML Files (*.htm, *.html)\0*.htm;*.html\0All files (*.*)\0*.*\0")
#define HTML_FILE_EXT		TEXT("html") 

#define DOC_FILES_FILTER	TEXT("Word Files (*.doc)\0*.doc\0Text Files (*.txt)\0*.txt\0All files (*.*)\0*.*\0")
#define DOC_FILE_EXT		TEXT("doc") 

#define HTML_FILES_FILTER	HTML_FILE_FILTER

#define ALL_FILES_FILTER	TEXT("All Files (*.*)\0*.*\0")

#ifndef POWERPOINT_ASSIST_APP
inline LPCTSTR get_gloss_save_filter()
{
	static LPCTSTR glossary_file_filter_english = 
		_T("Felix Glossary File (*.fgloss)\0*.fgloss\0")
		_T("XML File (*.xml)\0*.xml\0")
		_T("Multiterm 5.5 (*.txt)\0*.txt\0")
		_T("Multiterm 6.0 (*.txt)\0*.txt\0")
		_T("Excel File (*.xls)\0*.xls\0") 
		_T("Tab-separated Text File (*.txt)\0*.txt\0") ;

	static LPCTSTR glossary_file_filter_japanese = 
		_T("Felix 用語集 (*.fgloss)\0*.fgloss\0")
		_T("XML ファイル (*.xml)\0*.xml\0")
		_T("マルチターム(Multiterm) 5.5 (*.txt)\0*.txt\0")
		_T("マルチターム(Multiterm) 6.0 (*.txt)\0*.txt\0")
		_T("エクセルファイル (*.xls)\0*.xls\0") 
		_T("タブ区切りテキスト (*.txt)\0*.txt\0") ;

	if (boost::icontains(_Module.get_library(), _T("j")))
	{
		return glossary_file_filter_japanese ;
	}
	return glossary_file_filter_english ;
}
inline LPCTSTR get_gloss_open_filter()
{
	static LPCTSTR glossary_file_filter_english = 
		_T("Felix Glossary Files (*.fgloss;*.xml)\0*.fgloss;*.xml\0")
		_T("Multiterm Files (*.txt)\0*.txt\0")
		_T("Tab-delimited Text Files (*.txt)\0*.txt\0")
		_T("All files (*.*)\0*.*\0") ;

	static LPCTSTR glossary_file_filter_japanese = 
		_T("Felix 用語集 (*.fgloss;*.xml)\0*.fgloss;*.xml\0")
		_T("マルチターム(Multiterm) (*.txt)\0*.txt\0")
		_T("タブ区切り用語集 (*.txt)\0*.txt\0")
		_T("すべてのファイル (*.*)\0*.*\0") ;

	if (boost::icontains(_Module.get_library(), _T("j")))
	{
		return glossary_file_filter_japanese ;
	}
	return glossary_file_filter_english ;
}
inline LPCTSTR get_mem_save_filter()
{
	static LPCTSTR memory_file_save_filter_english = 
		_T("Felix Memory File (*.ftm)\0*.ftm\0")
		_T("XML File (*.xml)\0*.xml\0")
		_T("TMX File (*.tmx)\0*.tmx\0")
		_T("Trados Text File (*.txt)\0*.txt\0") 
		_T("Excel File (*.xls)\0*.xls\0") 
		_T("Tab-separated Text File (*.txt)\0*.txt\0") ;

	static LPCTSTR memory_file_save_filter_japanese = 
		_T("Felix TMファイル (*.ftm)\0*.ftm\0")
		_T("XML ファイル (*.xml)\0*.xml\0")
		_T("TMX ファイル (*.tmx)\0*.tmx\0")
		_T("トラドス(Trados)テキストファイル (*.txt)\0*.txt\0") 
		_T("エクセルファイル (*.xls)\0*.xls\0") 
		_T("タブ区切りテキスト (*.txt)\0*.txt\0") ;

	if (boost::icontains(_Module.get_library(), _T("j")))
	{
		return memory_file_save_filter_japanese ;
	}
	return memory_file_save_filter_english ;
}
inline LPCTSTR get_mem_open_filter()
{
	static LPCTSTR memory_file_filter_english = 
		_T("Felix Memory Files (*.ftm; *.xml)\0*.xml;*.ftm\0")
		_T("TMX Files (*.tmx)\0*.tmx\0")
		_T("Trados Text Files (*.txt)\0*.txt\0")
		_T("All files (*.*)\0*.*\0") ;

	static LPCTSTR memory_file_filter_japanese = 
		_T("Felix TMファイル (*.ftm; *.xml)\0*.xml;*.ftm\0")
		_T("TMX ファイル (*.tmx)\0*.tmx\0")
		_T("トラドス(Trados)ファイル(*.txt)\0*.txt\0")
		_T("すべてのファイル(*.*)\0*.*\0") ;

	if (boost::icontains(_Module.get_library(), _T("j")))
	{
		return memory_file_filter_japanese ;
	}
	return memory_file_filter_english ;
}
inline LPCTSTR get_prefs_filter()
{
	static LPCTSTR prefs_file_filter_english = 
		_T("Felix Preference Files (*.fprefx)\0*.fprefx\0")
		_T("Old Felix Preference Files (*.fprefs)\0*.fprefs\0") ;

	static LPCTSTR prefs_file_filter_japanese = 
		_T("Felix 設定ファイル(*.fprefx)\0*.fprefx\0")
		_T("前バージョンのFelix 設定ファイル(*.fprefs)\0*.fprefs\0") ;

	if (boost::icontains(_Module.get_library(), _T("j")))
	{
		return prefs_file_filter_japanese ;
	}
	return prefs_file_filter_english ;
}
#endif

/**
	@class windows_ui
	@brief Interface to the Windows UI (built-in dialogs).
 */
class windows_ui
{

	HWND	m_parent_hwnd ;

public:
	windows_ui( HWND parent = GetActiveWindow() ) : m_parent_hwnd( parent ) {}
	bool get_folder( CString &folder, LPCTSTR title = TEXT("Select Folder"), DWORD flags = BIF_BROWSEINCLUDEFILES | BIF_EDITBOX, LPOLESTR root = NULL ) ;

	bool get_save_file( CString &file_name, LPCTSTR prompt = TEXT("Save"), LPCTSTR filter = ALL_FILES_FILTER, LPCTSTR def_ext = NULL, LPTSTR custom_filter = NULL, int filter_len = 0, int index = 1 );
	CString get_save_file( LPCTSTR prompt = TEXT("Save"), LPCTSTR filter = ALL_FILES_FILTER, LPCTSTR def_ext = NULL, LPCTSTR default_file = TEXT(""), LPTSTR custom_filter = NULL, int filter_len = 0, int index = 1);

	bool get_open_files( file::OpenDlgList &list, LPCTSTR prompt = TEXT("Open"), LPCTSTR filter = ALL_FILES_FILTER, LPCTSTR def_ext = NULL ) ;
	CString get_open_file( LPCTSTR prompt = TEXT("Open"), LPCTSTR filter = ALL_FILES_FILTER, LPCTSTR def_ext = NULL, LPCTSTR default_file = NULL ) ;
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

/**
	@class save_file_dlg 
	@brief Wrapper for system save dialog.
 */
class save_file_dlg 
	: public file_dlg
{

public:
	save_file_dlg( HWND parent = GetActiveWindow() ) :
		file_dlg(FALSE, parent)
		{
			// flags
			m_dialog.m_ofn.Flags |= OFN_OVERWRITEPROMPT ;

			// title
			tstring title = _T("Save") ;
			_tcsncpy_s((LPTSTR)m_dialog.m_szFileTitle, 64, (LPCTSTR)title.c_str(), title.size() ) ;
		}

		CString get_save_file( ) ;
};

//////////////////////////////////////////////////////////////////////
// member function definitions
//////////////////////////////////////////////////////////////////////

inline bool windows_ui::get_folder( CString &folder, LPCTSTR title, DWORD flags, LPOLESTR root )
{
	BANNER( "windows_ui::get_folder" ) ;

	CFolderDialog folder_dialog( m_parent_hwnd, title , flags ) ;

	if ( root != NULL ) // ok, we have a root specified
	{
		// get the desktop folder. We need this to get the pidl of our root directory
		CComPtr< IShellFolder > pShell ;
		HRESULT hr = SHGetDesktopFolder( &pShell ) ;
		COM_ENFORCE(hr, _T("Failed to get desktop folder")) ;

		// now get the pidl for it
		LPITEMIDLIST ppidl ;
		ULONG eaten ;

		COM_ENFORCE( pShell->ParseDisplayName(
			NULL,		// HWND hwndOwner,
			NULL,		// LPBC pbcReserved -- always NULL
			root,		//  LPOLESTR lpszDisplayName,
			&eaten,		//  ULONG *pchEaten,
			&ppidl,		// LPITEMIDLIST *ppidl,
			NULL		// ULONG *pdwAttributes
			), _T("Failed to parse display name: ") + CString( root ) ) ;

		// now feed the pidl into our folder dialog
		folder_dialog.m_bi.pidlRoot = ppidl ;
	}

	if ( folder_dialog.DoModal() != IDOK )
		return false ;
	
	folder = folder_dialog.GetFolderPath() ;

	return true ;
}

inline bool windows_ui::get_save_file(CString &file_name, LPCTSTR prompt, LPCTSTR filter, LPCTSTR def_ext, LPTSTR custom_filter, int filter_len, int index )
{
	CFileDialog dialog(FALSE) ; // TRUE for FileOpen, FALSE for FileSaveAs

	dialog.m_ofn.lpstrTitle = prompt  ;
	dialog.m_ofn.lpstrFilter = 	filter  ;
	dialog.m_ofn.lpstrDefExt = def_ext  ;
	dialog.m_ofn.lpstrCustomFilter = (LPTSTR)custom_filter ;
	dialog.m_ofn.nMaxCustFilter = filter_len;
	dialog.m_ofn.nFilterIndex = index ;
	dialog.m_ofn.hwndOwner = m_parent_hwnd;
	tstring title = (LPCTSTR)file_name;
	_tcsncpy_s(dialog.m_ofn.lpstrFile, _MAX_PATH, (LPCTSTR)title.c_str(), title.size() ) ;

	INT_PTR response = dialog.DoModal(m_parent_hwnd) ;

	if (response == IDOK)
	{
		file_name = dialog.m_ofn.lpstrFile ;
		return true ;
	}

	return false ;
}

inline CString windows_ui::get_save_file( LPCTSTR prompt, LPCTSTR filter, LPCTSTR def_ext, LPCTSTR default_file, LPTSTR custom_filter, int filter_len, int index )
{
	CString file_name ;
	if (default_file != NULL)
	{
		file_name = default_file ;
	}
	get_save_file(file_name, prompt, filter, def_ext, custom_filter, filter_len, index) ;

	return file_name  ;
}

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
	dialog.m_ofn.lpstrFile  ;

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
		m_dialog.m_ofn.lpstrFilter = ALL_FILES_FILTER ;
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
		m_dialog.m_ofn.lpstrFilter = ALL_FILES_FILTER ;
	}

	if ( IDOK == m_dialog.DoModal(m_parent_hwnd)  )
	{
		return CString( m_dialog.m_ofn.lpstrFile ) ;
	}

	return CString() ;
}

inline CString save_file_dlg::get_save_file( ) 
{
	LPCTSTR filter = ALL_FILES_FILTER ;
	if ( m_dialog.m_ofn.lpstrFilter == NULL ) 
	{
		m_dialog.m_ofn.lpstrFilter = filter ;
	}

	if ( IDOK == m_dialog.DoModal(m_parent_hwnd)  )
	{
		return CString( m_dialog.m_ofn.lpstrFile ) ;
	}

	return CString() ;
}

