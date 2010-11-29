#include "stdafx.h"
#include "DumpExceptionInfo.h"
#include "Path.h"
#include "File.h"
#include "DebugUtilities.h"
#include "DbgHelp.h"
#pragma comment(lib, "DbgHelp.lib")
#include "MiniVersion.h"
#include "GetWinVer.h"
#include "MiscWrappers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif

////////////////////////////////////////////////////////////////////////////////
// FormatTime
// Format the specified FILETIME to output in a human readable format,
// without using the C run time.
string FormatTime( FILETIME TimeToPrint)
{
	misc_wrappers::date sysdate ;

	sysdate = TimeToPrint ;

	tstring output = sysdate.get_date_time_string() ;

	return string2string( output ) ;
}

///////////////////////////////////////////////////////////////////////////////
// DumpModuleInfo
// Print information about a code module (DLL or EXE) such as its size,
// location, time stamp, etc.
bool DumpModuleInfo(file::file &log_file, HINSTANCE ModuleHandle, int nModuleNo)
{
	bool rc = false;
	char szModName[MAX_PATH*2] = {0} ;

	if (GetModuleFileNameA(ModuleHandle, szModName, sizeof(szModName)-2) > 0)
	{
		// If GetModuleFileName returns greater than zero then this must
		// be a valid code module address. Therefore we can try to walk
		// our way through its structures to find the link time stamp.
		IMAGE_DOS_HEADER *DosHeader = (IMAGE_DOS_HEADER*)ModuleHandle;
		if (IMAGE_DOS_SIGNATURE != DosHeader->e_magic)
	    	return false;

		IMAGE_NT_HEADERS *NTHeader = (IMAGE_NT_HEADERS*)((char *)DosHeader
					+ DosHeader->e_lfanew);
		if (IMAGE_NT_SIGNATURE != NTHeader->Signature)
	    	return false;

		// open the code module file so that we can get its file date and size
		HANDLE mod_file = ::CreateFileA(szModName, GENERIC_READ,
			FILE_SHARE_READ, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0) ;

		char module_buffer[512] = {0} ;
		HRESULT hr = S_OK ;
		LPSTR dest_end = NULL ;
		size_t remaining = 0 ;
		DWORD flags = STRSAFE_FILL_BEHIND_NULL | STRSAFE_IGNORE_NULLS | STRSAFE_FILL_ON_FAILURE | STRSAFE_NULL_ON_FAILURE ;
		hr = StringCbPrintfExA( module_buffer, 512, &dest_end, &remaining, 
			flags,
			"Module %d:\r\n%s\r\n", 
			nModuleNo, 
			szModName 
			) ;
		ASSERT_HRESULT( hr ) ;

		log_file.write( module_buffer, 512 - remaining ) ;

		try
		{
			string time_buffer ;
			
			DWORD FileSize = 0;
			if ( mod_file != INVALID_HANDLE_VALUE )
			{
				FILETIME LastWriteTime;
				if (GetFileTime(mod_file, 0, 0, &LastWriteTime))
				{
					time_buffer = FormatTime(LastWriteTime);
				}
				FileSize = ::GetFileSize( mod_file, NULL ) ;
				::CloseHandle( mod_file ) ;
			}

			string line ;
			line << "File Size: " << int2string( FileSize ) << "\t" ;
			line << "File Time: " << time_buffer << "\r\n" ;
			log_file.write( line ) ;			
		}
		catch ( CException &e )
		{
			e ;
			log_file.write( "Unable to retrieve module file size and time\r\n" ) ;
		}

		hr = StringCbPrintfExA( module_buffer, 512, &dest_end, &remaining, 
				flags,
				"Image Base: 0x%08X  Image Size: 0x%08X\r\n", 
				NTHeader->OptionalHeader.ImageBase, 
				NTHeader->OptionalHeader.SizeOfImage 
			) ;
		ASSERT_HRESULT( hr ) ;
		log_file.write( module_buffer, 512 - remaining ) ;
		

		hr = StringCbPrintfExA( module_buffer, 512, &dest_end, &remaining, 
				flags,
				"Checksum:   0x%08X  Time Stamp: 0x%08X\r\n", 
				NTHeader->OptionalHeader.CheckSum, NTHeader->FileHeader.TimeDateStamp 
			) ;
		ASSERT_HRESULT( hr ) ;
		log_file.write( module_buffer, 512 - remaining ) ;

		log_file.write( "Version Information:\r\n" );

		CMiniVersion ver(szModName);
		char szBuf[200];
		WORD dwBuf[4];

		ver.GetCompanyName(szBuf, sizeof(szBuf)-1);
		log_file << "   Company:    " << szBuf << "\r\n" ;

		ver.GetProductName(szBuf, sizeof(szBuf)-1);
		log_file << "   Product:    " << szBuf << "\r\n" ;

		ver.GetFileDescription(szBuf, sizeof(szBuf)-1);
		log_file << "   FileDesc:   " << szBuf << "\r\n" ;

		ver.GetFileVersion(dwBuf);
		log_file << "   FileVer:    " 
			<< int2string( dwBuf[0] ) << "." 
			<< int2string( dwBuf[1] ) << "." 
			<< int2string( dwBuf[2] ) << "." 
			<< int2string( dwBuf[3] ) << "\r\n" ;

		ver.GetProductVersion(dwBuf);
		log_file << "   ProdVer:    " 
			<< int2string( dwBuf[0] ) << "." 
			<< int2string( dwBuf[1] ) << "." 
			<< int2string( dwBuf[2] ) << "." 
			<< int2string( dwBuf[3] ) << "\r\n" ;

		ver.Release();

		log_file.write( "\r\n" );

		rc = true;
	}
	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// DumpModuleList
// Scan memory looking for code modules (DLLs or EXEs). VirtualQuery is used
// to find all the blocks of address space that were reserved or committed,
// and ShowModuleInfo will display module information if they are code
// modules.
void DumpModuleList(file::file &log_file)
{
	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);

	const size_t PageSize = SystemInfo.dwPageSize;

	// Set NumPages to the number of pages in the 4GByte address space,
	// while being careful to avoid overflowing ints
	const size_t NumPages = 4 * size_t(ONEG / PageSize);
	size_t pageNum = 0;
	void *LastAllocationBase = 0;

	int nModuleNo = 1;

	while (pageNum < NumPages)
	{
		MEMORY_BASIC_INFORMATION MemInfo;
		if (VirtualQuery((void *)(pageNum * PageSize), &MemInfo,
					sizeof(MemInfo)))
		{
			if (MemInfo.RegionSize > 0)
			{
				// Adjust the page number to skip over this block of memory
				pageNum += MemInfo.RegionSize / PageSize;
				if (MemInfo.State == MEM_COMMIT && MemInfo.AllocationBase >
							LastAllocationBase)
				{
					// Look for new blocks of committed memory, and try
					// recording their module names - this will fail
					// gracefully if they aren't code modules
					LastAllocationBase = MemInfo.AllocationBase;
					if (DumpModuleInfo(log_file, 
									   (HINSTANCE)LastAllocationBase, 
									   nModuleNo))
					{
						nModuleNo++;
					}
				}
			}
			else
				pageNum += SIXTYFOURK / PageSize;
		}
		else
			pageNum += SIXTYFOURK / PageSize;

		// If VirtualQuery fails we advance by 64K because that is the
		// granularity of address space doled out by VirtualAlloc()
	}
}

