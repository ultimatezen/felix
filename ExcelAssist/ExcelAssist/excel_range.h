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
		m_range->GetCharacters()->Text = text.bstrVal ;
	}
	bool is_hidden()
	{
		if (m_range->EntireColumn->Hidden)
		{
			return true ;
		}
		if (m_range->EntireRow->Hidden)
		{
			return true ;
		}
		return false ;
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

typedef std::shared_ptr< range > range_ptr ;
}