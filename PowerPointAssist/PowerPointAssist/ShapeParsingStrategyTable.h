#pragma once
#include "shapeparsingstrategy.h"

class CShapeParsingStrategyTable :
	public CShapeParsingStrategy
{
	PowerPoint::TablePtr	m_table ;
	long					m_currentRow ;
	long					m_currentCol ;

public:
	CShapeParsingStrategyTable(void);
	~CShapeParsingStrategyTable(void);

	PowerPoint::TextRangePtr nextTextRange( ) ;
	void SetShape( PowerPoint::ShapePtr shape );

	bool advance_cell_pos();
	bool is_exhausted();
	bool current_cell_exhausted();
public:

	void get_selected_table_pos();
};
