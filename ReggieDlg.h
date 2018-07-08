
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

	static UINT CReggieDlg::WorkerThreadProc(LPVOID Param);

	CComboBox m_iotype;
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
};
