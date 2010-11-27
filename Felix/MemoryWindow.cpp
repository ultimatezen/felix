// MemoryWindow.cpp : Implementation of CMemoryWindow

#include "stdafx.h"
#include "MemoryWindow.h"

using namespace except ;

STDMETHODIMP CMemoryWindow::Raise()
{
	try
	{
		m_window->raise();
	}
	FELIX_AUTO_CATCH( "Raise method of CMemoryWindow" ) ;

	return S_OK ;
}


STDMETHODIMP CMemoryWindow::get_Height(LONG* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		*pVal = m_window->get_height();
	}
	FELIX_AUTO_CATCH( "get_Height method of CMemoryWindow" ) ;

	return S_OK ;
}
STDMETHODIMP CMemoryWindow::put_Height(LONG pVal)
{
	try
	{
		m_window->set_height(pVal) ;
	}
	FELIX_AUTO_CATCH( "put_Height method of CMemoryWindow" ) ;

	return S_OK ;
}



STDMETHODIMP CMemoryWindow::get_Width(LONG* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		*pVal = m_window->get_width();
	}
	FELIX_AUTO_CATCH( "get_Width method of CMemoryWindow" ) ;

	return S_OK ;
}
STDMETHODIMP CMemoryWindow::put_Width(LONG pVal)
{
	try
	{
		m_window->set_width(pVal) ;
	}
	FELIX_AUTO_CATCH( "put_Width method of CMemoryWindow" ) ;

	return S_OK ;
}


STDMETHODIMP CMemoryWindow::get_Left(LONG* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		*pVal = m_window->get_left();
	}
	FELIX_AUTO_CATCH( "get_Left method of CMemoryWindow" ) ;

	return S_OK ;
}
STDMETHODIMP CMemoryWindow::put_Left(LONG pVal)
{
	try
	{
		m_window->set_left(pVal) ;
	}
	FELIX_AUTO_CATCH( "put_Left method of CMemoryWindow" ) ;

	return S_OK ;
}


STDMETHODIMP CMemoryWindow::get_Top(LONG* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		*pVal = m_window->get_top();
	}
	FELIX_AUTO_CATCH( "get_Top method of CMemoryWindow" ) ;

	return S_OK ;
}
STDMETHODIMP CMemoryWindow::put_Top(LONG pVal)
{
	try
	{
		m_window->set_top(pVal) ;
	}
	FELIX_AUTO_CATCH( "put_Top method of CMemoryWindow" ) ;

	return S_OK ;
}


// CMemoryWindow

