#include "StdAfx.h"
#include "globalApp.h"

PowerPoint::_ApplicationPtr mApp ;

void globalApp::set( PowerPoint::_ApplicationPtr app )
{
	mApp = app ;
}
PowerPoint::_ApplicationPtr globalApp::get()
{
	return mApp ;
}
