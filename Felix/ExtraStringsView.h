#pragma once

#include "HtmlView.h"

#include "record.h" // record_pointer ;


class CExtraStringsView
{
	CStaticT<TWindow>				m_extra_strings_box ;

	boost::shared_ptr< html::CHtmlView > m_extra_strings_view ;

	TWindow				m_parent_dlg ;


public:
	CExtraStringsView(void);
	~CExtraStringsView(void);
	bool has_focus(TWindow focus_window) const;
	LRESULT send_command(WPARAM wParam, LPARAM lParam);
	void handle_size(void);
	void create(HWND box_hwnd, HWND dlg_hwnd );
	void show_extra_strings(mem_engine::record_pointer rec);

	void fill_user_strings( mem_engine::record_pointer &rec ) ;	

	void eliminate_whitespace_and_punctuation(const std::wstring& setting, std::wstring& key) const;

	void write_table_entries(std::wstring& desc, mem_engine::record_pointer& rec) const ;

	BOOL pre_translate( MSG *pMsg );

	bool is_edit_mode(void) const;
	void put_edit_mode(bool edit_mode);
	void cleanup(void);
	void gain_focus(void);
};
