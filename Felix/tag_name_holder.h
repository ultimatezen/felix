#pragma once

#include "StringEx.h"
#include "comutil.h"

/**
	tag_name_holder
	
	Holds various tag names in one class.
	In other words, this is a "data class."
	Start pulling your weight or face elimination!
 */
struct tag_name_holder
{
	// these are our id strings
	const wstring query_tag       ;
	const wstring source_tag		;
	const wstring trans_tag		;
	const wstring context_tag		;
	const wstring reliability_tag ;
	const wstring validated_tag    ;
	const wstring record_tag	 ; 
	const wstring refcount_tag	 ; 
	const wstring date_created_tag ;	
	const wstring last_modified_tag	 ;	

	// element ids
	const wstring dummy_source_id ;
	const wstring dummy_trans_id ;	
	const wstring refcount_id ;
	const wstring created_id ;
	const wstring modified_id ;
	const wstring memory_id ;
	const wstring rich_query_id ;
	const wstring rich_source_id ;

	// when we need narrow tags (utf-8 anyone?)
	const string record_tag_narrow		; 
	const string source_tag_narrow		; 
	const string trans_tag_narrow			;
	const string date_created_tag_narrow	;	
	const string last_modified_tag_narrow	;	
	const string reliability_tag_narrow	;		
	const string validated_tag_narrow		;		
	const string context_tag_narrow		;
	const string refcount_tag_narrow		;

	// end tags		
	const _bstr_t record_tag_end			; 
	const _bstr_t source_tag_end			; 
	const _bstr_t trans_tag_end			;
	const _bstr_t date_created_tag_end	;	
	const _bstr_t last_modified_tag_end	;	
	const _bstr_t reliability_tag_end	;		
	const _bstr_t validated_tag_end		;		
	const _bstr_t context_tag_end		;		
	const _bstr_t refcount_tag_end		;

	const string record_tag_end_narrow			; 
	const string source_tag_end_narrow			; 
	const string trans_tag_end_narrow				;
	const string date_created_tag_end_narrow		;	
	const string last_modified_tag_end_narrow		;	
	const string reliability_tag_end_narrow		;		
	const string validated_tag_end_narrow			;		
	const string context_tag_end_narrow			;		
	const string refcount_tag_end_narrow			;

	static tag_name_holder& instance()
	{
		static tag_name_holder holder ;
		return holder ;
	}
private:
	tag_name_holder() ;
} ;

