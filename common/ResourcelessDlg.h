//********************************************************************
//	filename:	ResourcelessDlg.h
//	version:	1.0
//	author:		Trilobyte-Solutions
//	created:	11/08/2004
//
//	purpose:	Create a dialog without the need for an resource
//				script. Useful when you want to export a dialog.
//				More detailed information can be found in the 
//				ResourcelessDlg.htm file which comes together with
//				this file.
//
//	contact information:
//				e-mail:		Contact@Trilobyte-Solutions.nl
//				Website:	http://www.Trilobyte-Solutions.nl
//				Support:	http://forum.Trilobyte-Solutions.nl
//*********************************************************************
#ifndef _RESOURCELESSDLG_TRILOBYTE_SOLUTIONS_H
#define _RESOURCELESSDLG_TRILOBYTE_SOLUTIONS_H
#ifndef __cplusplus
#error ATL requires C++ compilation (use a .cpp suffix)
#endif
#ifndef __ATLAPP_H__
#error DesourcelessDlg.h requires atlapp.h to be included first
#endif
#ifndef __ATLWIN_H__
#error DesourcelessDlg.h requires atlwin.h to be included first
#endif
namespace ATL
{
	typedef struct {  
		WORD   dlgVer; 
		WORD   signature; 
		DWORD  helpID; 
		DWORD  exStyle; 
		DWORD  style; 
		WORD   cDlgItems; 
		short  x; 
		short  y; 
		short  cx; 
		short  cy; 
		WORD menu;         // name or ordinal of a menu resource
		WORD windowClass;  // name or ordinal of a window class
		//		WCHAR  title[_MAX_PATH]; // title string of the dialog box
	} DLGTEMPLATEEX; 

	typedef struct { 
		DWORD  helpID; 
		DWORD  exStyle; 
		DWORD  style; 
		short  x; 
		short  y; 
		short  cx; 
		short  cy; 
		WORD   id; 
		WORD windowClass[2]; // name or ordinal of a window class
		//		WCHAR  title[_MAX_PATH]; // title string of the dialog item
	} DLGITEMTEMPLATEEX; 

	template<class T>
	class ATL_NO_VTABLE CResourcelessDlg: public CDialogImplBase
	{
	public:
		// Message map for IDOK & IDCANCEL
		BEGIN_MSG_MAP(CResourcelessDlg)
			COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
			COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
			COMMAND_ID_HANDLER(IDCLOSE, OnCloseCmd)
		END_MSG_MAP()

		T* init_vals( bool is_modal = false )
		{
			m_bModal	= is_modal;
			m_size		= 0;
			m_pTemplate	= NULL;
			m_offset	= 0;

			return		static_cast<T*>(this) ;
		}

		void create_template_buffer()
		{
			// Create the template buffer
			m_byte_buffer.resize( m_size ) ;
			m_pTemplate = (BYTE*)m_byte_buffer.begin() ;
			ZeroMemory( m_pTemplate, m_size) ;
		}

		virtual BOOL DestroyWindow()
		{
			ATLASSERT(::IsWindow(m_hWnd));
			ATLASSERT(!m_bModal);
			return ::DestroyWindow(m_hWnd);
		}

