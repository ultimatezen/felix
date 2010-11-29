////////////////////////////////////////////////////////////////////
// Full VC++ declarative replica of VXPLib.DLL Version 1.06
////////////////////////////////////////////////////////////////////
//
// This optional class library can be used in Microsoft Visual C++
// version 6 and 7 to facilitate access to the VXPLib component.
//
// Copyright (c) 2002-2005, Tooltips.Net L.L.C.
//
// Permission to use, copy, modify this code and distribute modified code is
// hereby granted without any limitations.
//
// For details and/or updates visit www.Tooltips.NET
//
// Last updated: 20/02/2005
//
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// <Class Library Specification>
////////////////////////////////////////////////////////////////////
// The library contains 10 classes - each represents a COM interface
// from the VXPLib component:
// **************************
// - CVXPFont
// - CVXPMargins
// - CVXPSound
// - CVXPSettings
// - CVXPQuickHTML
// - CVXPTooltip
// - CVXPCmdToolList
// - CVXPWndToolList
// - CVXPTooltipManager
// - CVXPMenuHook
// **************************
//
// It also contains a few service classes and structures used within
// the class library whose names are prefixed with double underscore,
// but their meaning and functionality are just to hide overhead
// of complexity of using COM in C++, and should be ignored.
//
// Wherever it was possible the class library was designed to be 1:1
// with the COM object model implemented in VXPLib. Therefore, using
// this library will be very similar to that in a script environment
// like Visual Basic.
//
// This header file is not necessary to include in your C++ project,
// but it is recommended to do so, if you want the COM library
// to be accessible seamlessly and in a secure way. This library
// offers a safe way of instantiating and releasing COM interfaces
// of the VXPLib component. Whenever an object goes out of scope it
// is guaranteed to release the COM interface it represents.
//
// Perhaps the main power of this class library as opposed to using
// pure C++ COM support is the events handling.
// Handling events in a C++ environment can be difficult to the
// majority of programmers who never dealt with COM events in C++.
// Even though in VC++ 7 were introduced C++ Attributes it still would
// require a lot of work to do in order to make your C++ library work
// more or less like the component itself does.
//
//////////////////////////////////////////////////////////////////
// HOW TO HANDLE EVENTS OF THE LIBRARY ///////////////////////////
//////////////////////////////////////////////////////////////////
//
// If you just include this file it will give you access to all features
// of the COM library but the events. In order to enable this library
// to handle all the events add definition VXPLIB_WITH_EVENTS in your
// project before including this file. For instance, you can include
// this file like this:
//
// #define VXPLIB_WITH_EVENTS
// #include "VXPLib.h"
//
// Alternatively, you can add VXPLIB_WITH_EVENTS as a precompiler
// directive in your project.
// When having activated the use of events of the library it gives
// the library access to using ATL (Active Template Library) in order
// to handle COM events. This obliges you to provide support for ATL
// in your project. If you are already using ATL in your project there
// is nothing extra to be done to handle events. If you are not using ATL,
// your code modifications for an MFC project could look like this:
// 
// 1. Adding two lines in your StdAfx.h:
//
// #define VXPLIB_WITH_EVENTS
// #include "VXPLib.h"
//
// 2. Adding one line in your StdAfx.cpp:
//
// CComModule _Module; 
//
// 3. Adding as the very first line in your App::InitInstance():
//
// _Module.Init(NULL, GetModuleHandle(NULL));
//
// 4. Adding in your App::ExitInstance():
//
// _Module.Term();
//
// When using events in a pure Windows API application steps 3 and 4
// are to be used in the very beginning and the end of the application
// accordingly.
/////////////////////////////////////////////////////////////////////////////
// Notes:
//
// 1. It is assumed that COM support for your project has already been
// enabled, and your GUI has entered STA (Single Thread Apartment) or
// MTA (MultiThread Apartment).
// An MFC project can enter STA by calling AfxOleInit() in your
// App::InitInstance() method as the first thing, and in a non-MFC
// application by calling CoInitialize(NULL); as the first call in the
// program (GUI thread).
// Note: Function AfxOleInit does some MFC initialization and then
// calls CoInitialize(NULL);
//
// 2. Apart from functionality of the class library itself COM methods
// and properties are not documented in this file because they are called
// and work exactly as COM methods and properties they represent,
// except for return values for methods and some methods parameters,
// which were changed for easier use with C++ native types.
// Read VXPLib Documentation for information on methods, properties
// and events of the COM interfaces contained in the library.
// The latest version of documentation on VXPLib can always be found
// on www.Tooltips.NET
//////////////////////////////////////////////////////////////////////////////

#ifndef VXPLib_Header
#define VXPLib_Header

namespace vxlib
{

#pragma warning(disable:4505)
#pragma warning(push)
#pragma warning(disable:4311 4312 4278 4244 4100 4097 4189)

#ifdef VXPLIB_WITH_EVENTS
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#endif

#import "VXPLib.tlb" no_namespace named_guids

#define VXP_DECLARE(T, A) __declspec(property(get=__Get##A, put=__Put##A)) T A;
#define VXP_DECLARE_READ(T, A) __declspec(property(get=__Get##A)) T A;

///////////////////////////////////////////////////////////////////////////////////
// These two macros are to aid exception handling fired by VXPLib COM interfaces:
#define VXPLIB_BEGIN	try{
#define VXPLIB_END		}catch(_com_error &error){OnError(error.WCode(), error.Description());}
///////////////////////////////////////////////////////////////////////////////////

#define VXP_IMPLEMENT_SIMPLE(T, A, D)\
T __Get##A()\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		return (T)m_pInterface->Get##A();\
	VXPLIB_END\
	return (T)D;\
}\
void __Put##A(T NewValue)\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		m_pInterface->Put##A(NewValue);\
	VXPLIB_END\
}

#define VXP_IMPLEMENT_READ_SIMPLE(T, A, D)\
T __Get##A()\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		return (T)m_pInterface->Get##A();\
	VXPLIB_END\
	return (T)D;\
}

#define VXP_IMPLEMENT_READ_BOOL(A)\
bool __Get##A()\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		return m_pInterface->Get##A()?true:false;\
	VXPLIB_END\
	return false;\
}

#define VXP_IMPLEMENT_REF(T, V)\
T & __Get##V()\
{\
	return m_##V;\
}

#define VXP_IMPLEMENT_LONG(A)	VXP_IMPLEMENT_SIMPLE(long, A, 0)
#define VXP_IMPLEMENT_SHORT(A)	VXP_IMPLEMENT_SIMPLE(short, A, 0)

#define VXP_IMPLEMENT_STR(A)\
_bstr_t __Get##A()\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		return m_pInterface->Get##A();\
	VXPLIB_END\
	return _bstr_t();\
}\
void __Put##A(_bstr_t NewValue)\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		m_pInterface->Put##A(NewValue);\
	VXPLIB_END\
}

#define VXP_IMPLEMENT_BOOL(A)\
bool __Get##A()\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
	return m_pInterface->Get##A()?true:false;\
	VXPLIB_END\
	return false;\
}\
void __Put##A(bool NewValue)\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		m_pInterface->Put##A((BOOL)NewValue);\
	VXPLIB_END\
}

#define VXP_IMPLEMENT_HANDLE(T, A)\
T __Get##A()\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		return (T)m_pInterface->Get##A();\
	VXPLIB_END\
	return NULL;\
}

#define VXP_IMPLEMENT_HANDLE_EX(T, A)\
VXP_IMPLEMENT_HANDLE(T, A)\
void __Put##A(T NewHandle)\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		m_pInterface->Put##A((long)NewHandle);\
	VXPLIB_END\
}

#define VXP_IMPLEMENT_READ(T, A)\
T __Get##A()\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		return m_pInterface->Get##A();\
	VXPLIB_END\
	return 0;\
}

#define VXP_IMPLEMENT_READ_BOOL(A)\
bool __Get##A()\
{\
	VXPLIB_BEGIN\
	if(m_pInterface)\
		return m_pInterface->Get##A()?true:false;\
	VXPLIB_END\
	return false;\
}

// These are error codes passed to virtual function __vxperrorhandler::OnError(...)
// as the first parameter when an exception occurs:
#define ER_GENERALFAILURE		40	// Generic or unknown run-time error.
#define ER_OUTPUTEXCESS			41	// Too big output size (more than 10.000.000 pixels).
#define ER_UNSUPPORTEDCODEPAGE	42	// The code page is not supported.
#define ER_INVALIDHANDLE		43	// Invalid handle passed.
#define ER_MENUFAILURE			44	// Failed to retrieve a menu handle, or find a menu item.
#define ER_RECURSIVECALL		45	// Method RecalculateLayout has been called recursively.
#define ER_TOOLRECURSION		46	// Recursive invocation of an IVXPTooltip method.
#define ER_INDEXOUTOFRANGE		47	// Index of the requested tag CONTROL is out of range.
//////////////////////////////////////////////////////////////////////////////////

// This is a base class for all CVXPxxx classes.
// If you want to handle all exceptions supported by VXPLib
// just derive from your CVXPxxx class, and override
// method OnError(...)
class __vxperrorhandler
{
protected:
	// Error handler for all COM exceptions supported by VXPLib.
	// The first parameter is one of the ER_xxx values declared just before the class;
	// The second parameter - explanation of the error.
	virtual void OnError(HRESULT ErrorCode, LPCTSTR ErrorMsg){};
};

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxpfont:public __vxperrorhandler
{
friend class CVXPFont;

public:

	VXP_IMPLEMENT_BOOL(Transparent)
	VXP_IMPLEMENT_BOOL(Italic)
	VXP_IMPLEMENT_BOOL(Underlined)
	VXP_IMPLEMENT_BOOL(StrikeOut)
	VXP_IMPLEMENT_SHORT(Size)
	VXP_IMPLEMENT_SHORT(Escapement)
	VXP_IMPLEMENT_SHORT(Quality)
	VXP_IMPLEMENT_SHORT(PitchAndFamily)
	VXP_IMPLEMENT_SHORT(Orientation)
	VXP_IMPLEMENT_SHORT(OutPrecision)
	VXP_IMPLEMENT_SHORT(ClipPrecision)
	VXP_IMPLEMENT_HANDLE(HFONT, hFont)
	VXP_IMPLEMENT_SIMPLE(VXPFontWeight, Weight, fwDontCare)
	VXP_IMPLEMENT_SIMPLE(VXPCharSet, CharSet, csDEFAULT)
	VXP_IMPLEMENT_STR(Name)
	VXP_IMPLEMENT_SIMPLE(COLORREF, Color, RGB(0, 0, 0))
	VXP_IMPLEMENT_SIMPLE(COLORREF, BackColor, RGB(0, 0, 0))

private:

	__vxpfont():m_pInterface(NULL){}
	IVXPFont * m_pInterface;
};

