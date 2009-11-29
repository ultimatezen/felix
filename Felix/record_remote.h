#pragma once
#include "record.h"
#include "DispatchWrapper.h"
#include "logging.h"

namespace memory_engine
{
	class record_remote : public translation_record
	{
	public:

		// construction
		record_remote(LPCWSTR server_name = L"Felix.RemoteRecord");
		record_remote( record_pointer rec );
		record_remote(CDispatchWrapper wrapper);

		// We really need to plug this leak, but for right now I can't find it!
		~record_remote();


		void copy_engine_values();
		// source

		const wstring & get_source_rich() const;

		const wstring get_source_plain();

		const wstring & get_source_cmp() const;

		// trans

		const wstring & get_trans_rich() const;

		const wstring get_trans_plain();

		const wstring & get_trans_cmp() const;

		// context 

		const wstring &get_context_rich() const;
		const wstring get_context_plain();
		const wstring &get_context_cmp() const;
		// reliability
		size_t get_reliability() const;
		void set_reliability( const size_t reliability );

		void set_item( const wstring &key, const wstring &value );

		// source, trans, and context
		void set_source( const wstring source );
		void set_trans( const wstring trans );
		void set_context( const wstring context );

		// validation
		bool is_validated() const;
		void set_validated_on();
		void set_validated_off();

		// modified/created
		const misc_wrappers::date &get_created() const;
		const misc_wrappers::date &get_modified() const;
		void set_created( const wstring &created );
		void set_modified( const wstring &modified );
		void set_created( const misc_wrappers::date &created );
		void set_modified( const misc_wrappers::date &modified );

		// set date created
		bool create( );
		// set the last-modifed date
		bool modify ( );

		// ref_count
		void set_refcount( size_t count );
		size_t get_refcount() const;

		bool is_valid_record();

		record_data_iterator data_begin();
		record_data_iterator data_end();
		record_data_map& get_data_map();

		bool item_exists( const wstring &key ) const;
		const wstring get_item( const wstring &key ) const;
		bool remove_item( const wstring &key);
		void clear_user_strings();

		// id
		virtual size_t get_id();
		virtual void set_id(size_t id);

		// CmpMaker
		void set_cmp_maker( const CmpMaker maker );

		record_pointer clone();

		virtual void set_creator(const wstring &creator);
		virtual void set_modified_by(const wstring &modified);
		virtual wstring get_creator();
		virtual wstring get_modified_by();

		// void record_local::copy_from_self( const record_local &rec )
		// commonizes copy code used for constructor and assignment
		record_pointer copy_from_self( record_pointer rec );
		void internal_copy( record_pointer rec );
		record_data_map		m_record_data ;
		// dates
		misc_wrappers::date			m_created ;
		misc_wrappers::date			m_modified ;

		Segment m_source ;
		Segment m_trans ;
		Segment m_context ;

		wstring m_creator ;
		wstring m_modified_by ;

		// Is the record validated?
		bool m_is_validated;
		// The reliability of the translation
		size_t m_reliability;
		// How many times the record has been referenced
		size_t m_refcount;
		size_t m_id ;

		CDispatchWrapper m_engine ;

		CComVariant get_engine();
	} ;


}