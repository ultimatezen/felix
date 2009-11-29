#pragma once

#include "TranslationMemory.h"	// for the trans_record struct
#include "StringEx.h"			// for string utilities
#include "resource.h"			// for built-in strings
#include "query.h"				// for href builder

/**
	@class record_presenter  
	@brief Formats memory records for presentation to user.
 */
class record_presenter  
	: public html_formatter
{
	record_pointer  m_record ;

public:
	void set_record ( record_pointer record = record_pointer(new record_local()) ) { m_record = record ; }

	// Function name	: translation_table
	// Description	    : 
	// Return type		: wstring 
	wstring translation_table()
	{
		BANNER( "translation_table" ) ;

		return 
			// header
			table_header() 
			// added translation + source + trans
			+ added_translation() + source() +  trans() 
			// footer
			+ table_footer() ;
	}

	// Function name	: navigation_table
	// Description	    : 
	// Return type		: wstring 
	wstring navigation_table()
	{
		BANNER( "navigation_table" ) ;

		// navigation
		return table_header( ) + navbar_edit_delete() + table_footer() ;
	}

	// Function name	: source
	// Description	    : 
	// Return type		: wstring 
	wstring source()
	{
		BANNER( "source" ) ;

		wstring source_title = resource2wstring( IDS_SOURCE_TITLE, RES_INST ) ;
		wstring source_id = resource2wstring( IDS_SOURCE_ID, RES_INST ) ;
		wstring title_id = resource2wstring( IDS_SET_SOURCE_TITLE, RES_INST ) ;

		wstring output ;
		output	   << L"<tr id=\"0\"><td bgcolor=\"#00CCFF\" title=\"" 
					<< title_id 
					<< L"\"><b>" 
				   << source_title 
				   << L"</b></td><td id=\"" << source_id << L"\">" 
				   << m_record->get_source_rich() ;
		return output ;
	}

	// Function name	: trans
	// Description	    : 
	// Return type		: wstring 
	wstring trans()
	{
		BANNER( "trans" ) ;

		wstring trans_title = resource2wstring( IDS_TRANS_TITLE, RES_INST ) ;
		wstring trans_id = resource2wstring( IDS_TRANS_ID, RES_INST ) ;
		wstring title_id = resource2wstring( IDS_SET_TRANS_TITLE, RES_INST ) ;
		
		wstring output ;
		output	   << L"<tr id=\"0\"><td bgcolor=\"#00CCFF\" title=\"" 
			<< title_id 
			<< L"\"><b>" 
			<< trans_title 
			<< L"</b></td><td id=\"" << trans_id << L"\">" 
			<< m_record->get_trans_rich() ;

		return output ;
	}

	// Function name	: table_header
	// Description	    : 
	// Return type		: wstring 
	// Argument         : int border=1
	// Argument         : int width=100
	wstring table_header( )
	{
		BANNER( "table_header" ) ;

		wstring content ;
		content << L"<table class=\"record\">" ;
		return content ;
	}

	// Function name	: added_translation
	// Description	    : 
	// Return type		: wstring 
	wstring added_translation()
	{
		BANNER( "added_translation" ) ;

		wstring output ;
		output << L"<tr><td colspan=\"2\" bgcolor=\"#00CCFF\"><center><b>" << R2W( IDS_ADDED_TRANSLATION ) << L"</b></center></td></tr>" ;
		return output ;
	}

	// Function name	: table_footer
	// Description	    : 
	// Return type		: wstring 
	wstring table_footer()
	{
		BANNER( "table_footer" ) ;

		return wstring( L"</table>" ) ;
	}

	// Function name	: navbar_edit_delete
	// Description	    : 
	// Return type		: wstring 
	wstring navbar_edit_delete()
	{
		BANNER( "navbar_edit_delete" ) ;

		ATLASSERT( IDC_EDIT != IDC_DELETE ) ;
		wstring w ;
		w << L"<tr><td>" ;
		w << make_href( IDC_EDIT, 0, R2W( IDS_EDIT ), IDS_EDIT_TITLE ) ;
		w << L" | " ;
		w << make_href( IDC_DELETE, 0, R2W( IDS_DELETE ), IDS_DELETE_TITLE ) ;
		w << L"</td></tr>" ;

		return w ;
	}

	// Function name	: navbar_reg_add
	// Description	    : 
	// Return type		: wstring 
	wstring navbar_reg_add()
	{
		BANNER( "navbar_reg_add" ) ;

		ATLASSERT( IDC_REGISTER != IDC_ADD ) ;

		wstring w ;
		w << L"<p><hr>" 
		  << make_href( IDC_REGISTER,		 0, R2W( IDS_REGISTER ), IDS_REGISTER_TITLE     )
		  << L" | "
		  << make_href( IDC_ADD_TO_GLOSSARY, 0, R2W( IDS_ADD_TO_GLOSS ), IDS_ADD2GLOSS_TITLE )  ;
	
		return w ;
	}

};

