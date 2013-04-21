/*
  Dialog for editing translation records.
*/
#pragma once

#include "EditTransRecordDialogCommon.h"

class EditRecordInterface
{
public:
	virtual void add_edit_record(mem_engine::record_pointer record, LPARAM display_state) = 0 ;
	virtual void edit_edit_record(mem_engine::record_pointer new_record, LPARAM display_state) = 0 ;
};


class CEditTransRecordDialog : public CEditTransRecordDialogCommon<CEditTransRecordDialog>
{
public:
	typedef CEditTransRecordDialogCommon<CEditTransRecordDialog> BaseClass ;

	record_type		m_new_record ;
	LPARAM			m_display_state ;
	bool			m_is_add ;
	EditRecordInterface* m_interface ;

	CEditTransRecordDialog();

	void fill_from_old_record() ;
	void fill_new_record();

	void set_new_record (record_type record );
	void set_old_record (record_type record );
	record_type get_old_record();
	record_type get_new_record();
	void set_display_state( LPARAM display_state );

	// message handlers
	LRESULT OnInitDialog() ;
	LRESULT OnCancel() ;
	LRESULT OnOK() ;

	LRESULT notify_calling_window();
	LRESULT OnEditStrings() ;
	LRESULT OnAddString() ;

	BEGIN_MSG_MAP_EX( CEditTransRecordDialog )
		CHAIN_MSG_MAP(BaseClass) ;
	END_MSG_MAP()
};
