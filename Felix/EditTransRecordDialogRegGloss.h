#pragma once

#include "EditTransRecordDialogCommon.h"
#include "resource.h"

/**
Modeless dialog for registering glossary entries.
*/
class CEditTransRecordDialogRegGloss :
	public CEditTransRecordDialogCommon<CEditTransRecordDialogRegGloss, IDD_EDIT_RECORD_GLOSS>
{
public:

	bool m_make_defaults ;
	typedef CEditTransRecordDialogCommon<CEditTransRecordDialogRegGloss, IDD_EDIT_RECORD_GLOSS> BaseClass ;

	CEditTransRecordDialogRegGloss(bool make_defaults=false);

	void fill_record();

	void set_record (record_type record );
	record_type get_record();

	// message handlers
	LRESULT OnInitDialog() ;
	LRESULT OnCancel() ;
	LRESULT OnOK() ;
	LRESULT OnEditStrings() ;
	LRESULT OnAddString() ;
	LRESULT OnSize(UINT type, CSize size);

	BEGIN_MSG_MAP_EX( CEditTransRecordDialogRegGloss )
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(BaseClass) ;
	END_MSG_MAP()
};
