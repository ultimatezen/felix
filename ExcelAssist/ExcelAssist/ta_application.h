#pragma once

#import "C:\dev\cpp\Assistant Suite\Felix\Unicode_Release\Felix.tlb"\
	rename("TranslateAccelerator", "FelixTranslateAccelerator")

class FelixApp
{
public:
	Felix::IAppPtr m_app ;

	// To allow subclassing
	virtual ~FelixApp()
	{

	}

	void set_app(Felix::IAppPtr app)
	{
		m_app = app ;
	}

	virtual void SetQuery( _bstr_t query )
	{
		m_app->Query = query ;
	}
	virtual void LookupTrans( _bstr_t trans)
	{
		m_app->LookupTrans(trans) ;
	}
	virtual void SetTrans( _bstr_t trans )
	{
		m_app->Trans = trans ;
	}
	virtual void CorrectTrans( _bstr_t trans)
	{
		m_app->CorrectTrans(trans) ;
	}
	virtual _bstr_t GetTrans()
	{
		return m_app->Trans ;
	}

	virtual double GetScore()
	{
		return m_app->Score ;
	}

	virtual _bstr_t GlossMatch( int num)
	{
		return m_app->GlossMatch[ static_cast<short>(num) ] ;
	}

	virtual void NextTrans()
	{
		m_app->NextTrans() ;
	}
	virtual void PrevTrans()
	{
		m_app->PrevTrans() ;
	}

	virtual void AddGlossaryEntry(const _bstr_t source,
								  const _bstr_t trans,
								  const _bstr_t context)
	{
		m_app->AddGlossaryEntry(source, trans, context) ;
	}

	virtual void AddMemoryEntry(const _bstr_t source,
		const _bstr_t trans,
		const _bstr_t context)
	{
		m_app->AddMemoryEntry(source, trans, context) ;
	}
};

typedef boost::shared_ptr< FelixApp > ta_ptr ;