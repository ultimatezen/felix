#pragma once

#pragma warning( disable:4996 ) // #pragma deprecated

#include "StringEx.h"			// useful string stuff
#include <map>					// map
#include "MiscWrappers.h"		// CDate
#include "StringConversions.h"	// string2long et al
#include "tag_stripper.h"
#include "RefPointer.h"			// ref-counted pointer
#include "segment.h"

namespace memory_engine
{
	void set_record_username(const wstring &username) ;
	wstring get_record_username() ;
	/**
	@class translation_record
	@brief Represents a memory record.
	*/
	class translation_record
	{
	public:
		typedef std::map< wstring, wstring >	record_data_map ;
		typedef record_data_map::iterator		record_data_iterator ;


		// construction
		translation_record( ) {}
		virtual ~translation_record() {}

		void set_validated(bool setting)
		{
			if(setting)
			{
				this->set_validated_on() ;
			}
			else
			{
				this->set_validated_off() ;
			}
		}

		// pure virtual

		// source
		virtual const wstring &get_source_rich() const = 0;
		virtual const wstring get_source_plain() = 0;
		virtual const wstring &get_source_cmp() const = 0;

		// trans
		virtual const wstring &get_trans_rich() const = 0;
		virtual const wstring get_trans_plain() = 0;
		virtual const wstring &get_trans_cmp() const = 0;

		// context
		virtual const wstring &get_context_rich() const = 0;
		virtual const wstring get_context_plain() = 0;
		virtual const wstring &get_context_cmp() const = 0;

		// reliability
		virtual size_t get_reliability() const  = 0;
		virtual void set_reliability( const size_t reliability )  = 0;
		void increment_reliability()
		{
			size_t reliability = this->get_reliability() ;
			this->set_reliability( reliability + 1u ) ;
		}

		virtual void set_item( const wstring &key, const wstring &value )  = 0;

		// source, trans, and context
		virtual void set_source( const wstring source )  = 0;	
		virtual void set_trans( const wstring trans ) = 0;
		virtual void set_context( const wstring context ) = 0;

		// validation
		virtual bool is_validated() const  = 0;
		virtual void set_validated_on()  = 0;
		virtual void set_validated_off()  = 0;

		// modified/created
		virtual const misc_wrappers::date &get_created() const  = 0;
		virtual const misc_wrappers::date &get_modified() const  = 0;
		virtual void set_created( const wstring &created ) = 0;
		virtual void set_modified( const wstring &modified ) = 0;
		virtual void set_created( const misc_wrappers::date &created ) = 0;
		virtual void set_modified( const misc_wrappers::date &modified ) = 0;

		virtual void set_creator(const wstring &creator) = 0 ;
		virtual void set_modified_by(const wstring &modified) = 0 ;
		virtual wstring get_creator() = 0 ;
		virtual wstring get_modified_by() = 0 ;

		// set date created
		virtual bool create( ) = 0;
		// set the last-modifed date
		virtual bool modify ( ) = 0;

		// ref_count
		void increment_refcount()
		{
			this->set_refcount(this->get_refcount()+1) ;
		}
		virtual void set_refcount( size_t count ) = 0;
		virtual size_t get_refcount() const  = 0;
		void reset_refcount()
		{
			if ( 0u < this->get_refcount() ) 
			{
				this->modify() ;
				this->set_refcount(0u) ;
			}
		}

		virtual bool is_valid_record() = 0;

		virtual record_data_iterator data_begin() = 0;
		virtual record_data_iterator data_end() = 0;

		virtual bool item_exists( const wstring &key ) const = 0;
		virtual const wstring get_item( const wstring &key ) const = 0;
		virtual bool remove_item( const wstring &key) = 0;
		virtual void clear_user_strings() = 0;

		// CmpMaker
		virtual void set_cmp_maker(const CmpMaker maker) = 0;

		virtual boost::shared_ptr< translation_record > clone() = 0;
		virtual record_data_map& get_data_map() = 0 ;

		virtual void internal_copy( boost::shared_ptr< translation_record > rec ) = 0;
		virtual boost::shared_ptr< translation_record > copy_from_self( boost::shared_ptr< translation_record > rec ) = 0;

		virtual size_t get_id() = 0;
		virtual void set_id(size_t id) = 0; 
	} ;

	/** ref-counted pointer to translation record.
	*/
	typedef boost::shared_ptr< translation_record > record_pointer ;

