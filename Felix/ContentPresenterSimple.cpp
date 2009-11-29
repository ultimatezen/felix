#include "StdAfx.h"
#include ".\contentpresentersimple.h"

namespace content_presenter
{

CContentPresenterSimple::CContentPresenterSimple(CHtmlView &html_view, CString res_name) :
	CContentPresenter( html_view ),
	m_res_name( res_name )
{
}

CContentPresenterSimple::~CContentPresenterSimple(void)
{
}

void content_presenter::CContentPresenterSimple::present(void)
{
	m_view->load_from_resource( m_res_name ) ;
}

}