#pragma once
#include "MultiPaneStatusBarEx.h"
#include "NumberFmt.h"
#include "background_processor.h"

class CFelixStatusBar
{
public:
	CMPSBarWithProgress		m_mp_sbar ;
	// background processing
	// (Checks for messages on the queue)
	background_processor	m_background_processor ;
	CNumberFmt				m_number_format ;
	CString					m_progress_file ;
	int						m_progress_max ;
	bool					m_cancel_requested ;

	CFelixStatusBar() : m_cancel_requested(false)
	{
	}
	void release()
	{
		m_mp_sbar.UnsubclassWindow( ) ;
	}
	bool user_feedback( const CString &feedback, int pane = 0 )
	{
		feedback ;
		pane ;
#ifndef UNIT_TEST
		ATLASSERT( m_mp_sbar.IsWindow() ) ;
		m_mp_sbar.SetText( pane, feedback ) ;
#endif

		return true ;
	}
	bool user_feedback( const UINT id, int pane = 0 )
	{
		return user_feedback( R2TS( id ), pane ) ;
	}
	bool user_feedback( const tstring &feedback, int pane = 0 )
	{
		CString feed( feedback.c_str() ) ;
		return user_feedback( feed, pane ) ;
	}

	void OnProgressInit( const CString &file_name, size_t min_val, size_t max_val )
	{
		m_cancel_requested = false ;
		m_progress_file = file::name( file_name ).file_name()  ;

		if (m_mp_sbar.m_Progress.IsWindow() ) 
		{
			m_mp_sbar.ProgDestroyWindow() ;
		}

#ifdef UNIT_TEST
		min_val ;
		max_val ;
		return ;
#else
		init_progress_bar(min_val, max_val);
#endif
	}

	void init_progress_bar( size_t min_val, size_t max_val )
	{
		m_mp_sbar.ProgCreate( 1, min_val, max_val ) ;
		m_progress_max = max_val ;
		set_pos(min_val) ;
	}
	void set_up_prog_bar(const int max_size )
	{
		this->init_progress_bar(0, max_size) ;
	}
	void OnProgressDoneLoad( size_t final_val )
	{
		set_pos( final_val ) ;
		final_feedback( IDS_DONE_LOADING, final_val ) ;
#ifdef UNIT_TEST
		return ;
#else
		m_mp_sbar.ProgDestroyWindow() ;
#endif		
	}
	void OnProgressDoneWrite( size_t final_val )
	{
#ifdef UNIT_TEST
	final_val ;
	return ;
#else

		set_pos( final_val ) ;
		final_feedback( IDS_EXPORTED_RECORDS, final_val ) ;
		m_mp_sbar.ProgDestroyWindow() ;
#endif		
	}
	void final_feedback( UINT msg_id, size_t final_val )
	{
		if ( m_cancel_requested == false ) 
		{
			CNumberFmt nf ;
			CString feedback ;
			feedback.FormatMessage( msg_id, nf.Format( final_val ), m_progress_file ) ;
			user_feedback( feedback ) ;
			user_feedback( CString("100%"), 2 ) ;
		}
		else
		{
			user_feedback(IDS_CANCELLED_ACTION) ;
		}

		user_feedback( CString(), 2 ) ;
	}

	void set_pos(size_t current_val)
	{
#ifdef UNIT_TEST
		current_val ;
		return ;
#else
		m_mp_sbar.ProgSetPos( current_val ) ;
#endif
	}

	bool OnProgressLoadUpdate( size_t current_val ) // true to continue
	{
		set_pos(current_val) ;

		if ( m_cancel_requested ) 
		{
			return false ;
		}

		do_progress_msg( current_val, IDS_MSG_ADDED_RECORDS ) ;

		do_percent_done_msg(current_val) ;

		m_background_processor.perform_background_processing() ;

		return true ;
	}
	bool OnProgressWriteUpdate( size_t current_val ) // true to continue
	{
		set_pos(current_val) ;

		if ( m_cancel_requested ) 
		{
			return false ;
		}

		do_progress_msg( current_val, IDS_SAVED_RECORDS ) ;

		do_percent_done_msg(current_val) ;

		m_background_processor.perform_background_processing() ;

		return true ;
	}
	void do_progress_msg( size_t current_val, UINT msg_id )
	{
		CString msg ;
		msg.FormatMessage( msg_id, m_number_format.Format( current_val ), m_progress_file ) ;

		this->user_feedback( msg ) ;
	}
	void do_percent_done_msg( size_t current_val )
	{
		int percent_done = ( current_val * 100 ) / m_progress_max ;

		CString percent_done_msg ;
		percent_done_msg.Format( _T("%d%%"), percent_done ) ;

		user_feedback(percent_done_msg, 2) ;
	}
	void SetPaneWidths(int* arrWidths, int nPanes)
	{ 
		// find the size of the borders
		int arrBorders[3];
		m_mp_sbar.GetBorders(arrBorders);

		// calculate right edge of default pane (0)
		arrWidths[0] += arrBorders[2];
		for (int i = 1; i < nPanes; i++)
			arrWidths[0] += arrWidths[i];

		// calculate right edge of remaining panes (1 thru nPanes-1)
		for (int j = 1; j < nPanes; j++)
			arrWidths[j] += arrBorders[2] + arrWidths[j - 1];

		// set the pane widths
		m_mp_sbar.SetParts(m_mp_sbar.m_nPanes, arrWidths); 
	}
};