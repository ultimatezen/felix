#pragma once

#include "resource.h"
#include "DebugUtilities.h"		// extra debugging stuff

#include "RegistryStuff.h"		// CRegMap
#include "resource_string.h"	// R2T
#include "SafeString.h"

/**
	@namespace app_props
	@brief Application properties (persisted to registry).
 */
namespace app_props
{

	static const int NumMems = 15 ;
/**
	@struct properties_loaded_history 
	@brief remembers what memories/glossaries we had loaded.
 */
struct properties_loaded_history : public props::CRegMap
{
	struct props_data
	{
		int m_num_mems ;
		int m_num_gloss ;
		int m_num_remote_mems ;
		int m_num_remote_gloss ;

		TCHAR m_mems[NumMems][MAX_PATH] ;
		TCHAR m_glosses[NumMems][MAX_PATH] ;
		TCHAR m_remote_mems[NumMems][MAX_PATH] ;
		TCHAR m_remote_glosses[NumMems][MAX_PATH] ;

		props_data() : 
			m_num_mems(0),
			m_num_gloss(0),
			m_num_remote_mems(0),
			m_num_remote_gloss(0)
		{
			for ( int i = 0 ; i < NumMems ; ++i )
			{
				ZeroMemory( m_mems[i], MAX_PATH  * sizeof( TCHAR )) ;
				ZeroMemory( m_glosses[i], MAX_PATH  * sizeof( TCHAR )) ;
				ZeroMemory( m_remote_mems[i], MAX_PATH  * sizeof( TCHAR )) ;
				ZeroMemory( m_remote_glosses[i], MAX_PATH  * sizeof( TCHAR )) ;
			}
		}
		props_data( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
		}
		props_data& operator= ( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
			return *this ;
		}
	} ;

	props_data m_data ;

	properties_loaded_history() 
	{}
	properties_loaded_history( const properties_loaded_history &rhs ) :
		m_data( rhs.m_data )
	{
	}
	properties_loaded_history &operator=( const properties_loaded_history &rhs )
	{
		m_data = rhs.m_data ;
		return *this ;
	}
	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, resource_string( IDS_REG_KEY ), _T("LOAD_HISTORY") ) ;

		REG_ENTRY_INT( _T("NUM_MEMS"), m_data.m_num_mems ) ;
		REG_ENTRY_INT( _T("NUM_GLOSS"), m_data.m_num_gloss ) ;
		REG_ENTRY_INT( _T("NUM_REMOTE_MEMS"), m_data.m_num_remote_mems ) ;
		REG_ENTRY_INT( _T("NUM_REMOTE_GLOSS"), m_data.m_num_remote_gloss ) ;