/////////////////////////////////////////////////
// class CVXPFont
// This class is to contain interface IVXPFont.
// It has all the same methods and properties
// as the IVXPFont interface to access them seamlessly
// just as though working directly with the COM interface.
/////////////////////////////////////////////////
class CVXPFont:public __vxpfont
{
public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	CVXPFont(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPFont * pFont;
			::CoCreateInstance(CLSID_VXPFont, NULL, CLSCTX_ALL, IID_IVXPFont, (void**)&pFont);
			Attach(pFont);
		}
	}

	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPFont()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPFont * pFont)
	{
		Detach();
		if(pFont)
			m_pInterface = pFont;
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		if(m_pInterface)
		{
			m_pInterface->Release();
			m_pInterface = NULL;
		}
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE(bool, Transparent)
	VXP_DECLARE(bool, Italic)
	VXP_DECLARE(bool, Underlined)
	VXP_DECLARE(bool, StrikeOut)
	VXP_DECLARE(short, Size)
	VXP_DECLARE(short, Escapement)
	VXP_DECLARE(short, Quality)
	VXP_DECLARE(short, PitchAndFamily)
	VXP_DECLARE(short, Orientation)
	VXP_DECLARE(short, OutPrecision)
	VXP_DECLARE(short, ClipPrecision)
	VXP_DECLARE_READ(HFONT, hFont)
	VXP_DECLARE(VXPFontWeight, Weight)
	VXP_DECLARE(VXPCharSet, CharSet)
	VXP_DECLARE(_bstr_t, Name)
	VXP_DECLARE(COLORREF, Color)
	VXP_DECLARE(COLORREF, BackColor)

////////////
// Methods:
////////////

	void SetDefaults()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->SetDefaults();

		VXPLIB_END
	}
	bool SetFromHandle(HFONT hFont)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->SetFromHandle((long)hFont)?true:false;
		
		VXPLIB_END

		return false;
	}
	bool SetFromSystemFont(VXPSysFont FontIndex)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->SetFromSystemFont(FontIndex)?true:false;
		
		VXPLIB_END

		return false;
	}

/////////////////////////
// Overloaded operators:
/////////////////////////

	CVXPFont & operator = (CVXPFont & font)
	{
		Transparent = font.Transparent;
		Name = font.Name;
		Size = font.Size;
		Weight = font.Weight;
		Italic = font.Italic;
		Underlined = font.Underlined;
		StrikeOut = font.StrikeOut;
		Orientation = font.Orientation;
		Escapement = font.Escapement;
		CharSet = font.CharSet;
		OutPrecision = font.OutPrecision;
		ClipPrecision = font.ClipPrecision;
		Quality = font.Quality;
		PitchAndFamily = font.PitchAndFamily;
		Color = font.Color;
		BackColor = font.BackColor;
		return *this;
	}
	bool operator == (CVXPFont & font)
	{
		return (Transparent == font.Transparent && Name == font.Name && Size == font.Size && Weight == font.Weight && Italic == font.Italic && Underlined == font.Underlined && StrikeOut == font.StrikeOut && Orientation == font.Orientation && Escapement == font.Escapement && CharSet == font.CharSet && OutPrecision == font.OutPrecision && ClipPrecision == font.ClipPrecision && Quality == font.Quality && PitchAndFamily == font.PitchAndFamily && Color == font.Color && BackColor == font.BackColor);
	}
	bool operator != (CVXPFont & font)
	{
		return (Transparent != font.Transparent || Name != font.Name || Size != font.Size || Weight != font.Weight || Italic != font.Italic || Underlined != font.Underlined || StrikeOut != font.StrikeOut || Orientation != font.Orientation || Escapement != font.Escapement || CharSet != font.CharSet || OutPrecision != font.OutPrecision || ClipPrecision != font.ClipPrecision || Quality != font.Quality || PitchAndFamily != font.PitchAndFamily || Color != font.Color || BackColor != font.BackColor);
	}
};

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxpsound:public __vxperrorhandler
{
friend class CVXPSound;

public:

	VXP_IMPLEMENT_STR(FilePath)
	VXP_IMPLEMENT_STR(Res)
	VXP_IMPLEMENT_STR(Info)
	VXP_IMPLEMENT_SIMPLE(VXPPlayFrom, PlayFrom, pfCustom)
	VXP_IMPLEMENT_LONG(Size)
	VXP_IMPLEMENT_LONG(Volume)
	VXP_IMPLEMENT_LONG(Param)
	VXP_IMPLEMENT_LONG(Balance)
	VXP_IMPLEMENT_LONG(Frequency)
	VXP_IMPLEMENT_LONG(StartPos)
	VXP_IMPLEMENT_LONG(EndPos)
	VXP_IMPLEMENT_LONG(Loop)
	VXP_IMPLEMENT_LONG(Delay)
	VXP_IMPLEMENT_LONG(Pause)
	VXP_IMPLEMENT_BOOL(KeepSettings)
	VXP_IMPLEMENT_HANDLE(HANDLE, Handle)

private:

	__vxpsound():m_pInterface(NULL){}
	IVXPSound * m_pInterface;
};

/////////////////////////////////////////////////
// class CVXPSound
// This class is to contain interface IVXPSound.
// It has all the same methods and properties
// as the IVXPSound interface to access them seamlessly
// just as though working directly with the COM interface.
/////////////////////////////////////////////////
class CVXPSound:public __vxpsound
{
public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	CVXPSound(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPSound * pSound;
			::CoCreateInstance(CLSID_VXPSound, NULL, CLSCTX_ALL, IID_IVXPSound, (void**)&pSound);
			Attach(pSound);
		}
	}

	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPSound()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPSound * pSound)
	{
		Detach();
		if(pSound)
			m_pInterface = pSound;
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		if(m_pInterface)
		{
			m_pInterface->Release();
			m_pInterface = NULL;
		}
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE(_bstr_t, FilePath)
	VXP_DECLARE(_bstr_t, Res)
	VXP_DECLARE(_bstr_t, Info)
	VXP_DECLARE(VXPPlayFrom, PlayFrom)
	VXP_DECLARE_READ(HANDLE, Handle)
	VXP_DECLARE(long, Size)
	VXP_DECLARE(long, Volume)
	VXP_DECLARE(long, Param)
	VXP_DECLARE(long, Balance)
	VXP_DECLARE(long, Frequency)
	VXP_DECLARE(long, StartPos)
	VXP_DECLARE(long, EndPos)
	VXP_DECLARE(long, Loop)
	VXP_DECLARE(long, Delay)
	VXP_DECLARE(long, Pause)
	VXP_DECLARE(bool, KeepSettings)

////////////
// Methods:
////////////

	void SetDefaults()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->SetDefaults();

		VXPLIB_END
	}

/////////////////////////
// Overloaded operators:
/////////////////////////

	CVXPSound & operator = (CVXPSound & sound)
	{
		FilePath = sound.FilePath;
		Res = sound.Res;
		Info = sound.Info;
		PlayFrom = sound.PlayFrom;
		Size = sound.Size;
		Volume = sound.Volume;
		Param = sound.Param;
		Balance = sound.Balance;
		Frequency = sound.Frequency;
		StartPos = sound.StartPos;
		EndPos = sound.EndPos;
		Loop = sound.Loop;
		Delay = sound.Delay;
		Pause = sound.Pause;
		KeepSettings = sound.KeepSettings;
		return *this;
	}
	bool operator == (CVXPSound & sound)
	{
		return (FilePath == sound.FilePath && Res == sound.Res && Info == sound.Info && PlayFrom == sound.PlayFrom && Size == sound.Size && Volume == sound.Volume && Param == sound.Param && Balance == sound.Balance && Frequency == sound.Frequency && StartPos == sound.StartPos && EndPos == sound.EndPos && Loop == sound.Loop && Delay == sound.Delay && Pause == sound.Pause && KeepSettings == sound.KeepSettings);
	}
	bool operator != (CVXPSound & sound)
	{
		return (FilePath != sound.FilePath || Res != sound.Res || Info != sound.Info || PlayFrom != sound.PlayFrom || Size != sound.Size || Volume != sound.Volume || Param != sound.Param || Balance != sound.Balance || Frequency != sound.Frequency || StartPos != sound.StartPos || EndPos != sound.EndPos || Loop != sound.Loop || Delay != sound.Delay || Pause != sound.Pause || KeepSettings != sound.KeepSettings);
	}
};

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxpmargins:public __vxperrorhandler
{
friend class CVXPMargins;

public:

	VXP_IMPLEMENT_SHORT(Left)
	VXP_IMPLEMENT_SHORT(Top)
	VXP_IMPLEMENT_SHORT(Right)
	VXP_IMPLEMENT_SHORT(Bottom)

private:

	__vxpmargins():m_pInterface(NULL){}
	IVXPMargins * m_pInterface;
};

/////////////////////////////////////////////////
// class CVXPMargins
// This class is to contain interface IVXPMargins.
// It has all the same methods and properties
// as the IVXPMargins interface to access them seamlessly
// just as though working directly with the COM interface.
/////////////////////////////////////////////////
class CVXPMargins:public __vxpmargins
{
public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	CVXPMargins(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPMargins * pMargins;
			::CoCreateInstance(CLSID_VXPMargins, NULL, CLSCTX_ALL, IID_IVXPMargins, (void**)&pMargins);
			Attach(pMargins);
		}
	}

	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPMargins()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPMargins * pMargins)
	{
		Detach();
		if(pMargins)
			m_pInterface = pMargins;
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		if(m_pInterface)
		{
			m_pInterface->Release();
			m_pInterface = NULL;
		}
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE(short, Left)
	VXP_DECLARE(short, Top)
	VXP_DECLARE(short, Right)
	VXP_DECLARE(short, Bottom)

////////////
// Methods:
////////////

	void AdjustMarginsByMin(short MinLeft, short MinTop, short MinRight, short MinBottom)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->AdjustMarginsByMin(MinLeft, MinTop, MinRight, MinBottom);

		VXPLIB_END
	}
	void AdjustMarginsByMax(short MaxLeft, short MaxTop, short MaxRight, short MaxBottom)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->AdjustMarginsByMax(MaxLeft, MaxTop, MaxRight, MaxBottom);

		VXPLIB_END
	}
	void SetDefaults()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->SetDefaults();

		VXPLIB_END
	}

/////////////////////////
// Overloaded operators:
/////////////////////////

	CVXPMargins & operator = (CVXPMargins & margins)
	{
		Left = margins.Left;
		Top = margins.Top;
		Right = margins.Right;
		Bottom = margins.Bottom;
		return *this;
	}
	bool operator == (CVXPMargins & margins)
	{
		return (Left == margins.Left && Top == margins.Top && Right == margins.Right && Bottom == margins.Bottom);
	}
	bool operator != (CVXPMargins & margins)
	{
		return (Left != margins.Left || Top != margins.Top || Right != margins.Right || Bottom != margins.Bottom);
	}
};

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxpsettings:public __vxperrorhandler
{
friend class CVXPSettings;

public:

	VXP_IMPLEMENT_REF(CVXPMargins, margins)
	VXP_IMPLEMENT_REF(CVXPSound, bgsound)
	VXP_IMPLEMENT_REF(CVXPFont, font)
	VXP_IMPLEMENT_SIMPLE(COLORREF, bgColor, RGB(0, 0, 0))
	VXP_IMPLEMENT_SIMPLE(COLORREF, gfColor, RGB(0, 0, 0))
	VXP_IMPLEMENT_BOOL(gfHorizontal)
	VXP_IMPLEMENT_BOOL(gfEnabled)
	VXP_IMPLEMENT_SHORT(DefIconWidth)
	VXP_IMPLEMENT_SHORT(DefIconHeight)
	VXP_IMPLEMENT_READ_SIMPLE(VXPOSVersion, osVersion, osUnknown)

private:

	__vxpsettings():m_pInterface(NULL), m_font(false), m_bgsound(false), m_margins(false){}

	IVXPSettings * m_pInterface;
	CVXPFont m_font;
	CVXPSound m_bgsound;
	CVXPMargins m_margins;
};

/////////////////////////////////////////////////
// class CVXPSettings
// This class is to contain interface IVXPSettings.
// It has all the same methods and properties
// as the IVXPSettings interface to access them seamlessly
// just as though working directly with the COM interface.
// It also contains objects CVXPFont, CVXPSound and CVXPMargins
// accessible through properties font, bgsound and margins
// respectively.
/////////////////////////////////////////////////
class CVXPSettings:public __vxpsettings
{
public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	CVXPSettings(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPSettings * pSettings;
			::CoCreateInstance(CLSID_VXPSettings, NULL, CLSCTX_ALL, IID_IVXPSettings, (void**)&pSettings);
			Attach(pSettings);
		}
	}

	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPSettings()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPSettings * pSettings)
	{
		Detach();
		if(pSettings)
		{
			m_pInterface = pSettings;

			IVXPFont * pFont;
			pSettings->get_font(&pFont);
			m_font.Attach(pFont);

			IVXPSound * pSound;
			pSettings->get_bgsound(&pSound);
			m_bgsound.Attach(pSound);

			IVXPMargins * pMargins;
			pSettings->get_margins(&pMargins);
			m_margins.Attach(pMargins);
		}
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		if(m_pInterface)
		{
			m_font.Detach();
			m_bgsound.Detach();
			m_margins.Detach();

			m_pInterface->Release();
			m_pInterface = NULL;
		}
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE(COLORREF, bgColor)
	VXP_DECLARE(COLORREF, gfColor)
	VXP_DECLARE(bool, gfEnabled)
	VXP_DECLARE(bool, gfHorizontal)
	VXP_DECLARE_READ(CVXPFont&, font)
	VXP_DECLARE_READ(CVXPSound&, bgsound)
	VXP_DECLARE_READ(CVXPMargins&, margins)
	VXP_DECLARE(short, DefIconWidth)
	VXP_DECLARE(short, DefIconHeight)
	VXP_DECLARE_READ(VXPOSVersion, osVersion)

////////////
// Methods:
////////////

	void SetDefaults()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->SetDefaults();

		VXPLIB_END
	}

/////////////////////////
// Overloaded operators:
/////////////////////////

	CVXPSettings & operator = (CVXPSettings & settings)
	{
		bgColor = settings.bgColor;
		m_bgsound = settings.m_bgsound;
		m_font = settings.m_font;
		m_margins = settings.m_margins;
		return *this;
	}
	bool operator == (CVXPSettings & settings)
	{
		return (bgColor == settings.bgColor && m_bgsound == settings.m_bgsound && m_font == settings.m_font && m_margins == settings.m_margins);
	}
	bool operator != (CVXPSettings & settings)
	{
		return (bgColor != settings.bgColor || m_bgsound != settings.m_bgsound || m_font != settings.m_font || m_margins != settings.m_margins);
	}
};

#ifdef VXPLIB_WITH_EVENTS

class CVXPQuickHTML;

///////////////////////////////////////////////
// This is a service interface class to handle
// events of the VXPLib, and should not be used
// directly by client applications.
///////////////////////////////////////////////
interface __vxpquickhtmlevents:public IDispEventImpl<0, __vxpquickhtmlevents, &DIID__IVXPQuickHTMLEvents, &LIBID_VXPLibrary, 1, 0>
{
friend class CVXPQuickHTML;
friend class __vxpquickhtml;

public:

BEGIN_SINK_MAP(__vxpquickhtmlevents)
	SINK_ENTRY_EX(0, DIID__IVXPQuickHTMLEvents, 0x01, OnDocumentGetSize)
	SINK_ENTRY_EX(0, DIID__IVXPQuickHTMLEvents, 0x02, OnDocumentDraw)
	SINK_ENTRY_EX(0, DIID__IVXPQuickHTMLEvents, 0x03, OnSetDefaults)
	SINK_ENTRY_EX(0, DIID__IVXPQuickHTMLEvents, 0x04, OnSourceChanged)
	SINK_ENTRY_EX(0, DIID__IVXPQuickHTMLEvents, 0x05, OnRecalculate)
	SINK_ENTRY_EX(0, DIID__IVXPQuickHTMLEvents, 0x06, OnGetText)
	SINK_ENTRY_EX(0, DIID__IVXPQuickHTMLEvents, 0x07, OnControlGetSize)
END_SINK_MAP()

private:

	__vxpquickhtmlevents()
	{
		m_pClient = NULL;
	}

	inline HRESULT __stdcall OnSetDefaults();
	inline HRESULT __stdcall OnDocumentGetSize(BSTR DocumentPath, BSTR Param, short * Width, short * Height);
	inline HRESULT __stdcall OnDocumentDraw(BSTR DocumentPath, BSTR Param, long DC, short x, short y, short Width, short Height);
	inline HRESULT __stdcall OnSourceChanged(VARIANT_BOOL IsEmpty);
	inline HRESULT __stdcall OnRecalculate(VARIANT_BOOL Begin, VARIANT_BOOL SourceChanged);
	inline HRESULT __stdcall OnGetText(BSTR FilePath, BSTR Res, long Size, long ID, BSTR * Result);
	inline HRESULT __stdcall OnControlGetSize(long ID, BSTR Name, short * Width, short * Height, BSTR Param);

	CVXPQuickHTML * m_pClient;
};

static _ATL_FUNC_INFO OnStateChangedInfo = {CC_STDCALL, VT_EMPTY, 1, {VT_I4}};

class CVXPTooltip;

///////////////////////////////////////////////
// This is a service interface class to handle
// events of the VXPLib, and should not be used
// directly by client applications.
///////////////////////////////////////////////
interface __vxptooltipevents:public IDispEventImpl<0, __vxptooltipevents, &DIID__IVXPTooltipEvents, &LIBID_VXPLibrary, 1, 0>
{
friend class CVXPTooltip;
friend class __vxptooltip;

public:

BEGIN_SINK_MAP(__vxptooltipevents)
	SINK_ENTRY_INFO(0, DIID__IVXPTooltipEvents, 0x01, OnStateChanged, &OnStateChangedInfo)
	SINK_ENTRY_EX(0, DIID__IVXPTooltipEvents, 0x02, OnSoundPlay)
	SINK_ENTRY_EX(0, DIID__IVXPTooltipEvents, 0x03, OnSoundAdjust)
	SINK_ENTRY_EX(0, DIID__IVXPTooltipEvents, 0x04, OnSoundStop)
	SINK_ENTRY_EX(0, DIID__IVXPTooltipEvents, 0x05, OnWndMsg)
	SINK_ENTRY_EX(0, DIID__IVXPTooltipEvents, 0x06, OnMouseClick)
END_SINK_MAP()

private:

	__vxptooltipevents()
	{
		m_pClient = NULL;
	}

	inline HRESULT __stdcall OnStateChanged(VXPTooltipState NewState);
	inline HRESULT __stdcall OnSoundPlay(interface IVXPSound * bgsound, VARIANT_BOOL * StartedPlaying );
	inline HRESULT __stdcall OnSoundAdjust(interface IVXPSound * bgsound, float FadingPercent);
	inline HRESULT __stdcall OnSoundStop(interface IVXPSound * bgsound);
	inline HRESULT __stdcall OnWndMsg(long uMsg, long wParam, long lParam, VARIANT_BOOL * bCallDefWndProc, long * result);
	inline HRESULT __stdcall OnMouseClick(long X, long Y, long uMsg, long hWnd, long wHitTestCode, VARIANT_BOOL * bProcessed);

	CVXPTooltip * m_pClient;
};

class CVXPTooltipManager;

///////////////////////////////////////////////
// This is a service interface class to handle
// events of the VXPLib, and should not be used
// directly by client applications.
///////////////////////////////////////////////
interface __vxptooltipmanagerevents:public IDispEventImpl<0, __vxptooltipmanagerevents, &DIID__IVXPTooltipManagerEvents, &LIBID_VXPLibrary, 1, 0>
{
friend class CVXPTooltipManager;
friend class __vxptooltipmanager;

public:

BEGIN_SINK_MAP(__vxptooltipmanagerevents)
	SINK_ENTRY_EX(0, DIID__IVXPTooltipManagerEvents, 0x01, OnActivateCustomTooltip)
END_SINK_MAP()

private:

