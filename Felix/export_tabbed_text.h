#pragma once

#include "record.h"
#include "output_device.h"
#include "TranslationMemory.h"

namespace tabbed_export
{

	class TabbedTextExporter
	{

		output_device_ptr m_output ;
		std::vector<std::string> m_headers ;

	public:
		DECLARE_SENSING_VAR ;

		TabbedTextExporter(output_device_ptr output);
		void write_memory(mem_engine::record_iterator begin, mem_engine::record_iterator end);
		void write_headers();
		void write_line(std::vector<std::string> &cols);
		string massage_text_field(const wstring text);
		void write_record(mem_engine::record_pointer rec);
		void write_records(mem_engine::record_iterator begin, mem_engine::record_iterator end);
		
	};

}