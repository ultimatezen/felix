/*
  Dialog for editing translation records.
*/
#pragma once

#include "EditTransRecordDialogCommon.h"


class CEditTransRecordDialog : public CEditTransRecordDialogCommon<CEditTransRecordDialog>
{
public:
	typedef CEditTransRecordDialogCommon<CEditTransRecordDialog> BaseClass ;

	record_type m_new_record ;
	LPARAM			m_display_state ;

	CEditTransRecordDialog();

	void fill_from_old_record() ;
	void fill_new_record();

	void set_new_record (record_type record ) { m_new_record = record ; }
	void set_old_record (record_type record );
	record_type get_old_record();
	record_type get_new_record();
	void set_display_state( LPARAM display_state ) { m_display_state = display_state ; }

	// message handlers
	LRESULT OnInitDialog() ;
	LRESULT OnCancel() ;
	LRESULT OnOK() ;
	LRESULT OnEditStrings() ;
	LRESULT OnAddString() ;

	BEGIN_MSG_MAP_EX( CEditTransRecordDialog )
		CHAIN_MSG_MAP(BaseClass) ;
	END_MSG_MAP()
};
