#include "StdAfx.h"
#include "ViewCollectionWalker.h"

using namespace except ;

bool is_source_tag( wstring id, tag_name_holder &tags )
{
	return id == tags.source_tag || id == tags.dummy_source_id;
}
bool is_trans_tag( wstring id, tag_name_holder &tags )
{
	return id == tags.trans_tag || id == tags.dummy_trans_id;
}
bool is_other_tag( wstring id, tag_name_holder &tags )
{
	return id != tags.refcount_id
		&& id != tags.created_id
		&& id != tags.modified_id
		&& id != tags.memory_id
		&& id != tags.rich_query_id
		&& id != tags.rich_source_id;
}

void CViewCollectionWalker::CheckLinkUrl( element_wrapper_ptr element, const wstring doc_path )
{
	const wstring tag_name = element->get_tag() ;

	if ( str::equal_nocase( tag_name, L"A" ))
	{
		RepairLinkUrl(element, doc_path);
	}
}

void CViewCollectionWalker::RepairLinkUrl( element_wrapper_ptr element, const wstring &doc_path )
{
	wstring href = element->get_attribute(L"HREF") ;

	if (str::left(href, 8) != L"file:///")
	{
		return ;
	}

	href = href.substr(8) ;

	// doc_url = "file://C:\Users\RyanVista\AppData\Local\Felix\html\en\start.html"
	wstring doc ;
	if (str::left(doc_path, 7) == L"file://")
	{
		doc = doc_path.substr(7) ;
	}
	else
	{
		doc = doc_path ;
	}

	// "bar.html" -> "file:///C:/Users/RyanVista/AppData/Local/Felix/html/en/bar.html"
	if ( str::left( href, doc.size() ) == doc )
	{
		element->set_attribute(L"HREF", href.substr(doc.size())) ;
	}
	// "/foo.html" -> "file:///C:/foo.html"
	else
	{
		element->set_attribute(L"HREF", href.substr(href.find_last_of(L':')+1)) ;
	}
}

void CViewCollectionWalker::SetReliabilityFromElement( element_wrapper_ptr element, record_pointer & record )
{
	const wstring rel_text = element->get_inner_text() ;
	if ( ! rel_text.empty() ) 
	{
		record->set_reliability( boost::lexical_cast< size_t >( rel_text ) ) ;
	}
}

void CViewCollectionWalker::SetValidatedFromElement( element_wrapper_ptr element, record_pointer & record )
{
	wstring val = element->get_inner_text();

	str::make_lower( val ) ;

	if ( val == L"true" || val == L"yes" )	
	{
		record->set_validated_on() ;
	}
	else
	{
		record->set_validated_off() ;
	}
}

void CViewCollectionWalker::SetItemFromElement( element_wrapper_ptr element, record_pointer rec, wstring id )
{
	const wstring tag_str = element->get_tag() ;

	if ( str::equal_nocase( tag_str, L"td" ) )
	{
		rec->set_item( id, element->get_inner_text() ) ;
	}
}

void CViewCollectionWalker::ElementToRecord( element_wrapper_ptr element, record_pointer rec )
{
	tag_name_holder &tags = tag_name_holder::instance() ;
	const wstring id = element->get_id() ;

	if ( id.empty() )
	{
		return ;
	}

	if ( is_source_tag(id, tags) )	
	{
		rec->set_source( element->get_inner_text() ) ;
		return ;
	}

	if ( is_trans_tag(id, tags) )		
	{
		rec->set_trans( element->get_inner_text() ) ;
		return ;
	}

	if ( id == tags.context_tag )		
	{
		rec->set_context( element->get_inner_text() ) ;
		return ;
	}

	if ( id == tags.reliability_tag ) 
	{
		SetReliabilityFromElement( element, rec ) ;
		return ;
	}

	if ( id == tags.validated_tag )	
	{
		SetValidatedFromElement( element, rec ) ;
		return ;
	}


	// final check
	if ( is_other_tag(id, tags)
		)
	{
		SetItemFromElement( element, rec, id ) ;
	}
}

void CViewCollectionWalker::AddRecordToList( record_pointer rec,  
											MemoryControllerType memories,  
											SearchMatchType match,  
											MatchListType &match_list )
{
	BANNER("CViewCollectionWalker::AddRecordToList") ;
	rec->modify() ;

	MemPtr mem ;
	try
	{
		mem = memories->get_memory_by_id(match->get_memory_id()) ;
	}
	catch (CProgramException& )
	{
		mem = memories->get_first_memory() ;
	}

	mem->add_record( rec ) ;

	match->set_record( rec ) ;
	match->set_values_to_record() ;

	// add the match to our match list
	match_list += match ;
	ATLTRACE(" ... Record added\n\n") ;
}

void CViewCollectionWalker::RecordToElement( MSHTML::IHTMLElementPtr &element, record_pointer &rec )
{
	tag_name_holder &tags = tag_name_holder::instance() ;
	const wstring id = BSTR2wstring( element->id ) ;

	if ( id == tags.source_tag )	
	{
		element->innerHTML = rec->get_source_rich().c_str() ;
		// we need to re-calculate the length, because we've been mucking with the html
		return ;
	}

	if ( id == tags.trans_tag )		
	{
		element->innerHTML = rec->get_trans_rich().c_str() ;
		// we need to re-calculate the length, because we've been mucking with the html
		return ;
	}

	if ( id == tags.context_tag )		
	{
		element->innerHTML = rec->get_context_rich().c_str() ;
		// we need to re-calculate the length, because we've been mucking with the html
		return ;
	}

	// dummy stuff
	if ( id == tags.dummy_source_id )
	{
		element->innerHTML = _bstr_t() ;
		// we need to re-calculate the length, because we've been mucking with the html
		return ;
	}

	if ( id == tags.dummy_trans_id )
	{
		element->innerHTML = _bstr_t() ;
		// we need to re-calculate the length, because we've been mucking with the html
		return ;
	}
}

CViewCollectionWalker::CViewCollectionWalker( void )
{

}

CViewCollectionWalker::~CViewCollectionWalker( void )
{

}