///////////////////////////////////////////////////////////////////////////////
// DumpSystemInformation
// Record information about the user's system, such as processor type, amount
// of memory, etc.
void DumpSystemInformation(file::file &log_file)
{
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	string time_buffer = FormatTime(CurrentTime);

	log_file << "Error occurred at " << time_buffer << ".\r\n" ;

	char szModuleName[MAX_PATH*2];
	ZeroMemory(szModuleName, sizeof(szModuleName));
	if (GetModuleFileNameA(0, szModuleName, _countof(szModuleName)-2) <= 0)
		strcpy(szModuleName, "Unknown");

	char szUserName[200];
	ZeroMemory(szUserName, sizeof(szUserName));
	DWORD UserNameSize = _countof(szUserName)-2;
	if (!GetUserNameA(szUserName, &UserNameSize))
		strcpy(szUserName, "Unknown");

	file::directory long_dir( szModuleName, true ) ;
	log_file <<	 szModuleName
		<< ", run by " 
		<<    szUserName
		<< ".\r\n" ;

	// print out operating system
	char szWinVer[50], szMajorMinorBuild[50];
	int nWinVer;
	GetWinVer(szWinVer, &nWinVer, szMajorMinorBuild);
	log_file << "Operating system:  " 
		<<  szWinVer
		<< " (" 
		<<  szMajorMinorBuild
		<< ").\r\n" ;

	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	log_file << int2string( SystemInfo.dwNumberOfProcessors ) 
		<< " processor(s), type " 
		<< int2string( SystemInfo.dwProcessorType ) 
		<< ".\r\n" ;

	MEMORYSTATUS MemInfo;
	MemInfo.dwLength = sizeof(MemInfo);
	GlobalMemoryStatus(&MemInfo);

	// Print out info on memory, rounded up.
	log_file << int2string( MemInfo.dwMemoryLoad ) 
		<< "% memory in use.\r\n" ;

	log_file << int2string( (static_cast< int >( MemInfo.dwTotalPhys )  + ONEM - 1) / ONEM ) 
		<< " MBytes physical memory.\r\n" ;

	log_file << int2string( (static_cast< int >( MemInfo.dwAvailPhys )  + ONEM - 1) / ONEM)  
		<< " MBytes physical memory free.\r\n" ;

	log_file << "" << int2string( (static_cast< int >( MemInfo.dwTotalPageFile )  + ONEM - 1) / ONEM)  
		<< " MBytes paging file.\r\n" ;

	log_file << "" << int2string( (static_cast< int >( MemInfo.dwAvailPageFile )  + ONEM - 1) / ONEM)  
		<< " MBytes paging file free.\r\n" ;

	log_file << "" << int2string( (static_cast< int >( MemInfo.dwTotalVirtual )  + ONEM - 1) / ONEM)  
		<< " MBytes user address space.\r\n" ;

	log_file << "" << int2string( (static_cast< int >( MemInfo.dwAvailVirtual )  + ONEM - 1) / ONEM)  
		<< " MBytes user address space free.\r\n" ;
}

