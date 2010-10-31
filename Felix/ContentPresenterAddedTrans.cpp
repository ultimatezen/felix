#include "StdAfx.h"
#include ".\contentpresenteraddedtrans.h"
#include "ResHtmlFile.h"
#include "text_templates.h"
#include "record_local.h"
#include "cpptempl.h"

namespace content_presenter
{

	using namespace cpptempl;

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

	data_map data ;
	data[L"source"] = make_data(m_rec->get_source_rich()) ;
	data[L"trans"] = make_data(m_rec->get_trans_rich()) ;

	return cpptempl::parse(tpl_text, data) ;
}

}
