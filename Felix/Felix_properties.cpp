#include "stdafx.h"
#include "Felix_properties.h"


#include "cstringinterface.h"
#include "input_device_file.h"
#include "logging.h"

const wchar_t* LOADED_HISTORY_FILENAME = L"felix_load_history.xml" ;
const wchar_t* PREFS_FILENAME = L"felix_prefs.fprefx" ;

string get_file_text(wstring filename, input_device_ptr input) ;

struct xml_string_writer: pugi::xml_writer
{
	std::string result;

	virtual void write(const void* data, size_t size)
	{
		result += std::string(static_cast<const char*>(data), size);
	}
};
wstring get_config_filename(wstring filename, output_device_ptr output)
{
	TCHAR szPath[MAX_PATH];
	HRESULT hr = SHGetFolderPath(NULL, // hwndOwner
		CSIDL_LOCAL_APPDATA,		  // nFolder
		(HANDLE)NULL,				  // hToken (-1 means "default user")
		SHGFP_TYPE_CURRENT,			  // dwFlags 
		szPath) ;
	if (! SUCCEEDED(hr))
	{
		logging::log_error("Failed to get local app data folder") ;
		except::CComException e(hr) ;
		logging::log_exception(e) ;
		return wstring();
	}

	file::CPath pathname(CString(static_cast<LPCTSTR>(szPath))) ;
	pathname.Append(_T("Felix")) ;
	pathname.Append(_T("prefs")) ;
	output->ensure_dirs(pathname.Path()) ;
	pathname.Append(filename.c_str()) ;
	return wstring(static_cast<LPCTSTR>(pathname.Path())) ;
}
string get_config_text(wstring filename, output_device_ptr output, input_device_ptr input)
{
	wstring config_filename = get_config_filename(filename, output) ;
	return get_file_text(config_filename, input) ;
}
string get_file_text(wstring filename, input_device_ptr input)
{
	if (! input->exists(filename.c_str()))
	{
		return string() ;
	}
	try
	{
		return string(input->create_view_const_char(filename.c_str())) ;
	}
	catch (except::CException& e)
	{
		logging::log_error("Program exception: configuration file could not be read") ;
		logging::log_exception(e) ;
		return string() ;
	}
}
void add_child(pugi::xml_node &node, string name, string value)
{
	pugi::xml_node child_node = node.append_child();
	child_node.set_name(name.c_str()) ;
	child_node.append_child(pugi::node_pcdata).set_value(value.c_str());
}