///////////////////////////////////////////////////////////////////////////////
// GetExceptionDescription
// Translate the exception code into something human readable
string GetExceptionDescription(DWORD ExceptionCode)
{
	struct ExceptionNames
	{
		DWORD	ExceptionCode;
		char *	ExceptionName;
	};

#if 0  // from winnt.h
#define STATUS_WAIT_0                    ((DWORD   )0x00000000L)    
#define STATUS_ABANDONED_WAIT_0          ((DWORD   )0x00000080L)    
#define STATUS_USER_APC                  ((DWORD   )0x000000C0L)    
#define STATUS_TIMEOUT                   ((DWORD   )0x00000102L)    
#define STATUS_PENDING                   ((DWORD   )0x00000103L)    
#define STATUS_SEGMENT_NOTIFICATION      ((DWORD   )0x40000005L)    
#define STATUS_GUARD_PAGE_VIOLATION      ((DWORD   )0x80000001L)    
#define STATUS_DATATYPE_MISALIGNMENT     ((DWORD   )0x80000002L)    
#define STATUS_BREAKPOINT                ((DWORD   )0x80000003L)    
#define STATUS_SINGLE_STEP               ((DWORD   )0x80000004L)    
#define STATUS_ACCESS_VIOLATION          ((DWORD   )0xC0000005L)    
#define STATUS_IN_PAGE_ERROR             ((DWORD   )0xC0000006L)    
#define STATUS_INVALID_HANDLE            ((DWORD   )0xC0000008L)    
#define STATUS_NO_MEMORY                 ((DWORD   )0xC0000017L)    
#define STATUS_ILLEGAL_INSTRUCTION       ((DWORD   )0xC000001DL)    
#define STATUS_NONCONTINUABLE_EXCEPTION  ((DWORD   )0xC0000025L)    
#define STATUS_INVALID_DISPOSITION       ((DWORD   )0xC0000026L)    
#define STATUS_ARRAY_BOUNDS_EXCEEDED     ((DWORD   )0xC000008CL)    
#define STATUS_FLOAT_DENORMAL_OPERAND    ((DWORD   )0xC000008DL)    
#define STATUS_FLOAT_DIVIDE_BY_ZERO      ((DWORD   )0xC000008EL)    
#define STATUS_FLOAT_INEXACT_RESULT      ((DWORD   )0xC000008FL)    
#define STATUS_FLOAT_INVALID_OPERATION   ((DWORD   )0xC0000090L)    
#define STATUS_FLOAT_OVERFLOW            ((DWORD   )0xC0000091L)    
#define STATUS_FLOAT_STACK_CHECK         ((DWORD   )0xC0000092L)    
#define STATUS_FLOAT_UNDERFLOW           ((DWORD   )0xC0000093L)    
#define STATUS_INTEGER_DIVIDE_BY_ZERO    ((DWORD   )0xC0000094L)    
#define STATUS_INTEGER_OVERFLOW          ((DWORD   )0xC0000095L)    
#define STATUS_PRIVILEGED_INSTRUCTION    ((DWORD   )0xC0000096L)    
#define STATUS_STACK_OVERFLOW            ((DWORD   )0xC00000FDL)    
#define STATUS_CONTROL_C_EXIT            ((DWORD   )0xC000013AL)    
#define STATUS_FLOAT_MULTIPLE_FAULTS     ((DWORD   )0xC00002B4L)    
#define STATUS_FLOAT_MULTIPLE_TRAPS      ((DWORD   )0xC00002B5L)    
#define STATUS_ILLEGAL_VLM_REFERENCE     ((DWORD   )0xC00002C0L)     
#endif

	ExceptionNames ExceptionMap[] =
	{
		{0x40010005, "Control-C"},
		{0x40010008, "Control-Break"},
		{0x80000002, "Datatype Misalignment"},
		{0x80000003, "a Breakpoint"},
		{0xc0000005, "an Access Violation"},
		{0xc0000006, "an In Page Error"},
		{0xc0000017, "a No Memory Error"},
		{0xc000001d, "an Illegal Instruction"},
		{0xc0000025, "a Noncontinuable Exception"},
		{0xc0000026, "an Invalid Disposition"},
		{0xc000008c, "an Array Bounds Exceeded"},
		{0xc000008d, "a Float Denormal Operand"},
		{0xc000008e, "a Float Divide by Zero"},
		{0xc000008f, "a Float Inexact Result"},
		{0xc0000090, "a Float Invalid Operation"},
		{0xc0000091, "a Float Overflow"},
		{0xc0000092, "a Float Stack Check"},
		{0xc0000093, "a Float Underflow"},
		{0xc0000094, "an Integer Divide by Zero"},
		{0xc0000095, "an Integer Overflow"},
		{0xc0000096, "a Privileged Instruction"},
		{0xc00000fD, "a Stack Overflow"},
		{0xc0000142, "a DLL Initialization Failed"},
		{0xe06d7363, "a Microsoft C++ Exception"},
	};

	for (int i = 0; i < sizeof(ExceptionMap) / sizeof(ExceptionMap[0]); i++)
		if (ExceptionCode == ExceptionMap[i].ExceptionCode)
			return string( ExceptionMap[i].ExceptionName );

	return string( "an Unknown exception type" );
}


