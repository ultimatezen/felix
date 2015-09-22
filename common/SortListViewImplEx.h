#pragma once


//lint -esym(1509, CSortListViewImplEx)				(Warning -- base class destructor for class is not virtual)
//lint -esym(1512, CSortListViewImplEx<*>)			(Warning -- destructor for base class is not virtual)
//lint -esym(1712, CSortListViewImplEx<*>)			(Info -- default constructor not defined for class)
//lint -esym(1932, WTL::CSortListViewImplEx<*>)		(Note -- Base class is not abstract -- More Effective C++ #33)

//lint -save -e534 -e578 -e613 -e616 -e713 -e732 -e737 -e785 -e825 -e826 -e919 -e929 -e1401 -e1705 -e1762 -e1773 -e1924 -e1926 -e1939

namespace WTL
{

template <class T>
class CSortListViewImplEx
{
public:
	enum
	{
		m_cchCmpTextMax = 32, // overrideable
		m_cxSortImage = 16,
		m_cySortImage = 15,
		m_cxSortArrow = 11,
		m_cySortArrow = 6,
		m_iSortUp = 0,        // index of sort bitmaps
		m_iSortDown = 1,
		m_nShellSortUpID = 133
	};

	// passed to LVCompare functions as lParam1 and lParam2 
	struct LVCompareParam
	{
		int iItem;
		DWORD_PTR dwItemData;
		union
		{
			long lValue;
			double dblValue;
			DECIMAL decValue;
			LPCTSTR pszValue;
		};
	};
	
	// passed to LVCompare functions as the lParamSort parameter
	struct LVSortInfo
	{
		T* pT;
		int iSortCol;
		bool bDescending;
	};

protected:
	bool m_bSortDescending;
	bool m_bCommCtrl6;
	int m_iSortColumn;
	CBitmap m_bmSort[2];
	int m_fmtOldSortCol;
	HBITMAP m_hbmOldSortCol;
	DWORD m_dwSortLVExtendedStyle;
	ATL::CSimpleArray<WORD> m_arrColSortType;
	bool m_bUseWaitCursor;

public:	
	CSortListViewImplEx() :
			m_bSortDescending(false),
			m_bCommCtrl6(false),
			m_iSortColumn(-1), 
			m_fmtOldSortCol(0),
			m_hbmOldSortCol(NULL),
			m_dwSortLVExtendedStyle(SORTLV_USESHELLBITMAPS),
			m_bUseWaitCursor(true)
	{
#ifndef _WIN32_WCE
		DWORD dwMajor = 0;
		DWORD dwMinor = 0;
		HRESULT hRet = ATL::AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
		m_bCommCtrl6 = SUCCEEDED(hRet) && dwMajor >= 6;
#endif // !_WIN32_WCE
	}
	
// Attributes
	void SetSortColumn(int iCol)
	{
		T* pT = static_cast<T*>(this);
		ATLASSERT(::IsWindow(pT->m_hWnd));
		CHeaderCtrl header = pT->GetHeader();
		ATLASSERT(header.m_hWnd != NULL);
		ATLASSERT(iCol >= -1 && iCol < m_arrColSortType.GetSize());

		int iOldSortCol = m_iSortColumn;
		m_iSortColumn = iCol;
		if(m_bCommCtrl6)
		{
#ifndef HDF_SORTUP
			const int HDF_SORTUP = 0x0400;	
#endif // HDF_SORTUP
#ifndef HDF_SORTDOWN
			const int HDF_SORTDOWN = 0x0200;	
#endif // HDF_SORTDOWN
			const int nMask = HDF_SORTUP | HDF_SORTDOWN;
			HDITEM hditem = { HDI_FORMAT };
			if(iOldSortCol != iCol && iOldSortCol >= 0 && header.GetItem(iOldSortCol, &hditem))
			{
				hditem.fmt &= ~nMask;
				header.SetItem(iOldSortCol, &hditem);
			}
			if(iCol >= 0 && header.GetItem(iCol, &hditem))
			{
				hditem.fmt &= ~nMask;
				hditem.fmt |= m_bSortDescending ? HDF_SORTDOWN : HDF_SORTUP;
				header.SetItem(iCol, &hditem);
			}
			return;
		}

		if(m_bmSort[m_iSortUp].IsNull())
			pT->CreateSortBitmaps();

		// restore previous sort column's bitmap, if any, and format
		HDITEM hditem = { HDI_BITMAP | HDI_FORMAT };
		if(iOldSortCol != iCol && iOldSortCol >= 0)
		{
			hditem.hbm = m_hbmOldSortCol;
			hditem.fmt = m_fmtOldSortCol;
			header.SetItem(iOldSortCol, &hditem);
		}

		// save new sort column's bitmap and format, and add our sort bitmap
		if(iCol >= 0 && header.GetItem(iCol, &hditem))
		{
			if(iOldSortCol != iCol)
			{
				m_fmtOldSortCol = hditem.fmt;
				m_hbmOldSortCol = hditem.hbm;
			}
			hditem.fmt &= ~HDF_IMAGE;
			hditem.fmt |= HDF_BITMAP | HDF_BITMAP_ON_RIGHT;
			int i = m_bSortDescending ? m_iSortDown : m_iSortUp;
			hditem.hbm = m_bmSort[i];
			header.SetItem(iCol, &hditem);
		}
	}

