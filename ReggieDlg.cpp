
// ReggieDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Reggie.h"
#include "ReggieDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Globals
LONGLONG g_IOPS;
LONG64 g_Throughput;
BOOL g_isActive;

DWORD g_TopIops;
DWORD g_TopThroughput;
DOUBLE g_TopLatency;


// CReggieDlg dialog

CReggieDlg::CReggieDlg(CWnd* pParent) : CDialogEx(CReggieDlg::IDD, pParent)
			
	, m_TestType(0)
	, m_intThreads(0)
	, m_intRootKey(0)
	, m_Desination("SOFTWARE")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReggieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IOTYPE, m_iotype);
	DDX_Control(pDX, IDC_IOSIZEGAUGE, m_iosizegauge);
	DDX_Control(pDX, IDC_IOSIZE, m_iosize);
	DDX_Control(pDX, IDC_ThreadCount, m_ThreadCount);
	DDX_Control(pDX, IDC_RootKey, m_RootKey);
	DDX_Control(pDX, IDC_IOPSProgress, m_IopsProgress);
	DDX_Control(pDX, IDC_ThroughputProgress, m_ThroughputProgress);
	DDX_Control(pDX, IDC_LatencyProgress, m_LatencyProgress);
	DDX_Control(pDX, IDC_IOPSEdit, m_IopsEdit);
	DDX_Control(pDX, IDC_ThroughputEdit, m_ThroughputEdit);
	DDX_Control(pDX, IDC_LatencyEdit, m_LatencyEdit);
	DDX_Control(pDX, IDC_TOPIOPS, m_TopIops);
	DDX_Control(pDX, IDC_TOPThroughput, m_TopThroughput);
	DDX_Control(pDX, IDC_TOPLatency, m_TopLatency);

	DDX_Radio(pDX, IDC_TransferTest, m_TestType);
	DDX_CBIndex(pDX, IDC_ThreadCount, m_intThreads);
	DDX_CBIndex(pDX, IDC_RootKey, m_intRootKey);
	DDX_Text(pDX, IDC_Destination, m_Desination);
}

BEGIN_MESSAGE_MAP(CReggieDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Start, &CReggieDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CReggieDlg::OnBnClickedStop)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CReggieDlg message handlers

BOOL CReggieDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//Initialize Controls
	m_iotype.AddString(L"READ");
	m_iotype.AddString(L"WRTIE");
	m_iotype.AddString(L"READ/WRITE");
	m_iotype.SetCurSel(0);

	m_iosize.SetRange(1, 1000);
	m_iosize.SetPos(100);
	m_iosizegauge.SetWindowTextW(L"100");

	m_ThreadCount.AddString(L"1");
	m_ThreadCount.AddString(L"2");
	m_ThreadCount.AddString(L"4");
	m_ThreadCount.AddString(L"8");
	m_ThreadCount.AddString(L"16");
	m_ThreadCount.AddString(L"32");
	m_ThreadCount.AddString(L"64");
	m_ThreadCount.AddString(L"128");
	m_ThreadCount.SetCurSel(0);

	m_RootKey.AddString(L"HKCU");
	m_RootKey.AddString(L"HKLM");
	m_RootKey.SetCurSel(0);

	//Reporting Timer
	SetTimer(1, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


//  draw the app icon. 
void CReggieDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags the minimized window.
HCURSOR CReggieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CReggieDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

	int nIndex = m_iosize.GetPos();
	CString StrText;
	StrText.Format(L"%d", nIndex);

	UpdateData();
	m_iosizegauge.SetWindowTextW(StrText);
	
}

UINT CReggieDlg::EnumWorkerThreadProc(LPVOID Param)
{
	WorkerParams* pWP = (WorkerParams*)Param;

	SearchReg(pWP->rootKey, pWP->Desination);

	return 0;
}

UINT CReggieDlg::TransferWorkerThreadProc(LPVOID Param) 
{
	WorkerParams* pWP = (WorkerParams*)Param;

	HKEY hKey;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);

	DWORD dwRet = 0;

	while (g_isActive) 
	{
		
		dwRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, pWP->Desination, 0, KEY_READ, &hKey);

		if (dwRet != ERROR_SUCCESS) return 0;

		dwRet = RegQueryValueExW(hKey, L"ProductName", 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);

		InterlockedIncrement64(&g_IOPS);
		InterlockedAdd64(&g_Throughput, dwBufferSize);
	}
	return 0;
}

