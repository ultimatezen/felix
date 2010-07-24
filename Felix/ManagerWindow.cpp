#include "stdafx.h"
#include "ManagerWindow.h"
#include "RegSettings.h"
#include "background_processor.h"
#include "text_templates.h"
#include "EditTransRecordDialogModal.h"
#include "replacer.h"
#include "record_local.h"
#include "WebPage.h"
#include "numberfmt.h"
#include "system_message.h"

#ifdef UNIT_TEST
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#endif

using namespace mem_engine ;
using namespace text_tmpl ;


LRESULT CManagerWindow::OnCreate( UINT, WPARAM, LPARAM )
{
	SENSE("OnCreate") ;

	logging::log_debug("Creating TM manager window") ;

	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;
	const CString filename = get_template_filename(_T("start_search.html")) ;
	m_view.create(*this, filename) ;

	CWindowSettings ws;
	if( ws.Load( resource_string(IDS_REG_KEY), m_settings_key ) )
	{
		ws.ApplyTo( *this ) ;
	}
	m_view.set_listener(static_cast<CHtmlViewListener *>(this)) ;

	CString title ;
	title.LoadString(IDS_SEARCH_MANAGER_TITLE) ;
	SetWindowText( title ) ;

	m_accelerator.LoadAccelerators(IDR_SEARCH_ACCEL) ;

#ifndef UNIT_TEST
	_Module.GetMessageLoop()->AddMessageFilter(this) ;
#endif
	return 0L ;
}

void CManagerWindow::save_window_settings()
{
	CWindowSettings ws;
	ws.GetFrom(*this);
	ws.Save( resource_string(IDS_REG_KEY), m_settings_key );
}

LRESULT CManagerWindow::OnDestroy( UINT, WPARAM, LPARAM )
{
	SENSE("OnDestroy") ;
	save_window_settings() ;
	m_search_runner.clear_terms() ;

#ifndef UNIT_TEST
	m_accelerator.DestroyObject() ;
	_Module.GetMessageLoop()->RemoveMessageFilter(this) ;
#endif
	return 0L ;
}

LRESULT CManagerWindow::OnSize(UINT, WPARAM, LPARAM)
{
	SENSE("OnSize") ;
	// get the client rect
	CRect ClientRect ;
	ClientRect.SetRectEmpty() ;
	GetClientRect( &ClientRect ) ;

	// resize edit window
	m_view.MoveWindow( &ClientRect, TRUE ) ;
	return 0L ;
}

void CManagerWindow::wait_for_doc_complete()
{
	background_processor backer( 100, NULL, *this ) ;

	while ( ! m_view.is_document_complete() || ! m_view.is_navigation_complete() ) 
	{
		backer.perform_background_processing() ;
	}
}

// When we navigate from the replace page, or when the window is created
void CManagerWindow::show_search_page()
{
	SENSE("CManagerWindow::show_search_page") ;
	const wstring filename = (LPCWSTR)get_template_filename(_T("start_search.html")) ;
	m_view.navigate(filename) ;

	wait_for_doc_complete() ;
	if (! m_search_runner.get_terms().empty())
	{
		show_search_results_page() ;
	}
}

// When we navigate from the search page
void CManagerWindow::handle_gotoreplace()
{
	SENSE("handle_gotoreplace") ;
	const wstring filename = (LPCWSTR)get_template_filename(_T("start_replace.html")) ;
	m_view.navigate(filename) ;

	wait_for_doc_complete() ;
	set_filterbox_text(get_doc3(),
		m_search_runner.get_terms()) ;
}

void CManagerWindow::show_search_results_page()
{
	SENSE("CManagerWindow::show_search_results_page") ;
	doc3_wrapper_ptr doc = get_doc3();
	set_filterbox_text(doc, m_search_runner.get_terms()) ;

	retrieve_and_show_matches(doc);
}

void CManagerWindow::show_replace_results_page()
{
	SENSE("CManagerWindow::show_replace_results_page") ;
	set_filterbox_text(get_doc3(),
		m_search_runner.get_terms()) ;
}

