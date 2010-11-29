// MenusDoc.h : interface of the CMenusDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUSDOC_H__87AB0ED9_CF02_4DF5_910D_9377B14566C7__INCLUDED_)
#define AFX_MENUSDOC_H__87AB0ED9_CF02_4DF5_910D_9377B14566C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMenusDoc : public CDocument
{
protected: // create from serialization only
	CMenusDoc();
	DECLARE_DYNCREATE(CMenusDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenusDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenusDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMenusDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUSDOC_H__87AB0ED9_CF02_4DF5_910D_9377B14566C7__INCLUDED_)
