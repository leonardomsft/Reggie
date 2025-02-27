
// Reggie.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Reggie.h"
#include "ReggieDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReggieApp

BEGIN_MESSAGE_MAP(CReggieApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CReggieApp construction

CReggieApp::CReggieApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


//CReggieApp object
CReggieApp theApp;


// CReggieApp initialization

BOOL CReggieApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	//Create the shell manager, in case the dialog contains any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager; 

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	
	CReggieDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	//Cleanup
	

	//Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