BOOL CManagerWindow::PreTranslateMessage( LPMSG pMsg )
{

	ENSURE_FOCUS

	if ( IsWindow() && m_accelerator.TranslateAccelerator( *this, pMsg ) )
	{
		return TRUE ;
	}

	// prevent the view from eating our menu shortcut keys...
	if (::GetKeyState(VK_MENU) & 0x8000)
	{
		return FALSE ;
	}

	if (m_view.IsWindow())
	{
		if (m_view.PreTranslateMessage( pMsg ))
		{
			return TRUE ;
		}
	}

	return FALSE ;
}

/*
 This is a callback from the CHtmlView class. 
 If we handle the URL ourselves, then we cancel navigation.
 Returns true to cancel, false to allow navigation.
 */
bool CManagerWindow::OnBeforeNavigate2( _bstr_t burl )
{
	SENSE("CManagerWindow::OnBeforeNavigate2") ;
	const wstring url = BSTR2wstring(burl) ;

	// "#" is used for JavaScript links.
	if(boost::ends_with(url, L"#"))
	{
		return false ; // don't cancel
	}

	if (boost::ends_with(url, L"gotoreplace"))
	{
		handle_gotoreplace() ;
		return true ;
	}
	if (boost::ends_with(url, L"gotosearch"))
	{
		show_search_page() ;
		return true ;
	}
	if (boost::ends_with(url, L"newsearch"))
	{
		m_search_runner.clear_terms() ;
		m_paginator.set_num_records(0) ;
		m_current_match = 0 ;
		show_search_page() ;
		return true ;
	}
	if (boost::ends_with(url, L"dosearch"))
	{
		perform_search(get_doc3());
		return true ;
	}
	// page navigation
	if (boost::ends_with(url, L"next_page"))
	{
		m_paginator.next_page() ;
		show_search_results(get_doc3(), m_matches) ;
		return true ;
	}
	if (boost::ends_with(url, L"prev_page"))
	{
		m_paginator.prev_page() ;
		show_search_results(get_doc3(), m_matches) ;
		return true ;
	}
	if (boost::ends_with(url, L"first_page"))
	{
		while(m_paginator.has_prev())
		{
			m_paginator.prev_page() ;
		}
		show_search_results(get_doc3(), m_matches) ;
		return true ;
	}
	if (boost::ends_with(url, L"last_page"))
	{
		while(m_paginator.has_next())
		{
			m_paginator.next_page() ;
		}
		show_search_results(get_doc3(), m_matches) ;
		return true ;
	}
	if (boost::ends_with(url, L"goto_page"))
	{
		std::vector<wstring> tokens ;
		boost::split(tokens, url, boost::is_any_of(L"/\\")) ;
		size_t penultimate = tokens.size() - 2 ;
		m_paginator.goto_page(boost::lexical_cast<size_t>(tokens[penultimate])-1) ;
		show_search_results(get_doc3(), m_matches) ;
		return true ;
	}

	if (boost::ends_with(url, L"deletefilter"))
	{
		handle_deletefilter(get_doc3(), url) ;
		return true ;
	}
	if (boost::ends_with(url, L"editrecord"))
	{
		handle_editrecord(get_doc3(), url) ;
		return true ;
	}
	if (boost::ends_with(url, L"deleterecord"))
	{
		handle_deleterecord(get_doc3(), url) ;
		return true ;
	}
	if (boost::ends_with(url, L"undodelete"))
	{
		handle_undodelete(get_doc3()) ;
		return true ;
	}


	// replace page links
	if (boost::ends_with(url, L"replace_find"))
	{
		handle_replace_find(get_doc3()) ;
		return true ;
	}
	if (boost::ends_with(url, L"replace_replace"))
	{
		handle_replace_replace(get_doc3()) ;
		return true ;
	}
	if (boost::ends_with(url, L"replace_all"))
	{
		handle_replace_all(get_doc3(), 
							get_template_text(_T("replace_match.txt")),
							get_template_text(_T("replacelinks.txt"))) ;
		return true ;
	}

	if (boost::ends_with(url, L".html"))
	{
		return false ;
	}
	return true ;
}

