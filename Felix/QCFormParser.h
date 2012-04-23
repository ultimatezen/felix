#pragma once

#include "document_wrapper.h"

/*
<input type="checkbox" name="numbers" id="numbers" value="numbers"
<input type="checkbox" name="allcaps" id="allcaps" value="allcaps"
<input type="checkbox" name="gloss" id="gloss" value="gloss"
*/

namespace mgrview
{

	class QCFormParser
	{
	public:
		doc3_wrapper_ptr m_doc ;
		QCFormParser(doc3_wrapper_ptr doc)
		{
			m_doc = doc ;
		}
		bool check_numbers();
		bool check_all_caps();
		bool check_gloss();
		bool live_check();

		void checked_glosses(const std::vector<size_t> &memids, std::vector<size_t> &existids);

		bool is_id_checked(size_t memid);
	};

}