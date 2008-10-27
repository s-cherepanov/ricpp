#ifndef _RICPP_RIBPREVIEW_LOGDOC_H
#define _RICPP_RIBPREVIEW_LOGDOC_H

// CLogDoc-Dokument

class CLogDoc : public CRichEditDoc
{
private:
	using CRichEditDoc::GetView;

protected: // Nur aus Serialisierung erstellen
	DECLARE_DYNCREATE(CLogDoc)

public:
	CLogDoc();
	virtual ~CLogDoc();
	virtual void Serialize(CArchive& ar);   // überladen für Dokument-E/A
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void Clear();
	BOOL VisibleView();
	void HideView(BOOL hide);
	RtVoid errorPrint (RtInt code, RtInt severity, const char *message);

	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo = NULL) const;

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL IsModified() { return FALSE; }
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void OnCloseDocument();
	virtual void SetTitle(LPCTSTR lpszTitle);
};

#endif // _RICPP_RIBPREVIEW_LOGDOC_H
