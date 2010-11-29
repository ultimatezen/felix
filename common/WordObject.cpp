// WordObject.cpp: implementation of the WordObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WordObject.h"
#include "StringEx.h"
#include "Exceptions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WordObject::WordObject()
: WordWasRunning( false ) 
{
	HRESULT hRes = ::OleInitialize(NULL) ;
	ATLASSERT( SUCCEEDED( hRes ) ) ;

	WordWasRunning = GetRunningObject() ;
	if ( WordWasRunning )
		return ;

	HRESULT hr = LaunchWord() ;
	if ( FAILED( hr ) )
		throw Exception( TEXT("Failed to launch Word"), hr ) ;
	wdApp.vt = VT_DISPATCH;
	ATLTRACE( "\t*** WordObject: Launched word\n") ;
}

WordObject::~WordObject()
{
	if ( ! WordWasRunning && IsRunning() )
		Quit() ;
	::OleUninitialize() ; 
}

bool WordObject::SetVisible(bool IsVisible)
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

	long visibility ;

	if ( IsVisible )
		visibility = 1 ;
	else visibility = 0 ;

    MyVariant rVal (visibility) ; // Temporary result holder
   // wdApp . Visible = 1 
	MyVariant LevelOne( wdApp ) ;
    AutoWrap(DISPATCH_PROPERTYPUT, NULL, LevelOne.pdispVal, L"Visible", 1, rVal);
	ATLTRACE( "\t*** WordObject: Set visible to %s\n", IsVisible ? "true" : "false" ) ;

	return true ;
}

bool WordObject::OpenFile(const TCHAR *const FileName)
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

	// wdApp . Documents . Open hello.txt 
	MyVariant LevelOne(wdApp) ;
	MyVariant LevelTwo ;
    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelTwo, LevelOne.pdispVal, L"Documents", 0);

	BasicString BFileName( FileName ) ;
	MyVariant Parameter( ::SysAllocString(BFileName) ) ;
    AutoWrap(DISPATCH_METHOD, NULL, LevelTwo.pdispVal, L"Open", 1, Parameter);
	ATLTRACE( "\t*** WordObject: Opened file %s\n", FileName ) ;

	return true ;

}

bool WordObject::SaveFileAs(const TCHAR *const FileName)
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

   // wdApp . ActiveDocument . SaveAs hello_WORKING.txt , 0 
	MyVariant LevelOne(wdApp) ;
	MyVariant LevelTwo ;
    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelTwo, LevelOne.pdispVal, L"ActiveDocument", 0);
	BasicString BFileName( FileName ) ;
	MyVariant Parameter0( ::SysAllocString(BFileName) ) ;
	MyVariant Parameter1( 0L ) ;
    AutoWrap(DISPATCH_METHOD, NULL, LevelTwo.pdispVal, L"SaveAs", 2, Parameter1, Parameter0 );
	ATLTRACE( "\t*** WordObject: Saved file as %s\n", FileName ) ;

	return true ;
}

bool WordObject::Quit()
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

    AutoWrap(DISPATCH_METHOD, NULL, wdApp.pdispVal, L"Quit", 0);
	wdApp.Clear() ;
	ATLTRACE( "\t*** WordObject: Quit from word\n" ) ;
	return true ;
}

bool WordObject::CloseActiveDocument()
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

	MyVariant LevelOne(wdApp) ;
	MyVariant LevelTwo ;
    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelTwo, LevelOne.pdispVal, L"ActiveWindow", 0);
    AutoWrap(DISPATCH_METHOD, NULL, LevelTwo.pdispVal, L"Close", 0);
	ATLTRACE( "\t*** WordObject: Closed active document\n" ) ;
	return true ;
}

bool WordObject::AddCustomProperty(const TCHAR *const Property, const TCHAR *const Value)
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

	// Word will return an error if the property we are trying to add already exists
	DeleteCustomProperty( Property ) ;

    // Line 8: wdApp . activedocument . CustomDocumentProperties . Add foo , 0 , 4 , bar , 0 
	MyVariant LevelOne(wdApp) ;
	MyVariant LevelTwo ;
	MyVariant LevelThree ;

    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelTwo, LevelOne.pdispVal, L"activedocument", 0);
    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelThree, LevelTwo.pdispVal, L"CustomDocumentProperties", 0);
	MyVariant	parm[5] ;
	BasicString bProp(Property) ;
	parm[0].vt = VT_BSTR; parm[0].bstrVal = ::SysAllocString(bProp); // Name
    parm[1].vt = VT_I4; parm[1].lVal = 0;	// don't link to content
    parm[2].vt = VT_I4; parm[2].lVal = 4;	// text property
	BasicString bVal(Value) ;				
    parm[3].vt = VT_BSTR; parm[3].bstrVal = ::SysAllocString(bVal);  // Value
    parm[4].vt = VT_I4; parm[4].lVal = 0;   // ignored, since parm[1] is false
    AutoWrap(DISPATCH_METHOD, NULL, LevelThree.pdispVal, L"Add", 5, parm[4], parm[3], parm[2], parm[1], parm[0]);

	ATLTRACE( "\t*** WordObject: Added custom property: %s / %s\n", Property, Value ) ;
	return true ;
}

