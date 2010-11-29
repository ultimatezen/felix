// rtf_preamble.h: interface for the rtf_preamble class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTF_PREAMBLE_H__EDE0DB11_CF3C_4ACE_B227_6A6C89AAB8FA__INCLUDED_)
#define AFX_RTF_PREAMBLE_H__EDE0DB11_CF3C_4ACE_B227_6A6C89AAB8FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StringEx.h"

namespace rtf
{
class rtf_preamble  
{
	int		m_default_lang ;
	int		m_default_asian_lang ;
	int		m_default_codepage ;
	
public:
	bool parse_element( const string &element );
	rtf_preamble();
	virtual ~rtf_preamble();

	int get_default_lang() 
	{
		return m_default_lang ;
	}
	int get_default_asian_lang()  
	{
		return m_default_asian_lang ;
	}
	int get_default_codepage()  
	{
		return m_default_codepage ;
	}

	void set_default_lang( const int setting ) 
	{
		m_default_lang = setting ;
	}
	void set_default_asian_lang( const int setting )  
	{
		m_default_asian_lang = setting ;
	}
	void set_default_codepage( const int setting )  
	{
		m_default_codepage = setting ;
	}

};

}

#endif // !defined(AFX_RTF_PREAMBLE_H__EDE0DB11_CF3C_4ACE_B227_6A6C89AAB8FA__INCLUDED_)
