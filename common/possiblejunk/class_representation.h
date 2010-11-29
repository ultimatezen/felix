// class_representation.h: interface for the class_representation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASS_REPRESENTATION_H__C46ED621_0581_4F7E_B961_AF9977C2D3DE__INCLUDED_)
#define AFX_CLASS_REPRESENTATION_H__C46ED621_0581_4F7E_B961_AF9977C2D3DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StringEx.h"
#include <list>
#include <set>

typedef std::list< string >	prog_lines ;
typedef std::set< string > type_set ;

struct types
{
	type_set m_types ;
	types()
	{
		m_types.insert("char") ;
		m_types.insert("short") ;
		m_types.insert("int") ;
		m_types.insert("long") ;
		m_types.insert("unsigned") ;
		m_types.insert("float") ;
		m_types.insert("double") ;
		m_types.insert("bool") ;
		m_types.insert("ABORTPROC") ;
		m_types.insert("ACMDRIVERENUMCB") ;
		m_types.insert("ACMDRIVERPROC") ;
		m_types.insert("ACMFILTERCHOOSEHOOKPROC") ;
		m_types.insert("ACMFILTERENUMCB") ;
		m_types.insert("ACMFILTERTAGENUMCB") ;
		m_types.insert("ACMFORMATCHOOSEHOOKPROC") ;
		m_types.insert("ACMFORMATENUMCB") ;
		m_types.insert("ACMFORMATTAGENUMCB") ;
		m_types.insert("APPLET_PROC") ;
		m_types.insert("ATOM") ;
		m_types.insert("BOOL") ;
		m_types.insert("BOOLEAN") ;
		m_types.insert("BYTE") ;
		m_types.insert("CALINFO_ENUMPROC") ;
		m_types.insert("CALLBACK") ;
		m_types.insert("CHAR") ;
		m_types.insert("COLORREF") ;
		m_types.insert("CONST") ;
		m_types.insert("CRITICAL_SECTION") ;
		m_types.insert("CTRYID") ;
		m_types.insert("DATEFMT_ENUMPROC") ;
		m_types.insert("DESKTOPENUMPROC") ;
		m_types.insert("DLGPROC") ;
		m_types.insert("DRAWSTATEPROC") ;
		m_types.insert("DWORD") ;
		m_types.insert("EDITWORDBREAKPROC") ;
		m_types.insert("ENHMFENUMPROC") ;
		m_types.insert("ENUMRESLANGPROC") ;
		m_types.insert("ENUMRESNAMEPROC") ;
		m_types.insert("ENUMRESTYPEPROC") ;
		m_types.insert("FARPROC") ;
		m_types.insert("FLOAT") ;
		m_types.insert("FILE_SEGMENT_ELEMENT") ;
		m_types.insert("FONTENUMPROC") ;
		m_types.insert("GOBJENUMPROC") ;
		m_types.insert("GRAYSTRINGPROC") ;
		m_types.insert("HACCEL") ;
		m_types.insert("HANDLE") ;
		m_types.insert("HBITMAP") ;
		m_types.insert("HBRUSH") ;
		m_types.insert("HCOLORSPACE") ;
		m_types.insert("HCONV") ;
		m_types.insert("HCONVLIST") ;
		m_types.insert("HCURSOR") ;
		m_types.insert("HDC") ;
		m_types.insert("HDDEDATA") ;
		m_types.insert("HDESK") ;
		m_types.insert("HDROP") ;
		m_types.insert("HDWP") ;
		m_types.insert("HENHMETAFILE") ;
		m_types.insert("HFILE") ;
		m_types.insert("HFONT") ;
		m_types.insert("HGDIOBJ") ;
		m_types.insert("HGLOBAL") ;
		m_types.insert("HHOOK") ;
		m_types.insert("HICON") ;
		m_types.insert("HIMAGELIST") ;
		m_types.insert("HIMC") ;
		m_types.insert("HINSTANCE") ;
		m_types.insert("HKEY") ;
		m_types.insert("HKL") ;
		m_types.insert("HLOCAL") ;
		m_types.insert("HMENU") ;
		m_types.insert("HMETAFILE") ;
		m_types.insert("HMODULE") ;
		m_types.insert("HMONITOR") ;
		m_types.insert("HOOKPROC") ;
		m_types.insert("HPALETTE") ;
		m_types.insert("HPEN") ;
		m_types.insert("HRGN") ;
		m_types.insert("HRSRC") ;
		m_types.insert("HSZ") ;
		m_types.insert("HTREEITEM") ;
		m_types.insert("HWINSTA") ;
		m_types.insert("HWND") ;
		m_types.insert("INT") ;
		m_types.insert("IPADDR") ;
		m_types.insert("LANGID") ;
		m_types.insert("LCID") ;
		m_types.insert("LCSCSTYPE") ;
		m_types.insert("LCSGAMUTMATCH") ;
		m_types.insert("LCTYPE") ;
		m_types.insert("LINEDDAPROC") ;
		m_types.insert("LOCALE_ENUMPROC") ;
		m_types.insert("LONG") ;
		m_types.insert("LONGLONG") ;
		m_types.insert("LPARAM") ;
		m_types.insert("LPBOOL") ;
		m_types.insert("LPBYTE") ;
		m_types.insert("LPCCHOOKPROC") ;
		m_types.insert("LPCFHOOKPROC") ;
		m_types.insert("LPCOLORREF") ;
		m_types.insert("LPCRITICAL_SECTION") ;
		m_types.insert("LPCSTR") ;
		m_types.insert("LPCTSTR") ;
		m_types.insert("LPCVOID") ;
		m_types.insert("LPCWSTR") ;
		m_types.insert("LPDWORD") ;
		m_types.insert("LPFIBER_START_ROUTINE") ;
		m_types.insert("LPFRHOOKPROC") ;
		m_types.insert("LPHANDLE") ;
		m_types.insert("LPHANDLER_FUNCTION") ;
		m_types.insert("LPINT") ;
		m_types.insert("LPLONG") ;
		m_types.insert("LPOFNHOOKPROC") ;
		m_types.insert("LPPAGEPAINTHOOK") ;
		m_types.insert("LPPAGESETUPHOOK") ;
		m_types.insert("LPPRINTHOOKPROC") ;
		m_types.insert("LPPROGRESS_ROUTINE") ;
		m_types.insert("LPSETUPHOOKPROC") ;
		m_types.insert("LPSTR") ;
		m_types.insert("LPSTREAM") ;
		m_types.insert("LPTHREAD_START_ROUTINE") ;
		m_types.insert("LPTSTR") ;
		m_types.insert("LPVOID") ;
		m_types.insert("LPWORD") ;
		m_types.insert("LPWSTR") ;
		m_types.insert("LRESULT") ;
		m_types.insert("LUID") ;
		m_types.insert("PBOOL") ;
		m_types.insert("PBOOLEAN") ;
		m_types.insert("PBYTE") ;
		m_types.insert("PCHAR") ;
		m_types.insert("PCRITICAL_SECTION") ;
		m_types.insert("PCSTR") ;
		m_types.insert("PCTSTR") ;
		m_types.insert("PCWCH") ;
		m_types.insert("PCWSTR") ;
		m_types.insert("PDWORD") ;
		m_types.insert("PFLOAT") ;
		m_types.insert("PFNCALLBACK") ;
		m_types.insert("PHANDLE") ;
		m_types.insert("PHANDLER_ROUTINE") ;
		m_types.insert("PHKEY") ;
		m_types.insert("PINT") ;
		m_types.insert("PLCID") ;
		m_types.insert("PLONG") ;
		m_types.insert("PLUID") ;
		m_types.insert("PROPENUMPROC") ;
		m_types.insert("PROPENUMPROCEX") ;
		m_types.insert("PSHORT") ;
		m_types.insert("PSTR") ;
		m_types.insert("PTBYTE") ;
		m_types.insert("PTCHAR") ;
		m_types.insert("PTIMERAPCROUTINE") ;
		m_types.insert("PTSTR") ;
		m_types.insert("PUCHAR") ;
		m_types.insert("PUINT") ;
		m_types.insert("PULONG") ;
		m_types.insert("PUSHORT") ;
		m_types.insert("PVOID") ;
		m_types.insert("PWCHAR") ;
		m_types.insert("PWORD") ;
		m_types.insert("PWSTR") ;
		m_types.insert("REGISTERWORDENUMPROC") ;
		m_types.insert("REGSAM") ;
		m_types.insert("SC_HANDLE") ;
		m_types.insert("SC_LOCK") ;
		m_types.insert("SENDASYNCPROC") ;
		m_types.insert("SERVICE_STATUS_HANDLE") ;
		m_types.insert("SHORT") ;
		m_types.insert("TBYTE") ;
		m_types.insert("TCHAR") ;
		m_types.insert("TIMEFMT_ENUMPROC") ;
		m_types.insert("TIMERPROC") ;
		m_types.insert("UCHAR") ;
		m_types.insert("UINT") ;
		m_types.insert("ULONG") ;
		m_types.insert("ULONGLONG") ;
		m_types.insert("UNSIGNED") ;
		m_types.insert("USHORT") ;
		m_types.insert("VOID") ;
		m_types.insert("WCHAR") ;
		m_types.insert("WINAPI") ;
		m_types.insert("WINSTAENUMPROC") ;
		m_types.insert("WNDENUMPROC") ;
		m_types.insert("WNDPROC") ;
		m_types.insert("WORD") ;
		m_types.insert("WPARAM") ;
		m_types.insert("YIELDPROC") ;
	}
	bool is_built_in_type( const string candidate )
	{
		if ( m_types.find(candidate) == m_types.end() ) 
		{
			return false ;
		}
		return true ;
	}
};