void CReggieDlg::SearchReg(HKEY hroot, CString rootKeyPath)
{
	HKEY rootKey;
	HKEY newsubKey;
	DWORD rootKeyCount, newsubKeyCount;
	WCHAR subKeyName[500];
	DWORD buffSize;
	WCHAR result[500];

	//Validade params
	WCHAR backslash[] = {L"\\"};
	if (rootKeyPath.Right(1) != backslash) rootKeyPath.Append(backslash);

	//open root
	DWORD dwRet = RegOpenKeyExW(hroot, rootKeyPath, NULL, KEY_READ, &rootKey); 

	if (dwRet != ERROR_SUCCESS) return;

	//how many subkeys in root
	dwRet = RegQueryInfoKey(rootKey, NULL, NULL, NULL, &rootKeyCount, NULL, NULL, NULL, NULL, NULL, NULL, NULL); 

	if (dwRet != ERROR_SUCCESS || !rootKeyCount || !g_isActive) return;

	for (int i = 0; i < rootKeyCount; i++)
	{
		buffSize = sizeof(subKeyName)/2;

		//enumerate subkeys in root
		dwRet = RegEnumKeyEx(rootKey, i, subKeyName, &buffSize, NULL, NULL, NULL, NULL); 

		//Add stats
		InterlockedIncrement64(&g_IOPS);
		InterlockedAdd64(&g_Throughput, buffSize);

		if (dwRet != ERROR_SUCCESS) return;

		CString newkeyName = rootKeyPath + subKeyName;

		//open subkey
		dwRet = RegOpenKeyEx(hroot, newkeyName, NULL, KEY_READ, &newsubKey); 

		if (dwRet != ERROR_SUCCESS) continue;

		//how many newsubkeys in subkey
		dwRet = RegQueryInfoKey(newsubKey, NULL, NULL, NULL, &newsubKeyCount, NULL, NULL, NULL, NULL, NULL, NULL, NULL); 

		//if subkey has children, it's a new root. recurse.
		if (newsubKeyCount)
		{
			SearchReg(hroot, newkeyName);
		}

	}

}


void CReggieDlg::OnBnClickedStart()
{
	UpdateData(true);

	WorkerParams* pWP = new WorkerParams();
	pWP->TestType = 0;
	pWP->Desination = m_Desination;
	pWP->rootKey = HKEY_CURRENT_USER;

	CString ThreadCount;
	
	m_ThreadCount.GetLBText(m_intThreads, ThreadCount);
	
	switch (m_intRootKey)
	{
	case 0:
	{
		pWP->rootKey = HKEY_CURRENT_USER;
		break;
	}
	case 1:
	{
		pWP->rootKey = HKEY_LOCAL_MACHINE;
		break;

	}
	}

	int nThreadCount = _ttoi(ThreadCount);

	g_isActive = TRUE;


	switch (m_TestType == 1)
	{
	case 0:

		for (int i = 0; i < nThreadCount; i++)
		{
			AfxBeginThread(TransferWorkerThreadProc, pWP);
		}

		break;

	case 1:
				
		AfxBeginThread(EnumWorkerThreadProc, pWP);

		break;
	}


}


void CReggieDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	CString StrText;
	float LatencyMs;

	//Update Current Values
	StrText.Format(L"%d", g_IOPS);
	m_IopsEdit.SetWindowTextW(StrText);

	StrText.Format(L"%d", g_Throughput / 1024);
	m_ThroughputEdit.SetWindowTextW(StrText);

	if (g_IOPS > 0)
		LatencyMs = (1 / (float)g_IOPS) * 1000;
	else
		LatencyMs = 0.0;

	StrText.Format(L"%.6f", LatencyMs);

	m_LatencyEdit.SetWindowTextW(StrText);

	//Update Top Values
	if (g_IOPS > g_TopIops) {
		g_TopIops = g_IOPS;
		m_IopsProgress.SetRange(0, g_TopIops/10);

	}
	StrText.Format(L"Top: %d", g_TopIops);
	m_TopIops.SetWindowTextW(StrText);

	if (g_Throughput > g_TopThroughput) {
		g_TopThroughput = g_Throughput;
		m_ThroughputProgress.SetRange(0, g_TopThroughput/1024);
	}
	StrText.Format(L"Top: %d", g_TopThroughput / 1024);
	m_TopThroughput.SetWindowTextW(StrText);

	if ((LatencyMs < g_TopLatency && LatencyMs > 0) || g_TopLatency == 0)
		g_TopLatency = LatencyMs;
	StrText.Format(L"Top: %.6f", (double)g_TopLatency);
	m_TopLatency.SetWindowTextW(StrText);

	//Update graphs
	m_IopsProgress.SetPos(g_IOPS/10);
	m_ThroughputProgress.SetPos(g_Throughput/1024);


	InterlockedAnd64(&g_IOPS, 0);
	InterlockedAnd64(&g_Throughput, 0);
}


void CReggieDlg::OnBnClickedStop()
{
	g_isActive = FALSE;
}


