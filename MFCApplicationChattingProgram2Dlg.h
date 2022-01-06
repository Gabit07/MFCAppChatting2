
// MFCApplicationChattingProgram2Dlg.h : header file
//
#include "CTCPSocketcomm.h"
#include "Socket.h"

#pragma once


// CMFCApplicationChattingProgram2Dlg dialog
class CMFCApplicationChattingProgram2Dlg : public CDialogEx
{
// Construction
public:
	CMFCApplicationChattingProgram2Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATIONCHATTINGPROGRAM2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonclose();
private:
	
	CTCPSocketcomm m_server;
	CTCPSocketcomm m_client;
	CSocket m_UDPserver;
	CSocket m_UDPclient;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
