#include "StdAfx.h"
#include "EditTransRecordDialogRegGloss.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

// message handlers 

// Function name	: CEditTransRecordDialogRegGloss::OnInitDialog
// Description	    : 
// Return type		: LRESULT 
LRESULT CEditTransRecordDialogRegGloss::OnInitDialog( )
{
	SENSE("OnInitDialog") ;

	initialize_dialog() ;

	fill_from_record( m_old_record ) ;

	// set focus to source edit
	m_source_edit.set_html_focus() ;

	return TRUE ;
}


LRESULT CEditTransRecordDialogRegGloss::OnCancel( )
{
	SENSE("OnCancel"); 
	EndDialog(IDCANCEL);
	return 0L;
}

LRESULT CEditTransRecordDialogRegGloss::OnOK( )
{
	SENSE("OnOK") ;

	try
	{
		// source
		const wstring source( strip_tags(BSTR2wstring(m_source_edit.GetText())) ) ;
		// trans
		const wstring trans( strip_tags(BSTR2wstring(m_trans_edit.GetText())) ) ;

		if ( source.empty() ) 
		{
			throw CException( R2T( m_is_glossary ? IDS_MSG_NO_EMPTY_S_GLOSS : IDS_MSG_NO_EMPTY_S ) ) ;
		}
		if ( trans.empty() ) 
		{
			throw CException( R2T( m_is_glossary ? IDS_MSG_NO_EMPTY_T_GLOSS : IDS_MSG_NO_EMPTY_T ) ) ;
		}
		fill_record() ;

	}
	catch( CException &e )
	{
		e.add_to_message( R2T( IDS_MSG_EDIT_REC_FAILED ) ) ;
		e.notify_user(0) ;
		return 0L ;
	}

	EndDialog(IDOK);
	return 0L;
}

void CEditTransRecordDialogRegGloss::fill_record()
{
	fill_rec_from_gui(m_old_record) ;

	m_old_record->modify() ;
}

void CEditTransRecordDialogRegGloss::set_record( record_type record )
{
	m_old_record = record ; 
}

CEditTransRecordDialogRegGloss::CEditTransRecordDialogRegGloss()
{
}

CEditTransRecordDialogRegGloss::record_type CEditTransRecordDialogRegGloss::get_record()
{
	return m_old_record ;
}

LRESULT CEditTransRecordDialogRegGloss::OnEditStrings()
{
	edit_strings(m_old_record) ;
	return 0L ;
}

LRESULT CEditTransRecordDialogRegGloss::OnAddString()
{
	add_string(m_old_record) ;
	return 0L ;
}
