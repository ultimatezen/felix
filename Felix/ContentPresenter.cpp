#include "StdAfx.h"
#include ".\contentpresenter.h"

namespace content_presenter
{

CContentPresenter::CContentPresenter(CHtmlView &html_view) :
	m_view( &html_view )
{
}

CContentPresenter::~CContentPresenter(void)
{
}

}
