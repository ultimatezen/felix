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
