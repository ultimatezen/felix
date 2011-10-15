#include "StdAfx.h"
#include "tag_name_holder.h"
#include "resource.h"
#include "StringConversions.h"

/************************************************************************/
/* tag_name_holder                                                      */
/************************************************************************/

tag_name_holder::tag_name_holder() :

query_tag			( L"query" ),
source_tag			( L"source" ), 
trans_tag			( L"trans" ),
context_tag			( L"context" ),
reliability_tag		( L"reliability" ),
validated_tag		( L"validated" ),
record_tag			( L"record" ), 
date_created_tag	( L"date_created" ),	
last_modified_tag	( L"last_modified" ),	
refcount_tag		( L"ref_count" ),

dummy_source_id( L"dummy_source" ),
dummy_trans_id( L"dummy_trans" ),	
refcount_id( L"ref_count" ),
created_id( L"date_created" ),
modified_id( L"modified" ),
memory_id( L"memory" ),
rich_query_id( L"rich_query" ),
rich_source_id( L"rich_source" ),


record_tag_narrow		( "<record>" ), 
source_tag_narrow		( "<source>" ), 
trans_tag_narrow		( "<trans>" ),
date_created_tag_narrow	( "<date_created>" ),	
last_modified_tag_narrow( "<last_modified>" ),	
reliability_tag_narrow	( "<reliability>" ),		
validated_tag_narrow	( "<validated>" ),		
context_tag_narrow		( "<context>" ),
refcount_tag_narrow		( "<ref_count>" ),

// end tags		
record_tag_end			( L"</record>" ), 
source_tag_end			( L"</source>" ), 
trans_tag_end			( L"</trans>" ),
date_created_tag_end	( L"</date_created>" ),	
last_modified_tag_end	( L"</last_modified>" ),	
reliability_tag_end		( L"</reliability>" ),		
validated_tag_end		( L"</validated>" ),		
context_tag_end			( L"</context>" ),		
refcount_tag_end		( L"</ref_count>" ),

record_tag_end_narrow			( "</record>" ), 
source_tag_end_narrow			( "</source>" ), 
trans_tag_end_narrow			( "</trans>" ),
date_created_tag_end_narrow		( "</date_created>" ),	
last_modified_tag_end_narrow	( "</last_modified>" ),	
reliability_tag_end_narrow		( "</reliability>" ),		
validated_tag_end_narrow		( "</validated>" ),		
context_tag_end_narrow			( "</context>" ),		
refcount_tag_end_narrow			( "</ref_count>" )
{
}

bool tag_name_holder::is_query_tag( const wstring &tagname )
{
	return tagname == this->query_tag ;
}

