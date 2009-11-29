/*!
 * Header file for class CFelixString
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "stringex.h"
#include "tag_stripper.h"

/*!
 * \brief
 * Wraps a string from Felix.
 *
 * Wraps a string from Felix (BSTR) so that we can feed BSTR's/VARIANT's
 * to Excel as needed. Also strips the tags from the Felix string,
 * because we only support plain text...
 */
class CFelixString
{
	/*!
	 * \brief
	 * Internal representation of the translation string.
	 */
	wstring m_data ;
public:
	/*!
	 * \brief
	 * Copy constructor.
	 * 
	 * \param text
	 * The text to set.
	 */
	CFelixString( const _bstr_t &text )
	{
		internal_copy( text ) ;
	}
	virtual ~CFelixString(){}
	/*!
	 * \brief
	 * Assignment operator.
	 * 
	 * \param text
	 * The text to assign.
	 */
	CFelixString& operator=( const _bstr_t &text )
	{
		internal_copy( text ) ;
	}
	/*!
	 * \brief
	 * Retrieves the translation as a BSTR.
	 * 
	 * \returns
	 * The translation as a BSTR.
	 */
	_bstr_t as_bstring()
	{
		return _bstr_t( m_data.c_str( ) ) ;
	}
	/*!
	 * \brief
	 * Retrieves the translation string as a _variant_t.
	 * 
	 * \returns
	 * The translation as a variant
	 */
	_variant_t as_variant()
	{
		return _variant_t( m_data.c_str( ) ) ;
	}
private:
	/*!
	 * \brief
	 * Internal copy routine.
	 * 
	 * \param text
	 * The text to set.
	 * 
	 * To avoid duplicating the BSTR->wstring conversion code in the constructor and 
	 * assignment operator.
	 */
	void internal_copy( const _bstr_t &text )
	{
		wstring tmp( (LPCWSTR)text, text.length() ) ;
		m_data = strip_tags( tmp ) ;
	}
};