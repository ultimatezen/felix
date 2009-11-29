#pragma once
#include "contentpresenter.h"
#include "record.h"

namespace content_presenter
{
	using namespace memory_engine ;
	using namespace html ;

class CContentPresenterAddedTrans :
	public CContentPresenter
{
	record_pointer m_rec ;

public:
	CContentPresenterAddedTrans(CHtmlView &html_view, record_pointer &rec );
	~CContentPresenterAddedTrans(void);
	void present(void);
	wstring generate_text() ;
};

}