namespace app_props
{
	// read xml notes
	BOOL read_xml_bool(pugi::xml_node &node, string name)
	{
		const string val = node.child(name.c_str()).child_value() ;
		return val == "true" ? TRUE : FALSE ;
	}
	long read_xml_long(pugi::xml_node &node, string name)
	{
		const string val = node.child(name.c_str()).child_value() ;
		return string2long(val) ;
	}
	unsigned long read_xml_ulong(pugi::xml_node &node, string name)
	{
		const string val = node.child(name.c_str()).child_value() ;
		return string2ulong(val) ;
	}
	wstring read_xml_string(pugi::xml_node &node, string name)
	{
		const string val = node.child(name.c_str()).child_value() ;
		return string2wstring(val) ;
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

	// write xml nodes
	void write_xml_bool(pugi::xml_node &node, string name, BOOL val)
	{
		pugi::xml_node child_node = node.append_child();
		child_node.set_name(name.c_str()) ;
		child_node.append_child(pugi::node_pcdata).set_value(val ? "true" : "false");
	}
	void write_xml_long(pugi::xml_node &node, string name, long val)
	{
		pugi::xml_node child_node = node.append_child();
		child_node.set_name(name.c_str()) ;
		child_node.append_child(pugi::node_pcdata).set_value(int2string(val).c_str());
	}
	void write_xml_ulong(pugi::xml_node &node, string name, unsigned long val)
	{
		pugi::xml_node child_node = node.append_child();
		child_node.set_name(name.c_str()) ;
		child_node.append_child(pugi::node_pcdata).set_value(ulong2string(val).c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	// properties_loaded_history
	//////////////////////////////////////////////////////////////////////////
	bool properties_loaded_history::write_xml_props()
	{
		output_device_ptr output(new OutputDeviceFile) ;
		write_xml_file(output);
		return true ;
	}

	void properties_loaded_history::write_xml_file( output_device_ptr output )
	{
		wstring config_filename = get_config_filename(LOADED_HISTORY_FILENAME, output) ;

		string text = make_xml_doc();

		output->open(config_filename.c_str()) ;
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
	void properties_loaded_history::load_xml_props_type(pugi::xml_node &parent, std::vector<wstring> &items, string node_name)
	{
		items.clear() ;
		pugi::xml_node nodes = parent.child(node_name.c_str()) ;
		for (pugi::xml_node tm_node = nodes.child("filename"); 
			tm_node; 
			tm_node = tm_node.next_sibling("filename"))
		{
			items.push_back(string2wstring(tm_node.child_value(), CP_UTF8)) ;
		}
	}

	bool properties_loaded_history::load_xml_props()
	{
		input_device_ptr input(new InputDeviceFile) ;
		output_device_ptr output(new OutputDeviceFile) ;
		string text = get_config_text(_T("felix_load_history.xml"), output, input) ;
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

		pugi::xml_node mem_node = loaded_history.append_child() ;
		mem_node.set_name("loaded_mems") ;
		foreach(wstring filename, m_loaded_mems)
		{
			add_child(mem_node, "filename", string2string(filename, CP_UTF8)) ;
		}
		pugi::xml_node gloss_node = loaded_history.append_child() ;
		gloss_node.set_name("loaded_gloss") ;
		foreach(wstring filename, m_loaded_gloss)
		{
			add_child(gloss_node, "filename", string2string(filename, CP_UTF8)) ;
		}
		pugi::xml_node rmem_node = loaded_history.append_child() ;
		rmem_node.set_name("loaded_remote_mems") ;
		foreach(wstring filename, m_loaded_remote_mems)
		{
			add_child(rmem_node, "filename", string2string(filename, CP_UTF8)) ;
		}
		pugi::xml_node rgloss_node = loaded_history.append_child() ;
		rgloss_node.set_name("loaded_remote_gloss") ;
		foreach(wstring filename, m_loaded_remote_gloss)
		{
			add_child(rgloss_node, "filename", string2string(filename, CP_UTF8)) ;
		}
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

	bool properties_memory::write_xml_props()
	{
		return false ;
	}

	bool properties_memory::copy_reg_props()
	{
		return false ;
	}

	bool properties_memory::load_xml_props()
	{
		return false ;
	}

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
	bool properties_glossary::load_xml_props()
	{
		return false ;
	}

	bool properties_glossary::write_xml_props()
	{
		return false ;
	}

	bool properties_glossary::copy_reg_props()
	{
		return false ;
	}

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
	// properties_algorithm

	bool properties_algorithm::write_xml_props()
	{
		return false ;
	}

	bool properties_algorithm::copy_reg_props()
	{
		return false ;
	}

	bool properties_algorithm::load_xml_props()
	{
		return false ;
	}

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
	// properties_view
	bool properties_view::write_xml_props()
	{
		return false ;
	}

	bool properties_view::copy_reg_props()
	{
		return false ;
	}

	bool properties_view::load_xml_props()
	{
		return false ;
	}

	void properties_view::build_xml_doc( pugi::xml_node &prefs )
	{
		pugi::xml_node parent = prefs.append_child();
		parent.set_name("properties_view") ;

		add_child(parent, "back_color", int2string(this->m_data.m_back_color)) ;
		add_child(parent, "query_color", int2string(this->m_data.m_query_color)) ;
		add_child(parent, "source_color", int2string(this->m_data.m_source_color)) ;
		add_child(parent, "trans_color", int2string(this->m_data.m_trans_color)) ;

		add_child(parent, "single_screen_matches", bool2string(!! this->m_data.m_single_screen_matches)) ;
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
	// properties_qc
	bool properties_qc::write_xml_props()
	{
		return false ;
	}

	bool properties_qc::copy_reg_props()
	{
		return false ;
	}
	bool properties_qc::load_xml_props()
	{
		return false ;
	}

	void properties_qc::build_xml_doc( pugi::xml_node &prefs )
	{
		pugi::xml_node parent = prefs.append_child();
		parent.set_name("properties_qc") ;

		add_child(parent, "check_numbers", bool2string(!! this->m_data.m_check_numbers)) ;
		add_child(parent, "check_all_caps", bool2string(!! this->m_data.m_check_all_caps)) ;
		add_child(parent, "check_gloss", bool2string(!! this->m_data.m_check_gloss)) ;
		add_child(parent, "live_checking", bool2string(!! this->m_data.m_live_checking)) ;
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
	bool properties_general::write_xml_props()
	{
		return false ;
	}

	bool properties_general::copy_reg_props()
	{
		return false ;
	}
	bool properties_general::load_xml_props()
	{
		return false ;
	}

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


		add_child(parent, "user_name", static_cast<const char*>(CW2A(this->m_data.m_user_name))) ;

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

	// properties
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
		bool result = true ;

		result = m_mem_props.parse_xml_doc(doc) ;

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
		string text = get_file_text(filename, input) ;
		if (text.empty())
		{
			return false ;
		}
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load(text.c_str());
		if (result.status != pugi::status_ok)
		{
			logging::log_error("Failed to parse felix preferences file: " + string2string(filename)) ;
			return false ;
		}
		return this->parse_xml_doc(doc) ;
	}

	void properties::save_file( CString filename )
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

		xml_string_writer writer ;
		doc.save(writer) ;
		string text = writer.result ;
		output->open(filename) ;
		output->write(text) ;
		output->close() ;
	}

	void properties::write_prefs()
	{
		output_device_ptr output(new OutputDeviceFile) ;
		wstring config_filename = get_config_filename(PREFS_FILENAME, output) ;

		this->save_file(config_filename.c_str()) ;
	}
}