	int GetSortColumn() const
	{
		return m_iSortColumn;
	}

	void SetColumnSortType(int iCol, WORD wType)
	{
		ATLASSERT(iCol >= 0 && iCol < m_arrColSortType.GetSize());
		ATLASSERT(wType >= LVCOLSORT_NONE && wType <= LVCOLSORT_LAST);
		m_arrColSortType[iCol] = wType;
	}

	WORD GetColumnSortType(int iCol) const
	{
		ATLASSERT((iCol >= 0) && iCol < m_arrColSortType.GetSize());
		return m_arrColSortType[iCol];
	}

	int GetColumnCount() const
	{
		const T* pT = static_cast<const T*>(this);
		ATLASSERT(::IsWindow(pT->m_hWnd));
		CHeaderCtrl header = pT->GetHeader();
		return header.m_hWnd != NULL ? header.GetItemCount() : 0;
	}

	bool IsSortDescending() const
	{
		return m_bSortDescending;
	}

	DWORD GetSortListViewExtendedStyle() const
	{
		return m_dwSortLVExtendedStyle;
	}

	DWORD SetSortListViewExtendedStyle(DWORD dwExtendedStyle, DWORD dwMask = 0)
	{
		DWORD dwPrevStyle = m_dwSortLVExtendedStyle;
		if(dwMask == 0)
			m_dwSortLVExtendedStyle = dwExtendedStyle;
		else
			m_dwSortLVExtendedStyle = (m_dwSortLVExtendedStyle & ~dwMask) | (dwExtendedStyle & dwMask);
		return dwPrevStyle;
	}

// Operations
	bool DoSortItems(int iCol, bool bDescending = false)
	{
		T* pT = static_cast<T*>(this);
		ATLASSERT(::IsWindow(pT->m_hWnd));
		ATLASSERT(iCol >= 0 && iCol < m_arrColSortType.GetSize());

		WORD wType = m_arrColSortType[iCol];
		if(wType == LVCOLSORT_NONE)
			return false;

		int nCount = pT->GetItemCount();
		if(nCount < 2)
		{
			m_bSortDescending = bDescending;
			SetSortColumn(iCol);
			return true;
		}

		CWaitCursor waitCursor(false);
		if(m_bUseWaitCursor)
			waitCursor.Set();

		bool bRet = false;

        bool bIsVirtualList = ( (::GetWindowLong(pT->m_hWnd, GWL_STYLE) & LVS_OWNERDATA) > 0);
        if (bIsVirtualList)
        {
            T* pT = static_cast<T*>(this);
            
            bRet = pT->DoSortItemsImpl(iCol, bDescending);
			if (bRet)
			{
				pT->Invalidate();
			}	
        }
		else
		{
			LVCompareParam* pParam = NULL;
			ATLTRY(pParam = new LVCompareParam[nCount]);
			PFNLVCOMPARE pFunc = NULL;
			TCHAR pszTemp[pT->m_cchCmpTextMax];
			bool bStrValue = false;

			switch(wType)
			{
			case LVCOLSORT_TEXT:
				pFunc = (PFNLVCOMPARE)pT->LVCompareText;
			case LVCOLSORT_TEXTNOCASE:
				if(pFunc == NULL)
					pFunc = (PFNLVCOMPARE)pT->LVCompareTextNoCase;
			case LVCOLSORT_CUSTOM:
				{
					if(pFunc == NULL)
						pFunc = (PFNLVCOMPARE)pT->LVCompareCustom;

					for(int i = 0; i < nCount; i++)
					{
						pParam[i].iItem = i;
						pParam[i].dwItemData = pT->GetItemData(i);
						pParam[i].pszValue = new TCHAR[pT->m_cchCmpTextMax];
						pT->GetItemText(i, iCol, (LPTSTR)pParam[i].pszValue, pT->m_cchCmpTextMax);
						pT->SetItemData(i, (DWORD_PTR)&pParam[i]);
					}
					bStrValue = true;
				}
				break;
			case LVCOLSORT_LONG:
				{
					pFunc = (PFNLVCOMPARE)pT->LVCompareLong;
					for(int i = 0; i < nCount; i++)
					{
						pParam[i].iItem = i;
						pParam[i].dwItemData = pT->GetItemData(i);
						pT->GetItemText(i, iCol, pszTemp, pT->m_cchCmpTextMax);
						pParam[i].lValue = pT->StrToLong(pszTemp);
						pT->SetItemData(i, (DWORD_PTR)&pParam[i]);
					}
				}
				break;
			case LVCOLSORT_DOUBLE:
				{
					pFunc = (PFNLVCOMPARE)pT->LVCompareDouble;
					for(int i = 0; i < nCount; i++)
					{
						pParam[i].iItem = i;
						pParam[i].dwItemData = pT->GetItemData(i);
						pT->GetItemText(i, iCol, pszTemp, pT->m_cchCmpTextMax);
						pParam[i].dblValue = pT->StrToDouble(pszTemp);
						pT->SetItemData(i, (DWORD_PTR)&pParam[i]);
					}
				}
				break;
			case LVCOLSORT_DECIMAL:
				{
					pFunc = (PFNLVCOMPARE)pT->LVCompareDecimal;
					for(int i = 0; i < nCount; i++)
					{
						pParam[i].iItem = i;
						pParam[i].dwItemData = pT->GetItemData(i);
						pT->GetItemText(i, iCol, pszTemp, pT->m_cchCmpTextMax);
						pT->StrToDecimal(pszTemp, &pParam[i].decValue);
						pT->SetItemData(i, (DWORD_PTR)&pParam[i]);
					}
				}
				break;
			case LVCOLSORT_DATETIME:
			case LVCOLSORT_DATE:
			case LVCOLSORT_TIME:
				{
					pFunc = (PFNLVCOMPARE)pT->LVCompareDouble;
					DWORD dwFlags = LOCALE_NOUSEROVERRIDE;
					if(wType == LVCOLSORT_DATE)
						dwFlags |= VAR_DATEVALUEONLY;
					else if(wType == LVCOLSORT_TIME)
						dwFlags |= VAR_TIMEVALUEONLY;
					for(int i = 0; i < nCount; i++)
					{
						pParam[i].iItem = i;
						pParam[i].dwItemData = pT->GetItemData(i);
						pT->GetItemText(i, iCol, pszTemp, pT->m_cchCmpTextMax);
						pParam[i].dblValue = pT->DateStrToDouble(pszTemp, dwFlags);
						pT->SetItemData(i, (DWORD_PTR)&pParam[i]);
					}
				}
				break;
			default:
				ATLTRACE2(atlTraceUI, 0, _T("Unknown value for sort type in CSortListViewImplEx::DoSortItems()\n"));
				break;
			} // switch(wType)

			ATLASSERT(pFunc != NULL);
			LVSortInfo lvsi = { pT, iCol, bDescending };
			bRet = ((BOOL)pT->DefWindowProc(LVM_SORTITEMS, (WPARAM)&lvsi, (LPARAM)pFunc) != FALSE);
			for(int i = 0; i < nCount; i++)
			{
				DWORD_PTR dwItemData = pT->GetItemData(i);
				LVCompareParam* p = (LVCompareParam*)dwItemData;
				ATLASSERT(p != NULL);
				if(bStrValue)
					delete [] (TCHAR*)p->pszValue;
				pT->SetItemData(i, p->dwItemData);
			}
			delete [] pParam;
		}
		if(bRet)
		{
			m_bSortDescending = bDescending;
			SetSortColumn(iCol);
		}

		if(m_bUseWaitCursor)
			waitCursor.Restore();

		return bRet;
	}