/*
 Perform the search based on the search form.
 Get the search term from the "searchbox" text box, add that to our list of search terms,
 and then retrieve and show the matches.
 Also show the list of terms in the "filterbox" div of the web page.
 */
void CManagerWindow::perform_search(doc3_wrapper_ptr doc)
{
	SENSE("CManagerWindow::perform_search") ;
	element_wrapper_ptr search_box = doc->get_element_by_id(L"searchbox") ;

	const wstring term = retrieve_input_value(search_box) ;
	if (term.empty())
	{
		ATLTRACE("Empty term in search box\n") ;
		return ;
	}
	m_search_runner.add_term(term) ;

	set_filterbox_text(doc, m_search_runner.get_terms());

	retrieve_and_show_matches(doc);
}

/*
 Generate the list of search terms, and show the list in the "filterbox" div
 of the hosted web page.
 */
void CManagerWindow::set_filterbox_text( const doc3_wrapper_ptr doc, const std::vector<wstring> &terms )
{
	SENSE("CManagerWindow::set_filterbox_text") ;
	element_wrapper_ptr filter_box = doc->get_element_by_id(L"filterbox") ;
	filter_box->set_inner_text(get_filter_text(terms));
}

/*
 The memory window or glossary window will set this.
 It contains the list of memories/glossaries that we'll be searching.
 */
void CManagerWindow::set_mem_controller( memory_controller controller )
{
	m_controller = controller ;
}

/*
 Fills `matches` with the potential records for replacement.
 This is separate from the normal search function (`get_search_matches`), 
 because we also have to make sure that the "replace_from" is matched.
 */
void CManagerWindow::get_replace_matches( std::vector<mem_engine::search_match_ptr> &matches,
										const wstring replace_from)
{
	matches.clear() ;
	mem_engine::search_match_container matchset ;
	foreach(mem_engine::memory_pointer mem, m_controller->get_memories())
	{
		if (mem->is_local())
		{
			foreach(record_pointer rec, mem->get_records())
			{
				if (m_search_runner.is_match(rec) 
					&& m_search_runner.term_matches(rec, replace_from))
				{
					search_match_ptr match(mem->make_match()) ;
					match->set_record(rec) ;
					match->get_markup()->SetTrans(rec->get_trans_rich()) ;
					matchset.insert(match) ;
				}
			}
		}
	}
	std::copy(matchset.begin(), matchset.end(), std::back_inserter(matches)) ;
}

/*
 Fills `matches` with the records that match our search terms.
 */
void CManagerWindow::get_search_matches( std::vector<mem_engine::search_match_ptr> &matches )
{
	matches.clear() ;
	mem_engine::search_match_container matchset ;
	foreach(mem_engine::memory_pointer mem, m_controller->get_memories())
	{
		if (mem->is_local())
		{
			m_search_runner.get_matches(mem, matchset) ;
		}
	}
	std::copy(matchset.begin(), matchset.end(), std::back_inserter(matches)) ;
	m_paginator.set_num_records(matches.size()) ;
	m_current_match = 0 ;
}

/*
 Generates the HTML rendering of the search results, and places it in 
 the "searchresults" div on the hosted web page.

 Search results are paginated, so we only show the range given us by
 the paginator.
 */
