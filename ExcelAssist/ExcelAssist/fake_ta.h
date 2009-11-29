#pragma once
#include "ta_application.h"

class FakeFelix : public FelixApp
{
public:
	
	std::vector< string > m_calls ;
	_bstr_t m_trans ;
	_bstr_t m_query ;
	_bstr_t m_match ;
	double m_score ;

	FakeFelix():
		m_score(0.0)
		{

		}

	void SetQuery( _bstr_t query )
	{
		m_calls.push_back("SetQuery") ;
		m_calls.push_back(BSTR2string(query)) ;
		m_query = query ;
	}
	void LookupTrans( _bstr_t trans)
	{
		m_calls.push_back("LookupTrans") ;
		m_calls.push_back(BSTR2string(trans)) ;
		m_query = trans ;
	}
	void SetTrans( _bstr_t trans )
	{
		m_calls.push_back("SetTrans") ;
		m_calls.push_back(BSTR2string(trans)) ;
		m_trans = trans ;
	}
	void CorrectTrans( _bstr_t trans)
	{
		m_calls.push_back("CorrectTrans") ;
		m_calls.push_back(BSTR2string(trans)) ;
		m_trans = trans ;
	}
	_bstr_t GetTrans()
	{
		m_calls.push_back("GetTrans") ;
		return m_trans ;
	}

	double GetScore()
	{
		m_calls.push_back("GetScore") ;
		return m_score ;
	}

	_bstr_t GlossMatch( int num)
	{
		m_calls.push_back("GlossMatch") ;
		m_calls.push_back(int2string(num)) ;
		return m_match ;
	}

	void NextTrans()
	{
		m_calls.push_back("NextTrans") ;
	}
	void PrevTrans()
	{
		m_calls.push_back("PrevTrans") ;
	}

	void AddGlossaryEntry(_bstr_t source,
		_bstr_t trans,
		_bstr_t context)
	{
		m_calls.push_back("AddGlossaryEntry") ;
		m_calls.push_back(BSTR2string(source)) ;
		m_calls.push_back(BSTR2string(trans)) ;
		m_calls.push_back(BSTR2string(context)) ;
	}

	void AddMemoryEntry(_bstr_t source,
		_bstr_t trans,
		_bstr_t context)
	{
		m_calls.push_back("AddMemoryEntry") ;
		m_calls.push_back(BSTR2string(source)) ;
		m_calls.push_back(BSTR2string(trans)) ;
		m_calls.push_back(BSTR2string(context)) ;
	}
};