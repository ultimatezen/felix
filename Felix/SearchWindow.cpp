#include "stdafx.h"
#include "SearchWindow.h"
#include "RegSettings.h"
#include "background_processor.h"
#include "text_templates.h"
#include "EditTransRecordDialogModal.h"
#include "replacer.h"
#include "record_local.h"
#include "WebPage.h"
#include "numberfmt.h"
#include "system_message.h"
#include "memory_local.h"

#ifdef UNIT_TEST
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#endif

using namespace mem_engine ;


// for displaying search results in HTML view
wstring escape_entities(const wstring text)
{
	return boost::replace_all_copy(boost::replace_all_copy(text, L"&", L"&amp;"), L"<", L"&lt;") ;
}

wstring tows(const size_t i)
{
	return boost::lexical_cast<wstring>(i) ;
}


LRESULT CSearchWindow::OnCreate( UINT, WPARAM, LPARAM )
{
	SENSE("OnCreate") ;

	logging::log_debug("Creating search window") ;

	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;
	const CString filename = cpptempl::get_template_filename(_T("start_search.html")) ;
	m_view.create(*this, filename) ;

	CWindowSettings ws;
	if( ws.Load( resource_string(IDS_REG_KEY), m_settings_key ) )
	{
		ws.ApplyTo( *this ) ;
	}
	m_view.set_listener(static_cast<CHtmlViewListener *>(this)) ;

	SetWindowText( m_title ) ;

	m_accelerator.LoadAccelerators(IDR_SEARCH_ACCEL) ;

#ifndef UNIT_TEST
	_Module.GetMessageLoop()->AddMessageFilter(this) ;
#endif
	return 0L ;
}

// Title and location of window settings depend on whether it's a memory
// or glossary search
void CSearchWindow::set_mem_window( bool is_mem )
{
	if (is_mem)
	{
		m_settings_key = _T("MemorySearchWindow") ;
		m_title = R2WSTR(IDS_SEARCHWIN_TITLE_MEM).c_str();
	}
	else
	{
		m_settings_key = _T("GlossarySearchWindow") ;
		m_title = R2WSTR(IDS_SEARCHWIN_TITLE_GLOSS).c_str() ;
	}
}

void CSearchWindow::save_window_settings()
{
	CWindowSettings ws;
	ws.GetFrom(*this);
	ws.Save( resource_string(IDS_REG_KEY), m_settings_key );
}

LRESULT CSearchWindow::OnDestroy( UINT, WPARAM, LPARAM )
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

LRESULT CSearchWindow::OnSize(UINT, WPARAM, LPARAM)
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

void CSearchWindow::wait_for_doc_complete()
{
	background_processor backer( 100, NULL, *this ) ;

	while ( ! m_view.is_document_complete() || ! m_view.is_navigation_complete() ) 
	{
		backer.perform_background_processing() ;
	}
}

// When we navigate from the replace page, or when the window is created
void CSearchWindow::show_search_page()
{
	SENSE("CSearchWindow::show_search_page") ;
	const wstring filename = (LPCWSTR)cpptempl::get_template_filename(_T("start_search.html")) ;
	m_view.navigate(filename) ;

	wait_for_doc_complete() ;
	if (! m_search_runner.get_terms().empty())
	{
		show_search_results_page() ;
	}
}

// When we navigate from the search page
void CSearchWindow::handle_gotoreplace()
{
	SENSE("handle_gotoreplace") ;
	const wstring filename = (LPCWSTR)cpptempl::get_template_filename(_T("start_replace.html")) ;
	m_view.navigate(filename) ;

	wait_for_doc_complete() ;
	set_filterbox_text(get_doc3(),
		m_search_runner.get_terms()) ;
}

void CSearchWindow::show_search_results_page()
{
	SENSE("CSearchWindow::show_search_results_page") ;
	doc3_wrapper_ptr doc = get_doc3();
	set_filterbox_text(doc, m_search_runner.get_terms()) ;

	retrieve_and_show_matches(doc);
}

void CSearchWindow::show_replace_results_page()
{
	SENSE("CSearchWindow::show_replace_results_page") ;
	set_filterbox_text(get_doc3(),
		m_search_runner.get_terms()) ;
}