void CManagerWindow::show_search_results( doc3_wrapper_ptr doc, match_vec &matches )
{
	CTextTemplate text_tmpl ;

	// message
	text_tmpl.Assign(L"message", m_message) ;
	m_message.clear() ;
	// page stuff
	text_tmpl.Assign(L"pagination", get_pagination_text(m_paginator)) ;
	text_tmpl.Assign(L"page", ulong2wstring(m_paginator.get_current_page()+1)) ;

	CNumberFmt number_format ;
	text_tmpl.Assign(L"num_pages", wstring((LPCWSTR)(number_format.Format(m_paginator.get_num_pages())))) ;

	text_tmpl.Assign(L"num_matches", wstring((LPCWSTR)(number_format.Format(matches.size())))) ;

	text_tmpl::DictListPtr items = text_tmpl.CreateDictList();

	for (size_t i = m_paginator.get_start() ; i < m_paginator.get_end() ; ++i)
	{
		text_tmpl::DictPtr item = text_tmpl.CreateDict() ;

		mem_engine::search_match_ptr match = matches[i] ;
		mem_engine::record_pointer record = match->get_record() ;

		item->insert(std::make_pair(L"num0", tows(i))) ;
		item->insert(std::make_pair(L"num", tows(i+1))) ;
		item->insert(std::make_pair(L"source", record->get_source_rich())) ;
		item->insert(std::make_pair(L"trans", record->get_trans_rich())) ;
		item->insert(std::make_pair(L"context", record->get_context_rich())) ;
		item->insert(std::make_pair(L"created", record->get_created().get_date_time_string())) ;
		item->insert(std::make_pair(L"modified", record->get_modified().get_date_time_string())) ;
		item->insert(std::make_pair(L"reliability", tows(record->get_reliability()))) ;
		item->insert(std::make_pair(L"validated", bool2wstring(record->is_validated()))) ;

		item->insert(std::make_pair(L"creator", record->get_creator())) ;
		item->insert(std::make_pair(L"modified_by", record->get_modified_by())) ;
		// other info
		const wstring filename = match->get_memory_location() ;
		wstring loc ;
		if ( filename.empty() )
		{
			loc = R2WSTR( IDS_NEW ) ;
		}
		else
		{
			loc = fs::wpath(filename).leaf();		
		}

		item->insert(std::make_pair(L"mem", loc)) ;
		item->insert(std::make_pair(L"memory", loc)) ;
		item->insert(std::make_pair(L"refcount", tows(record->get_refcount()))) ;
		item->insert(std::make_pair(L"ref_count", tows(record->get_refcount()))) ;

		items->push_back(item) ;
	}
	text_tmpl.Assign(L"results", items) ;

	wstring text = text_tmpl.Fetch(get_template_text(_T("search_matches.txt"))) ;

	doc->get_element_by_id(L"searchresults")->set_inner_text(text) ;
}

void CManagerWindow::retrieve_and_show_matches( doc3_wrapper_ptr doc )
{
	SENSE("CManagerWindow::retrieve_and_show_matches") ;
	get_search_matches(m_matches);
	show_search_results(doc, m_matches) ;
}

/*
 The user has chosen to delete one of the search filters.
 Extract the filter pos number from the url, then delete it,
 and recalculate the matches.
 */
void CManagerWindow::handle_deletefilter(doc3_wrapper_ptr doc, wstring url)
{
	SENSE("handle_deletefilter") ;

	m_search_runner.remove_term(get_pos_arg(url)) ;

	set_filterbox_text(doc, m_search_runner.get_terms());

	if (m_search_runner.get_terms().empty())
	{
		m_paginator.set_num_records(0) ;
		m_current_match = 0 ;
		show_search_page() ;
		return ;
	}
	retrieve_and_show_matches(doc);
}

/*
 The user wants to edit a record.
 Extract the match pos from the url.
 Use a modal version of the Edit Record dialog to edit the record.
 */
void CManagerWindow::handle_editrecord( doc3_wrapper_ptr doc, wstring url )
{
	SENSE("handle_editrecord") ;
	CEditTransRecordDialogModal editdlg ;

	size_t pos = get_pos_arg(url) ;
	if (pos >= m_matches.size())
	{
		m_message = L"Edit parameter is out of bounds: " + ulong2wstring(pos) ;
		return ;
	}
	mem_engine::search_match_ptr match = m_matches[pos] ;

	editdlg.set_record(match->get_record()) ;
	editdlg.set_memory_id(match->get_memory_id()) ;

#ifndef UNIT_TEST
	if (editdlg.DoModal(*this) == IDOK)
	{
#endif		
		show_search_results(doc, m_matches) ;
#ifndef UNIT_TEST
	}
#endif		
}

/*
 The user chose to delete the indicated record.
 Extract the match pos from the url.
 Add an undo link to recover the record.
 */
