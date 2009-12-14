/*
  Modal dialog for editing translation records.
*/
#pragma once

#include "EditTransRecordDialogCommon.h"


class CEditTransRecordDialogRegGloss : public CEditTransRecordDialogCommon<CEditTransRecordDialogRegGloss>
{
public:
	typedef CEditTransRecordDialogCommon<CEditTransRecordDialogRegGloss> BaseClass ;

	CEditTransRecordDialogRegGloss();

	void fill_record();

	void set_record (record_type record );
	record_type get_record();

	// message handlers
	LRESULT OnInitDialog() ;
	LRESULT OnCancel() ;
	LRESULT OnOK() ;
	LRESULT OnEditStrings() ;
	LRESULT OnAddString() ;

	BEGIN_MSG_MAP_EX( CEditTransRecordDialogRegGloss )
		CHAIN_MSG_MAP(BaseClass) ;
	END_MSG_MAP()
};
