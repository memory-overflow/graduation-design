
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "iostream"
#include "ScreenDlg.h"

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


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent), clientdata(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CClientDlg::~CClientDlg()
{
	if(clientdata)
		delete clientdata;
	clientdata = NULL;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CClientDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CClientDlg::OnBnClickedButtonRegister)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
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
	GetDlgItem(IDC_IPADDRESS_IP) -> SetWindowTextW(_T("192.168.1.29"));
	GetDlgItem(IDC_EDIT_PORT) -> SetWindowTextW(_T("8080"));
	GetDlgItem(IDC_EDIT_USER_NAME) -> SetWindowTextW(_T("test"));
	GetDlgItem(IDC_EDIT_PASS) -> SetWindowTextW(_T("zgh"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CClientDlg::PreTranslateMessage(MSG *pMsg)
{
	if (WM_KEYFIRST <= pMsg -> message && pMsg -> message <= WM_KEYLAST)
	{
		//判断是否按下键盘Enter键
	    if(pMsg -> wParam==VK_RETURN)
	    {
			OnBnClickedButtonLogin();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	if(clientdata)
		delete clientdata;
	clientdata = new ClientData();

	CIPAddressCtrl *IpAddr = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP);
	BYTE f0,f1,f2,f3;
	IpAddr -> GetAddress(f0, f1, f2, f3);
	sprintf_s(clientdata -> ServerIp, "%d.%d.%d.%d",f0, f1, f2, f3);
	GetDlgItem(IDC_EDIT_PORT) -> GetWindowText(clientdata -> Port);
	GetDlgItem(IDC_EDIT_USER_NAME) -> GetWindowText(clientdata -> UserName);
	GetDlgItem(IDC_EDIT_PASS) -> GetWindowText(clientdata -> UserPass);
	
	std::cout << std::endl;
	std::cout << "login " << std::endl;
	std::cout << "IP: " << clientdata -> ServerIp << std::endl;
	std::wcout << "Port: " << (LPCTSTR)clientdata -> Port << std::endl;
	std::wcout << "User: " << (LPCTSTR)clientdata -> UserName << std::endl;
	std::wcout << "Pass: " << (LPCTSTR)clientdata -> UserPass << std::endl;
	std::cout << std::endl;

	int result = clientdata -> Info();
	switch (result)
	{
	case 1:
		{
			//MessageBoxExA(NULL, "端口不能为空", "error", 0, MB_OK);
			AfxMessageBox(_T("端口不能为空"));
			return ;
		}
	case 2:
		{
			//MessageBoxExA(NULL, "用户名不能为空", "error", 0, MB_OK);
			AfxMessageBox(_T("用户名不能为空"));
			return ;
		}
	case 3:
		{
			//MessageBoxExA(NULL, "密码不能为空", "error", 0, MB_OK);
			AfxMessageBox(_T("密码不能为空"));
			return ;
		}
	default:
		break;
	}
	if(!clientdata -> Login())
		return ;
	EndDialog(IDOK);
}


void CClientDlg::OnBnClickedButtonRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	if(clientdata)
		delete clientdata;

	clientdata = new ClientData();

	CIPAddressCtrl *IpAddr = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP);
	BYTE f0,f1,f2,f3;
	IpAddr -> GetAddress(f0, f1, f2, f3);
	sprintf_s(clientdata -> ServerIp, "%d.%d.%d.%d",f0, f1, f2, f3);
	GetDlgItem(IDC_EDIT_PORT) -> GetWindowText(clientdata -> Port);
	GetDlgItem(IDC_EDIT_USER_NAME) -> GetWindowText(clientdata -> UserName);
	GetDlgItem(IDC_EDIT_PASS) -> GetWindowText(clientdata -> UserPass);

	std::cout << std::endl;
	std::cout << "Regester" << std::endl;
	std::cout << "IP: " << clientdata -> ServerIp << std::endl;
	std::wcout << "Port: " << (LPCTSTR)clientdata -> Port << std::endl;
	std::wcout << "User: " << (LPCTSTR)clientdata -> UserName << std::endl;
	std::wcout << "Pass: " << (LPCTSTR)clientdata -> UserPass << std::endl;
	std::cout << std::endl;

	int result = clientdata -> Info();
	switch (result)
	{
	case 1:
		{
			//MessageBoxExA(NULL, "端口不能为空", "error", 0, MB_OK);
			AfxMessageBox(_T("端口不能为空"));
			return ;
		}
	case 2:
		{
			//MessageBoxExA(NULL, "用户名不能为空", "error", 0, MB_OK);
			AfxMessageBox(_T("用户名不能为空"));
			return ;
		}
	case 3:
		{
			//MessageBoxExA(NULL, "密码不能为空", "error", 0, MB_OK);
			AfxMessageBox(_T("密码不能为空"));
			return ;
		}
	default:
		break;
	}
	clientdata -> Register();
}