void CManagerWindow::handle_deleterecord( doc3_wrapper_ptr doc, wstring url )
{
	using namespace mem_engine ;

	SENSE("handle_deleterecord") ;

	size_t pos = get_pos_arg(url) ;
	if (pos >= m_matches.size())
	{
		m_message = L"Deletion parameter is out of bounds: " + ulong2wstring(pos) ;
		return ;
	}
	m_deleted_match = m_matches[pos] ;
	m_matches.erase(m_matches.begin() + pos) ;
	// set the number of records, but don't reset the current page
	m_paginator.set_num_records(m_matches.size(), false) ;
	delete_record(m_deleted_match);
	show_search_results(doc, m_matches) ;
	if (m_current_match)
	{
		--m_current_match ;
	}
}

/*
 Undo the deleted record and recover it.
 */
void CManagerWindow::handle_undodelete( doc3_wrapper_ptr doc )
{
	SENSE("handle_undodelete") ;
	const int memid = m_deleted_match->get_memory_id() ;
	record_pointer record = m_deleted_match->get_record() ;

	foreach(mem_engine::memory_pointer mem, m_controller->get_memories())
	{
		if (mem->get_id() == memid)
		{
			mem->add_record(record) ;
			retrieve_and_show_matches(doc);
			return ;
		}
	}
}

/*
 Extract the pos argument from the URL.
 */
size_t CManagerWindow::get_pos_arg( const wstring url )
{
	std::vector<wstring> tokens ;
	boost::split(tokens, url, boost::is_any_of(L"/\\")) ;
	ATLASSERT(! tokens.empty()) ;
	const size_t last = tokens.size() -1 ;
	return boost::lexical_cast<size_t>(tokens[last-1]) ;
}

/*
 Get the match at the indicated position.

 Recalculates the matches each time.
 */
mem_engine::search_match_ptr CManagerWindow::get_match_at( const size_t i )
{
	match_vec matches; 
	get_search_matches(matches);
	ATLASSERT(i < matches.size()) ;
	return matches[i] ;
}

/*
 Get the document element from the web browser.
 We use this to get and set text in various elements by id.
 We serve up a fake doc3 element if we're unit testing.
 */
doc3_wrapper_ptr CManagerWindow::get_doc3()
{
#ifndef UNIT_TEST
	return make_doc3_wrapper(m_view.get_document3());
#else
	return get_fake_search_doc(); 
#endif
}

/*
 Delete the record from the appropriate memory.
 If we failed to delete it, check if the memory is locked.
 Give appropriate feedback.
 */
void CManagerWindow::delete_record( search_match_ptr match )
{
	memory_pointer mem = m_controller->get_memory_by_id(match->get_memory_id()) ;

	if (mem->erase(match->get_record()))
	{
		m_message = R2WSTR(IDS_DELETED_RECORD_MSG) ;
	}
	else if (mem->is_locked())
	{
		m_message = R2WSTR(IDS_DELETE_FAILED_MEM_LOCKED_MSG) ;
	}
}

/*
 Find the next candidate for replacement.
 */
void CManagerWindow::handle_replace_find( doc3_wrapper_ptr doc )
{
	SENSE("handle_replace_find") ;

	element_wrapper_ptr search_box = doc->get_element_by_id(L"replacefrom") ;
	const wstring replace_from = search_box->get_attribute(L"value") ;
	this->get_replace_matches(m_replace_matches, replace_from) ;

	show_replace_results(doc, m_replace_matches) ;
	m_current_match++ ;
}

/*
 Show the results of the found record, and what it will look like
 after we do the replacement.
 */
