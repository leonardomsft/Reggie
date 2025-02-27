
// ReggieDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CReggieDlg dialog
class CReggieDlg : public CDialogEx
{
// Construction
public:
	CReggieDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_REGGIE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
private:

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();

	static UINT CReggieDlg::TransferWorkerThreadProc(LPVOID Param);
	static UINT CReggieDlg::EnumWorkerThreadProc(LPVOID Param);
	static void CReggieDlg::SearchReg(HKEY rootKey, CString subKeyPath);

	CComboBox m_iotype;
	CComboBox m_RootKey;
	CSliderCtrl m_iosize;
	CComboBox m_ThreadCount;
	CProgressCtrl m_IopsProgress;
	CProgressCtrl m_ThroughputProgress;
	CProgressCtrl m_LatencyProgress;
	CEdit m_iosizegauge;
	CEdit m_IopsEdit;
	CEdit m_ThroughputEdit;
	CEdit m_LatencyEdit;
	CStatic m_TopIops;
	CStatic m_TopThroughput;
	CStatic m_TopLatency;
	
	CString m_Desination;
	int m_TestType;
	int m_intThreads;
	int m_intRootKey;


};

class WorkerParams
{
public:
	CString Desination;
	int TestType;
	HKEY rootKey;
};