#pragma once

#include "ExcelImport.h"
#include "dispatchwrapper.h"
#include "excel_characters.h"

namespace excel
{

class range
{
public:
	Excel::RangePtr m_range ;

	void set_range(Excel::RangePtr excel_range)
	{
		m_range = excel_range ;
	}

	virtual ~range()
	{

	}

	// interface functions
	virtual _variant_t GetFormula()
	{
		return m_range->FormulaR1C1 ;
	}
	virtual void SetFormula( _variant_t formula )
	{
		m_range->FormulaR1C1 = formula ;
	}
	virtual _variant_t GetText()
	{
		return m_range->Text ;
	}
	void SetText(const _variant_t text)
	{
		CDispatchWrapper selDisp = m_range ;
		Excel::CharactersPtr chars = selDisp.prop_get( L"Characters" ).pdispVal ;
		chars_ptr excel_chars(new characters) ;
		excel_chars->set_chars(chars) ;

		excel_chars->SetText(text.bstrVal) ;
	}
	virtual int Row()
	{
		return m_range->Row ;
	}
	virtual int Column()
	{
		return m_range->Column ;
	}
	virtual VARTYPE FormulaType()
	{
		return m_range->Formula.vt ;
	}
	virtual int FillColor()
	{
		return m_range->Interior->ColorIndex.intVal ;
	}
	virtual void SetFillColor( int color, bool is_solid=true )
	{
		m_range->Interior->ColorIndex = color ;

		if ( is_solid )
		{
			m_range->Interior->Pattern = Excel::xlSolid ;
		}
	}
	virtual void Select()
	{
		m_range->Select() ;
	}
};

typedef boost::shared_ptr< range > range_ptr ;
}