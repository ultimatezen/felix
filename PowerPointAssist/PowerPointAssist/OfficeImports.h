#pragma once

#pragma warning( disable:4146 )	// unary minus operator applied to unsigned type, result still unsigned
#import "C:\Development\C++\Common DLLs\Word 2000 OLBs\mso.dll" \
	rename_namespace("Office") rename("RGB", "OfficeRGB") \
	rename( "DocumentProperties", "OfficeDocumentProperties" ) rename( "SearchPath", "OfficeSearchPath" ) // , named_guids  
#pragma warning( default:4146 )

#import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.olb" \
	rename_namespace("VBE6")  

#import "C:\Development\C++\Common DLLs\Word 2000 OLBs\MSPPT9.OLB" \
	rename_namespace("PowerPoint"), rename( "RGB", "PowerPointRGB"), raw_interfaces_only, named_guids  

#import "C:\Development\C++\Common DLLs\Word 2000 OLBs\EXCEL9.OLB" \
	rename_namespace("Excel") \
	rename("Range","ExcelRange") \
	rename("DialogBox","ExcelDialogBox") \
	rename("RGB","ExcelRGB") \
	exclude("IFont","IPicture") \
	rename ("CopyFile", "ExcelCopyFile" ) \
	rename ("ReplaceText", "ExcelReplaceText" ) 
