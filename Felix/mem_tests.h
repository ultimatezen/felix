#pragma once

#include "memory_local.h"
#include "record_local.h"

using namespace mem_engine ;
using namespace except ;

inline record_pointer make_record(wstring source, wstring trans)
{
	record_pointer rec(new record_local) ;
	rec->set_source(source) ;
	rec->set_trans(trans) ;
	return rec ;
}
inline record_pointer make_record(string source, string trans)
{
	return make_record(string2wstring(source), string2wstring(trans)) ;
}
inline record_pointer add_record(memory_local &mem, wstring source, wstring trans)
{
	record_pointer rec = make_record(source, trans) ;
	mem.add_record(rec) ;
	mem.set_minimum_score(50) ;
	return rec ;
}
inline record_pointer add_record(memory_local &mem, string source, string trans)
{
	return add_record(mem, string2wstring(source), string2wstring(trans)) ;
}

inline record_pointer add_record(memory_pointer mem,  string source, string trans)
{
	record_pointer rec = make_record(source, trans) ;
	mem->add_record(rec) ;
	mem->set_minimum_score(50) ;
	return rec ;
}