		REG_ENTRY_STRING( _T("MEM1"),	m_data.m_mems[0], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM2"),	m_data.m_mems[1], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM3"),	m_data.m_mems[2], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM4"),	m_data.m_mems[3], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM5"),	m_data.m_mems[4], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM6"),	m_data.m_mems[5], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM7"),	m_data.m_mems[6], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM8"),	m_data.m_mems[7], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM9"),	m_data.m_mems[8], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM10"),	m_data.m_mems[9], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM11"),	m_data.m_mems[10], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM12"),	m_data.m_mems[11], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM13"),	m_data.m_mems[12], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM14"),	m_data.m_mems[13], MAX_PATH )
		REG_ENTRY_STRING( _T("MEM15"),	m_data.m_mems[14], MAX_PATH )

		REG_ENTRY_STRING( _T("GLOSS1"),	m_data.m_glosses[0], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS2"),	m_data.m_glosses[1], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS3"),	m_data.m_glosses[2], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS4"),	m_data.m_glosses[3], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS5"),	m_data.m_glosses[4], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS6"),	m_data.m_glosses[5], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS7"),	m_data.m_glosses[6], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS8"),	m_data.m_glosses[7], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS9"),	m_data.m_glosses[8], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS10"),	m_data.m_glosses[9], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS11"),	m_data.m_glosses[10], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS12"),	m_data.m_glosses[11], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS13"),	m_data.m_glosses[12], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS14"),	m_data.m_glosses[13], MAX_PATH )
		REG_ENTRY_STRING( _T("GLOSS15"),	m_data.m_glosses[14], MAX_PATH )


		REG_ENTRY_STRING( _T("REMOTE_MEM1"),	m_data.m_remote_mems[0], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM2"),	m_data.m_remote_mems[1], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM3"),	m_data.m_remote_mems[2], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM4"),	m_data.m_remote_mems[3], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM5"),	m_data.m_remote_mems[4], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM6"),	m_data.m_remote_mems[5], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM7"),	m_data.m_remote_mems[6], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM8"),	m_data.m_remote_mems[7], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM9"),	m_data.m_remote_mems[8], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM10"),	m_data.m_remote_mems[9], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM11"),	m_data.m_remote_mems[10], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM12"),	m_data.m_remote_mems[11], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM13"),	m_data.m_remote_mems[12], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM14"),	m_data.m_remote_mems[13], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_MEM15"),	m_data.m_remote_mems[14], MAX_PATH )

		REG_ENTRY_STRING( _T("REMOTE_GLOSS1"),	m_data.m_remote_glosses[0], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS2"),	m_data.m_remote_glosses[1], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS3"),	m_data.m_remote_glosses[2], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS4"),	m_data.m_remote_glosses[3], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS5"),	m_data.m_remote_glosses[4], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS6"),	m_data.m_remote_glosses[5], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS7"),	m_data.m_remote_glosses[6], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS8"),	m_data.m_remote_glosses[7], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS9"),	m_data.m_remote_glosses[8], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS10"),	m_data.m_remote_glosses[9], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS11"),	m_data.m_remote_glosses[10], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS12"),	m_data.m_remote_glosses[11], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS13"),	m_data.m_remote_glosses[12], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS14"),	m_data.m_remote_glosses[13], MAX_PATH )
		REG_ENTRY_STRING( _T("REMOTE_GLOSS15"),	m_data.m_remote_glosses[14], MAX_PATH )

	END_REGISTRY_MAP		
} ;


/**
	@struct properties_memory 
	@brief Properties to do with the memories.
 */
struct properties_memory : public props::CRegMap
{
	struct props_data
	{
		size_t	m_min_score ;
		// normalize
		BOOL	m_ignore_case ;
		BOOL	m_ignore_width ;
		BOOL	m_ignore_hir_kat ;
		// formatting
		BOOL	m_plaintext ;
		BOOL	m_assess_format_penalty ;
		// placements
		BOOL	m_place_numbers ;
		BOOL	m_place_gloss ;

		props_data() : 
			m_min_score( 50u ),
			m_ignore_case(TRUE),
			m_ignore_width(FALSE),
			m_ignore_hir_kat(FALSE),
			m_plaintext(FALSE),
			m_assess_format_penalty(FALSE),
			m_place_numbers(TRUE),
			m_place_gloss(FALSE)
		{
		}
		props_data( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
		}
		props_data& operator= ( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
			return *this ;
		}
	} ;

	props_data m_data ;

