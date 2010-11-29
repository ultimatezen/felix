#pragma once

#include "loki/AssocVector.h"
#include <string>
#include "boost/signals.hpp"
#include "boost/any.hpp"
#include "boost/function.hpp"
#include <boost/shared_ptr.hpp>
#include "boost/signals/connection.hpp"

class CBroker
{
	struct broker_accumulator
	{
		typedef bool result_type ;

		template<typename InputIterator>
		result_type operator()(InputIterator first, InputIterator last) const
		{
			// If there are no slots to call, just return the
			// default value
			if (first == last)
				return false ;

			while (first != last) 
			{
				if ( *first ) // handled
					return true ;
				++first;
			}

			// not handled...
			return false ;
		}
	};
public:
	typedef boost::any & DataType ;
	typedef boost::signal< bool ( DataType ), broker_accumulator > SignalType ;
	typedef boost::signals::connection conn_type ;
	typedef SignalType::slot_type SlotType ;
	typedef boost::shared_ptr< SignalType > SignalPtrType ;
	typedef std::string KeyType ;
	typedef boost::signals::connect_position PosType ;

private:
	typedef Loki::AssocVector< KeyType, SignalPtrType > VectorType ;
	VectorType m_SignalMap ;

public:
	static CBroker& instance()
	{
		static CBroker broker ;
		return broker ;
	}

	conn_type Register( const KeyType &DataName, const SlotType &theSlot, PosType pos = boost::signals::at_back )
	{
		EnsureExists(DataName);

		SignalPtrType theSig = m_SignalMap[DataName] ;
		return theSig->connect( theSlot, pos ) ;
	}

	bool Request( const KeyType &DataName, DataType RequestedData )
	{
		SignalPtrType theSig = RequestHandle( DataName ) ;
		return (*theSig)(RequestedData) ;
	}

	// this is for more permanent (efficient) access to info source
	SignalPtrType RequestHandle( const KeyType &DataName )
	{
		EnsureExists(DataName);

		return m_SignalMap[DataName] ;
	}
	~CBroker(void)
	{
	}
private:
	// best practices say be explicit to avoid memory leaks on exception
	void EnsureExists( const KeyType &DataName )
	{
		if ( m_SignalMap.find(DataName) == m_SignalMap.end() )
		{
			SignalPtrType theSignal( new SignalType() ) ;
			m_SignalMap[DataName] = theSignal ;
		}
	}

	CBroker(void)
	{
	}
};