void CManagerWindow::show_replace_results( doc3_wrapper_ptr doc, match_vec &matches )
{
	CTextTemplate text_tmpl ;

	// message
	text_tmpl.Assign(L"message", m_message) ;
	m_message.clear() ;

	if (m_current_match >= matches.size())
	{
		text_tmpl.Assign(L"found", L"") ;
		text_tmpl.Assign(L"result", L"") ;
		m_current_match = 0 ;
	}
	else
	{
		text_tmpl.Assign(L"match_num", ulong2wstring(m_current_match+1)) ;
		text_tmpl.Assign(L"num_matches", ulong2wstring(matches.size())) ;

		text_tmpl::DictPtr found = text_tmpl.CreateDict() ;
		text_tmpl::DictPtr result = text_tmpl.CreateDict() ;

		mem_engine::record_pointer record = matches[m_current_match]->get_record() ;
		// found
		text_tmpl.Assign(found, L"source", record->get_source_rich()) ;
		text_tmpl.Assign(found, L"trans", record->get_trans_rich()) ;
		text_tmpl.Assign(found, L"context", record->get_context_rich()) ;
		text_tmpl.Assign(found, L"created", record->get_created().get_date_time_string()) ;
		text_tmpl.Assign(found, L"date_created", record->get_created().get_date_time_string()) ;
		text_tmpl.Assign(found, L"modified", record->get_modified().get_date_time_string()) ;
		text_tmpl.Assign(found, L"last_modified", record->get_modified().get_date_time_string()) ;
		text_tmpl.Assign(found, L"reliability", tows(record->get_reliability())) ;
		text_tmpl.Assign(found, L"validated", bool2wstring(record->is_validated())) ;

		text_tmpl.Assign(found, L"creator", record->get_creator()) ;
		text_tmpl.Assign(found, L"created_by", record->get_creator()) ;
		text_tmpl.Assign(found, L"modified_by", record->get_modified_by()) ;
		text_tmpl.Assign(found, L"refcount", tows(record->get_refcount())) ;
		text_tmpl.Assign(found, L"ref_count", tows(record->get_refcount())) ;

		// result
		mem_engine::record_pointer replaced(new record_local(record)) ;
		perform_replace(doc, replaced);

		text_tmpl.Assign(result, L"source", replaced->get_source_rich()) ;
		text_tmpl.Assign(result, L"trans", replaced->get_trans_rich()) ;
		text_tmpl.Assign(result, L"context", replaced->get_context_rich()) ;
		text_tmpl.Assign(result, L"created", replaced->get_created().get_date_time_string()) ;
		text_tmpl.Assign(result, L"date_created", replaced->get_created().get_date_time_string()) ;
		text_tmpl.Assign(result, L"modified", replaced->get_modified().get_date_time_string()) ;
		text_tmpl.Assign(result, L"last_modified", replaced->get_modified().get_date_time_string()) ;
		text_tmpl.Assign(result, L"reliability", tows(replaced->get_reliability())) ;
		text_tmpl.Assign(result, L"validated", bool2wstring(replaced->is_validated())) ;

		text_tmpl.Assign(result, L"creator", replaced->get_creator()) ;
		text_tmpl.Assign(result, L"created_by", replaced->get_creator()) ;
		text_tmpl.Assign(result, L"modified_by", replaced->get_modified_by()) ;
		text_tmpl.Assign(result, L"refcount", tows(replaced->get_refcount())) ;
		text_tmpl.Assign(result, L"ref_count", tows(replaced->get_refcount())) ;


		text_tmpl.Assign(L"found", found) ;
		text_tmpl.Assign(L"result", result) ;

	}

	const wstring text = text_tmpl.Fetch(get_template_text(_T("replace_match.txt"))) ;

	doc->get_element_by_id(L"searchresults")->set_inner_text(text) ;

	wstring replacelinks = text_tmpl.Fetch(get_template_text(_T("replacelinks.txt"))) ;
	doc->get_element_by_id(L"replacelinks")->set_inner_text(replacelinks) ;
}

/*
 Replace the curround found record, and search for the next one.
 */
void CManagerWindow::handle_replace_replace( doc3_wrapper_ptr doc )
{
	SENSE("handle_replace_replace") ;

	if (m_current_match >= m_replace_matches.size())
	{
		return; 
	}
	ATLASSERT(m_current_match>0) ;
	search_match_ptr match = m_replace_matches[m_current_match-1] ;

	element_wrapper_ptr replacefrom_box = doc->get_element_by_id(L"replacefrom") ;
	const wstring replace_from = replacefrom_box->get_attribute(L"value") ;

	element_wrapper_ptr replaceto_box = doc->get_element_by_id(L"replaceto") ;
	const wstring replace_to = replaceto_box->get_attribute(L"value") ;

	replace_in_memory(match, replace_from, replace_to);
	handle_replace_find(doc) ;
}

