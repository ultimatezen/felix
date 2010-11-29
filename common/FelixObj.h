// FelixObj.h: interface for the CFelix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FelixOBJ_H__0C7D3078_71F1_467E_A820_7D4A710BCA13__INCLUDED_)
#define AFX_FelixOBJ_H__0C7D3078_71F1_467E_A820_7D4A710BCA13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OleAutoWrapper.h"

#include "StringEx.h"				// string stuff
#include <set>						// set

#include "StringConversions.h"
#include "MiscWrappers.h"

using misc_wrappers::date ;

class CFelix : COleAutoWrapper
{
public:
	CFelix() ;

	bool is_visible(void);
	void put_visible_on(void);
	void put_visible_off(void);

	int get_num_gloss_hits() ;
	_bstr_t get_gloss_entry( short index ) ;

	_bstr_t get_trans() ;
	_bstr_t get_query() ;

	double get_score() ;

	void lookup( _bstr_t query ) ;
	void lookup_trans( _bstr_t query ) ;
	void correct_trans( _bstr_t trans ) ;

	void set_trans(_bstr_t setting) ;
	void set_query(_bstr_t setting) ;

	void save() ;
	void quit() ;
	void next_translation() ;
	void previous_translation() ;
	void delete_trans() ;
	void concordance( _bstr_t query ) ;
	void translation_concordance( _bstr_t query ) ;
	void add_entry_to_glossary( _bstr_t query, _bstr_t trans, _bstr_t context = L"" ) ;
};

//
//class CFelixApp :
//	public CComPtr< Felix::IApp >
//{
//public:
//
//	CFelixApp(void)
//	{
//		COM_ENFORCE( CoCreateInstance( L"Felix.App", NULL, CLSCTX_LOCAL_SERVER|CLSCTX_INPROC_SERVER ), _T("Failed to create Felix instance") ) ;
//		(*this)->Visible = VARIANT_TRUE ;
//
//	}
//
//	~CFelixApp(void)
//	{
//	}
//};

