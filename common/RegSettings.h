/** Classes to persist window and other UI info.

	Persists window, rebar, and splitter state in the registry.
*/

#pragma once

/** Wrapper for WINDOWPLACEMENT struct.

	Initializes on construction.
 */
struct CWindowPlacement : public WINDOWPLACEMENT
{
	CWindowPlacement();
};

/** Persists window settings (pos, size, show state).
*/
class CWindowSettings
{
public:
	CWindowPlacement m_WindowPlacement;

	CWindowSettings();
	/// Retrieves the window settings from Wnd.
	void GetFrom(CWindow& Wnd);
	/// Applies the window settings to Wnd.
	void ApplyTo(CWindow& Wnd, int nCmdShow = SW_SHOWNORMAL) const;

	/// Loads window settings from registry.
	bool Load(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER);
	/// Saves window settings to registry.
	bool Save(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER) const;
};

/** Persists Rebar settings.
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

	/// Retrieves the rebar settings from ReBar.
	void GetFrom(CReBarCtrl& ReBar);
	/// Applies the rebar settings to ReBar.
	void ApplyTo(CReBarCtrl& ReBar) const;

	/// Loads rebar settings from registry.
	bool Load(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER);
	/// Saves rebar settings to registry.
	bool Save(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER) const;
};

