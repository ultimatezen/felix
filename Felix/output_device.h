#pragma once

#include "file.h"

class OutputDevice
{
public:
	virtual void write(const char* text, size_t len) = 0 ;
	virtual void write(const string &text) = 0 ;
	virtual void write(const wstring &text) = 0 ;
	virtual void close() {}
};

class OutputDeviceFile : public OutputDevice
{
public:
	file::file m_file ;
	void open(const CString &filename)
	{
		m_file.open(filename) ;
	}
	void write(const char* text, size_t len)
	{
		m_file.write(text, len) ;
	}
	void write(const string &text)
	{
		m_file.write(text) ;
	}
	void write(const wstring &text)
	{
		m_file.write(text) ;
	}
	void close()
	{
		m_file.write_eof() ;
		m_file.close() ;
	}
};