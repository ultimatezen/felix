#pragma once

class __declspec(novtable) CNavInterface
{
public:

	CNavInterface(void)
	{
	}

	virtual ~CNavInterface(void)
	{
	}

	//virtual void OnNavRegGlossSetSource( )
	//{
	//}
	//virtual void OnNavRegGlossSetTrans ( )
	//{
	//}
	//virtual void OnNavRegGlossBack( )
	//{
	//}
	//virtual void OnNavRegGlossAdd( )
	//{
	//}
	virtual void OnNavShowAbout( )
	{
	}

	virtual void OnNavEdit( long index )
	{
		index ;
	}
	virtual void OnNavDelete( long index )
	{
		index ;
	}
	virtual void OnNavAddGloss( long index )
	{
		index ;
	}
	virtual void OnNavAddToGloss( long index )
	{
		index ;
	}

};

#ifdef _DEBUG

#include <list>

/**
 * @class CNavInterfaceDummy
 * @brief mock object for unit testing.
 */
class CNavInterfaceDummy : public CNavInterface
{
	typedef std::list< CString > str_list ;
public:
	str_list m_call_history ;

	void OnNavRegGlossSetSource( ) 
	{
		m_call_history.push_front( _T("OnNavRegGlossSetSource") ) ;
	}
	void OnNavRegGlossSetTrans ( ) 
	{
		m_call_history.push_front( _T("OnNavRegGlossSetTrans") ) ;
	}
	void OnNavRegGlossBack( )  
	{
		m_call_history.push_front( _T("OnNavRegGlossBack") ) ;
	}
	void OnNavRegGlossAdd( )  
	{
		m_call_history.push_front( _T("OnNavRegGlossAdd") ) ;
	}
	void OnNavShowAbout( )  
	{
		m_call_history.push_front( _T("OnNavShowAbout") ) ;
	}
	void OnNavEdit( long index )
	{
		index ;
		m_call_history.push_front( _T("OnNavEdit") ) ;
	}
	void OnNavDelete( long index )
	{
		index ;
		m_call_history.push_front( _T("OnNavDelete") ) ;
	}
	void OnNavAddGloss( long index )
	{
		index ;
		m_call_history.push_front( _T("OnNavAddGloss") ) ;
	}
	void OnNavAddToGloss( long index )
	{
		index ;
		m_call_history.push_front( _T("OnNavAddToGloss") ) ;
	}

protected:
private:
};

#endif // #ifdef _DEBUG
