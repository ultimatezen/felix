#include "stdafx.h"
#include "Felix_properties.h"


#include "cstringinterface.h"
#include "input_device_file.h"
#include "logging.h"

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
	bool properties_loaded_history::write_xml_props()
	{
		output_device_ptr output(new OutputDeviceFile) ;
		write_xml_file(output);
		return true ;
	}

	void properties_loaded_history::write_xml_file( output_device_ptr output )
	{
		wstring config_filename = get_config_filename(_T("felix_load_history.xml"), output) ;

		string text = get_xml_doc();

		output->open(config_filename.c_str()) ;
		output->write(text) ;
		output->close() ;
	}
	string properties_loaded_history::get_xml_doc()
	{
		pugi::xml_document doc;
		pugi::xml_node loaded_history = doc.append_child();
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
		xml_string_writer writer ;
		doc.save(writer) ;
		return writer.result ;
	}
	void properties_loaded_history::load_xml_props_type(pugi::xml_document &doc, std::vector<wstring> &items, string node_name)
	{
		items.clear() ;
		pugi::xml_node nodes = doc.child("loaded_history").child(node_name.c_str()) ;
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
		pugi::xml_node nodes = doc.child("loaded_mems") ;

		load_xml_props_type(doc, m_loaded_mems, "loaded_mems") ;
		load_xml_props_type(doc, m_loaded_gloss, "loaded_gloss") ;
		load_xml_props_type(doc, m_loaded_remote_mems, "loaded_remote_mems") ;
		load_xml_props_type(doc, m_loaded_remote_gloss, "loaded_remote_gloss") ;
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

}