	__vxptooltipmanagerevents()
	{
		m_pClient = NULL;
	}

	inline HRESULT __stdcall OnActivateCustomTooltip(long hWnd, BSTR ClassName, short x, short y, BSTR * Text);

	CVXPTooltipManager * m_pClient;
};

static _ATL_FUNC_INFO OnDrawItemInfo = {CC_STDCALL, VT_EMPTY, 8, {VT_I4, VT_I4, VT_I4, VT_I4, VT_I2, VT_I2, VT_I2, VT_I2}};

///////////////////////////////////////////////
// This is a service interface class to handle
// events of the VXPLib, and should not be used
// directly by client applications.
///////////////////////////////////////////////
interface __vxpmenuhookevents:public IDispEventImpl<0, __vxpmenuhookevents, &DIID__IVXPMenuHookEvents, &LIBID_VXPLibrary, 1, 0>
{
friend class CVXPMenuHook;
friend class __vxpmenuhook;

public:

BEGIN_SINK_MAP(__vxpmenuhookevents)
	SINK_ENTRY_EX(0, DIID__IVXPMenuHookEvents, 0x01, OnMenuInit)
	SINK_ENTRY_EX(0, DIID__IVXPMenuHookEvents, 0x02, OnMenuDone)
	SINK_ENTRY_EX(0, DIID__IVXPMenuHookEvents, 0x03, OnMeasureItem)
	SINK_ENTRY_INFO(0, DIID__IVXPMenuHookEvents, 0x04, OnDrawItem, &OnDrawItemInfo)
	SINK_ENTRY_EX(0, DIID__IVXPMenuHookEvents, 0x05, OnMenuSelect)
	SINK_ENTRY_EX(0, DIID__IVXPMenuHookEvents, 0x06, OnSettingsChanged)
	SINK_ENTRY_EX(0, DIID__IVXPMenuHookEvents, 0x07, OnContextMenu)
	SINK_ENTRY_EX(0, DIID__IVXPMenuHookEvents, 0x08, OnDrawMenuBarExtender)
END_SINK_MAP()

private:

	__vxpmenuhookevents()
	{
		m_pClient = NULL;
	}

	inline HRESULT __stdcall OnMenuInit(long hMenu, short ItemPos, VARIANT_BOOL IsWindowMenu);
	inline HRESULT __stdcall OnMenuDone(long hMenu);
	inline HRESULT __stdcall OnMeasureItem(long ItemID, short * ItemWidth, short * ItemHeight);
	inline HRESULT __stdcall OnDrawItem(long hMenu, long hDC, long ItemID, enum VXPMenuItemFlags Flags, short X, short Y, short W, short H);
	inline HRESULT __stdcall OnMenuSelect(long hMenu, VARIANT_BOOL bOwnerDrawn, VARIANT_BOOL bSelected, VARIANT_BOOL bPopUp, long ItemID);
	inline HRESULT __stdcall OnSettingsChanged();
	inline HRESULT __stdcall OnContextMenu(long hWnd, short X, short Y);
	inline HRESULT __stdcall OnDrawMenuBarExtender(long hDC, short X, short Y, short W, short H);

	CVXPMenuHook * m_pClient;
};

#endif

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxpwndtoollist:public __vxperrorhandler
{
friend class CVXPWndToolList;

public:

	VXP_IMPLEMENT_READ_SIMPLE(short, Size, 0)

private:

	__vxpwndtoollist():m_pInterface(NULL){}
	IVXPWndToolList * m_pInterface;
};

/////////////////////////////////////////////////
// class CVXPWndToolList
// This class is to contain interface IVXPWndToolList.
// It has all the same methods and properties
// as the IVXPWndToolList interface to access them seamlessly
// just as though working directly with the COM interface.
// It is used by class CVXPTooltipManager.
/////////////////////////////////////////////////
class CVXPWndToolList:public __vxpwndtoollist
{
public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	CVXPWndToolList(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPWndToolList * pWndToolList;
			::CoCreateInstance(CLSID_VXPWndToolList, NULL, CLSCTX_ALL, IID_IVXPWndToolList, (void**)&pWndToolList);
			Attach(pWndToolList);
		}
	}
	
	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPWndToolList()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPWndToolList * pWndToolList)
	{
		Detach();
		if(pWndToolList)
			m_pInterface = pWndToolList;
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		if(m_pInterface)
		{
			m_pInterface->Release();
			m_pInterface = NULL;
		}
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE_READ(short, Size)

////////////
// Methods:
////////////

	void Add(HWND hWnd, LPCTSTR ToolTip)
	{
		VXPLIB_BEGIN

		if(m_pInterface && ToolTip)
		{
			_bstr_t tooltip(ToolTip);
			m_pInterface->Add((long)hWnd, tooltip);
		}

		VXPLIB_END
	}
	void Add(HWND hWnd, BSTR ToolTip)
	{
		VXPLIB_BEGIN

		if(m_pInterface && ToolTip)
			m_pInterface->Add((long)hWnd, ToolTip);

		VXPLIB_END
	}
	bool Modify(HWND hWnd, LPCTSTR ToolTip)
	{
		VXPLIB_BEGIN

		if(m_pInterface && ToolTip)
		{
			_bstr_t tooltip(ToolTip);
			return m_pInterface->Modify((long)hWnd, tooltip)?true:false;
		}
		
		VXPLIB_END

		return false;
	}
	bool Modify(HWND hWnd, BSTR ToolTip)
	{
		VXPLIB_BEGIN

		if(m_pInterface && ToolTip)
			return m_pInterface->Modify((long)hWnd, ToolTip)?true:false;
		
		VXPLIB_END

		return false;
	}
	bool Remove(HWND hWnd)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->Remove((long)hWnd)?true:false;

		VXPLIB_END

		return false;
	}
	void Clear()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->Clear();

		VXPLIB_END
	}
	bool HasTool(HWND hWnd)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->HasTool((long)hWnd)?true:false;

		VXPLIB_END

		return false;
	}
	bool IsActive(HWND hWnd)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->IsActive((long)hWnd)?true:false;

		VXPLIB_END

		return false;
	}
	bool SetActive(HWND hWnd, bool Active)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->SetActive((long)hWnd, Active?TRUE:FALSE)?true:false;

		VXPLIB_END

		return false;
	}
};

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxpcmdtoollist:public __vxperrorhandler
{
friend class CVXPCmdToolList;

public:

		VXP_IMPLEMENT_READ_SIMPLE(short, Size, 0)
private:

	__vxpcmdtoollist():m_pInterface(NULL){}
	IVXPCmdToolList * m_pInterface;
};

/////////////////////////////////////////////////
// class CVXPCmdToolList
// This class is to contain interface IVXPCmdToolList.
// It has all the same methods and properties
// as the IVXPCmdToolList interface to access them seamlessly
// just as though working directly with the COM interface.
// It is used by class CVXPTooltipManager.
/////////////////////////////////////////////////
class CVXPCmdToolList:public __vxpcmdtoollist
{
public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	CVXPCmdToolList(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPCmdToolList * pCmdToolList;
			::CoCreateInstance(CLSID_VXPCmdToolList, NULL, CLSCTX_ALL, IID_IVXPCmdToolList, (void**)&pCmdToolList);
			Attach(pCmdToolList);
		}
	}

	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPCmdToolList()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPCmdToolList * pCmdToolList)
	{
		Detach();
		if(pCmdToolList)
			m_pInterface = pCmdToolList;
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		if(m_pInterface)
		{
			m_pInterface->Release();
			m_pInterface = NULL;
		}
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE_READ(short, Size)

////////////
// Methods:
////////////

	void Add(long CommandID, LPCTSTR ToolTip)
	{
		VXPLIB_BEGIN

		if(m_pInterface && ToolTip)
		{
			_bstr_t tooltip(ToolTip);
			m_pInterface->Add(CommandID, tooltip);
		}
		
		VXPLIB_END
	}
	void Add(long CommandID, _bstr_t ToolTip)
	{
		VXPLIB_BEGIN

		if(m_pInterface && ToolTip.length() > 0 )
			m_pInterface->Add(CommandID, ToolTip);
		
		VXPLIB_END
	}
	bool Modify(long CommandID, LPCTSTR ToolTip)
	{
		VXPLIB_BEGIN

		if(m_pInterface && ToolTip)
		{
			_bstr_t tooltip(ToolTip);
			return m_pInterface->Modify(CommandID, tooltip)?true:false;
		}

		VXPLIB_END

		return false;
	}
	bool Modify(long CommandID, BSTR ToolTip)
	{
		VXPLIB_BEGIN

		if(m_pInterface && ToolTip)
			return m_pInterface->Modify(CommandID, ToolTip)?true:false;

		VXPLIB_END

		return false;
	}
	bool Remove(long CommandID)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->Remove(CommandID)?true:false;

		VXPLIB_END

		return false;
	}
	void Clear()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->Clear();

		VXPLIB_END
	}
	bool HasTool(long CommandID)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->HasTool(CommandID)?true:false;

		VXPLIB_END

		return false;
	}
	bool IsActive(long CommandID)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->IsActive(CommandID)?true:false;

		VXPLIB_END

		return false;
	}
	bool SetActive(long CommandID, bool Active)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->SetActive(CommandID, Active?TRUE:FALSE)?true:false;

		VXPLIB_END

		return false;
	}
};

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxpquickhtml:public __vxperrorhandler
{
friend class CVXPQuickHTML;

public:

	VXP_IMPLEMENT_BOOL(Transparent)
	VXP_IMPLEMENT_READ_BOOL(HasBackgroundPicture)
	VXP_IMPLEMENT_READ(short, Width)
	VXP_IMPLEMENT_READ(short, Height)
	VXP_IMPLEMENT_REF(CVXPSettings, settings)
	VXP_IMPLEMENT_LONG(BodyCodePage)
	VXP_IMPLEMENT_LONG(TxtCodePage)

private:

	__vxpquickhtml():m_pInterface(NULL), m_settings(false){};
	CVXPSettings m_settings;
	IVXPQuickHTML * m_pInterface;

#ifdef VXPLIB_WITH_EVENTS
	__vxpquickhtmlevents m_sink;
#endif

};