BOOL CSearchWindow::PreTranslateMessage( LPMSG pMsg )
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
bool CSearchWindow::OnBeforeNavigate2( _bstr_t burl )
{
	SENSE("CSearchWindow::OnBeforeNavigate2") ;
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
	if (boost::ends_with(url, L"goto_page"))
	{
		std::vector<wstring> tokens ;
		boost::split(tokens, url, boost::is_any_of(L"/\\")) ;
		std::reverse(tokens.begin(), tokens.end()) ;
		m_paginator.goto_page(boost::lexical_cast<size_t>(tokens[1])-1) ;
		show_search_results(get_doc3(), m_matches) ;
		return true ;
	}
	if (boost::ends_with(url, L"save_results"))
	{
		save_results(m_matches);
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
							cpptempl::get_template_text(_T("replace_match.txt")),
							cpptempl::get_template_text(_T("replacelinks.txt"))) ;
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
void CSearchWindow::perform_search(doc3_wrapper_ptr doc)
{
	SENSE("CSearchWindow::perform_search") ;
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
void CSearchWindow::set_filterbox_text( const doc3_wrapper_ptr doc, const std::vector<wstring> &terms )
{
	SENSE("CSearchWindow::set_filterbox_text") ;
	element_wrapper_ptr filter_box = doc->get_element_by_id(L"filterbox") ;
	filter_box->set_inner_text(get_filter_text(terms));
}

/*
 The memory window or glossary window will set this.
 It contains the list of memories/glossaries that we'll be searching.
 */
void CSearchWindow::set_mem_controller( memory_controller controller )
{
	m_controller = controller ;
}

/*
 Fills `matches` with the potential records for replacement.
 This is separate from the normal search function (`get_search_matches`), 
 because we also have to make sure that the "replace_from" is matched.
 */
void CSearchWindow::get_replace_matches( std::vector<mem_engine::search_match_ptr> &matches,
										const wstring replace_from)
{
	matches.clear() ;
	mem_engine::search_match_container matchset ;
	foreach(mem_engine::memory_pointer mem, m_controller->get_memories())
	{
		if (mem->is_local())
		{
			get_replace_matches_mem(mem, replace_from, matchset);
		}
	}
	matches.assign(matchset.begin(), matchset.end()) ;
}

void CSearchWindow::get_replace_matches_mem( mem_engine::memory_pointer mem, 
											 const wstring replace_from, 
											 mem_engine::search_match_container &matchset )
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

/*
 Fills `matches` with the records that match our search terms.
 */
void CSearchWindow::get_search_matches( std::vector<mem_engine::search_match_ptr> &matches )
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
	matches.assign(matchset.begin(), matchset.end()) ;
	m_paginator.set_num_records(matches.size()) ;
	m_current_match = 0 ;
}

/*
 Generates the HTML rendering of the search results, and places it in 
 the "searchresults" div on the hosted web page.

 Search results are paginated, so we only show the range given us by
 the paginator.
 */
void CSearchWindow::show_search_results( doc3_wrapper_ptr doc, match_vec &matches )
{
	cpptempl::data_map data ;

	// message
	data[L"message"] = cpptempl::make_data(m_message) ;
	m_message.clear() ;
	// page stuff
	data[L"pagination"] = cpptempl::make_data(get_pagination_text(m_paginator)) ;
	data[L"page"] = cpptempl::make_data(ulong2wstring(m_paginator.get_current_page()+1)) ;

	data[L"num_pages"] = cpptempl::make_data(fmt_num(m_paginator.get_num_pages())) ;

	data[L"num_matches"] = cpptempl::make_data(fmt_num(matches.size())) ;

	cpptempl::data_list items ;

	for (size_t i = m_paginator.get_start() ; i < m_paginator.get_end() ; ++i)
	{
		cpptempl::data_map item ;

		mem_engine::search_match_ptr match = matches[i] ;
		mem_engine::record_pointer record = match->get_record() ;

		item[L"num0"] = cpptempl::make_data(tows(i)) ;
		item[L"num"] = cpptempl::make_data(tows(i+1)) ;
		item[L"source"] = cpptempl::make_data(record->get_source_rich()) ;
		item[L"trans"] = cpptempl::make_data(record->get_trans_rich()) ;
		item[L"context"] = cpptempl::make_data(record->get_context_rich()) ;
		item[L"created"] = cpptempl::make_data(record->get_created().get_date_time_string()) ;
		item[L"modified"] = cpptempl::make_data(record->get_modified().get_date_time_string()) ;
		item[L"reliability"] = cpptempl::make_data(tows(record->get_reliability())) ;
		item[L"validated"] = cpptempl::make_data(bool2wstring(record->is_validated())) ;

		item[L"creator"] = cpptempl::make_data(record->get_creator()) ;
		item[L"modified_by"] = cpptempl::make_data(record->get_modified_by()) ;
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

		item[L"mem"] = cpptempl::make_data(loc) ;
		item[L"memory"] = cpptempl::make_data(loc) ;
		item[L"refcount"] = cpptempl::make_data(tows(record->get_refcount())) ;
		item[L"ref_count"] = cpptempl::make_data(tows(record->get_refcount())) ;

		items.push_back(cpptempl::make_data(item)) ;
	}
	data[L"results"] = cpptempl::make_data(items) ;

	wstring text = cpptempl::parse(cpptempl::get_template_text(_T("search_matches.txt")), data) ;

	doc->get_element_by_id(L"searchresults")->set_inner_text(text) ;
}

void CSearchWindow::retrieve_and_show_matches( doc3_wrapper_ptr doc )
{
	SENSE("CSearchWindow::retrieve_and_show_matches") ;
	get_search_matches(m_matches);
	show_search_results(doc, m_matches) ;
}

/*
 The user has chosen to delete one of the search filters.
 Extract the filter pos number from the url, then delete it,
 and recalculate the matches.
 */
void CSearchWindow::handle_deletefilter(doc3_wrapper_ptr doc, wstring url)
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
void CSearchWindow::handle_editrecord( doc3_wrapper_ptr doc, wstring url )
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
void CSearchWindow::handle_deleterecord( doc3_wrapper_ptr doc, wstring url )
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
void CSearchWindow::handle_undodelete( doc3_wrapper_ptr doc )
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
size_t CSearchWindow::get_pos_arg( const wstring url )
{
	std::vector<wstring> tokens ;
	boost::split(tokens, url, boost::is_any_of(L"/\\")) ;
	ATLASSERT(! tokens.empty()) ;
	std::reverse(tokens.begin(), tokens.end()) ;
	return boost::lexical_cast<size_t>(tokens[1]) ;
}

/*
 Get the match at the indicated position.

 Recalculates the matches each time.
 */
mem_engine::search_match_ptr CSearchWindow::get_match_at( const size_t i )
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
doc3_wrapper_ptr CSearchWindow::get_doc3()
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
void CSearchWindow::delete_record( search_match_ptr match )
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
void CSearchWindow::handle_replace_find( doc3_wrapper_ptr doc )
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
void CSearchWindow::show_replace_results( doc3_wrapper_ptr doc, match_vec &matches )
{
	cpptempl::data_map data ;

	// message
	data[L"message"] = cpptempl::make_data(m_message) ;
	m_message.clear() ;

	if (m_current_match >= matches.size())
	{
		data[L"found"] = cpptempl::make_data(L"") ;
		data[L"result"] = cpptempl::make_data(L"") ;
		m_current_match = 0 ;
	}
	else
	{
		data[L"match_num"] = cpptempl::make_data(ulong2wstring(m_current_match+1)) ;
		data[L"num_matches"] = cpptempl::make_data(ulong2wstring(matches.size())) ;

		cpptempl::data_map found ;
		cpptempl::data_map result ;

		mem_engine::record_pointer record = matches[m_current_match]->get_record() ;
		// found
		found[L"source"] = cpptempl::make_data(record->get_source_rich()) ;
		found[L"trans"] = cpptempl::make_data(record->get_trans_rich()) ;
		found[L"context"] = cpptempl::make_data(record->get_context_rich()) ;
		found[L"created"] = cpptempl::make_data(record->get_created().get_date_time_string()) ;
		found[L"date_created"] = cpptempl::make_data(record->get_created().get_date_time_string()) ;
		found[L"modified"] = cpptempl::make_data(record->get_modified().get_date_time_string()) ;
		found[L"last_modified"] = cpptempl::make_data(record->get_modified().get_date_time_string()) ;
		found[L"reliability"] = cpptempl::make_data(tows(record->get_reliability())) ;
		found[L"validated"] = cpptempl::make_data(bool2wstring(record->is_validated())) ;

		found[L"creator"] = cpptempl::make_data(record->get_creator()) ;
		found[L"created_by"] = cpptempl::make_data(record->get_creator()) ;
		found[L"modified_by"] = cpptempl::make_data(record->get_modified_by()) ;
		found[L"refcount"] = cpptempl::make_data(tows(record->get_refcount())) ;
		found[L"ref_count"] = cpptempl::make_data(tows(record->get_refcount())) ;

		// result
		mem_engine::record_pointer replaced(new record_local(record)) ;
		perform_replace(doc, replaced);

		result[L"source"] = cpptempl::make_data(replaced->get_source_rich()) ;
		result[L"trans"] = cpptempl::make_data(replaced->get_trans_rich()) ;
		result[L"context"] = cpptempl::make_data(replaced->get_context_rich()) ;
		result[L"created"] = cpptempl::make_data(replaced->get_created().get_date_time_string()) ;
		result[L"date_created"] = cpptempl::make_data(replaced->get_created().get_date_time_string()) ;
		result[L"modified"] = cpptempl::make_data(replaced->get_modified().get_date_time_string()) ;
		result[L"last_modified"] = cpptempl::make_data(replaced->get_modified().get_date_time_string()) ;
		result[L"reliability"] = cpptempl::make_data(tows(replaced->get_reliability())) ;
		result[L"validated"] = cpptempl::make_data(bool2wstring(replaced->is_validated())) ;

		result[L"creator"] = cpptempl::make_data(replaced->get_creator()) ;
		result[L"created_by"] = cpptempl::make_data(replaced->get_creator()) ;
		result[L"modified_by"] = cpptempl::make_data(replaced->get_modified_by()) ;
		result[L"refcount"] = cpptempl::make_data(tows(replaced->get_refcount())) ;
		result[L"ref_count"] = cpptempl::make_data(tows(replaced->get_refcount())) ;


		data[L"found"] = cpptempl::make_data(found) ;
		data[L"result"] = cpptempl::make_data(result) ;

	}

	const wstring text = cpptempl::parse(cpptempl::get_template_text(_T("replace_match.txt")), data) ;

	doc->get_element_by_id(L"searchresults")->set_inner_text(text) ;

	wstring replacelinks = cpptempl::parse(cpptempl::get_template_text(_T("replacelinks.txt")), data) ;
	doc->get_element_by_id(L"replacelinks")->set_inner_text(replacelinks) ;
}

/*
 Replace the curround found record, and search for the next one.
 */
void CSearchWindow::handle_replace_replace( doc3_wrapper_ptr doc )
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
void CSearchWindow::handle_replace_all(doc3_wrapper_ptr doc, 
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

	cpptempl::data_map data ;

	// message
	data[L"message"] = cpptempl::make_data(m_message) ;
	m_message.clear() ;

	data[L"found"] = cpptempl::make_data(L"") ;
	data[L"result"] = cpptempl::make_data(L"None") ;

	doc->get_element_by_id(L"searchresults")->set_inner_text(cpptempl::parse(search_template, data)) ;
	doc->get_element_by_id(L"replacelinks")->set_inner_text(cpptempl::parse(replace_template, data)) ;
	doc->get_element_by_id(L"filterbox")->set_inner_text(L"") ;
}

/*
 Do the actual replacement.
 Get the replace_from and replace_to from the appropriate text boxes in the 
 hosted web page.
 */
void CSearchWindow::perform_replace(doc3_wrapper_ptr doc, record_pointer rec)
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
bool CSearchWindow::replace_in_memory( search_match_ptr match, const wstring replace_from, const wstring replace_to )
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
LRESULT CSearchWindow::OnNewSearch()
{
	m_search_runner.clear_terms() ;
	m_paginator.set_num_records(0) ;
	m_current_match = 0 ;
	show_search_page() ;

	return 0L ;
}

// Navigate to the search page
LRESULT CSearchWindow::OnSearch()
{
	show_search_page() ;
	return 0L ;
}

// Navigate to the replace page
LRESULT CSearchWindow::OnReplace()
{
	handle_gotoreplace() ;
	return 0L ;
}

// Call the toggleHelp JavaScript function
LRESULT CSearchWindow::OnToggleHelp()
{
	html::CWebPage page ;
	page.SetDocument(m_view.get_document()) ;
	page.CallJScript("toggleHelp") ;

	return 0L ;
}

void CSearchWindow::save_results( match_vec &matches )
{
	mem_engine::memory_pointer mem(new mem_engine::memory_local) ;

	foreach(mem_engine::search_match_ptr match, matches)
	{
		mem->add_record(match->get_record()) ;
	}
	m_listener->save_memory_as(mem) ;
	m_message = R2WSTR(IDS_SAVED_RESULTS) ;

	show_search_results(get_doc3(), matches) ;
}
/*
 Get the value of the specified input box, and clear that value
 afterward.
 */
const wstring retrieve_input_value( element_wrapper_ptr input_box )
{
	const wstring val = input_box->get_attribute(L"value") ;
	input_box->set_attribute(L"value", L"") ;
	return val ;
}

/*
 Generate the list of filter terms for HTML rendering.
 */
wstring get_filter_text( const std::vector<wstring> & terms ) 
{
	cpptempl::data_map data ;
	cpptempl::data_list filters ;

	foreach(wstring term, terms)
	{
		filters.push_back(cpptempl::make_data(term)) ;
	}
	data[L"filters"] = cpptempl::make_data(filters) ;

	return cpptempl::parse(cpptempl::get_template_text(_T("filter_list.txt")), data) ;
}
