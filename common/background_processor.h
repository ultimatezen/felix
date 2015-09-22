
#pragma once


void pump_messages() ;

/**
	@class background_processor  
	@brief Poor man's multithreading.
 */
class background_processor  
{
	size_t	m_num_iterations ;
	size_t	m_iteration ;
	CAcceleratorHandle	m_accelerator ;
	HWND	m_hWnd ;
	MSG		msg;
	
	static const size_t DEFAULT_ITERATIONS = 100 ;
public:
	background_processor(size_t num_iterations=DEFAULT_ITERATIONS, 
						 HACCEL accel = NULL, 
						 HWND hwnd = GetActiveWindow() );

	void set_accelerator( HACCEL accel );
	void set_hwnd( HWND hwnd );
	size_t get_num_iterations() const;
	void set_iterations( const size_t num_iterations = DEFAULT_ITERATIONS );
	void refresh_iterations();
	bool check_iterations( );
	void perform_background_processing();

};

