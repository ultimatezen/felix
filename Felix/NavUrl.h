#pragma once

#include "HrefTags.h"
#include "NavInterface.h"
#include "comutil.h" // _bstr_t

#include <vector>

/**
	@class CNavUrl
	@brief Encapsulates URL navigation.
 */
class CNavUrl
{
	// typedefs
	typedef std::vector< wstring > token_vec ;
	typedef CHrefTags::TAG_NAME_CODE tag_name_code ;

	// member variables
	CNavInterface *m_interface ;
	bool				m_should_cancel ;

public:
	CNavUrl(CNavInterface *iface);
	~CNavUrl(void);

public:
	// Process a URL from BeforeNavigate2
	void process_url(_bstr_t& url);
	// @brief Tells the caller whether navigation should be cancelled.
	bool should_cancel(void);


	// Process the url into tokens
	void tokenize_url(_bstr_t& url, token_vec& tokens);
	bool is_url(_bstr_t& url);
};