struct variable_rep  
{
	string	m_type ;
	string	m_name ;
	string	m_default ;
	variable_rep()
	{
	}
	variable_rep( const variable_rep &rhs )
	{
		internal_copy( rhs ) ;
	}
	variable_rep& operator=( const variable_rep &rhs )
	{
		internal_copy( rhs ) ;
		return *this ;
	}
private:
	void internal_copy( const variable_rep &rhs )
	{
		m_type = rhs.m_type ;
		m_name = rhs.m_name ;
		m_default = rhs.m_default ;
	}
};

typedef std::list< variable_rep > variable_list ;

struct method_rep
{
	string			m_retval ;
	string			m_name ;
	variable_list	m_variables ;
	
	method_rep()
	{
	}
	method_rep( const method_rep &rhs )
	{
		internal_copy( rhs ) ;
	}
	method_rep& operator=( const method_rep &rhs )
	{
		internal_copy( rhs ) ;
		return *this ;
	}
private:
	void internal_copy( const method_rep &rhs )
	{
		m_retval = rhs.m_retval ;
		m_name = rhs.m_name ;

		// the old swap trick
		variable_list vars( rhs.m_variables ) ;
		m_variables.swap( vars ) ;
	}
};

typedef std::list< method_rep > method_list ;

class class_representation  
{
	variable_list	m_variables ;
	method_list		m_methods ;
	
public:
	variable_rep suss_variable( const string &line );
	method_rep suss_method(const string &line);
	int count_open_braces( const string &line );
	int count_close_braces( const string &line );
	void suss_line( const string &line );
	class_representation( const class_representation &rhs );
	class_representation();
	virtual ~class_representation();
	class_representation& operator =(const class_representation &rhs) ;

	prog_lines::iterator suss_class( prog_lines::iterator pos ) ;
private:
	void internal_copy( const class_representation &rhs );
};

#endif // !defined(AFX_CLASS_REPRESENTATION_H__C46ED621_0581_4F7E_B961_AF9977C2D3DE__INCLUDED_)
