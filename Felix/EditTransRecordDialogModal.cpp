#include "StdAfx.h"
#include "EditTransRecordDialogModal.h"

using namespace except ;
using namespace mem_engine ;

// Function name	: CEditTransRecordDialogModal::OnInitDialog
// Description	    : 
// Return type		: LRESULT 
LRESULT CEditTransRecordDialogModal::OnInitDialog( )
{
	initialize_dialog() ;
	init_dlg_part_two() ;
	return TRUE ;
}


LRESULT CEditTransRecordDialogModal::OnCancel( )
{
	SENSE("OnCancel"); 
#ifndef UNIT_TEST
	EndDialog(IDCANCEL);
#endif
	return 0L;
}

LRESULT CEditTransRecordDialogModal::OnOK( )
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
			throw except::CException( R2T( m_is_glossary ? IDS_MSG_NO_EMPTY_S_GLOSS : IDS_MSG_NO_EMPTY_S ) ) ;
		}
		if ( trans.empty() ) 
		{
			throw except::CException( R2T( m_is_glossary ? IDS_MSG_NO_EMPTY_T_GLOSS : IDS_MSG_NO_EMPTY_T ) ) ;
		}
		fill_record() ;

	}
	catch( CException &e )
	{
		e.add_to_message( R2T( IDS_MSG_EDIT_REC_FAILED ) ) ;
		e.notify_user(0) ;
		return 0L ;
	}

#ifndef UNIT_TEST
	EndDialog(IDOK);
#endif
	return 0L;
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
