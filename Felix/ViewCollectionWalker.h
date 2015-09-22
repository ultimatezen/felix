/** 
 *  ViewCollectionWalker   version:  1.0   -  date: 10/01/2006
 *
 */
#pragma once

#include "search_match.h"	// mem_engine::search_match_ptr
#include "record.h"			// mem_engine::record_pointer
#include "memory_model.h"	// mem_engine::memory_model
#include "TranslationMemory.h"	// memory_pointer
#include "HtmlDocument.h"		// html::collection_ptr
#include "element_wrapper.h"
#include "logging.h"
#include "query.h"
#include "record_local.h"

bool is_source_tag( wstring id, tag_name_holder &tags ) ;
bool is_trans_tag( wstring id, tag_name_holder &tags ) ;
bool is_other_tag( wstring id, tag_name_holder &tags ) ;

class CViewCollectionWalker
{
	typedef mem_engine::search_match_ptr SearchMatchType ;
	typedef std::vector< SearchMatchType > MatchListType ;

	typedef mem_engine::record_pointer record_pointer ;
	typedef std::shared_ptr<mem_engine::memory_model> MemoryControllerType ;

	typedef mem_engine::memory_pointer MemPtr ;

public:

	CViewCollectionWalker(void);
public:

	~CViewCollectionWalker(void);

	void EraseCurrentRecord( SearchMatchType &match, mem_engine::felix_query *matches, const wstring id, MemoryControllerType memories ) const;
	void RecordsToElements( mem_engine::felix_query *matches, html::collection_ptr collection ) const;

	void CheckLinkUrl( element_wrapper_ptr element, const wstring doc_path ) const;
	void RepairLinkUrl( element_wrapper_ptr element, const wstring doc_path ) const;
	void SetReliabilityFromElement( element_wrapper_ptr element, record_pointer & record ) const;
	void SetValidatedFromElement( element_wrapper_ptr element, record_pointer & record ) const;
	void SetItemFromElement( element_wrapper_ptr element, record_pointer rec, wstring id ) const;

	void ElementToRecord( element_wrapper_ptr element, record_pointer rec ) const;

	void AddRecordToList( record_pointer rec, 
		MemoryControllerType memories, 
		SearchMatchType match, 
		MatchListType &match_list ) const;

	void RecordToElement( MSHTML::IHTMLElementPtr &element, record_pointer &rec ) const;

};
