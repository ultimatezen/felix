#pragma once

#include "ui.h"


CString save_memory_file(CString dialog_title, const wstring last_save, const CString filename);
CString save_glossary_file(CString dialog_title, const wstring last_save, const CString filename);
CString save_preferences_file(CString dialog_title, const wstring last_save, const CString filename);

CString save_file(
	const wstring last_save,
	const CString filename,
	const CString title,
	filter_array file_filter,
	const CString default_ext);

class file_dialog
{
public:
	file_dialog();
	~file_dialog();
};

inline CString save_memory_file(CString dialog_title, const wstring last_save, const CString filename)
{
	//	CString dialog_title;
	//	dialog_title.FormatMessage(IDS_SAVE, resource_string(IDS_MEMORY));

	return save_file(last_save, filename, dialog_title, get_mem_save_filter(), L"ftm");
}
inline CString save_glossary_file(CString dialog_title, const wstring last_save, const CString filename)
{
	//	CString dialog_title;
	//	dialog_title.FormatMessage(IDS_SAVE, resource_string(IDS_GLOSSARY));
	return save_file(last_save, filename, dialog_title, get_gloss_save_filter(), L"fgloss");
}
inline CString save_preferences_file(CString dialog_title, const wstring last_save, const CString filename)
{
	//	CString dialog_title;
	//	dialog_title.LoadString(IDS_SAVE_PREFS_TITLE);
	return save_file(last_save, filename, dialog_title, get_prefs_filter(), L"fprefx");
}

class file_save_dialog
{
	wstring m_last_save;
	CString m_filename;
	CString m_title;
	filter_array m_file_filter;
	CString m_default_ext;

	CComPtr<IFileSaveDialog> m_dialog;

public:
	file_save_dialog(
		const wstring last_save,
		const CString filename,
		const CString title,
		filter_array file_filter,
		const CString default_ext) :
		m_last_save(last_save),
		m_filename(filename),
		m_title(title),
		m_file_filter(file_filter),
		m_default_ext(default_ext)
	{}

	void set_last_save(const wstring last_save)
	{
		m_last_save = last_save;
	}
	void set_filename(const CString filename)
	{
		m_filename = filename;
	}
	void set_title(const CString title)
	{
		m_title = title;
	}
	void set_file_filter(filter_array file_filter)
	{
		m_file_filter = file_filter;
	}
	void set_default_ext(const CString default_ext)
	{
		m_default_ext = default_ext;
	}

	bool show()
	{
		COM_ENFORCE(m_dialog.CoCreateInstance(CLSID_FileSaveDialog), L"Failed to create file save dialog");

		m_dialog->SetFileTypes(m_file_filter.size(), &m_file_filter[0]);
		m_dialog->SetTitle(m_title);
		if (!m_filename.IsEmpty() && file::CPath(m_filename).FileExists())
		{
			m_dialog->SetFileName(m_filename);
		}
		else if (!m_last_save.empty() && file::CPath(CString(m_last_save.c_str())).IsDirectory())
		{
			PIDLIST_ABSOLUTE pidl;
			if (SUCCEEDED(::SHParseDisplayName(m_last_save.c_str(), nullptr, &pidl, SFGAO_FOLDER, 0)))
			{
				IShellItem *psi;
				if (SUCCEEDED(::SHCreateShellItem(NULL, NULL, pidl, &psi)))
				{
					m_dialog->SetFolder(psi);
					ILFree(pidl);
				}
			}
		}
		m_dialog->SetDefaultExtension(m_default_ext);

		HRESULT hr = m_dialog->Show(::GetForegroundWindow());

		if (!SUCCEEDED(hr))
		{
			return false;
		}
		return true;
	}

	CString get_save_destination()
	{
		CComPtr<IShellItem> item;

		COM_ENFORCE(m_dialog->GetResult(&item), L"Failed to get result from save file dialog");

		LPOLESTR pwsz = nullptr;
		COM_ENFORCE(item->GetDisplayName(SIGDN_FILESYSPATH, &pwsz), L"Failed to get display name");
		CString destination(pwsz);
		::CoTaskMemFree(pwsz);
		return destination;
	}

	UINT get_selected_index()
	{
		UINT result = 0;
		COM_ENFORCE(m_dialog->GetFileTypeIndex(&result), L"Failed to get file type index");
		return result;
	}

};

inline CString save_file(
	const wstring last_save,
	const CString filename,
	const CString title,
	filter_array file_filter,
	const CString default_ext)
{
	file_save_dialog dialog(
		last_save,
		filename,
		title,
		file_filter,
		default_ext);

	if (!dialog.show())
	{
		return L"";
	}
	return dialog.get_save_destination();
}
