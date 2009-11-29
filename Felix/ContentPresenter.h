/*!
	@file ContentPresenter.h
	@brief Interface file for CContentPresenter class.
	@date 2005/07/05
	Time: 18:48:05
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include "HtmlView.h"

/**
	@namespace content_presenter
	@brief Namespace for content presenters.
 */
namespace content_presenter
{
	using namespace html ;
/**
	@class CContentPresenter
	@brief Base class for content presenters.
 */
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