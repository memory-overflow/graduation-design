
// fuwiqiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "fuwiqi.h"
#include "fuwiqiDlg.h"
#include "afxdialogex.h"
#include "Server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CfuwiqiDlg 对话框



CfuwiqiDlg::CfuwiqiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CfuwiqiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfuwiqiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfuwiqiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CfuwiqiDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CfuwiqiDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CfuwiqiDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CfuwiqiDlg 消息处理程序

BOOL CfuwiqiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 获取本机ip显示
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	char Name[256] = {0};
	if(!gethostname(Name, 256))
	{
		hostent *hostinfo;
		if(hostinfo = gethostbyname(Name))
		{
			CString ip = (CString)inet_ntoa(*(struct in_addr*)*hostinfo -> h_addr_list);
			GetDlgItem(IDC_IPADDRESS1) -> SetWindowTextW(ip);
		}
	}
	WSACleanup();
	// 显示端口
	GetDlgItem(IDC_EDIT3) -> SetWindowTextW(CString("8080"));
	//设置button disbale
	GetDlgItem(IDC_BUTTON1) -> EnableWindow(true);
	GetDlgItem(IDC_BUTTON2) -> EnableWindow(false);
	GetDlgItem(IDC_BUTTON3) -> EnableWindow(true);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CfuwiqiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CfuwiqiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CfuwiqiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CfuwiqiDlg::OnBnClickedButton1()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("[%02d:%02d:%02d]"), time.wHour, time.wMinute, time.wSecond);
	if(Serverspace::StartServer())
	{
		GetDlgItem(IDC_BUTTON1) -> EnableWindow(false);
		GetDlgItem(IDC_BUTTON2) -> EnableWindow(true);
		UpdataEvent(CString("服务器启动成功"));
	}
	else
	{
		UpdataEvent(CString("服务器启动失败"));
	}
}


void CfuwiqiDlg::OnBnClickedButton2()
{
	 
	Serverspace::StopServer();
	GetDlgItem(IDC_BUTTON1) -> EnableWindow(true);
	GetDlgItem(IDC_BUTTON2) -> EnableWindow(false);
	((CListBox*)GetDlgItem(IDC_LIST2)) -> ResetContent();
	//UpdateWindow();
	UpdataEvent(CString("服务器停用成功"));
}



void CfuwiqiDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(Serverspace::GetServerStaues())
	{
		Serverspace::StopServer();
	}
	EndDialog(IDOK);
}

void CfuwiqiDlg::UpdataEvent(CString eve)
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("[%4d-%02d-%02d %02d:%02d:%02d]"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	CListBox *m_listbox = (CListBox*)GetDlgItem(IDC_LIST4);
	m_listbox -> AddString(strTime + eve);
	int count = m_listbox -> GetCount();
	m_listbox -> SetCurSel(count - 1);
	CString str;
	str.Format(_T("用户(%d)人"),Serverspace::GetUserCount());
	SetDlgItemText(IDC_STATIC2, str);
}