
// LibraryAbsenceDetection.h : main header file for the LibraryAbsenceDetection application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLibraryAbsenceDetectionApp:
// See LibraryAbsenceDetection.cpp for the implementation of this class
//

class CLibraryAbsenceDetectionApp : public CWinApp
{
public:
	CLibraryAbsenceDetectionApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLibraryAbsenceDetectionApp theApp;
