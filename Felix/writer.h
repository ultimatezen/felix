#pragma once

#include "file.h"

class Writer
{
public:
	virtual void write(string value) = 0 ;
};

class FileWriter : public Writer
{
	file::file m_file ;
public:
	void open(CString filename)
	{
		m_file.open(filename) ;
	}
	void write(string value)
	{
		m_file.write(value) ;
	}
};

class FakeWriter : public Writer
{
public:
	std::vector<string> values ;
	void write(string value)
	{
		values += value ;
	}
};