	size_t get_min_score()
	{
		return m_data.m_min_score ;
	}
	void set_min_score( size_t setting )
	{
		ATLASSERT ( setting <= 100u ) ; 

		m_data.m_min_score = setting ;
	}
	properties_memory() 
	{
	}
	properties_memory( const properties_memory &rhs ) :
		m_data( rhs.m_data ) 
	{
	}
	properties_memory &operator=( const properties_memory &rhs )
	{
		m_data = rhs.m_data ;
		return *this ;
	}

	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, R2T( IDS_REG_KEY ), _T("PROPERTIES") ) ;

		REG_ENTRY_INT( _T("MEM_MIN_MATCH"),			m_data.m_min_score ) ;
		REG_ENTRY_BOOL( _T("MEM_IGNORE_CASE"),		m_data.m_ignore_case ) ;
		REG_ENTRY_BOOL( _T("MEM_IGNORE_HIR_KAT"), m_data.m_ignore_hir_kat );
		REG_ENTRY_BOOL( _T("MEM_IGNORE_WIDTH"),	m_data.m_ignore_width );
		REG_ENTRY_BOOL( _T("MEM_PLAINTEXT"),		m_data.m_plaintext );
		REG_ENTRY_BOOL( _T("MEM_FORMATPENALTY"),	m_data.m_assess_format_penalty );
		REG_ENTRY_BOOL( _T("MEM_PLACE_NUM"),	m_data.m_place_numbers );
		REG_ENTRY_BOOL( _T("MEM_PLACE_GLOSS"),	m_data.m_place_gloss );

	END_REGISTRY_MAP

} ;

/**
	@struct properties_glossary 
	@brief properties to do with the glossaries.
 */
struct properties_glossary : public props::CRegMap
{
	struct props_data
	{
		size_t		m_min_score ;
		size_t		m_max_add ;

		size_t		m_numbering ;

		BOOL	m_ignore_case ;
		BOOL	m_plaintext ;
		BOOL	m_to_lower ;
		BOOL	m_ignore_width ;
		BOOL	m_ignore_hir_kat ;
		BOOL	m_back_color ;

		props_data() : 
			m_min_score( 100u ),
			m_max_add( 15u ),
			m_numbering( 1u ),
			m_ignore_case(TRUE),
			m_plaintext(FALSE),
			m_to_lower(FALSE),
			m_ignore_width(FALSE),
			m_ignore_hir_kat(FALSE),
			m_back_color ( static_cast< int >( RGB( 255, 255, 255 ) ) )
		{
		}
		props_data( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
		}
		props_data& operator= ( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
			return *this ;
		}
	} ;

	props_data m_data ;

	size_t get_min_score()
	{
		return m_data.m_min_score ;
	}
	void set_min_score( size_t setting )
	{
		ATLASSERT ( setting <= 100u ) ; 

		m_data.m_min_score = setting ;
	}
	size_t get_max_add()
	{
		return m_data.m_max_add ;
	}
	void set_max_add( size_t setting )
	{
		m_data.m_max_add = setting ;
	}
	properties_glossary()
	{
	}
	properties_glossary( const properties_glossary &rhs ) : 
		m_data( rhs.m_data )
	{	}

	properties_glossary &operator=( const properties_glossary &rhs )
	{
		m_data = rhs.m_data ;
		return *this ;
	}

	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, resource_string( IDS_REG_KEY ), _T("PROPERTIES") )

		REG_ENTRY_INT( _T("GLOSS_MIN_MATCH"),			m_data.m_min_score ) ;
		REG_ENTRY_INT( _T("GLOSS_MAX_ADD"),				m_data.m_max_add ) ;
		REG_ENTRY_BOOL( _T("GLOSS_IGNORE_CASE"),		m_data.m_ignore_case ) ;
		REG_ENTRY_BOOL( _T("GLOSS_PLAINTEXT"),			m_data.m_plaintext ) ;
		REG_ENTRY_BOOL( _T("GLOSS_TO_LOWER"),			m_data.m_to_lower );
		REG_ENTRY_BOOL( _T("GLOSS_IGNORE_WIDTH"),		m_data.m_ignore_width ) ;
		REG_ENTRY_BOOL( _T("GLOSS_IGNORE_HIR_KAT"),		m_data.m_ignore_hir_kat ) ;
		REG_ENTRY_INT( _T("GLOSS_NUMBERING"),			m_data.m_numbering ) ;
		REG_ENTRY_INT( _T("GLOSS_BACK_COLOR"),			m_data.m_back_color ) ;

	END_REGISTRY_MAP	


} ;