		bool size_dlg_and_create_template(T* pT)
		{
			ATLASSERT( m_offset == 0 ) ;
			ATLASSERT( m_pTemplate == NULL ) ;

			// Get the size of the template
			pT->CreateDlg();
			m_size = m_offset ;
			
			create_template_buffer() ;
			
			ATLASSERT( IsValidReadPointer( m_pTemplate, m_size ) ) ;

			return ( m_pTemplate != NULL ) ;
		}
		void create_dlg( T* pT )
		{
			// Add window data
			_Module.AddCreateWndData(&m_thunk.cd, (CDialogImplBase*)this);
			
			m_offset = 0 ;
			pT->CreateDlg();

			// Set the number of controls, windows does not support more than 255 controls, create them in OnInitDialog()
			ATLASSERT(m_uiNumCtrls < 255);
			((DLGTEMPLATEEX*)m_pTemplate)->cDlgItems = m_uiNumCtrls;
		}
		// Dailog creation functions
		HWND Create(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL)
		{
			T* pT = init_vals( false ) ;

			bool success = size_dlg_and_create_template( pT ) ;
			ATLASSERT( success ) ;
			if ( ! success )
			{
				return NULL ;
			}

			create_dlg( pT ) ;

			// Display the dialog
			HWND retVal = 0;
			retVal = ::CreateDialogIndirectParam(GetModuleHandle(0), (DLGTEMPLATEEX*)m_pTemplate, hWndParent, (DLGPROC)T::StartDialogProc, dwInitParam);

			ATLASSERT(m_hWnd == retVal);

			return retVal;
		}
		int DoModal(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL)
		{
			T* pT = init_vals( true ) ;

			bool success = size_dlg_and_create_template( pT ) ;
			ATLASSERT( success ) ;
			if ( ! success )
			{
				return -1 ;
			}

			create_dlg( pT ) ;

			int retVal = 0;
			retVal = ::DialogBoxIndirectParam(GetModuleHandle(0), (DLGTEMPLATE *)m_pTemplate, hWndParent, (DLGPROC)T::StartDialogProc, dwInitParam);
			
			ASSERT_WITH_WINERR( retVal != 0 ) ;
			ASSERT_WITH_WINERR( retVal != -1 ) ;

			return retVal;
		}
		// Accessor functions
		bool IsModal()			const	{return m_bModal;}
		UINT NumberOfControls()	const	{return m_uiNumCtrls;}
	protected:
		// Misc functions
		void EndDialog(INT_PTR iResult)
		{
			ATLASSERT(::IsWindow(m_hWnd));
			if (IsModal())
				::EndDialog(m_hWnd, iResult);
			else
				DestroyWindow();
		}
		virtual	LRESULT	OnCloseCmd(UINT, int iId, HWND, BOOL&)
		{
			EndDialog(iId);
			return 0;
		}
		// Template create functions
		void CreateDlgTemplate(
			ATL::_U_STRINGorID pszTitle, 
			DWORD dwStyle, 
			DWORD dwExStyle, 
			short x, short y, 
			short cx, short cy
			)
		{
			ATLASSERT( m_offset == 0 );
			ATLASSERT( m_offset <= m_size ) ;

			DLGTEMPLATEEX* pDlgTemplate	= (DLGTEMPLATEEX *)(m_pTemplate + m_offset);
			m_offset						+= sizeof(*pDlgTemplate);
			WORD* pWrite				= NULL;

			// Set the number of controls to zero
			m_uiNumCtrls = 0;
			
			// Set the DLGTEMPLATEEX data
			if (pDlgTemplate)
			{
				pDlgTemplate->dlgVer			= 1 ;
				pDlgTemplate->signature			= 0xFFFF ;

				pDlgTemplate->helpID			= 0 ;

				pDlgTemplate->exStyle			= dwExStyle;
				pDlgTemplate->style				= dwStyle;

				pDlgTemplate->cDlgItems			= 0 ;

				pDlgTemplate->x					= x;
				pDlgTemplate->y					= y;
				pDlgTemplate->cx				= cx;
				pDlgTemplate->cy				= cy;

				pDlgTemplate->menu				= 0 ;

				pDlgTemplate->windowClass		= 0 ;
				
				// Set the pointer
				pWrite = (WORD*)(pDlgTemplate + 1);

			}

			CComBSTR res = res2bstr( pszTitle.m_lpstr ) ;
			m_offset += WriteString(pWrite, res, true );

			// Align the pointer to DWORD
			int tmpsize = m_offset;
			m_offset = (tmpsize + 3) & (~3);
		}
		CComBSTR res2bstr( LPCTSTR res )
		{
			CComBSTR retval ;
			if ( IS_INTRESOURCE( res ) )
			{
				retval.LoadString( (UINT)res ) ;
			}
			else
			{
				USES_CONVERSION ;
				retval = T2CW( res ) ;
			}

			return retval ;
		}
		void AddDlgItem(
			ATL::_U_STRINGorID pszTitle, 
			DWORD dwStyle, 
			DWORD dwExStyle, 
			short x, short y, 
			short cx, short cy, 
			short id, 
			unsigned short window_class )
		{
			ATLASSERT( m_offset >= sizeof(DLGTEMPLATEEX) ) ;
			
			DLGITEMTEMPLATEEX* pDlgItemTemplate = NULL ;
			if ( m_pTemplate )
			{
				pDlgItemTemplate = (DLGITEMTEMPLATEEX*)(m_pTemplate + m_offset);
			}

			m_offset		+= sizeof(*pDlgItemTemplate);
			WORD* pWrite	= NULL;

			if (pDlgItemTemplate)
			{
				ATLASSERT( m_offset < m_size ) ;
				ATLASSERT( m_offset >= sizeof( DLGTEMPLATEEX ) ) ;
				
				pDlgItemTemplate->helpID			= 0 ;
				pDlgItemTemplate->exStyle			= dwExStyle;
				pDlgItemTemplate->style				= dwStyle|WS_VISIBLE|WS_CHILD;
				pDlgItemTemplate->x					= x;
				pDlgItemTemplate->y					= y;
				pDlgItemTemplate->cx				= cx;
				pDlgItemTemplate->cy				= cy;
				pDlgItemTemplate->id				= id;

				pDlgItemTemplate->windowClass[0]	= 0xFFFF ;
				ATLASSERT( 
					window_class == 0x0080 ||	// Button 
					window_class == 0x0081 ||	// Edit 
					window_class == 0x0082 ||	// Static 
					window_class == 0x0083 ||	// List box 
					window_class == 0x0084 ||	// Scroll bar 
					window_class == 0x0085		// Combo box 
				) ;

				pDlgItemTemplate->windowClass[1]	= window_class ;

				pWrite = (WORD*)(pDlgItemTemplate + 1);

			}

			// Set the title
			CComBSTR title_res = res2bstr( pszTitle.m_lpstr ) ;
			m_offset += WriteString(pWrite, title_res, true ) ;

			// extra count
			m_offset += 2 ;
			if ( pWrite )
			{
				*pWrite = 0 ;
				++pWrite ;
			}

			// Align the pointer to DWORD
			int tmpsize = m_offset;
			m_offset = (tmpsize + 3) & (~3);

			// Increase the number of controls
			m_uiNumCtrls++;
		}
		void AddButton(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
		{
			AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_BUTTON );
		}
		void AddEditBox(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
		{
			AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_EDIT );
		}
		void AddStatic(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
		{
			AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_STATIC );
		}
		void AddListBox(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
		{
			AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_LIST );
		}
		void AddScrollBar(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
		{
			AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_SCROLLBAR );
		}
		void AddCombo(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
		{
			AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_COMBO );
		}
	private:
		int WriteString(WORD* &dest, CComBSTR &string_data, bool bWriteResource = false)
		{
			if ( ! bWriteResource && string_data.IsEmpty() )
			{
				return sizeof(WORD) ;
			}

			if ( ! dest )
			{
				return string_data.Length() * sizeof(WORD) ;
			}

			// if string_data is empty, it means bWriteResource is true... so we need to write 0
			if( string_data.IsEmpty() )
			{
				ATLASSERT( bWriteResource ) ;

				*dest = 0 ;
				++dest ;
				return sizeof(WORD) ;
			}

			ATLASSERT( m_offset < m_size ) ;
			int size_left = m_size - m_offset ;
			HRESULT hr = StringCbCopyW( dest, size_left, (LPCWSTR)string_data ) ;
			ASSERT_HRESULT( hr ) ;
			
			return string_data.Length() * sizeof(WORD) ;
		}
		
		enum	COMMONCONTROLS{
			DLG_BUTTON		= 0x80,
			DLG_EDIT		= 0x81,
			DLG_STATIC		= 0x82,
			DLG_LIST		= 0x83,
			DLG_SCROLLBAR	= 0x84,
			DLG_COMBO		= 0x85
		};
		WORD			m_uiNumCtrls;	// Number of controls on in the template
		int				m_size;			// Total size of the template
		int				m_offset;		// Offset
		BYTE*			m_pTemplate;	// Pointer to the template data
		bool			m_bModal;
		Array< BYTE >	m_byte_buffer ;
	};
}
#endif // _RESOURCELESSDLG_TRILOBYTE_SOLUTIONS_H