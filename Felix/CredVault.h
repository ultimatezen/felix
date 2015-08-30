#pragma once

#include <wincred.h>
#include "Exceptions.h"


/*
	Reads credentials from the cred vault.
	The Cred Vault is used to securely store
	credentials (passwords) that can only be retrieved
	by the logged-in user
*/
class CredentialReader {
	PCREDENTIALW m_credential;
public:
	CredentialReader(wstring connection) : m_credential(NULL)
	{
		BOOL success = ::CredReadW(connection.c_str(), CRED_TYPE_GENERIC, 0, &m_credential);

		if (! success)
		{
			throw except::CWinException(L"Failed to read credential");
		}
	}
	~CredentialReader()
	{
		if (m_credential)
		{
			::CredFree(m_credential);
		}
	}

	std::wstring get_password()
	{
		std::string password((char*)m_credential->CredentialBlob, m_credential->CredentialBlobSize);
		return string2wstring(password);
	}
};