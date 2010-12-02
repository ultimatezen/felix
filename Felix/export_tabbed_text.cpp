#include "stdafx.h"
#include "export_tabbed_text.h"

namespace tabbed_export
{
	TabbedTextExporter::TabbedTextExporter( output_device_ptr output ) : m_output(output)
	{
		m_headers += "Source", "Trans", "Context", "Reliability",
			"Created", "Modified", "Verified";
	}

	void TabbedTextExporter::write_headers()
	{
		SENSE("write_headers") ;
		write_line(m_headers) ;
	}

	void TabbedTextExporter::write_line( std::vector<std::string> &cols )
	{
		string line = boost::join(cols, "\t") ;
		m_output->write(line) ;
		m_output->write("\r\n") ;
	}

	string TabbedTextExporter::massage_text_field( const wstring text )
	{
		string result = string2string(text) ;
		boost::replace_all(result, "\r\n", " ") ;
		boost::replace_all(result, "\r", " ") ;
		boost::replace_all(result, "\n", " ") ;
		boost::replace_all(result, "\t", " ") ;
		return result ;
	}

	void TabbedTextExporter::write_record( mem_engine::record_pointer rec )
	{
		SENSE("write_record") ;
		std::vector<std::string> items ;
		items.push_back(massage_text_field(rec->get_source_plain())) ;
		items.push_back(massage_text_field(rec->get_trans_plain())) ;
		items.push_back(massage_text_field(rec->get_context_plain())) ;
		items.push_back(ulong2string(rec->get_reliability())) ;
		items.push_back(string2string(rec->get_created().get_date_time_string())) ;
		items.push_back(string2string(rec->get_modified().get_date_time_string())) ;
		items.push_back(bool2string(rec->is_validated())) ;
		write_line(items) ;
	}

	void TabbedTextExporter::write_records( mem_engine::record_iterator begin, mem_engine::record_iterator end )
	{
		std::for_each(begin, end, boost::bind(&TabbedTextExporter::write_record, this, _1)) ;
	}

	void TabbedTextExporter::write_memory( mem_engine::record_iterator begin, mem_engine::record_iterator end )
	{
		write_headers() ;
		write_records(begin, end) ;
	}
}