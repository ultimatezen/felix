#include "StdAfx.h"
#include "EditTransRecordDialogModal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

// message handlers 

// Function name	: CEditTransRecordDialogModal::OnInitDialog
// Description	    : 
// Return type		: LRESULT 
LRESULT CEditTransRecordDialogModal::OnInitDialog( )
{
	SENSE("OnInitDialog") ;

	initialize_dialog() ;

	fill_from_record( m_old_record ) ;

	// set focus to source edit
	m_source_edit.set_html_focus() ;

	return TRUE ;
}


LRESULT CEditTransRecordDialogModal::OnCancel( )
{
	SENSE("OnCancel"); 
	EndDialog(IDCANCEL);
	return 0;
}

LRESULT CEditTransRecordDialogModal::OnOK( )
{
	SENSE("OnOK") ;

	try
	{
		// source
		wstring source( strip_tags(BSTR2wstring(m_source_edit.GetText())) ) ;
		// trans
		wstring trans( strip_tags(BSTR2wstring(m_trans_edit.GetText())) ) ;

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
		return 0 ;
	}

	EndDialog(IDOK);
	return 0;
}

void CEditTransRecordDialogModal::fill_record()
{
	fill_rec_from_gui(m_old_record) ;

	m_old_record->modify() ;
}

void CEditTransRecordDialogModal::set_record( record_type record )
{
	m_old_record = record ; 
}

CEditTransRecordDialogModal::CEditTransRecordDialogModal()
{
}

CEditTransRecordDialogModal::record_type CEditTransRecordDialogModal::get_record()
{
	return m_old_record ;
}



LRESULT CEditTransRecordDialogModal::OnEditStrings()
{
	edit_strings(m_old_record) ;
	return 0L ;
}

LRESULT CEditTransRecordDialogModal::OnAddString()
{
	add_string(m_old_record) ;
	return 0L ;
}