#pragma once

#include "StringEx.h"			// useful string stuff
#include <map>					// map
#include "MiscWrappers.h"		// CDate
#include "StringConversions.h"	// string2long et al
#include "tag_stripper.h"
#include "segment.h"

namespace mem_engine
{
	static const size_t MIN_RELIABILITY = 0 ;
	static const size_t MAX_RELIABILITY = 9 ;

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

		void set_validated(bool setting);
		void increment_reliability();
		void increment_refcount();
		void reset_refcount();

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
		virtual void set_reliability( size_t reliability )  = 0;

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
		virtual const misc_wrappers::date get_created() const  = 0;
		virtual const misc_wrappers::date get_modified() const  = 0;
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
		virtual void set_refcount( size_t count ) = 0;
		virtual size_t get_refcount() const  = 0;

		virtual bool is_valid_record() const = 0;

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

		virtual size_t get_id() const = 0;
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



}