/////////////////////////////////////////////////
// class CVXPQuickHTML - Quick HTML parser;
// This class is to contain interface IVXPQuickHTML.
// It has all the same methods and properties
// as the IVXPQuickHTML interface to access them seamlessly
// just as though working directly with the COM interface.
//
// Additionally, this class handles exceptions that may occur
// as a result of calling a method of the interface.
/////////////////////////////////////////////////
class CVXPQuickHTML:public __vxpquickhtml
{
#ifdef VXPLIB_WITH_EVENTS
friend interface __vxpquickhtmlevents;
#endif
public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	explicit CVXPQuickHTML(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPQuickHTML * pHTML;
			::CoCreateInstance(CLSID_VXPQuickHTML, NULL, CLSCTX_ALL, IID_IVXPQuickHTML, (void**)&pHTML);
			Attach(pHTML);
		}
	};

	// Initialization constructors:
	explicit CVXPQuickHTML(LPCTSTR Text)
	{
		IVXPQuickHTML * pHTML;
		::CoCreateInstance(CLSID_VXPQuickHTML, NULL, CLSCTX_ALL, IID_IVXPQuickHTML, (void**)&pHTML);
		Attach(pHTML);
		SetSourceText(Text);
	};

	explicit CVXPQuickHTML(BSTR Text)
	{
		IVXPQuickHTML * pHTML;
		::CoCreateInstance(CLSID_VXPQuickHTML, NULL, CLSCTX_ALL, IID_IVXPQuickHTML, (void**)&pHTML);
		Attach(pHTML);
		SetSourceText(Text);
	};

	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPQuickHTML()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPQuickHTML * pObject)
	{
		Detach();
		if(pObject)
		{
			m_pInterface = pObject;
#ifdef VXPLIB_WITH_EVENTS
			m_sink.m_pClient = this;
			m_sink.DispEventAdvise(m_pInterface);
#endif
			IVXPSettings * pSettings;
			pObject->get_settings(&pSettings);
			m_settings.Attach(pSettings);
		}
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		if(m_pInterface)
		{
			m_settings.Detach();
#ifdef VXPLIB_WITH_EVENTS
			m_sink.DispEventUnadvise(m_pInterface);
			m_sink.m_pClient = NULL;
#endif
			m_pInterface->Release();
			m_pInterface = NULL;
		}
	}

	// This method will return true, if there was an instance
	// of IVXPQuickHTML attached to this class object.
	// This way it is also possible to verify whether VXPLib
	// is available on the computer.
	bool IsAttached()
	{
		return (m_pInterface != NULL);
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE(bool, Transparent)
	VXP_DECLARE_READ(bool, HasBackgroundPicture)
	VXP_DECLARE(short, Width)
	VXP_DECLARE(short, Height)
	VXP_DECLARE(CVXPSettings&, settings)
	VXP_DECLARE(long, BodyCodePage)
	VXP_DECLARE(long, TxtCodePage)

////////////
// Methods:
////////////

	bool RecalculateLayout()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->RecalculateLayout()?true:false;

		VXPLIB_END
		
		return false;
	}
	void SetSourceText(LPCTSTR Text)
	{
		VXPLIB_BEGIN

		if(m_pInterface && Text)
		{
			_bstr_t t(Text);
			m_pInterface->SetSourceText(t);
		}

		VXPLIB_END
	}
	void SetSourceText(BSTR Text)
	{
		VXPLIB_BEGIN

		if(m_pInterface && Text)
			m_pInterface->SetSourceText(Text);

		VXPLIB_END
	}
	bool Draw(HDC dc, short x, short y)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->Draw((long)dc, x, y)?true:false;

		VXPLIB_END

		return false;
	}
	bool DrawState(HDC dc, short x, short y, long dssFlags, COLORREF Color = 0)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->DrawState((long)dc, x, y, dssFlags, Color)?true:false;

		VXPLIB_END

		return false;
	}
	bool DrawStretched(HDC dc, short x, short y, short width, short height)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->DrawStretched((long)dc, x, y, width, height)?true:false;

		VXPLIB_END
		
		return false;
	}
	bool DrawBackground(HDC dc, short x, short y, short width, short height, bool DrawBackColor = true)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->DrawBackground((long)dc, x, y, width, height, DrawBackColor)?true:false;

		VXPLIB_END

		return false;
	}
	void AboutBox()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->AboutBox();

		VXPLIB_END
	}
	bool SetDefaultRes(LPCTSTR FilePath)
	{
		VXPLIB_BEGIN

		if(m_pInterface && FilePath)
		{
			_bstr_t fpath(FilePath);
			return m_pInterface->SetDefaultRes(fpath)?true:false;
		}

		VXPLIB_END

		return false;
	}
	short AddFilePath(LPCTSTR FilePath)
	{
		VXPLIB_BEGIN

		if(m_pInterface && FilePath)
		{
			_bstr_t fpath(FilePath);
			return m_pInterface->AddFilePath(fpath);
		}

		VXPLIB_END

		return 0;
	}
	_bstr_t GetSpeechText(VXPSpeechFormat format = sfNone, bool bIncludeComments = true, bool bIncludeText = true)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->GetSpeechText(format, bIncludeComments, bIncludeText);

		VXPLIB_END

		return _bstr_t();
	}
	bool GradientFillRect(HDC hDC, COLORREF StartColor, COLORREF EndColor, short x1, short y1, short x2, short y2, bool bHorizontal = true)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->GradientFillRect((long)hDC, StartColor, EndColor, x1, y1, x2, y2, bHorizontal)?true:false;

		VXPLIB_END

		return false;
	}
	bool GetControlPos(long ID, short & X, short & Y, short & Width, short & Height)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->GetControlPos(ID, &X, &Y, &Width, &Height)?true:false;

		VXPLIB_END
		
		return false;
	}
	long GetControlsInfo(long & MinID, long & MaxID)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->GetControlsInfo(&MinID, &MaxID);

		VXPLIB_END
		
		return 0;
	}
	long GetControlsInfo()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->GetControlsInfo(NULL, NULL);

		VXPLIB_END
		
		return 0;
	}


	// The following three functions allow to retrieve values of attributes
	// for tags <BODY> and <CONTROL>.
	// Field vt of the returned type indicates the type that was returned:
	// *********************************************************
	// taText:		vt = VT_BSTR,	result in field bstrVal
	// taColor:		vt = VT_I4,		result in field lVal
	// taByte:		vt = VT_UI1,	result in field bVal
	// taShort:		vt = VT_I2,		result in field iVal
	// taLong:		vt = VT_I4,		result in field lVal
	// taFloat:		vt = VT_R4,		result in field fltVal
	// taDouble:	vt = VT_R8,		result in field dblVal
	// taPercent:	vt = VT_UI1,	result in field bVal
	// taDateTime:	vt = VT_DATE,	result in field date
	// taBoolean:	vt = VT_BOOL,	result in field boolVal
	// *********************************************************
	// If in the returned value field vt equals VT_EMPTY then:
	//  - For tag BODY - attribute wasn't found;
	//  - For tag CONTROL - whether control with specified ID wasn't
	//    found, or it didn't have the specified attribute, or in case
	//    of using Index it went out of allowable range.
	VARIANT GetBodyAttr(LPCTSTR attrName, VXPTagAttribute attrType = taText)
	{
		VARIANT result;
		memset(&result, 0, sizeof(VARIANT));

		VXPLIB_BEGIN

		if(m_pInterface)
		{
			_bstr_t t(attrName);
			result = m_pInterface->GetBodyAttr(t, attrType);
		}

		VXPLIB_END

		return result;
	}
	// For this function read documentation above
	VARIANT GetControlAttr(long ID, LPCTSTR attrName, VXPTagAttribute attrType = taText)
	{
		VARIANT result;
		memset(&result, 0, sizeof(VARIANT));

		VXPLIB_BEGIN

		if(m_pInterface)
		{
			_bstr_t t(attrName);
			result = m_pInterface->GetControlAttr(ID, t, attrType);
		}

		VXPLIB_END

		return result;
	}
	// The same as GetControlAttr, except takes index of the tag
	// instead of ID. If the index is out of range this function
	// throws exception ER_INDEXOUTOFRANGE
	VARIANT GetControlAttrIdx(long Index, LPCTSTR attrName, VXPTagAttribute attrType = taText)
	{
		VARIANT result;
		memset(&result, 0, sizeof(VARIANT));

		VXPLIB_BEGIN

		if(m_pInterface)
		{
			_bstr_t t(attrName);
			result = m_pInterface->GetControlAttrIdx(Index, t, attrType);
		}

		VXPLIB_END

		return result;
	}

	// Function TextToHTML converts a string of plain text into
	// correct HTML text by changing some ASCII symbols not allowed
	// in HTML syntax. For details read VXPLib Documentation.
	_bstr_t TextToHTML(LPCTSTR strSource, long nMaxWidth = 0)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
		{
			_bstr_t t(strSource);
			return m_pInterface->TextToHTML(t, nMaxWidth);
		}

		VXPLIB_END

		return _bstr_t();
	}
	_bstr_t TextToHTML(BSTR strSource, long nMaxWidth = 0)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->TextToHTML(strSource, nMaxWidth);

		VXPLIB_END

		return _bstr_t();
	}

	// Performs advanced conversion of plain text into a valid HTML-formatted document.
	// Read VXPLib documentation on this function for details.
	_bstr_t TextToHTMLEx(LPCTSTR strSource, long nMaxWidth, long * nRows = NULL, bool * bTruncated = NULL, VXPTextToHTML tthOptions = tthDefault)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
		{
			_bstr_t t(strSource);
			VARIANT_BOOL bTrunc;
			long nRowsSafe = nRows?*nRows:0;
			_bstr_t strDest = m_pInterface->TextToHTMLEx(t, nMaxWidth, &nRowsSafe, &bTrunc, tthOptions);
			if(bTruncated)
				*bTruncated = bTrunc?true:false;
			if(nRows)
				*nRows = nRowsSafe;
			return strDest;
		}

		VXPLIB_END

		return _bstr_t();
	}
	_bstr_t TextToHTMLEx(BSTR strSource, long nMaxWidth, long * nRows = NULL, bool * bTruncated = NULL, VXPTextToHTML tthOptions = tthDefault)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
		{
			VARIANT_BOOL bTrunc;
			long nRowsSafe = nRows?*nRows:0;
			_bstr_t strDest = m_pInterface->TextToHTMLEx(strSource, nMaxWidth, &nRowsSafe, &bTrunc, tthOptions);
			if(bTruncated)
				*bTruncated = bTrunc?true:false;
			if(nRows)
				*nRows = nRowsSafe;
			return strDest;
		}

		VXPLIB_END

		return _bstr_t();
	}

	// Returns the number of purchased licenses for the VXPLib component,
	// if the component has been licensed, or 0 otherwise.
	// For terms of licensing and distribution of the component please
	// refer to the VXPLib Documentation, or visit www.Tootlips.NET
	long GetLicenses()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->GetLicenses();

		VXPLIB_END

		return 0;
	}

