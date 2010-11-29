/** ======================================================
	\file global_html_parser.h
	\brief Global CHtmlView object for parsing HTML
	Date:	    2004/12/15
	Time:   	20:04:50
	author:		Ryan Ginstrom

	We need to get rid of this soon. Currently it is only used to tabulate fonts.
====================================================== */

#if !defined(AFX_global_html_parser_H__71A49E41_5FCD_4071_9C79_DD4106DC118F__INCLUDED_)
#define AFX_global_html_parser_H__71A49E41_5FCD_4071_9C79_DD4106DC118F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HtmlView.h"

inline CHtmlView& global_html_parser()
{
	static CHtmlView parser ;

	return parser ;
}

inline document_ptr global_html_document()
{
	CHtmlView &parser = global_html_parser() ;
	return parser.get_document() ;
}



#endif