/*
 Do replacement on all found records.
 */
void CManagerWindow::handle_replace_all(doc3_wrapper_ptr doc, 
									   wstring search_template,
									   wstring replace_template)
{
	element_wrapper_ptr search_box = doc->get_element_by_id(L"replacefrom") ;
	const wstring replace_from = search_box->get_attribute(L"value") ;
	this->get_replace_matches(m_replace_matches, replace_from) ;

	element_wrapper_ptr replaceto_box = doc->get_element_by_id(L"replaceto") ;
	const wstring replace_to = replaceto_box->get_attribute(L"value") ;

	size_t num_replaced = 0 ;
	foreach(search_match_ptr match, m_replace_matches)
	{
		if (replace_in_memory(match, replace_from, replace_to))
		{
			num_replaced++;
		}
	}

	m_message = (LPCWSTR)system_message(IDS_REPLACE_COMPLETE_MSG, int_arg(num_replaced)) ;

	m_replace_matches.clear() ;
	m_current_match = 0 ;

	CTextTemplate text_tmpl ;

	// message
	text_tmpl.Assign(L"message", m_message) ;
	m_message.clear() ;

	text_tmpl.Assign(L"found", L"") ;
	text_tmpl.Assign(L"result", L"None") ;

	doc->get_element_by_id(L"searchresults")->set_inner_text(text_tmpl.Fetch(search_template)) ;
	doc->get_element_by_id(L"replacelinks")->set_inner_text(text_tmpl.Fetch(replace_template)) ;
	doc->get_element_by_id(L"filterbox")->set_inner_text(L"") ;
}

/*
 Do the actual replacement.
 Get the replace_from and replace_to from the appropriate text boxes in the 
 hosted web page.
 */
void CManagerWindow::perform_replace(doc3_wrapper_ptr doc, record_pointer rec)
{
	element_wrapper_ptr replacefrom_box = doc->get_element_by_id(L"replacefrom") ;
	wstring replace_from = replacefrom_box->get_attribute(L"value") ;

	element_wrapper_ptr replaceto_box = doc->get_element_by_id(L"replaceto") ;
	wstring replace_to = replaceto_box->get_attribute(L"value") ;

	if (replace_from.empty())
	{
		return ;
	}
	replacer::do_replace(rec, replace_from, replace_to) ;
}

/*
 Do the replacement in the memory.
 */
bool CManagerWindow::replace_in_memory( search_match_ptr match, const wstring replace_from, const wstring replace_to )
{
	if (replace_from.empty())
	{
		return false ;
	}

	record_pointer record = match->get_record() ;
	record_pointer modified = record->clone() ;
	record_pointer result = replacer::do_replace(modified, replace_from, replace_to) ;
	const int memid = match->get_memory_id() ;
	if( result != record)  // was there a modification?
	{
		m_controller->get_memory_by_id(memid)->replace(record, modified) ;
		return true ;
	}
	return false ;
}

// Clear the filters and load a new search page
LRESULT CManagerWindow::OnNewSearch()
{
	m_search_runner.clear_terms() ;
	m_paginator.set_num_records(0) ;
	m_current_match = 0 ;
	show_search_page() ;

	return 0L ;
}

// Navigate to the search page
LRESULT CManagerWindow::OnSearch()
{
	show_search_page() ;
	return 0L ;
}

// Navigate to the replace page
LRESULT CManagerWindow::OnReplace()
{
	handle_gotoreplace() ;
	return 0L ;
}

// Call the toggleHelp JavaScript function
LRESULT CManagerWindow::OnToggleHelp()
{
	html::CWebPage page ;
	page.SetDocument(m_view.get_document()) ;
	page.CallJScript("toggleHelp") ;

	return 0L ;
}
