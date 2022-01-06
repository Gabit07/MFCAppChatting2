
// MFCApplicationChattingProgram2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCApplicationChattingProgram2.h"
#include "MFCApplicationChattingProgram2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplicationChattingProgram2Dlg dialog



CMFCApplicationChattingProgram2Dlg::CMFCApplicationChattingProgram2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATIONCHATTINGPROGRAM2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationChattingProgram2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplicationChattingProgram2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Send, &CMFCApplicationChattingProgram2Dlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_close, &CMFCApplicationChattingProgram2Dlg::OnBnClickedButtonclose)
END_MESSAGE_MAP()


// CMFCApplicationChattingProgram2Dlg message handlers

BOOL CMFCApplicationChattingProgram2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	m_UDPclient.createUDPClientSocket(9000);
	m_UDPserver.createUDPServerSocket(9001, this);
	m_UDPserver.beginReceiving();
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplicationChattingProgram2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplicationChattingProgram2Dlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplicationChattingProgram2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplicationChattingProgram2Dlg::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_Send, str);
	m_UDPclient.send(str);

	//Packet packet;
	//packet.data = 98.5;
	//packet.packetNo = 10;
	//packet.pcktLength = sizeof(Packet);
	//m_client.sendPacket(packet);
}


void CMFCApplicationChattingProgram2Dlg::OnBnClickedButtonclose()
{
	// TODO: Add your control notification handler code here
	m_UDPserver._CloseSocket();
	m_UDPclient._CloseSocket();
	CDialogEx::OnOK();
}


LRESULT CMFCApplicationChattingProgram2Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_MY_MESSAGE)
	{
		CString* strMessage = reinterpret_cast<CString*>(wParam);
		SetDlgItemText(IDC_EDIT_Receive, *strMessage);

		/*Packet* pPacket = reinterpret_cast<Packet*>(wParam);
		Packet p = *pPacket;

		CString str;
		str.Format(_T("%f %d %d"), p.data, p.packetNo, p.length);
		SetDlgItemText(IDC_EDIT2, str);*/
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
