/*!
	Interface class for generic implementation of presenter.
 */

#pragma once

#include "HtmlView.h"

namespace content_presenter
{
	using namespace html ;

	class CContentPresenter
	{
	protected:
		CHtmlView			 *m_view ;

	public:
		CContentPresenter(CHtmlView &html_view);
		virtual ~CContentPresenter(void);

		virtual void present() = 0 ;
	};

}