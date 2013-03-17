/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#import "D:\dev\cpp\Common DLLs\Word 2000 OLBs\MSO.DLL"	\
	rename_namespace("MSOffice") rename("RGB", "OfficeRGB") rename( "SearchPath", "OfficeSearchPath" ) rename("DocumentProperties", "OfficeDocumentProperties") named_guids    

#import "D:\Program Files (x86)\Common Files\microsoft shared\VBA\VBA6\\VBE6EXT.olb" \
	rename_namespace("VBE6") 

#import "D:\dev\cpp\Common DLLs\Word 2000 OLBs\EXCEL9.olb" \
	exclude( "IFont" ) exclude( "IPicture" )\
	rename("DialogBox", "DialogBoxForExcel") rename( "RGB", "RGBForExcel") \
	rename( "CopyFile", "CopyFIleForExcel" ) rename( "ReplaceText", "ReplaceTextForExcel" ) \

static const GUID DIID__CommandBarButtonEvents = {0x000C0351,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};


