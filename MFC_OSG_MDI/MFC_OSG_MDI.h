// MFC_OSG_MDI.h : main header file for the MFC_OSG_MDI application
//
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "I_GxWorld.h"
#include "GVAnnotations.h"


// CMFC_OSG_MDIApp:
// See MFC_OSG_MDI.cpp for the implementation of this class
//

class CMFC_OSG_MDIApp : public CWinApp
{
public:
    CMFC_OSG_MDIApp();


// Overrides
public:
    virtual BOOL InitInstance();

// Implementation
    afx_msg void OnAppAbout();
	afx_msg void OnDealWithMenus(UINT nID);
    DECLARE_MESSAGE_MAP()
};

extern I_GxWorld* i_GxWorld;

extern CMFC_OSG_MDIApp theApp;