/////////////////////////
// Overloaded operators:
/////////////////////////

	CVXPQuickHTML & operator = (LPCTSTR Text)
	{
		SetSourceText(Text);
		return *this;
	}

	CVXPQuickHTML & operator = (BSTR Text)
	{
		SetSourceText(Text);
		return *this;
	}

protected:

	// Events:
	virtual void OnSetDefaults(){};
	virtual void OnDocumentGetSize(LPCTSTR DocumentPath, LPCTSTR Param, short & Width, short & Height){};
	virtual void OnDocumentDraw(LPCTSTR DocumentPath, LPCTSTR Param, HDC DC, short x, short y, short Width, short Height){};
	virtual void OnSourceChanged(bool IsEmpty){};
	virtual void OnRecalculate(bool Begin, bool SourceChanged){};
	virtual BSTR OnGetText(LPCTSTR FilePath, LPCTSTR Res, long Size, long ID){return NULL;};	
	virtual void OnControlGetSize(long ID, LPCTSTR Name, short & Width, short & Height, LPCTSTR Param){};
};

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxptooltip:public CVXPQuickHTML
{
	friend class CVXPTooltip;

public:

	VXP_IMPLEMENT_READ_SIMPLE(VXPTooltipState, State, ttsHidden)
	VXP_IMPLEMENT_SIMPLE(VXPTooltipTheme, Theme, ttDefault)
	VXP_IMPLEMENT_SIMPLE(BYTE, Transparency, 0)
	VXP_IMPLEMENT_SIMPLE(BYTE, Round, 0)
	VXP_IMPLEMENT_HANDLE(HWND, hWnd)
	VXP_IMPLEMENT_SHORT(FadeShow)
	VXP_IMPLEMENT_SHORT(FadeHide)
	VXP_IMPLEMENT_SHORT(FadeSound)
	VXP_IMPLEMENT_BOOL(HasTail)
	VXP_IMPLEMENT_BOOL(HasBorder)
	VXP_IMPLEMENT_BOOL(HasShadow)
	VXP_IMPLEMENT_BOOL(TraceMode)
	VXP_IMPLEMENT_BOOL(ShowWhenEmpty)
	VXP_IMPLEMENT_BOOL(Autohide)
	VXP_IMPLEMENT_SIMPLE(COLORREF, BorderColor, RGB(0, 0, 0))
	VXP_IMPLEMENT_SIMPLE(VXPMouseClickResponse, MouseClickResponse, mcrNoResponse)
	VXP_IMPLEMENT_HANDLE_EX(HWND, SyncWnd)

	CVXPQuickHTML & __Gethtml(){return *this;}

private:

	__vxptooltip():CVXPQuickHTML(false), m_pInterface(NULL){}
	IVXPTooltip * m_pInterface;

#ifdef VXPLIB_WITH_EVENTS
	__vxptooltipevents m_sink;
#endif
};

/////////////////////////////////////////////////////////////
// class CVXPTooltip
// This class is to contain interface IVXPTooltip.
// It has all the same methods and properties
// as the IVXPTooltip interface to access them seamlessly
// just as though working directly with the COM interface.
/////////////////////////////////////////////////////////////
class CVXPTooltip:public __vxptooltip
{
#ifdef VXPLIB_WITH_EVENTS
friend interface __vxptooltipevents;
#endif
public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	CVXPTooltip(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPTooltip * pTooltip;
			::CoCreateInstance(CLSID_VXPTooltip, NULL, CLSCTX_ALL, IID_IVXPTooltip, (void**)&pTooltip);
			Attach(pTooltip);
		}
	};

	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPTooltip()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPTooltip * pObject)
	{
		Detach();
		if(pObject)
		{
			__vxptooltip::m_pInterface = pObject;

#ifdef VXPLIB_WITH_EVENTS
			__vxptooltip::m_sink.m_pClient = this;
			__vxptooltip::m_sink.DispEventAdvise(__vxptooltip::m_pInterface);
#endif
			IVXPQuickHTML * pHTML;
			__vxptooltip::m_pInterface->get_html(&pHTML);
			CVXPQuickHTML::Attach(pHTML);
		}
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		CVXPQuickHTML::Detach();
		if(__vxptooltip::m_pInterface)
		{
#ifdef VXPLIB_WITH_EVENTS
			__vxptooltip::m_sink.DispEventUnadvise(__vxptooltip::m_pInterface);
			__vxptooltip::m_sink.m_pClient = NULL;
#endif
			__vxptooltip::m_pInterface->Release();
			__vxptooltip::m_pInterface = NULL;
		}
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE_READ(CVXPQuickHTML&, html)
	VXP_DECLARE_READ(VXPTooltipState, State)
	VXP_DECLARE(VXPTooltipTheme, Theme)
	VXP_DECLARE(bool, Autohide)
	VXP_DECLARE(short, FadeShow)
	VXP_DECLARE(short, FadeHide)
	VXP_DECLARE(short, FadeSound)
	VXP_DECLARE(BYTE, Transparency)
	VXP_DECLARE(BYTE, Round)
	VXP_DECLARE(bool, HasTail)
	VXP_DECLARE(bool, HasBorder)
	VXP_DECLARE(bool, HasShadow)
	VXP_DECLARE(bool, TraceMode)
	VXP_DECLARE(bool, ShowWhenEmpty)
	VXP_DECLARE_READ(HWND, hWnd)
	VXP_DECLARE(COLORREF, BorderColor)
	VXP_DECLARE(VXPMouseClickResponse, MouseClickResponse)
	VXP_DECLARE(HWND, SyncWnd)

////////////
// Methods:
////////////

	bool Show()
	{
		VXPLIB_BEGIN

		if(__vxptooltip::m_pInterface)
			return __vxptooltip::m_pInterface->Show()?true:false;

		VXPLIB_END

		return false;
	}
	bool Create()
	{
		VXPLIB_BEGIN

		if(__vxptooltip::m_pInterface)
			return __vxptooltip::m_pInterface->Create()?true:false;

		VXPLIB_END

		return false;
	}
	bool Destroy()
	{
		VXPLIB_BEGIN

		if(__vxptooltip::m_pInterface)
			return __vxptooltip::m_pInterface->Destroy()?true:false;

		VXPLIB_END

		return false;
	}
	bool Subscribe(UINT uMsg)
	{
		VXPLIB_BEGIN

		if(__vxptooltip::m_pInterface)
			return __vxptooltip::m_pInterface->Subscribe(uMsg)?true:false;

		VXPLIB_END
		
		return false;
	}
	bool UnSubscribe(UINT uMsg)
	{
		VXPLIB_BEGIN

		if(__vxptooltip::m_pInterface)
			return __vxptooltip::m_pInterface->UnSubscribe(uMsg)?true:false;

		VXPLIB_END

		return false;
	}
	LRESULT DefWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		VXPLIB_BEGIN

		if(__vxptooltip::m_pInterface)
			return __vxptooltip::m_pInterface->DefWndProc(uMsg, wParam, lParam);

		VXPLIB_END

		return 0;
	}
	bool ShowAt(short x, short y)
	{
		VXPLIB_BEGIN

		if(__vxptooltip::m_pInterface)
			return __vxptooltip::m_pInterface->ShowAt(x, y)?true:false;

		VXPLIB_END

		return false;
	}
	bool Hide(bool NoFadingEffect = false)
	{
		VXPLIB_BEGIN

		if(__vxptooltip::m_pInterface)
			return __vxptooltip::m_pInterface->Hide(NoFadingEffect)?true:false;

		VXPLIB_END

		return false;
	}

protected:

	// Events:
	virtual void OnStateChanged(VXPTooltipState NewState){};
	virtual bool OnSoundPlay(CVXPSound & bgsound){return false;};
	virtual void OnSoundAdjust(CVXPSound & bgsound, float FadingPercent){};
	virtual void OnSoundStop(CVXPSound & bgsound){};
	virtual LRESULT OnWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bCallDefWndProc){return 0;};
	virtual bool OnMouseClick(long X, long Y, UINT uMsg, HWND hWnd, UINT wHitTestCode){return false;};
};

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxptooltipmanager:public CVXPTooltip
{
friend class CVXPTooltipManager;
public:

	VXP_IMPLEMENT_REF(CVXPCmdToolList, commands)
	VXP_IMPLEMENT_REF(CVXPWndToolList, windows)
	VXP_IMPLEMENT_SHORT(ShowDelay)
	VXP_IMPLEMENT_SHORT(HideDelay)
	VXP_IMPLEMENT_BOOL(Active)
	VXP_IMPLEMENT_BOOL(DebugMode)
	VXP_IMPLEMENT_BOOL(sfdMenuItems)
	VXP_IMPLEMENT_BOOL(sfdToolbarButtons)
	VXP_IMPLEMENT_BOOL(UseMenuItemID)
	VXP_IMPLEMENT_BOOL(IgnoreMenus)
	VXP_IMPLEMENT_HANDLE_EX(HWND, Activator)
	CVXPTooltip & __Gettool(){return *this;}

private:

	__vxptooltipmanager():CVXPTooltip(false), m_pInterface(NULL), m_windows(false), m_commands(false){}

	IVXPTooltipManager * m_pInterface;
	CVXPWndToolList m_windows;
	CVXPCmdToolList m_commands;

#ifdef VXPLIB_WITH_EVENTS
	__vxptooltipmanagerevents m_sink;
#endif

};

