#pragma once

#include "atlstr.h"
#include <string>

/*!
 * Class used to format numbers with commas.
 */
class CNumberFmt
{
	const static int INBUFSIZE = 35 ;
	const static int OUTBUFSIZE = 40 ;
	TCHAR m_DecSep[5];
	TCHAR m_ThousandsSep[5];
	TCHAR in_buffer[INBUFSIZE] ;
	TCHAR out_buff[OUTBUFSIZE]  ;

	NUMBERFMT m_nf ;

public:


	/*! Default CTor.
	 */
	CNumberFmt(void)
	{
		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &m_DecSep[0], 5 );
		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, &m_ThousandsSep[0], sizeof( 5 ));

		m_nf.lpDecimalSep = m_DecSep;
		m_nf.lpThousandSep = m_ThousandsSep;

		/* I want no fractions */
		m_nf.NumDigits = 0;

		TCHAR buff[5];

		/* But all the system defaults for the others */
		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_ILZERO, &buff[0], 5 );
		m_nf.LeadingZero = _ttoi( buff );

		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SGROUPING, &buff[0], 5 ) ;
		m_nf.Grouping = _ttoi( buff );

		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, &buff[0], 5 ) ;
		m_nf.NegativeOrder = _ttoi( buff );
	}


	~CNumberFmt(void)
	{
	}

	/*! Formats an integer into the buffer.
	 */
	CString Format(int num)
	{
		_itot_s( num, in_buffer, INBUFSIZE, 10 ) ;

		GetNumberFormat( LOCALE_USER_DEFAULT, 0, in_buffer, &m_nf, out_buff, OUTBUFSIZE ) ;

		return CString(out_buff) ;
	}

	/*! Formats a size_t into the buffer.
	 */
	CString Format(size_t num)
	{
		_ultot_s( static_cast< unsigned long >( num ), in_buffer, INBUFSIZE, 10 ) ;

		GetNumberFormat( LOCALE_USER_DEFAULT, 0, in_buffer, &m_nf, out_buff, OUTBUFSIZE ) ;

		return CString(out_buff) ;
	}

};

template<typename T>
std::wstring fmt_num(T num)
{
	CNumberFmt fmt ;
	return std::wstring((LPCWSTR)(fmt.Format(num))) ;
}