// DeleteCustomProperty
// Parameters
//		const TCHAR *const Property		-- the property to delete
// Return
// bool
//		true: success
//		false: failure
bool WordObject::DeleteCustomProperty(const TCHAR *const Property)
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

	int NumProperties = GetCustomPropertyCount() ;
    MyVariant rVal; // Temporary result holder

	MyVariant LevelOne(wdApp) ;
	MyVariant LevelTwo ;
	MyVariant LevelThree ;

	MyVariant param ;
	param.vt = VT_I4;

	CComBSTR Name(Property) ;

	// If wdApp . activedocument . CustomDocumentProperties i . Name = Property Then 
    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelTwo, LevelOne.pdispVal, L"ActiveDocument", 0);
	for ( param.lVal = 1 ;  param.lVal <= NumProperties ;  param.lVal++ )
	{
        AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelThree, LevelTwo.pdispVal, L"CustomDocumentProperties", 1, param);
        AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &rVal, LevelThree.pdispVal, L"Name", 0);

		if ( ! wcscmp( (BSTR)Name, rVal.bstrVal ) )
		{
            // wdApp . activedocument . CustomDocumentProperties i . Delete 
			AutoWrap(DISPATCH_METHOD, NULL, LevelThree.pdispVal, L"Delete", 0);
			return true ;
        }
		LevelTwo.Clear() ;
		LevelThree.Clear() ;
    }
	ATLTRACE( "\t*** WordObject: Deleted custom property %s\n", Property) ;


	return true ;
}

int WordObject::GetCustomPropertyCount()
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

    // Line Dim i As Integer 
    MyVariant i ;

	MyVariant LevelOne(wdApp) ;
	MyVariant LevelTwo ;
	MyVariant LevelThree ;

    // Line 7: i = wdApp . activedocument . CustomDocumentProperties . Count 
    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelTwo, LevelOne.pdispVal, L"activedocument", 0);
    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelThree, LevelTwo.pdispVal, L"CustomDocumentProperties", 0);
    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &i, LevelThree.pdispVal, L"Count", 0);

	ATLTRACE( "\t*** WordObject: Document has %d custom properties\n", i.lVal ) ;

	return i.lVal ;
}

bool WordObject::AddNewDocument()
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

    // Line 5: wdApp . Documents . Add 
	MyVariant LevelOne(wdApp) ;
	MyVariant LevelTwo ;

    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelTwo, LevelOne.pdispVal, L"Documents", 0);
    AutoWrap(DISPATCH_METHOD, NULL, LevelTwo.pdispVal, L"Add", 0);

	ATLTRACE( "\t*** WordObject: Added new document\n" ) ;
	return true ;
}

bool WordObject::SaveChanges()
{
	ATLASSERT( wdApp.vt == VT_DISPATCH ) ;

	MyVariant LevelOne(wdApp) ;
	MyVariant LevelTwo ;

    AutoWrap(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &LevelTwo, LevelOne.pdispVal, L"ActiveDocument", 0);
    AutoWrap(DISPATCH_METHOD, NULL, LevelTwo.pdispVal, L"Save", 0);

	ATLTRACE( "\t*** WordObject: Saved changes\n" ) ;

	return true ;
}

bool WordObject::GetRunningObject()
{
	ATLASSERT( wdApp.vt == VT_EMPTY ) ;

    // Set wdApp = GetObject , Word.Application 
    CLSID clsid;
    IUnknown *pUnk;
    CLSIDFromProgID(L"Word.Application", &clsid);
    HRESULT hr = GetActiveObject(clsid, NULL, (LPUNKNOWN *)&pUnk);
    if(FAILED(hr)) 
	{
		ATLTRACE( "GetActiveObject() for \"Word.Application\" failed.\n") ;
		AtlTraceErrorRecords( hr ) ;
		return false ;
    }
    wdApp.vt = VT_DISPATCH;
    pUnk->QueryInterface(IID_IDispatch, (void **)&wdApp.pdispVal);
    pUnk->Release();
	ATLTRACE( "\t*** WordObject: Grabbed running instance of word\n") ;
	return true ;
}

HRESULT WordObject::LaunchWord()
{
	ATLASSERT( wdApp.vt == VT_EMPTY ) ;

	CLSID clsid;
	CLSIDFromProgID(L"word.application", &clsid);
	HRESULT hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER|CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&wdApp.pdispVal);
	ATLASSERT( SUCCEEDED( hr ) ) ;
	return hr ;
}

bool WordObject::IsRunning()
{
	return ( wdApp.vt == VT_DISPATCH ) ;
}