/////////////////////////////////////////////////////////////
// class CVXPTooltipManager
// This class is to contain interface IVXPTooltipManager.
// It has all the same methods and properties
// as the IVXPTooltipManager interface to access them seamlessly
// just as though working directly with the COM interface.
/////////////////////////////////////////////////////////////
class CVXPTooltipManager:public __vxptooltipmanager
{
#ifdef VXPLIB_WITH_EVENTS
friend interface __vxptooltipmanagerevents;
#endif

public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	CVXPTooltipManager(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPTooltipManager * pManager;
			::CoCreateInstance(CLSID_VXPTooltipManager, NULL, CLSCTX_ALL, IID_IVXPTooltipManager, (void**)&pManager);
			Attach(pManager);
		}
	};

	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPTooltipManager()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPTooltipManager * pObject)
	{
		Detach();
		if(pObject)
		{
			__vxptooltipmanager::m_pInterface = pObject;
#ifdef VXPLIB_WITH_EVENTS
			__vxptooltipmanager::m_sink.m_pClient = this;
			__vxptooltipmanager::m_sink.DispEventAdvise(__vxptooltipmanager::m_pInterface);
#endif
			IVXPTooltip * pTooltip;
			pObject->get_tool(&pTooltip);
			CVXPTooltip::Attach(pTooltip);

			IVXPWndToolList * pWndToolList;
			pObject->get_windows(&pWndToolList);
			m_windows.Attach(pWndToolList);

			IVXPCmdToolList * pCmdToolList;
			pObject->get_commands(&pCmdToolList);
			m_commands.Attach(pCmdToolList);
		}
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		CVXPTooltip::Detach();
		if(__vxptooltipmanager::m_pInterface)
		{
			m_commands.Detach();
			m_windows.Detach();
#ifdef VXPLIB_WITH_EVENTS
			__vxptooltipmanager::m_sink.DispEventUnadvise(__vxptooltipmanager::m_pInterface);
			__vxptooltipmanager::m_sink.m_pClient = NULL;
#endif
			__vxptooltipmanager::m_pInterface->Release();
			__vxptooltipmanager::m_pInterface = NULL;
		}
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE_READ(CVXPTooltip&, tool)
	VXP_DECLARE_READ(CVXPWndToolList&, windows)
	VXP_DECLARE_READ(CVXPCmdToolList&, commands)
	VXP_DECLARE(short, ShowDelay)
	VXP_DECLARE(short, HideDelay)
	VXP_DECLARE(bool, Active)
	VXP_DECLARE(bool, DebugMode)
	VXP_DECLARE(bool, sfdMenuItems)
	VXP_DECLARE(bool, sfdToolbarButtons)
	VXP_DECLARE(bool, UseMenuItemID)
	VXP_DECLARE(bool, IgnoreMenus)
	VXP_DECLARE(HWND, Activator)

////////////
// Methods:
////////////

	bool UpdateContextMenuOwner(HWND hWnd)
	{
		VXPLIB_BEGIN

		if(__vxptooltipmanager::m_pInterface)
			return __vxptooltipmanager::m_pInterface->UpdateContextMenuOwner((long)hWnd)?true:false;

		VXPLIB_END

		return false;
	}
	void AddMenuClass(LPCTSTR ClassName)
	{
		VXPLIB_BEGIN

		if(__vxptooltipmanager::m_pInterface && ClassName)
		{
			_bstr_t cname(ClassName);
			__vxptooltipmanager::m_pInterface->AddMenuClass(cname);
		}

		VXPLIB_END
	}
	void AddToolbarClass(LPCTSTR ClassName)
	{
		VXPLIB_BEGIN

		if(__vxptooltipmanager::m_pInterface && ClassName)
		{
			_bstr_t cname(ClassName);
			__vxptooltipmanager::m_pInterface->AddToolbarClass(cname);
		}

		VXPLIB_END
	}
	bool AddToolbarWndClass(HWND hWnd)
	{
		VXPLIB_BEGIN

		if(__vxptooltipmanager::m_pInterface)
			return __vxptooltipmanager::m_pInterface->AddToolbarWndClass((long)hWnd)?true:false;

		VXPLIB_END

		return false;
	}
	void MarkDirty()
	{
		VXPLIB_BEGIN

		if(__vxptooltipmanager::m_pInterface)
			__vxptooltipmanager::m_pInterface->MarkDirty();

		VXPLIB_END
	}

protected:

	// Events:
	virtual BSTR OnActivateCustomTooltip(HWND hWnd, LPCTSTR ClassName, short x, short y){return NULL;};
};

////////////////////////////////////////
// This is a service class, and should
// not be used by client applications.
////////////////////////////////////////
class __vxpmenuhook:public __vxperrorhandler
{
	friend class CVXPMenuHook;

public:

	VXP_IMPLEMENT_HANDLE(HWND, hWnd)
	VXP_IMPLEMENT_SIMPLE(COLORREF, bgColor, RGB(0, 0, 0))
	VXP_IMPLEMENT_SIMPLE(COLORREF, selColor, RGB(0, 0, 0))
	VXP_IMPLEMENT_SIMPLE(COLORREF, capColor, RGB(0, 0, 0))
	VXP_IMPLEMENT_LONG(MenuBarExtenderID)	
	VXP_IMPLEMENT_SIMPLE(BYTE, MenuTransparency, 0)
	VXP_IMPLEMENT_SIMPLE(BYTE, WindowTransparency, 0)
	VXP_IMPLEMENT_READ_BOOL(sysFadedMenus)

private:

	__vxpmenuhook():m_pInterface(NULL){}
	IVXPMenuHook * m_pInterface;

#ifdef VXPLIB_WITH_EVENTS
	__vxpmenuhookevents m_sink;
#endif
};

/////////////////////////////////////////////////
// class CVXPMenuHook
// This class is to contain interface IVXPMenuHook.
// It has all the same methods, properties and events
// as the IVXPMenuHook interface to access them seamlessly
// just as though working directly with the COM interface.
/////////////////////////////////////////////////
class CVXPMenuHook:public __vxpmenuhook
{
public:

	// Constructor;
	// Passing false will prevent the constructor from creating
	// a new instance of the interface object.
	CVXPMenuHook(bool bInitialize = true)
	{
		if(bInitialize)
		{
			IVXPMenuHook * pHook;
			::CoCreateInstance(CLSID_VXPMenuHook, NULL, CLSCTX_ALL, IID_IVXPMenuHook, (void**)&pHook);
			Attach(pHook);
		}
	}

	// Destructor;
	// It will detach and release the interface object
	// automatically when the object goes out of scope.
	virtual ~CVXPMenuHook()
	{
		Detach();
	}

	// Attaches an object instance to the class so that all
	// methods and properties of the COM interface would be
	// accessible via properties and methods of the class.
	void Attach(IVXPMenuHook * pHook)
	{
		Detach();
		if(pHook)
		{
			m_pInterface = pHook;
#ifdef VXPLIB_WITH_EVENTS
			m_sink.m_pClient = this;
			m_sink.DispEventAdvise(m_pInterface);
#endif
		}
	}

	// Detaches previously attached COM interface,
	// and releases it.
	void Detach()
	{
		if(m_pInterface)
		{
#ifdef VXPLIB_WITH_EVENTS
			m_sink.DispEventUnadvise(m_pInterface);
			m_sink.m_pClient = NULL;
#endif
			m_pInterface->Release();
			m_pInterface = NULL;
		}
	}

	// This method will return true, if there was an instance
	// of IVXPMenuHook attached to this class object.
	// This way it is also possible to verify whether VXPLib
	// is available on the computer.
	bool IsAttached()
	{
		return (m_pInterface != NULL);
	}

////////////////
// Properties:
////////////////

	VXP_DECLARE_READ(HWND, hWnd)
	VXP_DECLARE(COLORREF, bgColor)
	VXP_DECLARE(COLORREF, selColor)
	VXP_DECLARE(COLORREF, capColor)
	VXP_DECLARE(long, MenuBarExtenderID)
	VXP_DECLARE(BYTE, MenuTransparency)
	VXP_DECLARE(BYTE, WindowTransparency)
	VXP_DECLARE_READ(bool, sysFadedMenus)

////////////
// Methods:
////////////

	bool HookWnd(HWND hWnd)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->HookWnd((long)hWnd)?true:false;

		VXPLIB_END

		return false;
	}
	HWND UnHookWnd()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return (HWND)m_pInterface->UnHookWnd();

		VXPLIB_END

		return NULL;
	}
	bool Override(long uItem, bool bByPos = false, long ItemID = 0, long Sub1 = -1, long Sub2 = -1, long Sub3 = -1, long Sub4 = -1, long Sub5 = -1, long Sub6 = -1, long Sub7 = -1, long Sub8 = -1, long Sub9 = -1)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->Override(uItem, bByPos, ItemID, Sub1, Sub2, Sub3, Sub4, Sub5, Sub6, Sub7, Sub8, Sub9)?true:false;

		VXPLIB_END

		return false;
	}
	bool OverrideMenu(HMENU hMenu, long uItem, bool bByPos = false, long ItemID = 0, long Sub1 = -1, long Sub2 = -1, long Sub3 = -1, long Sub4 = -1, long Sub5 = -1, long Sub6 = -1, long Sub7 = -1, long Sub8 = -1, long Sub9 = -1)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->OverrideMenu((long)hMenu, uItem, bByPos, ItemID, Sub1, Sub2, Sub3, Sub4, Sub5, Sub6, Sub7, Sub8, Sub9)?true:false;

		VXPLIB_END

		return false;
	}
	bool OverrideSys(long uItem, bool bByPos = false, long ItemID = 0, long Sub1 = -1, long Sub2 = -1, long Sub3 = -1)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->OverrideSys(uItem, bByPos, ItemID, Sub1, Sub2, Sub3)?true:false;

		VXPLIB_END

		return false;
	}
	bool DrawItemBackground(bool bXPSelection = false)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->DrawItemBackground(bXPSelection)?true:false;

		VXPLIB_END

		return false;
	}
	bool DrawItemBackgroundEx(short LeftMargin = 0, short RightMargin = 0, bool bXPSelection = false)
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->DrawItemBackgroundEx(LeftMargin, RightMargin, bXPSelection)?true:false;

		VXPLIB_END

		return false;
	}

	void ResetPalette()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			m_pInterface->ResetPalette();

		VXPLIB_END
	}
	bool DrawMenuBar()
	{
		VXPLIB_BEGIN

		if(m_pInterface)
			return m_pInterface->DrawMenuBar()?true:false;

		VXPLIB_END

		return false;
	}

	// Events:
	virtual void OnMenuInit(HMENU hMenu, short ItemPos, bool IsWindowMenu){};
	virtual void OnMenuDone(HMENU hMenu){};
	virtual void OnMeasureItem(long ItemID, short & ItemWidth, short & ItemHeight){};
	virtual void OnDrawItem(HMENU hMenu, HDC hDC, long ItemID, VXPMenuItemFlags Flags, short X, short Y, short W, short H){};
	virtual void OnMenuSelect(HMENU hMenu, bool bOwnerDrawn, bool bSelected, bool bPopUp, long ItemID){};
	virtual void OnSettingsChanged(){};
	virtual void OnContextMenu(HWND hWnd, short X, short Y){};
	virtual void OnDrawMenuBarExtender(HDC hDC, short X, short Y, short W, short H){};
};

