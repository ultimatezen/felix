#include "StdAfx.h"
#include ".\navurl.h"
#include "StringEx.h"

#include "textstream_reader.h"
#include "StringConversions.h" // string2long

CNavUrl::CNavUrl(CNavInterface *iface) :
	m_interface(iface),
	m_should_cancel(true)
{
}

CNavUrl::~CNavUrl(void)
{
}

// Process a URL from BeforeNavigate2
void CNavUrl::process_url(const _bstr_t& url)
{
	m_should_cancel = true ;

	token_vec tokens ;
	tokenize_url( url, tokens ) ;

	CHrefTags tags ;

	if ( ! tokens.empty() ) 
	{
		switch( tags.get_tag_code( tokens[0] ) ) 
		{
		// added trans page
		case CHrefTags::TAG_EDIT:
			m_interface->OnNavEdit( string2long( tokens[1] ) ) ;
			return ;
		case CHrefTags::TAG_DELETE:
			m_interface->OnNavDelete( string2long( tokens[1] ) ) ;
			return ;
		case CHrefTags::TAG_ADD_GLOSS:
			m_interface->OnNavAddGloss( string2long( tokens[1] ) ) ;
			return ;
		case CHrefTags::TAG_ADD_TO_GLOSS:
			m_interface->OnNavAddToGloss( string2long( tokens[1] ) ) ;
			return ;
		}
	}

	if ( is_url( url ) ) 
	{
		m_should_cancel = false ;
		return ;
	}
}

/** Tells the caller whether navigation should be canceled.
	We will cancel navigation if we have handled the URL ourselves.
	@retval bool Should navigation be canceled?
 */ 
bool CNavUrl::should_cancel(void)
{
	return m_should_cancel;
}

// Process the url into tokens
void CNavUrl::tokenize_url(const _bstr_t& url, token_vec& tokens)
{
	textstream_reader< wchar_t > reader( url ) ;

	reader.jump_to_first_of( L':', true )  ;

	wstring token ;
	while ( reader.empty() == false ) 
	{
		reader.getline( token, L":|" ) ;
		tokens += token ;
	}
}

bool CNavUrl::is_url(const _bstr_t& url)
{
	CString url_tchar = CW2T( url ) ;
	return !! ::PathIsURL( url_tchar ) || !! ::PathFileExists(url_tchar) ;
}