/**
@	struct properties_algorithm : public CRegMap
@brief Algorithm properties.
*/
struct properties_algorithm : public props::CRegMap
{
	struct props_data
	{
		int		m_match_algo ;

		props_data() : 
			m_match_algo( IDC_ALGO_AUTO )
		{
		}
		props_data( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
		}
		props_data& operator= ( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
			return *this ;
		}
	} ;

	props_data m_data ;

	properties_algorithm()
	{
	}
	properties_algorithm( const properties_algorithm &rhs ) :
		m_data( rhs.m_data )
	{
	}
	properties_algorithm &operator=( const properties_algorithm &rhs )
	{
		m_data = rhs.m_data ;
		return *this ;
	}


	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, resource_string( IDS_REG_KEY ), _T("PROPERTIES") ) ;

		REG_ENTRY_INT( _T("ALGORITHM"),		m_data.m_match_algo );

		validate() ;

	END_REGISTRY_MAP

private:

	void validate()
	{
		if( m_data.m_match_algo != IDC_ALGO_CHAR && m_data.m_match_algo != IDC_ALGO_WORD ) 
		{
			m_data.m_match_algo = IDC_ALGO_AUTO ;
		}
	}
} ;

/**
	@struct properties_view  
	@brief View-related properties.
 */
struct properties_view : public props::CRegMap
{
	struct props_data
	{
		BOOL	m_single_screen_matches ;
		int		m_back_color ;
		int		m_query_color ;
		int		m_source_color ;
		int		m_trans_color ;

		props_data() : 
			m_single_screen_matches(FALSE),
			m_back_color ( static_cast< int >( RGB( 255, 255, 255 ) ) ),
			m_query_color ( static_cast< int >( RGB( 0, 0, 0 ) ) ),
			m_source_color ( static_cast< int >( RGB( 0, 0, 0 ) ) ),
			m_trans_color ( static_cast< int >( RGB( 0, 0, 0 ) ) )
		{
		}
		props_data( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
		}
		props_data& operator= ( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
			return *this ;
		}
	} ;

	props_data m_data ;

	properties_view() 
	{	}

	properties_view( const properties_view &rhs ) :
		m_data( rhs.m_data )
	{
	}
	properties_view &operator=( const properties_view &rhs )
	{
		m_data = rhs.m_data ;
		return *this ;
	}

	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, resource_string( IDS_REG_KEY ), _T("PROPERTIES") ) ;

		REG_ENTRY_BOOL( _T("VIEW_ONE_SCREEN"),		m_data.m_single_screen_matches );

		REG_ENTRY_INT( _T("VIEW_BACK_COLOR"),		m_data.m_back_color );
		REG_ENTRY_INT( _T("VIEW_QUERY_COLOR"),		m_data.m_query_color );
		REG_ENTRY_INT( _T("VIEW_SOURCE_COLOR"),	m_data.m_source_color );
		REG_ENTRY_INT( _T("VIEW_TRANS_COLOR"),		m_data.m_trans_color );

	END_REGISTRY_MAP

} ;

/**
	@	struct properties_general 
	@brief general properties.
*/
struct properties_general : public props::CRegMap
{
	struct props_data
	{
		int		m_window_size ;
		int		m_preferred_gui_lang ;
		BOOL	m_load_prev_mem_on_startup ;
		BOOL	m_load_prev_gloss_on_startup ;
		BOOL	m_show_markup ;
		BOOL	m_first_launch ;

		int		m_merge_choice ;
		BOOL	m_query_merge ;

		TCHAR	m_user_name[MAX_PATH] ;