///////////////////////////////////////////////////////////////////////////////
// DumpStack
void DumpStack(file::file &log_file, DWORD *pStack)
{
	log_file << "\r\n\r\nStack:\r\n" ;

	// Esp contains the bottom of the stack, or at least the bottom of
	// the currently used area.
	DWORD* pStackTop;

	__asm
	{
		// Load the top (highest address) of the stack from the
		// thread information block. It will be found there in
		// Win9x and Windows NT.
		mov	eax, fs:[4]
		mov pStackTop, eax
	}

	if (pStackTop > pStack + MaxStackDump)
		pStackTop = pStack + MaxStackDump;

	int Count = 0;

	DWORD* pStackStart = pStack;

	int nDwordsPrinted = 0;

	char stack_buffer[512] = {0} ;
	HRESULT hr = S_OK ;
	LPSTR dest_end = NULL ;
	size_t remaining = 0 ;
	DWORD flags = STRSAFE_FILL_BEHIND_NULL | STRSAFE_IGNORE_NULLS | STRSAFE_FILL_ON_FAILURE | STRSAFE_NULL_ON_FAILURE ;

	while (pStack + 1 <= pStackTop)
	{
		if ((Count % StackColumns) == 0)
		{
			pStackStart = pStack;
			nDwordsPrinted = 0;
			hr = StringCbPrintfExA( stack_buffer, 512, &dest_end, &remaining, 
				flags,
				"0x%08X: ", 
				*pStack 
				) ;
			ASSERT_HRESULT( hr ) ;
			log_file << stack_buffer ;
		}
		else
		{
			hr = StringCbPrintfExA( stack_buffer, 512, &dest_end, &remaining, 
				flags,
				"0x%08X ", 
				*pStack 
				) ;
			ASSERT_HRESULT( hr ) ;
			log_file << stack_buffer ;
		}

		if ((++Count % StackColumns) == 0 || pStack + 2 > pStackTop)
		{
			int n = nDwordsPrinted;
			while (n < 4)
			{
				log_file << "         "  ;
				n++;
			}

			char cbuf[2] = "\0" ;
			for (int i = 0; i < nDwordsPrinted; i++)
			{
				DWORD dwStack = *pStackStart;
				for (int j = 0; j < 4; j++)
				{
					cbuf[0] = (char)(dwStack & 0xFF);
					if (cbuf[0] < 0x20 || cbuf[0] > 0x7E)
						cbuf[0] = '.';
					log_file << cbuf[0] ;
					dwStack = dwStack >> 8;
				}
				pStackStart++;
			}

			log_file << "\r\n" ;
		}

		nDwordsPrinted++;
		pStack++;
	}
	log_file << "\r\n" ;
}

