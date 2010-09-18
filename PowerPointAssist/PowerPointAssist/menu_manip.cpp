#include "stdafx.h"
#include "menu_manip.h"
#include "Picture.h"
#include "DispatchWrapper.h"
#include "logging.h"

using namespace except ;

bool load_picture(Office::_CommandBarButtonPtr &button, int button_id)
{
	try
	{
		CPicture pic ;
		BOOL success = pic.Load( button_id ) ;
		if ( ! success ) 
		{
			logging::log_warn("Failed to load picture; pasting instead") ;
			return false ;
		}
		_variant_t vPicture ( static_cast< IUnknown* >( pic ) ) ;
		CDispatchWrapper wrapper( CComPtr< IDispatch >(static_cast< IDispatch* >( button ) ) );
		wrapper.prop_put( L"Picture", vPicture ) ;
	}
	catch( CException &e )
	{
		logging::log_error("Error loading picture; pasting instead") ;
		logging::log_exception(e) ;

		return false ;
	}

	return true ;

}


bool item_needs_killing( _bstr_t caption )
{
	wstring candidate_for_death = (LPCWSTR)caption ;
	boost::replace_all( candidate_for_death, L"&", L"" ) ;
	boost::to_lower( candidate_for_death ) ;

	wstring marked_for_death(L"felix") ;
	return ( candidate_for_death.find( marked_for_death ) != wstring::npos  ) ;
}