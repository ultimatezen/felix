#pragma once

#include "record.h"
#include "miscwrappers.h"

namespace replacer
{
	using mem_engine::record_pointer ;

	void mod_date(misc_wrappers::date &thedate, wstring datestring) ;

	record_pointer do_replace(record_pointer rec, wstring from, wstring to) ;

	record_pointer source(record_pointer rec, wstring from, wstring to) ;
	record_pointer trans(record_pointer rec, wstring from, wstring to) ;
	record_pointer context(record_pointer rec, wstring from, wstring to) ;
	record_pointer none_specified(record_pointer rec, wstring from, wstring to) ;

	record_pointer created_by(record_pointer rec, wstring from, wstring to) ;
	record_pointer date_created(record_pointer rec, wstring created_on) ;
	record_pointer modified_by(record_pointer rec,  wstring from, wstring to) ;
	record_pointer date_modified(record_pointer rec, wstring modified_on) ;
	record_pointer reliability(record_pointer rec, wstring reliability_score) ;
	record_pointer validated(record_pointer rec, wstring isvalidated) ;
	record_pointer refcount(record_pointer rec, wstring ref_count) ;


}