///////////////////////////////////////////////////////////////////////////////
// DumpRegisters
void DumpRegisters( file::file &log_file, PCONTEXT Context)
{
	char register_buff[1024] = {0} ;
	LPSTR dest_end(NULL) ;
	DWORD flags = STRSAFE_IGNORE_NULLS | STRSAFE_FILL_ON_FAILURE ;
	HRESULT str_cpy_result(S_OK) ;
	str_cpy_result = StringCbPrintfExA(register_buff, 1024, &dest_end, NULL, flags, 
		// Print out the register values in an XP error window compatible format.
		 "\r\n"
		 "Context:\r\n"
		 "EDI:    %08X  ESI: %08X  EAX:   %08X\r\n"
		 "EBX:    %08X  ECX: %08X  EDX:   %08X\r\n"
		 "EIP:    %08X  EBP: %08X  SegCs: %08X\r\n"
		 "EFlags: %08X  ESP: %08X  SegSs: %08X\r\n",
			// the arguments
			Context->Edi, 
			Context->Esi,
			Context->Eax, 
			Context->Ebx, 
			Context->Ecx,
			Context->Edx,
			Context->Eip, 
			Context->Ebp, 
			Context->SegCs, 
			Context->EFlags, 
			Context->Esp, 
			Context->SegSs
		) ;
	ASSERT_HRESULT( str_cpy_result ) ;

	log_file << register_buff ;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// RecordExceptionInfo
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4312) // conversion to smaller size

