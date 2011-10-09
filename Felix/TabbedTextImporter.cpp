#include "stdafx.h"
#include "TabbedTextImporter.h"

#include "record.h"
#include "record_local.h"
#include "input_device_file.h"

CTabbedTextImporter::CTabbedTextImporter(CProgressListener *listener,
										 app_props::props_ptr props) :
	m_listener(listener),
	m_memory(new mem_engine::memory_local(props))
{
}

CTabbedTextImporter::~CTabbedTextImporter(void)
{
}

void CTabbedTextImporter::add_record( const wstring line )
{
	std::vector<wstring> items ;
	boost::split(items, line, boost::is_any_of(L"\t")) ;

	if (items.size() < 2)
	{
		return ;
	}

	mem_engine::record_pointer record(new mem_engine::record_local) ;

	record->set_source( items[0] ) ;
	record->set_trans( items[1] ) ;

	if (items.size() > 2)
	{
		record->set_context( items[2] ) ;
	}

	m_memory->add_record( record ) ;
}

void CTabbedTextImporter::add_records( const wstring text )
{
	std::vector<wstring> lines ;
	boost::split(lines, text, boost::is_any_of(L"\r\n")) ;

	m_listener->OnProgressInit( _T("text file"), 
		0, 
		lines.size() ) ;

	foreach(wstring line, lines)
	{
		this->add_record(line) ;
		m_listener->OnProgressLoadUpdate( m_memory->size() ) ;
	}
	m_listener->OnProgressDoneLoad(m_memory->size()) ;
}

void CTabbedTextImporter::load_file( const CString filename )
{
	input_device_ptr input(new InputDeviceFile) ;
	file::file::BYTE_ORDER_MARK bom = input->get_file_bom(filename) ;
	const size_t fsize = input->get_size(filename) ;
	LPSTR raw_text = (LPSTR)input->create_view_char(filename) ;
	UINT encoding = get_textfile_encoding(bom, raw_text, fsize) ;

	if (encoding == CP_UNICODE)
	{
		add_records(wstring(reinterpret_cast<wchar_t*>(raw_text), fsize/2)) ;
	}
	else
	{
		CStringW wide_buffer ;
		const UINT len_needed = ::MultiByteToWideChar( encoding, NULL, raw_text, fsize, NULL, 0 ) ;

		// we need to create a new buffer here, because we may be loading
		// more than one memory at the same time.
		// If we do that, then we will wreck our current buffer, causing all kinds of havoc!!!
		LPWSTR dest = wide_buffer.GetBufferSetLength( len_needed+1 ) ;
		::MultiByteToWideChar( encoding, NULL, raw_text, fsize, dest, len_needed+1 ) ;

		// MultiByteToWideChar doesn't null-terminate the string
		wide_buffer.GetBuffer()[len_needed] = 0 ;
		wide_buffer.ReleaseBuffer(len_needed) ;

		add_records(wstring(dest)) ;
	}
}