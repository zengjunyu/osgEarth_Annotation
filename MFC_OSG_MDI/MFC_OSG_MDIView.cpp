// MFC_OSG_MDIView.cpp : implementation of the CMFC_OSG_MDIView class
//

#include "stdafx.h"
#include "MFC_OSG_MDI.h"
#include "MFC_OSG_MDIDoc.h"
#include "MFC_OSG_MDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CMFC_OSG_MDIView, CView)

BEGIN_MESSAGE_MAP(CMFC_OSG_MDIView, CView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_KEYDOWN()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CMFC_OSG_MDIView::CMFC_OSG_MDIView()
{
}

CMFC_OSG_MDIView::~CMFC_OSG_MDIView()
{
}

BOOL CMFC_OSG_MDIView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CView::PreCreateWindow(cs);
}

void CMFC_OSG_MDIView::OnDraw(CDC* /*pDC*/)
{
    CMFC_OSG_MDIDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
}

#ifdef _DEBUG
void CMFC_OSG_MDIView::AssertValid() const
{
    CView::AssertValid();
}

void CMFC_OSG_MDIView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CMFC_OSG_MDIDoc* CMFC_OSG_MDIView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_OSG_MDIDoc)));
    return (CMFC_OSG_MDIDoc*)m_pDocument;
}
#endif //_DEBUG

void CMFC_OSG_MDIView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

	i_GxWorld = GxWorld_CreateWorldObject(m_hWnd);
	i_GxWorld->Display();
	i_GxWorld->InitAnno();
}