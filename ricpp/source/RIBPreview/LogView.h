#ifndef _RICPP_RIBPREVIEW_LOGVIEW_H
#define _RICPP_RIBPREVIEW_LOGVIEW_H

// CLogView-Ansicht

class CLogView : public CRichEditView
{
	DECLARE_DYNCREATE(CLogView)

protected:
	CLogView();           // Dynamische Erstellung verwendet geschützten Konstruktor
	virtual ~CLogView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void AddLine(const char *str);
	void Clear();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


#endif // _RICPP_RIBPREVIEW_LOGVIEW_H