/*
#define TRANS_ENFORCE( test, message )	ensure_instantiated() ; AUTO_ENFORCE( test, message, m_ta_obj )

typedef Felix::IApplicationPtr IApplicationPtr ;
typedef Felix::IApplication2Ptr IApplication2Ptr ;
typedef Felix::IMemoriesPtr IMemoriesPtr ;
typedef Felix::IMemoryPtr IMemoryPtr ;
typedef Felix::IRecordPtr IRecordPtr ;

typedef CComPtr< Felix::IApplication > app_ptr ;
typedef CComPtr< Felix::IApplication2 > app2_ptr ;
typedef CComPtr< Felix::IMemories > memories_ptr ;
typedef CComPtr< Felix::IMemory > memory_ptr ;
typedef CComPtr< Felix::IRecord > record_ptr ;
typedef CComPtr< Felix::IGlossaryWindows > gloss_windows_ptr ;
typedef CComPtr< Felix::IGlossaryWindow > gloss_window_ptr ;

template< class TBase, class member_type >
class FelixObject
{
	typedef FelixObject< TBase, member_type >	this_obj_type ;

	protected:
		member_type		m_ta_obj ;
		bool			m_requires_pause ;
		
	public:
		FelixObject( bool requires_pause = false ) : m_requires_pause(requires_pause) {}
		FelixObject( member_type obj, bool requires_pause = false ) : m_ta_obj( obj ), m_requires_pause(requires_pause) {}
		this_obj_type& operator=( memory_ptr mem )
		{
			m_ta_obj = mem ;
			return *this ;
		}
		this_obj_type& operator=( const this_obj_type &obj )
		{
			m_ta_obj = obj.m_ta_obj ;
			return *this ;
		}
		void unload()
		{
			if ( is_instantiated() )
			{
				m_ta_obj.Release() ;
			}
		}
		member_type detach()
		{
			member_type member = m_ta_obj ;
			m_ta_obj.Release() ;
			return member ;
		}
	protected:
		void ensure_instantiated()
		{
			if ( ! is_instantiated() )
			{
				create() ;
			}
		}
		// is_instantiated
		bool is_instantiated()
		{
			return !! m_ta_obj ;
		}
		void create()
		{
			// using TRANS_ENFORCE here will cause an endless loop, due to ensure_instantiated!
			AUTO_ENFORCE( m_ta_obj.CoCreateInstance( TBase::get_object_string(), NULL, CLSCTX_LOCAL_SERVER|CLSCTX_INPROC_SERVER ), _T("Felix.Memory: Call to CoCreateInstance failed"), m_ta_obj ) ;
			
			if ( m_requires_pause )
			{
				MSG msg ;
				for ( int i=0 ; i<10 ; ++ i )
				{
					while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
					{ 
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					} 
					::Sleep( 10 ) ;
				}
			}
		}
};

class CMemory : public FelixObject< CMemory, memory_ptr >
{
	
public:
	CMemory() {}
	CMemory( memory_ptr mem ) : FelixObject< CMemory, memory_ptr >( mem ) {}
	CMemory( const CMemory &mem ) : FelixObject< CMemory, memory_ptr >( mem.m_ta_obj ) {}

	static _bstr_t get_object_string() 
	{
		return _bstr_t( L"Felix.Memory" ) ;
	}

	// size
	long size();

	long get_num_gloss_matches( const _bstr_t &query );
	double get_best_score( const _bstr_t &query );

	void get_reliability_statistics( long *low, long *high, double *ave );

	_bstr_t get_percent_validated_string();

	// source/target
	_bstr_t get_source_language();
	_bstr_t get_target_language();

	// add_record
	record_ptr add_record( const _bstr_t src, const _bstr_t trans, const _bstr_t context );

	// name
	_bstr_t get_name( );
	void set_name( const _bstr_t &memory_filename );

	// save
	void save( );

	// load
	void load( const _bstr_t &memory_filename );

	// load_header
	void load_header( const _bstr_t &memory_filename );

	// date_created
	date get_date_created();
	
	// created_by
	
	_bstr_t get_created_by();
	
	void set_memory_on() ;
	void set_memory_off() ;
	bool is_memory();


	// client
	void set_client( const _bstr_t &client );
	_bstr_t get_client();

	// field
	void set_field( const _bstr_t &field );
	_bstr_t get_field();

};

class CMemories : public FelixObject< CMemories, memories_ptr >
{
	
public:
	CMemories() {}
	CMemories( memories_ptr mems ) : FelixObject< CMemories, memories_ptr >( mems ) {}
	CMemories( CMemories &mems ) : FelixObject< CMemories, memories_ptr >( mems.m_ta_obj ) {}

	static _bstr_t get_object_string()
	{
		return _bstr_t( L"Felix.Memories" ) ;
	}
	memory_ptr add()
	{
		memory_ptr mem ;
		TRANS_ENFORCE( m_ta_obj->raw_Add( &mem ), _T("Failed to add memory") ) ;
		return mem ;

	}
};

class CGlossaryWindow : public FelixObject< CGlossaryWindow, gloss_window_ptr >
{
	
public:
	CGlossaryWindow() {}
	CGlossaryWindow( gloss_window_ptr gloss ) : FelixObject< CGlossaryWindow, gloss_window_ptr >( gloss ) {}
	CGlossaryWindow( CGlossaryWindow &gloss ) : FelixObject< CGlossaryWindow, gloss_window_ptr >( gloss.m_ta_obj ) {}

	static _bstr_t get_object_string()
	{
		return _bstr_t( L"Felix.GlossaryWindow" ) ;
	}
	memories_ptr get_memories()
	{
		memories_ptr mems ;
		TRANS_ENFORCE( m_ta_obj->get_Glossaries( &mems ), _T("Failed to get memories from Felix glossary window") ) ;
		return mems ;
	}
};

class CGlossaryWindows : public FelixObject< CGlossaryWindows, gloss_windows_ptr >
{
	
public:
	CGlossaryWindows() {}
	CGlossaryWindows( gloss_windows_ptr gloss ) : FelixObject< CGlossaryWindows, gloss_windows_ptr >( gloss ) {}
	CGlossaryWindows( CGlossaryWindows &gloss ) : FelixObject< CGlossaryWindows, gloss_windows_ptr >( gloss.m_ta_obj ) {}

	static _bstr_t get_object_string()
	{
		return _bstr_t( L"Felix.GlossaryWindows" ) ;
	}
	gloss_window_ptr add()
	{
		gloss_window_ptr gloss ;
		TRANS_ENFORCE( m_ta_obj->raw_Add( &gloss ), _T("Failed to get add glossary window to Felix") ) ;
		return gloss ;
	}
	gloss_window_ptr get_top()
	{
		gloss_window_ptr gloss ;
		if ( empty() )
		{
			TRANS_ENFORCE( m_ta_obj->raw_Add( &gloss ), _T("Failed to get add glossary window to Felix") ) ;
		}
		else
		{
			TRANS_ENFORCE( m_ta_obj->get_item( 1, &gloss ), _T("Failed to get add glossary window to Felix") ) ;
		}
		return gloss ;
	}
	bool empty()
	{
		return ( 0 == size() ) ;
	}
	size_t size()
	{
		long count = 0 ;
		TRANS_ENFORCE( m_ta_obj->get_Count( &count ), _T("Failed to get get number of glossary windows") ) ;
		return static_cast< size_t > ( count ) ;
	}
} ;

class CFelix2 : public FelixObject< CFelix2, app2_ptr >
{
	
public:
	CFelix2() { }
	CFelix2( app2_ptr app ) : FelixObject< CFelix2, app2_ptr >( app ) {}
	CFelix2( CFelix2 &app ) : FelixObject< CFelix2, app2_ptr >( app.m_ta_obj ) {}

	static _bstr_t get_object_string()
	{
		return _bstr_t( L"Felix.Application2" ) ;
	}

	memories_ptr get_memories()
	{
		ensure_instantiated() ;
		ATLASSERT( is_instantiated() ) ;

		memories_ptr memories ;
		TRANS_ENFORCE( m_ta_obj->get_Memories( &memories ), _T("Failed to get memories from Felix") ) ;
		return memories ;
	}
	gloss_windows_ptr get_glossary_windows()
	{
		ensure_instantiated() ;
		ATLASSERT( is_instantiated() ) ;
		
		gloss_windows_ptr glossary_windows ;
		TRANS_ENFORCE( m_ta_obj->get_GlossaryWindows( &glossary_windows ), _T("Failed to get glossary windows from Felix") ) ;
		return glossary_windows ;
	}

} ;

class CFelix : public FelixObject< CFelix, app_ptr >  
{

public:
	CFelix() : FelixObject< CFelix, app_ptr >( true ){ }
	CFelix( app_ptr app ) : FelixObject< CFelix, app_ptr >( app, true ) {}
	CFelix( CFelix &app ) : FelixObject< CFelix, app_ptr >( app.m_ta_obj ) {}

	static _bstr_t get_object_string()
	{
		return _bstr_t( L"Felix.Application" ) ;
	}

	void set_visible_off();
	void set_visible_on();

	void quit_silently();
	
	// set_translation
	void set_translation( const wstring &translation );
	void set_translation( const _bstr_t &translation );
	
	// lookup
	void lookup( const _bstr_t &query );
	void lookup( const wstring &query );
	
	// get_score
	double get_score();
	
	// correct_translation
	void correct_translation( const wstring &trans )
	{
		correct_translation( string2BSTR( trans ) ) ;
	}
	void correct_translation( const _bstr_t &trans ) ;

	// lookup_translation
	void lookup_translation(const wstring &query);
	void lookup_translation( const _bstr_t &query );
	
	// get_trans_concordance
	void get_trans_concordance( const wstring &query )
	{
		get_trans_concordance( string2BSTR( query ) ) ;
	}
	void get_trans_concordance( const _bstr_t &query );
	
	// get_concordance
	void get_concordance( const wstring &query )
	{
		get_concordance( string2BSTR( query ) ) ;
	}
	void get_concordance(const _bstr_t &query) ;
	
	// get_num_matches
	long get_num_matches();

	// get_glossary_hits
	void get_glossary_hits(std::set< string > &hits) ;
	void get_glossary_hits(std::set< wstring > &hits) ;

	// delete_current_translation
	void delete_current_translation();

	// add_gloss_entry
	void add_gloss_entry( const wstring &entry );
	void add_gloss_entry( const _bstr_t &entry );

	// save_memory
	void save_memory();
	
	// navigation
	void previous_translation();
	void next_translation();

	// get

	short get_num_gloss_matches();
	wstring get_gloss_entry( short entry_num );
	// translation
	wstring get_translation();
	// query
	wstring get_query() ;
	void get_query( _bstr_t &query ) ;

	// visible
	bool is_visible();

	// add an entry to the glossary
	void add_entry_to_glossary( const _bstr_t &source, const _bstr_t &trans, const _bstr_t &context ) ;

	// control running state
	

};
*/

#endif // !defined(AFX_FelixOBJ_H__0C7D3078_71F1_467E_A820_7D4A710BCA13__INCLUDED_)
