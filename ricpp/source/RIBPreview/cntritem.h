#ifndef _RICPP_RIBPREVIEW_CNTRITEM_H
#define _RICPP_RIBPREVIEW_CNTRITEM_H

class CLogDoc;
class CLogView;

class CLogCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CLogCntrItem)

// Konstruktoren
public:
	CLogCntrItem(REOBJECT* preo = NULL, CLogDoc* pContainer = NULL);

// Attribute
public:
	CLogDoc* GetDocument()
		{ return (CLogDoc*)COleClientItem::GetDocument(); }
	CLogView* GetActiveView()
		{ return (CLogView*)COleClientItem::GetActiveView(); }

	// Vom Klassen-Assistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CWordPadCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementierung
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#endif // _RICPP_RIBPREVIEW_CNTRITEM_H
