#include "stdafx.h"
#include "Felix_properties.h"
#include "xml_writer.h"

#include "cstringinterface.h"
#include "input_device_file.h"
#include "logging.h"
#include "config_file.h"

const wchar_t* LOADED_HISTORY_FILENAME = L"felix_load_history.xml" ;
const wchar_t* PREFS_FILENAME = L"felix_prefs.fprefx" ;

void add_child(pugi::xml_node &node, string name, string value)
{
	pugi::xml_node child_node = node.append_child();
	child_node.set_name(name.c_str()) ;
	child_node.append_child(pugi::node_pcdata).set_value(value.c_str());
}

pugi::xml_node get_prop_node( pugi::xml_document &doc, string node_name ) 
{
	pugi::xml_node child = doc.child(node_name.c_str()) ;
	if (! child )
	{
		child = doc.child("properties").child(node_name.c_str()) ;
	}
	return child ;
}


namespace app_props
{

	void load_xml_props_type(pugi::xml_node &parent, std::vector<wstring> &items, string node_name)
	{
		items.clear() ;
		pugi::xml_node nodes = parent.child(node_name.c_str()) ;
		for (pugi::xml_node tm_node = nodes.child("filename"); 
			tm_node; 
			tm_node = tm_node.next_sibling("filename"))
		{
			items.push_back(string2wstring(tm_node.child_value())) ;
		}
	}

	string get_config_file_text(CString filename)
	{
		input_device_ptr input(new InputDeviceFile) ;
		output_device_ptr output(new OutputDeviceFile) ;
		return get_config_text(filename, output, input) ;
	}

	output_device_ptr get_config_output_device( CString filename ) 
	{
		output_device_ptr output(new OutputDeviceFile) ;
		CString config_filename = get_config_filename(filename, output) ;
		output->open(config_filename) ;
		return output ;
	}

	void write_xml_doc( pugi::xml_document &doc, output_device_ptr output )
	{
		xml_string_writer writer ;
		doc.save(writer) ;
		output->write(writer.result) ;
		output->close() ;
	}

