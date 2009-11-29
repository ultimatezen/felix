// view_controller.cpp: implementation of the view_controller class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "view_controller.h"
#include "ColorRef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

view_controller::view_controller()
{

}

view_controller::~view_controller()
{

}

HWND view_controller::create( HWND parent )
{
	ATLASSERT( IsWindow(parent) )  ;
	m_view.AddRef() ;
	CWindow client = m_view.create( parent ) ;
	ATLASSERT( client.IsWindow() )  ;
	
	return client ;
}

// Function name	: clean_up_urls
// Description	    : 
// Return type		: void 
// Argument         : CComPtr< IHTMLElementCollection > &collection
void view_controller::clean_up_urls( CHtmlElementCollection &collection )
{
	BANNER( "CCommonWindowFunctionality::clean_up_urls" ) ;
	
	CHtmlDocument doc = m_view.get_document() ;
	wstring doc_url = doc.get_url() ;
	TRACE( doc_url ) ;
	
	size_t pos = doc_url.find_last_of( L'/' )	;
	// we add one to the length to get the slash
	wstring doc_path = str::left( doc_url, pos + 1 ) ;
	// get the length of the collection
	int len = collection.get_length() ;
	
	for ( int i=0 ; i<len ; ++i )
	{
		TRACE( i ) ;
		CHtmlElement element = collection.item( i ) ;
		wstring tag_name = element.get_tag_name() ;
		
		if ( str::equal_nocase( tag_name, L"A" ))
		{
			wstring key = L"HREF" ;
			CComVariant var_href = element.get_attribute( key ) ;
			ATLASSERT( var_href.vt == VT_BSTR ) ;
			wstring href ( (LPCWSTR)var_href.bstrVal, SysStringLen( var_href.bstrVal) )  ;
			if ( str::left( href, doc_path.length() ) == doc_path )
			{
				wstring relative_url = href.substr( doc_path.length() )	;
				element.set_attribute( key, relative_url ) ;
			}
		}
	} // increment i...
}
CComPtr< IHTMLElementCollection > view_controller::get_element_collection()
{
	CHtmlDocument doc = m_view.get_document() ;
	return doc.get_all() ;
}

// OnBold
// There is no default command for bold, so we have to intercept it 
// here and pass it down to our HTML view, in order to keep the view generic
// (i.e. no dependency on resource.h")
LRESULT view_controller::OnBold( )
{
	BANNER( "CCommonWindowFunctionality::OnBold" ) ;
	m_view.do_bold() ;
	return 0L ;
}

// OnUnderline
// There is no default command for bold, so we have to intercept it 
// here and pass it down to our HTML view, in order to keep the view generic
// (i.e. no dependency on resource.h")
LRESULT view_controller::OnUnderline( )
{
	BANNER( "CCommonWindowFunctionality::OnUnderline" ) ;
	m_view.do_underline() ;
	return 0L ;
}

// OnItalic
// There is no default command for italic, so we have to intercept it 
// here and pass it down to our HTML view, in order to keep the view generic
// (i.e. no dependency on resource.h")
LRESULT view_controller::OnItalic( )
{
	BANNER( "CCommonWindowFunctionality::OnItalic" ) ;
	m_view.do_italic() ;
	return 0L ;
}

// Function name	: CGlossaryDialog::OnFormatBackgroundColor
// Description	    : 
// Return type		: LRESULT 
bool view_controller::format_background_color()
{
	BANNER( "CGlossaryDialog::OnFormatBackgroundColor" ) ;
	
	COLORREF color = get_background_color() ;

	// query user for color
	CColorDialog dialog( color ) ; // current color is default
	
	if ( dialog.DoModal() == IDCANCEL )
		return false ;
	
	// get the color the user picked
	CColorRef color_ref = dialog.GetColor() ;
	
	doc.set_bg_color( color_ref.as_wstring() ) ;

	return true ;
}

COLORREF view_controller::get_background_color()
{
	CHtmlDocument doc = m_view.get_document() ;
	
	CColorRef color = doc.get_bg_color() ;

	return (COLORREF)color ;
}

void view_controller::new_glossary_added()
{
	m_view.set_body_text( wstring() ) ;
}

