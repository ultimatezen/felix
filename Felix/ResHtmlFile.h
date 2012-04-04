#pragma once

class CResHtmlFile
{
	CStringW m_file_text ;
public:
	CResHtmlFile(LPCTSTR res_name, UINT codepage = CP_UTF8 );
	~CResHtmlFile(void);
	operator CStringW& () { return m_file_text ; }
	CStringW text() { return m_file_text ; }
};