	void write_filenames( pugi::xml_node &node, const std::vector<wstring> &filenames, string node_name ) 
	{
		pugi::xml_node files_node = node.append_child() ;
		files_node.set_name(node_name.c_str()) ;
		FOREACH(wstring filename, filenames)
		{
			add_child(files_node, "filename", string2string(filename)) ;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// properties_loaded_history
	//////////////////////////////////////////////////////////////////////////

	properties_loaded_history::properties_loaded_history()
	{
		m_get_config_text = boost::bind(get_config_file_text, LOADED_HISTORY_FILENAME) ;
	}

	bool properties_loaded_history::write_xml_props()
	{
		string text = make_xml_doc();
		write_xml_file(text);

		return true ;
	}

	void properties_loaded_history::write_xml_file( const string &text )
	{
		output_device_ptr output = get_config_output_device(LOADED_HISTORY_FILENAME) ;
		output->write(text) ;
		output->close() ;
	}

	string properties_loaded_history::make_xml_doc()
	{
		pugi::xml_document doc;
		pugi::xml_node preferences = doc.append_child() ;
		preferences.set_name("properties") ;
		build_xml_doc(preferences);
		xml_string_writer writer ;
		doc.save(writer) ;
		return writer.result ;
	}


	bool properties_loaded_history::load_xml_props()
	{
		string text = m_get_config_text() ;
		if (text.empty())
		{
			return false ;
		}
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load(text.c_str());
		if (result.status != pugi::status_ok)
		{
			logging::log_error("Failed to parse felix_load_history.xml") ;
			return false ;
		}
		return parse_xml_doc(doc);
	}

	bool properties_loaded_history::parse_xml_doc( pugi::xml_document &doc )
	{
		try
		{
			pugi::xml_node child = get_prop_node(doc, "loaded_history");
			load_xml_props_type(child, m_loaded_mems, "loaded_mems") ;
			load_xml_props_type(child, m_loaded_gloss, "loaded_gloss") ;
			load_xml_props_type(child, m_loaded_remote_mems, "loaded_remote_mems") ;
			load_xml_props_type(child, m_loaded_remote_gloss, "loaded_remote_gloss") ;

			this->m_memory_location = read_xml_string(child, "memory_location");
			this->m_glossary_location = read_xml_string(child, "glossary_location");
			this->m_preferences_location = read_xml_string(child, "preferences_location");
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception") ;
			logging::log_exception(e) ;
			return false ;
		}
		catch (std::exception& e)
		{
			logging::log_error("COM exception") ;
			logging::log_error(e.what()) ;
			return false ;
		}
		return true ;
	}


	bool properties_loaded_history::copy_reg_props()
	{
		m_loaded_mems.clear() ;
		m_loaded_gloss.clear() ;
		m_loaded_remote_mems.clear() ;
		m_loaded_remote_gloss.clear() ;

		for ( int i = 0 ; i < m_data.m_num_mems ; ++i )
		{
			m_loaded_mems.push_back(m_data.m_mems[i]) ;
		}
		for ( int i = 0 ; i < m_data.m_num_gloss ; ++i )
		{
			m_loaded_gloss.push_back(m_data.m_glosses[i]) ;
		}
		for ( int i = 0 ; i < m_data.m_num_remote_mems ; ++i )
		{
			m_loaded_remote_mems.push_back(m_data.m_remote_mems[i]) ;
		}
		for ( int i = 0 ; i < m_data.m_num_remote_gloss ; ++i )
		{
			m_loaded_remote_gloss.push_back(m_data.m_remote_glosses[i]) ;
		}
		return true ;
	}

	void properties_loaded_history::build_xml_doc( pugi::xml_node &prefs )
	{
		pugi::xml_node loaded_history = prefs.append_child();
		loaded_history.set_name("loaded_history") ;

		write_filenames(loaded_history, m_loaded_mems, "loaded_mems");
		write_filenames(loaded_history, m_loaded_gloss, "loaded_gloss");
		write_filenames(loaded_history, m_loaded_remote_mems, "loaded_remote_mems");
		write_filenames(loaded_history, m_loaded_remote_gloss, "loaded_remote_gloss");
		add_child(loaded_history, "memory_location", string2string(this->m_memory_location));
		add_child(loaded_history, "glossary_location", string2string(this->m_glossary_location));
		add_child(loaded_history, "preferences_location", string2string(this->m_preferences_location));
	}


	properties_loaded_history & properties_loaded_history::operator=( const properties_loaded_history &rhs )
	{
		m_data = rhs.m_data ;

		std::vector<wstring> mem_tmp(rhs.m_loaded_mems) ;
		m_loaded_mems.swap(mem_tmp) ;

		std::vector<wstring> gloss_tmp(rhs.m_loaded_gloss) ;
		m_loaded_gloss.swap(gloss_tmp) ;

		std::vector<wstring> remote_mem_tmp(rhs.m_loaded_remote_mems) ;
		m_loaded_remote_mems.swap(remote_mem_tmp) ;

		std::vector<wstring> remote_gloss_tmp(rhs.m_loaded_remote_gloss) ;
		m_loaded_remote_gloss.swap(remote_gloss_tmp) ;

		return *this ;
	}


	// properties_memory

	void properties_memory::build_xml_doc( pugi::xml_node &prefs )
	{
		pugi::xml_node memory_node = prefs.append_child();
		memory_node.set_name("properties_memory") ;

		add_child(memory_node, "min_score", ulong2string(this->m_data.m_min_score)) ;
		add_child(memory_node, "ignore_case", bool2string(!! this->m_data.m_ignore_case)) ;
		add_child(memory_node, "ignore_width", bool2string(!! this->m_data.m_ignore_width)) ;
		add_child(memory_node, "ignore_hir_kat", bool2string(!! this->m_data.m_ignore_hir_kat)) ;
		add_child(memory_node, "plaintext", bool2string(!! this->m_data.m_plaintext)) ;
		add_child(memory_node, "assess_format_penalty", bool2string(!! this->m_data.m_assess_format_penalty)) ;
		add_child(memory_node, "place_numbers", bool2string(!! this->m_data.m_place_numbers)) ;
		add_child(memory_node, "place_gloss", bool2string(!! this->m_data.m_place_gloss)) ;
		add_child(memory_node, "place_rules", bool2string(!! this->m_data.m_place_rules)) ;
		add_child(memory_node, "one_trans_per_source", bool2string(!! this->m_data.m_one_trans_per_source)) ;
	}

	bool properties_memory::parse_xml_doc( pugi::xml_document &doc )
	{
		try
		{
			pugi::xml_node parent = doc.child("properties").child("properties_memory") ;

			this->m_data.m_min_score = read_xml_ulong(parent, "min_score") ;

			// normalize
			this->m_data.m_ignore_case = read_xml_bool(parent, "ignore_case") ;
			this->m_data.m_ignore_width = read_xml_bool(parent, "ignore_width") ;
			this->m_data.m_ignore_hir_kat = read_xml_bool(parent, "ignore_hir_kat") ;

			// formatting
			this->m_data.m_plaintext = read_xml_bool(parent, "plaintext") ;
			this->m_data.m_assess_format_penalty = read_xml_bool(parent, "assess_format_penalty") ;

			// placements
			this->m_data.m_place_numbers = read_xml_bool(parent, "place_numbers") ;
			this->m_data.m_place_gloss = read_xml_bool(parent, "place_gloss") ;
			this->m_data.m_place_rules = read_xml_bool(parent, "place_rules") ;

			// one trans per source
			this->m_data.m_one_trans_per_source = read_xml_bool(parent, "one_trans_per_source") ;
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception") ;
			logging::log_exception(e) ;
			return false ;
		}
		catch (std::exception& e)
		{
			logging::log_error("COM exception") ;
			logging::log_error(e.what()) ;
			return false ;
		}
		return true ;
	}
	// properties_glossary

	void properties_glossary::build_xml_doc( pugi::xml_node &prefs )
	{
		pugi::xml_node parent = prefs.append_child();
		parent.set_name("properties_glossary") ;

		add_child(parent, "min_score", ulong2string(this->m_data.m_min_score)) ;
		add_child(parent, "max_add", ulong2string(this->m_data.m_max_add)) ;
		add_child(parent, "numbering", ulong2string(this->m_data.m_numbering)) ;
		add_child(parent, "back_color", ulong2string(this->m_data.m_back_color)) ;

		add_child(parent, "ignore_case", bool2string(!! this->m_data.m_ignore_case)) ;
		add_child(parent, "plaintext", bool2string(!! this->m_data.m_plaintext)) ;
		add_child(parent, "to_lower", bool2string(!! this->m_data.m_to_lower)) ;

		add_child(parent, "ignore_width", bool2string(!! this->m_data.m_ignore_width)) ;
		add_child(parent, "ignore_hir_kat", bool2string(!! this->m_data.m_ignore_hir_kat)) ;
		add_child(parent, "simple_view", bool2string(!! this->m_data.m_simple_view)) ;
	}

	bool properties_glossary::parse_xml_doc( pugi::xml_document &doc )
	{
		try
		{
			pugi::xml_node parent = doc.child("properties").child("properties_glossary") ;

			const string  min_score = parent.child("min_score").child_value() ;
			this->m_data.m_min_score = string2ulong(min_score) ;

			const string  max_add = parent.child("max_add").child_value() ;
			this->m_data.m_max_add = string2ulong(max_add) ;

			const string  back_color = parent.child("back_color").child_value() ;
			this->m_data.m_back_color = string2long(back_color) ;

			const string  numbering = parent.child("numbering").child_value() ;
			this->m_data.m_numbering = string2ulong(numbering) ;

			this->m_data.m_ignore_case = read_xml_bool(parent, "ignore_case") ;
			this->m_data.m_plaintext = read_xml_bool(parent, "plaintext") ;
			this->m_data.m_to_lower = read_xml_bool(parent, "to_lower") ;

			this->m_data.m_ignore_width = read_xml_bool(parent, "ignore_width") ;
			this->m_data.m_ignore_hir_kat = read_xml_bool(parent, "ignore_hir_kat") ;
			this->m_data.m_simple_view = read_xml_bool(parent, "simple_view") ;
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception parsing XML prefs") ;
			logging::log_exception(e) ;
			return false ;
		}
		catch (std::exception& e)
		{
			logging::log_error("COM exception parsing XML prefs") ;
			logging::log_error(e.what()) ;
			return false ;
		}
		return true ;
	}

	//////////////////////////////////////////////////////////////////////////
	// properties_algorithm
	//////////////////////////////////////////////////////////////////////////

	void properties_algorithm::build_xml_doc( pugi::xml_node &prefs )
	{
		pugi::xml_node parent = prefs.append_child();
		parent.set_name("properties_algorithm") ;

		add_child(parent, "match_algo", int2string(this->m_data.m_match_algo)) ;
	}

	bool properties_algorithm::parse_xml_doc( pugi::xml_document &doc )
	{
		try
		{
			pugi::xml_node parent = doc.child("properties").child("properties_algorithm") ;

			const string  match_algo = parent.child("match_algo").child_value() ;
			this->m_data.m_match_algo = string2long(match_algo) ;
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception parsing XML prefs") ;
			logging::log_exception(e) ;
			return false ;
		}
		catch (std::exception& e)
		{
			logging::log_error("COM exception parsing XML prefs") ;
			logging::log_error(e.what()) ;
			return false ;
		}
		return true ;
	}

	//////////////////////////////////////////////////////////////////////////
	// properties_view
	//////////////////////////////////////////////////////////////////////////

	void properties_view::build_xml_doc( pugi::xml_node &prefs )
	{
		pugi::xml_node parent = prefs.append_child();
		parent.set_name("properties_view") ;

		add_child(parent, "back_color", int2string(this->m_data.m_back_color)) ;
		add_child(parent, "query_color", int2string(this->m_data.m_query_color)) ;
		add_child(parent, "source_color", int2string(this->m_data.m_source_color)) ;
		add_child(parent, "trans_color", int2string(this->m_data.m_trans_color)) ;

		add_child(parent, "mem_mousewheel", int2string(this->m_data.m_mem_mousewheel)) ;
		add_child(parent, "gloss_mousewheel", int2string(this->m_data.m_gloss_mousewheel)) ;

		add_child(parent, "single_screen_matches", bool2string(!! this->m_data.m_single_screen_matches)) ;
		add_child(parent, "show_gloss_matches", bool2string(!! this->m_data.m_show_gloss_matches)) ;
	}

	bool properties_view::parse_xml_doc( pugi::xml_document &doc )
	{
		try
		{
			pugi::xml_node parent = doc.child("properties").child("properties_view") ;

			this->m_data.m_back_color = read_xml_long(parent, "back_color") ;
			this->m_data.m_query_color = read_xml_long(parent, "query_color") ;
			this->m_data.m_source_color = read_xml_long(parent, "source_color") ;
			this->m_data.m_trans_color = read_xml_long(parent, "trans_color") ;

			this->m_data.m_single_screen_matches = read_xml_bool(parent, "single_screen_matches") ;
			this->m_data.m_show_gloss_matches = read_xml_bool(parent, "show_gloss_matches") ;

			if (! parent.child("mem_mousewheel"))
			{
				logging::log_debug("properties_view: Key `mem_mousewheel` not found. Loading mousewheel preferences from old Props service.") ;
				load_mousewheel_props();
			}
			else
			{
				this->m_data.m_mem_mousewheel = clamp_mousewheel(read_xml_long(parent, "mem_mousewheel")) ;
				this->m_data.m_gloss_mousewheel = clamp_mousewheel(read_xml_long(parent, "gloss_mousewheel")) ;
			}
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception parsing XML prefs") ;
			logging::log_exception(e) ;
			return false ;
		}
		catch (std::exception& e)
		{
			logging::log_error("COM exception parsing XML prefs") ;
			logging::log_error(e.what()) ;
			return false ;
		}


		return true ;

	}

	void properties_view::load_mousewheel_props()
	{
#ifdef UNIT_TEST
		this->m_data.m_mem_mousewheel = -20 ;
		this->m_data.m_gloss_mousewheel = 20 ;
#else
		this->m_data.m_mem_mousewheel = clamp_mousewheel(this->m_data.m_mem_mousewheel);
		this->m_data.m_gloss_mousewheel = clamp_mousewheel(this->m_data.m_gloss_mousewheel);
#endif // UNIT_TEST
	}

	int properties_view::get_mem_mousewheel()
	{
		return m_data.m_mem_mousewheel ;
	}

	int properties_view::get_gloss_mousewheel()
	{
		return m_data.m_gloss_mousewheel ;
	}

	int properties_view::clamp_mousewheel( const int val )
	{
		return min(max(val, -10), 10);
	}

	//////////////////////////////////////////////////////////////////////////
	// properties_qc
	//////////////////////////////////////////////////////////////////////////

	void properties_qc::build_xml_doc( pugi::xml_node &prefs )
	{
		pugi::xml_node parent = prefs.append_child();
		parent.set_name("properties_qc") ;

		add_child(parent, "check_numbers", bool2string(!! this->m_data.m_check_numbers)) ;
		add_child(parent, "check_all_caps", bool2string(!! this->m_data.m_check_all_caps)) ;
		add_child(parent, "check_gloss", bool2string(!! this->m_data.m_check_gloss)) ;
		add_child(parent, "live_checking", bool2string(!! this->m_data.m_live_checking)) ;
		write_filenames(parent, m_qc_glosses, "qc_glosses");
	}

	bool properties_qc::parse_xml_doc( pugi::xml_document &doc )
	{
		try
		{
			pugi::xml_node parent = doc.child("properties").child("properties_qc") ;

			this->m_data.m_check_numbers = read_xml_bool(parent, "check_numbers") ;
			this->m_data.m_check_all_caps = read_xml_bool(parent, "check_all_caps") ;
			this->m_data.m_check_gloss = read_xml_bool(parent, "check_gloss") ;
			this->m_data.m_live_checking = read_xml_bool(parent, "live_checking") ;
			load_xml_props_type(parent, m_qc_glosses, "qc_glosses") ;
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception parsing XML prefs") ;
			logging::log_exception(e) ;
			return false ;
		}
		catch (std::exception& e)
		{
			logging::log_error("COM exception parsing XML prefs") ;
			logging::log_error(e.what()) ;
			return false ;
		}
		return true ;

	}

	//////////////////////////////////////////////////////////////////////////
	// properties_general
	//////////////////////////////////////////////////////////////////////////

	void properties_general::build_xml_doc( pugi::xml_node &prefs )
	{
		pugi::xml_node parent = prefs.append_child();
		parent.set_name("properties_general") ;

		add_child(parent, "window_size", int2string(this->m_data.m_window_size)) ;
		add_child(parent, "preferred_gui_lang", int2string(this->m_data.m_preferred_gui_lang)) ;
		add_child(parent, "merge_choice", int2string(this->m_data.m_merge_choice)) ;

		add_child(parent, "load_prev_mem_on_startup", bool2string(!! this->m_data.m_load_prev_gloss_on_startup)) ;
		add_child(parent, "load_prev_gloss_on_startup", bool2string(!! this->m_data.m_load_prev_gloss_on_startup)) ;
		add_child(parent, "show_markup", bool2string(!! this->m_data.m_show_markup)) ;
		add_child(parent, "first_launch", bool2string(!! this->m_data.m_first_launch)) ;

		add_child(parent, "query_merge", bool2string(!! this->m_data.m_query_merge)) ;
		add_child(parent, "old_mem_mgr", bool2string(!! this->m_data.m_old_mem_mgr)) ;
		add_child(parent, "must_login", bool2string(!!this->m_data.m_must_login));
		add_child(parent, "save_credentials", bool2string(!!this->m_data.m_save_credentials));

		add_child(parent, "user_name", static_cast<const char*>(CW2A(this->m_data.m_user_name))) ;

		if (! m_saved_credentials.empty())
		{
			serialize_credentials(parent);
		}

	}
	void properties_general::serialize_credentials(pugi::xml_node &parent)
	{
		pugi::xml_node credentials = parent.append_child();
		credentials.set_name("credentials");
		for(auto var: m_saved_credentials)
		{
			if (! var.first.empty() && ! var.second.empty())
			{
				logging::log_debug("saving credential: " + 
					string2string(var.first) + "::" + string2string(var.second));
				// credential
				auto credential = credentials.append_child();
				credential.set_name("credential");

				// connection
				auto connection = credential.append_child();
				connection.set_name("connection");
				string connection_value = string2string(var.first);
				connection.append_child(pugi::node_pcdata).set_value(connection_value.c_str());

				// username
				auto username = credential.append_child();
				username.set_name("username");
				string username_value = string2string(var.second);
				username.append_child(pugi::node_pcdata).set_value(username_value.c_str());
			}
		}
	}

	bool properties_general::parse_xml_doc( pugi::xml_document &doc )
	{
		try
		{
			pugi::xml_node parent = doc.child("properties").child("properties_general") ;

			this->m_data.m_window_size = read_xml_long(parent, "window_size") ;
			this->m_data.m_preferred_gui_lang = read_xml_long(parent, "preferred_gui_lang") ;
			this->m_data.m_merge_choice = read_xml_long(parent, "merge_choice") ;

			this->m_data.m_load_prev_mem_on_startup = read_xml_bool(parent, "load_prev_mem_on_startup") ;
			this->m_data.m_load_prev_gloss_on_startup = read_xml_bool(parent, "load_prev_gloss_on_startup") ;
			this->m_data.m_show_markup = read_xml_bool(parent, "show_markup") ;
			this->m_data.m_first_launch = read_xml_bool(parent, "first_launch") ;

			this->m_data.m_query_merge = read_xml_bool(parent, "query_merge") ;
			this->m_data.m_old_mem_mgr = read_xml_bool(parent, "old_mem_mgr") ;
			this->m_data.m_must_login = read_xml_bool(parent, "must_login") ;
			this->m_data.m_save_credentials= read_xml_bool(parent, "save_credentials");

			auto credentials = parent.child("credentials");
			if (credentials)
			{
				this->parse_credentials(credentials);
			}

			const wstring user_name = read_xml_string(parent, "user_name") ;
			_tcscpy_s( m_data.m_user_name, MAX_PATH, user_name.c_str()) ;
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception parsing XML prefs") ;
			logging::log_exception(e) ;
			return false ;
		}
		catch (std::exception& e)
		{
			logging::log_error("COM exception parsing XML prefs") ;
			logging::log_error(e.what()) ;
			return false ;
		}
		return true ;
	}

	void properties_general::parse_credentials(pugi::xml_node &credentials)
	{
		m_saved_credentials.clear();
		for (pugi::xml_node credential = credentials.child("credential");
			credential;
			credential = credential.next_sibling("credential"))
		{
			auto result = this->parse_credential(credential);
			if (!result.first.empty() && !result.second.empty())
			{
				m_saved_credentials[result.first] = result.second;
			}
		}
	}

	std::pair<wstring, wstring> properties_general::parse_credential(pugi::xml_node &credential)
	{
		pugi::xml_node connection_node = credential.child("connection");
		pugi::xml_node username_node = credential.child("username");

		if (!connection_node || !username_node)
		{
			logging::log_warn("Empty connection or username node in credential");
			return std::make_pair(wstring(), wstring());
		}

		return std::make_pair(
			string2wstring(connection_node.child_value()),
			string2wstring(username_node.child_value()));
	}

	//////////////////////////////////////////////////////////////////////////
	// properties
	//////////////////////////////////////////////////////////////////////////

	bool properties::read_from_registry()
	{
		output_device_ptr output(new OutputDeviceFile) ;
		wstring filename = get_config_filename(PREFS_FILENAME, output) ;
		if (this->load_file(filename))
		{
			return true ;
		}


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
		if ( ! m_qc_props.read_from_registry() )
		{
			retval = false ;
		}
		if ( ! m_history_props.read_from_registry() )
		{
			retval = false ;
		}
		return retval ;
	}

	bool properties::write_to_registry()
	{
		try
		{
			output_device_ptr output(new OutputDeviceFile) ;
			pugi::xml_document doc;
			pugi::xml_node preferences = doc.append_child() ;
			preferences.set_name("properties") ;

			m_mem_props.build_xml_doc(preferences) ;
			m_gloss_props.build_xml_doc(preferences) ;
			m_gen_props.build_xml_doc(preferences) ;
			m_alg_props.build_xml_doc(preferences) ;
			m_view_props.build_xml_doc(preferences) ;
			m_qc_props.build_xml_doc(preferences) ;
			m_history_props.build_xml_doc(preferences) ; 

			wstring config_filename = get_config_filename(PREFS_FILENAME, output) ;

			xml_string_writer writer ;
			doc.save(writer) ;
			string text = writer.result ;
			output->open(config_filename.c_str()) ;
			output->write(text) ;
			output->close() ;


			return true ;
		}
		catch (...)
		{
			logging::log_error("Exception reading prefs from file") ;
		}

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
		if ( ! m_qc_props.write_to_registry() )
		{
			retval = false ;
		}
		if ( ! m_history_props.write_to_registry() )
		{
			retval = false ;
		}
		return retval ;
	}

	bool properties::parse_xml_doc( pugi::xml_document &doc )
	{
		bool result = m_mem_props.parse_xml_doc(doc) ;

		if (!m_gloss_props.parse_xml_doc(doc))
		{
			result = false ;
		}

		if (!m_gen_props.parse_xml_doc(doc))
		{
			result = false ;
		}

		if (!m_alg_props.parse_xml_doc(doc))
		{
			result = false ;
		}

		if (!m_view_props.parse_xml_doc(doc))
		{
			result = false ;
		}
		if (!m_qc_props.parse_xml_doc(doc))
		{
			result = false ;
		}
		if (!m_history_props.parse_xml_doc(doc))
		{
			result = false ;
		}

		return result ;
	}

	bool properties::load_file(wstring filename )
	{
		input_device_ptr input(new InputDeviceFile) ;
		string text = get_file_text(filename.c_str(), input) ;
		if (text.empty())
		{
			return false ;
		}
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load(text.c_str());
		if (result.status != pugi::status_ok)
		{
			logging::log_error("Failed to parse Felix preferences file: " + string2string(filename)) ;
			return false ;
		}
		return this->parse_xml_doc(doc) ;
	}

	void properties::save_prefs()
	{
		output_device_ptr output = get_config_output_device(PREFS_FILENAME);
		this->save_xml_doc(output);
	}

	void properties::save_file( CString filename )
	{
		output_device_ptr output(new OutputDeviceFile) ;
		output->open(filename) ;
		this->save_xml_doc(output) ;
	}

	void properties::save_xml_doc( output_device_ptr output )
	{
		pugi::xml_document doc;
		build_xml_doc(doc);
		write_xml_doc(doc, output);
	}
	void properties::build_xml_doc( pugi::xml_document &doc )
	{
		pugi::xml_node preferences = doc.append_child() ;
		preferences.set_name("properties") ;

		m_mem_props.build_xml_doc(preferences) ;
		m_gloss_props.build_xml_doc(preferences) ;
		m_gen_props.build_xml_doc(preferences) ;
		m_alg_props.build_xml_doc(preferences) ;
		m_view_props.build_xml_doc(preferences) ;
		m_qc_props.build_xml_doc(preferences) ;
		m_history_props.build_xml_doc(preferences) ;
	}



}
