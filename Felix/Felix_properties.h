#pragma once

#include "resource.h"
#include "DebugUtilities.h"		// extra debugging stuff

#include "RegistryStuff.h"		// CRegMap
#include "resource_string.h"	// R2T

#include "output_device.h"
#include "input_device.h"

#include "xml_utils.h"

pugi::xml_node get_prop_node( pugi::xml_document &doc, string node_name ) ;

/**
	@namespace app_props
	@brief Application properties (persisted to registry).
 */
namespace app_props
{
	void write_xml_doc( pugi::xml_document &doc, output_device_ptr output );
	void write_filenames( pugi::xml_node &node, const std::vector<wstring> &filenames, string node_name ) ;
	void load_xml_props_type(pugi::xml_node &parent, std::vector<wstring> &items, string node_name) ;

/**
	@struct properties_loaded_history 
	@brief remembers what memories/glossaries we had loaded.
 */
	// constant for old registry verison
	static const int NumMems = 15 ;

	struct properties_loaded_history : public props::CRegMap
{
	std::vector<wstring> m_loaded_mems ;
	std::vector<wstring> m_loaded_gloss ;
	std::vector<wstring> m_loaded_remote_mems ;
	std::vector<wstring> m_loaded_remote_gloss ;

	wstring m_memory_location;
	wstring m_glossary_location;
	wstring m_preferences_location;

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

	boost::function<string()> m_get_config_text ;

	properties_loaded_history();
	properties_loaded_history( const properties_loaded_history &rhs ) :
		m_data( rhs.m_data )
	{
	}
	properties_loaded_history &operator=( const properties_loaded_history &rhs );
	bool load_xml_props();

	bool write_xml_props();

	void write_xml_file( const string &text );

	void write_xml_file( output_device_ptr output );
	string make_xml_doc();

	// dealing with the actual XML doc objects
	void build_xml_doc( pugi::xml_node &prefs );
	bool parse_xml_doc( pugi::xml_document &doc );

	bool copy_reg_props();
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

		if (! is_read)
		{
			if (write_xml_props())
			{
				return true ;
			}
			copy_reg_props() ;

		}		
		if (is_read)
		{
			if (load_xml_props())
			{
				return true ;
			}
			copy_reg_props() ;
		}

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
		BOOL	m_place_rules;

		BOOL	m_one_trans_per_source ;

		props_data() : 
			m_min_score( 50u ),
			m_ignore_case(TRUE),
			m_ignore_width(FALSE),
			m_ignore_hir_kat(FALSE),
			m_plaintext(FALSE),
			m_assess_format_penalty(FALSE),
			m_place_numbers(TRUE),
			m_one_trans_per_source(FALSE),
			m_place_gloss(FALSE),
			m_place_rules(FALSE)
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

