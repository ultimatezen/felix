#include "StdAfx.h"
#include "record_remote.h"
#include "Felix_properties.h"

// implementation here
namespace memory_engine
{
	record_remote::record_remote(LPCWSTR server_name) : m_engine(server_name)
	{
		copy_engine_values() ;
	}

	record_remote::record_remote( record_pointer rec ) : 
	m_engine(L"Felix.RemoteRecord")
	{
		internal_copy( rec ) ;
	}

	record_remote::record_remote( CDispatchWrapper wrapper ) : 
	m_engine(wrapper)
	{
		copy_engine_values() ;
	}

	void record_remote::copy_engine_values()
	{
		wstring source(m_engine.prop_get(L"Source").bstrVal) ;
		m_source.set_value(source) ;

		wstring trans(m_engine.prop_get(L"Trans").bstrVal) ;
		m_trans.set_value(trans) ;

		wstring context( m_engine.prop_get(L"Context").bstrVal) ;
		m_context.set_value(context) ;

		m_is_validated = m_engine.prop_get(L"Validated").boolVal != VARIANT_FALSE ;

		m_reliability = static_cast< size_t >( m_engine.prop_get(L"Reliability").lVal ) ;

		m_refcount = static_cast< size_t >( m_engine.prop_get(L"RefCount").lVal ) ;

		m_created = BSTR2tstring(m_engine.prop_get(L"Created").bstrVal) ;
		m_modified = BSTR2tstring(m_engine.prop_get(L"Modified").bstrVal) ;

		m_id = static_cast< size_t >( m_engine.prop_get(L"Id").lVal ) ;

	}

	const wstring & record_remote::get_source_rich() const
	{
		return m_source.rich() ;
	}

	const wstring record_remote::get_source_plain()
	{
		return m_source.plain() ;
	}

	const wstring & record_remote::get_source_cmp() const
	{
		return m_source.cmp() ;
	}

	const wstring & record_remote::get_trans_rich() const
	{
		return m_trans.rich() ;
	}

	const wstring record_remote::get_trans_plain()
	{
		return m_trans.plain() ;
	}

	const wstring & record_remote::get_trans_cmp() const
	{
		return m_trans.cmp() ;
	}

	const wstring & record_remote::get_context_rich() const
	{
		return m_context.rich() ;
	}

	const wstring record_remote::get_context_plain()
	{
		return m_context.plain() ;
	}

	const wstring & record_remote::get_context_cmp() const
	{
		return m_context.cmp() ;
	}

	size_t record_remote::get_reliability() const
	{
		return m_reliability ;
	}

	void record_remote::set_reliability( const size_t reliability )
	{
		m_engine.prop_put(L"Reliability", reliability) ;
		CComVariant result = m_engine.prop_get(L"Reliability") ;
		m_reliability = static_cast<size_t>(result.lVal) ;
	}

	void record_remote::set_item( const wstring &key, const wstring &value )
	{
		key ;
		value ;
	}

	void record_remote::set_source( const wstring source )
	{
		m_engine.prop_put(L"Source", source.c_str()); 
		m_source.set_value(source) ;
	}

	void record_remote::set_trans( const wstring trans )
	{
		m_engine.prop_put(L"Trans", trans.c_str()); 
		m_trans.set_value(trans) ;
	}

	void record_remote::set_context( const wstring context )
	{
		m_engine.prop_put(L"Context", context.c_str()); 
		m_context.set_value(context) ;
	}

	bool record_remote::is_validated() const
	{
		return m_is_validated ;
	}

	void record_remote::set_validated_on()
	{
		m_engine.prop_put(L"Validated", VARIANT_TRUE) ;
		m_is_validated = true ;
	}

	void record_remote::set_validated_off()
	{
		m_engine.prop_put(L"Validated", VARIANT_FALSE) ;
		m_is_validated = false ;
	}

	const misc_wrappers::date & record_remote::get_created() const
	{
		return m_created ;
	}

	const misc_wrappers::date & record_remote::get_modified() const
	{
		return m_modified ;
	}

