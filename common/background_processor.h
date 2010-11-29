/*!
	@file background_processor.h
	@brief interface for the background_processor class.
	@date 2005/06/25
	Time: 15:10:53
	@author Ryan Ginstrom
 */

#pragma once

#include "DebugUtilities.h"
#include "atlapp.h"
#include "atluser.h"

/**
	@class background_processor  
	@brief Poor man's multithreading.
 */
class background_processor  
{
	size_t	m_num_iterations ;
	size_t	m_iteration ;
	CAcceleratorHandle	m_accelerator ;
	HWND		m_hWnd ;
	MSG		msg;
	
	static const size_t DEFAULT_ITERATIONS = 100 ;
public:
	background_processor(size_t num_iterations=DEFAULT_ITERATIONS, 
						 HACCEL accel = NULL, 
						 HWND hwnd = GetActiveWindow() ) : 
		m_num_iterations ( num_iterations ), 
		m_iteration ( 0 ), 
		m_accelerator( accel ), 
		m_hWnd( hwnd )
	{
		ZeroMemory( &msg, sizeof( MSG ) ) ;
	}

	void set_accelerator( HACCEL accel )
	{
		m_accelerator = accel ;
	}
	void set_hwnd( HWND hwnd )
	{
		m_hWnd = hwnd ;
	}
	size_t get_num_iterations() const 
	{ 
		return m_iteration ; 
	}
	void set_iterations( const size_t num_iterations = DEFAULT_ITERATIONS ) 
	{ 
		m_num_iterations = num_iterations ; 
		refresh_iterations() ; 
	}
	void refresh_iterations() 
	{
		m_iteration = 0 ; 
	}
	bool check_iterations( )
	{
		if ( ++m_iteration % m_num_iterations == 0 )
		{
			perform_background_processing() ;
			return true ;
		}
		return false ;
	}
	void perform_background_processing()
	{
#ifdef UNIT_TEST
		return ;
#else
		// process any messages on the queue
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{
			if ( 
			#ifdef _DEBUG
				m_accelerator.IsNull() || 
				! ::IsWindow(m_hWnd) || 
			#endif
				! m_accelerator.TranslateAccelerator( m_hWnd, &msg ) )
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		} 
#endif
	}

};

