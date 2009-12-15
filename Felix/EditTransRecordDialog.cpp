#include "StdAfx.h"
#include "EditTransRecordDialog.h"
#include "record_local.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

using namespace except ;
using namespace memory_engine ;

// Function name	: CEditTransRecordDialog::OnInitDialog
// Description	    : 
// Return type		: LRESULT 
LRESULT CEditTransRecordDialog::OnInitDialog( )
{
	SENSE("OnInitDialog") ;

	initialize_dialog() ;

	// set focus to source edit
	m_source_edit.set_html_focus() ;

	return TRUE ;
}

LRESULT CEditTransRecordDialog::OnCancel( )
{
	SENSE("OnCancel") ;

	m_new_record = m_old_record ;
	::SendMessage( GetParent(), UWM_USER_MESSAGE, IDC_REPLACE_EDIT_RECORD, m_display_state ) ;
	ShowWindow( SW_HIDE ) ;
	return 0;
}


LRESULT CEditTransRecordDialog::OnOK( )
{
	SENSE("OnOK") ;

	try
	{
		fill_new_record() ;
		
		const wstring source = m_new_record->get_source_plain() ;
		if ( source.empty() ) 
		{
			throw CException( R2T( m_is_glossary ? IDS_MSG_NO_EMPTY_S_GLOSS : IDS_MSG_NO_EMPTY_S ) ) ;
		}

		const wstring trans = m_new_record->get_trans_plain() ;
		if ( trans.empty() ) 
		{
			throw CException( R2T( m_is_glossary ? IDS_MSG_NO_EMPTY_T_GLOSS : IDS_MSG_NO_EMPTY_T ) ) ;
		}

		::SendMessage( GetParent(), UWM_USER_MESSAGE, IDC_RETRIEVE_EDIT_RECORD, m_display_state ) ;
	}
	catch( CException &e )
	{
		e.add_to_message( R2T( IDS_MSG_EDIT_REC_FAILED ) ) ;
		throw CException ( e ) ;
	}

	ShowWindow( SW_HIDE ) ;
	return 0;
}


void CEditTransRecordDialog::fill_new_record()
{
	fill_rec_from_gui(m_new_record) ;
	fill_from_old_record();

	m_new_record->modify() ;
}



void CEditTransRecordDialog::set_old_record( record_type record )
{
	m_old_record = record ; 

#ifdef UNIT_TEST
	return ;
#endif

	fill_from_record( record ) ;
}

CEditTransRecordDialog::CEditTransRecordDialog(): 
	m_display_state( 0 )
{
	m_new_record = record_type(new record_local) ;
}

void CEditTransRecordDialog::fill_from_old_record()
{
	m_new_record->set_created(m_old_record->get_created()) ;
	m_new_record->set_refcount(m_old_record->get_refcount()) ;
	m_new_record->set_id(m_old_record->get_id()) ;
}

CEditTransRecordDialog::record_type CEditTransRecordDialog::get_old_record()
{
	return m_old_record ;
}

CEditTransRecordDialog::record_type CEditTransRecordDialog::get_new_record()
{
	return m_new_record ;
}

LRESULT CEditTransRecordDialog::OnEditStrings()
{
	edit_strings(m_new_record) ;
	return 0L ;
}

LRESULT CEditTransRecordDialog::OnAddString()
{
	add_string(m_new_record) ;
	return 0L ;
}

void CEditTransRecordDialog::set_new_record( record_type record )
{
	m_new_record = record ;
}
