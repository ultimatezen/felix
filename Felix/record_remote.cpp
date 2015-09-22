#include "StdAfx.h"
#include "record_remote.h"
#include "Felix_properties.h"

// implementation here
namespace mem_engine
{
	// CTOR/DTOR
	record_remote::record_remote(LPCWSTR server_name) : m_engine(server_name)
	{
	}

	record_remote::record_remote( const record_pointer rec ) : 
	m_engine(L"Felix.RemoteRecord")
	{
		internal_copy( rec ) ;
	}

	record_remote::record_remote( CDispatchWrapper wrapper ) : 
	m_engine(wrapper)
	{
	}

	record_remote::~record_remote()
	{
	}

	// Internal method (non-polymorphic)
	ATL::CComVariant record_remote::get_engine()
	{
		return CComVariant(m_engine.get_dispatch());
	}

	// source
	const wstring  record_remote::get_source_rich() const
	{
		const_cast<record_remote*>(this)->ensure_source() ;
		return m_source.rich() ;
	}
	const wstring record_remote::get_source_plain()
	{
		const_cast<record_remote*>(this)->ensure_source() ;
		return m_source.plain() ;
	}

	const wstring  record_remote::get_source_cmp() const
	{
		const_cast<record_remote*>(this)->ensure_source() ;
		return m_source.cmp() ;
	}

	// trans
	const wstring  record_remote::get_trans_rich() const
	{
		const_cast<record_remote*>(this)->ensure_trans() ;
		return m_trans.rich() ;
	}

	const wstring record_remote::get_trans_plain()
	{
		const_cast<record_remote*>(this)->ensure_trans() ;
		return m_trans.plain() ;
	}

	const wstring  record_remote::get_trans_cmp() const
	{
		const_cast<record_remote*>(this)->ensure_trans() ;
		return m_trans.cmp() ;
	}

	// context
	const wstring  record_remote::get_context_rich() const
	{
		const_cast<record_remote*>(this)->ensure_context() ;
		return m_context.rich() ;
	}

	const wstring record_remote::get_context_plain()
	{
		const_cast<record_remote*>(this)->ensure_context() ;
		return m_context.plain() ;
	}

	const wstring  record_remote::get_context_cmp() const
	{
		const_cast<record_remote*>(this)->ensure_context() ;
		return m_context.cmp() ;
	}

	size_t record_remote::get_reliability() const
	{
		return static_cast<size_t>(const_cast<record_remote*>(this)->m_engine.prop_get(L"Reliability").lVal) ;
	}

	void record_remote::set_reliability( const size_t reliability )
	{
		m_engine.prop_put(L"Reliability", reliability) ;
	}

	void record_remote::set_source( const wstring source )
	{
		m_engine.prop_put(L"Source", source.c_str()); 
		m_source.set_value(source) ;
		this->set_modified_by(get_record_username()) ;
	}

	void record_remote::set_trans( const wstring trans )
	{
		m_engine.prop_put(L"Trans", trans.c_str()); 
		m_trans.set_value(trans) ;
		this->set_modified_by(get_record_username()) ;
	}

	void record_remote::set_context( const wstring context )
	{
		m_engine.prop_put(L"Context", context.c_str()); 
		m_context.set_value(context) ;
		this->set_modified_by(get_record_username()) ;
	}

	bool record_remote::is_validated() const
	{
		return const_cast<record_remote*>(this)->m_engine.prop_get(L"Validated").boolVal != VARIANT_FALSE ;
	}

	void record_remote::set_validated_on()
	{
		m_engine.prop_put(L"Validated", VARIANT_TRUE) ;
	}

	void record_remote::set_validated_off()
	{
		m_engine.prop_put(L"Validated", VARIANT_FALSE) ;
	}

	const misc_wrappers::date record_remote::get_created() const
	{
		return misc_wrappers::date(BSTR2wstring(const_cast<record_remote*>(this)->m_engine.prop_get(L"Created").bstrVal)) ;
	}

	const misc_wrappers::date record_remote::get_modified() const
	{
		return misc_wrappers::date(BSTR2wstring(const_cast<record_remote*>(this)->m_engine.prop_get(L"Modified").bstrVal)) ;
	}

	void record_remote::set_created( const wstring created )
	{
		const misc_wrappers::date created_date(created) ;
		this->set_created(created_date) ;
	}

	void record_remote::set_created( const misc_wrappers::date &created )
	{
		misc_wrappers::date created_date = created ;
		m_engine.prop_put(L"Created", created_date.get_date_time_string().c_str()) ;
	}
	void record_remote::set_modified( const wstring modified )
	{
		const misc_wrappers::date mod_date(modified) ;
		this->set_modified(mod_date) ;
	}

	void record_remote::set_modified(const misc_wrappers::date &modified)
	{
		m_engine.prop_put(L"Modified", modified.get_date_time_string().c_str()) ;
		if ( modified < this->get_created() )
		{
			set_created(modified) ;
		}
	}

	// create
	bool record_remote::create( )
	{
		misc_wrappers::date created_time ;
		created_time.set_to_local_time() ;
		m_engine.prop_put(L"Created", created_time.get_date_time_string().c_str()) ;

		return modify() ;
	}

	// modify
	// set the last-modified value to the current date-time
	bool record_remote::modify()
	{
		this->set_modified_by(get_record_username()) ;
		misc_wrappers::date modified_time ;
		modified_time.set_to_local_time() ;
		m_engine.prop_put(L"Modified", modified_time.get_date_time_string().c_str()) ;
		return true ;
	}


	void record_remote::set_refcount( size_t count )
	{
		m_engine.prop_put(L"RefCount", count) ;
	}

