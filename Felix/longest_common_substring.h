#pragma once

class LCS
{
public:
	wstring longest_common_substring(const std::set<wstring> &strings) const;
	wstring longest_common_substring(const wstring &first, const wstring &second) const;
};