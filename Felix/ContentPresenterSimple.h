#pragma once
#include "contentpresenter.h"

namespace content_presenter
{

class CContentPresenterSimple :
	public CContentPresenter
{
	CString m_res_name ;

public:
	CContentPresenterSimple(CHtmlView &html_view, const CString res_name );
	~CContentPresenterSimple(void);
	void present(void);
};

}