		props_data() : 
			m_window_size( IDC_MEMSIZE_FULL ),
			m_preferred_gui_lang( static_cast< int >( PRIMARYLANGID( ::GetUserDefaultLangID() ) ) ),
			m_load_prev_mem_on_startup( FALSE ),
			m_load_prev_gloss_on_startup( FALSE ),
			m_show_markup(TRUE),
			m_first_launch(TRUE),
			m_merge_choice(IDC_MERGE),
			m_query_merge(TRUE)
		{
			DWORD path_len(MAX_PATH) ;
			::GetUserName( m_user_name, &path_len ) ;
		}
		props_data( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
		}
		props_data& operator= ( const props_data &rhs )
		{
			memcpy( this, &rhs, sizeof( props_data ) ) ;
			return *this ;
		}
	} ;

	props_data m_data ;

	properties_general() 
	{	}
	properties_general( const properties_general &rhs ) :
		m_data( rhs.m_data )
	{
	}
	properties_general &operator=( const properties_general &rhs )
	{
		m_data = rhs.m_data ;
		return *this ;
	}

	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, resource_string( IDS_REG_KEY ), _T("PROPERTIES") ) ;

		REG_ENTRY_INT( _T("GENERAL_WINDOW_SIZE"),		m_data.m_window_size ) 
		REG_ENTRY_INT( _T("GENERAL_GUI_LANG"),			m_data.m_preferred_gui_lang ) 
		REG_ENTRY_BOOL( _T("GENERAL_LOAD_MEMS"),		m_data.m_load_prev_mem_on_startup )
		REG_ENTRY_BOOL( _T("GENERAL_LOAD_GLOSS"),		m_data.m_load_prev_gloss_on_startup )
		REG_ENTRY_BOOL( _T("GENERAL_SHOW_MARKUP"),		m_data.m_show_markup )
		REG_ENTRY_BOOL( _T("GENERAL_FIRST_LAUNCH"),		m_data.m_first_launch)
		REG_ENTRY_INT( _T("GENERAL_MERGE_CHOICE"),		m_data.m_merge_choice)
		REG_ENTRY_BOOL( _T("GENERAL_QUERY_MERGE"),		m_data.m_query_merge)
		REG_ENTRY_STRING( _T("GENERAL_USER_NAME"),		m_data.m_user_name, MAX_PATH )

	END_REGISTRY_MAP		

} ;

/**
@	struct properties
@brief application properties holder.
*/
struct properties
{
	properties_memory		m_mem_props ;
	properties_glossary		m_gloss_props ;
	properties_general		m_gen_props ;
	properties_algorithm	m_alg_props ;
	properties_view			m_view_props ;

	properties& operator=( const properties &rhs )
	{
		m_mem_props		= rhs.m_mem_props ;
		m_gloss_props	= rhs.m_gloss_props ;
		m_gen_props		= rhs.m_gen_props ;
		m_alg_props		= rhs.m_alg_props ;
		m_view_props	= rhs.m_view_props;

		return *this ;
	}
	bool read_from_registry()
	{
		bool retval = true ;

		if ( ! m_mem_props.read_from_registry() )
		{
			retval = false ;
		}
		if ( ! m_gloss_props.read_from_registry() )
		{
			retval = false ;
		}
		if ( ! m_gen_props.read_from_registry() )
		{
			retval = false ;
		}
		if ( ! m_alg_props.read_from_registry() )
		{
			retval = false ;
		}
		if ( ! m_view_props.read_from_registry() )
		{
			retval = false ;
		}

		return retval ;
	}
	bool write_to_registry()
	{
		bool retval = true ;

		if ( ! m_mem_props.write_to_registry() )
		{
			retval = false ;
		}
		if ( ! m_gloss_props.write_to_registry() )
		{
			retval = false ;
		}
		if ( ! m_gen_props.write_to_registry() )
		{
			retval = false ;
		}
		if ( ! m_alg_props.write_to_registry() )
		{
			retval = false ;
		}
		if ( ! m_view_props.write_to_registry() )
		{
			retval = false ;
		}

		return retval ;
	}
} ;

typedef boost::shared_ptr<properties> props_ptr ;

} ;