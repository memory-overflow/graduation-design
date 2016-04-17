// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ChatDlg.h"
#include "afxdialogex.h"

#include "iostream"
// CChatDlg 对话框

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CChatDlg::~CChatDlg()
{
	OnBnClickedButtonClose();
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND_CHAT, &CChatDlg::OnBnClickedButtonSendChat)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CChatDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CChatDlg 消息处理程序
void CChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else if(nID==SC_CLOSE)
	{
		OnBnClickedButtonClose();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

BOOL CChatDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);		// 设置大图标
	SetIcon(m_hIcon, TRUE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_edit_message = (CEdit*)GetDlgItem(IDC_EDIT_MESSAGE_CHAT);
	m_edit_send = (CEdit*)GetDlgItem(IDC_EDIT_SEND);
	SetWindowText(name);
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatDlg::OnPaint()
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

HCURSOR CChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChatDlg::OnCancel()
{
	onChat->erase(name);
	CDialogEx::OnCancel();
}
//取消相应键盘输入事件
BOOL CChatDlg::PreTranslateMessage(MSG *pMsg)
{
	if (WM_KEYFIRST <= pMsg -> message && pMsg -> message <= WM_KEYLAST)
	{
		//判断是否按下键盘Enter键
	    if(pMsg -> wParam==VK_RETURN)
	    {
			OnBnClickedButtonSendChat();
			return TRUE;
		}
		else if(pMsg -> hwnd == GetDlgItem(IDC_EDIT_MESSAGE_CHAT) -> m_hWnd)
		{
			return TRUE;
		}
	}
	else if(pMsg -> message == WM_RBUTTONDOWN)
	{
		std::cout << "Rbutton " << std::endl;
		if(pMsg -> hwnd == GetDlgItem(IDC_EDIT_MESSAGE_CHAT) -> m_hWnd)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CChatDlg::OnBnClickedButtonSendChat()
{
	// TODO: 在此添加控件通知处理程序代码
	std::cout << "Chat :: Send" << std::endl;
	CString message;
	m_edit_send->GetWindowTextW(message);
	m_edit_send-> SetWindowTextW(_T(""));
	if(message.IsEmpty())
		return ;
	(*this->message)[name] += CString(_T("\r\n")) + name + CString(_T(":\r\n    ")) + message;
	m_edit_message->SetWindowTextW((*this->message)[name]);
	UpdateWindow();
	m_edit_message->LineScroll(m_edit_message->GetLineCount() - 1);

	char SendBuf[1000];
	USES_CONVERSION;
	sprintf_s(SendBuf, "3+%s+%s", W2A(name), W2A(message));
	send(*conn, SendBuf, strlen(SendBuf), 0);
	
}


void CChatDlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	std::cout << "Chat :: Close" << std::endl;
	onChat->erase(name);
	DestroyWindow();
}