	void record_remote::set_created( const wstring &created )
	{
		m_created = created ;
		m_engine.prop_put(L"Created", m_created.get_date_time_string().c_str()) ;
	}

	void record_remote::set_created( const misc_wrappers::date &created )
	{
		m_created = created ;
		m_engine.prop_put(L"Created", m_created.get_date_time_string().c_str()) ;
	}
	void record_remote::set_modified( const wstring &modified )
	{
		m_modified = modified ;
		m_engine.prop_put(L"Modified", m_modified.get_date_time_string().c_str()) ;
		if ( m_modified < m_created )
		{
			set_created(m_modified) ;
		}
	}

	void record_remote::set_modified( const misc_wrappers::date &modified )
	{
		m_modified = modified ;
		m_engine.prop_put(L"Modified", m_modified.get_date_time_string().c_str()) ;
		if ( m_modified < m_created )
		{
			set_created(m_modified) ;
		}
	}

	// create
	bool record_remote::create(  )
	{
		m_created.set_to_local_time() ;
		m_engine.prop_put(L"Created", m_created.get_date_time_string().c_str()) ;
		return modify() ;
	}


	// modify
	// set the last-modified value to the current date-time
	bool record_remote::modify (  )
	{
		m_modified.set_to_local_time() ;
		m_engine.prop_put(L"Modified", m_modified.get_date_time_string().c_str()) ;

		return true ;
	}


	void record_remote::set_refcount( size_t count )
	{
		m_engine.prop_put(L"RefCount", count) ;
		m_refcount = count ;
	}

	size_t record_remote::get_refcount() const
	{
		return m_refcount ;
	}

	bool record_remote::is_valid_record()
	{
		return true ;
	}

	translation_record::record_data_iterator record_remote::data_begin()
	{
		return m_record_data.begin() ;
	}

	translation_record::record_data_iterator record_remote::data_end()
	{
		return m_record_data.end() ;
	}

	translation_record::record_data_map& record_remote::get_data_map()
	{
		return this->m_record_data ;
	}

	bool record_remote::item_exists( const wstring &key ) const
	{
		key ;
		return false ;
	}

	const wstring record_remote::get_item( const wstring &key ) const
	{
		key ;
		return wstring() ;
	}

	bool record_remote::remove_item( const wstring &key )
	{
		key ;
		return false ;
	}

	void record_remote::clear_user_strings()
	{

	}

	void record_remote::set_cmp_maker( const CmpMaker maker )
	{
		m_source.set_cmp_maker(maker) ;
		m_trans.set_cmp_maker(maker) ;
		m_context.set_cmp_maker(maker) ;
	}

	record_pointer record_remote::clone()
	{
		record_pointer cloned(new record_remote()) ;
		this->copy_from_self(cloned) ;
		return cloned ;
	}

	record_pointer record_remote::copy_from_self( record_pointer rec )
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
		return rec ;
	}

	void record_remote::internal_copy( record_pointer rec )
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
		set_id(rec->get_id()) ;
	}

	size_t record_remote::get_id()
	{
		CComVariant result = m_engine.prop_get(L"Id") ;
		m_id = static_cast< size_t >( result.lVal ) ;
		return m_id;
	}

	void record_remote::set_id( size_t id )
	{
		m_engine.prop_put(L"Id", id) ;
		m_id = id ;
	}

	ATL::CComVariant record_remote::get_engine()
	{
		return CComVariant(m_engine.get_dispatch());
	}

	record_remote::~record_remote()
	{
	}

	void record_remote::set_creator( const wstring &creator )
	{
		m_creator = creator ;
	}

	void record_remote::set_modified_by( const wstring &modified )
	{
		m_modified_by = modified ;
	}

	wstring record_remote::get_creator()
	{
		if (m_creator.empty())
		{
			m_creator = get_record_username() ;
		}
		return m_creator ;
	}

	wstring record_remote::get_modified_by()
	{
		if (m_modified_by.empty())
		{
			m_modified_by = get_record_username() ;
		}
		return m_modified_by ;
	}
}
