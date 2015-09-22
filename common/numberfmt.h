#pragma once

/*!
 * Class used to format numbers with commas.
 */
class CNumberFmt
{
	const static int INBUFSIZE = 35 ;
	const static int OUTBUFSIZE = 40 ;
	const static int SEPSIZE = 5 ;
	TCHAR m_DecSep[SEPSIZE];
	TCHAR m_ThousandsSep[SEPSIZE];
	TCHAR in_buffer[INBUFSIZE] ;
	TCHAR out_buff[OUTBUFSIZE]  ;

	NUMBERFMT m_nf ;

public:


	/*! Default CTor.
	 */
	CNumberFmt(void)
	{
		// initialize arrays
		ZeroMemory(m_DecSep, SEPSIZE * sizeof( TCHAR )) ;
		ZeroMemory(m_ThousandsSep, SEPSIZE * sizeof( TCHAR )) ;
		ZeroMemory(in_buffer, INBUFSIZE * sizeof(TCHAR)) ;
		ZeroMemory(out_buff, OUTBUFSIZE * sizeof(TCHAR)) ;

		// get our separators
		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &m_DecSep[0], SEPSIZE );
		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, &m_ThousandsSep[0], SEPSIZE );

		m_nf.lpDecimalSep = m_DecSep;
		m_nf.lpThousandSep = m_ThousandsSep;

		/* I want no fractions */
		m_nf.NumDigits = 0;

		const size_t BUFF_SIZE = 10 ;
		TCHAR buff[BUFF_SIZE];

		/* But all the system defaults for the others */
		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_ILZERO, &buff[0], BUFF_SIZE );
		m_nf.LeadingZero = _ttoi( buff );

		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SGROUPING, &buff[0], BUFF_SIZE ) ;
		m_nf.Grouping = _ttoi( buff );

		GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, &buff[0], BUFF_SIZE ) ;
		m_nf.NegativeOrder = _ttoi( buff );
	}


	~CNumberFmt(void)
	{
	}

	/*! Formats an integer into the buffer.
	 */
	CString Format(int num)
	{
		/*
		errno_t _itow_s(
		int value,
		wchar_t *buffer,
		size_t sizeInCharacters,
		int radix 
		);
		Returns 0 on success.
		*/
		errno_t result = _itot_s( num, in_buffer, INBUFSIZE, 10 ) ;
		if (result)
		{
			ATLTRACE("Number format failed with code %i\n", result) ;
			ATLASSERT(FALSE && "Failed to convert number into string") ;
			return CString() ;
		}

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