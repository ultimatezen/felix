#include "StdAfx.h"
#include ".\contentpresenteraddedtrans.h"
#include "ResHtmlFile.h"
#include "text_templates.h"
#include "TextTemplate.h"
#include "record_local.h"

namespace content_presenter
{


CContentPresenterAddedTrans::CContentPresenterAddedTrans(CHtmlView &html_view, record_pointer &rec ) :
	CContentPresenter(html_view),
	m_rec(rec)
{
}

CContentPresenterAddedTrans::~CContentPresenterAddedTrans(void)
{
	m_rec = record_pointer(new record_local()) ;
}
void CContentPresenterAddedTrans::present()
{
	const wstring body_text = generate_text() ;

	m_view->set_body_text( body_text ) ;
}
wstring CContentPresenterAddedTrans::generate_text()
{
	const wstring tpl_text = get_template_text("match_new.txt") ;

	text_template::CTextTemplate engine ;
	engine.Assign(L"source", m_rec->get_source_rich()) ;
	engine.Assign(L"trans", m_rec->get_trans_rich()) ;

	return engine.Fetch(tpl_text) ;
}

}
