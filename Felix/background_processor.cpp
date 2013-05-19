#include "stdafx.h"
#include "background_processor.h"
#include "DebugUtilities.h"
#include "atlapp.h"
#include "atluser.h"

void pump_messages()
{
	MSG msg = {0};
	while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
	{ 
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	} 
}


background_processor::background_processor( size_t num_iterations/*=DEFAULT_ITERATIONS*/, HACCEL accel /*= NULL*/, HWND hwnd /*= GetActiveWindow() */ ) : 
m_num_iterations ( num_iterations ), 
	m_iteration ( 0 ), 
	m_accelerator( accel ), 
	m_hWnd( hwnd )
{
	ZeroMemory( &msg, sizeof( MSG ) ) ;
}

void background_processor::set_accelerator( HACCEL accel )
{
	m_accelerator = accel ;
}

void background_processor::set_hwnd( HWND hwnd )
{
	m_hWnd = hwnd ;
}

size_t background_processor::get_num_iterations() const
{
	return m_iteration ;
}

void background_processor::set_iterations( const size_t num_iterations /*= DEFAULT_ITERATIONS */ )
{
	m_num_iterations = num_iterations ; 
	refresh_iterations() ;
}

void background_processor::refresh_iterations()
{
	m_iteration = 0 ;
}

bool background_processor::check_iterations()
{
	if ( ++m_iteration % m_num_iterations == 0 )
	{
		perform_background_processing() ;
		return true ;
	}
	return false ;
}

void background_processor::perform_background_processing()
{
#ifdef UNIT_TEST
	return ;
#else
	if (m_accelerator.IsNull() || ! ::IsWindow(m_hWnd))
	{
		pump_messages() ;
	}

	// process any messages on the queue
	while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
	{
		if (! m_accelerator.TranslateAccelerator(m_hWnd, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	} 
#endif
}