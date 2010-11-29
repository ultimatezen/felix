// VXPHyperLink.cpp : implementation file for class CVXPHyperLink
//

#include "stdafx.h"
#include "VXPHyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVXPHyperLink

CVXPHyperLink::CVXPHyperLink()
{
	for(int i = 0;i < 4;i ++)	// For all 4 documents
	{
		// Set drawing to be done using transparent mode
		m_links[i].Transparent = true;
	}
}

// Returning size of the currently used QHTML document:
CSize CVXPHyperLink::GetSize()
{
	hlState s = GetSupportedState();					// Finding the supported state

	return CSize(m_links[s].Width, m_links[s].Height);	// Returning the size
}

// This method draws the hyperlink using alignment
// set for the static control.
void CVXPHyperLink::OnDrawLink(CDC & dc, int x, int y)
{
	hlState s = GetSupportedState();	// Finding the current supported state

	if(IsWindowEnabled())				// If the window is enabled
	{
		if(bSunken)
			m_links[s].DrawState(dc, (short)x, (short)y, dssDisabled);	// Drawing disabled state
		m_links[s].Draw(dc, (short)x, (short)y);		// Drawing the document
	}
	else
		m_links[s].DrawState(dc, (short)x, (short)y, dssDisabled);	// Drawing disabled state
}

// Called by class CHyperLink to notify derived classes
// when they can do some initialization. We can use it here
// to recalculate QHTML documents:
void CVXPHyperLink::OnInitialUpdate()
{
	// Recalculate QHTML document layout.
	// This will significantly improve the speed at which
	// these documents will be drawn initially.
	RecalculateLayout();
}

void CVXPHyperLink::RecalculateLayout()
{
	for(int i = 0;i < 4;i ++)	// For all 4 documents
		m_links[i].RecalculateLayout();
}
