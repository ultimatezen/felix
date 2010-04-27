#pragma once

class LCS
{
public:
	wstring longest_common_substring(const std::set<wstring> strings)
	{
		if (strings.empty())
		{
			return wstring() ;
		}

		std::set<wstring>::const_iterator pos = strings.begin(); 
		wstring first = *pos; 
		++pos ;

		while (pos != strings.end())
		{
			first = this->longest_common_substring(first, *pos) ;
			++pos ;
		}
		return first ;
	}

	wstring longest_common_substring(const wstring first, const wstring second)
	{
		if (first.empty() || second.empty())
		{
			return wstring() ;
		}
		wstring lcsub, max_str;

		for (size_t i=0; i < first.length(); i++)
		{
			for (size_t j=0; j < second.length(); j++)
			{
				for (size_t k=1; k <= first.length() && k <= second.length(); k++)
				{
					if (first.substr(i,k) == second.substr(j,k))
					{
						lcsub = first.substr(i,k);
					}
					else
					{
						if (lcsub.length() > max_str.length())
						{
							max_str=lcsub;
						}
						lcsub.clear();
					}
				}
				if (lcsub.length() > max_str.length())
				{
					max_str=lcsub;
				}
			} 
	
		}
		return max_str ;
	}
};