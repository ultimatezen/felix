#pragma once

#include "StringEx.h"			// useful string stuff
#include "MiscWrappers.h"		// CDate
#include "StringConversions.h"	// string2long et al
#include "tag_stripper.h"
#include "segment.h"

/** 
	Translation memory "business logic."
*/
namespace mem_engine
{
	static const size_t MIN_RELIABILITY = 0 ;
	static const size_t MAX_RELIABILITY = 9 ;

	void set_record_username(const wstring username) ;
	wstring get_record_username() ;
	
	/**
	translation_record
	Represents a memory record.

	Key data is source and translation.
	*/
	class __declspec(novtable) translation_record
	{
	public:

		typedef std::map< wstring, wstring >	record_data_map ;
		typedef record_data_map::iterator		record_data_iterator ;

		virtual ~translation_record(){}

		// pure virtual

		virtual void set_validated(bool setting) = 0 ;
		virtual void increment_reliability() = 0 ;

		// source

		//! Gets the source string as rich HTML text
		virtual const wstring get_source_rich() const = 0;
		//! Gets the source string as plain text
		virtual const wstring get_source_plain() = 0;
		//! Gets the normalized source string used for comparisons
		//! A normalized source string might be lower or have
		//! wide characters converted to narrow equivalents
		virtual const wstring get_source_cmp() const = 0;

		// trans
		//! Gets the translation string as rich HTML text
		virtual const wstring get_trans_rich() const = 0;
		//! Gets the translation string as plain text
		virtual const wstring get_trans_plain() = 0;
		//! Gets the normalized translation string used for comparisons
		//! A normalized translation string might be lower or have
		//! wide characters converted to narrow equivalents
		virtual const wstring get_trans_cmp() const = 0;

		// context
		//! Gets the context string as rich HTML text
		virtual const wstring get_context_rich() const = 0;
		//! Gets the context string as plain text
		virtual const wstring get_context_plain() = 0;
		//! Gets the normalized context string used for comparisons
		//! A normalized context string might be lower or have
		//! wide characters converted to narrow equivalents
		virtual const wstring get_context_cmp() const = 0;

		// reliability
		//! Gets the reliability score
		virtual size_t get_reliability() const  = 0;
		//! Sets the reliability score
		virtual void set_reliability( size_t reliability )  = 0;

		virtual void set_item( const wstring key, const wstring value )  = 0;

		// source, trans, and context
		//! Sets the rich and cmp values, and marks the record as modified
		virtual void set_source( const wstring source )  = 0;	
		//! Sets the rich and cmp values, and marks the record as modified
		virtual void set_trans(const wstring trans) = 0;
		//! Sets the rich and cmp values, and marks the record as modified
		virtual void set_context(const wstring context) = 0;

		// validation
		virtual bool is_validated() const  = 0;
		virtual void set_validated_on()  = 0;
		virtual void set_validated_off()  = 0;

		// modified/created
		virtual const misc_wrappers::date get_created() const  = 0;
		virtual const misc_wrappers::date get_modified() const  = 0;
		virtual void set_created( const wstring created ) = 0;
		virtual void set_modified( const wstring modified ) = 0;
		virtual void set_created( const misc_wrappers::date &created ) = 0;
		virtual void set_modified( const misc_wrappers::date &modified ) = 0;

		//! Sets the name of the record creator.
		//! Default is logged-on username, but the user can
		//! set this to any value.
		virtual void set_creator(const wstring creator) = 0 ;
		//! Sets the name of the last user to modify the record.
		//! Default is logged-on username, but the user can
		//! set this to any value.
		virtual void set_modified_by(const wstring modified) = 0;
		//! Gets the creator name
		virtual wstring get_creator() = 0 ;
		//! Gets the last-modified user name
		virtual wstring get_modified_by() = 0 ;

		//! set date created
		virtual bool create( ) = 0;
		//! set the last-modifed date
		virtual bool modify ( ) = 0;

		//////////////////////////////////////////////////////////////////////////
		// ref_count
		//////////////////////////////////////////////////////////////////////////

		/**
		* Increments the record's reference count.
		* This is not a smart-pointer reference count, but a count
		* of the number of times the record's translation has been used, or
		* "referenced." This is an indicator of popularity. The more a
		* translation is used, the more likely the translator is going to
		* want to use it in the next translation.
		*/
		virtual void increment_refcount() = 0;

		/**
		* Resets the reference count to 0.
		* If the reference count was not already zero, updates the
		* record's modified date.
		*/
		virtual void reset_refcount() = 0;
		//! Sets the reference count
		virtual void set_refcount(size_t count) = 0;
		//! Retrieves the reference count
		virtual size_t get_refcount() const  = 0;

		//! Returns whether the record is valid to use as a translation pair.
		virtual bool is_valid_record() const = 0;

		//////////////////////////////////////////////////////////////////////////
		// Handlers for extra record data
		//////////////////////////////////////////////////////////////////////////

		//! Retrieves the map of extra key/value string pairs
		//! The user can specify any extra string pairs they want.
		//! This is also used to preserve external format data that
		//! is not used internally, when importing/exporting TMX or
		//! Trados formats.
		virtual record_data_map& get_data_map() = 0;
		//! Data map iterator
		virtual record_data_iterator data_begin() = 0;
		//! Data map iterator
		virtual record_data_iterator data_end() = 0;

		//! Whether the key exists in the data map
		virtual bool item_exists( const wstring key ) const = 0;
		//! Retrieves value corresponding to `key` in data map
		virtual const wstring get_item( const wstring key ) const = 0;
		//! Deletes value corresponding to `key` in data map
		virtual bool remove_item(const wstring key) = 0;
		//! Deletes all entries in data map
		virtual void clear_user_strings() = 0;

		//! Sets the class used to make cmp strings
		virtual void set_cmp_maker(CmpMaker *maker) = 0;

		//! Clones the translation record
		virtual std::shared_ptr< translation_record > clone() = 0;

		//! Copies `rec` values into self
		virtual void internal_copy( const std::shared_ptr< translation_record > rec ) = 0;
		//! Copies internal values into `rec`
		virtual std::shared_ptr< translation_record > copy_from_self( std::shared_ptr< translation_record > &rec ) = 0;

		//! Retrieves the record's ID
		//! The ID is unique to each record within a TM.
		//! Not guaranteed to be unique across TMs -- two TMs might each have a record
		//! with the ID '17'.
		virtual size_t get_id() const = 0;
		//! Sets the record's unique ID
		//! No checking is performed to ensure that the ID is unique. Caller must
		//! ensure this themselves.
		virtual void set_id(size_t id) = 0; 
	} ;

	/** ref-counted pointer to translation record.
	*/
	typedef std::shared_ptr< translation_record > record_pointer ;

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

}