	void CreateSortBitmaps()
	{
		if((m_dwSortLVExtendedStyle & SORTLV_USESHELLBITMAPS) != 0)
		{
			bool bFree = false;
			LPCTSTR pszModule = _T("shell32.dll"); 
			HINSTANCE hShell = ::GetModuleHandle(pszModule);

			if (hShell == NULL)		
			{
				hShell = ::LoadLibrary(pszModule);
				bFree = true;
			}
 
			if (hShell != NULL)
			{
				bool bSuccess = true;
				for(int i = m_iSortUp; i <= m_iSortDown; i++)
				{
					if(!m_bmSort[i].IsNull())
						m_bmSort[i].DeleteObject();
					m_bmSort[i] = (HBITMAP)::LoadImage(hShell, MAKEINTRESOURCE(m_nShellSortUpID + i), 
#ifndef _WIN32_WCE
						IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
#else // CE specific
						IMAGE_BITMAP, 0, 0, 0);
#endif // _WIN32_WCE
					if(m_bmSort[i].IsNull())
					{
						bSuccess = false;
						break;
					}
				}
				if(bFree)
					::FreeLibrary(hShell);
				if(bSuccess)
					return;
			}
		}

		T* pT = static_cast<T*>(this);
		for(int i = m_iSortUp; i <= m_iSortDown; i++)
		{
			if(!m_bmSort[i].IsNull())
				m_bmSort[i].DeleteObject();

			CDC dcMem;
			CClientDC dc(::GetDesktopWindow());
			dcMem.CreateCompatibleDC(dc.m_hDC);
			m_bmSort[i].CreateCompatibleBitmap(dc.m_hDC, m_cxSortImage, m_cySortImage);
			HBITMAP hbmOld = dcMem.SelectBitmap(m_bmSort[i]);
			RECT rc = {0,0,m_cxSortImage, m_cySortImage};
			pT->DrawSortBitmap(dcMem.m_hDC, i, &rc);
			dcMem.SelectBitmap(hbmOld);
			dcMem.DeleteDC();
		}
	}