	// bool operator < 
	// compares two translation_records -- for use in stl containers etc.
	inline bool operator < ( const record_pointer lhs, const record_pointer rhs )
	{
		if ( lhs->get_source_rich() < rhs->get_source_rich() ) 
			return true ;
		if ( lhs->get_source_rich() > rhs->get_source_rich() ) 
			return false ;
		return ( lhs->get_trans_rich() < rhs->get_trans_rich( ) ) ;
	}

	// bool operator == 
	// compares two translation_records -- for use in stl containers etc.
	inline bool operator == ( const record_pointer lhs, const record_pointer rhs )
	{
		if ( lhs->get_source_rich() != rhs->get_source_rich() ) 
			return false ;
		return ( lhs->get_trans_rich() == rhs->get_trans_rich() ) ;
	}

	// bool operator != 
	// compares two translation_records -- for use in stl containers etc.
	inline bool operator != ( const record_pointer lhs, const record_pointer rhs )
	{
		return ! ( lhs == rhs ) ;
	}


	/** class record_local
	* local version of translation record.
	*/
	class record_local : public translation_record
	{
	public:
		record_local() ;
		record_local( record_pointer rec )
		{ 
			internal_copy( rec ) ; 
		}

		// assignment
		record_local &operator =( record_pointer rec ) 
		{
			internal_copy( rec ) ;
			return *this ;
		}

		record_pointer clone()
		{
			record_pointer cloned(new record_local()) ;
			copy_from_self(cloned) ;
			return cloned ;
		}

		record_data_iterator data_begin() ;
		record_data_iterator data_end() ;

		bool item_exists( const wstring &key ) const ;
		const wstring get_item( const wstring &key ) const ;

		// CmpMaker
		void set_cmp_maker(const CmpMaker maker);

		// source
		const wstring &get_source_rich() const;
		const wstring get_source_plain() ;
		const wstring &get_source_cmp() const;

		// trans
		const wstring &get_trans_rich() const;
		const wstring get_trans_plain() ;
		const wstring &get_trans_cmp() const;

		// context
		const wstring &get_context_rich() const;
		const wstring get_context_plain() ;
		const wstring &get_context_cmp() const;

		// reliability
		size_t get_reliability() const ;
		void set_reliability( const size_t reliability ) ;

		// source, trans, and context
		void set_source( const wstring source ) ;	
		void set_trans( const wstring trans ) ;
		void set_context( const wstring context );

		// validation
		bool is_validated() const  ;
		void set_validated_on() ;
		void set_validated_off() ;

		// created/modified
		void set_created( const wstring &created );
		void set_modified( const wstring &modified ) ;
		void set_created( const misc_wrappers::date &created );
		void set_modified( const misc_wrappers::date &modified ) ;

		const misc_wrappers::date &get_created() const  ;
		const misc_wrappers::date &get_modified() const  ;


		// ref_count
		void set_refcount( size_t count );
		size_t get_refcount() const ;

		// set date created
		bool create( ) ;
		// set the last-modifed date
		bool modify ( ) ;

		// id
		virtual size_t get_id()
		{
			return m_id;
		}
		virtual void set_id(size_t id)
		{
			m_id = id ;
		}

		/** Clear the record. 
		@return	bool success
		*/ 
		void clear_user_strings() ;
		void set_item( const wstring &key, const wstring &value ) ;

		bool is_valid_record();
		bool remove_item( const wstring &key);

		// copy functions
		void internal_copy( record_pointer rec ) ;
		record_pointer copy_from_self( record_pointer rec ) ;

		virtual void set_creator(const wstring &creator);
		virtual void set_modified_by(const wstring &modified);
		virtual wstring get_creator();
		virtual wstring get_modified_by();

		record_data_map& get_data_map()
		{
			return this->m_record_data ;
		}
		Segment m_source ;
		Segment m_trans ;
		Segment m_context ;

		wstring m_creator ;
		wstring m_modified_by ;

		// other data that may or may not be present
		record_data_map		m_record_data ;
		// Is the record validated?
		bool m_is_validated;
		// The reliability of the translation
		size_t m_reliability;
		// How many times the record has been referenced
		size_t m_refcount;

		// dates
		misc_wrappers::date			m_created ;
		misc_wrappers::date			m_modified ;

		size_t m_id ;
	};


}