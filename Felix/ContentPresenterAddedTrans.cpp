#include "StdAfx.h"
#include ".\contentpresenteraddedtrans.h"
#include "ResHtmlFile.h"
#include "text_templates.h"
#include "TextTemplate.h"

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
	wstring body_text = generate_text() ;

#ifndef UNIT_TEST
	m_view->set_body_text( body_text ) ;
#endif
}
wstring CContentPresenterAddedTrans::generate_text()
{
	wstring tpl_text = get_template_text("match_new.txt") ;
	CTextTemplate engine ;

	engine.Assign(L"source", m_rec->get_source_rich()) ;
	engine.Assign(L"trans", m_rec->get_trans_rich()) ;

	return engine.Fetch(tpl_text) ;
}

}