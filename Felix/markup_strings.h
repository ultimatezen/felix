#pragma once

#include "StringEx.h"

namespace mem_engine
{

	class markup_strings
	{	
	private:
		wstring m_query;
		wstring m_source;
		wstring m_trans;
		wstring m_context;

	public:
		markup_strings(void){}

		void SetQuery(const std::wstring& query)
		{
			m_query = query;
		}

		const std::wstring& GetQuery() const
		{
			return m_query;
		}

		void SetSource(const std::wstring& source)
		{
			m_source = source;
		}

		const std::wstring& GetSource() const 
		{
			return m_source;
		}

		void SetTrans(const std::wstring& trans)
		{
			m_trans = trans;
		}

		const std::wstring& GetTrans() const 
		{
			return m_trans;
		}

		void SetContext(const std::wstring& context)
		{
			m_context = context;
		}

		const std::wstring& GetContext() const 
		{
			return m_context;
		}

	};

	typedef std::shared_ptr< markup_strings > markup_ptr ;
}