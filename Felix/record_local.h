#pragma once

#include "record.h"

namespace mem_engine
{
		/** class record_local
	* local version of translation record.
	*/
	class record_local : public translation_record
	{
	public:
		record_local() ;
		record_local( const record_pointer rec )
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
			return copy_from_self(cloned) ;
		}

		record_data_iterator data_begin() ;
		record_data_iterator data_end() ;

		bool item_exists( const wstring key ) const ;
		const wstring get_item( const wstring key ) const ;

		// CmpMaker
		void set_cmp_maker(const CmpMaker maker);

		// source
		const wstring get_source_rich() const;
		const wstring get_source_plain() ;
		const wstring get_source_cmp() const;

		// trans
		const wstring get_trans_rich() const;
		const wstring get_trans_plain() ;
		const wstring get_trans_cmp() const;

		// context
		const wstring get_context_rich() const;
		const wstring get_context_plain() ;
		const wstring get_context_cmp() const;

		// reliability
		size_t get_reliability() const ;
		void set_reliability( size_t reliability ) ;

		// source, trans, and context
		void set_source( const wstring source ) ;	
		void set_trans( const wstring trans ) ;
		void set_context( const wstring context );

		// validation
		bool is_validated() const  ;
		void set_validated_on() ;
		void set_validated_off() ;

		// created/modified
		void set_created( const wstring created );
		void set_modified( const wstring modified ) ;
		void set_created( const misc_wrappers::date &created );
		void set_modified( const misc_wrappers::date &modified ) ;

		const misc_wrappers::date get_created() const  ;
		const misc_wrappers::date get_modified() const  ;


		// ref_count
		void set_refcount( size_t count );
		size_t get_refcount() const ;

		// set date created
		bool create( ) ;
		// set the last-modifed date
		bool modify ( ) ;

		// id
		virtual size_t get_id() const;
		virtual void set_id(size_t id);

		/** Clear the record. 
		@return	bool success
		*/ 
		void clear_user_strings() ;
		void set_item( const wstring key, const wstring value ) ;

		bool is_valid_record() const;
		bool remove_item( const wstring key);

		// copy functions
		void internal_copy( const record_pointer rec ) ;
		record_pointer copy_from_self( record_pointer &rec ) ;

		virtual void set_creator(const wstring creator);
		virtual void set_modified_by(const wstring modified);
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