#include "stdafx.h"
#include "menu_manip.h"
#include "DispatchWrapper.h"
#include "logging.h"

using namespace except ;

bool load_picture(Office::_CommandBarButtonPtr &button, int button_id)
{
	try
	{
		HINSTANCE hInst = _AtlModule.GetResourceInstance() ;

		CComPtr< IPicture > iPicture ;
		PICTDESC *pd = new PICTDESC;
		pd->cbSizeofstruct = sizeof(PICTDESC);
		pd->picType = PICTYPE_BITMAP;
		pd->bmp.hbitmap = LoadBitmap( hInst, MAKEINTRESOURCE(button_id));
		pd->bmp.hpal = 0;
		HRESULT hr = OleCreatePictureIndirect( pd, IID_IPictureDisp, FALSE, (void**)(&iPicture));
		delete pd;
		if ( FAILED( hr ) ) 
		{
			if ( hr == CTL_E_INVALIDPICTURE ) 
			{
				ATLTRACE( "%s(%d): 0x%0X Invalid picture\n", __FILE__, __LINE__, hr ) ;
				TRACE( button_id ) ;
			}
			else
			{
				TRACE_HRESULT( hr ) ;
			}
			return false ;
		}

		CDispatchWrapper but( CComPtr<IDispatch>( (IDispatch*)button ) ) ;
		_variant_t vPicture ( (IUnknown*)iPicture ) ;
		but.prop_put( L"Picture", vPicture ) ;
	}
	catch( CComException &e )
	{
		TRACE( e.what() ) ;
		TRACE( e.description() ) ;
		logging::log_error("Failed to load picture") ;
		logging::log_exception(e) ;
		return false ;

	}
	catch ( _com_error &e ) 
	{
		TRACE( e.ErrorMessage() ) ;
		TRACE_HRESULT( e.Error() ) ;
		logging::log_error("Failed to load picture") ;
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