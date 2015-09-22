/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "stringex.h"
#include "atlstr.h"

/*!
 * \brief
 * Write brief comment for CExcelString here.
 * 
 * Write detailed description for CExcelString here.
 * 
 * \remarks
 * Write remarks for CExcelString here.
 * 
 * \see
 * Separate items with the '|' character.
 */
class CExcelString
{
	_bstr_t m_data ;
public:

	/*!
	 * \brief
	 * Write brief comment for CExcelString here.
	 * 
	 * \param init_value
	 * Description of parameter init_value.
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for CExcelString here.
	 * 
	 * \remarks
	 * Write remarks for CExcelString here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	CExcelString(const _variant_t &init_value) :
	  m_data( init_value.bstrVal )
	{
		normalize() ;
	}
	/*!
	 * \brief
	 * Write brief comment for CExcelString here.
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for CExcelString here.
	 * 
	 * \remarks
	 * Write remarks for CExcelString here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	CExcelString(void)
	{
	}

	/*!
	 * \brief
	 * Write brief comment for ~CExcelString here.
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for ~CExcelString here.
	 * 
	 * \remarks
	 * Write remarks for ~CExcelString here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	~CExcelString(void)
	{
	}

	/*!
	 * \brief
	 * Write brief comment for operator = here.
	 * 
	 * \param rhs
	 * Description of parameter rhs.
	 * 
	 * \returns
	 * Write description of return value here.
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for operator = here.
	 * 
	 * \remarks
	 * Write remarks for operator = here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	CExcelString& operator=( const _variant_t &rhs )
	{
		m_data = rhs.bstrVal ;
		normalize() ;
		return *this ;
	}
	/*!
	 * \brief
	 * Write brief comment for empty here.
	 * 
	 * \returns
	 * Write description of return value here.
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for empty here.
	 * 
	 * \remarks
	 * Write remarks for empty here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	bool empty()
	{
		return m_data.length() == 0 ;
	}
	/*!
	 * \brief
	 * Write brief comment for as_bstr here.
	 * 
	 * \returns
	 * Write description of return value here.
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for as_bstr here.
	 * 
	 * \remarks
	 * Write remarks for as_bstr here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	_bstr_t as_bstr()
	{
		return m_data ;
	}
private:
	/*!
	 * \brief
	 * Write brief comment for normalize here.
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for normalize here.
	 * 
	 * \remarks
	 * Write remarks for normalize here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	void normalize()
	{
		if ( m_data.length() == 0) 
		{
			return ;
		}
		wstring out( (LPCWSTR)m_data ) ;
		boost::replace_all( out, L"&", L"&amp;" ) ;
		boost::replace_all( out, L"<", L"&lt;" ) ;
		boost::replace_all( out, L">", L"&gt;" ) ;
		m_data = out.c_str() ;
	}
};
