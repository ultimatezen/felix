#include "stdafx.h"
#include "Felix_properties.h"


#include "cstringinterface.h"
#include "input_device_file.h"
#include "logging.h"

const wchar_t* LOADED_HISTORY_FILENAME = L"felix_load_history.xml" ;
const wchar_t* PREFS_FILENAME = L"felix_prefs.xml" ;

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
	if (! input->exists(config_filename.c_str()))
	{
		return string() ;
	}
	try
	{
		return string(input->create_view_const_char(config_filename.c_str())) ;
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
	// properties_loaded_history
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
		preferences.set_name("preferences") ;
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
		parse_xml_doc(doc);

		return true ;
	}

	void properties_loaded_history::parse_xml_doc( pugi::xml_document &doc )
	{
		pugi::xml_node child = doc.child("loaded_history") ;
		if (! child )
		{
			child = doc.child("properties").child("loaded_history") ;
		}
		load_xml_props_type(child, m_loaded_mems, "loaded_mems") ;
		load_xml_props_type(child, m_loaded_gloss, "loaded_gloss") ;
		load_xml_props_type(child, m_loaded_remote_mems, "loaded_remote_mems") ;
		load_xml_props_type(child, m_loaded_remote_gloss, "loaded_remote_gloss") ;
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
		pugi::xml_node parent = prefs.append_child();
		parent.set_name("properties_memory") ;

	}

	void properties_memory::parse_xml_doc( pugi::xml_document &doc )
	{
		pugi::xml_node parent = doc.child("properties").child("properties_memory") ;

		const string  min_score = parent.child("min_score").child_value() ;
		this->m_data.m_min_score = string2ulong(min_score) ;

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

	}

	void properties_glossary::parse_xml_doc( pugi::xml_document &doc )
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

	}

	void properties_algorithm::parse_xml_doc( pugi::xml_document &doc )
	{
		pugi::xml_node parent = doc.child("properties").child("properties_algorithm") ;

		const string  match_algo = parent.child("match_algo").child_value() ;
		this->m_data.m_match_algo = string2long(match_algo) ;
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

	}

	void properties_view::parse_xml_doc( pugi::xml_document &doc )
	{
		pugi::xml_node parent = doc.child("properties").child("properties_view") ;

		this->m_data.m_back_color = read_xml_long(parent, "back_color") ;
		this->m_data.m_query_color = read_xml_long(parent, "query_color") ;
		this->m_data.m_source_color = read_xml_long(parent, "source_color") ;
		this->m_data.m_trans_color = read_xml_long(parent, "trans_color") ;

		this->m_data.m_single_screen_matches = read_xml_bool(parent, "single_screen_matches") ;

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

	}

	void properties_qc::parse_xml_doc( pugi::xml_document &doc )
	{
		pugi::xml_node parent = doc.child("properties").child("properties_qc") ;

		this->m_data.m_check_numbers = read_xml_bool(parent, "check_numbers") ;
		this->m_data.m_check_all_caps = read_xml_bool(parent, "check_all_caps") ;
		this->m_data.m_check_gloss = read_xml_bool(parent, "check_gloss") ;
		this->m_data.m_live_checking = read_xml_bool(parent, "live_checking") ;

	}
	// properties_general
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

	}

	void properties_general::parse_xml_doc( pugi::xml_document &doc )
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

		const string user_name = parent.child("user_name").child_value() ;
		_tcscpy_s( m_data.m_user_name, MAX_PATH, string2wstring(user_name).c_str()) ;
	}

	// properties
	bool properties::read_from_registry()
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
}
