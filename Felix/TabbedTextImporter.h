#pragma once

#include "TranslationMemory.h"
#include "memory_local.h"
#include "record_local.h"
#include "ScintillaEdit.h"
#include "ImportMultitermFile.h"

class CTabbedTextImporter
{
	CProgressListener *m_listener ;
	app_props::props_ptr		m_props ;

public:
	mem_engine::memory_pointer	   m_memory ;

	CTabbedTextImporter(CProgressListener *listener,
						app_props::props_ptr props);
	~CTabbedTextImporter(void);

	void add_record(const wstring line);
	void add_records(const wstring text);
	void load_file(const CString filename);
};