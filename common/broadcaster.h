#pragma once

#include <boost/signal.hpp>

class CBroadcaster
{
	struct dispatch_accumulator
	{
		typedef bool result_type ;

		template<typename InputIterator>
		result_type operator()(InputIterator first, InputIterator last) const
		{
			// If there are no slots to call, just return the
			// default value
			if (first == last)
			{
				return false ;
			}

			while (first != last) 
			{
				if ( *first )
				{
					return true ;
				}
				++first;
			}

			// not handled...
			return false ;
		}
	};
public:
	typedef boost::signal< bool ( boost::any& ), dispatch_accumulator > SignalType ;
	typedef SignalType::slot_type SlotType ;
	typedef boost::shared_ptr< SignalType > SignalPtrType ;
	typedef boost::signals::connection conn_type ;

private:
	typedef std::pair< std::string, std::string > KeyType ;
	typedef std::map< KeyType, SignalPtrType > VectorType ;
	VectorType m_SignalMap ;

	typedef boost::signals::connect_position conn_pos ;

public:
	static CBroadcaster& instance()
	{
		static CBroadcaster dispatcher ;
		return dispatcher ;
	}

	SignalPtrType Register( const std::string &Source, const std::string &Name )
	{
		const KeyType key( Source, Name ) ;

		EnsureExists(key);

		return m_SignalMap[key] ;
	}

	conn_type Subscribe( const std::string &Source, const std::string &Name, const SlotType &Slot, conn_pos pos = boost::signals::at_back )
	{
		const KeyType key( Source, Name ) ;

		EnsureExists(key);

		SignalPtrType sig = m_SignalMap[key] ;
		return sig->connect( Slot, pos ) ;
	}
	~CBroadcaster(void)
	{
	}
private:
	// best practices say be explicit to avoid memory leaks on exception
	void EnsureExists(const KeyType &key)
	{
		if ( m_SignalMap.find(key) == m_SignalMap.end() )
		{
			SignalPtrType theSignal( new SignalType() ) ;
			m_SignalMap[key] = theSignal ;
		}
	}

	CBroadcaster(void)
	{
	}
};