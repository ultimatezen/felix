#pragma once

#include "ExcelInterface.h"

class ExcelInterfaceFake : public ExcelInterface
{
public:
	typedef std::pair<int, int> cellcoord ;
	typedef std::pair<cellcoord, wstring> celltext ;

	std::vector<cellcoord> m_bold_cells ;
	std::vector<celltext> m_cell_text ;

	CString m_location ;

	void SaveAs(const CString location)
	{
		m_location = location ;
	}

	void SetCellText(int row, int col, const wstring text)
	{
		m_cell_text.push_back(celltext(cellcoord(row, col), text)) ;
	}
	void MakeCellBold(int row, int col)
	{
		m_bold_cells.push_back(cellcoord(row, col)) ;
	}

};
