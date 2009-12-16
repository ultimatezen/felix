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

bool is_source_tag( wstring id, tag_name_holder &tags ) ;
bool is_trans_tag( wstring id, tag_name_holder &tags ) ;
bool is_other_tag( wstring id, tag_name_holder &tags ) ;

class CViewCollectionWalker
{
	typedef mem_engine::search_match_ptr SearchMatchType ;
	typedef std::vector< SearchMatchType > MatchListType ;

	typedef mem_engine::record_pointer record_pointer ;
	typedef boost::shared_ptr<mem_engine::memory_model> MemoryControllerType ;

	typedef mem_engine::memory_pointer MemPtr ;

public:

	CViewCollectionWalker(void);
public:

	~CViewCollectionWalker(void);

	template< typename MatchContainerType >
	void EraseCurrentRecord( SearchMatchType &match, MatchContainerType &matches, const wstring &id, MemoryControllerType memories )
	{
		BANNER("CViewCollectionWalker::EraseCurrentRecord") ;
		if ( matches.empty())
		{
			return ;
		}
		match = matches.at( boost::lexical_cast< long >( id ) ) ;

		try
		{
			memories->remove_record( match->get_record(), match->get_memory_id() ) ;
			ATLTRACE(" ... Record erased\n\n") ;
		}
		catch (except::CProgramException &e)
		{
			logging::log_error("Failed to delete record: memory not found") ;
			logging::log_exception(e) ;
			e.notify_user("Failed to delete record: memory not found") ;
		}
	}

	template< typename MatchContainerType >
	void RecordsToElements( MatchContainerType &matches, html::collection_ptr collection )
	{
		record_pointer rec(new record_local()) ;

		// loop through each of the elements
		for ( int i=0 ; i < collection->length ; ++i )
		{
			MSHTML::IHTMLElementPtr element = collection->item(_variant_t( i ), _variant_t(0) ) ;

			wstring id = BSTR2wstring( element->id ) ;

			if ( id.empty() == false )
			{
				if ( str::is_int_rep( id ) )
				{
					SearchMatchType match = matches.at( boost::lexical_cast< long >( id ) ) ;
					rec = match->get_record() ;
				}
				else
				{
					RecordToElement( element, rec ) ;
				}
			}
		}
	}

	void CheckLinkUrl( element_wrapper_ptr element, const wstring doc_path );
	void RepairLinkUrl( element_wrapper_ptr element, const wstring &doc_path );
	void SetReliabilityFromElement( element_wrapper_ptr element, record_pointer & record );
	void SetValidatedFromElement( element_wrapper_ptr element, record_pointer & record );
	void SetItemFromElement( element_wrapper_ptr element, record_pointer rec, wstring id );

	void ElementToRecord( element_wrapper_ptr element, record_pointer rec );

	void AddRecordToList( record_pointer rec, 
		MemoryControllerType memories, 
		SearchMatchType match, 
		MatchListType &match_list );

	void RecordToElement( MSHTML::IHTMLElementPtr &element, record_pointer &rec );

};
