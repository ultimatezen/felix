#include "StdAfx.h"
#include "AppTooltips.h"

typedef boost::wformat wformat ;

CAppTooltips::CAppTooltips(void)
{
	CBroadcaster &broadcaster = CBroadcaster::instance() ;
	broadcaster.Subscribe( "App", "Quit", boost::bind( &CAppTooltips::OnAppQuit, this, _1 ) ) ;
	
	m_TipManager.ShowDelay = 250;	// Display tooltips after 1/4 second
	m_TipManager.HideDelay = 6000;	// Hide tooltips automatically after 6 seconds

	m_TipManager.tool.HasBorder = false ;

	vxlib::CVXPSettings settings ;
	settings.bgColor = RGB( 255, 255, 255 ) ;

	vxlib::CVXPFont font ;
	font.Name = _T( "Times New Roman" ) ;

	//std::wstring fmt_str = L""
	//	L"<img res='ico:idr_mainframe' width=48 height=48>"
	//	L"%s<hr>%s" ;

	std::wstring FmtStr = L"<body font='tool' BGCOLOR=White GRAD=VERT GFC=#DFEFFF lm=10 rm=10 bm=5>"
		L"<table><tr><td><img src='info.ico' width=16 height=16></td>"
		L"<td valign=bottom>&nbsp;<b>%s</b></td></tr></table>"
		L"<hr size=1 color=#607080>"
		L"%s<br><br>"
		L"<font color=#404040>%s</font><br><br>"
		L"<font color=#404040>%s</font>" ;

	std::wstring NoShortcutFmtStr = L"<body font='tool' BGCOLOR=White GRAD=VERT GFC=#DFEFFF lm=10 rm=10 bm=5>"
		L"<table><tr><td><img src='info.ico' width=16 height=16></td>"
		L"<td valign=bottom>&nbsp;<b>%s</b></td></tr></table>"
		L"<hr size=1 color=#607080>"
		L"<font color=#404040>%s</font><br><br>"
		L"<font color=#404040>%s</font>" ;

	// standard toolbar
	m_tips[ID_FILE_NEW] =   
		( wformat( FmtStr ) 
			% L"New File" 
			% L"Ctrl + N" 
			% L"Create a new document." 
			% L"The new document will be of type HTML by default.<br>"
			  L"To create a different type of file, select <br>"
			  L"<b>File<b> &gt;&gt; <b>New</b> &gt;&gt; <b>File Type...</b>" ).str() ;
	m_tips[ID_FILE_OPEN] = 
		( wformat( FmtStr ) 
			% L"Open File" 
			% L"Ctrl + O" 
			% L"Open an existing file." 
			% L"Recently opened files can be found under<br>"
			  L"<b>File<b> &gt;&gt; <b>Recent Files</b>" ).str() ;
	m_tips[ID_FILE_SAVE] = 
		( wformat( FmtStr ) % L"Save File" 
			% L"Ctrl + S" 
			% L"Save the current file." 
			% L"If you have not saved the file before, a dialog will <br> "
			  L"prompt you to specify a name." ).str() ;
	m_tips[ID_FILE_SAVEALL] = 
		( wformat( NoShortcutFmtStr ) % L"Save All" 
			% L"Save all open files." 
			% L"This is useful after you have modified many files,<br> "
			  L"for example after search and replace across files." ).str() ;
	m_tips[ID_EDIT_CUT] = 
		( wformat( FmtStr ) % L"Cut" 
			% L"Ctrl + X" 
			% L"Delete the current selection and place it on the clipboard." 
			% L" " ).str() ;
	m_tips[ID_EDIT_COPY] = 
		( wformat( FmtStr ) % L"Copy" 
			% L"Ctrl + C" 
			% L"Copy the current selection onto the clipboard." 
			% L" " ).str() ;
	m_tips[ID_EDIT_PASTE] = 
		( wformat( FmtStr ) % L"Paste" 
			% L"Ctrl + V" 
			% L"Paste the current clipboard contents at the cursor position." 
			% L" " ).str() ;
	m_tips[ID_EDIT_DELETE] = 
		( wformat( FmtStr ) % L"Delete" 
			% L"Del" 
			% L"Delete the current selection." 
			% L" " ).str() ;
	m_tips[ID_EDIT_UNDO] = 
		( wformat( FmtStr ) % L"Undo" 
			% L"Ctrl + Z" 
			% L"Undo the last action." 
			% L"You can undo more than one action. <br>"
			  L"Just keep pressing Ctrl + Z." ).str() ;
	m_tips[ID_EDIT_REDO] = 
		( wformat( FmtStr ) % L"Redo" 
			% L"Ctrl + Y" 
			% L"Redo the last undone action." 
			% L"You can redo more than one undone action. <br>"
			  L"Just keep pressing Ctrl + Y." ).str() ;
	m_tips[ID_EDIT_FIND] = 
		( wformat( FmtStr ) % L"Find" 
			% L"Ctrl + F" 
			% L"Find text in the current document." 
			% L"You can also search for text in multiple files. Use <br>"
			  L"<b>Edit<b> &gt;&gt; <b>Find in Files...</b>" ).str() ;

	m_tips[ID_EDIT_INDENT] = 
		( wformat( FmtStr ) % L"Indent" 
		% L"Alt + Tab" 
		% L"Indent the current text." 
		% L"The current text is indented one level.").str() ;

	m_tips[ID_EDIT_OUTDENT] = 
		( wformat( FmtStr ) % L"Outdent" 
		% L"Alt + Shift + Tab" 
		% L"Outdent the current text." 
		% L"The indenting of the current text is reduced"
		  L"by one level.").str() ;


	m_tips[ID_EDIT_FIND_NEXT] = 
		( wformat( FmtStr ) % L"Find Again" 
			% L"F3" 
			% L"Repeat the last search." 
			% L"Use this to quickly find and edit multiple <br> "
			L"instances of a search string." ).str() ;
	m_tips[ID_FILE_PRINT] = 
		( wformat( FmtStr ) % L"Print" 
			% L"Ctrl + P" 
			% L"Print the current document." 
			% L"The type of document that is printed will depend <br> "
			L"on the current document view." ).str() ;
	m_tips[ID_APP_ABOUT] = 
		( wformat( NoShortcutFmtStr ) % L"About" 
			% L"View version and copyright information." 
			% L" " ).str() ;

	// Felix toolbar
	m_tips[ID_TA_LOOKUP] = 
		( wformat( FmtStr ) % L"Look Up" 
			% L"Alt + L" 
			% L"<table><tr><td valign=bottom>Look up the current selection in Felix.</td>"
			  L"<td><img src='Felix.ico' width=16 height=16></td></tr></table>" 
			% L"This operation is useful when you want to look up a segment that<br>"
			  L"Is not recognized as a sentence." ).str() ;
	m_tips[ID_TA_LOOKUP_NEXT] = 
		( wformat( FmtStr ) % L"Look Up Next" 
			% L"Alt + R ARR" 
			% L"<table><tr><td valign=bottom>Look up the next selection in Felix.</td>"
			L"<td><img src='Felix.ico' width=16 height=16></td></tr></table>" 
			% L"TagAssist will automatically select the next sentence, and look <br>"
			  L"it up in Felix." ).str() ;
	m_tips[ID_TA_SET] = 
		( wformat( FmtStr ) % L"Set" 
			% L"Alt + UP ARR" 
			% L"<table><tr><td valign=bottom>Set the current translation in Felix.</td>"
			L"<td><img src='Felix.ico' width=16 height=16></td></tr></table>" 
			% L"If no text is selected, the text from the cursor to the beginning<br>"
			  L"of the lookup segment is automatically selected and registered.<br>"
			  L"If there is a selection, that is registered." ).str() ;
	m_tips[ID_TA_SET_AND_NEXT] = 
		( wformat( FmtStr ) % L"Set And Next" 
			% L"Alt + S" 
			
			% L"<table><tr><td valign=bottom>Set the current translation in Felix, and <br> "
			L"look up the next sentence</td>"
			L"<td><img src='Felix.ico' width=16 height=16></td></tr></table>" 

			% L"If no text is selected, the text from the cursor to the beginning<br>"
			L"of the lookup segment is automatically selected and registered.<br>"
			L"If there is a selection, that is registered. The lookup then proceeds<br>"
			L"from the end of the current selection." ).str() ;
	m_tips[ID_TA_GET] = 
		( wformat( FmtStr ) % L"Get" 
			% L"Alt + DN ARR" 
			% L"<table><tr><td valign=bottom>Get the translation for the current query from Felix.</td>"
			L"<td><img src='Felix.ico' width=16 height=16></td></tr></table>" 
			% L"If text is selected, it is replaced by the translation text." ).str() ;
	m_tips[ID_TA_GET_AND_NEXT] = 
		( wformat( FmtStr ) % L"Get And Next" 
			% L"Alt + G" 
			
			% L"<table><tr><td valign=bottom>Get the current translation from Felix, and <br> "
			L"look up the next sentence</td>"
			L"<td><img src='Felix.ico' width=16 height=16></td></tr></table>" 

			% L"If text is selected, it is replaced by the translation text.<br>"
			L"The lookup then proceeds from the end of the current selection." ).str() ;
	m_tips[ID_TA_GLOSS] = 
		( wformat( NoShortcutFmtStr ) % L"Glossary" 
			% L"<table><tr><td valign=bottom>Get a translation from the glossary matches.</td>"
			L"<td><img src='Felix.ico' width=16 height=16></td></tr></table>" 
			% L"You can also use the autocomplete feature to suggest glossary<br>"
			  L"entries as you type." ).str() ;
	m_tips[ID_TA_SAVE] = 
		( wformat( NoShortcutFmtStr ) % L"Save Memory" 
			% L"<table><tr><td valign=bottom>Save the active memory in Felix.</td>"
			L"<td><img src='Felix.ico' width=16 height=16></td></tr></table>" 
			% L"It is recommended that you save your memory frequently, in order<br>"
			  L"to avoid loss of data in case of a computer crash." ).str() ;
	m_tips[ID_TA_SWITCH] = 
		( wformat( NoShortcutFmtStr ) % L"Switch" 
			% L"<table><tr><td valign=bottom>Switch between translation and revision modes.</td>"
			L"<td><img src='Felix.ico' width=16 height=16></td></tr></table>" 
			% L"Translation mode is the normal mode for translating documents.<br>"
			  L"Use revision mode to review your translations. Looking up a translation<br>"
			  L"shows its source in Felix." ).str() ;

	// format toolbar
	m_tips[ID_FORMAT_BOLD] = 
		( wformat( FmtStr ) % L"Bold" 
			% L"Ctrl + B" 
			% L"Make the currently selected text bold." 
			% L"If the text is already bold, this command will <br> "
			L"remove the bold formatting." ).str() ;
	m_tips[ID_FORMAT_ITALIC] = 
		( wformat( FmtStr ) % L"Italic" 
			% L"Ctrl + I" 
			% L"Make the currently selected text italic." 
			% L"If the text is already italic, this command will <br> "
			L"remove the italic formatting." ).str() ;
	m_tips[ID_FORMAT_UNDERLINE] = 
		( wformat( FmtStr ) % L"Underline" 
			% L"Ctrl + U" 
			% L"Underline the currently selected text." 
			% L"If the text is already underlined, this command <br> "
			L"will remove the underline." ).str() ;
	m_tips[IDB_BITMAP_BLANK] = 
		( wformat( FmtStr ) % L"Blank" 
			% L"" 
			% L" " 
			% L" " ).str() ;
	m_tips[ID_FORMAT_FONT]	= 
		( wformat( NoShortcutFmtStr ) % L"Font" 
			% L"Set the font for the currently selected text." 
			% L"Use this command to set the font face, text color, <br> "
			L"and other attributes all at once." ).str() ;
	m_tips[ID_FORMAT_FONT_COLOR] = 
		( wformat( NoShortcutFmtStr ) % L"Font Color" 
			% L"Set the color for the currently selected text." 
			% L"Sets the text color." ).str() ;
	m_tips[ID_FORMAT_HIGHLIGHT]	= 
		( wformat( NoShortcutFmtStr ) % L"Highlight" 
			% L"Highlight the currently selected text." 
			% L"This is different from background color (<b>Format</b> &gt;&gt; <b>Background</b>),<br>"
			  L"which sets the color for the entire document." ).str() ;
	m_tips[ID_FORMAT_LIST_BULLETS] = 
		( wformat( NoShortcutFmtStr ) % L"Bullet List" 
			% L"Create a bullet list at the current cursor location." 
			% L" " ).str() ;
	m_tips[ID_FORMAT_LIST_NUMBERED]	= 
		( wformat( NoShortcutFmtStr ) % L"Numbered List" 
			% L"Create a numbered list at the current cursor location." 
			% L" " ).str() ;


	// align stuff
	m_tips[ID_ALIGN_JUSTIFY] = 
		( wformat( NoShortcutFmtStr ) % L"Justify" 
		% L"Justify the current text." 
		% L"The text will have a uniform left and right margin." ).str() ;
	m_tips[ID_ALIGN_LEFT] = 
		( wformat( FmtStr ) % L"Align Left" 
		% L"Ctrl + L" 
		% L"Align the current text to the left." 
		% L"" ).str() ;
	m_tips[ID_ALIGN_CENTER] = 
		( wformat( FmtStr ) % L"Align Center" 
		% L"Ctrl + E" 
		% L"Center the current text." 
		% L"Useful for titles, captions, etc." ).str() ;
	m_tips[ID_ALIGN_RIGHT] = 
		( wformat( FmtStr ) % L"Align Right" 
		% L"Ctrl + R" 
		% L"Align the current text to the right." 
		% L"" ).str() ;
}

CAppTooltips::~CAppTooltips(void)
{
}