	size_t record_remote::get_refcount() const
	{
		return static_cast<size_t>(const_cast<record_remote*>(this)->m_engine.prop_get(L"RefCount").lVal) ;
	}

	bool record_remote::is_valid_record() const
	{
		return true ;
	}


	void record_remote::set_cmp_maker( CmpMaker *maker )
	{
		m_source.set_cmp_maker(maker) ;
		m_trans.set_cmp_maker(maker) ;
		m_context.set_cmp_maker(maker) ;
	}

	record_pointer record_remote::clone()
	{
		record_pointer cloned(new record_remote()) ;
		return this->copy_from_self(cloned) ;
	}

	size_t record_remote::get_id() const
	{
		return static_cast< size_t >( const_cast<record_remote*>(this)->m_engine.prop_get(L"Id").lVal ) ;
	}

	void record_remote::set_id( const size_t id )
	{
		m_engine.prop_put(L"Id", id) ;
	}

	void record_remote::set_creator( const wstring creator )
	{
		m_engine.prop_put(L"CreatedBy", creator.c_str()) ;
	}

	void record_remote::set_modified_by( const wstring modified )
	{
		m_engine.prop_put(L"ModifiedBy", modified.c_str()) ;
	}

	wstring record_remote::get_creator()
	{
		return BSTR2wstring(m_engine.prop_get(L"CreatedBy").bstrVal) ;
	}

	wstring record_remote::get_modified_by()
	{
		return BSTR2wstring(m_engine.prop_get(L"ModifiedBy").bstrVal) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Unsupported methods
	//////////////////////////////////////////////////////////////////////////

	void record_remote::set_item( const wstring key, const wstring value )
	{
		key ;
		value ;
		logging::log_warn("set_item method is not supported by remote records") ;
	}

	translation_record::record_data_iterator record_remote::data_begin()
	{
		logging::log_warn("data_begin method is not supported by remote records") ;
		return m_record_data.begin() ;
	}

	translation_record::record_data_iterator record_remote::data_end()
	{
		logging::log_warn("data_end method is not supported by remote records") ;
		return m_record_data.end() ;
	}

	translation_record::record_data_map& record_remote::get_data_map()
	{
		logging::log_warn("get_data_map method is not supported by remote records") ;
		return this->m_record_data ;
	}

	bool record_remote::item_exists( const wstring key ) const
	{
		key ;
		logging::log_warn("item_exists method is not supported by remote records") ;
		return false ;
	}

	const wstring record_remote::get_item( const wstring key ) const
	{
		key ;
		logging::log_warn("get_item method is not supported by remote records") ;
		return wstring() ;
	}

	bool record_remote::remove_item( const wstring key )
	{
		key ;
		logging::log_warn("remove_item method is not supported by remote records") ;
		return false ;
	}

	void record_remote::clear_user_strings()
	{
		logging::log_warn("clear_user_strings method is not supported by remote records") ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Internal methods
	//////////////////////////////////////////////////////////////////////////

	record_pointer record_remote::copy_from_self( record_pointer &rec )
	{
		rec->set_source( this->get_source_rich() ) ;
		rec->set_trans( this->get_trans_rich() ) ;
		rec->set_context( this->get_context_rich() ) ;			

		// dates
		rec->set_created(this->get_created()) ;
		rec->set_modified(this->get_modified()) ;

		// other info
		rec->set_validated(this->is_validated()) ;
		rec->set_reliability(this->get_reliability()) ;
		rec->set_refcount(this->get_refcount()) ;

		rec->set_creator(this->get_creator()) ;
		rec->set_modified_by(this->get_modified_by()) ;
		return rec ;
	}

	void record_remote::internal_copy( const record_pointer rec )
	{
		set_source( rec->get_source_rich() ) ;
		set_trans( rec->get_trans_rich() ) ;
		set_context( rec->get_context_rich() ) ;			

		// dates
		set_created(rec->get_created()) ;
		set_modified(rec->get_modified()) ;

		// other info
		set_validated(rec->is_validated()) ;
		set_reliability(rec->get_reliability()) ;
		set_refcount(rec->get_refcount()) ;

		set_creator(rec->get_creator()) ;
		set_modified_by(rec->get_modified_by()) ;

		set_id(rec->get_id()) ;
	}
	// Copy from the COM server
	void record_remote::ensure_source()
	{
		if (m_source.rich().empty())
		{
			this->m_source.set_value(BSTR2wstring(m_engine.prop_get(L"Source").bstrVal)) ;
		}
	}

	void record_remote::ensure_trans()
	{
		if (m_trans.rich().empty())
		{
			this->m_trans.set_value(BSTR2wstring(m_engine.prop_get(L"Trans").bstrVal)) ;
		}
	}

	void record_remote::ensure_context()
	{
		if (m_context.rich().empty())
		{
			this->m_context.set_value(BSTR2wstring(m_engine.prop_get(L"Context").bstrVal)) ;
		}
	}


	// set_validated
	void record_remote::set_validated( const bool setting )
	{
		setting ? this->set_validated_on() : this->set_validated_off() ;
	}

	// increment_reliability
	void record_remote::increment_reliability()
	{
		this->set_reliability( this->get_reliability() + 1u ) ;
	}

	// increment_refcount
	void record_remote::increment_refcount()
	{
		this->set_refcount(this->get_refcount() + 1u) ;
	}

	// reset_refcount
	// If we actually change the refcount, then we update
	// the last-modified value.
	void record_remote::reset_refcount()
	{
		if ( 0u < this->get_refcount() ) 
		{
			this->modify() ;
			this->set_refcount(0u) ;
		}
	}


}