	void NotifyParentSortChanged(int iNewSortCol, int iOldSortCol)
	{
		T* pT = static_cast<T*>(this);
		int nID = pT->GetDlgCtrlID();
		NMSORTLISTVIEW nm = { { pT->m_hWnd, nID, SLVN_SORTCHANGED }, iNewSortCol, iOldSortCol };
		::SendMessage(pT->GetParent(), WM_NOTIFY, (WPARAM)nID, (LPARAM)&nm);
	}

// Overrideables
	int CompareItemsCustom(LVCompareParam* /*pItem1*/, LVCompareParam* /*pItem2*/, int /*iSortCol*/)
	{
		// pItem1 and pItem2 contain valid iItem, dwItemData, and pszValue members.
		// If item1 > item2 return 1, if item1 < item2 return -1, else return 0.
		return 0;
	}

	void DrawSortBitmap(CDCHandle dc, int iBitmap, LPRECT prc)
	{
		dc.FillRect(prc, ::GetSysColorBrush(COLOR_BTNFACE));	
		HBRUSH hbrOld = dc.SelectBrush(::GetSysColorBrush(COLOR_BTNSHADOW));
		CPen pen;
		pen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_BTNSHADOW));
		HPEN hpenOld = dc.SelectPen(pen);
		POINT ptOrg = { (m_cxSortImage - m_cxSortArrow) / 2, (m_cySortImage - m_cySortArrow) / 2 };
		if(iBitmap == m_iSortUp)
		{
			POINT pts[3] = 
			{
				{ ptOrg.x + m_cxSortArrow / 2, ptOrg.y },
				{ ptOrg.x, ptOrg.y + m_cySortArrow - 1 }, 
				{ ptOrg.x + m_cxSortArrow - 1, ptOrg.y + m_cySortArrow - 1 }
			};
			dc.Polygon(pts, 3);
		}
		else
		{
			POINT pts[3] = 
			{
				{ ptOrg.x, ptOrg.y },
				{ ptOrg.x + m_cxSortArrow / 2, ptOrg.y + m_cySortArrow - 1 },
				{ ptOrg.x + m_cxSortArrow - 1, ptOrg.y }
			};
			dc.Polygon(pts, 3);
		}
		dc.SelectBrush(hbrOld);
		dc.SelectPen(hpenOld);
	}

	double DateStrToDouble(LPCTSTR lpstr, DWORD dwFlags)
	{
		ATLASSERT(lpstr != NULL);
		if(lpstr == NULL || lpstr[0] == _T('\0'))
			return 0;

		USES_CONVERSION;
		HRESULT hRet = E_FAIL;
		DATE dRet = 0;
		if (FAILED(hRet = ::VarDateFromStr((LPOLESTR)T2COLE(lpstr), LANG_USER_DEFAULT, dwFlags, &dRet)))
		{
			ATLTRACE2(atlTraceUI, 0, _T("VarDateFromStr failed with result of 0x%8.8X\n"), hRet);
			dRet = 0;
		}
		return dRet;
	}

	long StrToLong(LPCTSTR lpstr)
	{
		ATLASSERT(lpstr != NULL);
		if(lpstr == NULL || lpstr[0] == _T('\0'))
			return 0;
		
		USES_CONVERSION;
		HRESULT hRet = E_FAIL;
		long lRet = 0;
		if (FAILED(hRet = ::VarI4FromStr((LPOLESTR)T2COLE(lpstr), LANG_USER_DEFAULT, LOCALE_NOUSEROVERRIDE, &lRet)))
		{
			ATLTRACE2(atlTraceUI, 0, _T("VarI4FromStr failed with result of 0x%8.8X\n"), hRet);
			lRet = 0;
		}
		return lRet;
	}

	double StrToDouble(LPCTSTR lpstr)
	{
		ATLASSERT(lpstr != NULL);
		if(lpstr == NULL || lpstr[0] == _T('\0'))
			return 0;

		USES_CONVERSION;
		HRESULT hRet = E_FAIL;
		double dblRet = 0;
		if (FAILED(hRet = ::VarR8FromStr((LPOLESTR)T2COLE(lpstr), LANG_USER_DEFAULT, LOCALE_NOUSEROVERRIDE, &dblRet)))
		{
			ATLTRACE2(atlTraceUI, 0, _T("VarR8FromStr failed with result of 0x%8.8X\n"), hRet);
			dblRet = 0;
		}
		return dblRet;
	}

	bool StrToDecimal(LPCTSTR lpstr, DECIMAL* pDecimal)
	{
		ATLASSERT(lpstr != NULL);
		ATLASSERT(pDecimal != NULL);
		if(lpstr == NULL || pDecimal == NULL)
			return false;

		USES_CONVERSION;
		HRESULT hRet = E_FAIL;
		if (FAILED(hRet = ::VarDecFromStr((LPOLESTR)T2COLE(lpstr), LANG_USER_DEFAULT, LOCALE_NOUSEROVERRIDE, pDecimal)))
		{
			ATLTRACE2(atlTraceUI, 0, _T("VarDecFromStr failed with result of 0x%8.8X\n"), hRet);
			pDecimal->Lo64 = 0;
			pDecimal->Hi32 = 0;
			pDecimal->signscale = 0;
			return false;
		}
		return true;
	}

