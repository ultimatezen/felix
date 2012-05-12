#pragma once

#include "document_wrapper.h"

/*
<input type="text" name="creator" id="creator" value="{$creator}" />

<input type="text" name="field" id="field" value="{$field}" />

<input type="text" name="created_on" id="created_on" value="{$created_on}" />

<input type="text" name="source_language" id="source_language" value="{$source_language}" />

<input type="target_language" name="target_language" id="target_language" value="{$target_language}" />

<input type="text" name="client" id="client" value="{$client}" /></td>

<input type="radio" name="memtype" id="mem_type_tm" value="tm" {% if memtype == "mem" %}checked="checked" {% endif %}/>
<input type="radio" name="memtype" id="mem_type_tm" value="tm" {% if memtype == "gloss" %}checked="checked" {% endif %}/>

<input type="checkbox" name="locked" id="locked" value="locked" {% if locked == "true" %}checked="checked" {% endif %}/>
*/

namespace mgrview
{

class EditFormParser
{
public:
	doc3_wrapper_ptr m_doc ;
	EditFormParser(doc3_wrapper_ptr doc);
	wstring creator();

	wstring field();
	wstring created_on();
	wstring source_language();
	wstring target_language();
	wstring client();
	wstring default_context() ;
	bool is_memory();
	bool locked();
	bool is_active();

	bool element_checked(const wstring key);
	wstring element_value(const wstring key);
};

}