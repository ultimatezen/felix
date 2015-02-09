/*
 Interface to Excel COM server
 */

#pragma once

class ExcelInterface
{

public:

	virtual void SaveAs(const CString location) = 0 ;

	virtual void SetCellText(int row, int col, const wstring text) = 0 ;
	virtual void MakeCellBold(int row, int col) = 0 ;

};

typedef std::shared_ptr<ExcelInterface>	ExcelInterfacePtr ;