// Overrideable PFNLVCOMPARE functions
	static int CALLBACK LVCompareText(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		ATLASSERT(lParam1 != NULL && lParam2 != NULL && lParamSort != NULL);

		LVCompareParam* pParam1 = (LVCompareParam*)lParam1;
		LVCompareParam* pParam2 = (LVCompareParam*)lParam2;
		LVSortInfo* pInfo = (LVSortInfo*)lParamSort;
		
		int nRet = lstrcmp(pParam1->pszValue, pParam2->pszValue);
		return pInfo->bDescending ? -nRet : nRet;
	}

	static int CALLBACK LVCompareTextNoCase(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		ATLASSERT(lParam1 != NULL && lParam2 != NULL && lParamSort != NULL);

		LVCompareParam* pParam1 = (LVCompareParam*)lParam1;
		LVCompareParam* pParam2 = (LVCompareParam*)lParam2;
		LVSortInfo* pInfo = (LVSortInfo*)lParamSort;
		
		int nRet = lstrcmpi(pParam1->pszValue, pParam2->pszValue);
		return pInfo->bDescending ? -nRet : nRet;
	}

	static int CALLBACK LVCompareLong(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		ATLASSERT(lParam1 != NULL && lParam2 != NULL && lParamSort != NULL);

		LVCompareParam* pParam1 = (LVCompareParam*)lParam1;
		LVCompareParam* pParam2 = (LVCompareParam*)lParam2;
		LVSortInfo* pInfo = (LVSortInfo*)lParamSort;
		
		int nRet = 0;
		if(pParam1->lValue > pParam2->lValue)
			nRet = 1;
		else if(pParam1->lValue < pParam2->lValue)
			nRet = -1;
		return pInfo->bDescending ? -nRet : nRet;
	}

	static int CALLBACK LVCompareDouble(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		ATLASSERT(lParam1 != NULL && lParam2 != NULL && lParamSort != NULL);

		LVCompareParam* pParam1 = (LVCompareParam*)lParam1;
		LVCompareParam* pParam2 = (LVCompareParam*)lParam2;
		LVSortInfo* pInfo = (LVSortInfo*)lParamSort;
		
		int nRet = 0;
		if(pParam1->dblValue > pParam2->dblValue)
			nRet = 1;
		else if(pParam1->dblValue < pParam2->dblValue)
			nRet = -1;
		return pInfo->bDescending ? -nRet : nRet;
	}

	static int CALLBACK LVCompareCustom(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		ATLASSERT(lParam1 != NULL && lParam2 != NULL && lParamSort != NULL);

		LVCompareParam* pParam1 = (LVCompareParam*)lParam1;
		LVCompareParam* pParam2 = (LVCompareParam*)lParam2;
		LVSortInfo* pInfo = (LVSortInfo*)lParamSort;
		
		int nRet = pInfo->pT->CompareItemsCustom(pParam1, pParam2, pInfo->iSortCol);
		return pInfo->bDescending ? -nRet : nRet;
	}

