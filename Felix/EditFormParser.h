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

// Parse the TM edit form
class EditFormParser
{
public:
	doc3_wrapper_ptr m_doc ;
	EditFormParser(doc3_wrapper_ptr doc);

	// the memory creator
	wstring creator();
	// the memory field
	wstring field();
	// date created
	wstring created_on();
	// source language
	wstring source_language();
	// target language
	wstring target_language();
	// client
	wstring client();
	// default context (context added if not supplied)
	//  -- blank to have no default context
	wstring default_context() ;
	// true if TM, false if glossary
	bool is_memory();
	// locked? (read only)
	bool locked();
	// active?
	bool is_active();

	// is checkbox checked?
	bool element_checked(const wstring key);
	// element value (text boxes)
	wstring element_value(const wstring key);
};

}