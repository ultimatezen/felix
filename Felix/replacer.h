#pragma once

#include "record.h"
#include "miscwrappers.h"

namespace replacer
{
	using mem_engine::record_pointer ;

	void mod_date(misc_wrappers::date &thedate, wstring datestring) ;


	void parse_time(const wstring &timestring, misc_wrappers::date &thedate ) ;
	void parse_date(const wstring &datestring, misc_wrappers::date &thedate ) ;
	wstring getrest(wstring from, wstring tag) ;
	/* Do the dates match?
	   Allows year or year/month matching. For example:
	    2010 == 2010-10-10
		2010-10 == 2010-10-10
	*/
	bool dates_match(const misc_wrappers::date query,
		const misc_wrappers::date entry) ;

	/* Is the query before the record date?
	   Allows year or year/month matching. For example:
	    2010 < 2010-10-10
		2010-10 < 2010-11-10
		BUT:
		! 2010-02 < 2010-02-01 (they are equal)
	*/
	bool date_after(const misc_wrappers::date query,
		const misc_wrappers::date entry) ;

	record_pointer do_replace(record_pointer rec, wstring from, wstring to) ;

	record_pointer source(record_pointer rec, wstring from, wstring to) ;

	void replace_text( wstring &old, const wstring &from, const wstring &to )  ;
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