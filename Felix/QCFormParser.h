#pragma once

#include "document_wrapper.h"

/*
<input type="checkbox" name="numbers" id="numbers" value="numbers"
<input type="checkbox" name="allcaps" id="allcaps" value="allcaps"
<input type="checkbox" name="gloss" id="gloss" value="gloss"
*/

namespace mgrview
{

	class QCFormParser
	{
	public:
		doc3_wrapper_ptr m_doc ;
		QCFormParser(doc3_wrapper_ptr doc)
		{
			m_doc = doc ;
		}
		bool check_numbers()
		{
			wstring ischecked = m_doc->get_element_by_id(L"numbers")->get_attribute(L"checked") ;
			return (ischecked == L"true") ;
		}
		bool check_all_caps()
		{
			wstring ischecked = m_doc->get_element_by_id(L"allcaps")->get_attribute(L"checked") ;
			return (ischecked == L"true") ;
		}
		bool check_gloss()
		{
			wstring ischecked = m_doc->get_element_by_id(L"gloss")->get_attribute(L"checked") ;
			return (ischecked == L"true") ;
		}
		bool live_check()
		{
			wstring ischecked = m_doc->get_element_by_id(L"live")->get_attribute(L"checked") ;
			return (ischecked == L"true") ;
		}
	};

}