#ifndef _WIN32_WCE
	static int CALLBACK LVCompareDecimal(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		ATLASSERT(lParam1 != NULL && lParam2 != NULL && lParamSort != NULL);

		LVCompareParam* pParam1 = (LVCompareParam*)lParam1;
		LVCompareParam* pParam2 = (LVCompareParam*)lParam2;
		LVSortInfo* pInfo = (LVSortInfo*)lParamSort;
		
		int nRet = (int)::VarDecCmp(&pParam1->decValue, &pParam2->decValue);
		nRet--;
		return pInfo->bDescending ? -nRet : nRet;
	}
#else
	// Compare mantissas, ignore sign and scale
	static int CompareMantissas(const DECIMAL& decLeft, const DECIMAL& decRight)
	{
		if (decLeft.Hi32 < decRight.Hi32)
		{
			return -1;
		}
		if (decLeft.Hi32 > decRight.Hi32)
		{
			return 1;
		}
		// Here, decLeft.Hi32 == decRight.Hi32
		if (decLeft.Lo64 < decRight.Lo64)
		{
			return -1;
		}
		if (decLeft.Lo64 > decRight.Lo64)
		{
			return 1;
		}
		return 0;
	}

	// return values: VARCMP_LT, VARCMP_EQ, VARCMP_GT, VARCMP_NULL
	static HRESULT VarDecCmp(const DECIMAL* pdecLeft, const DECIMAL* pdecRight)
	{
		static const ULONG powersOfTen[] =
		{
			10ul,
			100ul,
			1000ul,
			10000ul,
			100000ul,
			1000000ul,
			10000000ul,
			100000000ul,
			1000000000ul
		};
		static const int largestPower = sizeof(powersOfTen) / sizeof(powersOfTen[0]);
		if (!pdecLeft || !pdecRight)
		{
			return VARCMP_NULL;
		}
		
		// Degenerate case - at least one comparand is of the form
		// [+-]0*10^N (denormalized zero)
		bool bLeftZero = (!pdecLeft->Lo64 && !pdecLeft->Hi32);
		bool bRightZero = (!pdecRight->Lo64 && !pdecRight->Hi32);
		if (bLeftZero && bRightZero)
		{
			return VARCMP_EQ;
		}
		bool bLeftNeg = ((pdecLeft->sign & DECIMAL_NEG) != 0);
		bool bRightNeg = ((pdecRight->sign & DECIMAL_NEG) != 0);
		if (bLeftZero)
		{
			return (bRightNeg ? VARCMP_GT : VARCMP_LT);
		}
		// This also covers the case where the comparands have different signs
		if (bRightZero || bLeftNeg != bRightNeg)
		{
			return (bLeftNeg ? VARCMP_LT : VARCMP_GT);
		}

		// Here both comparands have the same sign and need to be compared
		// on mantissa and scale. The result is obvious when
		// 1. Scales are equal (then compare mantissas)
		// 2. A number with smaller scale is also the one with larger mantissa
		//    (then this number is obviously larger)
		// In the remaining case, we would multiply the number with smaller
		// scale by 10 and simultaneously increment its scale (which amounts to
		// adding trailing zeros after decimal point), until the numbers fall under
		// one of the two cases above
		DECIMAL temp;
		bool bInvert = bLeftNeg; // the final result needs to be inverted
		if (pdecLeft->scale < pdecRight->scale)
		{
			temp = *pdecLeft;
		}
		else
		{
			temp = *pdecRight;
			pdecRight = pdecLeft;
			bInvert = !bInvert;
		}

		// Now temp is the number with smaller (or equal) scale, and
		// we can modify it freely without touching original parameters
		int comp;
		while ((comp = CompareMantissas(temp, *pdecRight)) < 0 &&
			temp.scale < pdecRight->scale)
		{
			// Multiply by an appropriate power of 10
			int scaleDiff = pdecRight->scale - temp.scale;
			if (scaleDiff > largestPower)
			{
				// Keep the multiplier representable in 32bit
				scaleDiff = largestPower;
			}
			DWORDLONG power = powersOfTen[scaleDiff - 1];
			// Multiply temp's mantissa by power
			DWORDLONG product = temp.Lo32 * power;
			ULONG carry = static_cast<ULONG>(product >> 32);
			temp.Lo32  = static_cast<ULONG>(product);
			product = temp.Mid32 * power + carry;
			carry = static_cast<ULONG>(product >> 32);
			temp.Mid32 = static_cast<ULONG>(product);
			product = temp.Hi32 * power + carry;
			if (static_cast<ULONG>(product >> 32))
			{
				// Multiplication overflowed - pdecLeft is clearly larger
				break;
			}
			temp.Hi32 = static_cast<ULONG>(product);
			temp.scale = (BYTE)(temp.scale + scaleDiff);
		}
		if (temp.scale < pdecRight->scale)
		{
			comp = 1;
		}
		if (bInvert)
		{
			comp = -comp;
		}
		return (comp > 0 ? VARCMP_GT : comp < 0 ? VARCMP_LT : VARCMP_EQ);
	}

	static int CALLBACK LVCompareDecimal(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		ATLASSERT(lParam1 != NULL && lParam2 != NULL && lParamSort != NULL);

		LVCompareParam* pParam1 = (LVCompareParam*)lParam1;
		LVCompareParam* pParam2 = (LVCompareParam*)lParam2;
		LVSortInfo* pInfo = (LVSortInfo*)lParamSort;
		
		int nRet = (int)VarDecCmp(&pParam1->decValue, &pParam2->decValue);
		nRet--;
		return pInfo->bDescending ? -nRet : nRet;
	}
