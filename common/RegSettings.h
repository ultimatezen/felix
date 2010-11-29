/*!
	@file RegSettings.h
	@brief Classes for persisting registry info
	@date 2005/06/22
	Time: 9:12:20
	@author Ryan Ginstrom
 */

#pragma once

#include <atlbase.h>			// must be included before atlapp.h
#include <atlapp.h>				// must be included before atlmisc.h
								// must be included before atlctrls.h
#include <AtlMisc.h>
#include <AtlCtrls.h>

/**
	@class CWindowSettings
	@brief Wrapper for WINDOWPLACEMENT struct.
 */
class CWindowSettings
{
public:
	WINDOWPLACEMENT m_WindowPlacement;

	CWindowSettings();
	void GetFrom(TWindow& Wnd);
	void ApplyTo(TWindow& Wnd, int nCmdShow = SW_SHOWNORMAL)const;

	bool Load(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER);
	bool Save(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER)const;
};

/**
	@class CReBarSettings
	@brief Persists Rebar settings.
 */
class CReBarSettings
{
public:
	struct BandInfo
	{
		DWORD ID;
		DWORD cx;
		bool BreakLine;
	}* m_pBands;

	DWORD m_cbBandCount;

	CReBarSettings();
	~CReBarSettings();

	void GetFrom(CReBarCtrl& ReBar);
	void ApplyTo(CReBarCtrl& ReBar)const;

	bool Load(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER);
	bool Save(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER)const;
};

/**
	@class CSplitterSettings
	@brief Persists splitter settings.
 */
class CSplitterSettings
{
public:
	DWORD m_dwPos;

	CSplitterSettings() : m_dwPos(0) {}

	template <class T> void GetFrom(const T& Splitter)
	{
		m_dwPos = Splitter.GetSplitterPos();
	}

	template <class T> void ApplyTo(T& Splitter)const
	{
		Splitter.SetSplitterPos(m_dwPos);
	}

	bool Load(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER);
	bool Save(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER)const;
};