bool RecordExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs, 
								LPCSTR lpszMessage)
{
	// avoid recursion into this routine!!
	static bool first_time = true ;

	if ( ! first_time ) return false ;

	try
	{
		first_time = false ;

		file::CPath module_path ;

		module_path.GetModulePath( _Module.GetModuleInstance() ) ;
		module_path.Append( XCRASHREPORT_ERROR_LOG_FILE ) ;

		file::file log_file ;
		log_file.open_always( module_path.Path() ) ;

		ATLASSERT( log_file.is_open() ) ;
		if ( ! log_file.is_open() )
		{
			OutputDebugString(_T("Error creating exception report\r\n"));
			return false ;
		}

		PEXCEPTION_RECORD Exception = pExceptPtrs->ExceptionRecord;
		PCONTEXT          Context   = pExceptPtrs->ContextRecord;

		file::CPath crash_module_path ;

		MEMORY_BASIC_INFORMATION MemInfo;

		// VirtualQuery can be used to get the allocation base associated with a
		// code address, which is the same as the ModuleHandle. This can be used
		// to get the filename of the module that the crash happened in.
		if (VirtualQuery( (LPCVOID)( Context->Eip ), &MemInfo, sizeof(MemInfo)) )
		{
			DWORD len_copied(0) ;
			len_copied = crash_module_path.GetModFileName( (HINSTANCE)MemInfo.AllocationBase ) ;
			ATLASSERT( len_copied != 0 ) ;
		}

		string exception_description = GetExceptionDescription(Exception->ExceptionCode) ;
		char crash_buffer[ONEK] = {0} ;
		HRESULT hr = S_OK ;
		LPSTR dest_end = NULL ;
		size_t remaining = 0 ;
		DWORD flags = STRSAFE_FILL_BEHIND_NULL | STRSAFE_IGNORE_NULLS | STRSAFE_FILL_ON_FAILURE | STRSAFE_NULL_ON_FAILURE ;
		hr = StringCbPrintfExA( crash_buffer, ONEK, &dest_end, &remaining, 
			flags,
			"%s caused %s (0x%08X) \r\nin module %s at 0x%08Xx:%08X.\r\n\r\n", 
			CT2A( module_path.Path() ), 
			exception_description.c_str(), 
			Exception->ExceptionCode, 
			CT2A( crash_module_path.Path() ),
			Context->SegCs ,
			Context->Eip ) ;

		ASSERT_HRESULT( hr ) ;

		log_file.write( crash_buffer, ONEK - remaining ) ;
		

		log_file << "Exception handler called for error " << lpszMessage << ".\r\n" ;

		DumpSystemInformation(log_file);

		// If the exception was an access violation, print out some additional
		// information, to the error log and the debugger.
		if (Exception->ExceptionCode == STATUS_ACCESS_VIOLATION &&
					Exception->NumberParameters >= 2)
		{
			hr = StringCbPrintfExA( crash_buffer, ONEK, &dest_end, &remaining, 
				flags,
				"%s location 0x%08X caused an access violation.\r\n", 
				Exception->ExceptionInformation[0] ? "Write to" : "Read from",
				Exception->ExceptionInformation[1] ) ;
			ASSERT_HRESULT( hr ) ;

			log_file.write( crash_buffer, ONEK - remaining ) ;

	#ifdef	_DEBUG
			// The Visual C++ debugger doesn't actually tell you whether a read
			// or a write caused the access violation, nor does it tell what
			// address was being read or written. So I fixed that.
			OutputDebugString(_T("Exception handler: "));
			OutputDebugStringA(crash_buffer);
	#endif

		}

		DumpRegisters(log_file, Context);

		// Print out the bytes of code at the instruction pointer. Since the
		// crash may have been caused by an instruction pointer that was bad,
		// this code needs to be wrapped in an exception handler, in case there
		// is no memory to read. If the dereferencing of code[] fails, the
		// exception handler will print '??'.
		log_file.write(  "\r\nBytes at CS:EIP:\r\n" );
		BYTE * code = (BYTE *)( Context->Eip ) ;
		for (int codebyte = 0; codebyte < NumCodeBytes; codebyte++)
		{
			hr = StringCbPrintfExA( crash_buffer, ONEK, &dest_end, &remaining, 
				flags,
				"%02X ", 
				code[codebyte] ) ;
			ASSERT_HRESULT( hr ) ;

			log_file.write( crash_buffer, ONEK - remaining ) ;
		}
		log_file << "\r\n\r\n" ;

		// Time to print part or all of the stack to the error log. This allows
		// us to figure out the call stack, parameters, local variables, etc.

		// Esp contains the bottom of the stack, or at least the bottom of
		// the currently used area
	//		DWORD* pStack = (DWORD *)Context->Esp;

	//		DumpStack(log_file, pStack);

		DumpModuleList(log_file);

		log_file << "\r\n===== [end of " 
			<< string2string( XCRASHREPORT_ERROR_LOG_FILE ) 
			<< "] =====\r\n\r\n" ;

		// allow the user to send the crash report...
		log_file.write_eof() ; // cut off the file here...
		log_file.close() ;
	}
	catch( ... )
	{
		return false ;
	}
	first_time = true ;
	return true ;
}

#pragma warning(default: 4312) // conversion to smaller size
