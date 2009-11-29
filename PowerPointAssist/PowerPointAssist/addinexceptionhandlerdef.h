#pragma once

#define ADDIN_CATCH( failureMessage ) \
	catch( _com_error &e ) \
{\
	CString msg = failureMessage ; MessageBox( NULL, e.ErrorMessage(), msg, MB_OK ) ;\
}\
	catch( CException &myException )\
{\
	CString msg = failureMessage ; myException.notify_user( msg ) ;\
}\
	catch ( std::exception &stdE )\
{\
	CString msg1 = failureMessage ; CString msg = CA2T( stdE.what() ) ;\
	MessageBox( NULL, msg, msg1, MB_OK ) ;\
}\
	catch( CAtlException &atlE )\
{\
	_com_error ce( atlE ) ;\
	CString msg = failureMessage ; MessageBox( NULL, ce.ErrorMessage(), msg, MB_OK ) ;\
}\
	catch( ... )\
{\
	CString msg = failureMessage ; MessageBox( NULL, _T("Internal error.\r\nPlease contact customer support if the problem persists."), msg, MB_OK ) ;\
}

