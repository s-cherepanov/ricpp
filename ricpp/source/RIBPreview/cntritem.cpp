#include "stdafx.h"
#include "RIBPreview.h"

#include "LogDoc.h"
#include "LogView.h"
#include "cntritem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogCntrItem-Implementierung

IMPLEMENT_SERIAL(CLogCntrItem, CRichEditCntrItem, 0)

CLogCntrItem::CLogCntrItem(REOBJECT *preo, CLogDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
}

/////////////////////////////////////////////////////////////////////////////
// CLogCntrItem-Diagnose

#ifdef _DEBUG
void CLogCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CLogCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
