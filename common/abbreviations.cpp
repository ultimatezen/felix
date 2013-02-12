#include "stdafx.h"
#include "abbreviations.h"

void Abbreviations::load(const wstring &text)
{
	std::vector<wstring> tokens ;
	boost::split(tokens, text, boost::is_space()) ;
	if (! tokens.empty())
	{
		m_abbreviations.clear() ;
		FOREACH(wstring token, tokens)
		{
			if (! token.empty())
			{
				m_abbreviations.insert(token) ;
			}
		}
	}
}

Abbreviations::Abbreviations()
{
	std::vector<wstring> tmp ;
	tmp += L"Mr.", L"Mrs.", L"Ms.", L"Dr.", L"e.g.", L"i.e." ;
	std::copy(tmp.begin(), tmp.end(), std::inserter(m_abbreviations, m_abbreviations.begin())) ;
}

