#pragma once

#include "file.h"

class OutputDevice
{
public:
	virtual void write(const char* text, size_t len) = 0 ;
	virtual void write(const string &text) = 0 ;
	virtual void write(const wstring text) = 0 ;
	virtual void close() = 0 ;
	virtual void open(const CString &filename) = 0 ;
};
typedef boost::shared_ptr<OutputDevice> output_device_ptr  ;

class OutputDeviceFile : public OutputDevice
{
public:
	file::file m_file ;
	void open(const CString &filename)
	{
		m_file.open_always(filename) ;
	}
	void write(const char* text, size_t len)
	{
		ATLASSERT(m_file.is_open()) ;
		m_file.write(text, len) ;
	}
	void write(const string &text)
	{
		ATLASSERT(m_file.is_open()) ;
		m_file.write(text) ;
	}
	void write(const wstring text)
	{
		ATLASSERT(m_file.is_open()) ;
		m_file.write(text) ;
	}
	void close()
	{
		if (m_file.is_open())
		{
			m_file.write_eof() ;
			m_file.close() ;
		}
		ATLASSERT ( ! m_file.is_open() ) ; 
	}
};