#endif // !_WIN32_WCE

	BEGIN_MSG_MAP(CSortListViewImplEx)
		MESSAGE_HANDLER(LVM_INSERTCOLUMN, OnInsertColumn)
		MESSAGE_HANDLER(LVM_DELETECOLUMN, OnDeleteColumn)
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKA, OnHeaderItemClick)
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKW, OnHeaderItemClick)
		MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
	END_MSG_MAP()

	LRESULT OnInsertColumn(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)	
	{
		T* pT = static_cast<T*>(this);
		LRESULT lRet = pT->DefWindowProc(uMsg, wParam, lParam);
		if(lRet == -1)
			return -1;

		WORD wType = 0;
		m_arrColSortType.Add(wType);
		int nCount = m_arrColSortType.GetSize();
		ATLASSERT(nCount == GetColumnCount());

		for(int i = nCount - 1; i > lRet; i--)
			m_arrColSortType[i] = m_arrColSortType[i - 1];
		m_arrColSortType[(int)lRet] = LVCOLSORT_TEXT;

		if(lRet <= m_iSortColumn)
			m_iSortColumn++;

		return lRet;
	}

	LRESULT OnDeleteColumn(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)	
	{
		T* pT = static_cast<T*>(this);	//lint !e1929
		LRESULT lRet = pT->DefWindowProc(uMsg, wParam, lParam);
		if(lRet == 0)
			return 0;

		int iCol = static_cast<int>(wParam);
		if(m_iSortColumn == iCol)
			m_iSortColumn = -1;
		else if(m_iSortColumn > iCol)
			m_iSortColumn--;
		m_arrColSortType.RemoveAt(iCol);

		return lRet;
	}

	LRESULT OnHeaderItemClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
	{
		LPNMHEADER p = (LPNMHEADER)pnmh;
		if(p->iButton == 0)
		{
			int iOld = m_iSortColumn;
			bool bDescending = (m_iSortColumn == p->iItem) ? !m_bSortDescending : false;
			if(DoSortItems(p->iItem, bDescending))
				NotifyParentSortChanged(p->iItem, iOld);				
		}
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnSettingChange(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
#ifndef _WIN32_WCE
		if(wParam == SPI_SETNONCLIENTMETRICS)
			GetSystemSettings();
#else  // CE specific
		wParam; // avoid level 4 warning
		GetSystemSettings();
#endif // _WIN32_WCE
		bHandled = FALSE;
		return 0;
	}

	void GetSystemSettings()
	{
		if(!m_bCommCtrl6 && !m_bmSort[m_iSortUp].IsNull())
		{
			T* pT = static_cast<T*>(this);
			pT->CreateSortBitmaps();
			if(m_iSortColumn != -1)
				SetSortColumn(m_iSortColumn);
		}
	}

	bool DoSortItemsImpl(int /*iCol*/, bool /*bDescending*/)
	{
        ATLASSERT(false);       // Must be overridden

		return false;
	}

};

}; // namespace WTL
//lint -restore
