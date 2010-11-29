/////////////////////////////////////////////////////////////
// Class CVXPHyperLink
//
// Written to demostrate how a standard hyperlink class
// can be overridden to use Quick HTML to display hyperlinks.
//
// Last Modified on Jan 30, 2003
//
// This library is an intellectual property of Vitaly Tomilov
//
// For more details and updates visit www.tooltips.net
//////////////////////////////////////////////////////////////

#ifndef VXP_HyperLink
#define VXP_HyperLink

#include "HyperLink.h"	// Base class CHyperLink
#include "VXPLib.h"		// Class CVXPQuickHTML and other VXPLib objects

class CVXPHyperLink:public CHyperLink
{
public:

	// Constructor:
	CVXPHyperLink();

	// Two operators for easier access to QHTML documents
	// representing the hyperlink:
	CVXPQuickHTML & operator [](hlState s){return m_links[s];}
	CVXPQuickHTML & operator ()(hlState s){return m_links[s];}

	// Call this method in responce to WM_SETTINGCHANGE, if you
	// handle this message. The method recalculates all 4
	// QHTML documents.
	void RecalculateLayout();

protected:

	//////////////////////////////////////////////////////
	// These methods override standard behavour of class
	// CHyperLink, and use QHTML documents for drawing
	// the hyperlink
	////////////////////////////////////////////////////

	// Returns size of the QHTML document used for drawing
	// the current state of the hyperlink:
	virtual CSize GetSize();

	// Draws the hyperlink depending on its current state:
	virtual void OnDrawLink(CDC & dc, int x, int y);

	// Called by class CHyperLink to notify derived classes
	// when they can do some initialization. We can use it
	// to recalculate QHTML documents.
	virtual void OnInitialUpdate();

private:

	// Four QHTML documents to represent the hyperlink in its
	// four possible states: hlNormal, hlHighlight, hlNormalVisited
	// and hlHighlightVisited. Use operator () or [] to access
	// these documents:
	CVXPQuickHTML m_links[4];
};

#endif
