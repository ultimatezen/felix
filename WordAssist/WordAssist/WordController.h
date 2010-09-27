// WordController.h: interface for the WordController class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "resource.h"	// for our context menu...
#include "app_state.h"
#include "WordParser.h"
#include "word_writer.h"

#include "app_state.h"
#include <map>
#include "Word2HtmlParser.h"
#include "logging.h"
#import "C:\dev\cpp\Assistant Suite\Felix\Unicode_Release\Felix.tlb"\
	rename("TranslateAccelerator", "FelixTranslateAccelerator")

class WordController 
{

public:
	bool		m_is_auto ;
	typedef CComObject< CWord2HtmlParser > Word2HtmlObject ;
	Word2HtmlObject *m_word2html ;

	CDispatchWrapper *m_trans_history ;
	wstring			m_history_name ;
	LPDISPATCH		m_app ;

	typedef std::map<wstring, long> map_type ;

	map_type			m_query_starts ;
	long				m_query_start ;

	bool				m_is_trans_mode ;
	app_state			m_properties ;
	Abbreviations		m_abbreviations ;
	bool				m_isAndSetAction ;


	WordApplication			m_word_object ;

public:
	WordController(LPCWSTR history_server = L"FelixTransHistory.Word");

	void load_abbreviations() 
	{
		m_abbreviations.load(get_config_text(_T("abbreviations.txt"))) ;
	}
	virtual ~WordController();

	long get_query_start();

	wstring get_active_doc_name();
	void set_query_start(long query_start);
	void SetQueryStart( long start );
	bool OnTrans2MemAction( bool as_plaintext );
	bool OnMem2TransAction( bool as_plaintext );
	bool OnToMaruAction( bool as_plaintext );
	bool is_translation_mode();
	void OnSwitchModeAction();
	bool refresh_properties( boost::any &Param ) ;
	virtual bool set_properties( const app_state params )
	{
		m_properties = params ;
		return true ;
	}

	void ensure_trans_history();
	void set_status_bar_text( const int string_id );
	bool new_window_has_focus();
	bool extend_lookup( );
	bool IsActive();

	virtual void write_translation(const wstring& text, bool as_plaintext );
	virtual _bstr_t get_selection_text( bool as_plaintext ) ;

	void OnWindowActivate();
	void OnWindowDeactivate();
	void write_gloss_entry( int index, bool as_plaintext );
	bool select_next_sentence();
	virtual void set_app( LPDISPATCH Application );

	bool OnAutoTransAction ( bool as_plaintext ) ;
	bool OnAutoTransFuzzyAction ( bool as_plaintext ) ;
	
	// get and set translations
	bool OnGetAction( bool as_plaintext ) ;
	bool OnSetAction( bool as_plaintext ) ;
	bool OnGetAndNextAction( bool as_plaintext ) ;
	bool OnSetAndNextAction( bool as_plaintext ) ;
	
	// look up source segment
	bool OnLookupAction( bool as_plaintext ) ;
	bool OnLookupNextAction( bool as_plaintext ) ;
	bool OnConcordanceAction ( bool as_plaintext ) ;
	bool OnExtendLookupAction ( bool as_plaintext ) ;
	
	// register a glossary entry
	bool OnRegisterGlossAction ( bool as_plaintext )	;
	// delete entry
	bool OnDeleteAction ( ) ;
	// navigate through matches
	bool OnNextAction ( ) ;
	bool OnPrevAction ( ) ;
	// get glossary entries
	bool OnEntry0Action ( bool as_plaintext ) ;
	bool OnEntry1Action ( bool as_plaintext ) ;
	bool OnEntry2Action ( bool as_plaintext ) ;
	bool OnEntry3Action ( bool as_plaintext ) ;
	bool OnEntry4Action ( bool as_plaintext ) ;
	bool OnEntry5Action ( bool as_plaintext ) ;
	bool OnEntry6Action ( bool as_plaintext ) ;
	bool OnEntry7Action ( bool as_plaintext ) ;
	bool OnEntry8Action ( bool as_plaintext ) ;
	bool OnEntry9Action ( bool as_plaintext ) ;
	bool OnGlossNAction( bool as_plaintext ) ;
	
	// look up translation
	bool OnLookupTransAction ( bool as_plaintext ) ;
	bool OnLookupNextTransAction ( bool as_plaintext ) ;
	bool OnTransConcordanceAction ( bool as_plaintext ) ;
	bool OnExtendTransLookupAction ( bool as_plaintext ) ;
	bool OnCorrectAction ( bool as_plaintext ) ;
	bool OnCorrectAndNextAction ( bool as_plaintext ) ;
	
	bool OnRestoreAction ( bool as_plaintext ) ;
	bool OnRestoreAndNextAction ( bool as_plaintext ) ;
	
	bool OnShowContextMenu() ;
	bool OnShowContextMenuCtl() ;

	bool OnHelpAction() ;

	CString get_help_file_name();
	bool OnSaveMemoryAction();
	bool OnShiftStateAction( );
	// Tells our controller that a new document has opened
	void OnDocumentOpenAction(LPDISPATCH doc);
	void set_parser(LPDISPATCH parser);
private:
	bool handle_exception(std::exception &e, CString msg );
	bool handle_exception(except::CComException &com_exception, CString msg );
	bool handle_exception(except::CWinException &e, CString msg );
	bool handle_exception(except::CSWException &e, CString msg );
	bool handle_exception(_com_error &e, CString msg );
	bool handle_exception(except::CException &e, CString msg);


	void configure_parser_font_settings(boost::shared_ptr<WordParser> parser);

	void configure_writer_font_settings(word_writer& writer);

private:
	Felix::IAppPtr getAssistant(void);

public:

	bool has_j( const wstring &text );

	bool is_ignorable(const wstring& text, bool skip_numbers, int skip_j );
	void dispose_trans_history();

	virtual void close_doc( IDispatch* doc );


};

