#include "stdafx.h"
#include "replacer.h"

namespace replacer
{

	bool startswith(wstring from, wstring tag)
	{
		return boost::trim_copy(from).substr(0, tag.size()) == tag ;
	}
	wstring getrest(wstring from, wstring tag)
	{
		return from.substr(tag.size()) ;
	}
	void mod_date(misc_wrappers::date &thedate, wstring datestring)
	{
		textstream_reader<wchar_t> reader ;
		reader.set_buffer(datestring.c_str()) ;
		std::vector<wstring> bits ;
		reader.split(bits, L"-/") ;
		if (bits.size() >= 1)
		{
			thedate.set_year(bits[0]) ;
		}
		if (bits.size() >= 2)
		{
			thedate.set_month(bits[1]) ;
		}
		if (bits.size() >= 3)
		{
			thedate.set_day(bits[2]) ;
		}

	}

	record_pointer do_replace(record_pointer rec, wstring from, wstring to)
	{
		wstring tag ;

		tag = L"source:" ;
		if (startswith(from, tag))
		{
			return source(rec, getrest(from, tag), to) ;
		}
		tag = L"trans:" ;
		if (startswith(from, tag))
		{
			return trans(rec, getrest(from, tag), to) ;
		}
		tag = L"context:" ;
		if (startswith(from, tag))
		{
			return context(rec, getrest(from, tag), to) ;
		}


		tag = L"reliability:" ;
		if (startswith(from, tag))
		{
			return reliability(rec, to) ;
		}
		tag = L"validated:" ;
		if (startswith(from, tag))
		{
			return validated(rec, to) ;
		}
		tag = L"created-by:" ;
		if (startswith(from, tag))
		{
			return created_by(rec, getrest(from, tag), to) ;
		}
		tag = L"modified-by:" ;
		if (startswith(from, tag))
		{
			return modified_by(rec, getrest(from, tag), to) ;
		}
		tag = L"refcount:" ;
		if (startswith(from, tag))
		{
			return refcount(rec, to) ;
		}

		tag = L"created:" ;
		if (startswith(from, tag))
		{
			return date_created(rec, to) ;
		}
		tag = L"modified:" ;
		if (startswith(from, tag))
		{
			return date_modified(rec, to) ;
		}

		return none_specified(rec, from, to) ;
	}

	void replace_text( wstring &old, const wstring &from, const wstring &to ) 
	{
		const wstring REGEX_TAG = L"regex:" ;
		if (boost::starts_with(from, REGEX_TAG))
		{
			const wstring pattern  = L"(" + getrest(from, REGEX_TAG) + L")" ;
			boost::wregex from_regex( pattern, boost::regex::extended|boost::regex::icase) ;
			old = boost::regex_replace(old, from_regex, to) ;
		}
		else
		{
			boost::replace_all(old, from, to) ;
		}
	}
	record_pointer source(record_pointer rec, wstring from, wstring to)
	{
		wstring old_source = rec->get_source_rich() ;
		replace_text(old_source, from, to);
		rec->set_source(old_source) ;
		return rec ;
	}
	record_pointer trans(record_pointer rec, wstring from, wstring to)
	{
		wstring old_trans = rec->get_trans_rich() ;
		replace_text(old_trans, from, to) ;
		rec->set_trans(old_trans) ;
		return rec ;
	}
	record_pointer context(record_pointer rec, wstring from, wstring to)
	{
		wstring old_context = rec->get_context_rich() ;
		replace_text(old_context, from, to) ;
		rec->set_context(old_context) ;
		return rec ;
	}
	record_pointer none_specified(record_pointer rec, wstring from, wstring to)
	{
		source(rec, from, to) ;
		trans(rec, from, to) ;
		context(rec, from, to) ;
		return rec ;
	}

	record_pointer created_by(record_pointer rec, wstring from, wstring to)
	{
		wstring old = rec->get_creator() ;
		if (from == L"*")
		{
			old = to ;
		}
		else
		{
			boost::replace_all(old, from, to) ;
		}
		rec->set_creator(old) ;
		return rec ;
	}
	record_pointer date_created(record_pointer rec, wstring created_on)
	{
		misc_wrappers::date old = rec->get_created() ;
		mod_date(old, created_on) ;
		rec->set_created(old) ;
		return rec ;
	}
	record_pointer modified_by(record_pointer rec,  wstring from, wstring to)
	{
		wstring old = rec->get_modified_by() ;
		if (from == L"*")
		{
			old = to ;
		}
		else
		{
			boost::replace_all(old, from, to) ;
		}
		rec->set_modified_by(old) ;
		return rec ;
	}
	record_pointer date_modified(record_pointer rec, wstring modified_on)
	{
		misc_wrappers::date old = rec->get_modified() ;
		mod_date(old, modified_on) ;
		rec->set_modified(old) ;
		return rec ;
	}
	record_pointer reliability(record_pointer rec, wstring reliability_score)
	{
		rec->set_reliability(boost::lexical_cast<size_t>(reliability_score)) ;
		return rec ;
	}
	record_pointer refcount(record_pointer rec, wstring ref_count)
	{
		rec->set_refcount(boost::lexical_cast<size_t>(ref_count)) ;
		return rec ;
	}

	record_pointer validated(record_pointer rec, wstring isvalidated)
	{
		rec->set_validated(string2bool(isvalidated)) ;
		return rec ;
	}

}
