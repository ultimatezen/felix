#pragma once

#include "StringEx.h"
#include "comutil.h"

/**
	@class tag_name_holder
	@brief Holds various tag names in one class.
	In other words, this is a "data class."
	Start pulling your weight or face elimination!

	[2006-10-01] -- made factory class
 */
class tag_name_holder
{
public:

	// these are our id strings
	wstring query_tag       ;
	wstring source_tag		;
	wstring trans_tag		;
	wstring context_tag		;
	wstring reliability_tag ;
	wstring validated_tag    ;
	wstring record_tag	 ; 
	wstring refcount_tag	 ; 
	wstring date_created_tag ;	
	wstring last_modified_tag	 ;	

	// element ids
	wstring dummy_source_id ;
	wstring dummy_trans_id ;	
	wstring refcount_id ;
	wstring created_id ;
	wstring modified_id ;
	wstring memory_id ;
	wstring rich_query_id ;
	wstring rich_source_id ;

	// when we need narrow tags (utf-8 anyone?)
	string record_tag_narrow		; 
	string source_tag_narrow		; 
	string trans_tag_narrow			;
	string date_created_tag_narrow	;	
	string last_modified_tag_narrow	;	
	string reliability_tag_narrow	;		
	string validated_tag_narrow		;		
	string context_tag_narrow		;
	string refcount_tag_narrow		;

	// end tags		
	_bstr_t record_tag_end			; 
	_bstr_t source_tag_end			; 
	_bstr_t trans_tag_end			;
	_bstr_t date_created_tag_end	;	
	_bstr_t last_modified_tag_end	;	
	_bstr_t reliability_tag_end	;		
	_bstr_t validated_tag_end		;		
	_bstr_t context_tag_end		;		
	_bstr_t refcount_tag_end		;

	string record_tag_end_narrow			; 
	string source_tag_end_narrow			; 
	string trans_tag_end_narrow				;
	string date_created_tag_end_narrow		;	
	string last_modified_tag_end_narrow		;	
	string reliability_tag_end_narrow		;		
	string validated_tag_end_narrow			;		
	string context_tag_end_narrow			;		
	string refcount_tag_end_narrow			;

	static tag_name_holder& instance()
	{
		static tag_name_holder holder ;
		return holder ;
	}
private:
	tag_name_holder() ;
} ;

