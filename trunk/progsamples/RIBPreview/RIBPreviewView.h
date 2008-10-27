// RIBPreviewView.h : interface of the CRIBPreviewView class
//


#pragma once


class CRIBPreviewView : public CView
{
protected: // create from serialization only
	CRIBPreviewView();
	DECLARE_DYNCREATE(CRIBPreviewView)

// Attributes
public:
	CRIBPreviewDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CRIBPreviewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RIBPreviewView.cpp
inline CRIBPreviewDoc* CRIBPreviewView::GetDocument() const
   { return reinterpret_cast<CRIBPreviewDoc*>(m_pDocument); }
#endif