	bool is_one_trans_per_source(void)
	{
		return !! m_data.m_one_trans_per_source ;
	}
	size_t get_min_score()
	{
		return m_data.m_min_score ;
	}
	void set_min_score( size_t setting )
	{
		ATLASSERT ( setting <= 100u ) ; 

		m_data.m_min_score = setting ;
	}
	bool is_plaintext()
	{
		return !! m_data.m_plaintext ;
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
		REG_ENTRY_BOOL( _T("MEM_PLACE_RULES"),	m_data.m_place_rules);

	END_REGISTRY_MAP

	// dealing with the actual XML doc objects
	void build_xml_doc( pugi::xml_node &prefs );
	bool parse_xml_doc( pugi::xml_document &doc );


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
		BOOL	m_simple_view ;

		long	m_back_color ;

		props_data() : 
			m_min_score( 100u ),
			m_max_add( 15u ),
			m_numbering( 1u ),
			m_ignore_case(TRUE),
			m_plaintext(FALSE),
			m_to_lower(FALSE),
			m_ignore_width(FALSE),
			m_ignore_hir_kat(FALSE),
			m_simple_view(FALSE),
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

	bool get_simple_view() const
	{
		return !! m_data.m_simple_view ;
	}
	bool get_ignore_case() const
	{
		return !!  m_data.m_ignore_case ;
	}
	bool get_ignore_width() const
	{
		return !!  m_data.m_ignore_width ;
	}
	bool get_m_ignore_hir_kat() const
	{
		return !!  m_data.m_ignore_hir_kat ;
	}
	size_t get_numbering() const
	{
		return m_data.m_numbering ;
	}
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
	bool is_plaintext()
	{
		return !! m_data.m_plaintext ;
	}
	bool is_to_lower()
	{
		return !! m_data.m_to_lower ;
	}
	void set_numbering(int numbering)
	{
		m_data.m_numbering = numbering ;
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
		REG_ENTRY_BOOL( _T("GLOSS_SIMPLE_VIEW"),		m_data.m_simple_view) ;
		REG_ENTRY_INT( _T("GLOSS_BACK_COLOR"),			m_data.m_back_color ) ;

	END_REGISTRY_MAP

	// dealing with the actual XML doc objects
	void build_xml_doc( pugi::xml_node &prefs );
	bool parse_xml_doc( pugi::xml_document &doc );

} ;

/**
@struct properties_algorithm : public CRegMap
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

	// dealing with the actual XML doc objects
	void build_xml_doc( pugi::xml_node &prefs );
	bool parse_xml_doc( pugi::xml_document &doc );

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
	std::vector<wstring> m_qc_glosses;

	struct props_data
	{
		BOOL	m_single_screen_matches ;
		BOOL	m_show_gloss_matches ;
		int		m_back_color ;
		int		m_query_color ;
		int		m_source_color ;
		int		m_trans_color ;

		int		m_mem_mousewheel ;
		int		m_gloss_mousewheel ;

		props_data() : 
			m_single_screen_matches(FALSE),
			m_show_gloss_matches(FALSE),
			m_back_color ( static_cast< int >( RGB( 255, 255, 255 ) ) ),
			m_query_color ( static_cast< int >( RGB( 0, 0, 0 ) ) ),
			m_source_color ( static_cast< int >( RGB( 0, 0, 0 ) ) ),
			m_trans_color ( static_cast< int >( RGB( 0, 0, 0 ) ) ),
			m_mem_mousewheel(0),
			m_gloss_mousewheel(0)
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

	int get_mem_mousewheel();
	int get_gloss_mousewheel();

	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, resource_string( IDS_REG_KEY ), _T("PROPERTIES") ) ;

		REG_ENTRY_BOOL( _T("VIEW_ONE_SCREEN"),		m_data.m_single_screen_matches );
		REG_ENTRY_BOOL( _T("VIEW_GLOSS_MATCHES"),	m_data.m_show_gloss_matches );
		REG_ENTRY_INT( _T("VIEW_BACK_COLOR"),		m_data.m_back_color );
		REG_ENTRY_INT( _T("VIEW_QUERY_COLOR"),		m_data.m_query_color );
		REG_ENTRY_INT( _T("VIEW_SOURCE_COLOR"),		m_data.m_source_color );
		REG_ENTRY_INT( _T("VIEW_TRANS_COLOR"),		m_data.m_trans_color );

	END_REGISTRY_MAP

	// dealing with the actual XML doc objects
	void build_xml_doc( pugi::xml_node &prefs );
	bool parse_xml_doc( pugi::xml_document &doc );

	void load_mousewheel_props();

	int clamp_mousewheel( const int val );

} ;


struct properties_qc : public props::CRegMap
{
	std::vector<wstring> m_qc_glosses ;

	struct props_data
	{
		BOOL	m_check_numbers ;
		BOOL	m_check_all_caps ;
		BOOL	m_check_gloss ;
		BOOL	m_live_checking ;

		props_data() : 
		m_check_numbers(FALSE),
			m_check_all_caps(FALSE),
			m_check_gloss(FALSE),
			m_live_checking(FALSE)
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

	properties_qc() 
	{	}

	properties_qc( const properties_qc &rhs ) :
	m_data( rhs.m_data )
	{
	}
	properties_qc &operator=( const properties_qc &rhs )
	{
		m_data = rhs.m_data ;
		return *this ;
	}
	bool qc_enabled() const
	{
		return check_numbers() || check_gloss() || check_all_caps() ;
	}
	bool check_numbers() const
	{
		return !! m_data.m_check_numbers ;
	}
	bool check_all_caps() const
	{
		return !! m_data.m_check_all_caps ;
	}
	bool check_gloss() const
	{
		return !! m_data.m_check_gloss ;
	}
	bool live_checking() const
	{
		return !! m_data.m_live_checking ;
	}
	bool check_gloss_name(const wstring name) const
	{
		return std::find(m_qc_glosses.begin(), m_qc_glosses.end(), name) != m_qc_glosses.end() ;
	}
	BEGIN_REGISTRY_MAP( HKEY_CURRENT_USER, resource_string( IDS_REG_KEY ), _T("PROPERTIES") ) ;
		REG_ENTRY_BOOL( _T("qc_check_numbers"),		m_data.m_check_numbers );
		REG_ENTRY_BOOL( _T("qc_check_all_caps"),	m_data.m_check_all_caps );
		REG_ENTRY_BOOL( _T("qc_check_gloss"),		m_data.m_check_gloss );
		REG_ENTRY_BOOL( _T("qc_live_checking"),		m_data.m_live_checking );
	END_REGISTRY_MAP

	// dealing with the actual XML doc objects
	void build_xml_doc( pugi::xml_node &prefs );
	bool parse_xml_doc( pugi::xml_document &doc );
} ;

/**
General properties
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
		BOOL	m_old_mem_mgr ;

		BOOL	m_must_login ;
		BOOL	m_save_credentials;

		TCHAR	m_user_name[MAX_PATH] ;

		props_data() : 
			m_window_size( IDC_MEMSIZE_FULL ),
			m_preferred_gui_lang( static_cast< int >( PRIMARYLANGID( ::GetUserDefaultLangID() ) ) ),
			m_load_prev_mem_on_startup( FALSE ),
			m_load_prev_gloss_on_startup( FALSE ),
			m_show_markup(TRUE),
			m_first_launch(TRUE),
			m_merge_choice(IDC_MERGE),
			m_query_merge(TRUE),
			m_old_mem_mgr(FALSE),
			m_must_login(FALSE),
			m_save_credentials(FALSE)
		{
			ZeroMemory(m_user_name, sizeof(m_user_name)) ;
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

	std::map<wstring, wstring> m_saved_credentials;

	properties_general() 
	{	}
	properties_general( const properties_general &rhs ) :
		m_data(rhs.m_data), m_saved_credentials(rhs.m_saved_credentials)
	{
	}

	// getters/setters
	bool load_prev_mem_on_startup(void)
	{
		return !! m_data.m_load_prev_mem_on_startup ;
	}
	bool load_prev_gloss_on_startup(void)
	{
		return !! m_data.m_load_prev_gloss_on_startup ;
	}
	void set_must_log_in(BOOL value)
	{
		m_data.m_must_login = value;
	}
	void set_save_credentials(BOOL value)
	{
		m_data.m_save_credentials = value;
	}
	void set_not_first_launch()
	{
		m_data.m_first_launch = FALSE ;
	}

	properties_general &operator=( const properties_general &rhs )
	{
		m_data = rhs.m_data ;
		std::map<wstring, wstring> tmp(rhs.m_saved_credentials);
		tmp.swap(m_saved_credentials);
		return *this;
	}
	wstring get_user_name()
	{
		return wstring(m_data.m_user_name) ;
	}
	bool is_first_launch()
	{
		return !! m_data.m_first_launch ;
	}
	bool must_log_in(void)
	{
		return !!m_data.m_must_login;
	}
	bool save_credentials(void)
	{
		return !!m_data.m_save_credentials;
	}
	bool credential_is_saved(wstring connection)
	{
		return m_saved_credentials.find(connection) != m_saved_credentials.end();
	}
	wstring username_for_connection(wstring connection)
	{
		return m_saved_credentials[connection];
	}
	void set_credential(wstring connection, wstring username)
	{
		m_saved_credentials[connection] = username;
	}
	void remove_credential(wstring connection)
	{
		if (credential_is_saved(connection))
		{
			m_saved_credentials.erase(connection);
		}
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
		REG_ENTRY_BOOL( _T("GENERAL_OLD_MEM_MGR"),		m_data.m_old_mem_mgr)
		REG_ENTRY_STRING( _T("GENERAL_USER_NAME"),		m_data.m_user_name, MAX_PATH )
		REG_ENTRY_BOOL( _T("GENERAL_MUST_LOGIN"),		m_data.m_must_login)
		REG_ENTRY_BOOL(_T("GENERAL_SAVE_CREDENTIALS"), m_data.m_save_credentials)
	END_REGISTRY_MAP

	// dealing with the actual XML doc objects
	void build_xml_doc( pugi::xml_node &prefs );
	bool parse_xml_doc( pugi::xml_document &doc );
	
	void serialize_credentials(pugi::xml_node &parent);
	std::pair<wstring, wstring> parse_credential(pugi::xml_node &credential);
	void parse_credentials(pugi::xml_node &credentials);

} ;

/**
struct properties
*/
struct properties
{
	properties_memory		m_mem_props ;
	properties_glossary		m_gloss_props ;
	properties_general		m_gen_props ;
	properties_algorithm	m_alg_props ;
	properties_view			m_view_props ;
	properties_qc			m_qc_props; 
	properties_loaded_history	m_history_props ;

	properties& operator=( const properties &rhs )
	{
		m_mem_props		= rhs.m_mem_props ;
		m_gloss_props	= rhs.m_gloss_props ;
		m_gen_props		= rhs.m_gen_props ;
		m_alg_props		= rhs.m_alg_props ;
		m_view_props	= rhs.m_view_props;
		m_qc_props		= rhs.m_qc_props;
		m_history_props		= rhs.m_history_props;

		return *this ;
	}
	bool read_from_registry();

	bool load_file(wstring filename);
	bool parse_xml_doc( pugi::xml_document &doc );
	
	bool write_to_registry();

	void save_prefs();
	void save_file(CString filename);

	void save_xml_doc( output_device_ptr output );

	void build_xml_doc(pugi::xml_document &doc);
} ;

typedef std::shared_ptr<properties> props_ptr ;

inline props_ptr get_props()
{
	static props_ptr props;
	if(! props)
	{
		props = props_ptr(new properties) ;
	}
	return props ;
}

} ;