#ifdef VXPLIB_WITH_EVENTS

// All the following event handlers simply redirect all invocations
// to the appropriate C++ events, whilst converting some parameters
// into C++ native types

HRESULT __stdcall __vxpquickhtmlevents::OnSetDefaults()
{
	if(m_pClient)
		m_pClient->OnSetDefaults();
	return S_OK;
};

HRESULT __stdcall __vxpquickhtmlevents::OnDocumentGetSize(BSTR DocumentPath, BSTR Param, short * Width, short * Height)
{
	if(m_pClient)
	{
		_bstr_t doc_path(DocumentPath);
		_bstr_t param(Param);
		m_pClient->OnDocumentGetSize(doc_path, param, *Width, *Height);
	}
	return S_OK;
}

HRESULT __stdcall __vxpquickhtmlevents::OnControlGetSize(long ID, BSTR Name, short * Width, short * Height, BSTR Param)
{
	if(m_pClient)
	{
		_bstr_t name(Name);
		_bstr_t param(Param);
		m_pClient->OnControlGetSize(ID, name, *Width, *Height, param);
	}
	return S_OK;
}


HRESULT __stdcall __vxpquickhtmlevents::OnDocumentDraw(BSTR DocumentPath, BSTR Param, long DC, short x, short y, short Width, short Height)
{
	if(m_pClient)
	{
		_bstr_t doc_path(DocumentPath);
		_bstr_t param(Param);
		m_pClient->OnDocumentDraw(doc_path, param, (HDC)DC, x, y, Width, Height);
	}
	return S_OK;
}

HRESULT __stdcall __vxpquickhtmlevents::OnSourceChanged(VARIANT_BOOL IsEmpty)
{
	if(m_pClient)
		m_pClient->OnSourceChanged(IsEmpty?true:false);
	return S_OK;
}

HRESULT __stdcall __vxpquickhtmlevents::OnRecalculate(VARIANT_BOOL Begin, VARIANT_BOOL SourceChanged)
{
	if(m_pClient)
		m_pClient->OnRecalculate(Begin?true:false, SourceChanged?true:false);
	return S_OK;
}

HRESULT __stdcall __vxpquickhtmlevents::OnGetText(BSTR FilePath, BSTR Res, long Size, long ID, BSTR * Result)
{
	if(m_pClient)
	{
		_bstr_t file_path(FilePath);
		_bstr_t res(Res);
		*Result = m_pClient->OnGetText(file_path, res, Size, ID);
	}
	return S_OK;
}

HRESULT __stdcall __vxptooltipevents::OnStateChanged(VXPTooltipState NewState)
{
	if(m_pClient)
		m_pClient->OnStateChanged(NewState);
	return S_OK;
}

HRESULT __stdcall __vxptooltipevents::OnSoundPlay(IVXPSound * bgsound, VARIANT_BOOL * StartedPlaying)
{
	if(m_pClient)
		*StartedPlaying = m_pClient->OnSoundPlay(m_pClient->settings.bgsound)?TRUE:FALSE;
	return S_OK;
}

HRESULT __stdcall __vxptooltipevents::OnSoundAdjust(IVXPSound * bgsound, float FadingPercent)
{
	if(m_pClient)
		m_pClient->OnSoundAdjust(m_pClient->settings.bgsound, FadingPercent);
	return S_OK;
}

HRESULT __stdcall __vxptooltipevents::OnSoundStop(IVXPSound * bgsound)
{
	if(m_pClient)
		m_pClient->OnSoundStop(m_pClient->settings.bgsound);
	return S_OK;
}

HRESULT __stdcall __vxptooltipevents::OnWndMsg(long uMsg, long wParam, long lParam, VARIANT_BOOL * bCallDefWndProc, long * result)
{
	if(m_pClient)
	{
		bool bCallDef = (*bCallDefWndProc)?true:false;
		*result = m_pClient->OnWndMsg(uMsg, wParam, lParam, bCallDef);
		*bCallDefWndProc = bCallDef?TRUE:FALSE;
	}
	return S_OK;
}

HRESULT __stdcall __vxptooltipevents::OnMouseClick(long X, long Y, long uMsg, long hWnd, long wHitTestCode, VARIANT_BOOL * bProcessed)
{
	if(m_pClient)
		*bProcessed = m_pClient->OnMouseClick(X, Y, uMsg, (HWND)hWnd, wHitTestCode);
	return S_OK;
}

HRESULT __stdcall __vxptooltipmanagerevents::OnActivateCustomTooltip(long hWnd, BSTR ClassName, short x, short y, BSTR * Text)
{
	if(m_pClient)
	{
		_bstr_t class_name(ClassName);
		*Text = m_pClient->OnActivateCustomTooltip((HWND)hWnd, class_name, x, y);
	}
	return S_OK;
}

HRESULT __stdcall __vxpmenuhookevents::OnMenuInit(long hMenu, short ItemPos, VARIANT_BOOL IsWindowMenu)
{
	if(m_pClient)
		m_pClient->OnMenuInit((HMENU)hMenu, ItemPos, IsWindowMenu?true:false);
	return S_OK;
}

HRESULT __stdcall __vxpmenuhookevents::OnMenuDone(long hMenu)
{
	if(m_pClient)
		m_pClient->OnMenuDone((HMENU)hMenu);
	return S_OK;
}

HRESULT __stdcall __vxpmenuhookevents::OnMeasureItem(long ItemID, short * ItemWidth, short * ItemHeight)
{
	if(m_pClient)
		m_pClient->OnMeasureItem(ItemID, *ItemWidth, *ItemHeight);
	return S_OK;
}

HRESULT __stdcall __vxpmenuhookevents::OnDrawItem(long hMenu, long hDC, long ItemID, enum VXPMenuItemFlags Flags, short X, short Y, short W, short H)
{
	if(m_pClient)
		m_pClient->OnDrawItem((HMENU)hMenu, (HDC)hDC, ItemID, Flags, X, Y, W, H);
	return S_OK;
}

HRESULT __stdcall __vxpmenuhookevents::OnMenuSelect(long hMenu, VARIANT_BOOL bOwnerDrawn, VARIANT_BOOL bSelected, VARIANT_BOOL bPopUp, long ItemID)
{
	if(m_pClient)
		m_pClient->OnMenuSelect((HMENU)hMenu, bOwnerDrawn?true:false, bSelected?true:false, bPopUp?true:false, ItemID);
	return S_OK;
}

HRESULT __stdcall __vxpmenuhookevents::OnSettingsChanged()
{
	if(m_pClient)
		m_pClient->OnSettingsChanged();
	return S_OK;
}

HRESULT __stdcall __vxpmenuhookevents::OnContextMenu(long hWnd, short X, short Y)
{
	if(m_pClient)
		m_pClient->OnContextMenu((HWND)hWnd, X, Y);
	return S_OK;
}

HRESULT __stdcall __vxpmenuhookevents::OnDrawMenuBarExtender(long hDC, short X, short Y, short W, short H)
{
	if(m_pClient)
		m_pClient->OnDrawMenuBarExtender((HDC)hDC, X, Y, W, H);
	return S_OK;
}

#endif

// This namespace contains 3 functions to simplify
// registering/unregistering VXPLib.DLL programmatically.
namespace VXPLib
{
	// Returns True, if the library is available
	// (if it has been copied and registered on the PC)
	static bool IsLibraryAvailable()
	{
		ITypeLib * ptlib;
		if(LoadRegTypeLib(LIBID_VXPLibrary, 1, 0, 0, &ptlib) == S_OK)
		{
			ptlib->Release();
			// Now try to create a new instance of the parser
			// (in case the library was registered, and then
			// deleted from PC)
			IVXPQuickHTML * pHTML;
			::CoCreateInstance(CLSID_VXPQuickHTML, NULL, CLSCTX_ALL, IID_IVXPQuickHTML, (void**)&pHTML);
			if(pHTML)
			{
				pHTML->Release();
				return true;
			}
		}
		return false;
	}

	// Registers the library using full path to VXPLib.dll
	// It also can be a local path, or just the library name,
	// if the file is in the current directory.
	// Function returns True, if the library was registered
	// successfully, or False otherwise.
	// Calling this procedure is the same as command line:
	// RegSvr32.exe /S path\VXPLib.dll
	static bool RegisterLibrary(LPCTSTR strFullPath)
	{
		HMODULE hModule = ::LoadLibrary(strFullPath);
		if(hModule)
		{
			FARPROC RegProc = GetProcAddress(hModule, "DllRegisterServer");
			if(RegProc)
				RegProc();
			::FreeLibrary(hModule);
			return RegProc?true:false;
		}
		return false;
	}

	// Unregisters the library using full path to VXPLib.dll
	// It also can be a local path, or just the library name,
	// if the file is in the current directory.
	// Function returns True, if the library was unregistered
	// successfully, or False otherwise.
	// Calling this procedure is the same as command line:
	// RegSvr32.exe /U /S path\VXPLib.dll
	static bool UnregisterLibrary(LPCTSTR strFullPath)
	{
		HMODULE hModule = ::LoadLibrary(strFullPath);
		if(hModule)
		{
			FARPROC RegProc = GetProcAddress(hModule, "DllUnregisterServer");
			if(RegProc)
				RegProc();
			::FreeLibrary(hModule);
			return RegProc?true:false;
		}
		return false;
	}
};

#pragma warning(pop)

}

#endif
